//////////////////////////////////////////////////////////
// Created by L. Loiacono
// Modified by L. Fields
//////////////////////////////////////////////////////////

#ifndef eventRates_h
#define eventRates_h

// C++
#include <string>
#include <vector>
#include <fstream>
//ROOT
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class eventRates {
public :

   TChain          *fChain;   //!pointer to the analyzed TTree or TChain
   //TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain
   
   Double_t        fTotalPOT; //total pot used for all files
   std::string     ffilename; //filename for saving histograms
   std::string     detectorname; // name for det location
   double detx;     // detector location
   double dety;
   double detz;

   // Declaration of leaf types
 //LBNEDataNtp_t   *data;
   Int_t           run;
   Int_t           evtno;
   Int_t           protonN;
   Float_t         beamHWidth;
   Float_t         beamVWidth;
   Float_t         beamX;
   Float_t         beamY;
   Float_t         protonX;
   Float_t         protonY;
   Float_t         protonZ;
   Float_t         protonPx;
   Float_t         protonPy;
   Float_t         protonPz;
   Float_t         nuTarZ;
   Float_t         hornCurrent;
   Float_t         Ndxdz;
   Float_t         Ndydz;
   Float_t         Npz;
   Float_t         Nenergy;
   Float_t         NdxdzNear[5];
   Float_t         NdydzNear[5];
   Float_t         NenergyN[5];
   Double_t        NWtNear[5];
   Float_t         NdxdzFar[3];
   Float_t         NdydzFar[3];
   Float_t         NenergyF[3];
   Double_t        NWtFar[3];
   Int_t           Norig;
   Int_t           Ndecay;
   Int_t           Ntype;
   Float_t         Vx;
   Float_t         Vy;
   Float_t         Vz;
   Float_t         pdPx;
   Float_t         pdPy;
   Float_t         pdPz;
   Float_t         ppdxdz;
   Float_t         ppdydz;
   Float_t         pppz;
   Float_t         ppenergy;
   Float_t         ppmedium;
   Int_t           ptype;
   Int_t           ptrkid;
   Float_t         ppvx;
   Float_t         ppvy;
   Float_t         ppvz;
   Float_t         muparpx;
   Float_t         muparpy;
   Float_t         muparpz;
   Float_t         mupare;
   Float_t         Necm;
   Double_t        Nimpwt;
   Float_t         xpoint;
   Float_t         ypoint;
   Float_t         zpoint;
   Float_t         tvx;
   Float_t         tvy;
   Float_t         tvz;
   Float_t         tpx;
   Float_t         tpy;
   Float_t         tpz;
   Int_t           tptype;
   Int_t           tgen;
 //map<int,TrackPoint_t> fTrkPtMap;

   // List of branches
   TBranch        *b_data_run;   //!
   TBranch        *b_data_evtno;   //!
   TBranch        *b_data_protonN;   //!
   TBranch        *b_data_beamHWidth;   //!
   TBranch        *b_data_beamVWidth;   //!
   TBranch        *b_data_beamX;   //!
   TBranch        *b_data_beamY;   //!
   TBranch        *b_data_protonX;   //!
   TBranch        *b_data_protonY;   //!
   TBranch        *b_data_protonZ;   //!
   TBranch        *b_data_protonPx;   //!
   TBranch        *b_data_protonPy;   //!
   TBranch        *b_data_protonPz;   //!
   TBranch        *b_data_nuTarZ;   //!
   TBranch        *b_data_hornCurrent;   //!
   TBranch        *b_data_Ndxdz;   //!
   TBranch        *b_data_Ndydz;   //!
   TBranch        *b_data_Npz;   //!
   TBranch        *b_data_Nenergy;   //!
   TBranch        *b_data_NdxdzNear;   //!
   TBranch        *b_data_NdydzNear;   //!
   TBranch        *b_data_NenergyN;   //!
   TBranch        *b_data_NWtNear;   //!
   TBranch        *b_data_NdxdzFar;   //!
   TBranch        *b_data_NdydzFar;   //!
   TBranch        *b_data_NenergyF;   //!
   TBranch        *b_data_NWtFar;   //!
   TBranch        *b_data_Norig;   //!
   TBranch        *b_data_Ndecay;   //!
   TBranch        *b_data_Ntype;   //!
   TBranch        *b_data_Vx;   //!
   TBranch        *b_data_Vy;   //!
   TBranch        *b_data_Vz;   //!
   TBranch        *b_data_pdPx;   //!
   TBranch        *b_data_pdPy;   //!
   TBranch        *b_data_pdPz;   //!
   TBranch        *b_data_ppdxdz;   //!
   TBranch        *b_data_ppdydz;   //!
   TBranch        *b_data_pppz;   //!
   TBranch        *b_data_ppenergy;   //!
   TBranch        *b_data_ppmedium;   //!
   TBranch        *b_data_ptype;   //!
   TBranch        *b_data_ptrkid;   //!
   TBranch        *b_data_ppvx;   //!
   TBranch        *b_data_ppvy;   //!
   TBranch        *b_data_ppvz;   //!
   TBranch        *b_data_muparpx;   //!
   TBranch        *b_data_muparpy;   //!
   TBranch        *b_data_muparpz;   //!
   TBranch        *b_data_mupare;   //!
   TBranch        *b_data_Necm;   //!
   TBranch        *b_data_Nimpwt;   //!
   TBranch        *b_data_xpoint;   //!
   TBranch        *b_data_ypoint;   //!
   TBranch        *b_data_zpoint;   //!
   TBranch        *b_data_tvx;   //!
   TBranch        *b_data_tvy;   //!
   TBranch        *b_data_tvz;   //!
   TBranch        *b_data_tpx;   //!
   TBranch        *b_data_tpy;   //!
   TBranch        *b_data_tpz;   //!
   TBranch        *b_data_tptype;   //!
   TBranch        *b_data_tgen;   //!

   eventRates(TTree *tree=0);
   virtual ~eventRates();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   std::string GetPOTAsString(const double dpot);
   void SetTitles(TH1* h, 
		  const std::string &xtitle = "", 
		  const std::string &ytitle = "");
   
   double GetWeight(const std::vector<double> xdet,
			    double& nu_wght, 
			    double& nu_energy);

   double GetXSec(  double nu_type, 
		    double nu_energy,
		    std::string current);

   void ReadXSecsFromFiles(  );

 private:

    std::ifstream fdat_file[4];
    int fnbins;
    int fnlines;
    double f_e_arr[1500][4][2]; // energy bins; neutrino type; CC/NC
    double f_xsec_arr[1500][4][2]; // energy bins; neutrino type; CC/NC
   
};

