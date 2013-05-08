#define eventRates_cxx
#include "eventRates.h"
#include <TH2.h>
#include <TH1D.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <math.h>
#include <TGaxis.h>

using namespace std;

int iread = 0;

void eventRates::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L eventRates.C++  // The ++ is important because the code runs MUCH faster when compiled
//      Root > eventRates t
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;


//????????????????????????????
//????????????????????????????
//
   //
   //define the Reference pot !!!!!!!!!!!!!!!!
   //
   double refpot          = 1;
   std::string potref_str = eventRates::GetPOTAsString(refpot);

   //
   //make histograms 
   //
   // simple binning for some alignment plots
   int nbins   = 40;
   double xmin = 0.0;
   double xmax = 20.0;

   // more complex binning for passing to fastMC
   std::vector< Double_t > fastmc_bins;
   // 0.125 GeV bins up to 8 GeV
   for(int i = 0; i<8/0.125; i++)
     fastmc_bins.push_back(i*.125);
   // 0.5 GeV bins up to 20 GeV
   for(int i = 0; i<(20-8)/0.5; i++)
     fastmc_bins.push_back(8.0+i*.5);
  // 2.0 GeV bins up to 120 GeV
   for(int i = 0; i<(120-20)/2.0; i++)
     fastmc_bins.push_back(20.0+i*2.0);

   TH1D *fhNuMuFlux    = new TH1D("numu_flux_forplots",  
				  "numu_flux_forplots", nbins,xmin,xmax);
   TH1D *fhNuMuBarFlux = new TH1D("numubar_flux_forplots", 
				  "numubar_flux_forplots", nbins/2,xmin,xmax);
   TH1D *fhNuEFlux     = new TH1D("nue_flux_forplots",
				  "nue_flux_forplots", nbins/2,xmin,xmax);
   TH1D *fhNuEBarFlux  = new TH1D("nuebar_flux_forplots",
				  "nuebar_flux_forplots", nbins/2,xmin,xmax);
 
   TH1D *fhNuTauFlux     = new TH1D("nutau_flux_forplots",
				    "nutau_flux_forplots", nbins/2,xmin,xmax);
   TH1D *fhNuTauBarFlux  = new TH1D("nutaubar_flux_forplots",
				    "nutaubar_flux_forplots", nbins/2,xmin,xmax);
   
   TH1D *fhNuMuCCEventRate    = new TH1D("numu_cceventrate_forplots",  
				  "numu_cceventrate_forplots", nbins,xmin,xmax);
   TH1D *fhNuMuBarCCEventRate = new TH1D("numubar_cceventrate_forplots", 
				  "numubar_cceventrate_forplots", nbins/2,xmin,xmax);
   TH1D *fhNuECCEventRate     = new TH1D("nue_cceventrate_forplots",
				  "nue_cceventrate_forplots", nbins/2,xmin,xmax);
   TH1D *fhNuEBarCCEventRate  = new TH1D("nuebar_cceventrate_forplots",
				  "nuebar_cceventrate_forplots", nbins/2,xmin,xmax);
 
   TH1D *fhNuTauCCEventRate     = new TH1D("nutau_cceventrate_forplots",
				    "nutau_cceventrate_forplots", nbins/2,xmin,xmax);
   TH1D *fhNuTauBarCCEventRate  = new TH1D("nutaubar_cceventrate_forplots",
				    "nutaubar_cceventrate_forplots", nbins/2,xmin,xmax);

   TH1D *fhNuMuNCEventRate    = new TH1D("numu_nceventrate_forplots",  
				  "numu_nceventrate_forplots", nbins,xmin,xmax);
   TH1D *fhNuMuBarNCEventRate = new TH1D("numubar_nceventrate_forplots", 
				  "numubar_nceventrate_forplots", nbins/2,xmin,xmax);
   TH1D *fhNuENCEventRate     = new TH1D("nue_nceventrate_forplots",
				  "nue_nceventrate_forplots", nbins/2,xmin,xmax);
   TH1D *fhNuEBarNCEventRate  = new TH1D("nuebar_nceventrate_forplots",
				  "nuebar_nceventrate_forplots", nbins/2,xmin,xmax);
 
   TH1D *fhNuTauNCEventRate     = new TH1D("nutau_nceventrate_forplots",
				    "nutau_nceventrate_forplots", nbins/2,xmin,xmax);
   TH1D *fhNuTauBarNCEventRate  = new TH1D("nutaubar_nceventrate_forplots",
				    "nutaubar_nceventrate_forplots", nbins/2,xmin,xmax);

   TH1D *fhNuMuFlux_FastMC    = new TH1D("numu_flux",  
					 "numu_flux",fastmc_bins.size()-1,&fastmc_bins[0]);
   TH1D *fhNuMuBarFlux_FastMC = new TH1D("numubar_flux", 
					 "numubar_flux", fastmc_bins.size()-1,&fastmc_bins[0]);
   TH1D *fhNuEFlux_FastMC     = new TH1D("nue_flux",
					 "nue_flux", fastmc_bins.size()-1,&fastmc_bins[0]);
   TH1D *fhNuEBarFlux_FastMC  = new TH1D("nuebar_flux",
					 "nuebar_flux", fastmc_bins.size()-1,&fastmc_bins[0]);
   TH1D *fhNuTauFlux_FastMC     = new TH1D("nutau_flux",
					   "nutau_flux", fastmc_bins.size()-1,&fastmc_bins[0]);
   TH1D *fhNuTauBarFlux_FastMC  = new TH1D("nutaubar_flux",
					   "nutaubar_flux", fastmc_bins.size()-1,&fastmc_bins[0]);

   TH1D *fhNuMuCCEventRate_FastMC    = new TH1D("numu_cceventrate",  
					 "numu_cceventrate",fastmc_bins.size()-1,&fastmc_bins[0]);
   TH1D *fhNuMuBarCCEventRate_FastMC = new TH1D("numubar_cceventrate", 
					 "numubar_cceventrate", fastmc_bins.size()-1,&fastmc_bins[0]);
   TH1D *fhNuECCEventRate_FastMC     = new TH1D("nue_cceventrate",
					 "nue_cceventrate", fastmc_bins.size()-1,&fastmc_bins[0]);
   TH1D *fhNuEBarCCEventRate_FastMC  = new TH1D("nuebar_cceventrate",
					 "nuebar_cceventrate", fastmc_bins.size()-1,&fastmc_bins[0]);
   TH1D *fhNuTauCCEventRate_FastMC     = new TH1D("nutau_cceventrate",
					   "nutau_cceventrate", fastmc_bins.size()-1,&fastmc_bins[0]);
   TH1D *fhNuTauBarCCEventRate_FastMC  = new TH1D("nutaubar_cceventrate",
					   "nutaubar_cceventrate", fastmc_bins.size()-1,&fastmc_bins[0]);

   TH1D *fhNuMuNCEventRate_FastMC    = new TH1D("numu_nceventrate",  
					 "numu_nceventrate",fastmc_bins.size()-1,&fastmc_bins[0]);
   TH1D *fhNuMuBarNCEventRate_FastMC = new TH1D("numubar_nceventrate", 
					 "numubar_nceventrate", fastmc_bins.size()-1,&fastmc_bins[0]);
   TH1D *fhNuENCEventRate_FastMC     = new TH1D("nue_nceventrate",
					 "nue_nceventrate", fastmc_bins.size()-1,&fastmc_bins[0]);
   TH1D *fhNuEBarNCEventRate_FastMC  = new TH1D("nuebar_nceventrate",
					 "nuebar_nceventrate", fastmc_bins.size()-1,&fastmc_bins[0]);
   TH1D *fhNuTauNCEventRate_FastMC     = new TH1D("nutau_nceventrate",
					   "nutau_nceventrate", fastmc_bins.size()-1,&fastmc_bins[0]);
   TH1D *fhNuTauBarNCEventRate_FastMC  = new TH1D("nutaubar_nceventrate",
					   "nutaubar_nceventrate", fastmc_bins.size()-1,&fastmc_bins[0]);


 
   // make space for sumw2
   fhNuMuFlux->Sumw2();   
   fhNuMuBarFlux->Sumw2();   
   fhNuEFlux->Sumw2();   
   fhNuEBarFlux->Sumw2();   
   fhNuTauFlux->Sumw2();   
   fhNuTauBarFlux->Sumw2();   

   fhNuMuFlux_FastMC->Sumw2();   
   fhNuMuBarFlux_FastMC->Sumw2();   
   fhNuEFlux_FastMC->Sumw2();   
   fhNuEBarFlux_FastMC->Sumw2();   
   fhNuTauFlux_FastMC->Sumw2();   
   fhNuTauBarFlux_FastMC->Sumw2();   

   fhNuMuCCEventRate->Sumw2();   
   fhNuMuBarCCEventRate->Sumw2();   
   fhNuECCEventRate->Sumw2();   
   fhNuEBarCCEventRate->Sumw2();   
   fhNuTauCCEventRate->Sumw2();   
   fhNuTauBarCCEventRate->Sumw2();   
   fhNuMuNCEventRate->Sumw2();   
   fhNuMuBarNCEventRate->Sumw2();   
   fhNuENCEventRate->Sumw2();   
   fhNuEBarNCEventRate->Sumw2();   
   fhNuTauNCEventRate->Sumw2();   
   fhNuTauBarNCEventRate->Sumw2();   

   fhNuMuCCEventRate_FastMC->Sumw2();   
   fhNuMuBarCCEventRate_FastMC->Sumw2();   
   fhNuECCEventRate_FastMC->Sumw2();   
   fhNuEBarCCEventRate_FastMC->Sumw2();   
   fhNuTauCCEventRate_FastMC->Sumw2();   
   fhNuTauBarCCEventRate_FastMC->Sumw2();   
   fhNuMuNCEventRate_FastMC->Sumw2();   
   fhNuMuBarNCEventRate_FastMC->Sumw2();   
   fhNuENCEventRate_FastMC->Sumw2();   
   fhNuEBarNCEventRate_FastMC->Sumw2();   
   fhNuTauNCEventRate_FastMC->Sumw2();   
   fhNuTauBarNCEventRate_FastMC->Sumw2();   

   //
   //Note: the units are per GeV ONLY if you divide the weight by the binwidth, when filling, see below
   //It is a good idea to ALWAYS make the units /GeV. Then when different people have different binning
   // you can still compare the Y-axis this also goes for using 1e12 as your reference pot.
   std::string fluxtitle      = "Neutrinos / GeV / m^{2} / POT";
   std::string cceventratetitle      = "CC Events / POT";
   std::string nceventratetitle      = "CC Events / POT";

   SetTitles(fhNuMuFlux,         "#nu_{#mu} Energy (GeV)", fluxtitle);
   SetTitles(fhNuMuBarFlux,      "#bar{#nu}_{#mu} Energy (GeV)", fluxtitle);
   SetTitles(fhNuEFlux,          "#nu_{e} Energy (GeV)", fluxtitle);
   SetTitles(fhNuEBarFlux,       "#bar{#nu}_{e} Energy (GeV)", fluxtitle);
   SetTitles(fhNuTauFlux,          "#nu_{tau} Energy (GeV)", fluxtitle);
   SetTitles(fhNuTauBarFlux,       "#bar{#nu}_{#tau} Energy (GeV)", fluxtitle);

   SetTitles(fhNuMuFlux_FastMC,         "#nu_{#mu} Energy (GeV)", fluxtitle);
   SetTitles(fhNuMuBarFlux_FastMC,      "#bar{#nu}_{#mu} Energy (GeV)", fluxtitle);
   SetTitles(fhNuEFlux_FastMC,          "#nu_{e} Energy (GeV)", fluxtitle);
   SetTitles(fhNuEBarFlux_FastMC,       "#bar{#nu}_{e} Energy (GeV)", fluxtitle);
   SetTitles(fhNuTauFlux_FastMC,          "#nu_{tau} Energy (GeV)", fluxtitle);
   SetTitles(fhNuTauBarFlux_FastMC,       "#bar{#nu}_{#tau} Energy (GeV)", fluxtitle);

   SetTitles(fhNuMuCCEventRate,         "#nu_{#mu} Energy (GeV)", "#nu_{#mu} CC Events / GeV / kTon / POT");
   SetTitles(fhNuMuBarCCEventRate,      "#bar{#nu}_{#mu} Energy (GeV)", "#bar{#nu}_{#mu} CC Events / GeV / kTon / POT");
   SetTitles(fhNuECCEventRate,          "#nu_{e} Energy (GeV)", "#nu_{e} CC Events / GeV / kTon / POT");
   SetTitles(fhNuEBarCCEventRate,       "#bar{#nu}_{e} Energy (GeV)", "#bar{#nu}_{e} CC Events / GeV / kTon / POT");
   SetTitles(fhNuTauCCEventRate,          "#nu_{#tau} Energy (GeV)", "#nu_{#tau} CC Events / GeV / kTon / POT");
   SetTitles(fhNuTauBarCCEventRate,       "#bar{#nu}_{#tau} Energy (GeV)", "#bar{#nu}_{#tau} CC Events / GeV / kTon /  POT");

   SetTitles(fhNuMuCCEventRate_FastMC,         "#nu_{#mu} Energy (GeV)", "#nu_{#mu} CC Events / GeV / kTon / POT");
   SetTitles(fhNuMuBarCCEventRate_FastMC,      "#bar{#nu}_{#mu} Energy (GeV)", "#bar{#nu}_{#mu} CC Events / GeV / kTon / POT");
   SetTitles(fhNuECCEventRate_FastMC,          "#nu_{e} Energy (GeV)", "#nu_{e} CC Events / GeV / kTon / POT");
   SetTitles(fhNuEBarCCEventRate_FastMC,       "#bar{#nu}_{e} Energy (GeV)", "#bar{#nu}_{e} CC Events / GeV / kTon / POT");
   SetTitles(fhNuTauCCEventRate_FastMC,          "#nu_{#tau} Energy (GeV)", "#nu_{#tau} CC Events / GeV / kTon / POT");
   SetTitles(fhNuTauBarCCEventRate_FastMC,       "#bar{#nu}_{#tau} Energy (GeV)", "#bar{#nu}_{#tau} CC Events / GeV / kTon /  POT");

   SetTitles(fhNuMuNCEventRate,         "#nu_{#mu} Energy (GeV)", "#nu_{#mu} NC Events / GeV / kTon / POT");
   SetTitles(fhNuMuBarNCEventRate,      "#bar{#nu}_{#mu} Energy (GeV)","#bar{#nu}_{#mu} NC Events / GeV / kTon / POT");
   SetTitles(fhNuENCEventRate,          "#nu_{e} Energy (GeV)", "#nu_{e} NC Events / GeV / kTon / POT");
   SetTitles(fhNuEBarNCEventRate,       "#bar{#nu}_{e} Energy (GeV)", "#bar{#nu}_{e} NC Events / GeV / kTon / POT");
   SetTitles(fhNuTauNCEventRate,          "#nu_{#tau} Energy (GeV)", "#nu_{#tau} NC Events / GeV / kTon / POT");
   SetTitles(fhNuTauBarNCEventRate,       "#bar{#nu}_{#tau} Energy (GeV)", "#bar{#nu}_{#tau} NC Events / GeV / kTon / POT");

   SetTitles(fhNuMuNCEventRate_FastMC,         "#nu_{#mu} Energy (GeV)", "#nu_{#mu} NC Events / GeV / kTon / POT");
   SetTitles(fhNuMuBarNCEventRate_FastMC,      "#bar{#nu}_{#mu} Energy (GeV)","#bar{#nu}_{#mu} NC Events / GeV / kTon / POT");
   SetTitles(fhNuENCEventRate_FastMC,          "#nu_{e} Energy (GeV)", "#nu_{e} NC Events / GeV / kTon / POT");
   SetTitles(fhNuEBarNCEventRate_FastMC,       "#bar{#nu}_{e} Energy (GeV)", "#bar{#nu}_{e} NC Events / GeV / kTon / POT");
   SetTitles(fhNuTauNCEventRate_FastMC,          "#nu_{#tau} Energy (GeV)", "#nu_{#tau} NC Events / GeV / kTon / POT");
   SetTitles(fhNuTauBarNCEventRate_FastMC,       "#bar{#nu}_{#tau} Energy (GeV)", "#bar{#nu}_{#tau} NC Events / GeV / kTon / POT");

   //
   //start loop
   //
   Long64_t nentries = fChain->GetEntries();
   std::cout << "Total number of Entries = " << nentries << std::endl;

   

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) 
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      ++iread;
      
      if(iread % 100 == 0)
      {
	 std::cout << "Reading Entry " << iread << std::endl;
      }


      //
      // compute the flux at any detector location by
      // specifying the detector coordinates and computing the
      // detector weight for that location
      //
      //Call the reweighting function and give it the x, y and z coords
      //of your detector location in cm with respect to the MC origin.
      //For example the... 
      // MINERVA Coords are (x,y,z)              = (0.0, 0.0, 103099.0) cm.
      // MINOS Near Detector COORDS are  (x,y,z) = (0.0, 0.0, 103649.0) cm.
      // MINOS Far  Detector COORDS are  (x,y,z) = (0.0, 0.0, 73534000.0) cm.
      // NOVA  Far  Detector COORDS are  (x,y,z) = (1104000.0, -420000.0, 81045000.0)
      // should confirm with nova the coords, these are pretty old numbers.
      //
      // LBNE  Far Detector COORDS are   (x,y,z) = (0.0, 0.0, 129700000.0) cm.
      //                        

      //
      //note that these coords are the location of the MINOS ND 
      //(almost, these are more up-to-date coords)
      //So you can compare the MINOSND plots and the 
      // PLOTS and they should be nearly identical
      //this is a good check that the code works properly
      //

      double nuenergyatsomedet     = -999.0;
      double detectorwghtatsomedet = -999.0;
      std::vector<double> detvec;
      detvec.push_back(detx);
      detvec.push_back(dety);
      detvec.push_back(detz);

      //this function computes the dectector weight and neutrino energy at detx,y,z
      eventRates::GetWeight(detvec, detectorwghtatsomedet, nuenergyatsomedet);

      //
      //Note that for off-axis locations this function, eventRates::GetWeight, 
      //becomes more and more of
      //an approximation, this is because this function determines the weight
      //at one point in space and normalizes to a fidicuial volume of 1 meter
      //in radius around the beam axis. This works fine for on axis detectors
      //because the flux within a cross section of 1 meter around the beam axis
      //is reasonably flat as a function of neutrino energy
      //However the farther off axis the detector the more the flux varies 
      //across the cross sectional area of the detector
      //

      //Nimpwt is the same no matter what detector you are referring to
      double fluxwghtsomedet = (detectorwghtatsomedet*Nimpwt/3.1415)*(refpot/fTotalPOT);

      std::string current_string = "CC";
      double cceventratewghtsomedet       = fluxwghtsomedet * GetXSec((double)Ntype,(double)NenergyN[0],current_string);

      current_string = "NC";
      double nceventratewghtsomedet       = fluxwghtsomedet * GetXSec(Ntype,NenergyN[0],current_string);

      ///
      //Now fill the histograms
      //by neutrino type
      //
      if(Ntype == 56)
      {
	 fhNuMuFlux -> Fill(nuenergyatsomedet, fluxwghtsomedet);
	 fhNuMuCCEventRate -> Fill(nuenergyatsomedet, cceventratewghtsomedet);
	 fhNuMuNCEventRate -> Fill(nuenergyatsomedet, nceventratewghtsomedet);

	 fhNuMuFlux_FastMC -> Fill(nuenergyatsomedet, fluxwghtsomedet);
	 fhNuMuCCEventRate_FastMC -> Fill(nuenergyatsomedet, cceventratewghtsomedet);
	 fhNuMuNCEventRate_FastMC -> Fill(nuenergyatsomedet, nceventratewghtsomedet);
      }
      if(Ntype == 55)
      {
	 fhNuMuBarFlux -> Fill(nuenergyatsomedet, fluxwghtsomedet);
	 fhNuMuBarCCEventRate -> Fill(nuenergyatsomedet, cceventratewghtsomedet);
	 fhNuMuBarNCEventRate -> Fill(nuenergyatsomedet, nceventratewghtsomedet);

	 fhNuMuBarFlux_FastMC -> Fill(nuenergyatsomedet, fluxwghtsomedet);
	 fhNuMuBarCCEventRate_FastMC -> Fill(nuenergyatsomedet, cceventratewghtsomedet);
	 fhNuMuBarNCEventRate_FastMC -> Fill(nuenergyatsomedet, nceventratewghtsomedet);
      }
      if(Ntype == 53)
      {
	 fhNuEFlux -> Fill(nuenergyatsomedet, fluxwghtsomedet);
	 fhNuECCEventRate -> Fill(nuenergyatsomedet, cceventratewghtsomedet);
	 fhNuENCEventRate -> Fill(nuenergyatsomedet, nceventratewghtsomedet);

	 fhNuEFlux_FastMC -> Fill(nuenergyatsomedet, fluxwghtsomedet);
	 fhNuECCEventRate_FastMC -> Fill(nuenergyatsomedet, cceventratewghtsomedet);
	 fhNuENCEventRate_FastMC -> Fill(nuenergyatsomedet, nceventratewghtsomedet);
      }
      if(Ntype == 52)
      {
	 fhNuEBarFlux -> Fill(nuenergyatsomedet, fluxwghtsomedet);
	 fhNuEBarCCEventRate -> Fill(nuenergyatsomedet, cceventratewghtsomedet);
	 fhNuEBarNCEventRate -> Fill(nuenergyatsomedet, nceventratewghtsomedet);

	 fhNuEBarFlux_FastMC -> Fill(nuenergyatsomedet, fluxwghtsomedet);
	 fhNuEBarCCEventRate_FastMC -> Fill(nuenergyatsomedet, cceventratewghtsomedet);
	 fhNuEBarNCEventRate_FastMC -> Fill(nuenergyatsomedet, nceventratewghtsomedet);
      }
      
      if(Ntype == 58)
      {
	 fhNuTauFlux -> Fill(nuenergyatsomedet, fluxwghtsomedet);
	 fhNuTauCCEventRate -> Fill(nuenergyatsomedet, cceventratewghtsomedet);
	 fhNuTauNCEventRate -> Fill(nuenergyatsomedet, nceventratewghtsomedet);

	 fhNuTauFlux_FastMC -> Fill(nuenergyatsomedet, fluxwghtsomedet);
	 fhNuTauCCEventRate_FastMC -> Fill(nuenergyatsomedet, cceventratewghtsomedet);
	 fhNuTauNCEventRate_FastMC -> Fill(nuenergyatsomedet, nceventratewghtsomedet);
      }
      if(Ntype == 59)
      {
	 fhNuTauBarFlux -> Fill(nuenergyatsomedet, fluxwghtsomedet);
	 fhNuTauBarCCEventRate -> Fill(nuenergyatsomedet, cceventratewghtsomedet);
	 fhNuTauBarNCEventRate -> Fill(nuenergyatsomedet, nceventratewghtsomedet);

	 fhNuTauBarFlux_FastMC -> Fill(nuenergyatsomedet, fluxwghtsomedet);
	 fhNuTauBarCCEventRate_FastMC -> Fill(nuenergyatsomedet, cceventratewghtsomedet);
	 fhNuTauBarNCEventRate_FastMC -> Fill(nuenergyatsomedet, nceventratewghtsomedet);
      }
   }//end loop over entries

   // normalize by bin width
   fhNuMuFlux->Scale(1.0,"width");
   fhNuMuCCEventRate->Scale(1.0,"width");
   fhNuMuNCEventRate->Scale(1.0,"width");

   fhNuMuBarFlux->Scale(1.0,"width");
   fhNuMuBarCCEventRate->Scale(1.0,"width");
   fhNuMuBarNCEventRate->Scale(1.0,"width");

   fhNuEFlux->Scale(1.0,"width");
   fhNuECCEventRate->Scale(1.0,"width");
   fhNuENCEventRate->Scale(1.0,"width");

   fhNuEBarFlux->Scale(1.0,"width");
   fhNuEBarCCEventRate->Scale(1.0,"width");
   fhNuEBarNCEventRate->Scale(1.0,"width");

   fhNuTauFlux->Scale(1.0,"width");
   fhNuTauCCEventRate->Scale(1.0,"width");
   fhNuTauNCEventRate->Scale(1.0,"width");

   fhNuTauBarFlux->Scale(1.0,"width");
   fhNuTauBarCCEventRate->Scale(1.0,"width");
   fhNuTauBarNCEventRate->Scale(1.0,"width");

   //fhNuMuFlux_FastMC->Scale(1.0,"width");
   //fhNuMuCCEventRate_FastMC->Scale(1.0,"width");
   //fhNuMuNCEventRate_FastMC->Scale(1.0,"width");

   //fhNuMuBarFlux_FastMC->Scale(1.0,"width");
   //fhNuMuBarCCEventRate_FastMC->Scale(1.0,"width");
   //fhNuMuBarNCEventRate_FastMC->Scale(1.0,"width");

   //fhNuEFlux_FastMC->Scale(1.0,"width");
   //fhNuECCEventRate_FastMC->Scale(1.0,"width");
   //fhNuENCEventRate_FastMC->Scale(1.0,"width");

   //fhNuEBarFlux_FastMC->Scale(1.0,"width");
   //fhNuEBarCCEventRate_FastMC->Scale(1.0,"width");
   //fhNuEBarNCEventRate_FastMC->Scale(1.0,"width");

   //fhNuTauFlux_FastMC->Scale(1.0,"width");
   //fhNuTauCCEventRate_FastMC->Scale(1.0,"width");
   //fhNuTauNCEventRate_FastMC->Scale(1.0,"width");

   //fhNuTauBarFlux_FastMC->Scale(1.0,"width");
   //fhNuTauBarCCEventRate_FastMC->Scale(1.0,"width");
   //fhNuTauBarNCEventRate_FastMC->Scale(1.0,"width");

   //
   //draw and save histograms
   //

   TCanvas *c1 = new TCanvas("c1","Canvas 1",1);
   TCanvas *c2 = new TCanvas("c2","Canvas 2",1);
   TCanvas *c3 = new TCanvas("c3","Canvas 3",1);
   TCanvas *c4 = new TCanvas("c4","Canvas 4",1);
   TCanvas *c5 = new TCanvas("c5","Canvas 5",1);

   TCanvas *c6 = new TCanvas("c6","Canvas 6",1);
   TCanvas *c7 = new TCanvas("c7","Canvas 7",1);
   TCanvas *c8 = new TCanvas("c8","Canvas 8",1);
   TCanvas *c9 = new TCanvas("c9","Canvas 9",1);
   TCanvas *c10 = new TCanvas("c10","Canvas 10",1);
   TCanvas *c11 = new TCanvas("c11","Canvas 11",1);
   TCanvas *c12 = new TCanvas("c12","Canvas 12",1);
   TCanvas *c13 = new TCanvas("c13","Canvas 13",1);
   TCanvas *c14 = new TCanvas("c14","Canvas 14",1);
   TCanvas *c15 = new TCanvas("c15","Canvas 15",1);
   TCanvas *c16 = new TCanvas("c16","Canvas 16",1);
   TCanvas *c17 = new TCanvas("c17","Canvas 17",1);
   TCanvas *c18 = new TCanvas("c18","Canvas 18",1);

   // Style
   TGaxis::SetMaxDigits(2);
   fhNuMuFlux_FastMC->GetYaxis()->SetTitleOffset(1.4);
   fhNuMuBarFlux_FastMC->GetYaxis()->SetTitleOffset(1.4);
   fhNuEFlux_FastMC->GetYaxis()->SetTitleOffset(1.4);
   fhNuEBarFlux_FastMC->GetYaxis()->SetTitleOffset(1.4);
   fhNuTauFlux_FastMC->GetYaxis()->SetTitleOffset(1.4);
   fhNuTauBarFlux_FastMC->GetYaxis()->SetTitleOffset(1.4);

   fhNuMuCCEventRate_FastMC->GetYaxis()->SetTitleOffset(1.4);
   fhNuMuBarCCEventRate_FastMC->GetYaxis()->SetTitleOffset(1.4);
   fhNuECCEventRate_FastMC->GetYaxis()->SetTitleOffset(1.4);
   fhNuEBarCCEventRate_FastMC->GetYaxis()->SetTitleOffset(1.4);
   fhNuTauCCEventRate_FastMC->GetYaxis()->SetTitleOffset(1.4);
   fhNuTauBarCCEventRate_FastMC->GetYaxis()->SetTitleOffset(1.4);
 
   fhNuMuNCEventRate_FastMC->GetYaxis()->SetTitleOffset(1.4);
   fhNuMuBarNCEventRate_FastMC->GetYaxis()->SetTitleOffset(1.4);
   fhNuENCEventRate_FastMC->GetYaxis()->SetTitleOffset(1.4);
   fhNuEBarNCEventRate_FastMC->GetYaxis()->SetTitleOffset(1.4);
   fhNuTauNCEventRate_FastMC->GetYaxis()->SetTitleOffset(1.4);
   fhNuTauBarNCEventRate_FastMC->GetYaxis()->SetTitleOffset(1.4);

   //drawing histograms
   //
   {
     ffilename = ffilename + "_" + detectorname;
      
     c1->cd();
     fhNuMuFlux->Draw();
     std::string cpath1 = ffilename + "_" + fhNuMuFlux->GetName() + ".eps";
     std::string cpath2 = ffilename + "_" + fhNuMuFlux->GetName() + ".png";
     c1->SaveAs(cpath1.c_str());
     c1->SaveAs(cpath2.c_str());

     c2->cd();
     fhNuMuBarFlux->Draw();
     cpath1 = ffilename + "_" + fhNuMuBarFlux->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuMuBarFlux->GetName() + ".png";
     c2->SaveAs(cpath1.c_str());
     c2->SaveAs(cpath2.c_str());

     c3->cd();
     fhNuEFlux->Draw();
     cpath1 = ffilename + "_" + fhNuEFlux->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuEFlux->GetName() + ".png";
     c3->SaveAs(cpath1.c_str());
     c3->SaveAs(cpath2.c_str());
     
     c4->cd();
     fhNuEBarFlux->Draw();
     cpath1 = ffilename + "_" + fhNuEBarFlux->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuEBarFlux->GetName() + ".png";
     c4->SaveAs(cpath1.c_str());
     c4->SaveAs(cpath2.c_str());      

     c5->cd();
     fhNuTauFlux->Draw();
     cpath1 = ffilename + "_" + fhNuTauFlux->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuTauFlux->GetName() + ".png";
     c5->SaveAs(cpath1.c_str());
     c5->SaveAs(cpath2.c_str());
     
     c6->cd();
     fhNuTauBarFlux->Draw();
     cpath1 = ffilename + "_" + fhNuTauBarFlux->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuTauBarFlux->GetName() + ".png";
     c6->SaveAs(cpath1.c_str());
     c6->SaveAs(cpath2.c_str());      

     c7->cd();
     fhNuMuCCEventRate->Draw();
     cpath1 = ffilename + "_" + fhNuMuCCEventRate->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuMuCCEventRate->GetName() + ".png";
     c7->SaveAs(cpath1.c_str());
     c7->SaveAs(cpath2.c_str());

     c8->cd();
     fhNuMuBarCCEventRate->Draw();
     cpath1 = ffilename + "_" + fhNuMuBarCCEventRate->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuMuBarCCEventRate->GetName() + ".png";
     c8->SaveAs(cpath1.c_str());
     c8->SaveAs(cpath2.c_str());

     c9->cd();
     fhNuECCEventRate->Draw();
     cpath1 = ffilename + "_" + fhNuECCEventRate->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuECCEventRate->GetName() + ".png";
     c9->SaveAs(cpath1.c_str());
     c9->SaveAs(cpath2.c_str());
     
     c10->cd();
     fhNuEBarCCEventRate->Draw();
     cpath1 = ffilename + "_" + fhNuEBarCCEventRate->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuEBarCCEventRate->GetName() + ".png";
     c10->SaveAs(cpath1.c_str());
     c10->SaveAs(cpath2.c_str());      

     c11->cd();
     fhNuTauCCEventRate->Draw();
     cpath1 = ffilename + "_" + fhNuTauCCEventRate->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuTauCCEventRate->GetName() + ".png";
     c11->SaveAs(cpath1.c_str());
     c11->SaveAs(cpath2.c_str());
     
     c12->cd();
     fhNuTauBarCCEventRate->Draw();
     cpath1 = ffilename + "_" + fhNuTauBarCCEventRate->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuTauBarCCEventRate->GetName() + ".png";
     c12->SaveAs(cpath1.c_str());
     c12->SaveAs(cpath2.c_str());      

     c13->cd();
     fhNuMuNCEventRate->Draw();
     cpath1 = ffilename + "_" + fhNuMuNCEventRate->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuMuNCEventRate->GetName() + ".png";
     c13->SaveAs(cpath1.c_str());
     c13->SaveAs(cpath2.c_str());

     c14->cd();
     fhNuMuBarNCEventRate->Draw();
     cpath1 = ffilename + "_" + fhNuMuBarNCEventRate->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuMuBarNCEventRate->GetName() + ".png";
     c14->SaveAs(cpath1.c_str());
     c14->SaveAs(cpath2.c_str());

     c15->cd();
     fhNuENCEventRate->Draw();
     cpath1 = ffilename + "_" + fhNuENCEventRate->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuENCEventRate->GetName() + ".png";
     c15->SaveAs(cpath1.c_str());
     c15->SaveAs(cpath2.c_str());
     
     c16->cd();
     fhNuEBarNCEventRate->Draw();
     cpath1 = ffilename + "_" + fhNuEBarNCEventRate->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuEBarNCEventRate->GetName() + ".png";
     c16->SaveAs(cpath1.c_str());
     c16->SaveAs(cpath2.c_str());      

     c17->cd();
     fhNuTauNCEventRate->Draw();
     cpath1 = ffilename + "_" + fhNuTauNCEventRate->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuTauNCEventRate->GetName() + ".png";
     c17->SaveAs(cpath1.c_str());
     c17->SaveAs(cpath2.c_str());
     
     c18->cd();
     fhNuTauBarNCEventRate->Draw();
     cpath1 = ffilename + "_" + fhNuTauBarNCEventRate->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuTauBarNCEventRate->GetName() + ".png";
     c18->SaveAs(cpath1.c_str());
     c18->SaveAs(cpath2.c_str());      
   }

