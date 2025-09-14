/*
 *  Macro to run the online for the ACTAF detector
 *
 *  Authors: Pablo Gonzalez Rusell <pablo.grusell@udc.es>
 *           Jose Luis <j.l.rodriguez.sanchez@udc.es>
 *  @since Feb 27th, 2025
 *
 */

typedef struct EXT_STR_h101_t
{
    EXT_STR_h101_unpack_t unpack;
    // EXT_STR_h101_ACTAF2023_onion_t actaf;
    EXT_STR_h101_ACTAF2025_onion_t actaf;
} EXT_STR_h101;

void actaf_online(TString filename = "/nucl_lustre/amber/lmd_2025/run0002_0000_st.lmd",
                  const Int_t fRunId = 1,
                  const Int_t nev = -1)
{
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
    TString pardir = gSystem->Getenv("PARAMDIR");
    TString ntuple_options = "RAW"; // For stitched data
    // TString ntuple_options = "RAW,time-stitch=1000"; // For no stitched data
    TString outputFilename, ucesb_path;

    // Path to the output root file  ---------------------------
    const TString output_path = gSystem->Getenv("OUTROOTPATH");
    if (output_path == "")
    {
        std::cout
            << "OUTROOTPATH is not set, this defines the path for output ROOT files, example: export OUTROOTPATH=./"
            << std::endl;
        gApplication->Terminate();
    }
    std::cout << "Output Root file path: " << output_path << std::endl;

    outputFilename = output_path + "/unpacked_data" + oss.str() + ".root";
    outputFilename.ReplaceAll("//", "/");

    // Path to UPEXPS   ---------------------------------------
    const TString upexps_dir = gSystem->Getenv("UPEXPS_DIR");
    if (upexps_dir == "")
    {
        std::cout << "UPEXPS_DIR is not set, load the configuration file on your PC > source conf.sh" << std::endl;
        gApplication->Terminate();
    }
    std::cout << "UPEXPS_DIR = " << upexps_dir << std::endl;

    ucesb_path = upexps_dir + "/actaf --allow-errors --input-buffer=100Mi";
    ucesb_path.ReplaceAll("//", "/");

    // Online server configuration --------------------------
    const Int_t refresh = 1; // Refresh rate for online histograms
    const Int_t port = 8888; // Port number for the online visualization
    const Int_t fExpId = 2025;

    // Create online run ------------------------------------
    auto* EvntHeader = new R3BEventHeader();
    EvntHeader->SetExpId(fExpId);
    auto* run = new FairRunOnline();
    run->SetEventHeader(EvntHeader);
    run->ActivateHttpServer(refresh, port);

    // Create source using ucesb for input ------------------
    EXT_STR_h101 ucesb_struct;

    auto* source = new R3BUcesbSource(filename, ntuple_options, ucesb_path, &ucesb_struct, sizeof(ucesb_struct));
    source->SetMaxEvents(nev);

    source->AddReader(new R3BUnpackReader(&ucesb_struct.unpack, offsetof(EXT_STR_h101, unpack)));

    if (fActaf)
    {
        // auto actafreader =
        //     new R3BActafReader((EXT_STR_h101_ACTAF2023_onion
        //     *)&ucesb_struct.actaf,
        //                        offsetof(EXT_STR_h101, actaf));

        auto actafreader =
            new R3BActafReader((EXT_STR_h101_ACTAF2025_onion*)&ucesb_struct.actaf, offsetof(EXT_STR_h101, actaf));
        actafreader->SetOnline();
        source->AddReader(actafreader);
    }

    run->SetSource(source);
    run->SetRunId(fRunId);
    run->SetSink(new FairRootFileSink(outputFilename));

    // Runtime data base ------------------------------------
    auto* rtdb = run->GetRuntimeDb();
    auto* parRoot = new FairParRootFileIo(true);
    auto* parAscii = new FairParAsciiFileIo();

    auto* parListRoot = new TList();
    // parListRoot->Add(new TObjString(pardir + "/actaf/actaf_cal_v1.root"));
    parRoot->open(parListRoot);
    rtdb->setFirstInput(parRoot);

    auto* parListAscii = new TList();
    parListAscii->Add(new TObjString(pardir + "/actaf/actaf_mapping_v2.par"));
    parAscii->open(parListAscii);
    rtdb->setSecondInput(parAscii);
    rtdb->print();

    // Create analysis task ------------------------------------------------------
    auto* actafmap2cal = new R3BActafMapped2Cal();
    // actafmap2cal->SetTimeConversion(51.44); // ns/bin
    // actafmap2cal->SetVelocity(0.28); // cm/ns
    actafmap2cal->SetOnline();
    run->AddTask(actafmap2cal);

    auto* actafcal2hit = new R3BActafCal2Hit();
    actafcal2hit->SetOnline();
    run->AddTask(actafcal2hit);

    auto* actafonline = new R3BActafOnlineSpectra();
    run->AddTask(actafonline);

    // Initialize -------------------------------------------
    run->Init();

    // Run --------------------------------------------------
    run->Run((nev < 0) ? nev : 0, (nev < 0) ? 0 : nev);

    // Finish -----------------------------------------------
    timer.Stop();
    Double_t rtime = timer.RealTime() / 60.;
    Double_t ctime = timer.CpuTime() / 60.;
    std::cout << std::endl << std::endl;
    std::cout << "Macro finished succesfully." << std::endl;
    std::cout << "Output file is " << outputFilename << std::endl;
    std::cout << "Real time " << rtime << " min, CPU time " << ctime << " min" << std::endl << std::endl;
}
