#include <getopt.h>

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4DecayTable.hh"
#include "G4KL3DecayChannel.hh"
#include "G4StepLimiter.hh"
#include "G4ParticleTable.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4VEnergyLossProcess.hh"

#include "LBNEDetectorConstruction.hh"

// Interaction Physics Lists
#include "G4PhysListFactory.hh"

#include "LBNEPrimaryGeneratorAction.hh"
#include "LBNEEventAction.hh"
#include "LBNEStackingAction.hh"
#include "LBNESteppingAction.hh"
#include "LBNETrackingAction.hh"
#include "LBNERunAction.hh"
#include "LBNERunManager.hh"
#include "LBNESurveyor.hh"
#include "LBNEVolumePlacements.hh"
#include "LBNEPlacementMessenger.hh"


#ifdef G4VIS_USE
#include "LBNEVisManager.hh"
#endif


void DisplayHelp()
{
  // Helpful help message goes here
  G4cout << "" << G4endl;
  G4cout << "-------------------------------------------------" << G4endl;
  G4cout << "G4LBNE Help" << G4endl;
  G4cout << "Usage:" << G4endl;
  G4cout << "  /path/to/g4lbne --input /path/to/inputfile ";
  G4cout << "--physicslist physlistname /path/to/macrofile" << G4endl;
  G4cout << "ALTERNATELY: " << G4endl;
  G4cout << "  /path/to/g4lbne -i /path/to/inputfile -p physlistname ";
  G4cout << "/path/to/macrofile " << G4endl;
  G4cout << "The default physics list is QGSP_BERT" << G4endl;
  G4cout << "The options --help or -h display this message " << G4endl;
  G4cout << "-------------------------------------------------" << G4endl;
  G4cout << "" << G4endl;
}

int main(int argc,char** argv)
{

  G4String macroFileName = "";
  G4String inputFileName = "";
  G4String physListName = "QGSP_BERT"; // default
  
  // Parse Arguments
  int c;
  while(1){
    int option_index = 0;
    static struct option long_options[] = {
      {"help", 0, 0, 0},
      {"input", required_argument, 0, 0},
      {"physicslist", required_argument, 0, 0},
      {0,0,0,0}
    };
    c = getopt_long(argc, argv, "hi:p:", long_options, &option_index);
    if(c == -1) break;
    switch (c) {
      case 0:
        switch(option_index){
          case 0: // --help
            DisplayHelp();
            exit(0);
          case 1: // --input
            inputFileName = optarg;
            break;
          case 2: // --physicslist
            physListName = optarg;
            break;
        }
        break;
      case 'h':
        DisplayHelp();
        exit(0);
        break;
      case 'i':
        inputFileName = optarg;
        break;
      case 'p':
        physListName = optarg;
        break;
    }  
  }
  if(optind < argc){ 
    G4cout << "Setting macroFile to " << argv[argc-1] << G4endl;
    macroFileName = argv[argc-1];
  }

  // Invoke both the Surveoyr class and the Volume placement 
  // such the internal variable are define and the messenger are set 
  // prior to declare a run 
  
  LBNESurveyor::Instance();
//  std::cerr << " LBNESurveyor created  " << std::endl;
  
  LBNEVolumePlacements::Instance();
  LBNEPlacementMessenger::Instance();
  
// The class that sets up the geometry.. 
  
   LBNEDetectorConstruction *theDetector = new LBNEDetectorConstruction;
   
  // get the pointer to the UI manager and set verbosities
  G4UImanager* UI = G4UImanager::GetUIpointer();

  // Construct the default run manager
  G4RunManager* runManager = new LBNERunManager();

  runManager->SetUserInitialization(theDetector);
  // set user action classes
  
#ifdef G4VIS_USE
  // Visualization, if you choose to have it!
  G4VisManager* visManager = new LBNEVisManager;
  visManager->Initialize();
#endif

  if(argc==1)
  // Define (G)UI terminal for interactive mode  
  { 
    G4UIsession * session = 0;

    // G4UIterminal is a (dumb) terminal.
#ifdef G4UI_USE_TCSH
      session = new G4UIterminal(new G4UItcsh);      
#else
      session = new G4UIterminal();
#endif    

    session->SessionStart();
    delete session;
  }
  else  // Batch mode
  { 
    G4String command = "/control/execute ";
    G4cout << "Executing " << command+macroFileName << G4endl;
    UI->ApplyCommand(command+macroFileName);
  }

#  // job termination
  G4cout<<"Delete RunManager"<<G4endl;
  delete runManager;
  return 0;
}


