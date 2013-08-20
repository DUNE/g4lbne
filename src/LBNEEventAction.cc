//
//LBNEEventAction.cc
//

//C++
#include <string>
 
#include "LBNEEventAction.hh"
#include "LBNERunManager.hh"
#include "LBNEAnalysis.hh"
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
   pRunManager=(LBNERunManager*)LBNERunManager::GetRunManager();

   if(pRunManager->GetVerboseLevel() > 0)
   {
      std::cout << "LBNEEventAction Constructor Called." << std::endl;
   }
}

//-------------------------------------------------------------------------------------
LBNEEventAction::~LBNEEventAction()
{

   if(pRunManager->GetVerboseLevel() > 0)
   {
      std::cout << "LBNEEventAction Destructor Called." << std::endl;
   }
}

//-------------------------------------------------------------------------------------
void LBNEEventAction::BeginOfEventAction(const G4Event* evt)
{
   if(pRunManager->GetVerboseLevel() > 1) 
   { 
      G4cout << "LBNEEventAction::BeginOfEventAction called...Beginning Event #" << evt-> GetEventID() << G4endl;
   }
      
}


//-------------------------------------------------------------------------------------
void LBNEEventAction::EndOfEventAction(const G4Event* evt)
{
  if(pRunManager->GetVerboseLevel() > 1) 
    { 
      G4cout << "LBNEEventAction::EndOfEventAction called...Ending Event # " << evt-> GetEventID() << G4endl;
    }


}