// Save histograms to a root file
   TFile f((ffilename+".root").c_str(),"recreate");
   fhNuMuFlux->Write();
   fhNuMuBarFlux->Write();
   fhNuEFlux->Write();
   fhNuEBarFlux->Write();
   fhNuTauFlux->Write();
   fhNuTauBarFlux->Write();
   fhNuMuCCEventRate->Write();
   fhNuMuBarCCEventRate->Write();
   fhNuECCEventRate->Write();
   fhNuEBarCCEventRate->Write();
   fhNuTauCCEventRate->Write();
   fhNuTauBarCCEventRate->Write();
   fhNuMuNCEventRate->Write();
   fhNuMuBarNCEventRate->Write();
   fhNuENCEventRate->Write();
   fhNuEBarNCEventRate->Write();
   fhNuTauNCEventRate->Write();
   fhNuTauBarNCEventRate->Write();
   f.Close();

   TFile g((ffilename+"_fastmc.root").c_str(),"recreate");
   fhNuMuFlux_FastMC->Write();
   fhNuMuBarFlux_FastMC->Write();
   fhNuEFlux_FastMC->Write();
   fhNuEBarFlux_FastMC->Write();
   fhNuTauFlux_FastMC->Write();
   fhNuTauBarFlux_FastMC->Write();
   fhNuMuCCEventRate_FastMC->Write();
   fhNuMuBarCCEventRate_FastMC->Write();
   fhNuECCEventRate_FastMC->Write();
   fhNuEBarCCEventRate_FastMC->Write();
   fhNuTauCCEventRate_FastMC->Write();
   fhNuTauBarCCEventRate_FastMC->Write();
   fhNuMuNCEventRate_FastMC->Write();
   fhNuMuBarNCEventRate_FastMC->Write();
   fhNuENCEventRate_FastMC->Write();
   fhNuEBarNCEventRate_FastMC->Write();
   fhNuTauNCEventRate_FastMC->Write();
   fhNuTauBarNCEventRate_FastMC->Write();
   g.Close();
      
}

