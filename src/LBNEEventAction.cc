//
//LBNEEventAction.cc
//

//C++
#include <string>
#include <stdlib.h>

#include "LBNEEventAction.hh"
#include "LBNERunManager.hh"
#include "LBNEAnalysis.hh"
#include "LBNETrajectory.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
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
      G4cout << "LBNEEventAction::BeginOfEventAction called...Beginning Event #" << evt->GetEventID() << G4endl;
   }
   // This code does not work.. Need to be studied.. 
//   if (evt->GetEventID() == -2222) {
//    
//     CLHEP::HepRandom::restoreEngineStatus("./rndm/beginRun_103Evt_32135.rndm");
//     std::cerr << " Reloaded random from 103Evt_32135 " << std::endl;
//   }
// 
//   if (evt->GetEventID() > 32130) {
//     const G4Run* theRun = pRunManager->GetCurrentRun();
//     std::ostringstream fOutRndmStrStr; 
//     fOutRndmStrStr << "./rndm/beginRun_" << theRun->GetRunID() << "Evt_" << evt->GetEventID() << ".rndm";
//     std::string fOutRndmStr(fOutRndmStrStr.str());
//     CLHEP::HepRandom::saveEngineStatus(fOutRndmStr.c_str());
     // delete the previous previous file 
//     std::ostringstream fOutRndmPStrStr; 
//     fOutRndmPStrStr << "rm -f ./rndm/beginRun_" << theRun->GetRunID() << "Evt_" << evt->GetEventID()-2 << ".rndm";
//     std::string fOutRndmPStr(fOutRndmPStrStr.str());
//     system(fOutRndmPStr.c_str());
     // Also get the seeds May this what we have to save... 
//     std::cerr << " The seed is now " << CLHEP::HepRandom::getTheSeed() << std::endl;
//   }
}


//-------------------------------------------------------------------------------------
void LBNEEventAction::EndOfEventAction(const G4Event* evt)
{
  if(pRunManager->GetVerboseLevel() > 1) 
    { 
      G4cout << "LBNEEventAction::EndOfEventAction called...Ending Event # " << evt-> GetEventID() << G4endl;
    }


}

