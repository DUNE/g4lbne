#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>

  class AlignmentVariation {

    std::string m_variation_type; // macro, location, or current
    
    std::string m_quantity_varied;
    std::vector<std::string> m_variation_names;
    std::string m_units;
    
    std::string m_version; // e.g. v3r0p4
    std::string m_nominal_version;
    
    std::string m_user;
    std::string m_nominal_user;

    std::string m_nominal_plist; 
    std::string m_nominal_current;

    std::string m_tolerance;

    std::string m_cv;

    std::string m_disk;
    
    double m_scale_factor;

  public:
    AlignmentVariation(std::string quantity,
		       std::string type, 
		       std::string version) {
      
      m_variation_type = type;
      m_quantity_varied = quantity;

      m_version = version;
      m_nominal_version = version;

      m_user = std::getenv("USER");
      m_nominal_user = std::getenv("USER");

      m_nominal_plist = "QGSP_BERT";
      m_nominal_current = "200kA";      

      m_tolerance = "";

      m_cv = "0";
      
      m_scale_factor = 1.0;

      m_disk = "/lbne/data/";
    }
   
    void SetDisk(std::string disk) {
     m_disk = disk;
    }


    void SetUnits(std::string units) {
      m_units=units;
    }

    void SetTolerance(std::string tolerance) {
      m_tolerance=tolerance;
    }

    void SetNominalVersion(std::string v) {
      m_nominal_version = v;
    }

    void SetUser(std::string u) {
      m_user = u;
    }

    void SetNominalUser(std::string u) {
      m_nominal_user = u;
    }

    void SetCV(std::string cv) {
      m_cv = cv;
    }

    void SetScaleFactor(double scale_factor = 1) {
      m_scale_factor = scale_factor;
    }

    void AddVariation(std::string variation) {
      m_variation_names.push_back(variation);
    }

    std::string GetVariedQuantity() {
      return m_quantity_varied;
    }

    std::string GetVariationType() {
      return m_variation_type;
    }

    std::vector<std::string> GetVariationNames() {
      return m_variation_names;
    }

    std::string GetUnits() {
      return m_units;
    }

    std::string GetTolerance() {
      return m_tolerance;
    }

    std::string GetNominalHistoFile(std::string beam_mode = "nu",std::string loc="LBNEFD",bool fastmc=false) {

      std::string temp_current = m_nominal_current;
      if(beam_mode=="antinu") temp_current = "-"+m_nominal_current;

      std::string fmc = "";
      if(fastmc) fmc="_fastmc";

      std::string the_file = m_disk+"/users/"+m_nominal_user+"/fluxfiles/g4lbne/"+m_nominal_version;

      the_file += "/"+m_nominal_plist+"/Nominal/"+temp_current+"/flux/";
      the_file += "histos_g4lbne_"+m_nominal_version+"_"+m_nominal_plist+"_Nominal_"+temp_current+"_"+loc+fmc+".root";

      std::cout<<"Nominal file: "<<the_file<<std::endl;
      return the_file;
    }

    std::vector<std::string> GetVariedHistoFiles(std::string beam_mode = "nu",std::string loc="LBNEFD",bool fastmc=false) {

      std::string fmc = "";
      if(fastmc) fmc="_fastmc";
      
      std::vector<std::string> the_files;

      std::cout<<"Looking up flux histograms for variation "<<m_quantity_varied<<" which has type "<<m_variation_type<<std::endl;

      if(m_variation_type=="shift") {

	// "shift" type variations don't have multiple variations.  Example: baffle scraping involves shifting the nominal flux by a fraction of the flux from the baffle

	std::string temp_current = m_nominal_current;
	if(beam_mode=="antinu") temp_current = "-"+m_nominal_current;
	
	std::string the_file = m_disk+"/users/"+m_user+"/fluxfiles/g4lbne/"+m_version;
	the_file += "/"+m_nominal_plist+"/"+m_quantity_varied+m_units+"/"+temp_current+"/flux/";
	the_file += "histos_g4lbne_"+m_version+"_"+m_nominal_plist+"_"+m_quantity_varied+m_units+"_"+temp_current+"_"+loc+fmc+".root";
	the_files.push_back(the_file);
      }
      else
	for(unsigned int i = 0; i<m_variation_names.size(); i++) {
	  std::string variation = m_quantity_varied+m_variation_names[i]+m_units;
	if(m_variation_type=="current")
	  variation = m_variation_names[i]+m_units;

	std::string the_file = m_disk+"/users/"+m_user+"/fluxfiles/g4lbne/"+m_version;
	
	if(m_variation_type=="macro") {
	  std::string temp_current = m_nominal_current;
	  if(beam_mode=="antinu") temp_current = "-"+m_nominal_current;

	  the_file += "/"+m_nominal_plist+"/"+variation+"/"+temp_current+"/flux/";
	  the_file += "histos_g4lbne_"+m_version+"_"+m_nominal_plist+"_"+variation+"_"+temp_current+"_"+loc+fmc+".root";
	}
	else if(m_variation_type=="current") {
	  std::string current_sign = "";
	  if(beam_mode=="antinu") current_sign = "-";

	  the_file += "/"+m_nominal_plist+"/Nominal/"+current_sign+variation+"/flux/";
	  the_file += "histos_g4lbne_"+m_nominal_version+"_"+m_nominal_plist+"_Nominal_"+current_sign+variation+"_"+loc+fmc+".root";
	}
	else if(m_variation_type=="location") {
	  std::string temp_current = m_nominal_current;
	  if(beam_mode=="antinu") temp_current = "-"+m_nominal_current;
	  
	  if(m_quantity_varied.find(loc)==0)
	    loc = m_quantity_varied+m_variation_names[i];
	  the_file += "/"+m_nominal_plist+"/Nominal/"+temp_current+"/flux/";
	  the_file += "histos_g4lbne_"+m_nominal_version+"_"+m_nominal_plist+"_Nominal_"+temp_current+"_"+loc+fmc+".root";
	}
	else {
	  std::cout<<"Variation type"<<m_variation_type<<" not yet supported"<<std::endl;
	  return the_files;
	}
	the_files.push_back(the_file);
	std::cout<<"varied file "<<the_file<<std::endl;
      }
      return the_files;
    }

    std::string GetCV() { return m_cv; }

    double GetScaleFactor() { return m_scale_factor; }

    std::string GetUser() { return m_user; }

  };
    