//-------------------------------------------------------------------------------------
std::string eventRates::GetPOTAsString(const double dpot)
{

   std::stringstream potstrm;
   potstrm << scientific << dpot;

   string potstr = potstrm.str();

   //
   //get base
   //
   size_t baselength;
   if(potstr.find("e",0) != string::npos)
   {
      baselength = potstr.find("e",0);
   }
   else if(potstr.find("E",0) != string::npos)
   {
      baselength = potstr.find("E",0);
   }
   else
   {
      cout << "eventRates::GetPOTAsString - PROBLEM: pot is not in scientific notation" << endl;
      return "Problem";
   }
   
   string base = potstr.substr(0, baselength);

   //
   //get exp
   //
   size_t exppos;
   if(potstr.find("+",baselength) != string::npos)
   {
      exppos = potstr.find("+",baselength);
   }
   else if(potstr.find("-",baselength) != string::npos)
   {
      exppos = potstr.find("-",baselength);
   }
   else
   {
      cout << "eventRates::GetPOTAsString - PROBLEM: pot is not in scientific notation" << endl;
      return "Problem";
   }
   
   string exp = potstr.substr(exppos);


   //
   //modify base string if needed
   //
   string baseNumber = base;   

   size_t baseDecimalpos = base.find(".",0);
   if(baseDecimalpos != string::npos)
   {
      size_t baseNotZeropos = base.find_last_not_of("0", string::npos);
      if(baseNotZeropos != string::npos)
      {
         if(baseNotZeropos > baseDecimalpos)
         {
            baseNumber = base.substr(0,baseNotZeropos+1);
         }
         else
         {
            baseNumber = base.substr(0,baseDecimalpos+2);
         }

      }      
   }
   else
   {
      baseNumber = baseNumber + ".0";
   }

   //
   //modify exp string if needed
   //

   string expSign   = exp.substr(0, 1);
   string expNumber = exp.substr(1, string::npos);

   size_t expNotZeropos = expNumber.find_first_not_of("0",0);
   if(expNotZeropos != string::npos)
   {
      expNumber = expNumber.substr(expNotZeropos, string::npos);
   }



   string potfinalstr;

   //
   //put base and exp together
   //

   if(baseNumber.empty() && expNumber.empty())
   {
      cout << "eventRates::GetPOTAsString - PROBLEM: base number and exp number are both empty" << endl;
      return "Problem";
   }
   
   if(baseNumber == "1.0")
   {
      if(expSign == "-")
         potfinalstr = "10^{" + expSign + expNumber + "}";
      else
         potfinalstr = "10^{" + expNumber + "}";
   }
   else
   {
      if(expSign == "-")
         potfinalstr = baseNumber + "#times10^{" + expSign + expNumber + "}";
      else
         potfinalstr = baseNumber + "#times10^{" + expNumber + "}";
   }

   
   /*
   cout << "pot str = " << potstr << endl
        << " base = " << base << endl
        << " baseNumber = " << baseNumber << endl
        << " expSign = " << expSign << endl
        << " expNumber = " << expNumber << endl
        << " potfinalstr = " << potfinalstr << endl;
   */
   
   
  return potfinalstr;
}


