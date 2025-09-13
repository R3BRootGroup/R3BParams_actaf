/******************************************************************************
 *   Copyright (C) 2025 GSI Helmholtzzentrum für Schwerionenforschung GmbH    *
 *   Copyright (C) 2025 Members of R3B Collaboration                          *
 *                                                                            *
 *             This software is distributed under the terms of the            *
 *                 GNU General Public Licence (GPL) version 3,                *
 *                    copied verbatim in the file "LICENSE".                  *
 *                                                                            *
 * In applying this license GSI does not waive the privileges and immunities  *
 * granted to it by virtue of its status as an Intergovernmental Organization *
 * or submit itself to any jurisdiction.                                      *
 ******************************************************************************/

#include "TGeoManager.h"
#include "TMath.h"
#include <iomanip>
#include <iostream>

void create_actaf_geo(const char* geoTag = "v25")
{
    auto fRefRot = new TGeoRotation();
    TGeoManager* gGeoMan = nullptr;

    Double_t worldLength = 60.;               // 60cm^3 world box side
    Double_t chamberInnerRadius = 35.0;       // cylindrical inner radius (cm)
    Double_t chamberThickness = 0.6;          // chamber aluminum thickness (cm)
    Double_t chamberPlateRadius = 31.0;       // back stainless steel part (cm)
    Double_t chamberPlateThickness = 2.0;     // back part thickness (cm)
    Double_t chamberHalfLength = 23.5;        // half-length APPROX. (cm)
    Double_t backwardsBeamWindowRadius = 2.5; // inlet radius(cm)
    Double_t forwardsBeamWindowRadius = 3.5;  // outlet radius (cm)
    Double_t inletWindowRadius = 2.45;        // inlet berylium window radius (cm)
    Double_t inletWindowThickness = 0.05;     // inlet berylium window thickness (cm)
    Double_t inletWindowCenter = -chamberHalfLength - chamberPlateThickness - 1.0 - inletWindowRadius -
                                 inletWindowThickness; // Z coordinate of inlet window center (cm)
    Double_t outletWindowRadius = 3.45;                // inlet berylium window radius (cm)
    Double_t outletWindowThickness = 0.05;             // inlet berylium window thickness (cm)
    Double_t outletWindowCenter =
        chamberHalfLength + outletWindowRadius + outletWindowThickness; // Z coordinate of inlet window center (cm)

    Double_t anodeThickness = 0.10;                  //(cm)
    Double_t cathodeThickness = 0.10;                //(cm)
    Double_t anodeRadius = 32.4;                     // back stainless steel part (cm)
    Double_t positionAnode = -10.0;                  // anode center Z position (cm)
    Double_t positionCathode = positionAnode + 21.6; // anode center Z position (cm)
    Double_t positionGrid = positionAnode + 0.3;     // grid center Z position (cm)
    Double_t gridThickness = 0.001;                  // 55 microm, 1mm step, RECALCULATE!!! (cm)
    Double_t ringRadius = 34.0;                      // field mapping rings radius (cm)
    Double_t ringGauge = 0.1;                        // field mapping rings gauge (diameter) (cm)
    Double_t ringDistance = 1.8;                     // distance between rings (cm)

    // -------   Load media from media file   -----------------------------------
    FairGeoLoader* geoLoad = new FairGeoLoader("TGeo", "FairGeoLoader");
    FairGeoInterface* geoFace = geoLoad->getGeoInterface();
    TString geoPath = gSystem->Getenv("VMCWORKDIR");
    TString medFile = geoPath + "/geometry/media_r3b.geo";
    geoFace->setMediaFile(medFile);
    geoFace->readMedia();
    gGeoMan = gGeoManager;
    // --------------------------------------------------------------------------

    // -------   Geometry file name (output)   ----------------------------------
    TString geoFileName = geoPath + "/geometry/actaf_";
    geoFileName = geoFileName + geoTag + ".geo.root";
    // --------------------------------------------------------------------------

    // -----------------   Get and create the required media    -----------------
    FairGeoMedia* geoMedia = geoFace->getMedia();
    FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

    FairGeoMedium* mAir = geoMedia->getMedium("Air");
    if (!mAir)
        Fatal("Main", "FairMedium Air not found");
    geoBuild->createMedium(mAir);
    TGeoMedium* pAirMedium = gGeoMan->GetMedium("Air");
    if (!pAirMedium)
        Fatal("Main", "Medium Air not found");

    FairGeoMedium* mCar = geoMedia->getMedium("CarbonFibre");
    if (!mCar)
        Fatal("Main", "FairMedium CarbonFibre not found");
    geoBuild->createMedium(mCar);
    TGeoMedium* pCarbonFibreMedium = gGeoMan->GetMedium("CarbonFibre");
    if (!pCarbonFibreMedium)
        Fatal("Main", "Medium CarbonFibre not found");

    FairGeoMedium* mTfl = geoMedia->getMedium("Tefflon");
    if (!mTfl)
        Fatal("Main", "FairMedium Tefflon not found");
    geoBuild->createMedium(mTfl);
    TGeoMedium* pWrappingMedium = gGeoMan->GetMedium("Tefflon");
    if (!pWrappingMedium)
        Fatal("Main", "Medium Tefflon not found");

    FairGeoMedium* mAl = geoMedia->getMedium("aluminium");
    if (!mAl)
        Fatal("Main", "FairMedium aluminium not found");
    geoBuild->createMedium(mAl);
    TGeoMedium* pAlHousing = gGeoMan->GetMedium("aluminium");
    if (!pAlHousing)
        Fatal("Main", "Medium aluminium not found");

    FairGeoMedium* mSteel = geoMedia->getMedium("Steel");
    if (!mSteel)
        Fatal("Main", "FairMedium Steel not found");
    geoBuild->createMedium(mSteel);
    TGeoMedium* pMedSteel = gGeoMan->GetMedium("Steel");
    if (!pMedSteel)
        Fatal("Main", "Medium Steel not found");

    FairGeoMedium* mHe = geoMedia->getMedium("helium");
    if (!mHe)
        Fatal("Main", "FairMedium helium not found");
    geoBuild->createMedium(mHe);
    TGeoMedium* pMed4 = gGeoMan->GetMedium("helium");
    if (!pMed4)
        Fatal("Main", "Medium helium not found");

    FairGeoMedium* mBeryllium = geoMedia->getMedium("beryllium");
    if (!mBeryllium)
        Fatal("Main", "FairMedium beryllium not found");
    geoBuild->createMedium(mBeryllium);
    TGeoMedium* pMedBe = gGeoMan->GetMedium("beryllium");
    if (!pMedBe)
        Fatal("Main", "Medium beryllium not found");

    FairGeoMedium* mCopper = geoMedia->getMedium("copper");
    if (!mCopper)
        Fatal("Main", "FairMedium copper not found");
    geoBuild->createMedium(mCopper);
    TGeoMedium* pMedCu = gGeoMan->GetMedium("copper");
    if (!pMedCu)
        Fatal("Main", "Medium copper not found");
    // --------------------------------------------------------------------------

    // --------------   Create geometry and top volume  -------------------------
    gGeoMan = static_cast<TGeoManager*>(gROOT->FindObject("FAIRGeom"));
    gGeoMan->SetName("ACTAFgeom");
    TGeoVolume* top = new TGeoVolumeAssembly("TOP");
    gGeoMan->SetTopVolume(top);

    // --------------------------------------------------------------------------
    auto* pWorld = new TGeoVolumeAssembly("ActafWorld");

    auto* t0 = new TGeoCombiTrans();
    t0->RegisterYourself();

    // defining top volume
    top->AddNode(pWorld, 0, t0);

    double anode_length = 20.;

    std::vector<TGeoTubeSeg*> rings = { new TGeoTubeSeg("Ring1_section",
                                                        2.0,               // rmin
                                                        4.6,               // rmax
                                                        anode_length / 2., // dz (half-length in z)
                                                        0.,                // phi start
                                                        360. / 12.),       // phi end

                                        new TGeoTubeSeg("Ring2_section", 4.6, 8.5, anode_length / 2., 0., 360. / 8.),

                                        new TGeoTubeSeg("Ring3_section", 8.6, 12.5, anode_length / 2., 0., 360. / 8.),

                                        new TGeoTubeSeg("Ring4_section", 12.6, 16.5, anode_length / 2., 0., 360. / 8.),

                                        new TGeoTubeSeg("Ring5_section", 16.6, 20.5, anode_length / 2., 0., 360. / 8.),

                                        new TGeoTubeSeg("Ring6_section", 20.6, 24.5, anode_length / 2., 0., 360. / 6.),

                                        new TGeoTubeSeg("Ring7_section", 24.6, 28.5, anode_length / 2., 0., 360. / 7.),

                                        new TGeoTubeSeg(
                                            "Ring8_section", 28.6, 30.9, anode_length / 2., 0., 360. / 6.) };

    std::vector<int> nb_pads_ring = { 12, 8, 8, 8, 8, 6, 7, 6 };
    std::vector<int> offset_phi = { 0, 0, 15, 30, 0, 20, 0, 40 };

    int padval = 2;

    auto* Section = new TGeoVolumeAssembly("Section");

    for (int r = 0; r < 8; r++)
    {
        for (int s = 0; s < nb_pads_ring[r]; s++)
        {
            TString name;
            name.Form("Ring%d_Pad%d", r + 1, padval);

            auto section = new TGeoVolume(name.Data(), rings[r], pMed4);
            section->SetVisLeaves(kTRUE);
            section->SetLineColor(r + 1);

            auto rot = new TGeoRotation();
            rot->RotateZ(offset_phi[r] + s * 360. / nb_pads_ring[r]);

            Section->AddNode(section, 0, new TGeoCombiTrans(0., 0., -anode_length / 2. - 1, rot));

            padval++;
        }
    }

    padval++;
    for (int r = 0; r < 8; r++)
    {
        for (int s = 0; s < nb_pads_ring[r]; s++)
        {
            TString name;
            name.Form("Ring%d_Pad%d", r + 1, padval);

            auto section = new TGeoVolume(name.Data(), rings[r], pMed4);
            section->SetVisLeaves(kTRUE);
            section->SetLineColor(r + 1);

            auto rot = new TGeoRotation();
            rot->RotateZ(offset_phi[r] + s * 360. / nb_pads_ring[r]);

            Section->AddNode(section, 0, new TGeoCombiTrans(0., 0., anode_length / 2. + 1., rot));

            padval++;
        }
    }

    pWorld->AddNode(Section, 1, t0);

    // ---------------   Finish   -----------------------------------------------
    gGeoMan->CloseGeometry();
    gGeoMan->CheckOverlaps(0.001);
    gGeoMan->PrintOverlaps();
    gGeoMan->Test();

    TFile geoFile(geoFileName, "RECREATE");
    top->Write();
    top->Draw("ogl");
    geoFile.Close();
    std::cout << "\033[34m Creating geometry:\033[0m "
              << "\033[33m" << geoFileName << " \033[0m" << std::endl;
    std::cout << "Macro finished successfully." << std::endl;
    // gApplication->Terminate();
}
