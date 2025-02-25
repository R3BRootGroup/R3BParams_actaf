//
//   ----- General Macro to create the geometry of R3B-MUSIC detector
//
//         Author: Jose Luis <joseluis.rodriguez.sanchez@usc.es>
//
//         Last Update: 06/10/19 (Jose Luis)
//
//         Comments: Dimensions provided by Bernd Voss on 4th Oct 2019
//

#include "TGeoManager.h"
#include "TMath.h"
#include <iomanip>
#include <iostream>

// Create Matrix Unity
TGeoRotation *fGlobalRot = new TGeoRotation();

// Create a null translation
TGeoTranslation *fGlobalTrans = new TGeoTranslation();
TGeoRotation *fRefRot = NULL;

TGeoManager *gGeoMan = NULL;

Double_t fThetaX = 0.;
Double_t fThetaY = 0.;
Double_t fThetaZ = 0.;
Double_t fPhi = 0.;
Double_t fTheta = 0.;
Double_t fPsi = 0.;
Double_t fX = 0.;
Double_t fY = 0.;
Double_t fZ = 0.;
Bool_t fLocalTrans = kFALSE;

TGeoCombiTrans *GetGlobalPosition(TGeoCombiTrans *fRef);

void create_music_geo(const char *geoTag = "s467") {
  // --------------------------------------------------------------------------
  // Configurable geometry for the target vacuum chamber according to the
  // e-mail on 4th October 2019.
  // Use this macro to create root files with the different configurations
  // and positions of the vacuum chamber.
  //
  // Execute macro:  root -l
  //                 .L create_music_geo.C
  //                 create_music_geo()
  // --------------------------------------------------------------------------

  fGlobalTrans->SetTranslation(0.0, 0.0, 0.0);

  // -------   Load media from media file   -----------------------------------
  FairGeoLoader *geoLoad = new FairGeoLoader("TGeo", "FairGeoLoader");
  FairGeoInterface *geoFace = geoLoad->getGeoInterface();
  TString geoPath = gSystem->Getenv("VMCWORKDIR");
  TString medFile = geoPath + "/geometry/media_r3b.geo";
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  gGeoMan = gGeoManager;
  // --------------------------------------------------------------------------

  // -------   Geometry file name (output)   ----------------------------------
  TString geoFileName = "../../trim/music_";
  geoFileName = geoFileName + geoTag + ".geo.root";
  // --------------------------------------------------------------------------

  // -----------------   Get and create the required media    -----------------
  FairGeoMedia *geoMedia = geoFace->getMedia();
  FairGeoBuilder *geoBuild = geoLoad->getGeoBuilder();

  FairGeoMedium *mCu = geoMedia->getMedium("copper");
  if (!mCu)
    Fatal("Main", "FairMedium copper not found");
  geoBuild->createMedium(mCu);
  TGeoMedium *pMed1 = gGeoMan->GetMedium("copper");
  if (!pMed1)
    Fatal("Main", "Medium copper not found");

  FairGeoMedium *mAl = geoMedia->getMedium("aluminium");
  if (!mAl)
    Fatal("Main", "FairMedium aluminium not found");
  geoBuild->createMedium(mAl);
  TGeoMedium *pMed2 = gGeoMan->GetMedium("aluminium");
  if (!pMed2)
    Fatal("Main", "Medium aluminium not found");

  FairGeoMedium *mSteel = geoMedia->getMedium("Steel");
  if (!mSteel)
    Fatal("Main", "FairMedium Steel not found");
  geoBuild->createMedium(mSteel);
  TGeoMedium *pMed3 = gGeoMan->GetMedium("Steel");
  if (!pMed3)
    Fatal("Main", "Medium Steel not found");

  FairGeoMedium *mMylar = geoMedia->getMedium("mylar");
  if (!mMylar)
    Fatal("Main", "FairMedium mylar not found");
  geoBuild->createMedium(mMylar);
  TGeoMedium *pMed4 = gGeoMan->GetMedium("mylar");
  if (!pMed4)
    Fatal("Main", "Medium mylar not found");

  FairGeoMedium *mAr = geoMedia->getMedium("ArCO2");
  if (!mAr)
    Fatal("Main", "FairMedium ArCO2 not found");
  geoBuild->createMedium(mAr);
  TGeoMedium *pMed5 = gGeoMan->GetMedium("ArCO2");
  if (!pMed5)
    Fatal("Main", "Medium ArCO2 not found");
  // --------------------------------------------------------------------------

  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager *)gROOT->FindObject("FAIRGeom");
  gGeoMan->SetName("MUSICgeom");
  TGeoVolume *top = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(top);
  // --------------------------------------------------------------------------

  // Defintion of the Mother Volume
  TGeoShape *pbox1 =
      new TGeoBBox("musicbox1", 40.3 / 2.0, 40.3 / 2.0, 50. / 2.0);
  TGeoCombiTrans *t_box1 = new TGeoCombiTrans("t_box1", 0., 0., 0., fRefRot);
  t_box1->RegisterYourself();

  TGeoShape *pbox2 = new TGeoBBox("musicbox2", 20 / 2.0, 20. / 2.0, 2.5 / 4.0);
  TGeoCombiTrans *t_box2 =
      new TGeoCombiTrans("t_box2", 0., 0., -40.3 / 2 - 2.5 / 4.0, fRefRot);
  t_box2->RegisterYourself();

  TGeoCompositeShape *pMWorld =
      new TGeoCompositeShape("MUSICbox", "musicbox1:t_box1 + musicbox2:t_box2");

  TGeoVolume *pWorld = new TGeoVolume("MUSICWorld", pMWorld, pMed5);

  TGeoCombiTrans *t0 = new TGeoCombiTrans();
  TGeoCombiTrans *pGlobalc = GetGlobalPosition(t0);

  // Adding the Mother Volume
  top->AddNode(pWorld, 0, pGlobalc);

  Double_t dx, dy, dz;
  Double_t thx, thy, thz;
  Double_t phx, phy, phz;

  Int_t nbanodes = 8;

  TGeoVolumeAssembly *MusicDet = new TGeoVolumeAssembly("MusicDet");

  // Anode Shape & volume: TGeoBBox
  dx = 20.0;
  dy = 20.0;
  dz = 5.00;
  TGeoVolume *Anode =
      gGeoManager->MakeBox("Anode", pMed5, dx / 2., dy / 2., dz / 2.);
  Anode->SetVisLeaves(kTRUE);
  Anode->SetLineColor(2);

  for (Int_t i = 0; i < nbanodes; i++) {
    dz = -20.0 + 2.5 + 5. * i;
    dy = 0.000;
    dx = 0.000;
    TGeoCombiTrans *pMatrix = new TGeoCombiTrans("", dx, dy, dz, fRefRot);
    MusicDet->AddNode(Anode, i, pMatrix);
  }

  dx = 0.000;
  dy = 0.000;
  dz = 0.000;
  TGeoRotation *arot0 = new TGeoRotation();
  TGeoCombiTrans *pMatrixd0 = new TGeoCombiTrans("", dx, dy, dz, arot0);
  pWorld->AddNode(MusicDet, 0, pMatrixd0);

  // Screening anodes, Shape & volume: TGeoBBox
  dx = 20.0;
  dy = 20.0;
  dz = 2.00;
  TGeoVolume *SAnode =
      gGeoManager->MakeBox("Screening", pMed5, dx / 2., dy / 2., dz / 2.);
  SAnode->SetVisLeaves(kTRUE);
  SAnode->SetLineColor(5);

  dx = 0.000;
  dy = 0.000;
  dz = -21.0;
  TGeoRotation *arot1 = new TGeoRotation();
  TGeoCombiTrans *pMatrixd1 = new TGeoCombiTrans("", dx, dy, dz, arot1);
  pWorld->AddNode(SAnode, 0, pMatrixd1);

  dz = 21.0;
  TGeoRotation *arot2 = new TGeoRotation();
  TGeoCombiTrans *pMatrixd2 = new TGeoCombiTrans("", dx, dy, dz, arot2);
  pWorld->AddNode(SAnode, 1, pMatrixd2);

  // Windows, Shape & volume: TGeoBBox
  dx = 20.0;
  dy = 20.0;
  dz = 0.0025;
  TGeoVolume *Wind =
      gGeoManager->MakeBox("Window", pMed4, dx / 2., dy / 2., dz / 2.);
  Wind->SetVisLeaves(kTRUE);

  dx = 0.000;
  dy = 0.000;
  dz = -22.0 - 0.0025 / 2.;
  TGeoRotation *arot3 = new TGeoRotation();
  TGeoCombiTrans *pMatrixd3 = new TGeoCombiTrans("", dx, dy, dz, arot3);
  pWorld->AddNode(Wind, 0, pMatrixd3);

  dz = 22.0 + 0.0025 / 2.;
  TGeoRotation *arot4 = new TGeoRotation();
  TGeoCombiTrans *pMatrixd4 = new TGeoCombiTrans("", dx, dy, dz, arot4);
  pWorld->AddNode(Wind, 1, pMatrixd4);

  // ---------------   Finish   -----------------------------------------------
  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.001);
  gGeoMan->PrintOverlaps();
  gGeoMan->Test();

  TFile *geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();

  std::cout << "Creating geometry: " << geoFileName << std::endl;

  // --------------------------------------------------------------------------
}