//---------------------------------------------------------------------------------------------
void eventRates::SetTitles(TH1* h, const std::string &xtitle, const std::string &ytitle)
{
   if(!ytitle.empty())
   {
      h -> GetYaxis() -> SetTitle(ytitle.c_str());
      h -> GetYaxis() -> CenterTitle();
   }
   if(!xtitle.empty())
   {
      h -> GetXaxis() -> SetTitle(xtitle.c_str());
      h -> GetXaxis() -> CenterTitle();
   }
}

//---------------------------------------------------------------------------------------------
double eventRates::GetWeight(const std::vector<double> xdet,
			 double& nu_wght, 
			 double& nu_energy)
{

//   if(iread > 60000)
//      std::cout << "start iread = " << iread;

   //assumes units are GeV and cm

   const double rdet    = 100.0; //in cm
   const double pimass  =    0.13957; //in GeV
   const double kmass   =    0.49368;
   const double k0mass  =    0.49767;
   const double mumass  =    0.105658389;
   const double taumass =    1.77682;
   
   //these are geant codes not PDG
   const int    nue     =  53;
   const int    nuebar  =  52;
   const int    numu    =  56;
   const int    numubar =  55;
   const int    nutau    =  58; // is this right?
   const int    nutaubar =  59; // is this right?
   const int    muplus  =  5;
   const int    muminus =  6;
   
   double parent_mass=0.;
   if      (ptype == 8  || ptype == 9)  parent_mass = pimass;
   else if (ptype == 11 || ptype == 12) parent_mass = kmass;
   else if (ptype == 10)                parent_mass = k0mass;
   else if (ptype == 5  || ptype == 6)  parent_mass = mumass;
   else 
   {
      cout <<"eventRates::GetWeight - Wrong parent type!! "<< ptype << " = "
	   << ptype << " Decay code = " << Ndecay <<endl;
      
     return -999;
   }
   
   double parent_energy = sqrt(pdPx*pdPx +
			       pdPy*pdPy +
			       pdPz*pdPz + 
			       parent_mass*parent_mass);
   double gamma = parent_energy / parent_mass;
   double gamma_sqr = gamma*gamma;
   double beta_mag = sqrt((gamma_sqr-1.)/gamma_sqr);
   
   double enuzr = Necm;
   
   double rad = sqrt((xdet[0]-Vx)*(xdet[0]-Vx) +
		     (xdet[1]-Vy)*(xdet[1]-Vy) +
		     (xdet[2]-Vz)*(xdet[2]-Vz));
   
   double parentp = sqrt((pdPx*pdPx)+
			 (pdPy*pdPy)+
			 (pdPz*pdPz));
   double costh_pardet = (pdPx*(xdet[0]-Vx) +
			 pdPy*(xdet[1]-Vy) +
			  pdPz*(xdet[2]-Vz))/(parentp*rad);

  if (costh_pardet>1.) costh_pardet = 1.;
  else if (costh_pardet<-1.) costh_pardet = -1.;
  double theta_pardet = acos(costh_pardet);

  double emrat = 1./(gamma*(1. - beta_mag * cos(theta_pardet)));

  nu_energy = emrat*enuzr;

  double sangdet = (rdet*rdet /(rad*rad)/ 4.); 

  nu_wght = sangdet * emrat * emrat;

  //done for all except polarized muon
  // in which case need to modify weight
  if (ptype==muplus || ptype==muminus)
  {
     //boost new neutrino to mu decay cm
     double beta[3];
     double p_nu[3]; //nu momentum
     beta[0]=pdPx / parent_energy;
     beta[1]=pdPy / parent_energy;
     beta[2]=pdPz / parent_energy;
     
     p_nu[0] = (xdet[0]- Vx) * nu_energy / rad;
     p_nu[1] = (xdet[1]- Vy) * nu_energy / rad;
     p_nu[2] = (xdet[2]- Vz) * nu_energy / rad;

     double partial = gamma*(beta[0]*p_nu[0]+
			     beta[1]*p_nu[1]+
			     beta[2]*p_nu[2]);
     partial = nu_energy-partial / (gamma+1.);
     double p_dcm_nu[4];
     for (int i=0;i<3;i++) p_dcm_nu[i]=p_nu[i]-beta[i]*gamma*partial;
     p_dcm_nu[3]=0.;
     for (int i=0;i<3;i++) p_dcm_nu[3]+=p_dcm_nu[i]*p_dcm_nu[i];
     p_dcm_nu[3]=sqrt(p_dcm_nu[3]);
     
     //boost parent of mu to mu production cm
     gamma=ppenergy / parent_mass;
     beta[0] = ppdxdz * pppz / ppenergy;
     beta[1] = ppdydz * pppz / ppenergy;
     beta[2] =                  pppz / ppenergy;
     partial = gamma*(beta[0]*muparpx+
		      beta[1]*muparpy+
		      beta[2]*muparpz);
     partial = mupare - partial / (gamma+1.);
     double p_pcm_mp[4];
     p_pcm_mp[0]=muparpx-beta[0]*gamma*partial;
     p_pcm_mp[1]=muparpy-beta[1]*gamma*partial;
     p_pcm_mp[2]=muparpz-beta[2]*gamma*partial;
     p_pcm_mp[3]=0.;
     for (int i=0;i<3;i++) p_pcm_mp[3]+=p_pcm_mp[i]*p_pcm_mp[i];
     p_pcm_mp[3]=sqrt(p_pcm_mp[3]);
     
     double wt_ratio = 1.;
     //have to check p_pcm_mp
     //it can be 0 if mupar..=0. (I guess muons created in target??)
     if (p_pcm_mp[3] != 0. ) {
	//calc new decay angle w.r.t. (anti)spin direction
	double costh = (p_dcm_nu[0]*p_pcm_mp[0]+ 
			p_dcm_nu[1]*p_pcm_mp[1]+ 
			p_dcm_nu[2]*p_pcm_mp[2])/(p_dcm_nu[3]*p_pcm_mp[3]);
	
	if (costh>1.) costh = 1.;
	else if (costh<-1.) costh = -1.;
	
	//calc relative weight due to angle difference
	if (Ntype == nue || Ntype == nuebar)
	{
	   wt_ratio = 1.-costh;
	}
	else if (Ntype == numu || Ntype == numubar) 
	{
	   double xnu = 2.* enuzr / mumass;
	   wt_ratio = ( (3.-2.*xnu) - (1.-2.*xnu)*costh ) / (3.-2.*xnu);
	}
	else if (Ntype == nutau || Ntype == nutaubar) 
	{
	   double xnu = 2.* enuzr / taumass;
	   wt_ratio = ( (3.-2.*xnu) - (1.-2.*xnu)*costh ) / (3.-2.*xnu);
	   std::cout << "calculating weight for tau neutrino; this may not be correct" << std::endl;
	}
	else 
	{
	   std::cout << "eventRates:: Bad neutrino type = " << Ntype << std::endl;
	}
     }
     nu_wght *= wt_ratio;
  }
  
//   if(iread > 60000)
//      std::cout << " end iread = " << iread << std::endl;
  
  return nu_wght;
}

