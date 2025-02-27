/*
 *  Macro to run the online for the ACTAF detector
 *
 *  Authors: Pablo Gonzalez Rusell <pablo.grusell@udc.es>
 *           Jose Luis <j.l.rodriguez.sanchez@udc.es>
 *  @since Feb 27th, 2025
 *
 */

typedef struct EXT_STR_h101_t {
  EXT_STR_h101_unpack_t unpack;
  EXT_STR_h101_ACTAF_onion_t actaf;

} EXT_STR_h101;

void actaf_online(const Int_t fRunId = 1, const Int_t nev = -1) {
  TStopwatch timer;
  timer.Start();

  FairLogger::GetLogger()->SetLogScreenLevel("info");
  FairLogger::GetLogger()->SetColoredLog(true);

  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);
  std::ostringstream oss;
  oss << std::put_time(&tm, "%Y%m%d_%H%M%S");

  Bool_t fActaf = true;

  // NumSoiSci, file names and paths -----------------------------
  TString dir = gSystem->Getenv("VMCWORKDIR");
  TString ntuple_options = "RAW"; // For stitched data
  // TString ntuple_options = "RAW,time-stitch=1000"; // For no stitched data
  TString ucesb_dir = getenv("UCESB_DIR");
  // TString ucesb_dir = "/nucl_lustre/pablogrusell/amber_TPC_DAQ/bl/ucesb";
  TString filename, outputFilename, upexps_dir, ucesb_path;

  filename = "/nucl_lustre/pablogrusell/amber_TPC_DAQ/data/20230925_amber/"
             "stitched/eth001_0000_stitched.lmd";
  outputFilename = "unpacked_data" + oss.str() + ".root";
  upexps_dir = "/nucl_lustre/pablogrusell/amber_TPC_DAQ/bl/upexps"; // for local
                                                                    // computers
  ucesb_path =
      upexps_dir + "/2023_actar/actar --allow-errors --input-buffer=100Mi";
  ucesb_path.ReplaceAll("//", "/");

  // Online server configuration --------------------------
  const Int_t refresh = 1; // Refresh rate for online histograms
  const Int_t port = 8888; // Port number for the online visualization
  const Int_t fExpId = 2025;

  // Create online run ------------------------------------
  auto *EvntHeader = new R3BEventHeader();
  EvntHeader->SetExpId(fExpId);
  auto *run = new FairRunOnline();
  run->SetEventHeader(EvntHeader);

  run->SetSink(new FairRootFileSink(outputFilename));
  run->ActivateHttpServer(refresh, port);

  // Create source using ucesb for input ------------------
  EXT_STR_h101 ucesb_struct;

  auto *source = new R3BUcesbSource(filename, ntuple_options, ucesb_path,
                                    &ucesb_struct, sizeof(ucesb_struct));
  source->SetMaxEvents(nev);

  source->AddReader(new R3BUnpackReader(&ucesb_struct.unpack,
                                        offsetof(EXT_STR_h101, unpack)));

  if (fActaf) {
    source->AddReader(
        new R3BActafReader((EXT_STR_h101_ACTAF_onion *)&ucesb_struct.actaf,
                           offsetof(EXT_STR_h101, actaf)));
  }

  run->SetSource(source);
  run->SetRunId(fRunId);
  run->SetSink(new FairRootFileSink(outputFilename));

  // Initialize -------------------------------------------
  run->Init();

  // Run --------------------------------------------------
  run->Run((nev < 0) ? nev : 0, (nev < 0) ? 0 : nev);

  // Finish -----------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished succesfully." << std::endl;
  std::cout << "Output file is " << outputFilename << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s"
            << std::endl
            << std::endl;
}
