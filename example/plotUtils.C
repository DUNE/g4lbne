void add_plot_label( const char* label, double x, double y, double size = 0.05, int color = 1, int font = 62, int align = 22 ){

  TLatex *latex = new TLatex( x, y, label );
  latex->SetNDC();
  latex->SetTextSize(size);
  latex->SetTextColor(color);
  latex->SetTextFont(font);
  latex->SetTextAlign(align);
  latex->Draw();
}


void stringReplaceAll(std::string& str, std::string& from, std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

std::vector<std::string> &split(std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

void drawRatio( TH1* hist1, TH1 *hist2, std::string ytitle, std::string opts=""){

  TH1* tmp1 = (TH1*)hist1->Clone("tmp1");
  TH1* tmp2 = (TH1*)hist2->Clone("tmp2");

  if( tmp1->GetSumw2N() == 0 ) tmp1->Sumw2();
  if( tmp2->GetSumw2N() == 0 )   tmp2->Sumw2();

  TH1 *ratio = (TH1*)tmp2->Clone("ratio");
  ratio->Divide(tmp1, tmp2);

  ratio->SetLineColor(tmp1->GetLineColor());

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

  ratio->SetMinimum(0.8);
  ratio->SetMaximum(1.2);

  ratio->DrawCopy((opts+"X0").c_str());

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
