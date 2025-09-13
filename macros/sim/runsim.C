/** --------------------------------------------------------------------
 **
 **  Define the simulation with for experiments with ACTAF
 **  Author: <j.l.rodriguez.sanchez@udc.es>
 **
 **  Last Update: 13/09/25
 **  Comments:
 **         - 13/09/25 : Initial setup
 **
 **  Execute it as follows:
 **  root -l 'runsim.C(1000)'
 **  where 1000 means the number of events
 **
 **/

void runsim(Int_t nEvents = 1)
{
    // ----------- Configuration area ----------------------------------

    TString OutFile = "sim.root"; // Output file for data
    TString ParFile = "par.root"; // Output file for params

    Bool_t fVis = true;        // Store tracks for visualization
    Bool_t fUserPList = false; // Use of R3B special physics list

    // MonteCarlo engine: TGeant3, TGeant4, TFluka  --------------------
    TString fMC = "TGeant4";

    // Event generator type: box for particles or ascii&inclroot for p2p-fission
    TString generator1 = "box";
    TString generator2 = "ascii";
    TString generator3 = "inclroot";
    TString fGenerator = generator1;

    // ---------  Detector selection: true - false ---------------------
    bool fActaf = true;
    TString fActafGeo = "actaf_v25.geo.root";

    // ---- End of Configuration area   ---------------------------------------

    FairLogger::GetLogger()->SetLogVerbosityLevel("low");

    // ---- Stable part   -----------------------------------------------------
    TString dir = gSystem->Getenv("VMCWORKDIR");

    TString r3b_geomdir = dir + "/geometry/";
    gSystem->Setenv("GEOMPATH", r3b_geomdir.Data());
    r3b_geomdir.ReplaceAll("//", "/");

    TString r3b_confdir = dir + "/gconfig/";
    gSystem->Setenv("CONFIG_DIR", r3b_confdir.Data());
    r3b_confdir.ReplaceAll("//", "/");

    // ----    Debug option   -------------------------------------------------
    gDebug = 0;

    // -----   Timer   --------------------------------------------------------
    TStopwatch timer;
    timer.Start();

    // -----   Create simulation run   ----------------------------------------
    FairRunSim* run = new FairRunSim();
    run->SetName(fMC);                           // Transport engine
    run->SetSink(new FairRootFileSink(OutFile)); // Output file

    // -----   Runtime data base   --------------------------------------------
    auto* rtdb = run->GetRuntimeDb();

    // -----   Create media   -------------------------------------------------
    run->SetMaterials("media_r3b.geo"); // Materials

    // -----   Create R3B geometry --------------------------------------------

    // Cave definition
    FairModule* cave = new R3BCave("CAVE");
    cave->SetGeometryFileName("r3b_cave.geo");
    run->AddModule(cave);

    if (fActaf)
    {
        auto* actaf = new R3BActaf(fActafGeo);
        run->AddModule(actaf);
    }

    // -----   Create PrimaryGenerator   --------------------------------------

    // 1 - Create the Main API class for the Generator
    auto* primGen = new FairPrimaryGenerator();

    if (fGenerator.CompareTo("box") == 0)
    {
        // Define the BOX generator
        Int_t pdgId = 2212;     // proton beam
        Double32_t theta1 = 4.; // polar angle distribution
        Double32_t theta2 = 90.;
        Double32_t momentum = 0.8;
        auto* boxGen = new FairBoxGenerator(pdgId, 1);
        boxGen->SetThetaRange(theta1, theta2);
        boxGen->SetPRange(momentum, 2.0 * momentum);
        boxGen->SetPhiRange(0., 360.);
        boxGen->SetXYZ(0.0, 0.0, -10.0);
        primGen->AddGenerator(boxGen);

        // 208-Pb fragment
        auto* ionGen = new FairIonGenerator(82, 208, 82, 1, 0., 0., 1.09, 0., 0., -75.);
        // primGen->AddGenerator(ionGen);
    }

    if (fGenerator.CompareTo("ascii") == 0)
    {
        auto* gen = new R3BAsciiGenerator((dir + "/input/").Data());
        gen->SetDxDyDz(0., 0., 0.);
        primGen->AddGenerator(gen);
    }

    run->SetGenerator(primGen);

    //-------Set visualisation flag to true------------------------------------
    run->SetStoreTraj(fVis);

    // -----   Initialize simulation run   ------------------------------------
    run->Init();

    // -----   Runtime database   ---------------------------------------------
    auto* parOut = new FairParRootFileIo(true);
    parOut->open(ParFile.Data());
    rtdb->setOutput(parOut);
    rtdb->saveOutput();
    rtdb->print();

    // -----   Start run   ----------------------------------------------------
    if (nEvents > 0)
        run->Run(nEvents);

    // -----   Finish   -------------------------------------------------------
    timer.Stop();
    Double_t rtime = timer.RealTime() / 60.;
    Double_t ctime = timer.CpuTime() / 60.;
    cout << endl << endl;
    cout << "Macro finished succesfully." << endl;
    cout << "Output file is " << OutFile << endl;
    cout << "Parameter file is " << ParFile << endl;
    cout << "Real time " << rtime << " min, CPU time " << ctime << " min" << endl << endl;

    cout << " Test passed" << endl;
    cout << " All ok " << endl;
    gApplication->Terminate();
}
