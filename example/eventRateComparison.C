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

int axis_title_font_x = 22;
int axis_title_font_y = 32;
double axis_title_size_x = 0.06;
double axis_title_size_y = 0.05;
int axis_lable_font = 12;
double axis_label_size = 0.045;
int ratio_marker = 20;
int ratio_line_width = 1;
int ratio_color = 1;

void stringReplaceAll(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

void drawRatio( TH1* hist1, TH1 *hist2, std::string ytitle){

  TH1* tmp1 = (TH1*)hist1->Clone("tmp1");
  TH1* tmp2 = (TH1*)hist2->Clone("tmp2");

  if( tmp1->GetSumw2N() == 0 ) tmp1->Sumw2();
  if( tmp2->GetSumw2N() == 0 )   tmp2->Sumw2();

  TH1 *ratio = (TH1*)tmp2->Clone("ratio");
  ratio->Divide(tmp1, tmp2);

  for(int i = 0; i<tmp2->GetNbinsX(); i++) {
    // use central value fractional errors for ratio errors
    if(hist2->GetBinContent(i+1) !=0)
      ratio->SetBinError(i+1,ratio->GetBinContent(i+1)*hist2->GetBinError(i+1)/hist2->GetBinContent(i+1));
    else
      ratio->SetBinError(i+1,0);
  }

  ytitle[0]=toupper(ytitle[0]);
  ratio->GetYaxis()->SetTitle((ytitle).c_str());
  //ratio->GetXaxis()->SetTitleSize(axis_title_size_x);
  //ratio->GetYaxis()->SetTitleSize(axis_title_size_y);
  //ratio->GetXaxis()->SetLabelFont(axis_label_font);
  //ratio->GetYaxis()->SetLabelFont(axis_label_font);
  //ratio->GetXaxis()->SetLabelSize(axis_label_size);
  //ratio->GetYaxis()->SetLabelSize(axis_label_size);
  ratio->GetXaxis()->CenterTitle(kTRUE);
  ratio->SetMarkerStyle(ratio_marker);
  //ratio->SetLineWidth(ratio_line_width);
  //ratio->SetLineColor(ratio_color);

  ratio->DrawCopy("X0");
  
  {
    double lowX = ratio->GetBinLowEdge(1);
    double highX = ratio->GetBinLowEdge(ratio->GetNbinsX() ) + ratio->GetBinWidth(ratio->GetNbinsX() );
    TF1 *line = new TF1("oneLine","1.0", lowX, highX);
    line->SetLineStyle(2);
    line->SetLineWidth(3);
    line->SetLineColor(3);
    line->DrawCopy("same");
    delete line;
  }

  gPad->Update();

  delete tmp1;
  delete tmp2;
  delete ratio;
}


void eventRateComparison(std::string osc, std::string sim1, std::string sim2, std::string version1, std::string version2, std::string input1, std::string input2, std::string loc1, std::string loc2, std::string plist1, std::string plist2,std::string descriptor1, std::string descriptor2) {

  // inputs:
  // osc: "oscillated" or "unoscillated"
  // sim1/sim2: simulations of the two samples you want to compare (usually G4PBeam)
  // version1/version2: g4lbne versions you want to compare
  // input1/input2: name of input cards you want to compare
  // loc1/loc2: name of locations you want to compare
  // plist1/plist2: physics lists of samples you want to compare
  // descriptor1/descriptor2: label that will be used in plot legends for the two samples

  gStyle->SetTitleOffset(1.3,"X");
  gStyle->SetTitleOffset(1.3,"Y");
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
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadRightMargin(0.15);
  gStyle->SetFrameLineWidth(2);
  gStyle->SetHistLineWidth(2);


  std::string detector = loc1;

  std::string username(getenv("USER"));

  // open histogram files
  // also open "fastmc" version of files, which have more finely binned histos

  TFile *f1, *f1_fastmc, *f2, *f2_fastmc;

  f1 = new TFile(("/lbne/data/users/"+username+"/fluxfiles/g4lbne/"+version1+"/"+input1+"/"+plist1+"/nubeam-"+sim1+"-stdnubeam/flux/histos_g4lbne_"+input1+"_"+plist1+"_"+loc1+".root").c_str());
  f1_fastmc = new TFile(("/lbne/data/users/"+username+"/fluxfiles/g4lbne/"+version1+"/"+input1+"/"+plist1+"/nubeam-"+sim1+"-stdnubeam/flux/histos_g4lbne_"+input1+"_"+plist1+"_"+loc1+"_fastmc.root").c_str());

  f2 = new TFile(("/lbne/data/users/"+username+"/fluxfiles/g4lbne/"+version2+"/"+input2+"/"+plist2+"/nubeam-"+sim2+"-stdnubeam/flux/histos_g4lbne_"+input2+"_"+plist2+"_"+loc2+".root").c_str());
  f2_fastmc = new TFile(("/lbne/data/users/"+username+"/fluxfiles/g4lbne/"+version2+"/"+input2+"/"+plist2+"/nubeam-"+sim2+"-stdnubeam/flux/histos_g4lbne_"+input2+"_"+plist2+"_"+loc2+"_fastmc.root").c_str());

  std::vector< TH1D* > h1;
  std::vector< TH1D* > h2;
  std::vector< TH1D* > h1_fastmc;
  std::vector< TH1D* > h2_fastmc;

  std::vector<std::string> histo_names;
  if(osc == "unoscillated") {
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

  for(int i = 0; i<18; i++) {
    h1.push_back((TH1D*)f1->Get(histo_names[i].c_str()));
    h2.push_back((TH1D*)f2->Get(histo_names[i].c_str()));

    std::string fastmc_name = histo_names[i];
    stringReplaceAll(fastmc_name,"_forplots","");
    h1_fastmc.push_back((TH1D*)f1_fastmc->Get(fastmc_name.c_str()));
    h2_fastmc.push_back((TH1D*)f2_fastmc->Get(fastmc_name.c_str()));
  }

  cout<<f1_fastmc->GetName()<<endl;
  cout<<f2_fastmc->GetName()<<endl;

 TLegend *leg = new TLegend(0.55,0.65,0.8,0.8);
 leg->SetFillColor(0);
 leg->SetBorderSize(0);
 leg->AddEntry(h1[0],descriptor1.c_str(),"l");
 leg->AddEntry(h2[0],descriptor2.c_str(),"l");

 TCanvas *c1 = new TCanvas("flux","flux",1500,900);
 c1->Divide(3,2);

 TCanvas *c2 = new TCanvas("cceventrate","cceventrate",1500,900);
  c2->Divide(3,2);

  TCanvas *c3 = new TCanvas("nceventrate","nceventrate",1500,900);
  c3->Divide(3,2);

  TCanvas *c4 = new TCanvas("flux_ratio","flux_ratio",1500,900);
  c4->Divide(3,2);

  TCanvas *c5 = new TCanvas("cceventrate_ratio","cceventrate_ratio",1500,900);
  c5->Divide(3,2);

  TCanvas *c6 = new TCanvas("nceventrate_ratio","nceventrate_ratio",1500,900);
  c6->Divide(3,2);

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


  for(int i = 0; i<18; i++) {
    h1[i]->UseCurrentStyle();
    h2[i]->UseCurrentStyle();
  }
  
  for(int i = 0; i<6; i++) {
    c1->cd(i+1);
    h1[i]->Sumw2();
    h1[i]->SetLineWidth(3);
    h1[i]->SetMaximum(TMath::Max(h1[i]->GetMaximum(),h2[i]->GetMaximum())*1.1);
    h1[i]->Draw("histe");
    h2[i]->SetLineColor(2);
    h2[i]->Draw("histsame");
    leg->Draw();
    
    c4->cd(i+1);
    std::string ytitle = osc+" "+nu_names[i]+"s  "+descriptor2+" / "+descriptor1;
    drawRatio( h2[i], h1[i],ytitle);

  }

  double n_kton = 34;
  if(detector == "LBNEND") n_kton = 0.018;

  for(int i = 0; i<6; i++) {
    c2->cd(i+1);
    //gPad->SetLogx(1);
    //h1[i+6]->GetXaxis()->SetRange(2,40);
    std::string title = osc + " " + nu_names[i]+" CC Events / GeV / 34 kTon / 3.25#times10^{21} POT";
    if(detector=="LBNEND")
      title = nu_names[i]+"ND CC Events / GeV / 18 ton / 3.25#times10^{21} POT";
    
    h1[i+6]->GetYaxis()->SetTitle(title.c_str());
    h1[i+6]->Scale(3.25e21*n_kton);
    h2[i+6]->Scale(3.25e21*n_kton);
    for(int j = 0; j< h1[i+6]->GetNbinsX(); j++) {
      h1[i+6]->SetBinError(j+1,TMath::Sqrt(h1[i+6]->GetBinContent(j+1)));
    }
    h1[i]->SetLineWidth(3);
    h1[i]->SetMaximum(TMath::Max(h1[i]->GetMaximum(),h2[i]->GetMaximum())*1.1);
    h1[i+6]->Draw("histe");
    h2[i+6]->SetLineColor(2);
    h2[i+6]->Draw("histsame");
    leg->Draw();

    c5->cd(i+1);
    std::string ratio_ytitle = osc+" "+nu_names[i]+" CC events  "+descriptor2+" / "+descriptor1;
    drawRatio( h2[i+6], h1[i+6],ratio_ytitle);
    
  }

  for(int i = 0; i<6; i++) {
    c3->cd(i+1);
    h1[i+12]->Scale(3.25e21*n_kton);
    h2[i+12]->Scale(3.25e21*n_kton);
    std::string title = osc + " " + nu_names[i]+" NC Events / GeV / 34 kTon / 3.25#times10^{21} POT";
    if(detector=="LBNEND")
      title = nu_names[i]+" ND NC Events / GeV / 18 ton / 3.25#times10^{21} POT";
    h1[i+12]->GetXaxis()->SetTitle(title.c_str());
    for(int j = 0; j< h1[i+6]->GetNbinsX(); j++) {
      h1[i+12]->SetBinError(j+1,TMath::Sqrt(h1[i+6]->GetBinContent(j+1)));
    }
    h1[i]->SetLineWidth(3);
    h1[i]->SetMaximum(TMath::Max(h1[i]->GetMaximum(),h2[i]->GetMaximum())*1.1);
    h1[i+12]->Draw("histe");
    h2[i+12]->SetLineColor(2);
    h2[i+12]->Draw("histsame");
    leg->Draw();

    c6->cd(i+1);
    std::string ratio_ytitle = osc+" "+nu_names[i]+" NC events  "+descriptor2+" / "+descriptor1;
    drawRatio( h2[i+12], h1[i+12],ratio_ytitle);
  }

  std::string variation = descriptor1+"_vs_"+descriptor2;

  system(("mkdir -p eventRateComparisons/"+variation).c_str());

  c1->Print(("eventRateComparisons/"+variation+"/flux_"+variation+"_"+osc+".eps").c_str());
  c1->Print(("eventRateComparisons/"+variation+"/flux_"+variation+"_"+osc+".png").c_str());

  c2->Print(("eventRateComparisons/"+variation+"/cceventrate_"+variation+"_"+osc+".eps").c_str());
  c2->Print(("eventRateComparisons/"+variation+"/cceventrate_"+variation+"_"+osc+".png").c_str());

  c3->Print(("eventRateComparisons/"+variation+"/nceventrate_"+variation+"_"+osc+".eps").c_str());
  c3->Print(("eventRateComparisons/"+variation+"/nceventrate_"+variation+"_"+osc+".png").c_str());

  c4->Print(("eventRateComparisons/"+variation+"/flux_ratios_"+variation+"_"+osc+".eps").c_str());
  c4->Print(("eventRateComparisons/"+variation+"/flux_ratios_"+variation+"_"+osc+".png").c_str());

  c5->Print(("eventRateComparisons/"+variation+"/cceventrate_ratios_"+variation+"_"+osc+".eps").c_str());
  c5->Print(("eventRateComparisons/"+variation+"/cceventrate_ratios_"+variation+"_"+osc+".png").c_str());

  c6->Print(("eventRateComparisons/"+variation+"/nceventrate_ratios_"+variation+"_"+osc+".eps").c_str());
  c6->Print(("eventRateComparisons/"+variation+"/nceventrate_ratios_"+variation+"_"+osc+".png").c_str());

  // integrate over several energy ranges and make a text file
  int bin0 = -1;
  int bin0point5 = -1;
  int bin2 = -1;
  int bin5 = -1;
  int bin20 = -1;
  int bin120 = -1; 

  for(int i = 0 ; i < h1_fastmc[0]->GetNbinsX()+1; i++) {
    if(h1_fastmc[0]->GetBinLowEdge(i)==0)
      bin0=i; 
    if(h1_fastmc[0]->GetBinLowEdge(i)==0.5)
      bin0point5=i; 
    if(h1_fastmc[0]->GetBinLowEdge(i)==2)
      bin2=i; 
    if(h1_fastmc[0]->GetBinLowEdge(i)==5)
      bin5=i; 
    if(h1_fastmc[0]->GetBinLowEdge(i)==20)
      bin20=i; 
    if(h1_fastmc[0]->GetBinLowEdge(i)==120)
      bin120=i; 
  }
  if(bin0==-1)
    std::cout<<"ERROR: 0 is not a bin edge."<<std::endl;
  if(bin0point5==-1)
    std::cout<<"ERROR: 0.5 is not a bin edge."<<std::endl;
  if(bin0==-1)
      std::cout<<"ERROR: 2 is not a bin edge."<<std::endl;
  if(bin0==-1)
    std::cout<<"ERROR: 5 is not a bin edge."<<std::endl;
  if(bin0==-1)
    std::cout<<"ERROR: 20 is not a bin edge."<<std::endl;
  if(bin0==-1)
    std::cout<<"ERROR: 120 is not a bin edge."<<std::endl;
  
  double ratio_0to0point5[18];
  double ratio_0point5to2[18];
  double ratio_2to5[18];
  double ratio_5to20[18];
  double ratio_20to120[18];
  double ratio_0to20[18];
  double ratio_0to120[18];
  for(int i=0; i<18; i++) {
    if(h1_fastmc[i]->Integral(bin0,bin0point5-1)!=0)
      ratio_0to0point5[i] = h2_fastmc[i]->Integral(bin0,bin0point5-1)/
	h1_fastmc[i]->Integral(bin0,bin0point5-1);
    else
      ratio_0to0point5[i] = 1;

    if(h1_fastmc[i]->Integral(bin0point5,bin2-1)!=0)
      ratio_0point5to2[i] = h2_fastmc[i]->Integral(bin0point5,bin2-1)/
	h1_fastmc[i]->Integral(bin0point5,bin2-1);
    else
      ratio_0point5to2[i] = 1;

    if(h1_fastmc[i]->Integral(bin2,bin5-1)!=0)
      ratio_2to5[i] = h2_fastmc[i]->Integral(bin2,bin5-1)/
	h1_fastmc[i]->Integral(bin2,bin5-1);
    else
      ratio_2to5[i] = 1;

    if(h1_fastmc[i]->Integral(bin5,bin20-1)!=0)
      ratio_5to20[i] = h2_fastmc[i]->Integral(bin5,bin20-1)/
	h1_fastmc[i]->Integral(bin5,bin20-1);
    else
      ratio_5to20[i] = 1;

    if(h1_fastmc[i]->Integral(bin20,bin120-1)!=0)
      ratio_20to120[i] = h2_fastmc[i]->Integral(bin20,bin120-1)/
	h1_fastmc[i]->Integral(bin20,bin120-1);
    else
      ratio_20to120[i] = 1;

    if(h1_fastmc[i]->Integral(bin0,bin20-1)!=0)
      ratio_0to20[i] = h2_fastmc[i]->Integral(bin0,bin20-1)/
	h1_fastmc[i]->Integral(bin0,bin20-1);
    else
      ratio_0to20[i] = 1;
  }


  ofstream myfile;
  myfile.open(("eventRateComparisons/"+variation+"/summary_"+variation+"_"+osc+".txt").c_str());

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

  /*

  for(int i = 6; i<12; i++) {
    myfile << nu_names[i+6] <<" cc event rate "
	   <<ratio_0to0point5[i+6]<<" "
	   <<ratio_0point5to2[i+6]<<" "
	   <<ratio_2to5[i+6]<<" "
	   <<ratio_5to20[i+6]<<" "
	   <<ratio_20to120[i+6]<<" "
	   <<ratio_0to20[i+6]<<" "
	   <<ratio_0to120[i+6]<<std::endl;
  }
  for(int i = 12; i<18; i++) {
    myfile << nu_names[i+12] <<" nc event rate "
	   <<ratio_0to0point5[i+12]<<" "
	   <<ratio_0point5to2[i+12]<<" "
	   <<ratio_2to5[i+12]<<" "
	   <<ratio_5to20[i+12]<<" "
	   <<ratio_20to120[i+12]<<" "
	   <<ratio_0to20[i+12]<<" "
	   <<ratio_0to120[i+12]<<std::endl;
  }   
  */
  //myfile.close();  

}
