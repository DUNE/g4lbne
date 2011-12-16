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

   //
   //define the Reference pot
   //
   double refpot   = 1.0e12;
   std::string potref_str = Bma::GetPOTAsString(refpot);

   //
   //make histograms
   //
   int nbins   = 120;
   double xmin = 0.0;
   double xmax = 60.0;
   double binwidth = (xmax-xmin)/(double)nbins;

   TH1F *fhTotalFluxNear   = new TH1F("totalfluxnear"  ,"totalfluxnear"  ,nbins,xmin,xmax);
   TH1F *fhNuMuFluxNear    = new TH1F("numufluxnear"   ,"numufluxnear"   ,nbins,xmin,xmax);
   TH1F *fhNuMuBarFluxNear = new TH1F("numubarfluxnear","numubarfluxnear",nbins,xmin,xmax);
   TH1F *fhNuEFluxNear     = new TH1F("nuefluxnear"    ,"nuefluxnear"    ,nbins,xmin,xmax);
   TH1F *fhNuEBarFluxNear  = new TH1F("nuebarfluxnear" ,"nuebarfluxnear" ,nbins,xmin,xmax);

   std::string fluxtitle      = "Neutrinos/GeV/m^{2}/"     + potref_str + "POT";

   SetTitles(fhTotalFluxNear,   "Neutrino Energy (GeV)", fluxtitle);
   SetTitles(fhNuMuFluxNear,    "Neutrino Energy (GeV)", fluxtitle);
   SetTitles(fhNuMuBarFluxNear, "Neutrino Energy (GeV)", fluxtitle);
   SetTitles(fhNuEFluxNear,     "Neutrino Energy (GeV)", fluxtitle);
   SetTitles(fhNuEBarFluxNear,  "Neutrino Energy (GeV)", fluxtitle);


   //
   //start loop
   //
   Long64_t nentries = fChain->GetEntries();
   std::cout << "nentries = " << nentries << std::endl;

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) 
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      //
      //Note [0] is the index for the LBNE (in this case near) detector
      //THe location of the detector is set in the
      //geometry input card that is used to produce 
      //the flux file
      //
      double fluxwghtnear = (NWtNear[0]*Nimpwt/3.1415)*(refpot/fTotalPOT);

      fhTotalFluxNear->Fill(NenergyN[0], fluxwghtnear/binwidth);

      //
      //by neutrino type
      //
      if(Ntype == 56)
      {
	 fhNuMuFluxNear   ->Fill(NenergyN[0], fluxwghtnear/binwidth);
      }
      if(Ntype == 55)
      {
	 fhNuMuBarFluxNear->Fill(NenergyN[0], fluxwghtnear/binwidth);
      }
      if(Ntype == 53)
      {
	 fhNuEFluxNear    ->Fill(NenergyN[0], fluxwghtnear/binwidth);
      }
      if(Ntype == 52)
      {
	 fhNuEBarFluxNear ->Fill(NenergyN[0], fluxwghtnear/binwidth);
      }


   }//end loop over entries


   //
   //draw and save histograms
   //

   std::string cpath1;
   std::string cpath2;


   fhTotalFluxNear->Draw();
   cpath1 = ffilename + "_" + fhTotalFluxNear->GetName() + ".eps";
   cpath2 = ffilename + "_" + fhTotalFluxNear->GetName() + ".png";
   c1 -> Modified();
   c1 -> Update();
   c1 -> SaveAs(cpath1.c_str());
   c1 -> SaveAs(cpath2.c_str());

   c1 -> WaitPrimitive();

   fhNuMuFluxNear->Draw();
   cpath1 = ffilename + "_" + fhNuMuFluxNear->GetName() + ".eps";
   cpath2 = ffilename + "_" + fhNuMuFluxNear->GetName() + ".png";
   c1 -> Modified();
   c1 -> Update();
   c1 -> SaveAs(cpath1.c_str());
   c1 -> SaveAs(cpath2.c_str());

   c1 -> WaitPrimitive();

   fhNuMuBarFluxNear->Draw();
   cpath1 = ffilename + "_" + fhNuMuBarFluxNear->GetName() + ".eps";
   cpath2 = ffilename + "_" + fhNuMuBarFluxNear->GetName() + ".png";
   c1 -> Modified();
   c1 -> Update();
   c1 -> SaveAs(cpath1.c_str());
   c1 -> SaveAs(cpath2.c_str());

   c1 -> WaitPrimitive();

   fhNuEFluxNear->Draw();
   cpath1 = ffilename + "_" + fhNuEFluxNear->GetName() + ".eps";
   cpath2 = ffilename + "_" + fhNuEFluxNear->GetName() + ".png";
   c1 -> Modified();
   c1 -> Update();
   c1 -> SaveAs(cpath1.c_str());
   c1 -> SaveAs(cpath2.c_str());

   c1 -> WaitPrimitive();

   fhNuEBarFluxNear->Draw();
   cpath1 = ffilename + "_" + fhNuEBarFluxNear->GetName() + ".eps";
   cpath2 = ffilename + "_" + fhNuEBarFluxNear->GetName() + ".png";
   c1 -> Modified();
   c1 -> Update();
   c1 -> SaveAs(cpath1.c_str());
   c1 -> SaveAs(cpath2.c_str());

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
      cout << "BeamUtil::GetPOTAsString - PROBLEM: pot is not in scientific notation" << endl;
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
      cout << "BeamUtil::GetPOTAsString - PROBLEM: pot is not in scientific notation" << endl;
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
      cout << "BeamUtil::GetPOTAsString - PROBLEM: base number and exp number are both empty" << endl;
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
