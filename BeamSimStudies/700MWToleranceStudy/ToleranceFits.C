#include "AlignmentVariation.h"
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
#include <iomanip>

// To run:
// root -q -b ToleranceFits.C+\(\"nof\"\)
// root -q -b ToleranceFits.C+\(\"far\"\)
// root -q -b ToleranceFits.C+\(\"near\"\)

// This script uses AlignmentVariation.h and AlignmentVariations.h
// which should be in the same directory
// it loops over sources of alignment uncertainty, reads in histograms to 
// estimate that uncertainty, plots individual uncertainties
// and totals


//  some helper functions
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

// gets max value of histogram within a range bin bins (inclusive)
double getHistMaximum( TH1D* hist, int lowbin, int highbin) {
  
  // make sure bounds are within limits of histogram
  if(lowbin < 0) lowbin = 0;
  if(highbin > hist->GetNbinsX()+1) highbin = hist->GetNbinsX()+1;

  double max = hist->GetBinContent(lowbin);

  for(int i = lowbin+1; i<=highbin; i++) {
    if(hist->GetBinContent(i)>max)
      max = hist->GetBinContent(i);
  }
  return max;
}




void ToleranceFits(std::string loc = "nof") {


  // Setup the canvases for drawing stuff
  TCanvas *c1 = new TCanvas("Fits","Fits");
  TCanvas *c2 = new TCanvas("Summary","Summary");
  TCanvas *c3 = new TCanvas("Tot_Error","Total Error");
  TCanvas *c4 = new TCanvas("Error_summary","Error Summary");

  // Define a set of colors for plotting
  int colors[13] = {2,4,6,65,8,28,1,52,96,38,92,15,96};

  // Define the flux binning
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

  // create histograms to hold the total and individual errors
    TH1D *h_tot_err = new TH1D("h_tot_err","h_tot_err",xbins.size()-1,&xbins[0]);
    TH1D *h_sing_err = new TH1D("h_sing_err","h_sing_err",xbins.size()-1,&xbins[0]);

    // open a latex file to hold a table of errors
    ofstream error_table;
    error_table.open(("error_table_"+loc+".tex").c_str());
    error_table<<"\\begin{sidewaystable}[ht]"<<std::endl; 
    error_table<<"\\centering"<<std::endl;
    error_table<<"\\tiny"<<std::endl;
    error_table<<"\\begin{tabular}{|c | c c c c c c c c c c c c c c c c c c c c | }"<<std::endl;
    error_table<<"\\hline"<<std::endl; 
    error_table<<"Energy( GeV) & 0-0.5 & 0.5-1 & 1-1.5 & 1.5-2 & 2-2.5 & 2.5-3 & 3-3.5 & 3.5-4 & 4-4.5 & 4.5-5 & 5-5.5 & 5.5-6 & 6-6.5 & 6.5-7 & 7-7.5 & 7.5-8 & 8-8.5 & 8.5-9 & 9-9.5 & 9.5-10 \\\\"<<std::endl; 
    error_table<<"\\hline"<<std::endl;

    // these come from the fast mc -- approximate statistics for 5 years
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


    // calculate fractional statistical ucnertaintanties from event rates
    for(int n = 0; n<20; n++) {
      double n_events = v_app[n*2]+v_app[n*2+1];
      if(n_events != 0)
	h_app->SetBinContent(n+1,TMath::Sqrt(n_events)/n_events);
      n_events = v_dis[n*2]+v_dis[n*2+1];
      if(n_events != 0)
	h_dis->SetBinContent(n+1,TMath::Sqrt(n_events)/n_events);
    }

    // Create an object of type AlignmentVariations, which
    // sets the names, characteristics of each 
    AlignmentVariations h;
    
    // This is the list of alignment quantities you want to consider
    std::vector<AlignmentVariation*> theVariations;

    theVariations.push_back(h.FarDetX);
    theVariations.push_back(h.FarDetY);
    theVariations.push_back(h.NearDetX);
    theVariations.push_back(h.NearDetY);
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
    theVariations.push_back(h.BaffleScraping);
    //theVariations.push_back(h.NoShielding);
    theVariations.push_back(h.DecayPipeOffsetX);
    theVariations.push_back(h.SkinDepthIC);
    theVariations.push_back(h.BeamTiltX);
    theVariations.push_back(h.BeamTiltY);
    theVariations.push_back(h.BeamOffsetX);
    theVariations.push_back(h.BeamOffsetY);
    theVariations.push_back(h.TargetDensity);

    // create a legend
    TLegend *leg4 = new TLegend(.715,0.25,1,0.75);
    leg4->SetFillStyle(0);
    leg4->SetBorderSize(0);

    // loop over uncertainties
    int n_errors_on_summary_plot = 0;
    for(unsigned int i = 0; i<theVariations.size(); i++) {

      std::cout<<"Doing tolerance fit for "<<theVariations[i]->GetVariedQuantity()<<std::endl;
      
      // Get nominal near and far histograms
      TFile *f_nom_near = new TFile((theVariations[i]->GetNominalHistoFile("nu","LBNEND")).c_str());
      TFile *f_nom_far = new TFile((theVariations[i]->GetNominalHistoFile("nu","LBNEFD")).c_str());
      
      TH1D *h_nom_near = (TH1D*)f_nom_near->Get("numu_flux_forplots");
      TH1D *h_nom_far = (TH1D*)f_nom_far->Get("numu_flux_forplots");
      
      h_nom_near->SetDirectory(0);
      h_nom_far->SetDirectory(0);
      
      h_nom_near = (TH1D*)h_nom_near->Rebin(20,"h_nom_near",&xbins[0]);
      h_nom_far = (TH1D*)h_nom_far->Rebin(20,"h_nom_far",&xbins[0]);
      
      // h_nom_nof is poorly names - it holds the nomina near, far or near/far flux
      // depending on what the user has asked for
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
      
      int n_variations = theVariations[i]->GetVariationNames().size();

    
      // if this is a standard type of uncertainty,
      // loop over the different variations, do fits and extract
      // the uncertainty using the alignment tolerance
      if(theVariations[i]->GetVariationType() != "shift") {

	TH1D *h_nof[20];
	TH1D *h_nof_fits[20];
      

	// read in variations and calculate near over far fluxes
      
	for(int j = 0; j< n_variations; j++) {
	  
	  TFile *f_near = new TFile((theVariations[i]->GetVariedHistoFiles("nu","LBNEND")[j]).c_str());
	  TFile *f_far = new TFile((theVariations[i]->GetVariedHistoFiles("nu","LBNEFD")[j]).c_str());
	  
	  TH1D *h_near = (TH1D*)f_near->Get("numu_flux_forplots");
	  TH1D *h_far = (TH1D*)f_far->Get("numu_flux_forplots");
	  
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

	  double x[20];
	  double y[20];
	  double ex[20];
	  double ey[20];
	  double xmin = 999999;
	  double xmax = -999999;
	  for(int j = 0; j<n_variations; j++) {
	    x[j] = atof(theVariations[i]->GetVariationNames()[j].c_str())-
	      atof(theVariations[i]->GetCV().c_str());
	    //if(theVariations[i]->GetVariedQuantity()=="SkinDepthIC_") {
	    //  x[j] = 1/x[j];
	    //  std::cout<<"BLAH "<<std::endl;
	    // }
	    if(x[j]<xmin) xmin = x[j];
	    if(x[j]>xmax) xmax = x[j];
	    y[j] = h_nof[j]->GetBinContent(k+1)-1;
	    ex[j] = .000001;
	    ey[j] = h_nof[j]->GetBinError(k+1);
	  }
	  
	  // widen range for nicer looking plots
	  xmin = xmin - (xmax-xmin)*.1;
	  xmax = xmax + (xmax-xmin)*.1;
	  if(xmin > 0 ) xmin = 0;
	  if(xmax < 0 ) xmax = 0;

	  TGraph *gr = new TGraphErrors(n_variations,x,y,ex,ey);
	  gr->SetMarkerStyle(21);
	  gr->SetMarkerSize(1);
	  gr->GetXaxis()->SetLimits(xmin,xmax);
	  gr->GetXaxis()->SetNdivisions(4);
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

	  add_plot_label(Form("#chi^{2}/dof=%3.1f",bestfunc->GetChisquare()/bestfunc->GetNDF()),0.97,0.5,0.10,kMagenta-9,62,22,90);

	  for(int j = 0; j<n_variations; j++) {
	    h_nof_fits[j]->SetBinContent(k+1,1+bestfunc->Eval(atof(theVariations[i]->GetVariationNames()[j].c_str())-atof(theVariations[i]->GetCV().c_str())));
	    h_nof_fits[j]->SetBinError(k+1,0);
	  }
	  h_sing_err->SetBinContent(k+1,TMath::Power(bestfunc->Eval(atof(theVariations[i]->GetTolerance().c_str())),2));
	  h_tot_err->SetBinContent(k+1,h_tot_err->GetBinContent(k+1)+TMath::Power(bestfunc->Eval(atof(theVariations[i]->GetTolerance().c_str())),2));   
	  //if(theVariations[i]->GetVariedQuantity()=="SkinDepthIC_") {
	  //  h_sing_err->SetBinContent(k+1,TMath::Power(bestfunc->Eval(1/atof(theVariations[i]->GetTolerance().c_str())),2));
	  //  h_tot_err->SetBinContent(k+1,h_tot_err->GetBinContent(k+1)+TMath::Power(bestfunc->Eval(1/atof(theVariations[i]->GetTolerance().c_str())),2));   
	  //}
	}
	c1->Print((theVariations[i]->GetVariedQuantity()+theVariations[i]->GetMacroSuffix()+"_"+loc+"_fits.eps").c_str());
	c1->Print((theVariations[i]->GetVariedQuantity()+theVariations[i]->GetMacroSuffix()+"_"+loc+"_fits.png").c_str());
      
      c2->cd();
      for(int j = 0; j< n_variations; j++) {
	
	gPad->SetRightMargin(0.15);   // Set Margin Right
	double chisquare = 0;
	for(int m = 0; m<h_nof[j]->GetNbinsX(); m++) 
	  chisquare += pow(h_nof[j]->GetBinContent(m+1)-h_nof_fits[j]->GetBinContent(m+1),2)/pow(h_nof[j]->GetBinError(m+1),2);

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
    
      c2->Print((theVariations[i]->GetVariedQuantity()+theVariations[i]->GetMacroSuffix()+"_"+loc+"_summary.eps").c_str());
      c2->Print((theVariations[i]->GetVariedQuantity()+theVariations[i]->GetMacroSuffix()+"_"+loc+"_summary.png").c_str());

      add_plot_label(theVariations[i]->GetVariedQuantity().c_str(),0.95,0.5,0.05,1,62,22,90);

      
      }
      // for uncertainties that don't have multiple variations,
      // calculate the uncertainty from a single shifted flux
      // this is only baffle scraping at the moment
      else {

	TFile *f_near = new TFile((theVariations[i]->GetVariedHistoFiles("nu","LBNEND")[0]).c_str());
	TFile *f_far = new TFile((theVariations[i]->GetVariedHistoFiles("nu","LBNEFD")[0]).c_str());
	  
	TH1D *h_near = (TH1D*)f_near->Get("numu_flux_forplots");
	TH1D *h_far = (TH1D*)f_far->Get("numu_flux_forplots");
	  
	h_near->SetDirectory(0);
	h_far->SetDirectory(0);
      
	std::string variation = theVariations[i]->GetVariedQuantity();
	
	h_near = (TH1D*)h_near->Rebin(20,("h_nom_near_"+variation).c_str(),&xbins[0]);
	h_far = (TH1D*)h_far->Rebin(20,("h_nom_far_"+variation).c_str(),&xbins[0]);

	// Baffle scraping is a special case where we add a small fraction
	// of the baffle flux to the nominal flux
	if(theVariations[i]->GetVariedQuantity()=="BeamOffsetX7Sigma0pnt001") {
	  h_near->Add(h_nom_near,h_near,1,theVariations[i]->GetScaleFactor());
	  h_far->Add(h_nom_far,h_far,1,theVariations[i]->GetScaleFactor());
	}

	TH1D *h_nof[20];

	if(loc=="near" || loc=="nof") {
	  h_nof[0]= (TH1D*)h_near->Clone(("h_nof"+variation).c_str());
	  h_nof[0]->GetYaxis()->SetTitle("Near Flux Ratio to Nominal");
	}
	if(loc=="far") {
	  h_nof[0]= (TH1D*)h_far->Clone(("h_nof"+variation).c_str());
	  h_nof[0]->GetYaxis()->SetTitle("Far Flux to Nominal");
	}
	if(loc=="nof") {
	  h_nof[0]->Divide(h_far);
	  h_nof[0]->GetYaxis()->SetTitle("Near/Far Ratio to Nominal");
	}
	h_nof[0]->Divide(h_nom_nof);
	h_nof[0]->SetMarkerColor(colors[0]);    
      
	for(int shift_bins = 0; shift_bins <h_nof[0]->GetNbinsX(); shift_bins++)
	  h_sing_err->SetBinContent(shift_bins+1,TMath::Power(1 - h_nof[0]->GetBinContent(shift_bins+1),2));
		
	for(int k = 0; k<h_tot_err->GetNbinsX(); k++) {
	  h_tot_err->SetBinContent(k+1,h_tot_err->GetBinContent(k+1)+(h_sing_err->GetBinContent(k+1)));   
	}
      }
      
      // Plot the results
      c2->cd();
      for(unsigned int kk = 0; kk<xbins.size()-1; kk++)
	h_sing_err->SetBinContent(kk+1,TMath::Sqrt(h_sing_err->GetBinContent(kk+1)));

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
      
      add_plot_label("Stat from Fast MC, 5 years #times 700 kW x 35 kTon",0.65,0.925,0.04,38,62,22);

  
      c2->Print((theVariations[i]->GetVariedQuantity()+theVariations[i]->GetMacroSuffix()+"_"+loc+"_statcomp.eps").c_str());
      c2->Print((theVariations[i]->GetVariedQuantity()+theVariations[i]->GetMacroSuffix()+"_"+loc+"_statcomp.png").c_str());
      
      h_sing_err->GetYaxis()->SetTitleOffset(1.2);
      h_sing_err->SetMaximum(0.04);
      h_sing_err->SetMinimum(0);
      h_sing_err->Draw("p");
      TLegend *leg2 = new TLegend(.25,0.65,.65,0.85);
      leg2->SetFillStyle(0);
      leg2->SetBorderSize(0);
      //leg2->AddEntry(h_app,"Stat, #nu_{e} App","p");
      //leg2->AddEntry(h_dis,"Stat, #nu_{#mu} Dis","p");
      leg2->AddEntry(h_sing_err,(theVariations[i]->GetLabel()+" Error").c_str(),"p");
      leg2->Draw();
    
      c2->Print((theVariations[i]->GetVariedQuantity()+theVariations[i]->GetMacroSuffix()+"_"+loc+"_error.eps").c_str());
      c2->Print((theVariations[i]->GetVariedQuantity()+theVariations[i]->GetMacroSuffix()+"_"+loc+"_error.png").c_str());
    
      c4->cd(); 
    
      // exclude really tiny errors from the error summary
      // to make the plot readable
    
      if(getHistMaximum(h_sing_err,0,16) >.006) {
	h_sing_err->SetLineColor(colors[n_errors_on_summary_plot%13]);
	h_sing_err->SetMarkerColor(colors[n_errors_on_summary_plot%13]);
	h_sing_err->SetMarkerStyle(20+n_errors_on_summary_plot%4);
	h_sing_err->SetMarkerSize(1);
	if(n_errors_on_summary_plot==0) {
	  TH2D *frame = new TH2D("frame","frame",10,0,8,10,0,0.035);
	  frame->GetXaxis()->SetTitle("#nu_{#mu} Energy (GeV)");
	  frame->GetYaxis()->SetTitle("Fractional Error");
	  frame->GetYaxis()->SetTitleOffset(1.2);
	  frame->DrawCopy();
	  gPad->SetRightMargin(0.3);   // Set Margin Right
	  h_sing_err->DrawCopy("PLsame");
	}
	else
	  h_sing_err->DrawCopy("PLsame");
	n_errors_on_summary_plot++;  
	leg4->AddEntry(h_sing_err->Clone(),theVariations[i]->GetLabel().c_str(),"pl");
      }
      error_table<<std::fixed<<std::setprecision(2);
      error_table<<theVariations[i]->GetLabel();
      for(int k = 0; k<h_sing_err->GetNbinsX(); k++)
	error_table <<" & "<<h_sing_err->GetBinContent(k+1)*100<<std::endl;
      error_table<<"\\\\"<<std::endl;
      
    }

    error_table <<std::endl<<"\\hline"<<std::endl;
    error_table <<"\\end{tabular}"<<std::endl; 
    error_table <<"\\label{tab:errorsummary}"<<std::endl;
    error_table<<"\\caption{Systematic errors on the near/far ratio (in percent) in each energy bin for each source of alignment uncertainty.}"<<std::endl;  
    error_table <<"\\end{sidewaystable}"<<std::endl; 
    error_table.close();

  c4->cd();
  leg4->Draw();
  c4->Print(("error_summary_"+loc+".eps").c_str());  
  c4->Print(("error_summary_"+loc+".png").c_str());



  c3->cd();
  for(unsigned int k_bin = 0; k_bin<xbins.size()-1; k_bin++)
    h_tot_err->SetBinContent(k_bin+1,TMath::Sqrt(h_tot_err->GetBinContent(k_bin+1)));
  h_tot_err->GetXaxis()->SetTitle("#nu_{#mu} Energy (GeV)");
  h_tot_err->GetYaxis()->SetTitle("Fractional Error");
  h_tot_err->SetMaximum(0.3);
  h_tot_err->SetMarkerStyle(21);
  h_tot_err->SetMarkerSize(1);
  h_tot_err->Draw("p");

  h_dis->SetMarkerColor(2);
  h_app->SetMarkerColor(3);
  h_dis->SetMarkerStyle(21);
  h_dis->SetMarkerSize(1);
  h_app->SetMarkerStyle(21);
  h_app->SetMarkerSize(1);
  if(loc != "near") {
    h_dis->Draw("psame");
    h_app->Draw("psame");
  }

  TLegend *leg = new TLegend(.25,0.65,.45,0.85);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  if(loc != "near") {
    leg->AddEntry(h_app,"Stat, #nu_{e} App","p");
    leg->AddEntry(h_dis,"Stat, #nu_{#mu} Dis","p");
  }
  leg->AddEntry(h_tot_err,"Alignment","p");
  leg->Draw();
  
  if(loc != "near")
    add_plot_label("Stat from Fast MC, 5 years #times 700 kW x 35 kTon",0.65,0.925,0.04,38,62,22);
  c3->Print(("tot_error_"+loc+".eps").c_str());
  c3->Print(("tot_error_"+loc+".png").c_str());
  TFile f(("tot_error_"+loc+".root").c_str(),"RECREATE");
  h_tot_err->Write();

}
