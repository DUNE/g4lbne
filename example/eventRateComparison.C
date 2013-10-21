#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <cstdlib>

#include <TH1D.h>
#include <TStyle.h>
#include <TFile.h>
#include <TPad.h>
#include <TH1.h>
#include <TF1.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TMath.h>
#include <plotUtils.C>

int axis_title_font_x = 22;
int axis_title_font_y = 32;
double axis_title_size_x = 0.06;
double axis_title_size_y = 0.05;
int axis_lable_font = 12;
double axis_label_size = 0.045;
int ratio_marker = 20;
int ratio_line_width = 1;
int ratio_color = 1;



void eventRateComparison(std::string osc, std::string macro, std::string user, std::string version, std::string current, std::string ndloc, std::string fdloc, std::string plist, std::string descriptor) {

  // inputs:
  // osc: "oscillated" or "unoscillated"
  // macro: list of macros you want to compare (sep by "/")
  // version: list of versions you want to compare (sep by "/")
  // current: list of currents you want to compare (sep by "/")
  // ndloc: list of near detector locations you want to compare (sep by "/")
  // fdloc: list of far detector locations you want to compare (sep by "/")
  // plist: list of physics lists you want to compare (sep by "/")
  // descriptor: list of descriptors of the variations you're comparing (sep by "/")
  // All lists must have the same length
  std::vector<std::string> macros = split(macro,'/');
  std::vector<std::string> users = split(user,'/');
  std::vector<std::string> versions = split(version,'/');
  std::vector<std::string> currents = split(current,'/');
  std::vector<std::string> ndlocs = split(ndloc,'/');
  std::vector<std::string> fdlocs = split(fdloc,'/');
  std::vector<std::string> plists = split(plist,'/');
  std::vector<std::string> descriptors = split(descriptor,'/');

  std::cout<<"user directories "<<users[0]<<std::endl;
  std::cout<<"versions "<<versions[0]<<std::endl;
  std::cout<<"macros "<<macros[0]<<std::endl;
  std::cout<<"currents "<<currents[0]<<std::endl;
  std::cout<<"plists "<<plists[0]<<std::endl;
  std::cout<<"ndlocs "<<ndlocs[0]<<std::endl;
  std::cout<<"fdlocs "<<fdlocs[0]<<std::endl;

  double n_plots = macros.size();
  if(users.size()!=n_plots) {
    std::cout<<"You specified a different number of macros and users"<<std::endl;
    return;
  }
  if(versions.size()!=n_plots) {
    std::cout<<"You specified a different number of versions and macros"<<std::endl;
    return;
  }
  if(currents.size()!=n_plots) {
    std::cout<<"You specified a different number of horn currents and macros"<<std::endl;
    return;
  }
  if(versions.size()!=n_plots) {
    std::cout<<"You specified a different number of g4lbne versions and macros"<<std::endl;
    return;
  }
  if(ndlocs.size()!=n_plots) {
    std::cout<<"You specified a different number of macros and near detector locations"<<std::endl;
    return;
  }
  if(fdlocs.size()!=n_plots) {
    std::cout<<"You specified a different number of macros and far detector locations"<<std::endl;
    return;
  }
  if(descriptors.size()!=n_plots) {
    std::cout<<"You specified a different number of variation descriptors and macros"<<std::endl;
    return;
  }  

  if(n_plots>10) {
    std::cout<<"I can't handle more than 10 variations... only plotting first 10 variatons"<<std::endl;
    n_plots=10;
  }

  std::string current_polarity = "FHC";
  if(currents[0].find("-")!=string::npos) current_polarity = "RHC";

  gStyle->SetTitleOffset(1.3,"X");
  gStyle->SetTitleOffset(1.5,"Y");
  gStyle->SetLabelOffset(0.015,"X");
  gStyle->SetLabelOffset(0.015,"Y");
  gStyle->SetTitleSize(0.06,"X");
  gStyle->SetTitleSize(0.06,"Y");
  gStyle->SetLabelSize(0.06,"X");
  gStyle->SetLabelSize(0.06,"Y");
  gStyle->SetLineWidth(3);
  gStyle->SetMarkerSize(0);
  gStyle->SetOptStat(kFALSE);
  gStyle->SetOptTitle(kFALSE);
  gStyle->SetOptFit(0000);
  gStyle->SetCanvasColor(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetPadTopMargin(0.09);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetPadLeftMargin(0.2);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetFrameLineWidth(2);
  gStyle->SetHistLineWidth(2);


  std::string ndetector = ndlocs[0];
  std::string fdetector = fdlocs[0];

  // open histogram files
  // also open "fastmc" version of files, which have more finely binned histos

  std::vector<TFile*> nf;
  std::vector<TFile*> nf_fastmc;
  std::vector<TFile*> ff;
  std::vector<TFile*> ff_fastmc;


  std::vector<std::string> histo_directory;
  std::vector<std::string> nhisto_file;
  std::vector<std::string> fhisto_file;
  for(int i = 0; i<n_plots; i++) {
    histo_directory.push_back("/lbne/data/users/"+users[i]+"/fluxfiles/g4lbne/"+versions[i]+"/"+plists[i]+"/"+macros[i]+"/"+currents[i]+"/flux/");
    if(user[i]=="beam") {
      histo_directory[i] = "/lbne/data/beam/fluxfiles/g4lbne/"+versions[i]+"/"+plists[i]+"/"+macros[i]+"/"+currents[i]+"/flux/";
    }
    nhisto_file.push_back(histo_directory[i]+"histos_g4lbne_"+versions[i]+"_"+plists[i]+"_"+macros[i]+"_"+currents[i]+"_"+ndlocs[i]);
    fhisto_file.push_back(histo_directory[i]+"histos_g4lbne_"+versions[i]+"_"+plists[i]+"_"+macros[i]+"_"+currents[i]+"_"+fdlocs[i]);
    
    if(versions[i].find("v2")!= std::string::npos) {
      macro_w_current = macros[i]+"_"+current_polarity;
      histo_directory[i] = "/lbne/data/users/"+users[i]+"/fluxfiles/g4lbne/"+versions[i]+"/"+macro_w_current+"/"+plists[i]+"/nubeam-G4PBeam-stdnubeam/flux/";  
      nhisto_file = histo_directory[i]+"histos_g4lbne_"+macro_w_current+"_"+plists[i]+"_"+ndlocs[i];
      fhisto_file = histo_directory[i]+"histos_g4lbne_"+macro_w_current+"_"+plists[i]+"_"+fdlocs[i];
    }

    nf.push_back(new TFile((nhisto_file[i]+".root").c_str()));
    nf_fastmc.push_back(new TFile((nhisto_file[i]+"_fastmc.root").c_str()));
    
    ff.push_back(new TFile((fhisto_file[i]+".root").c_str()));
    ff_fastmc.push_back(new TFile((fhisto_file[i]+"_fastmc.root").c_str()));

  }    

  std::vector< std::vector< std::vector< TH1D* > > > hists;
  std::vector< std::vector< std::vector< TH1D* > > > hists_fastmc;

  std::vector<std::string> histo_names;
  std::string short_osc = "Osc";
  if(osc == "unoscillated") {
    short_osc = "Unosc";
    histo_names.push_back("numu_flux_forplots");
    histo_names.push_back("numubar_flux_forplots");
    histo_names.push_back("nue_flux_forplots");
    histo_names.push_back("nuebar_flux_forplots");
    histo_names.push_back("nutau_flux_forplots");
    histo_names.push_back("nutaubar_flux_forplots");
    histo_names.push_back("numu_cceventrate_forplots");
    histo_names.push_back("numubar_cceventrate_forplots");
    histo_names.push_back("nue_cceventrate_forplots");
    histo_names.push_back("nuebar_cceventrate_forplots");
    histo_names.push_back("nutau_cceventrate_forplots");
    histo_names.push_back("nutaubar_cceventrate_forplots");
    histo_names.push_back("numu_nceventrate_forplots");
    histo_names.push_back("numubar_nceventrate_forplots");
    histo_names.push_back("nue_nceventrate_forplots");
    histo_names.push_back("nuebar_nceventrate_forplots");
    histo_names.push_back("nutau_nceventrate_forplots");
    histo_names.push_back("nutaubar_nceventrate_forplots");
  }
  else {
    histo_names.push_back("numu_fluxosc_forplots");
    histo_names.push_back("numubar_fluxosc_forplots");
    histo_names.push_back("nue_fluxosc_forplots");
    histo_names.push_back("nuebar_fluxosc_forplots");
    histo_names.push_back("nutau_fluxosc_forplots");
    histo_names.push_back("nutaubar_fluxosc_forplots");
    histo_names.push_back("numu_cceventrateosc_forplots");
    histo_names.push_back("numubar_cceventrateosc_forplots");
    histo_names.push_back("nue_cceventrateosc_forplots");
    histo_names.push_back("nuebar_cceventrateosc_forplots");
    histo_names.push_back("nutau_cceventrateosc_forplots");
    histo_names.push_back("nutaubar_cceventrateosc_forplots");
    histo_names.push_back("numu_nceventrateosc_forplots");
    histo_names.push_back("numubar_nceventrateosc_forplots");
    histo_names.push_back("nue_nceventrateosc_forplots");
    histo_names.push_back("nuebar_nceventrateosc_forplots");
    histo_names.push_back("nutau_nceventrateosc_forplots");
    histo_names.push_back("nutaubar_nceventrateosc_forplots");
  }

  for(int loc = 0; loc<3; loc++) {
    std::vector< std::vector< TH1D*> > blah1;
    std::vector< std::vector< TH1D*> > blah2;

    hists.push_back(blah1);
    hists_fastmc.push_back(blah2);
    for(int var = 0; var<n_plots; var++) {
      std::vector<TH1D*> blah3;
      std::vector<TH1D*> blah4;
      hists[loc].push_back(blah3);
      hists_fastmc[loc].push_back(blah4);
      for(int i = 0; i<18; i++) {
	if(loc==0) {
	  hists[loc][var].push_back((TH1D*)nf[var]->Get(histo_names[i].c_str()));
	  
	  std::string fastmc_name = histo_names[i];
	  stringReplaceAll(fastmc_name,"_forplots","");
	  hists_fastmc[loc][var].push_back((TH1D*)nf_fastmc[var]->Get(fastmc_name.c_str()));
	}
	if(loc==1) {
	  hists[loc][var].push_back((TH1D*)ff[var]->Get(histo_names[i].c_str()));
	  
	  std::string fastmc_name = histo_names[i];
	  stringReplaceAll(fastmc_name,"_forplots","");
	      hists_fastmc[loc][var].push_back((TH1D*)ff_fastmc[var]->Get(fastmc_name.c_str()));
	}
	if(loc==2) {
	  string blah = histo_names[i];
	  TH1D *temp = (TH1D*)hists[0][var][i]->Clone((blah+"_nf").c_str());
	  hists[loc][var].push_back(temp);
	  hists[loc][var][i]->Divide(hists[1][var][i]);	  
	  std::string fastmc_name = histo_names[i];
	  stringReplaceAll(fastmc_name,"_forplots","");
	  hists_fastmc[loc][var].push_back((TH1D*)(hists_fastmc[0][var][i]->Clone((fastmc_name+"_nf").c_str())));
	  hists_fastmc[loc][var][i]->Divide(hists_fastmc[1][var][i]);
	}
	
      }
    }
  }
  
  TLegend *leg = new TLegend(0.4,0.6,0.85,0.8);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  for(int var = 0; var<n_plots; var++) 
    leg->AddEntry(hists[0][var][0],descriptors[var].c_str(),"l");

  TLegend *rleg = new TLegend(0.35,0.66,0.8,0.85);
  rleg->SetFillStyle(0);
  rleg->SetBorderSize(0);
  for(int var = 1; var<n_plots; var++) 
    rleg->AddEntry(hists[0][var][0],descriptors[var].c_str(),"l");

  TCanvas *c1 = new TCanvas("flux","flux",2000,1200);
  c1->Divide(3,2);
  
  TCanvas *c2 = new TCanvas("cceventrate","cceventrate",2000,1200);
  c2->Divide(3,2);
  
  TCanvas *c3 = new TCanvas("nceventrate","nceventrate",2000,1200);
  c3->Divide(3,2);
  
  TCanvas *c4 = new TCanvas("flux_ratio","flux_ratio",2000,1200);
  c4->Divide(3,2);
  
  TCanvas *c5 = new TCanvas("cceventrate_ratio","cceventrate_ratio",2000,1200);
  c5->Divide(3,2);
 
  TCanvas *c6 = new TCanvas("nceventrate_ratio","nceventrate_ratio",2000,1200);
  c6->Divide(3,2);

  for(int j = 0; j<3; j++) {
 
   std::string loc_string = "near";
   if(j==1) loc_string = "far";
   if(j==2) loc_string = "noverf";

   std::vector<std::string> nu_names;
   nu_names.push_back("#nu_{#mu}");
   nu_names.push_back("#bar{#nu}_{#mu}");
   nu_names.push_back("#nu_{e}");
   nu_names.push_back("#bar{#nu}_{e}");
   nu_names.push_back("#nu_{#tau}");
   nu_names.push_back("#bar{#nu}_{#tau}");
   
   std::vector<std::string> nu_names_simple;
   nu_names_simple.push_back("numu");
   nu_names_simple.push_back("numubar");
   nu_names_simple.push_back("nue");
   nu_names_simple.push_back("nuebar");
   nu_names_simple.push_back("nutau");
   nu_names_simple.push_back("nutaubar");
 
   for(int var = 0; var<n_plots; var++)
     for(int i = 0; i<18; i++) {
       hists[j][var][i]->UseCurrentStyle();
     }
 
   int colors[10] = {1,2,4,6,8,3,5,7,9,30};

   for(int i = 0; i<6; i++) {

     for(int var = 0; var < n_plots; var++) {

       c1->cd(i+1);

       if(j==2) {
	 std::string ytitle = short_osc+" "+nu_names[i]+" Near/Far Flux";
	 hists[j][var][i]->GetYaxis()->SetTitle(ytitle.c_str());
       }
       if( hists[j][var][i]->GetSumw2N() == 0 )
	 hists[j][var][i]->Sumw2();
       hists[j][var][i]->SetLineWidth(3);
       //hists[j][var][i]->SetMinimum(0);
       hists[j][var][i]->SetLineColor(colors[var%10]);
       if(var==0)
	 hists[j][var][i]->Draw("histe");
       else
	 hists[j][var][i]->Draw("histsame");
       
       if ((current_polarity=="FHC" && i==0) || (current_polarity=="RHC" && i==1))
	 leg->Draw();
       
       
       c4->cd(i+1);
       std::string ytitle = short_osc+" "+nu_names[i]+"s Ratio to "+descriptors[0];
       if(j==2) 
	 ytitle = short_osc+" "+nu_names[i]+" Flux N/F Ratio to Nominal";

       if(var==1) {
	 drawRatio( hists[j][1][i], hists[j][0][i],ytitle);
       }
       if(var>1) {
	 drawRatio( hists[j][var][i], hists[j][0][i],ytitle,"same");
       }
       if ((current_polarity=="FHC" && i==0) || (current_polarity=="RHC" && i==1))
	 rleg->Draw();
     }
   }
  
   double n_kton = 34;
   if(loc_string=="nd") n_kton = 0.018;
  
   for(int i = 0; i<6; i++) {

     std::string title = short_osc + " " + nu_names[i]+" CC Events / GeV";
   
     for(int var = 0; var<n_plots; var++) {
       hists[j][var][i+6]->GetYaxis()->SetTitle(title.c_str());
       hists[j][var][i+6]->Scale(3.25e21*n_kton);
    
       for(int k = 0; k<hists[j][var][i+6]->GetNbinsX(); k++) {
	 hists[j][var][i+6]->SetBinError(k+1,TMath::Sqrt(hists[j][var][i+6]->GetBinContent(k+1)));
       }
       hists[j][var][i+6]->SetLineWidth(3);
       hists[j][var][i+6]->SetLineColor(colors[var%10]);

       c2->cd(i+1);
       if(j==2) {
	 std::string ytitle = short_osc+" "+nu_names[i]+" Near/Far CC Rate";
	 hists[j][var][i+6]->GetYaxis()->SetTitle(ytitle.c_str());
       }
       if(var==0) {
	 hists[j][var][i+6]->Draw("histe");
       }
       else {
	 hists[j][var][i+6]->Draw("histesame");
       }
       if ((current_polarity=="FHC" && i==0) || (current_polarity=="RHC" && i==1))
	 leg->Draw();
    
       if(loc==0)
	 add_plot_label("18 Ton #bullet 3.25#times10^{21} POT",0.7,0.94,.05,38);          
       else
       	 add_plot_label("34 kTon #bullet 3.25#times10^{21} POT",0.7,0.94,.05,38);          
       
       c5->cd(i+1);
       std::string ratio_ytitle = short_osc+" "+nu_names[i]+" CC Ratio to "+descriptors[0];
       if(j==2)
	 ratio_ytitle = short_osc+" "+nu_names[i]+" CC N/F Ratio to Nominal";
       if(var==1)
	 drawRatio( hists[j][var][i+6], hists[j][0][i+6],ratio_ytitle);
       else if(var>1)
	 drawRatio( hists[j][var][i+6], hists[j][0][i+6],ratio_ytitle,"same");
       if ((current_polarity=="FHC" && i==0) || (current_polarity=="RHC" && i==1))
	 rleg->Draw();
     }
   }


   
   for(int i = 0; i<6; i++) {

     std::string title = short_osc + " " + nu_names[i]+" NC Events / GeV";
     
     for(int var = 0; var<n_plots; var++) {

       hists[j][var][i+12]->GetYaxis()->SetTitle(title.c_str());
       hists[j][var][i+12]->Scale(3.25e21*n_kton);

       for(int k = 0; k< hists[j][var][i+12]->GetNbinsX(); k++) {
	 hists[j][var][i+12]->SetBinError(k+1,TMath::Sqrt(hists[j][var][i+12]->GetBinContent(k+1)));
       }
       hists[j][var][i+12]->SetLineColor(colors[var%10]);
       hists[j][var][i+12]->SetLineWidth(3);
       c3->cd(i+1);
       if(j==2) {
	 std::string ytitle = short_osc+" "+nu_names[i]+" Near/Far NC Rate";
	 hists[j][var][i+12]->GetYaxis()->SetTitle(ytitle.c_str());
       }
       if(var==0)
	 hists[j][var][i+12]->Draw("histe");
       else
	 hists[j][var][i+12]->Draw("histesame");
       if ((current_polarity=="FHC" && i==0) || (current_polarity=="RHC" && i==1))
	 leg->Draw();
       
       if(loc==0)
	 add_plot_label("18 Ton #bullet 3.25#times10^{21} POT",0.7,0.94,.05,38);          
       else
       	 add_plot_label("34 kTon #bullet 3.25#times10^{21} POT",0.7,0.94,.05,38);          

       c6->cd(i+1);
       std::string ratio_ytitle = short_osc+" "+nu_names[i]+" NC Ratio to "+descriptors[0];
       if(j==2)
	 ratio_ytitle = short_osc+" "+nu_names[i]+" CC N/F Ratio to Nominal";
       if(var==1)
	 drawRatio( hists[j][var][i+12], hists[j][0][i+12],ratio_ytitle);
       else if(var>1)
	 drawRatio( hists[j][var][i+12], hists[j][0][i+12],ratio_ytitle,"same");
       if ((current_polarity=="FHC" && i==0) || (current_polarity=="RHC" && i==1))
	 rleg->Draw();
     
   }
   }

   std::string filename = descriptors[0];
   for(int i = 1; i<n_plots; i++) {
     filename += "_"+descriptors[i];
   }
  
   for(int var = 0; var<n_plots; var++) {
     c1->Print((histo_directory[var]+"comp_flux_"+current_polarity+"_"+osc+"_"+filename+"_"+loc_string+".eps").c_str());
     c1->Print((histo_directory[var]+"comp_flux_"+current_polarity+"_"+osc+"_"+filename+"_"+loc_string+".png").c_str());
   
     c2->Print((histo_directory[var]+"comp_cceventrate_"+current_polarity+"_"+osc+"_"+filename+"_"+loc_string+".eps").c_str());
     c2->Print((histo_directory[var]+"comp_cceventrate_"+current_polarity+"_"+osc+"_"+filename+"_"+loc_string+".png").c_str());
     
     c3->Print((histo_directory[var]+"comp_nceventrate_"+current_polarity+"_"+osc+"_"+filename+"_"+loc_string+".eps").c_str());
     c3->Print((histo_directory[var]+"comp_nceventrate_"+current_polarity+"_"+osc+"_"+filename+"_"+loc_string+".png").c_str());
     
     if(n_plots>1) {
       c4->Print((histo_directory[var]+"comp_flux_ratios_"+current_polarity+"_"+osc+"_"+filename+"_"+loc_string+".eps").c_str());
       c4->Print((histo_directory[var]+"comp_flux_ratios_"+current_polarity+"_"+osc+"_"+filename+"_"+loc_string+".png").c_str());
       
       c5->Print((histo_directory[var]+"comp_cceventrate_ratios_"+current_polarity+"_"+osc+"_"+filename+"_"+loc_string+".eps").c_str());
       c5->Print((histo_directory[var]+"comp_cceventrate_ratios_"+current_polarity+"_"+osc+"_"+filename+"_"+loc_string+".png").c_str());
       
       c6->Print((histo_directory[var]+"comp_nceventrate_ratios_"+current_polarity+"_"+osc+"_"+filename+"_"+loc_string+".eps").c_str());
       c6->Print((histo_directory[var]+"comp_nceventrate_ratios_"+current_polarity+"_"+osc+"_"+filename+"_"+loc_string+".png").c_str());
     }   

   
     // integrate over several energy ranges and make a text file
     if(i>0 && n_plots>1) {
       int bin0 = -1;
       int bin0point5 = -1;
       int bin2 = -1;
       int bin5 = -1;
       int bin20 = -1;
       int bin120 = -1; 

       for(int bin = 0 ; bin < hists_fastmc[j][0][0]->GetNbinsX()+1; bin++) {
	 if(hists_fastmc[j][0][0]->GetBinLowEdge(bin)==0)
	   bin0=bin; 
       if(hists_fastmc[j][0][0]->GetBinLowEdge(bin)==0.5)
	 bin0point5=i; 
       if(hists_fastmc[j][0][0]->GetBinLowEdge(bin)==2)
	 bin2=bin; 
       if(hists_fastmc[j][0][0]->GetBinLowEdge(bin)==5)
	 bin5=bin; 
       if(hists_fastmc[j][0][0]->GetBinLowEdge(bin)==20)
	 bin20=bin;
       if(hists_fastmc[j][0][0]->GetBinLowEdge(bin)==120)
	 bin120=bin;
     }

     if(bin0==-1)
       std::cout<<"ERROR: 0 is not a bin edge."<<std::endl;
     if(bin0point5==-1)
       std::cout<<"ERROR: 0.5 is not a bin edge."<<std::endl;
     if(bin2==-1)
       std::cout<<"ERROR: 2 is not a bin edge."<<std::endl;
     if(bin5==-1)
       std::cout<<"ERROR: 5 is not a bin edge."<<std::endl;
     if(bin20==-1)
       std::cout<<"ERROR: 20 is not a bin edge."<<std::endl;
     if(bin120==-1)
       std::cout<<"ERROR: 120 is not a bin edge."<<std::endl;

     double ratio_0to0point5[18];
     double ratio_0point5to2[18];
     double ratio_2to5[18];
     double ratio_5to20[18];
     double ratio_20to120[18];
     double ratio_0to20[18];
     double ratio_0to120[18];
     
     
     for(int i=0; i<18; i++) {
	

       if(hists_fastmc[j][0][i]->Integral(bin0,bin0point5-1)!=0)
	 ratio_0to0point5[i] = hists_fastmc[j][var][i]->Integral(bin0,bin0point5-1)/
	   hists_fastmc[j][0][i]->Integral(bin0,bin0point5-1);
       else
	 ratio_0to0point5[i] = 1;
      
       if(hists_fastmc[j][0][i]->Integral(bin0point5,bin2-1)!=0)
	 ratio_0point5to2[i] = hists_fastmc[j][var][i]->Integral(bin0point5,bin2-1)/
	   hists_fastmc[j][0][i]->Integral(bin0point5,bin2-1);
       else
	 ratio_0point5to2[i] = 1;
       
       if(hists_fastmc[j][0][i]->Integral(bin2,bin5-1)!=0)
	 ratio_2to5[i] = hists_fastmc[j][var][i]->Integral(bin2,bin5-1)/
	   hists_fastmc[j][0][i]->Integral(bin2,bin5-1);
       else
	 ratio_2to5[i] = 1;
       
       if(hists_fastmc[j][0][i]->Integral(bin5,bin20-1)!=0)
	 ratio_5to20[i] = hists_fastmc[j][var][i]->Integral(bin5,bin20-1)/
	   hists_fastmc[j][0][i]->Integral(bin5,bin20-1);
       else
	 ratio_5to20[i] = 1;
       
       //if(hists_fastmc[j][0][i]->Integral(bin20,bin120-1)!=0)
//	 ratio_20to120[i] = hists_fastmc[j][var][i]->Integral(bin20,bin120-1)/
//	   hists_fastmc[j][0][i]->Integral(bin20,bin120-1);
 //      else
//	 ratio_20to120[i] = 1;
       
       if(hists_fastmc[j][0][i]->Integral(bin0,bin20-1)!=0)
	 ratio_0to20[i] = hists_fastmc[j][var][i]->Integral(bin0,bin20-1)/
	   hists_fastmc[j][0][i]->Integral(bin0,bin20-1);
       else
	 ratio_0to20[i] = 1;
       
       //if(hists_fastmc[j][0][i]->Integral(bin0,bin120-1)!=0)
//	 ratio_0to120[i] = hists_fastmc[j][var][i]->Integral(bin0,bin120-1)/
//	   hists_fastmc[j][0][i]->Integral(bin0,bin120-1);
 //      else
//	 ratio_0to120[i] = 1;
     }

     ofstream myfile;
     myfile.open((histo_directory[var]+"comp_flux_summary_"+current_polarity+"_"+osc+"_"+filename+"_"+loc_string+".txt").c_str());
     myfile << "                0-0.5   0.5-2  2-5      5-20    20-120  0-20     0-120"<<std::endl;
     myfile<<std::setprecision(4)<<fixed;
     for(int i = 0; i<6; i++) {
       myfile << setw(15)<<nu_names_simple[i]+" flux"
	      <<setw(7)<<ratio_0to0point5[i]<<" "
	      <<setw(7)<<ratio_0point5to2[i]<<" "
	      <<setw(7)<<ratio_2to5[i]<<" "
	      <<setw(7)<<ratio_5to20[i]<<" "
	      <<setw(7)<<ratio_20to120[i]<<" "
	      <<setw(7)<<ratio_0to20[i]<<" "
	      <<setw(7)<<ratio_0to120[i]<<std::endl;
     }

     myfile.close();
     } 
   }
  }
}
