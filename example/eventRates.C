#define eventRates_cxx
#include "eventRates.h"
#include <TH2.h>
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
//      Root > .L eventRates.C
//      Root > eventRates t
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
   std::string potref_str = eventRates::GetPOTAsString(refpot);

   //
   //make histograms !!!!!!!!!!!define bin size and xmin and xmax
   //
   int nbins   = 40;
   double xmin = 0.0;
   double xmax = 20.0;
   double binwidth = (xmax-xmin)/(double)nbins;

   std::string detectorname  = "";
   
   TH1F *fhNuMuFlux    = new TH1F(("numufluxat"    + detectorname).c_str(),  
					      ("numufluxat"    + detectorname).c_str(), nbins,xmin,xmax);
   TH1F *fhNuMuBarFlux = new TH1F(("numubarfluxat" + detectorname).c_str(), 
					      ("numubarfluxat" + detectorname).c_str(), nbins/2,xmin,xmax);
   TH1F *fhNuEFlux     = new TH1F(("nuefluxat"     + detectorname).c_str(),
					      ("nuefluxat"     + detectorname).c_str(), nbins/2,xmin,xmax);
   TH1F *fhNuEBarFlux  = new TH1F(("nuebarfluxat"  + detectorname).c_str(),
					      ("nuebarfluxat"  + detectorname).c_str(), nbins/2,xmin,xmax);

   TH1F *fhNuMuCCEventRate    = new TH1F(("numucceventrateat"    + detectorname).c_str(),  
						("numucceventrateat"    + detectorname).c_str(), nbins,xmin,xmax);
   TH1F *fhNuMuBarCCEventRate = new TH1F(("numubarcceventrateat" + detectorname).c_str(), 
						("numubarcceventrateat" + detectorname).c_str(), nbins/2,xmin,xmax);
   TH1F *fhNuECCEventRate     = new TH1F(("nuecceventrateat"     + detectorname).c_str(),
						("nuecceventrateat"     + detectorname).c_str(), nbins/2,xmin,xmax);
   TH1F *fhNuEBarCCEventRate  = new TH1F(("nuebarcceventrateat"  + detectorname).c_str(),
						("nuebarcceventrateat"  + detectorname).c_str(), nbins/2,xmin,xmax);
   
   TH1F *fhNuMuNCEventRate    = new TH1F(("numunceventrateat"    + detectorname).c_str(),  
						("numunceventrateat"    + detectorname).c_str(), nbins,xmin,xmax);
   TH1F *fhNuMuBarNCEventRate = new TH1F(("numubarnceventrateat" + detectorname).c_str(), 
						("numubarnceventrateat" + detectorname).c_str(), nbins/2,xmin,xmax);
   TH1F *fhNuENCEventRate     = new TH1F(("nuenceventrateat"     + detectorname).c_str(),
						("nuenceventrateat"     + detectorname).c_str(), nbins/2,xmin,xmax);
   TH1F *fhNuEBarNCEventRate  = new TH1F(("nuebarnceventrateat"  + detectorname).c_str(),
						("nuebarnceventrateat"  + detectorname).c_str(), nbins/2,xmin,xmax);
   
   //
   //Note: the units are per GeV ONLY if you divide the weight by the binwidth, when filling, see below
   //It is a good idea to ALWAYS make the units /GeV. Then when different people have different binning
   // you can still compare the Y-axis this also goes for using 1e12 as your reference pot.
   std::string fluxtitle      = "Neutrinos/GeV/m^{2}/"     + potref_str + "POT";
   std::string cceventratetitle      = "CC Events/"     + potref_str + "POT";
   std::string nceventratetitle      = "CC Events/"     + potref_str + "POT";

   SetTitles(fhNuMuFlux,         "#nu_{#mu} Energy (GeV)", fluxtitle);
   SetTitles(fhNuMuBarFlux,      "#bar{#nu}_{#mu} Energy (GeV)", fluxtitle);
   SetTitles(fhNuEFlux,          "#nu_{e} Energy (GeV)", fluxtitle);
   SetTitles(fhNuEBarFlux,       "#bar{#nu}_{e} Energy (GeV)", fluxtitle);

   SetTitles(fhNuMuCCEventRate,         "#nu_{#mu} Energy (GeV)", "#nu_{#mu} CC Events / GeV / kTon / 10^{12} POT");
   SetTitles(fhNuMuBarCCEventRate,      "#bar{#nu}_{#mu} Energy (GeV)", "#bar{#nu}_{#mu} CC Events / GeV / kTon / 10^{12} POT");
   SetTitles(fhNuECCEventRate,          "#nu_{e} Energy (GeV)", "#nu_{e} CC Events / GeV / kTon / 10^{12} POT");
   SetTitles(fhNuEBarCCEventRate,       "#bar{#nu}_{e} Energy (GeV)", "#bar{#nu}_{e} CC Events / GeV / kTon / 10^{12} POT");

   SetTitles(fhNuMuNCEventRate,         "#nu_{#mu} Energy (GeV)", "#nu_{#mu} NC Events / GeV / kTon / 10^{12} POT");
   SetTitles(fhNuMuBarNCEventRate,      "#bar{#nu}_{#mu} Energy (GeV)","#bar{#nu}_{#mu} NC Events / GeV / kTon / 10^{12} POT");
   SetTitles(fhNuENCEventRate,          "#nu_{e} Energy (GeV)", "#nu_{e} NC Events / GeV / kTon / 10^{12} POT");
   SetTitles(fhNuEBarNCEventRate,       "#bar{#nu}_{e} Energy (GeV)", "#bar{#nu}_{e} NC Events / GeV / kTon / 10^{12} POT");

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
      double detx = 0.0;
      double dety = 0.0;
      double detz = 103649.0; 
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
	 fhNuMuFlux -> Fill(nuenergyatsomedet, fluxwghtsomedet/binwidth);
	 fhNuMuCCEventRate -> Fill(nuenergyatsomedet, cceventratewghtsomedet/binwidth);
	 fhNuMuNCEventRate -> Fill(nuenergyatsomedet, nceventratewghtsomedet/binwidth);
      }
      if(Ntype == 55)
      {
	 fhNuMuBarFlux -> Fill(nuenergyatsomedet, fluxwghtsomedet/binwidth);
	 fhNuMuBarCCEventRate -> Fill(nuenergyatsomedet, cceventratewghtsomedet/binwidth);
	 fhNuMuBarNCEventRate -> Fill(nuenergyatsomedet, nceventratewghtsomedet/binwidth);
      }
      if(Ntype == 53)
      {
	 fhNuEFlux -> Fill(nuenergyatsomedet, fluxwghtsomedet/binwidth);
	 fhNuECCEventRate -> Fill(nuenergyatsomedet, cceventratewghtsomedet/binwidth);
	 fhNuENCEventRate -> Fill(nuenergyatsomedet, nceventratewghtsomedet/binwidth);
      }
      if(Ntype == 52)
      {
	 fhNuEBarFlux -> Fill(nuenergyatsomedet, fluxwghtsomedet/binwidth);
	 fhNuEBarCCEventRate -> Fill(nuenergyatsomedet, cceventratewghtsomedet/binwidth);
	 fhNuEBarNCEventRate -> Fill(nuenergyatsomedet, nceventratewghtsomedet/binwidth);
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
   TCanvas *c11 = new TCanvas("c11","Canvas 11",1);
   TCanvas *c12 = new TCanvas("c12","Canvas 12",1);

   // Style
   TGaxis::SetMaxDigits(2);
   fhNuMuFlux->GetYaxis()->SetTitleOffset(1.4);
   fhNuMuBarFlux->GetYaxis()->SetTitleOffset(1.4);
   fhNuEFlux->GetYaxis()->SetTitleOffset(1.4);
   fhNuEBarFlux->GetYaxis()->SetTitleOffset(1.4);

   fhNuMuCCEventRate->GetYaxis()->SetTitleOffset(1.4);
   fhNuMuBarCCEventRate->GetYaxis()->SetTitleOffset(1.4);
   fhNuECCEventRate->GetYaxis()->SetTitleOffset(1.4);
   fhNuEBarCCEventRate->GetYaxis()->SetTitleOffset(1.4);
 
   fhNuMuNCEventRate->GetYaxis()->SetTitleOffset(1.4);
   fhNuMuBarNCEventRate->GetYaxis()->SetTitleOffset(1.4);
   fhNuENCEventRate->GetYaxis()->SetTitleOffset(1.4);
   fhNuEBarNCEventRate->GetYaxis()->SetTitleOffset(1.4);

   //drawing histograms
   //
   {
      
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
     fhNuMuCCEventRate->Draw();
     cpath1 = ffilename + "_" + fhNuMuCCEventRate->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuMuCCEventRate->GetName() + ".png";
     c5->SaveAs(cpath1.c_str());
     c5->SaveAs(cpath2.c_str());

     c6->cd();
     fhNuMuBarCCEventRate->Draw();
     cpath1 = ffilename + "_" + fhNuMuBarCCEventRate->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuMuBarCCEventRate->GetName() + ".png";
     c6->SaveAs(cpath1.c_str());
     c6->SaveAs(cpath2.c_str());

     c7->cd();
     fhNuECCEventRate->Draw();
     cpath1 = ffilename + "_" + fhNuECCEventRate->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuECCEventRate->GetName() + ".png";
     c7->SaveAs(cpath1.c_str());
     c7->SaveAs(cpath2.c_str());
     
     c8->cd();
     fhNuEBarCCEventRate->Draw();
     cpath1 = ffilename + "_" + fhNuEBarCCEventRate->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuEBarCCEventRate->GetName() + ".png";
     c8->SaveAs(cpath1.c_str());
     c8->SaveAs(cpath2.c_str());      

     c9->cd();
     fhNuMuNCEventRate->Draw();
     cpath1 = ffilename + "_" + fhNuMuNCEventRate->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuMuNCEventRate->GetName() + ".png";
     c9->SaveAs(cpath1.c_str());
     c9->SaveAs(cpath2.c_str());

     c10->cd();
     fhNuMuBarNCEventRate->Draw();
     cpath1 = ffilename + "_" + fhNuMuBarNCEventRate->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuMuBarNCEventRate->GetName() + ".png";
     c10->SaveAs(cpath1.c_str());
     c10->SaveAs(cpath2.c_str());

     c11->cd();
     fhNuENCEventRate->Draw();
     cpath1 = ffilename + "_" + fhNuENCEventRate->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuENCEventRate->GetName() + ".png";
     c11->SaveAs(cpath1.c_str());
     c11->SaveAs(cpath2.c_str());
     
     c12->cd();
     fhNuEBarNCEventRate->Draw();
     cpath1 = ffilename + "_" + fhNuEBarNCEventRate->GetName() + ".eps";
     cpath2 = ffilename + "_" + fhNuEBarNCEventRate->GetName() + ".png";
     c12->SaveAs(cpath1.c_str());
     c12->SaveAs(cpath2.c_str());      

   }

// Save histograms to a root file
   TFile f("flux_histograms.root","recreate");
   fhNuMuFlux->Write();
   fhNuMuBarFlux->Write();
   fhNuEFlux->Write();
   fhNuEBarFlux->Write();
   fhNuMuCCEventRate->Write();
   fhNuMuBarCCEventRate->Write();
   fhNuECCEventRate->Write();
   fhNuEBarCCEventRate->Write();
   fhNuMuNCEventRate->Write();
   fhNuMuBarNCEventRate->Write();
   fhNuENCEventRate->Write();
   fhNuEBarNCEventRate->Write();
   f.Close();
      
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
  double tmpe = (nu_energy/(f_e_arr[1][file_index][current_index]-f_e_arr[0][file_index][current_index]));
  int nenergy = (int) ceil(tmpe+0.5);

  double sig1 = 0.;
  double sig2 = 0.;
  
  if ( nu_energy > f_e_arr[fnlines-2][file_index][current_index] ) return f_xsec_arr[fnlines-1][file_index][current_index]*f_e_arr[fnlines-1][file_index][current_index]*4.09e9;
  
  sig1 = f_xsec_arr[nenergy][file_index][current_index];
  sig2 = f_xsec_arr[nenergy+1][file_index][current_index];
	
    thexsec = sig1 + ((sig2 - sig1)/(f_e_arr[nenergy+1][file_index][current_index]-f_e_arr[nenergy][file_index][current_index]))*(nu_energy - f_e_arr[nenergy][file_index][current_index]);
	thexsec = thexsec * nu_energy;

	return thexsec * 1.076e-11;

//      xseccc = xseccc * 4.09e9
//     10**-38 cm2 * 10**6 kg/kton * 3.8e20 POT/year *1iron/56/1.66e-27kg
//     * 10**-4 m2/cm2 = 4.09e9 conversion factor
//     since table was 26*sigma(nu-proton) + 30*sigma(nu-neutron)
//     
//     10**-38 cm2 * 10**6 kg/kton *1iron/56/1.66e-27kg
//     * 10**-4 m2/cm2 = 1.076e-23 conversion factor

}


void eventRates::ReadXSecsFromFiles() {

  fnbins = 1000;
  
  std::string base = "data/";
   
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

