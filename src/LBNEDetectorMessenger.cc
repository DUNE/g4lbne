//----------------------------------------------------------------------
// $Id
//----------------------------------------------------------------------

#include "LBNEDetectorMessenger.hh"
#include "LBNEDetectorConstruction.hh"
#include "LBNEDataInput.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UnitsTable.hh"

LBNEDetectorMessenger::LBNEDetectorMessenger( LBNEDetectorConstruction* LBNEDet):LBNEDetector(LBNEDet)
{

   LBNEDataInput *LBNEData = LBNEDataInput::GetLBNEDataInput();
   
   if(LBNEData->GetDebugLevel() > 0)
   {
      G4cout << "LBNEDetectorMessenger Constructor Called." << G4endl;
   }
   
   LBNEDir = new G4UIdirectory("/LBNE/");
   LBNEDir->SetGuidance("UI commands for detector geometry");
   
   detDir = new G4UIdirectory("/LBNE/det/");
   detDir->SetGuidance("detector control");
   
   
   ConstructTarget = new G4UIcmdWithABool("/LBNE/det/constructTarget",this); 
   ConstructTarget->SetGuidance("Target construction on/off"); 
   ConstructTarget->SetParameterName("constructTarget",true); 
   ConstructTarget->AvailableForStates(G4State_PreInit,G4State_Idle);
   
   UpdateCmd = new G4UIcmdWithoutParameter("/LBNE/det/update",this);
   UpdateCmd->SetGuidance("Update LBNE geometry.");
   UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
   UpdateCmd->SetGuidance("if you changed geometrical value(s).");
   UpdateCmd->AvailableForStates(G4State_Idle);
   
   
	new G4UnitDefinition("kiloampere" , "kA", "Electric current", 1000.*ampere);
        
        

        
	
}

LBNEDetectorMessenger::~LBNEDetectorMessenger() 
{

   delete detDir;
   delete LBNEDir;
   delete ConstructTarget;
   delete UpdateCmd;
}


void LBNEDetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{

   LBNEDataInput *LBNEData = LBNEDataInput::GetLBNEDataInput();

   if(LBNEData->GetDebugLevel() > 0)
   {
      G4cout << "LBNEDetectorMessenger::SetNewValue - Setting Parameter value from input macro." << G4endl;
   }

   
   
   if ( command == ConstructTarget ) 
   {
      LBNEData->SetConstructTarget(ConstructTarget->GetNewBoolValue(newValue));
   }

   
   if ( command == UpdateCmd ) 
   {
      LBNEDetector->UpdateGeometry();
      return;
   }
   
}
	
	
