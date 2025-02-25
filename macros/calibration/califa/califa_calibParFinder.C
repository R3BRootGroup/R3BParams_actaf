/* Information:
 *
 * Before executing the macro, set up the paths to lmd files, upexps and
 * califa mapping file "califamapfilename". Also set up the name of output
 * file "outputCalFile"
 *
 * Execute it as:
 * root -l califa_calibParFinder.C
 *
 * @since July 13th, 2021
 */

typedef struct EXT_STR_h101_t {
  EXT_STR_h101_unpack_t unpack;
  EXT_STR_h101_CALIFA_t califa;
} EXT_STR_h101;

void califa_calibParFinder() {
  TStopwatch timer;
  timer.Start();

  const Int_t nev = -1; // number of events to read, -1 - until CTRL+C

  // Create source using ucesb for input ------------------
  TString filename = "~/lmd-path/main_000*.lmd";
  TString outputFileName = "OutputFile.root";

  // Create source using ucesb for input ------------------
  // UCESB paths
  TString ntuple_options = "RAW";
  TString ucesb_dir = getenv("UCESB_DIR");
  TString upexps_dir = "/u/land/fake_cvmfs/9.13/upexps/202104_s455/";

  TString ucesb_path;
  ucesb_path = upexps_dir + "/202103_s455 --allow-errors --input-buffer=70Mi";
  ucesb_path.ReplaceAll("//", "/");

  // Parameters for CALIFA
  TString dir = gSystem->Getenv("VMCWORKDIR");
  TString califamapdir = dir + "/path-to-parameter-folder/";
  TString califamapfilename = "/Califa_mapping.par";
  califamapfilename.ReplaceAll("//", "/");

  // CALIFA output file with the parameters calibrated in keV
  TString outputCalFile = "Califa_CalPar.par";

  // Create source using ucesb for input ------------------
  EXT_STR_h101 ucesb_struct;

  // Create online run ------------------------------------
  FairRunOnline *run = new FairRunOnline();
  R3BEventHeader *EvntHeader = new R3BEventHeader();
  run->SetEventHeader(EvntHeader);
  run->SetRunId(1);
  run->SetSink(new FairRootFileSink(outputFileName));

  R3BUcesbSource *source =
      new R3BUcesbSource(filename, ntuple_options, ucesb_path, &ucesb_struct,
                         sizeof(ucesb_struct));
  source->SetMaxEvents(nev);

  source->AddReader(new R3BUnpackReader((EXT_STR_h101_unpack *)&ucesb_struct,
                                        offsetof(EXT_STR_h101, unpack)));
  source->AddReader(
      new R3BCalifaFebexReader((EXT_STR_h101_CALIFA *)&ucesb_struct.califa,
                               offsetof(EXT_STR_h101, califa)));
  run->SetSource(source);

  // Runtime data base ------------------------------------
  FairRuntimeDb *rtdb = run->GetRuntimeDb();
  // CALIFA mapping
  FairParAsciiFileIo *parIo1 = new FairParAsciiFileIo(); // Ascii
  parIo1->open(califamapfilename, "in");
  rtdb->setFirstInput(parIo1);
  rtdb->print();

  // Add analysis task ------------------------------------

  // R3BCalifaMapped2CrystalCalPar ----
  TArrayF *EnergythePeaks = new TArrayF();
  // e1 must be always the highest energy
  Float_t e1 = 1332.5;
  Float_t e2 = 1173.2;
  EnergythePeaks->Set(2);
  EnergythePeaks->AddAt(e1, 0);
  EnergythePeaks->AddAt(e2, 1);

  R3BCalifaMapped2CrystalCalPar *CalPar = new R3BCalifaMapped2CrystalCalPar();
  CalPar->SetMinStadistics(1000);
  CalPar->SetNumParameterFit(2); // OPTIONAL by default 2
  // Gamma range
  CalPar->SetCalRange_left(750);
  CalPar->SetCalRange_right(1300);
  CalPar->SetCalRange_bins(130);
  // particle range
  CalPar->SetCalRangeP_left(80);
  CalPar->SetCalRangeP_right(130);
  CalPar->SetCalRangeP_bins(100);
  CalPar->SetSigma(3.0);
  CalPar->SetThreshold(0.0001);
  CalPar->SetEnergyPeaks(EnergythePeaks);
  CalPar->SetDebugMode(1);
  run->AddTask(CalPar);

  // Initialize -------------------------------------------
  run->Init();
  //    FairLogger::GetLogger()->SetLogScreenLevel("WARNING");
  //    FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");

  // Choose Root or Ascii file
  // 1-Root file with the Calibration Parameters
  /*Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo *parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(outputCalFile);
  rtdb->setOutput(parOut);*/
  // 2-Ascii file with the Calibration Parameters
  FairParAsciiFileIo *parOut = new FairParAsciiFileIo();
  parOut->open(outputCalFile, "out");
  rtdb->setOutput(parOut);

  // Run --------------------------------------------------
  run->Run((nev < 0) ? nev : 0, (nev < 0) ? 0 : nev);

  // Save parameters (if needed) --------------------------
  rtdb->saveOutput();

  // Finish -----------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime() / 60.;
  Double_t ctime = timer.CpuTime() / 60.;
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished succesfully." << std::endl;
  std::cout << "Output file is " << outputFileName << std::endl;
  std::cout << "Real time " << rtime << " min, CPU time " << ctime << " min"
            << std::endl
            << std::endl;
  gApplication->Terminate();
}