double eventRates::GetXSec( double nu_type, 
			    double nu_energy, 
			    std::string current)

{

  if( current != "NC" && current != "CC") {
    cout <<" eventRates::GetXSec: Current other than NC or CC specified... I don't know what to do." << endl;
    return -999;
  }

  int file_index=0;
  if (nu_type == 52) file_index = 0;
  if (nu_type == 53) file_index = 1;
  if (nu_type == 55) file_index = 2;
  if (nu_type == 56) file_index = 3;

  int current_index = 0;
  if( current == "NC")
    current_index = 1;

  // calculate cross section
  double thexsec = 0.;

  // scale factor
  double scale_factor = 6.026e-10;
  //      xseccc = xseccc * 4.09e9
  //     10**-38 cm2 * 10**6 kg/kton * 3.8e20 POT/year *1iron/56/1.66e-27kg
  //     * 10**-4 m2/cm2 = 4.09e9 conversion factor
  //     since table was 26*sigma(nu-proton) + 30*sigma(nu-neutron)
  //     
  //     10**-38 cm2 * 10**6 kg/kton *1iron/56/1.66e-27kg
  //     * 10**-4 m2/cm2 = 1.076e-11 conversion factor
  //	
  //     10**-38 cm2 * 10**6 kg/kton * 1 nucleon / 1.66e-27 kg
  //     * 10**-4 m2/cm2 = 6.026e-10

  // if energy is higher than any available xsection point
  // return xsection of highest available energy point
  if ( nu_energy > f_e_arr[fnlines-1][file_index][current_index] ) {
    thexsec = f_xsec_arr[fnlines-1][file_index][current_index]*f_e_arr[fnlines-1][file_index][current_index]*scale_factor;
  }
  // if energy is lower than any available xsection point
  // return xsection of lowest available energy poitn  
  else if ( nu_energy < f_e_arr[0][file_index][current_index] ) {
    thexsec = f_xsec_arr[0][file_index][current_index]*f_e_arr[0][file_index][current_index]*scale_factor;
  }
  else {
    // if not, find the xsections for energy values immendiately above and 
    // below the requested energy
    int energy_index = 0;
    for(int i = 0; i< fnlines-1; i++)
      if( nu_energy > f_e_arr[i][file_index][current_index] && 
	  nu_energy < f_e_arr[i+1][file_index][current_index]) {
	energy_index = i;
	break;
      }

    double sig1 = f_xsec_arr[energy_index][file_index][current_index];
    double sig2 = f_xsec_arr[energy_index+1][file_index][current_index];
	
    thexsec = sig1 + ((sig2 - sig1)/(f_e_arr[energy_index+1][file_index][current_index]-f_e_arr[energy_index][file_index][current_index]))*(nu_energy - f_e_arr[energy_index][file_index][current_index]);
    thexsec = thexsec * nu_energy;

    thexsec = thexsec * scale_factor;
  }

  //std::cout<<"nu_type "<<nu_type<<" current "<<current<<" energy "<<nu_energy <<" xsec "<<thexsec/nu_energy/scale_factor<<std::endl;

  return thexsec;



}


