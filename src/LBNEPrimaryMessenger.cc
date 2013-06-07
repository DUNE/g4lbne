#include "LBNEPrimaryMessenger.hh"
#include "LBNEDataInput.hh"

#include "LBNEPrimaryGeneratorAction.hh"
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

LBNEPrimaryMessenger::LBNEPrimaryMessenger(LBNEPrimaryGeneratorAction* RA)
  :fPrimaryAction (RA)
{
   LBNEDataInput *LBNEData = LBNEDataInput::GetLBNEDataInput();
   
   if(LBNEData->GetDebugLevel() > 0)
   {
      G4cout << "LBNEPrimaryMessenger Constructor Called." << G4endl;
   }
  fDirectory = new G4UIdirectory("/LBNE/generator/");

  fBeamlineThetaCmd =
    new G4UIcmdWithADoubleAndUnit("/LBNE/generator/beamTheta",this);
  fBeamlineThetaCmd->SetGuidance("Set the angle (theta) of the proton beam");
  fBeamlineThetaCmd->SetUnitCategory("Angle");
  fBeamlinePhiCmd =
    new G4UIcmdWithADoubleAndUnit("/LBNE/generator/beamPhi",this);
  fBeamlinePhiCmd->SetGuidance("Set the angle (phi) of the proton beam.");
  fBeamlinePhiCmd->SetUnitCategory("Angle");
}

LBNEPrimaryMessenger::~LBNEPrimaryMessenger()
{
  delete fBeamlinePhiCmd;
  delete fBeamlineThetaCmd;
  delete fDirectory;
}

void LBNEPrimaryMessenger::SetNewValue(G4UIcommand* cmd, G4String val)
{
   LBNEDataInput *LBNEData = LBNEDataInput::GetLBNEDataInput();

   if(LBNEData->GetDebugLevel() > 1)
   {
      G4cout << "LBNEPrimaryMessenger::SetNewValue - Setting Parameter value from input macro." << G4endl;
   }


   if(LBNEData->GetDebugLevel() > 1)
   {
      G4cout << "LBNEPrimaryMessenger::SetNewValue - Done Setting parameter value." << G4endl;
   }

  if(cmd == fBeamlineThetaCmd){
    fPrimaryAction->SetBeamTheta(fBeamlineThetaCmd->GetNewDoubleValue(val));   
  }
  if(cmd == fBeamlinePhiCmd){
    fPrimaryAction->SetBeamPhi(fBeamlinePhiCmd->GetNewDoubleValue(val));   
  }
}

