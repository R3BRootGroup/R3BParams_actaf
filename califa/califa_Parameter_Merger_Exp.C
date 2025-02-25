
void califa_Parameter_Merger_Exp() {

  // PLACE HERE YOUR PARAMETER VERSIONS
  std::vector<TString> calParList
  {"califaCalibParams_V1.par"
  ,"califaCalibParams_V2.par"};

  std::vector<TString> mapParList
  {"califaMappingParams_V1.par"};


  UInt_t fDummyRunId=1; // In the root file we cannot assign RunId's, as the same parameter version can be used by different RunId's.

  R3BCalifaCrystalCalPar *calPars;
  R3BCalifaMappingPar *mapPars;

  FairParAsciiFileIo* inputAscii = new FairParAsciiFileIo();

  FairRuntimeDb* rtdb = FairRuntimeDb::instance();

  calPars = (R3BCalifaCrystalCalPar*)rtdb->getContainer("califaCrystalCalPar");
  mapPars = (R3BCalifaMappingPar*)rtdb->getContainer("califaMappingPar");

  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open("califaExpParams.root");
  rtdb->setOutput(parOut);


  for (Int_t k =0; k<calParList.size();k++) {

   rtdb->addRun(k+1);
   rtdb->setInputVersion(fDummyRunId,(char*)"califaCrystalCalPar",1,k+1);

   inputAscii->open(calParList.at(k),"in");
   rtdb->setFirstInput(inputAscii);
   rtdb->initContainers(fDummyRunId);
   rtdb->saveOutput();

  }


  for (Int_t k =0; k<mapParList.size();k++) {

   rtdb->addRun(k+1);
   rtdb->setInputVersion(fDummyRunId,(char*)"califaMappingPar",1,k+1);

   inputAscii->open(mapParList.at(k),"in");
   rtdb->setFirstInput(inputAscii);
   rtdb->initContainers(fDummyRunId);
   rtdb->saveOutput();

  }


}
