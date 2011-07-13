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
  :PrimaryAction (RA)
{
   LBNEDataInput *LBNEData = LBNEDataInput::GetLBNEDataInput();
   
   if(LBNEData->GetDebugLevel() > 0)
   {
      G4cout << "LBNEPrimaryMessenger Constructor Called." << G4endl;
   }
  
}

LBNEPrimaryMessenger::~LBNEPrimaryMessenger()
{

}

void LBNEPrimaryMessenger::SetNewValue(G4UIcommand* /*command*/,G4String /*newValues*/)
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

}

