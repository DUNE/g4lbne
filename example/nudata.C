#define nudata_cxx
#include "nudata.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <math.h>

using namespace std;

int iread = 0;

void nudata::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L nudata.C
//      Root > nudata t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
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
   double refpot          = 1.0e12;
   std::string potref_str = nudata::GetPOTAsString(refpot);

   //
   //make histograms !!!!!!!!!!!define bin size and xmin and xmax
   //
   int nbins   = 120;
   double xmin = 0.0;
   double xmax = 60.0;
   double binwidth = (xmax-xmin)/(double)nbins;

   //!!!!!!!!!!!!!!!!!!
   //"name" the detector location that you are plotting the flux at
   // This example is plotting the flux at the LBNE Far Detector using 
   // the detector weight and energy that is contained within the flux ntuple
   // also using the a detector weight computed independently
   //!!!!!!!!!!!!!!!!!!!

   std::string detectorname  = "LBNEFD_A";
   
   TH1F *fhTotalFluxLBNEFD_A   = new TH1F(("totalfluxat"   + detectorname).c_str(),  
					 ("totalfluxat"   + detectorname).c_str(), nbins,xmin,xmax);
   TH1F *fhNuMuFluxLBNEFD_A    = new TH1F(("numufluxat"    + detectorname).c_str(),  
					 ("numufluxat"    + detectorname).c_str(), nbins,xmin,xmax);
   TH1F *fhNuMuBarFluxLBNEFD_A = new TH1F(("numubarfluxat" + detectorname).c_str(), 
					 ("numubarfluxat" + detectorname).c_str(), nbins,xmin,xmax);
   TH1F *fhNuEFluxLBNEFD_A     = new TH1F(("nuefluxat"     + detectorname).c_str(),
					 ("nuefluxat"     + detectorname).c_str(), nbins,xmin,xmax);
   TH1F *fhNuEBarFluxLBNEFD_A  = new TH1F(("nuebarfluxat"  + detectorname).c_str(),
					 ("nuebarfluxat"  + detectorname).c_str(), nbins,xmin,xmax);
   

   detectorname  = "LBNEFD_B";
   
   TH1F *fhTotalFluxLBNEFD_B   = new TH1F(("totalfluxat"   + detectorname).c_str(),  
					      ("totalfluxat"   + detectorname).c_str(), nbins,xmin,xmax);
   TH1F *fhNuMuFluxLBNEFD_B    = new TH1F(("numufluxat"    + detectorname).c_str(),  
					      ("numufluxat"    + detectorname).c_str(), nbins,xmin,xmax);
   TH1F *fhNuMuBarFluxLBNEFD_B = new TH1F(("numubarfluxat" + detectorname).c_str(), 
					      ("numubarfluxat" + detectorname).c_str(), nbins,xmin,xmax);
   TH1F *fhNuEFluxLBNEFD_B     = new TH1F(("nuefluxat"     + detectorname).c_str(),
					      ("nuefluxat"     + detectorname).c_str(), nbins,xmin,xmax);
   TH1F *fhNuEBarFluxLBNEFD_B  = new TH1F(("nuebarfluxat"  + detectorname).c_str(),
					      ("nuebarfluxat"  + detectorname).c_str(), nbins,xmin,xmax);

   //
   //Note: the units are per GeV ONLY if you divide the weight by the binwidth, when filling, see below
   //It is a good idea to ALWAYS make the units /GeV. Then when different people have different binning
   // you can still compare the Y-axis this also goes for using 1e12 as your reference pot.
   std::string fluxtitle      = "Neutrinos/GeV/m^{2}/"     + potref_str + "POT";

   SetTitles(fhTotalFluxLBNEFD_A,        "All Neutrino Energy (GeV)", fluxtitle);
   SetTitles(fhNuMuFluxLBNEFD_A,         "#nu_{#mu} Energy (GeV)", fluxtitle);
   SetTitles(fhNuMuBarFluxLBNEFD_A,      "#bar{#nu}_{#mu} Energy (GeV)", fluxtitle);
   SetTitles(fhNuEFluxLBNEFD_A,          "#nu_{e} Energy (GeV)", fluxtitle);
   SetTitles(fhNuEBarFluxLBNEFD_A,       "#bar{#nu}_{e} Energy (GeV)", fluxtitle);

   SetTitles(fhTotalFluxLBNEFD_B,        "All Neutrino Energy (GeV)", fluxtitle);
   SetTitles(fhNuMuFluxLBNEFD_B,         "#nu_{#mu} Energy (GeV)", fluxtitle);
   SetTitles(fhNuMuBarFluxLBNEFD_B,      "#bar{#nu}_{#mu} Energy (GeV)", fluxtitle);
   SetTitles(fhNuEFluxLBNEFD_B,          "#nu_{e} Energy (GeV)", fluxtitle);
   SetTitles(fhNuEBarFluxLBNEFD_B,       "#bar{#nu}_{e} Energy (GeV)", fluxtitle);

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
      
      if(iread % 10000 == 0)
      {
	 std::cout << "Reading Entry " << iread << std::endl;
      }


      //
      // compute the flux using the detector weights stored in the ntuple
      //

      //
      //To use the detector weight and energy that is stored in the flux ntuple
      // you must know what detector index [0] of the arrays NenergyF 
      // and NWtFar corresponds to.
      // The g4lbne input file that you used contains the x, y and z corrdinates
      // of the detector that corresponds to the NenergyF[0] and NWtFar[0] values
      // in the flux ntuple.
      // Look under the "FluxArea" section of the input file you used to generate flux 
      // flux files you are now looping over.
      // The Flux Area parameters in that file are
      //
      //FluxArea       1
      //C=======================================================================
      // FluxAreaX0near 0.0
      // FluxAreaY0near 0.0
      // FluxAreaZ0near 1039.35
      // FluxAreaRnear  1.0
      // FluxAreaX0far  0.0
      // FluxAreaY0far  0.0
      // FluxAreaZ0far  735.34E3
      // FluxAreaRfar   1.0
      //...

      double nuenergyatlbnefd     = NenergyF[0];
      double detectorwghtatlbnefd = NWtFar[0];
      //Nimpwt is the same no matter what detector you are referring to
      double fluxwghtlbnefd      = (detectorwghtatlbnefd*Nimpwt/3.1415)*(refpot/fTotalPOT);


      //
      // compute the flux at any detector location by
      // specifying the detector coordinates and computing the
      // detector weight for that location
      //
      

      //!!!!!!!!!!!!!!!!!!!!!!!
      //
      //Call the reweighting function and give it the x, y and z coords
      //of your detector location in cm with respect to the MC origin.
      //For example the... 
      // MINERVA Coords are (x,y,z)              = (0.0, 0.0, 103099.0) cm.
      // MINOS Near Detector COORDS are  (x,y,z) = (0.0, 0.0, 103649.0) cm.
      // MINOS Far  Detector COORDS are  (x,y,z) = (0.0, 0.0, 73534000.0) cm.
      // NOVA  Far  Detector COORDS are  (x,y,z) = (1104000.0, -420000.0, 81045000.0)
      // should confirm with nova the coords, these are pretty old numbers.
      // LBNE  Far Detector COORDS are   (x,y,z) = (0.0, 0.0, 129700000.0) cm.
      //
      double detx = 0.0;
      double dety = 0.0;
      double detz = 129700000.0; 
      //
      //note that these coords are the location of the LBNE FD 
      //So you can compare the LBNEFD_A plots and the 
      //LBNEFD_B PLOTS and they should be nearly identical

      double nuenergyatsomedet     = -999.0;
      double detectorwghtatsomedet = -999.0;
      std::vector<double> detvec;
      detvec.push_back(detx);
      detvec.push_back(dety);
      detvec.push_back(detz);

      //this function computes the dectector weight and neutrino energy at detx,y,z
      nudata::GetWeight(detvec, detectorwghtatsomedet, nuenergyatsomedet);

      //
      //Note that for off-axis locations this function, nudata::GetWeight, 
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

      ///
      //Now fill the histograms
      //

      fhTotalFluxLBNEFD_A      -> Fill(nuenergyatlbnefd, fluxwghtlbnefd/binwidth);
      fhTotalFluxLBNEFD_B -> Fill(nuenergyatsomedet, fluxwghtsomedet/binwidth);
      //
      //by neutrino type
      //
      if(Ntype == 56)
      {
	 fhNuMuFluxLBNEFD_A      -> Fill(nuenergyatlbnefd, fluxwghtlbnefd/binwidth);
	 fhNuMuFluxLBNEFD_B -> Fill(nuenergyatsomedet, fluxwghtsomedet/binwidth);
      }
      if(Ntype == 55)
      {
	 fhNuMuBarFluxLBNEFD_A      -> Fill(nuenergyatlbnefd, fluxwghtlbnefd/binwidth);
	 fhNuMuBarFluxLBNEFD_B -> Fill(nuenergyatsomedet, fluxwghtsomedet/binwidth);
      }
      if(Ntype == 53)
      {
	 fhNuEFluxLBNEFD_A      -> Fill(nuenergyatlbnefd, fluxwghtlbnefd/binwidth);
	 fhNuEFluxLBNEFD_B -> Fill(nuenergyatsomedet, fluxwghtsomedet/binwidth);
      }
      if(Ntype == 52)
      {
	 fhNuEBarFluxLBNEFD_A      -> Fill(nuenergyatlbnefd, fluxwghtlbnefd/binwidth);
	 fhNuEBarFluxLBNEFD_B -> Fill(nuenergyatsomedet, fluxwghtsomedet/binwidth);
      }


   }//end loop over entries

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

   // Style
   TGaxis::SetMaxDigits(2);

   //
   //drawing for LBNEFD_A histograms
   //
   {
      
     c1->cd();
     fhTotalFluxLBNEFD_A->Draw();

     std::string cpath1 = ffilename + "_" + fhTotalFluxLBNEFD_A->GetName() + ".eps";
     std::string cpath2 = ffilename + "_" + fhTotalFluxLBNEFD_A->GetName() + ".png";
     c1->SaveAs(cpath1.c_str());
     c1->SaveAs(cpath2.c_str());
     
     c2->cd();
     fhNuMuFluxLBNEFD_A->Draw();
     cpath1 = ffilename + "_" + fhNuMuFluxLBNEFD_A->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuMuFluxLBNEFD_A->GetName() + ".png";
     c2->SaveAs(cpath1.c_str());
     c2->SaveAs(cpath2.c_str());

     c3->cd();
     fhNuMuBarFluxLBNEFD_A->Draw();
     cpath1 = ffilename + "_" + fhNuMuBarFluxLBNEFD_A->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuMuBarFluxLBNEFD_A->GetName() + ".png";
     c3->SaveAs(cpath1.c_str());
     c3->SaveAs(cpath2.c_str());

     c4->cd();
     fhNuEFluxLBNEFD_A->Draw();
     cpath1 = ffilename + "_" + fhNuEFluxLBNEFD_A->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuEFluxLBNEFD_A->GetName() + ".png";
     c4->SaveAs(cpath1.c_str());
     c4->SaveAs(cpath2.c_str());
     
     c5->cd();
     fhNuEBarFluxLBNEFD_A->Draw();
     cpath1 = ffilename + "_" + fhNuEBarFluxLBNEFD_A->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuEBarFluxLBNEFD_A->GetName() + ".png";
     c5->SaveAs(cpath1.c_str());
     c5->SaveAs(cpath2.c_str());

   }


   //
   //drawing for LBNEFD_B histograms
   //
   {
      
     c6->cd();
     fhTotalFluxLBNEFD_B->Draw();

     std::string cpath1 = ffilename + "_" + fhTotalFluxLBNEFD_B->GetName() + ".eps";
     std::string cpath2 = ffilename + "_" + fhTotalFluxLBNEFD_B->GetName() + ".png";
     c6->SaveAs(cpath1.c_str());
     c6->SaveAs(cpath2.c_str());
     
     c7->cd();
     fhNuMuFluxLBNEFD_B->Draw();
     cpath1 = ffilename + "_" + fhNuMuFluxLBNEFD_B->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuMuFluxLBNEFD_B->GetName() + ".png";
     c7->SaveAs(cpath1.c_str());
     c7->SaveAs(cpath2.c_str());

     c8->cd();
     fhNuMuBarFluxLBNEFD_B->Draw();
     cpath1 = ffilename + "_" + fhNuMuBarFluxLBNEFD_B->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuMuBarFluxLBNEFD_B->GetName() + ".png";
     c8->SaveAs(cpath1.c_str());
     c8->SaveAs(cpath2.c_str());

     c9->cd();
     fhNuEFluxLBNEFD_B->Draw();
     cpath1 = ffilename + "_" + fhNuEFluxLBNEFD_B->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuEFluxLBNEFD_B->GetName() + ".png";
     c9->SaveAs(cpath1.c_str());
     c9->SaveAs(cpath2.c_str());
     
     c10->cd();
     fhNuEBarFluxLBNEFD_B->Draw();
     cpath1 = ffilename + "_" + fhNuEBarFluxLBNEFD_B->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuEBarFluxLBNEFD_B->GetName() + ".png";
     c10->SaveAs(cpath1.c_str());
     c10->SaveAs(cpath2.c_str());      

   }