void eventRates::ReadXSecsFromFiles() {

  fnbins = 1500;
  
  std::string base = "data/argon/";
   
  for(int current = 1; current <= 2; current++) {

  string charge = "cc";

  if ( current == 2 ) {
      charge = "nc";
    }

    const int narr=4;
    string suffix[narr];
    if (current == 1) {
      suffix[0] = "_nuebar.dat";
      suffix[1] = "_nue.dat";
      suffix[2] = "_numubar.dat";
      suffix[3] = "_numu.dat";
    } else if (current==2) {
      suffix[0] = "_numubar.dat";
      suffix[1] = "_numu.dat";
      suffix[2] = "_numubar.dat";
      suffix[3] = "_numu.dat";
    }
    string filename[narr];
    for (int i=0; i<narr; i++) {
      filename[i] = "xsec_"+charge+suffix[i];
      string tmpfilename = base + filename[i];
      
      fdat_file[i].open(tmpfilename.c_str());
      if (fdat_file[i].fail()) {
	cout << " File not found: " << filename[i] << endl;
	//assert(0);
      }
      else {
	cout << " Opened "<<filename[i] << endl;
      }
      double row[2];
      fnlines = 0;
      
      while ( fdat_file[i] >> row[0] >> row[1] ) {
	
	if (fnlines >= fnbins) {
	  cout << " length of data file exceed array size. Fix me. " << filename << endl;
	  //assert(0);
	}

	f_e_arr[fnlines][i][current-1] = row[0];
	f_xsec_arr[fnlines][i][current-1] = row[1];
	fnlines++;
      }
      fdat_file[i].close();
    }
  }
}




















/*
   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) 
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
   }
*/

