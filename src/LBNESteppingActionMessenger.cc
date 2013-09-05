#include "LBNESteppingActionMessenger.hh"

#include "LBNESteppingAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4ios.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "G4RunManager.hh"

LBNESteppingActionMessenger::LBNESteppingActionMessenger(LBNESteppingAction* RA)
  :SteppingAction (RA)
{
     //
  //LBNE/stepping
  //
  StepDir = new G4UIdirectory("/LBNE/stepping/");
  StepDir->SetGuidance("In Step analysis and  control.");
  
  SimulationNameCmd  = new G4UIcmdWithAString("/LBNE/stepping/name",this);
  SimulationNameCmd->SetGuidance("Name for the study in question");
  SimulationNameCmd->SetParameterName("Name",true);
  SimulationNameCmd->SetDefaultValue ("Geom101");
  SimulationNameCmd->AvailableForStates(G4State_Idle);
    
  OutputASCIIFileNameCmd  = new G4UIcmdWithAString("/LBNE/stepping/filename",this);
  OutputASCIIFileNameCmd->SetGuidance("Ascii file Name for a plain ASCII file with positions of the geantino  ");
  OutputASCIIFileNameCmd->SetParameterName("FileName",true);
  OutputASCIIFileNameCmd->SetDefaultValue ("./steppingActionOut.txt");
  OutputASCIIFileNameCmd->AvailableForStates(G4State_Idle);
    
}
LBNESteppingActionMessenger::~LBNESteppingActionMessenger() {

  delete SimulationNameCmd; 
  delete OutputASCIIFileNameCmd;
  delete StepDir;
}

void LBNESteppingActionMessenger::SetNewValue(G4UIcommand* command,G4String newValues) {

   if (command == SimulationNameCmd)
   { 
      G4cout << "\n---> Stepping Output info Data Set Name " << newValues << G4endl;
      
   } else if(command == OutputASCIIFileNameCmd) {
     SteppingAction->OpenAscii(newValues.c_str());
   }

}
