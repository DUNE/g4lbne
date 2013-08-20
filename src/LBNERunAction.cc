//
// LBNERunAction.cc
//

#include "G4Run.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "LBNERunAction.hh"
#include "LBNERunActionMessenger.hh"
#include "LBNEAnalysis.hh"
#include "LBNETrajectory.hh"
#include "Randomize.hh"
#include "LBNERunManager.hh"
#include "LBNEPrimaryGeneratorAction.hh"
#include "G4ProcessTable.hh"

//------------------------------------------------------------------------------
LBNERunAction::LBNERunAction():
fVerboseLevel(1)
{
   
   if(fVerboseLevel > 0)
   {
      std::cout << "LBNERunAction Constructor Called." << std::endl;
   }

   
   runMessenger = new LBNERunActionMessenger(this);
}

//------------------------------------------------------------------------------
LBNERunAction::~LBNERunAction()
{ 
   if(fVerboseLevel > 0)
   {
      std::cout << "LBNERunAction Destructor Called." << std::endl;
   }

  delete runMessenger;
}

//------------------------------------------------------------------------------
void LBNERunAction::BeginOfRunAction(const G4Run* aRun)
{
    if(fVerboseLevel > 0)
   {
      std::cout << "LBNERunAction::BeginOfRunAction() Called." << std::endl;
   }

   
   LBNERunManager *pRunManager = (LBNERunManager*)LBNERunManager::GetRunManager();

   G4String spaces = "   ";
   //std::cout << std::endl;
   std::cout << "LBNERunAction::BeginOfRunAction() - Initializing Run " 
	     << aRun->GetRunID() << "..." << std::endl;
   
   //std::cout << spaces << "Preparing \"" <<  fLBNEData -> GetSimulation() << "\" Simulation." << std::endl;

   //
   //Random number generator
   //
   G4String randomFile="rndm/beginOfRun_";
   char runN[4];
   sprintf(runN,"%03d",aRun->GetRunID());
   randomFile.append(runN);
   randomFile.append(".rndm");
   CLHEP::HepRandom::saveEngineStatus(randomFile);
   std::cout << spaces << "Intializing Random Number generator... " << std::endl
	     << spaces << "   Seed = " << CLHEP::HepRandom::getTheSeed() << std::endl
	     << spaces << "   Saving Random engine status in "<< randomFile << std::endl;
   //


   //
   //Open input ntuples set Number of Events to loop over
   //
//   LBNEPrimaryGeneratorAction* primaryGeneratorAction = dynamic_cast<LBNEPrimaryGeneratorAction*>(pRunManager->userPrimaryGeneratorAction);
//   const LBNEPrimaryGeneratorAction* primaryGeneratorAction = 
//      dynamic_cast<const LBNEPrimaryGeneratorAction*>(pRunManager->GetUserPrimaryGeneratorAction());

   LBNEPrimaryGeneratorAction* primaryGeneratorAction = 
      (pRunManager->GetLBNEPrimaryGeneratorAction());

   
   if(!primaryGeneratorAction)
   {
      std::cout << spaces << "PROBLEM: INVALID LBNEPrimaryGeneratorAction POINTER" << std::endl;
//      fLBNEData -> SetOKToRun(false);
      return;
   }

//   if(fLBNEData->GetSimulation() == "Standard Neutrino Beam" ||
//      fLBNEData->GetSimulation() == "Target Tracking"        ||
//      fLBNEData->GetSimulation() == "Horn 1 Tracking"    ||
//      fLBNEData->GetSimulation() == "Horn 2 Tracking" )
   {

    LBNERunManager* theRunManager = dynamic_cast<LBNERunManager*>(G4RunManager::GetRunManager());
      if (theRunManager->GetUseFlukaInput() || theRunManager->GetUseMarsInput()) 
      {
	 G4bool ntpOpen = primaryGeneratorAction->OpenNtuple(theRunManager->GetNptInputFileName());
//	 
	 if(!ntpOpen)
	 {
	    std::cout << spaces << "PROBLEM: FAILED TO OPEN INPUT NTUPLE. Fatal " << std::endl;
	    exit(2);
	    return;
	 } 
//	 
	 std::cout << spaces << "Successfully opened input ntuple \"" 
		   << theRunManager->GetNptInputFileName() << "\"" << std::endl;
//	 
	theRunManager->SetNumberOfEventsLBNE(primaryGeneratorAction->GetNoOfPrimaries());
	// Note there are public accessors in geant4 v4.9.6 inside the G4RunManager... but not in v4.9.3... 
//	 
      }
      else
      {
	 primaryGeneratorAction->SetProtonBeam();

      }
      
   }
   //

   //
   //If still ok to run open output file
   //

   LBNEAnalysis* analysis = LBNEAnalysis::getInstance();
   LBNERunManager* theRunManager = dynamic_cast<LBNERunManager*>(G4RunManager::GetRunManager());
   if(theRunManager -> GetCreateOutput()) 
      if(!(analysis->CreateOutput())) {
	    std::cout << spaces << "PROBLEM: FAILED TO OPEN OUTPUT NTUPLE. Fatal " << std::endl;
	    exit(2);
      } 
      
   std::cout << std::endl;
   
   std::cout << "LBNERunAction::BeginOfRunAction() - ...completed run initializtion. " << std::endl;
   std::cout << std::endl;

  
}

