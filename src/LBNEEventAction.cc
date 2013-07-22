//
//LBNEEventAction.cc
//

//C++
#include <string>
 
#include "LBNEEventAction.hh"
#include "LBNERunManager.hh"
#include "LBNEAnalysis.hh"
#include "LBNEDataInput.hh"
#include "LBNETrajectory.hh"

#include "G4Event.hh"
#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"

//------------------------------------------------------------------------------------- 
LBNEEventAction::LBNEEventAction()
{
   LBNEData = LBNEDataInput::GetLBNEDataInput();
   pRunManager=(LBNERunManager*)LBNERunManager::GetRunManager();

   if(LBNEData->GetDebugLevel() > 0)
   {
      std::cout << "LBNEEventAction Constructor Called." << std::endl;
   }
}

//-------------------------------------------------------------------------------------
LBNEEventAction::~LBNEEventAction()
{

   if(LBNEData->GetDebugLevel() > 0)
   {
      std::cout << "LBNEEventAction Destructor Called." << std::endl;
   }
}

//-------------------------------------------------------------------------------------
void LBNEEventAction::BeginOfEventAction(const G4Event* evt)
{
   if(LBNEData->GetDebugLevel() > 1) 
   { 
      G4cout << "LBNEEventAction::BeginOfEventAction called...Beginning Event #" << evt-> GetEventID() << G4endl;
   }
   LBNEAnalysis* analysis = LBNEAnalysis::getInstance();
   analysis->ResetEvent();
      
}


//-------------------------------------------------------------------------------------
void LBNEEventAction::EndOfEventAction(const G4Event* evt)
{
  if(LBNEData->GetDebugLevel() > 1) 
    { 
      G4cout << "LBNEEventAction::EndOfEventAction called...Ending Event # " << evt-> GetEventID() << G4endl;
    }
  
  LBNEAnalysis* analysis = LBNEAnalysis::getInstance();
  analysis->FillEvent();

}