TGeoCombiTrans *GetGlobalPosition(TGeoCombiTrans *fRef) {
  if (fLocalTrans == kTRUE) {

    if ((fThetaX == 0) && (fThetaY == 0) && (fThetaZ == 0) && (fX == 0) &&
        (fY == 0) && (fZ == 0))
      return fRef;

    // X axis
    Double_t xAxis[3] = {1., 0., 0.};
    Double_t yAxis[3] = {0., 1., 0.};
    Double_t zAxis[3] = {0., 0., 1.};
    // Reference Rotation
    fRefRot = fRef->GetRotation();

    if (fRefRot) {
      Double_t mX[3] = {0., 0., 0.};
      Double_t mY[3] = {0., 0., 0.};
      Double_t mZ[3] = {0., 0., 0.};

      fRefRot->LocalToMasterVect(xAxis, mX);
      fRefRot->LocalToMasterVect(yAxis, mY);
      fRefRot->LocalToMasterVect(zAxis, mZ);

      Double_t a[4] = {mX[0], mX[1], mX[2], fThetaX};
      Double_t b[4] = {mY[0], mY[1], mY[2], fThetaY};
      Double_t c[4] = {mZ[0], mZ[1], mZ[2], fThetaZ};

      ROOT::Math::AxisAngle aX(a, a + 4);
      ROOT::Math::AxisAngle aY(b, b + 4);
      ROOT::Math::AxisAngle aZ(c, c + 4);

      ROOT::Math::Rotation3D fMatX(aX);
      ROOT::Math::Rotation3D fMatY(aY);
      ROOT::Math::Rotation3D fMatZ(aZ);

      ROOT::Math::Rotation3D fRotXYZ = (fMatZ * (fMatY * fMatX));

      // cout << fRotXYZ << endl;

      Double_t fRotable[9] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
      fRotXYZ.GetComponents(fRotable[0], fRotable[3], fRotable[6], fRotable[1],
                            fRotable[4], fRotable[7], fRotable[2], fRotable[5],
                            fRotable[8]);
      TGeoRotation *pRot = new TGeoRotation();
      pRot->SetMatrix(fRotable);
      TGeoCombiTrans *pTmp = new TGeoCombiTrans(*fGlobalTrans, *pRot);

      // ne peut pas etre applique ici
      // il faut differencier trans et rot dans la multi.
      TGeoRotation rot_id;
      rot_id.SetAngles(0.0, 0.0, 0.0);

      TGeoCombiTrans c1;
      c1.SetRotation(rot_id);
      const Double_t *t = pTmp->GetTranslation();
      c1.SetTranslation(t[0], t[1], t[2]);

      TGeoCombiTrans c2;
      c2.SetRotation(rot_id);
      const Double_t *tt = fRefRot->GetTranslation();
      c2.SetTranslation(tt[0], tt[1], tt[2]);

      TGeoCombiTrans cc = c1 * c2;

      TGeoCombiTrans c3;
      c3.SetRotation(pTmp->GetRotation());
      TGeoCombiTrans c4;
      c4.SetRotation(fRefRot);

      TGeoCombiTrans ccc = c3 * c4;

      TGeoCombiTrans pGlobal;
      pGlobal.SetRotation(ccc.GetRotation());
      const Double_t *allt = cc.GetTranslation();
      pGlobal.SetTranslation(allt[0], allt[1], allt[2]);

      return (new TGeoCombiTrans(pGlobal));

    } else {

      cout << "-E- R3BDetector::GetGlobalPosition() \
	      No. Ref. Transformation defined ! "
           << endl;
      cout << "-E- R3BDetector::GetGlobalPosition() \
	      cannot create Local Transformation "
           << endl;
      return NULL;
    } //! fRefRot

  } else {
    // Lab Transf.
    if ((fPhi == 0) && (fTheta == 0) && (fPsi == 0) && (fX == 0) && (fY == 0) &&
        (fZ == 0))
      return fRef;

    return (new TGeoCombiTrans(*fGlobalTrans, *fGlobalRot));
  }
}