//------------------------------------------------------------------------------
void LBNERunAction::EndOfRunAction(const G4Run* aRun)
{

   LBNERunManager* theRunManager = dynamic_cast<LBNERunManager*>(G4RunManager::GetRunManager());
    if(theRunManager->GetVerboseLevel() > 0)
   {
      std::cout << "LBNERunAction::EndOfRunAction() Called." << std::endl;
   }

   LBNERunManager *pRunManager = (LBNERunManager*)LBNERunManager::GetRunManager();

   G4String spaces = "   ";
   std::cout << std::endl;
   std::cout << "LBNERunAction::EndOfRunAction() - Terminating Run " 
	     << aRun->GetRunID() << "..." << std::endl;

   //
   //Random number generator
   //
   G4String randomFile="rndm/endOfRun_";
   char runN[4];
   sprintf(runN,"%03d",aRun->GetRunID());
   randomFile.append(runN);
   randomFile.append(".rndm");
   CLHEP::HepRandom::saveEngineStatus(randomFile);
   std::cout << spaces << "Closing Random Number generator... " << std::endl
	     << spaces << "   Seed = " << CLHEP::HepRandom::getTheSeed() << std::endl
	     << spaces << "   Random engine status saved in "<< randomFile << std::endl;
   //
   
   //
   //Close Input and Output Ntuple
   //
      
   std::cout << spaces << "Closing Input File... " << std::endl;
   LBNEPrimaryGeneratorAction* primaryGeneratorAction = (pRunManager->GetLBNEPrimaryGeneratorAction());
   
   if(primaryGeneratorAction)
   {
      primaryGeneratorAction->CloseNtuple();
   }
   
   if(theRunManager -> GetCreateOutput()) 
   {
      std::cout << spaces << "Closing Output File... " << std::endl;
      LBNEAnalysis* analysis = LBNEAnalysis::getInstance();
      analysis->CloseOutput();
   }
   //
   
   std::cout << "LBNERunAction::EndOfRunAction() - ...completed run termination. " << std::endl;
   std::cout << std::endl;

/*

   G4cout << G4endl;
  G4cout << G4endl;
  G4cout << "********************************************************************" << G4endl;
  G4cout << "********************************************************************" << G4endl;
  G4cout << "LBNERunAction::EndOfRunAction..." << G4endl;
  G4cout << "********************************************************************" << G4endl;

  G4String randomFile="rndm/endOfRun_";
  char runN[4];
  sprintf(runN,"%04d",aRun->GetRunID());
  randomFile.append(runN);
  randomFile.append(".rndm");
  CLHEP::HepRandom::saveEngineStatus(randomFile);
  G4cout << "  Random engine status at the end of the run saved in "<<randomFile<<G4endl;
  LBNEAnalysis* analysis = LBNEAnalysis::getInstance();
  analysis->finish();

  G4cout << "********************************************************************" << G4endl;
  G4cout << "LBNERunAction::EndOfRunAction - Completed." << G4endl;
  G4cout << "********************************************************************" << G4endl;
  G4cout << "********************************************************************" << G4endl;
  G4cout << G4endl;
  G4cout << G4endl;
*/
}


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void LBNERunAction::CheckOKToRun()
{
   //check the simulation
   // Obsolte..
   /*
   G4bool knownSim = false;
   const vstring_t simvec = fLBNEData -> GetListOfSimulations();
   const G4String simulation = fLBNEData -> GetSimulation();
   
   for(vstring_t::const_iterator sit = simvec.begin(); sit != simvec.end(); ++sit)
   {
      if (simulation == *sit) { knownSim = true; break;}
   }

   if(knownSim) { fLBNEData -> SetOKToRun(true);}
   else
   { 
      std::cout << "LBNERunAction::CheckOKToRun() - PROBLEM: Simulation "
		<< "\"" << simulation << "\"" << " unknown. Currently "
		<< "implemented simulations are " << std::endl;

      for(vstring_t::const_iterator sit = simvec.begin(); sit != simvec.end(); ++sit)
      {
	 std::cout << "     " << *sit << std::endl;
      }
      std::cout << std::endl;

      fLBNEData -> SetOKToRun(false);
   }
   */
   //
}



