#endif

#ifdef eventRates_cxx
eventRates::eventRates(TTree *tree)
{

  // Read the cross-sections from files
  ReadXSecsFromFiles();

//????????????????????????????
//????????????????????????????
//
   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   //
   //Add files here
   //
  std::vector<std::string> fFileVec;

  std::string simulation = "G4PBeam";
  //std::string simulation = "Fluka";
  std::string geometry = "CD1-CDR_GeoRHC";
  
  //detectorname = "LBNEND";
  //detx = 0.0;
  //dety = 0.0;
  //detz = 47915.0;


  //detectorname  = "LBNEFD";
  //detx = 0.0;
  //dety = 0.0;
  //detz = 129700000.0;
   
  //detectorname  = "LBNEFD_XSHIFT21";
  // detx = 2100.0;
  // dety = 0.0;
  //detz = 129700000.0; 

   //detectorname  = "LBNEFD_XSHIFT500";
   //detx = 50000.0;
   //dety = 0.0;
   //detz = 129700000.0; 

   detectorname  = "LBNEFD_XSHIFT2000";
   detx = 200000.0;
   dety = 0.0;
   detz = 129700000.0; 

  std::string physics_list = "QGSP";
  
  std::string physics_list_file_path1 = "";
  std::string physics_list_file_path2 = "";
  if(physics_list != "QGSP") {
    physics_list_file_path1 = "/"+physics_list;
    physics_list_file_path2 = "_"+physics_list;
  }

   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_001.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_002.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_003.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_004.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_005.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_006.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_007.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_008.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_009.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_010.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_011.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_012.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_013.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_014.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_015.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_016.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_017.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_018.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_019.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_020.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_021.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_022.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_023.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_024.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_025.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_026.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_027.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_028.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_029.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_030.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_031.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_032.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_033.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_034.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_035.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_036.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_037.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_038.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_039.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_040.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_041.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_042.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_043.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_044.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_045.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_046.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_047.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_048.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_049.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_050.root").c_str());

   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_051.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_052.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_053.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_054.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_055.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_056.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_057.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_058.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_059.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_060.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_061.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_062.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_063.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_064.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_065.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_066.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_067.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_068.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_069.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_070.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_071.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_072.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_073.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_074.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_075.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_076.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_077.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_078.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_079.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_080.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_081.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_082.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_083.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_084.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_085.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_086.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_087.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_088.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_089.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_090.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_091.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_092.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_093.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_094.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_095.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_096.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_097.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_098.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_099.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_100.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_101.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_102.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_103.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_104.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_105.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_106.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_107.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_108.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_109.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_110.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_111.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_112.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_113.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_114.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_115.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_116.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_117.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_118.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_119.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_120.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_121.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_122.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_123.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_124.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_125.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_126.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_127.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_128.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_129.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_130.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_131.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_132.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_133.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_134.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_135.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_136.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_137.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_138.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_139.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_140.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_141.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_142.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_143.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_144.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_145.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_146.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_147.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_148.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_149.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_150.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_151.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_152.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_153.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_154.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_155.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_156.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_157.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_158.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_159.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_160.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_161.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_162.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_163.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_164.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_165.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_166.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_167.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_168.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_169.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_170.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_171.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_172.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_173.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_174.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_175.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_176.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_177.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_178.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_179.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_180.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_181.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_182.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_183.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_184.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_185.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_186.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_187.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_188.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_189.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_190.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_191.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_192.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_193.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_194.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_195.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_196.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_197.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_198.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_199.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_200.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_201.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_202.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_203.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_204.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_205.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_206.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_207.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_208.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_209.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_210.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_211.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_212.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_213.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_214.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_215.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_216.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_217.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_218.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_219.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_220.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_221.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_222.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_223.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_224.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_225.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_226.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_227.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_228.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_229.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_230.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_231.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_232.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_233.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_234.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_235.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_236.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_237.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_238.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_239.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_240.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_241.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_242.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_243.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_244.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_245.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_246.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_247.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_248.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_249.root").c_str());
   fFileVec.push_back(("/lbne/data/users/ljf26/fluxfiles/g4lbne/"+geometry+physics_list_file_path1+"/nubeam-"+simulation+"-stdnubeam/flux/"+geometry+physics_list_file_path2+"_nubeam-"+simulation+"-stdnubeam_250.root").c_str());

   //
   //set number of pot per file !!!!!!!!!!!!!!!!!!!!!!!!!!!!
   //
   double potperfile = 100000.0;
   fTotalPOT  = potperfile*(double)fFileVec.size();

   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   //set the filename prefix for saving histogram plots 
   //
   ffilename = "g4lbne_"+geometry+"_"+physics_list;
   if(simulation=="Fluka")
     ffilename = "fluka_"+geometry+"_"+physics_list;
   
   //
   //????????????????????????????
   //????????????????????????????

   fChain = new TChain("nudata");
   for(std::vector<std::string>::const_iterator sit = fFileVec.begin(); sit != fFileVec.end(); ++sit)
   {
      fChain -> Add(sit -> c_str());
   }

   Init(fChain);


}
/*
eventRates::eventRates(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("g4lbne_example_le010z185i_NumiDPHelium_110.root");");
      if (!f) {
         f = new TFile("g4lbne_example_le010z185i_NumiDPHelium_110.root");
      }
      tree = (TTree*)gDirectory->Get("nudata");

   }
   Init(tree);
}
*/


