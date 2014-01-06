
#include "AlignmentVariations.h"
#include "TFile.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TLine.h"
#include "TLatex.h"
#include "TPad.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TMath.h"
#include "TStyle.h"
#include "TPad.h"

// To run:
// root -q -b ToleranceFits.C+\(\"nof\"\)
// root -q -b ToleranceFits.C+\(\"far\"\)
// root -q -b ToleranceFits.C+\(\"near\"\)



void add_plot_label( const char* label, double x, double y, double size = 0.05, int color = 1, int font = 62, int align = 22, double rotate = 0 ){

  TLatex *latex = new TLatex( x, y, label );
  latex->SetNDC();
  latex->SetTextSize(size);
  latex->SetTextColor(color);
  latex->SetTextFont(font);
  latex->SetTextAlign(align);
  latex->SetTextAngle(rotate);
  latex->Draw();
}


void ToleranceFits(std::string loc = "nof") {

  TCanvas *c1 = new TCanvas("Fits","Fits");
  TCanvas *c2 = new TCanvas("Summary","Summary");
  TCanvas *c3 = new TCanvas("Tot_Error","Total Error");
  TCanvas *c4 = new TCanvas("Error_summary","Error Summary");

  AlignmentVariations h;

  int colors[13] = {2,4,6,65,8,28,1,52,96,38,92,15,96};

    std::vector<Double_t> xbins;
    xbins.push_back(0);
    xbins.push_back(0.5);
    xbins.push_back(1);
    xbins.push_back(1.5);
    xbins.push_back(2);
    xbins.push_back(2.5);
    xbins.push_back(3);
    xbins.push_back(3.5);
    xbins.push_back(4);
    xbins.push_back(4.5);
    xbins.push_back(5);
    xbins.push_back(5.5);
    xbins.push_back(6);
    xbins.push_back(6.5);
    xbins.push_back(7);
    xbins.push_back(7.5);
    xbins.push_back(8);
    xbins.push_back(8.5);
    xbins.push_back(9);
    xbins.push_back(9.5);
    xbins.push_back(10);

    TH1D *h_tot_err = new TH1D("h_tot_err","h_tot_err",xbins.size()-1,&xbins[0]);
    TH1D *h_sing_err = new TH1D("h_sing_err","h_sing_err",xbins.size()-1,&xbins[0]);

    TH1D *h_app = new TH1D("h_app","h_app",20,0,10);  
    TH1D *h_dis = new TH1D("h_dis","h_dis",20,0,10);
    double v_app[40] = {6,12,15,20,25,
			34,46,56,60,61,
			60,55,50,44,38,
			32,27,22,19,16,
			14,13,12,11,10,
			9,8.9,8.8,8.7,8.6,
			8.4,8.2,8.1,8,7.9,
			7.8,7.7,7.6,7.5,7.5};
    double v_dis[40] = {0,0,18,81,258,
			373,397,185,104,82,
			95,125,160,193,215,
			220,220,210,202,194,
			181,170,160,159,159,
			140,130,123,118,111,
			108,108,108,108,108,
			108,108,108,108,108};
    
    for(int i = 0; i<20; i++) {
      double n_events = v_app[i*2]+v_app[i*2+1];
      if(n_events != 0)
	h_app->SetBinContent(i+1,TMath::Sqrt(n_events)/n_events);
      n_events = v_dis[i*2]+v_dis[i*2+1];
      if(n_events != 0)
	h_dis->SetBinContent(i+1,TMath::Sqrt(n_events)/n_events);
    }

    // This is the list of alignment quantities you want to consider
    std::vector<AlignmentVariation*> theVariations;
    theVariations.push_back(h.FarDetX);
    theVariations.push_back(h.FarDetY);
    theVariations.push_back(h.HornCurrent);
    theVariations.push_back(h.Horn1XTilt);
    theVariations.push_back(h.Horn1YTilt);
    theVariations.push_back(h.Horn1XOffset);
    theVariations.push_back(h.Horn1YOffset);
    theVariations.push_back(h.Horn2XOffset);
    theVariations.push_back(h.Horn2YOffset);
    theVariations.push_back(h.Horn2XTilt);
    theVariations.push_back(h.Horn2YTilt);
    theVariations.push_back(h.TargetXOffset);
    theVariations.push_back(h.TargetYOffset);
    theVariations.push_back(h.TargetXTilt);
    theVariations.push_back(h.TargetYTilt);
    theVariations.push_back(h.BeamSigmaX);
    theVariations.push_back(h.BeamSigmaY);
    theVariations.push_back(h.DecayPipeRadius);
    theVariations.push_back(h.WaterLayerThickness);

    TLegend *leg4 = new TLegend(.6,0.4,.85,0.85);
    leg4->SetFillStyle(0);
    leg4->SetBorderSize(0);

    for(unsigned int i = 0; i<theVariations.size(); i++) {

      std::cout<<"Doing tolerance fit for "<<theVariations[i]->GetVariedQuantity()<<std::endl;
      
      TFile *f_nom_near = new TFile((theVariations[i]->GetNominalHistoFile("nu","LBNEND")).c_str());
      TFile *f_nom_far = new TFile((theVariations[i]->GetNominalHistoFile("nu","LBNEFD")).c_str());
      
      TH1D *h_nom_near = (TH1D*)f_nom_near->Get("numu_fluxosc_forplots");
      TH1D *h_nom_far = (TH1D*)f_nom_far->Get("numu_fluxosc_forplots");
      
      h_nom_near->SetDirectory(0);
      h_nom_far->SetDirectory(0);
      
      h_nom_near = (TH1D*)h_nom_near->Rebin(20,"h_nom_near",&xbins[0]);
      h_nom_far = (TH1D*)h_nom_far->Rebin(20,"h_nom_far",&xbins[0]);

      TH1D *h_nom_nof;
      if(loc=="near" || loc=="nof") {
	h_nom_nof = (TH1D*)h_nom_near->Clone("h_nom_nof");
	h_nom_nof->GetYaxis()->SetTitle("Near Flux");
      }
      if(loc=="far") {
	h_nom_nof = (TH1D*)h_nom_far->Clone("h_nom_nof");
	h_nom_nof->GetYaxis()->SetTitle("Far Flux");
      }
      if(loc=="nof") {
	for(int j = 0; j<h_nom_nof->GetNbinsX()+1; j++) {
	  // mc stat errors are nearly fully correlated
	  // only count them once?
	  h_nom_nof->SetBinError(j+1,0);
	}
	h_nom_nof->Divide(h_nom_far);
	h_nom_nof->GetYaxis()->SetTitle("Near/Far");
      }
      h_nom_nof->Draw();
      
      int n_variations = theVariations[i]->GetVariationNames().size();

    // read in variations and calculate near over far fluxes

    TH1D *h_nof[20];
    TH1D *h_nof_fits[20];
    for(int j = 0; j< n_variations; j++) {

      TFile *f_near = new TFile((theVariations[i]->GetVariedHistoFiles("nu","LBNEND")[j]).c_str());
      TFile *f_far = new TFile((theVariations[i]->GetVariedHistoFiles("nu","LBNEFD")[j]).c_str());
      
      TH1D *h_near = (TH1D*)f_near->Get("numu_fluxosc_forplots");
      TH1D *h_far = (TH1D*)f_far->Get("numu_fluxosc_forplots");
      
      h_near->SetDirectory(0);
      h_far->SetDirectory(0);

      std::string variation = theVariations[i]->GetVariedQuantity()+theVariations[i]->GetVariationNames()[j];

      h_near = (TH1D*)h_near->Rebin(20,("h_nom_near_"+variation).c_str(),&xbins[0]);
      h_far = (TH1D*)h_far->Rebin(20,("h_nom_far_"+variation).c_str(),&xbins[0]);
    
      
      if(loc=="near" || loc=="nof") {
	h_nof[j]= (TH1D*)h_near->Clone(("h_nof"+variation).c_str());
	h_nof[j]->GetYaxis()->SetTitle("Near Flux Ratio to Nominal");
      }
      if(loc=="far") {
	h_nof[j]= (TH1D*)h_far->Clone(("h_nof"+variation).c_str());
	h_nof[j]->GetYaxis()->SetTitle("Far Flux to Nominal");
      }
      if(loc=="nof") {
	h_nof[j]->Divide(h_far);
	h_nof[j]->GetYaxis()->SetTitle("Near/Far Ratio to Nominal");
      }
      h_nof[j]->Divide(h_nom_nof);
      h_nof[j]->SetMarkerColor(colors[j%13]);
      h_nof_fits[j]= (TH1D*)h_nof[j]->Clone(("h_nof_fits"+variation).c_str());
    
    }
  
  
    // Do the fits
    c1->Clear();
    c1->Divide(5,4);
    for(int k = 0; k<h_nof[0]->GetNbinsX(); k++) {
      c1->cd(k+1);
      std::cout<<"k"<<k<<std::endl;
      double x[20];
      double y[20];
      double ex[20];
      double ey[20];
      double xmin = 999999;
      double xmax = -999999;
      for(int j = 0; j<n_variations; j++) {
	x[j] = atof(theVariations[i]->GetVariationNames()[j].c_str())-
	  atof(theVariations[i]->GetCV().c_str());
	if(x[j]<xmin) xmin = x[j];
	if(x[j]>xmax) xmax = x[j];
	y[j] = h_nof[j]->GetBinContent(k+1)-1;
	ex[j] = .000001;
	ey[j] = h_nof[j]->GetBinError(k+1);
      }

      // widen range for nicer looking plots
      xmin = xmin - (xmax-xmin)*.1;
      xmax = xmax + (xmax-xmin)*.1;

      std::cout<<"xmin "<<xmin<<" xmax "<<xmax<<std::endl;
      TGraph *gr = new TGraphErrors(n_variations,x,y,ex,ey);
      gr->SetMarkerStyle(21);
      gr->SetMarkerSize(1);
      gr->GetXaxis()->SetLimits(xmin,xmax);
      double newmax = gr->GetMinimum()+(gr->GetMaximum()-gr->GetMinimum())*1.4;
      gr->SetMaximum(newmax);
      gr->Draw("AP");
      TF1 *myfunc = new TF1("myfunc","[0]*x^[1]",xmin,xmax);
      TF1 *myfunc2 = new TF1("myfunc2","[0]*x^[1]",xmin,xmax);
      
      gr->GetXaxis()->SetLabelSize(0.08);
      gr->GetYaxis()->SetLabelSize(0.08);

      myfunc->SetParName(0,"A");
      myfunc->SetParName(1,"p");
      myfunc->SetParameter(0,0.01);
      myfunc->FixParameter(1,1);
      myfunc2->SetLineColor(2);

      myfunc2->SetParName(0,"A");
      myfunc2->SetParName(1,"p");
      myfunc2->SetParameter(0,0.01);
      myfunc2->FixParameter(1,2);
      gr->Fit("myfunc","0");
      gr->Fit("myfunc2","0");
      
      TF1 *bestfunc = myfunc;
      if(myfunc2->GetChisquare()<myfunc->GetChisquare())
	bestfunc = myfunc2;

      bestfunc->Draw("same");

      add_plot_label(Form("%1.1f < E < %1.1f GeV",h_nof[0]->GetBinLowEdge(k+1),h_nof[0]->GetBinLowEdge(k+1)+h_nof[0]->GetBinWidth(k+1)),0.55,0.96,0.09,38,62,22);
      add_plot_label(Form("A = %1.3f, p = %1.3f",bestfunc->GetParameter(0),bestfunc->GetParameter(1)),0.55,0.83,0.09,30,62,22);

      std::cout<<"CHISQUARE "<<bestfunc->GetChisquare()<<std::endl;

      add_plot_label(Form("#chi^{2}/dof=%3.1f",bestfunc->GetChisquare()/bestfunc->GetNDF()),0.97,0.5,0.10,kMagenta-9,62,22,90);

      for(int j = 0; j<n_variations; j++) {
	h_nof_fits[j]->SetBinContent(k+1,1+bestfunc->Eval(atof(theVariations[i]->GetVariationNames()[j].c_str())-atof(theVariations[i]->GetCV().c_str())));
	h_nof_fits[j]->SetBinError(k+1,0);
      }
      h_tot_err->SetBinContent(k+1,h_tot_err->GetBinContent(k+1)+TMath::Power(bestfunc->Eval(atof(theVariations[i]->GetTolerance().c_str())),2));
      h_sing_err->SetBinContent(k+1,TMath::Power(bestfunc->Eval(atof(theVariations[i]->GetTolerance().c_str())),2));
    }
    c1->Print((theVariations[i]->GetVariedQuantity()+"_"+loc+"_fits.eps").c_str());
    c1->Print((theVariations[i]->GetVariedQuantity()+"_"+loc+"_fits.png").c_str());
 
    
    // Plot the results
    c2->cd();
    for(unsigned int k = 0; k<xbins.size()-1; k++)
      h_sing_err->SetBinContent(k+1,TMath::Sqrt(h_sing_err->GetBinContent(k+1)));
    h_sing_err->GetXaxis()->SetTitle("#nu_{#mu} Energy (GeV)");
    h_sing_err->GetYaxis()->SetTitle("Fractional Error");
    h_sing_err->SetMaximum(0.3);
    h_sing_err->SetMarkerStyle(21);
    h_sing_err->SetMarkerSize(1);
    h_sing_err->Draw("p");
    h_dis->SetMarkerColor(2);
    h_app->SetMarkerColor(3);
    h_dis->SetMarkerStyle(21);
    h_dis->SetMarkerSize(1);
    h_app->SetMarkerStyle(21);
    h_app->SetMarkerSize(1);
    h_dis->Draw("psame");
    h_app->Draw("psame");
    
    TLegend *leg = new TLegend(.25,0.65,.45,0.85);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->AddEntry(h_app,"Stat, #nu_{e} App","p");
    leg->AddEntry(h_dis,"Stat, #nu_{#mu} Dis","p");
    leg->AddEntry(h_sing_err,theVariations[i]->GetVariedQuantity().c_str(),"p");
    leg->Draw();

  
  c2->Print((theVariations[i]->GetVariedQuantity()+"_"+loc+"_statcomp.eps").c_str());
  c2->Print((theVariations[i]->GetVariedQuantity()+"_"+loc+"_statcomp.png").c_str());

  h_sing_err->GetYaxis()->SetTitleOffset(1.2);
  h_sing_err->SetMaximum(0.04);
  h_sing_err->Draw("p");
  TLegend *leg2 = new TLegend(.25,0.65,.65,0.85);
  leg2->SetFillStyle(0);
  leg2->SetBorderSize(0);
  //leg2->AddEntry(h_app,"Stat, #nu_{e} App","p");
  //leg2->AddEntry(h_dis,"Stat, #nu_{#mu} Dis","p");
  leg2->AddEntry(h_sing_err,(theVariations[i]->GetVariedQuantity()+" Error").c_str(),"p");
    leg2->Draw();

  c2->Print((theVariations[i]->GetVariedQuantity()+"_"+loc+"_error.eps").c_str());
  c2->Print((theVariations[i]->GetVariedQuantity()+"_"+loc+"_error.png").c_str());
  c4->cd(); 
  
  h_sing_err->SetLineColor(colors[i%13]);
  if(i==0)
    h_sing_err->DrawCopy("c");
  else
    h_sing_err->DrawCopy("csame");
  leg4->AddEntry(h_sing_err->Clone(),theVariations[i]->GetVariedQuantity().c_str(),"l");


    c2->cd();
    for(int j = 0; j< n_variations; j++) {

      gPad->SetRightMargin(0.15);   // Set Margin Right
      double chisquare = 0;
      for(int k = 0; k<h_nof[j]->GetNbinsX(); k++) 
	chisquare += pow(h_nof[j]->GetBinContent(k+1)-h_nof_fits[j]->GetBinContent(k+1),2)/pow(h_nof[j]->GetBinError(k+1),2);
      std::cout<<"Chi2: "<<chisquare<<"/"<<h_nof[j]->GetNbinsX()<<" = "<<chisquare/h_nof[j]->GetNbinsX()<<" "<<j<<std::endl;
      std::cout<<Form("#chi^{2}/dof=%3.1f",chisquare/h_nof[j]->GetNbinsX())<<std::endl;
      
      h_nof[j]->Scale(1+j*0.1);
      h_nof_fits[j]->Scale(1+j*0.1);

      h_nof_fits[j]->SetLineColor(colors[j%13]);

      h_nof[j]->SetMinimum(0.95);
      h_nof[j]->SetMaximum(1+0.1*(n_variations));
      h_nof[j]->SetLineWidth(1);

      h_nof[j]->SetMarkerStyle(21);
      h_nof[j]->SetMarkerSize(1);

      if(j==0)
	h_nof[j]->Draw("pe1");
      else
	h_nof[j]->Draw("pe1same");

      h_nof_fits[j]->Draw("pcsame");

      add_plot_label(Form("#chi^{2}/dof=%3.1f",chisquare/h_nof[j]->GetNbinsX()),0.93,0.1+(0.75/(double)n_variations)*(j+1),0.04,colors[j%13],62,22);




      TLine l(0,1+0.1*j,10,1+0.1*j);
      l.DrawClone("same");
    
      double x_low = 0.1+(0.8/(double)n_variations)*(j+1)-0.15;
      double x_high = 0.1+(0.8/(double)n_variations)*(j+1);
      
      TLegend *leg3 = new TLegend(x_low,0.9,x_high,0.95);
      leg3->SetFillStyle(0);
      leg3->SetBorderSize(0);
      std::string label = theVariations[i]->GetVariationNames()[j]+" "+theVariations[i]->GetUnits();
      leg3->AddEntry(h_nof[j],label.c_str(),"pe");
      leg3->Draw();
	    
    }
    
    c2->Print((theVariations[i]->GetVariedQuantity()+"_"+loc+"_summary.eps").c_str());
    c2->Print((theVariations[i]->GetVariedQuantity()+"_"+loc+"_summary.png").c_str());

    add_plot_label(theVariations[i]->GetVariedQuantity().c_str(),0.95,0.5,0.05,1,62,22,90);
      std::cout<<"AA"<<std::endl;
    }

  c3->cd();
  std::cout<<"A"<<std::endl;
  for(unsigned int k = 0; k<xbins.size()-1; k++)
    h_tot_err->SetBinContent(k+1,TMath::Sqrt(h_tot_err->GetBinContent(k+1)));
  h_tot_err->GetXaxis()->SetTitle("#nu_{#mu} Energy (GeV)");
  h_tot_err->GetYaxis()->SetTitle("Fractional Error");
  h_tot_err->SetMaximum(0.3);
  h_tot_err->SetMarkerStyle(21);
  h_tot_err->SetMarkerSize(1);
  h_tot_err->Draw("p");
  std::cout<<"B"<<std::endl;
  h_dis->SetMarkerColor(2);
  h_app->SetMarkerColor(3);
  h_dis->SetMarkerStyle(21);
  h_dis->SetMarkerSize(1);
  h_app->SetMarkerStyle(21);
  h_app->SetMarkerSize(1);
  h_dis->Draw("psame");
  h_app->Draw("psame");

  std::cout<<"C"<<std::endl;
  TLegend *leg = new TLegend(.25,0.65,.45,0.85);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->AddEntry(h_app,"Stat, #nu_{e} App","p");
  leg->AddEntry(h_dis,"Stat, #nu_{#mu} Dis","p");
  leg->AddEntry(h_tot_err,"Alignment","p");
  leg->Draw();

  std::cout<<"D"<<std::endl;
  c3->Print(("tot_error_"+loc+".eps").c_str());
  c3->Print(("tot_error_"+loc+".png").c_str());
  std::cout<<"E"<<std::endl;

  c4->cd();
  leg4->Draw();
  c4->Print(("error_summary_"+loc+".eps").c_str());  
  c4->Print(("error_summary_"+loc+".png").c_str());

}
