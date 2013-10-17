void makeFluxHistograms(std::string version = "v3r0p1", std::string macro = "Nominal", std::string current = "200kA", std::string location = "LBNEFD", std::string physics_list = "QGSP_BERT", std::string n_files = "250", std::string pot_per_file = "100000") {

  gROOT->ProcessLine(".L eventRates.C++");

  std::string command = "eventRates t(\""+version+"\", \""+macro+"\", \""+current+"\", \""+location+"\", \""+physics_list+"\", "+n_files+", "+pot_per_file+")";

  std::cout<<"Executing: "<<command<<std::endl;

  gROOT->ProcessLine(command.c_str());

  gROOT->ProcessLine("t.Loop()");
}
