{
 ////////////////////////////////////////////////////////////////////////////
 //                                                                        
 //   Laura Fields
 //   16 October 2013
 ////////////////////////////////////////////////////////////////////////////

 // Definition of new styles
 TStyle *LauraStyle      = new TStyle("LauraStyle","LauraStyle");

 // Set the default parameters for style LauraStyle
 LauraStyle->SetCanvasBorderMode(0);     // Removes Canvas Border
 LauraStyle->SetPadBorderMode(0);        // Removes Pad Border
 LauraStyle->SetCanvasColor(0);          // Changes Canvas color to white
 LauraStyle->SetPadColor(0);             // Changes Pad color to white
 LauraStyle->SetStatColor(0);            // Changes Stats bg color to white

 LauraStyle->SetTitleFillColor(0);       // Changes Title bg color to white
 LauraStyle->SetLabelFont(42,"xyz");     // Changes Label Font
 LauraStyle->SetTitleFont(42,"xyz");     // Changes Axis Title Font
 LauraStyle->SetTitleXSize(0.05);
 LauraStyle->SetTitleYSize(0.05);
 LauraStyle->SetLabelSize(0.055,"X");
 LauraStyle->SetLabelSize(0.055,"Y");
 LauraStyle->SetTitleYOffset(1.05);
 LauraStyle->SetTitleXOffset(1.15);
 LauraStyle->SetPadTickX(1);             // Sets tic marks on both horizontal axes
 LauraStyle->SetPadTickY(1);             // Sets tic marks on both vertical axes
 LauraStyle->SetTickLength(0.018,"xyz"); // Sets tic length
 LauraStyle->SetOptStat(0);              // Turns off Statistics display
 LauraStyle->SetOptTitle(0);             // Turns off Title display
 LauraStyle->SetHistLineWidth(2);        // Changes Histogram Line width
 LauraStyle->SetFrameBorderMode(0);      // Removes the Frame Border
 LauraStyle->SetFrameFillStyle(0);
 LauraStyle->SetCanvasDefH(494);         // Sets Default Canva Height
 LauraStyle->SetCanvasDefW(800);         // Sets Default Canvas Width
 LauraStyle->SetPalette(1);

 // legends
 LauraStyle->SetLegendBorderSize(0);
 LauraStyle->SetLegendFillColor(2);

 // center histogram titles; doesn't work!
 LauraStyle->SetTitleX(0.5);
 LauraStyle->SetTitleAlign(22);

 // The next for commands set the default margin size
 // n.b. the margins do not take axis labels into account! grr!!
 LauraStyle->SetPadTopMargin(0.1);     // Set Margin Top
 LauraStyle->SetPadRightMargin(0.1);   // Set Margin Right
 LauraStyle->SetPadBottomMargin(0.15);    // Set Margin Bottom
 LauraStyle->SetPadLeftMargin(0.15);     // Set Margin Left

 //make a nice palette
 int NRGBs = 7, NCont = 999;
 LauraStyle->SetNumberContours(NCont);
 Double_t stops[NRGBs] = { 0.00, 0.10, 0.25, 0.45, 0.60, 0.75, 1.00 };
 Double_t red[NRGBs]   = { 1.00, 0.00, 0.00, 0.00, 0.97, 0.97, 0.10 };
 Double_t green[NRGBs] = { 1.00, 0.97, 0.30, 0.40, 0.97, 0.00, 0.00 };
 Double_t blue[NRGBs]  = { 1.00, 0.97, 0.97, 0.00, 0.00, 0.00, 0.00 };
 TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);

 LauraStyle->SetErrorX(0.0001);

 gROOT->SetStyle("LauraStyle");

 // Force Root to run LauraStyle
 gROOT->ForceStyle();

}
