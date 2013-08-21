void makeFluxHistograms(std::string simulation="G4PBeam", std::string version = "v2r4p1", std::string geometry = "Nominal_FHC", std::string location = "LBNEFD", std::string physics_list = "QGSP_BERT", std::string n_files = "250", std::string pot_per_file = "100000") {

  gROOT->ProcessLine(".L eventRates.C++");

  std::string command = "eventRates t(\""+simulation+"\", \""+version+"\", \""+geometry+"\", \""+location+"\", \""+physics_list+"\", "+n_files+", "+pot_per_file+")";

  std::cout<<"Executing: "<<command<<std::endl;

  gROOT->ProcessLine(command.c_str());

  gROOT->ProcessLine("t.Loop()");
}
