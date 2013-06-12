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

  fBeamOffsetXCmd = 
    new G4UIcmdWithADoubleAndUnit("/LBNE/generator/beamOffsetX",this);
  fBeamOffsetXCmd->SetGuidance("Set the X offset of the proton beam");
  fBeamOffsetXCmd->SetParameterName("beamOffsetX", false);
  fBeamOffsetXCmd->SetUnitCategory("Length");
  
  fBeamOffsetYCmd = 
    new G4UIcmdWithADoubleAndUnit("/LBNE/generator/beamOffsetY",this);
  fBeamOffsetYCmd->SetGuidance("Set the Y offset of the proton beam");
  fBeamOffsetYCmd->SetParameterName("beamOffsetY", false);
  fBeamOffsetYCmd->SetUnitCategory("Length");
  
  fBeamThetaCmd =
    new G4UIcmdWithADoubleAndUnit("/LBNE/generator/beamTheta",this);
  fBeamThetaCmd->SetGuidance("Set the angle (theta) of the proton beam");
  fBeamThetaCmd->SetParameterName("beamTheta", false);
  fBeamThetaCmd->SetUnitCategory("Angle");
  
  fBeamPhiCmd =
    new G4UIcmdWithADoubleAndUnit("/LBNE/generator/beamPhi",this);
  fBeamPhiCmd->SetGuidance("Set the angle (phi) of the proton beam.");
  fBeamPhiCmd->SetParameterName("beamPhi", false);
  fBeamPhiCmd->SetUnitCategory("Angle");

  fCorrectForAngleCmd = 
    new G4UIcmdWithABool("/LBNE/generator/correctForAngle", this);
  fCorrectForAngleCmd->SetGuidance("If true, beam x/y position is corrected");
  fCorrectForAngleCmd->SetGuidance("to hit center of target using angle of");
  fCorrectForAngleCmd->SetGuidance("beam. Offsets in x and y specified via");
  fCorrectForAngleCmd->SetGuidance("messenger are still respected.");
  
  fBeamOnTargetCmd = 
    new G4UIcmdWithABool("/LBNE/generator/beamOnTarget", this);
  fBeamOnTargetCmd->SetGuidance("If true, forces beam to hit the center");
  fBeamOnTargetCmd->SetGuidance("of target. Any x or y offsets supplied");
  fBeamOnTargetCmd->SetGuidance("via messenger are ignored");
  

}

LBNEPrimaryMessenger::~LBNEPrimaryMessenger()
{
  delete fBeamOffsetXCmd;
  delete fBeamOffsetYCmd;
  delete fBeamPhiCmd;
  delete fBeamThetaCmd;
  delete fCorrectForAngleCmd;
  delete fBeamOnTargetCmd;
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

  if(cmd == fBeamOffsetXCmd){
    fPrimaryAction->SetBeamOffsetX(fBeamOffsetXCmd->GetNewDoubleValue(val));   
  }
  if(cmd == fBeamOffsetYCmd){
    fPrimaryAction->SetBeamOffsetY(fBeamOffsetYCmd->GetNewDoubleValue(val));   
  }
  if(cmd == fBeamThetaCmd){
    fPrimaryAction->SetBeamTheta(fBeamThetaCmd->GetNewDoubleValue(val));   
  }
  if(cmd == fBeamPhiCmd){
    fPrimaryAction->SetBeamPhi(fBeamPhiCmd->GetNewDoubleValue(val));   
  }
  if(cmd == fCorrectForAngleCmd){
    fPrimaryAction->SetCorrectForAngle(fCorrectForAngleCmd->GetNewBoolValue(val));
  }
  if(cmd == fBeamOnTargetCmd){
    fPrimaryAction->SetBeamOnTarget(fBeamOnTargetCmd->GetNewBoolValue(val));
  }
}

