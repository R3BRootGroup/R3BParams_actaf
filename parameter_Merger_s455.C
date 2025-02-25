
void parameter_Merger_s455() {

  // ---------------------- Parameter Version List ------------------
  // Put here last parameter version for each detector at the end of the list

  std::vector<TString> califaCalParList{"califa/exp/califaCalibParams_V1.par",
                                        "califa/exp/califaCalibParams_V2.par"};

  std::vector<TString> califaMapParList{
      "califa/exp/califaMappingParams_V1.par"};

  std::vector<TString> califaGeoParList{"califa/exp/califaGeoParams_V1.par"};

  std::vector<TString> amsMapParList{"ams/amsMappingParams_V1.par"};

  std::vector<TString> amsCalParList{"ams/amsCalibParams_V1.par"};

  std::vector<TString> gladFieldParList{"glad/gladFieldParams_V1.par"};

  std::vector<TString> mwpc0CalParList{"mwpc/mwpc0CalibParams_V1.par"};

  std::vector<TString> mwpc1CalParList{"mwpc/mwpc1CalibParams_V1.par"};

  std::vector<TString> mwpc2CalParList{"mwpc/mwpc2CalibParams_V1.par"};

  std::vector<TString> mwpc3CalParList{"mwpc/mwpc3CalibParams_V1.par"};

  std::vector<TString> mwpc0GeoParList{"mwpc/mwpc0GeoParams_V1.par"};

  std::vector<TString> mwpc1GeoParList{"mwpc/mwpc1GeoParams_V1.par"};

  std::vector<TString> mwpc2GeoParList{"mwpc/mwpc2GeoParams_V1.par"};

  std::vector<TString> mwpc3GeoParList{"mwpc/mwpc3GeoParams_V1.par"};

  std::vector<TString> sofSciCalParList{"sofsci/sofSciCalibParams_V1.par"};

  std::vector<TString> sofTofWallCalParList{
      "softofwall/sofTofCalibParams_V1.par"};

  std::vector<TString> sofTofWallGeoParList{
      "softofwall/sofTofGeoParams_V1.par"};

  std::vector<TString> sofTofWallHitParList{
      "softofwall/sofTofHitParams_V1.par"};

  std::vector<TString> targetGeoParList{"target/targetGeoParams_V1.par"};

  std::vector<TString> trimGeoParList{"trim/trimGeoParams_V1.par"};

  std::vector<TString> trimHitParList{"trim/trimHitParams_V1.par"};

  std::vector<TString> trimCalParList{"trim/trimCalibParams_V1.par"};

  std::vector<TString> twimCalParList{"twim/twimCalibParams_V1.par"};

  std::vector<TString> twimHitParList{"twim/twimHitParams_V1.par"};

  std::vector<TString> twimGeoParList{"twim/twimGeoParams_V1.par"};

  // -------------------- Setting Parameters --------------------

  R3BCalifaCrystalCalPar *califaCalPar;
  R3BCalifaMappingPar *califaMapPar;

  R3BSofMwpc0CalPar *mwpc0CalPar;
  R3BSofMwpc1CalPar *mwpc1CalPar;
  R3BSofMwpc2CalPar *mwpc2CalPar;
  R3BSofMwpc3CalPar *mwpc3CalPar;

  R3BSofTrimCalPar *trimCalPar;
  R3BSofTrimHitPar *trimHitPar;

  R3BSofTwimCalPar *twimCalPar;
  R3BSofTwimHitPar *twimHitPar;

  R3BSofSciMapped2TcalPar *sciTCalPar;
  R3BSofSciRawTofPar *sciRawTofPar;
  R3BSofSciCalPosPar *sciCalPosPar;
  R3BSofSciCalTofPar *sciCalTofPar;

  R3BSofTofWMapped2TcalPar *tofTCalPar;
  R3BSofTofWHitPar *tofHitPar;

  R3BSofGladFieldPar *gladFieldPar;

  R3BAmsStripCalPar *amsStripCalPar;
  R3BAmsMappingPar *amsMapPar;

  R3BTGeoPar *tofGeoPar;
  R3BTGeoPar *targetGeoPar;
  R3BTGeoPar *trimGeoPar;
  R3BTGeoPar *califaGeoPar;
  R3BTGeoPar *twimGeoPar;
  R3BTGeoPar *mwpc0GeoPar;
  R3BTGeoPar *mwpc1GeoPar;
  R3BTGeoPar *mwpc2GeoPar;
  R3BTGeoPar *mwpc3GeoPar;

  FairParAsciiFileIo *inputAscii = new FairParAsciiFileIo();

  FairRuntimeDb *rtdb = FairRuntimeDb::instance();

  // ------------------- Setting Outputs ----------------------------
  // Here Run Id's do not matter. We only need versions of each parameter

  Bool_t kParameterMerged = kFALSE;
  FairParRootFileIo *parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open("./parameters/s455_expParams.root", "RECREATE");
  rtdb->setOutput(parOut);
  rtdb->addRun(1);

  for (Int_t k = 0; k < califaCalParList.size(); k++) {

    califaCalPar =
        (R3BCalifaCrystalCalPar *)rtdb->getContainer("califaCrystalCalPar");

    inputAscii->open(califaCalParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < califaMapParList.size(); k++) {

    califaMapPar =
        (R3BCalifaMappingPar *)rtdb->getContainer("califaMappingPar");

    inputAscii->open(califaMapParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < califaGeoParList.size(); k++) {

    califaGeoPar = (R3BTGeoPar *)rtdb->getContainer("CalifaGeoPar");

    inputAscii->open(califaGeoParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < amsMapParList.size(); k++) {

    amsMapPar = (R3BAmsMappingPar *)rtdb->getContainer("amsMappingPar");

    inputAscii->open(amsMapParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < amsCalParList.size(); k++) {

    amsStripCalPar = (R3BAmsStripCalPar *)rtdb->getContainer("amsStripCalPar");

    inputAscii->open(amsCalParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < gladFieldParList.size(); k++) {

    gladFieldPar = (R3BSofGladFieldPar *)rtdb->getContainer("GladFieldPar");

    inputAscii->open(gladFieldParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < mwpc0CalParList.size(); k++) {

    mwpc0CalPar = (R3BSofMwpc0CalPar *)rtdb->getContainer("mwpc0CalPar");

    inputAscii->open(mwpc0CalParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < mwpc1CalParList.size(); k++) {

    mwpc1CalPar = (R3BSofMwpc1CalPar *)rtdb->getContainer("mwpc1CalPar");

    inputAscii->open(mwpc1CalParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < mwpc2CalParList.size(); k++) {

    mwpc2CalPar = (R3BSofMwpc2CalPar *)rtdb->getContainer("mwpc2CalPar");

    inputAscii->open(mwpc2CalParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < mwpc3CalParList.size(); k++) {

    mwpc3CalPar = (R3BSofMwpc3CalPar *)rtdb->getContainer("mwpc3CalPar");

    inputAscii->open(mwpc3CalParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < mwpc0GeoParList.size(); k++) {

    mwpc0GeoPar = (R3BTGeoPar *)rtdb->getContainer("Mwpc0GeoPar");

    inputAscii->open(mwpc0GeoParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < mwpc1GeoParList.size(); k++) {

    mwpc1GeoPar = (R3BTGeoPar *)rtdb->getContainer("Mwpc1GeoPar");

    inputAscii->open(mwpc1GeoParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < mwpc2GeoParList.size(); k++) {

    mwpc2GeoPar = (R3BTGeoPar *)rtdb->getContainer("Mwpc2GeoPar");

    inputAscii->open(mwpc2GeoParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < mwpc3GeoParList.size(); k++) {

    mwpc3GeoPar = (R3BTGeoPar *)rtdb->getContainer("Mwpc3GeoPar");

    inputAscii->open(mwpc3GeoParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < sofSciCalParList.size(); k++) {

    sciTCalPar = (R3BSofSciMapped2TcalPar *)rtdb->getContainer("SofSciTcalPar");
    sciRawTofPar = (R3BSofSciRawTofPar *)rtdb->getContainer("SofSciRawTofPar");
    sciCalPosPar = (R3BSofSciCalPosPar *)rtdb->getContainer("SofSciCalPosPar");
    sciCalTofPar = (R3BSofSciCalTofPar *)rtdb->getContainer("SofSciCalTofPar");

    inputAscii->open(sofSciCalParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < sofTofWallHitParList.size(); k++) {

    tofHitPar = (R3BSofTofWHitPar *)rtdb->getContainer("tofwHitPar");

    inputAscii->open(sofTofWallHitParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < sofTofWallGeoParList.size(); k++) {

    tofGeoPar = (R3BTGeoPar *)rtdb->getContainer("TofwGeoPar");

    inputAscii->open(sofTofWallGeoParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < sofTofWallCalParList.size(); k++) {

    tofTCalPar =
        (R3BSofTofWMapped2TcalPar *)rtdb->getContainer("SofTofWTcalPar");

    inputAscii->open(sofTofWallCalParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < targetGeoParList.size(); k++) {

    targetGeoPar = (R3BTGeoPar *)rtdb->getContainer("TargetGeoPar");

    inputAscii->open(targetGeoParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < trimCalParList.size(); k++) {

    trimCalPar = (R3BSofTrimCalPar *)rtdb->getContainer("trimCalPar");

    inputAscii->open(trimCalParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < trimGeoParList.size(); k++) {

    trimGeoPar = (R3BTGeoPar *)rtdb->getContainer("TrimGeoPar");

    inputAscii->open(trimGeoParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < trimHitParList.size(); k++) {

    trimHitPar = (R3BSofTrimHitPar *)rtdb->getContainer("trimHitPar");

    inputAscii->open(trimHitParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < twimCalParList.size(); k++) {

    twimCalPar = (R3BSofTwimCalPar *)rtdb->getContainer("twimCalPar");

    inputAscii->open(twimCalParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < twimHitParList.size(); k++) {

    twimHitPar = (R3BSofTwimHitPar *)rtdb->getContainer("twimHitPar");

    inputAscii->open(twimHitParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < twimGeoParList.size(); k++) {

    twimGeoPar = (R3BTGeoPar *)rtdb->getContainer("TwimGeoPar");

    inputAscii->open(twimGeoParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }
}