eventRates::~eventRates()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t eventRates::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t eventRates::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void eventRates::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers

   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("run", &run, &b_data_run);
   fChain->SetBranchAddress("evtno", &evtno, &b_data_evtno);
   fChain->SetBranchAddress("protonN", &protonN, &b_data_protonN);
   fChain->SetBranchAddress("beamHWidth", &beamHWidth, &b_data_beamHWidth);
   fChain->SetBranchAddress("beamVWidth", &beamVWidth, &b_data_beamVWidth);
   fChain->SetBranchAddress("beamX", &beamX, &b_data_beamX);
   fChain->SetBranchAddress("beamY", &beamY, &b_data_beamY);
   fChain->SetBranchAddress("protonX", &protonX, &b_data_protonX);
   fChain->SetBranchAddress("protonY", &protonY, &b_data_protonY);
   fChain->SetBranchAddress("protonZ", &protonZ, &b_data_protonZ);
   fChain->SetBranchAddress("protonPx", &protonPx, &b_data_protonPx);
   fChain->SetBranchAddress("protonPy", &protonPy, &b_data_protonPy);
   fChain->SetBranchAddress("protonPz", &protonPz, &b_data_protonPz);
   fChain->SetBranchAddress("nuTarZ", &nuTarZ, &b_data_nuTarZ);
   fChain->SetBranchAddress("hornCurrent", &hornCurrent, &b_data_hornCurrent);
   fChain->SetBranchAddress("Ndxdz", &Ndxdz, &b_data_Ndxdz);
   fChain->SetBranchAddress("Ndydz", &Ndydz, &b_data_Ndydz);
   fChain->SetBranchAddress("Npz", &Npz, &b_data_Npz);
   fChain->SetBranchAddress("Nenergy", &Nenergy, &b_data_Nenergy);
   fChain->SetBranchAddress("NdxdzNear[5]", NdxdzNear, &b_data_NdxdzNear);
   fChain->SetBranchAddress("NdydzNear[5]", NdydzNear, &b_data_NdydzNear);
   fChain->SetBranchAddress("NenergyN[5]", NenergyN, &b_data_NenergyN);
   fChain->SetBranchAddress("NWtNear[5]", NWtNear, &b_data_NWtNear);
   fChain->SetBranchAddress("NdxdzFar[3]", NdxdzFar, &b_data_NdxdzFar);
   fChain->SetBranchAddress("NdydzFar[3]", NdydzFar, &b_data_NdydzFar);
   fChain->SetBranchAddress("NenergyF[3]", NenergyF, &b_data_NenergyF);
   fChain->SetBranchAddress("NWtFar[3]", NWtFar, &b_data_NWtFar);
   fChain->SetBranchAddress("Norig", &Norig, &b_data_Norig);
   fChain->SetBranchAddress("Ndecay", &Ndecay, &b_data_Ndecay);
   fChain->SetBranchAddress("Ntype", &Ntype, &b_data_Ntype);
   fChain->SetBranchAddress("Vx", &Vx, &b_data_Vx);
   fChain->SetBranchAddress("Vy", &Vy, &b_data_Vy);
   fChain->SetBranchAddress("Vz", &Vz, &b_data_Vz);
   fChain->SetBranchAddress("pdPx", &pdPx, &b_data_pdPx);
   fChain->SetBranchAddress("pdPy", &pdPy, &b_data_pdPy);
   fChain->SetBranchAddress("pdPz", &pdPz, &b_data_pdPz);
   fChain->SetBranchAddress("ppdxdz", &ppdxdz, &b_data_ppdxdz);
   fChain->SetBranchAddress("ppdydz", &ppdydz, &b_data_ppdydz);
   fChain->SetBranchAddress("pppz", &pppz, &b_data_pppz);
   fChain->SetBranchAddress("ppenergy", &ppenergy, &b_data_ppenergy);
   fChain->SetBranchAddress("ppmedium", &ppmedium, &b_data_ppmedium);
   fChain->SetBranchAddress("ptype", &ptype, &b_data_ptype);
   fChain->SetBranchAddress("ptrkid", &ptrkid, &b_data_ptrkid);
   fChain->SetBranchAddress("ppvx", &ppvx, &b_data_ppvx);
   fChain->SetBranchAddress("ppvy", &ppvy, &b_data_ppvy);
   fChain->SetBranchAddress("ppvz", &ppvz, &b_data_ppvz);
   fChain->SetBranchAddress("muparpx", &muparpx, &b_data_muparpx);
   fChain->SetBranchAddress("muparpy", &muparpy, &b_data_muparpy);
   fChain->SetBranchAddress("muparpz", &muparpz, &b_data_muparpz);
   fChain->SetBranchAddress("mupare", &mupare, &b_data_mupare);
   fChain->SetBranchAddress("Necm", &Necm, &b_data_Necm);
   fChain->SetBranchAddress("Nimpwt", &Nimpwt, &b_data_Nimpwt);
   fChain->SetBranchAddress("xpoint", &xpoint, &b_data_xpoint);
   fChain->SetBranchAddress("ypoint", &ypoint, &b_data_ypoint);
   fChain->SetBranchAddress("zpoint", &zpoint, &b_data_zpoint);
   fChain->SetBranchAddress("tvx", &tvx, &b_data_tvx);
   fChain->SetBranchAddress("tvy", &tvy, &b_data_tvy);
   fChain->SetBranchAddress("tvz", &tvz, &b_data_tvz);
   fChain->SetBranchAddress("tpx", &tpx, &b_data_tpx);
   fChain->SetBranchAddress("tpy", &tpy, &b_data_tpy);
   fChain->SetBranchAddress("tpz", &tpz, &b_data_tpz);
   fChain->SetBranchAddress("tptype", &tptype, &b_data_tptype);
   fChain->SetBranchAddress("tgen", &tgen, &b_data_tgen);
   Notify();
}

Bool_t eventRates::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void eventRates::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t eventRates::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef eventRates_cxx