// Save histograms to a root file
   TFile f("flux_histograms.root","recreate");
   fhTotalFluxLBNEFD_A->Write();
   fhNuMuFluxLBNEFD_A->Write();
   fhNuMuBarFluxLBNEFD_A->Write();
   fhNuEFluxLBNEFD_A->Write();
   fhNuEBarFluxLBNEFD_A->Write();
   fhTotalFluxLBNEFD_B->Write();
   fhNuMuFluxLBNEFD_B->Write();
   fhNuMuBarFluxLBNEFD_B->Write();
   fhNuEFluxLBNEFD_B->Write();
   fhNuEBarFluxLBNEFD_B->Write();
   f.Close();
      
}

//-------------------------------------------------------------------------------------
std::string nudata::GetPOTAsString(const double dpot)
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
      cout << "nudata::GetPOTAsString - PROBLEM: pot is not in scientific notation" << endl;
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
      cout << "nudata::GetPOTAsString - PROBLEM: pot is not in scientific notation" << endl;
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
      cout << "nudata::GetPOTAsString - PROBLEM: base number and exp number are both empty" << endl;
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
void nudata::SetTitles(TH1* h, const std::string &xtitle, const std::string &ytitle)
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
double nudata::GetWeight(const std::vector<double> xdet,
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
   //these are geant codes not PDG
   const int    nue     =  53;
   const int    nuebar  =  52;
   const int    numu    =  56;
   const int    numubar =  55;
   const int    muplus  =  5;
   const int    muminus =  6;
   
   double parent_mass=0.;
   if      (ptype == 8  || ptype == 9)  parent_mass = pimass;
   else if (ptype == 11 || ptype == 12) parent_mass = kmass;
   else if (ptype == 10)                parent_mass = k0mass;
   else if (ptype == 5  || ptype == 6)  parent_mass = mumass;
   else 
   {
      cout <<"nudata::GetWeight - Wrong parent type!! "<< ptype << " = "
	   << ptype << " Decay code = " << Ndecay <<endl;
      
     return;
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
	else 
	{
	   std::cout << "nudata:: Bad neutrino type = " << Ntype << std::endl;
	}
     }
     nu_wght *= wt_ratio;
  }
  
//   if(iread > 60000)
//      std::cout << " end iread = " << iread << std::endl;
  
  return nu_wght;
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

