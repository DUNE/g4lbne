//----------------------------------------------------------------------
// LBNERunManager.cc
// $Id: LBNERunManager.cc,v 1.1.1.1 2011/07/13 16:20:52 loiacono Exp $
//----------------------------------------------------------------------

#include "G4RunManager.hh"
#include "LBNERunManager.hh"
#include "LBNEDataInput.hh"
#include "TStopwatch.h"
#include "TTime.h"
#include "LBNEPrimaryGeneratorAction.hh"

//------------------------------------------------------------------------------------
LBNERunManager::LBNERunManager()
//:primaryGeneratorAction(0)
{
   LBNEData = LBNEDataInput::GetLBNEDataInput();
   if(LBNEData->GetDebugLevel() > 0)
   {
      std::cout << "LBNERunManager Constructor Called." << std::endl;
   }
}
//------------------------------------------------------------------------------------
LBNERunManager::~LBNERunManager()
{
   if(LBNEData->GetDebugLevel() > 0)
   {
      std::cout << "LBNERunManager Destructor Called." << std::endl;
   }

}
//------------------------------------------------------------------------------------
void LBNERunManager::BeamOn(G4int n_event,const char* macroFile,G4int n_select)
{
   if(LBNEData->GetDebugLevel() > 0)
   {
      G4cout << "LBNERunManager::BeamOn() Called." << G4endl;
   }
   
   
   
   
  nEvents = n_event;
  G4bool runOn(true);
  G4bool cond = ConfirmBeamOnCondition();
  if(cond)
  {
     if(runOn)
     {
	TStopwatch *sWatch=new TStopwatch();
	sWatch->Start(true);
	numberOfEventToBeProcessed = n_event;
	
	//
	//RunInitializtion() calls a lot of functions one of which is 
	//LBNERunAction::BeginOfRunAction(). Put all run Initiliation code here
	//like opening the Output Ntuple and checking to make sure that
	//the simulation is setup correctly and it is OK to Run.
	//
	G4cout << G4endl;
	std::cout << "********************************************************" << std::endl;
	std::cout << "********************************************************" << std::endl;
	std::cout << "*****LBNERunManager::BeamOn - Initializting Run...******" << std::endl;	 
	std::cout << "********************************************************" << std::endl;
	RunInitialization();
	std::cout << "********************************************************" << std::endl;
	std::cout << "***LBNERunManager::BeamOn - ...Done Initializing Run.***" << std::endl;
	std::cout << "********************************************************" << std::endl;
	std::cout << "********************************************************" << std::endl;
	G4cout << G4endl;	 
	
	if(LBNEData->GetOKToRun())
	{
	   n_event = LBNEData->GetNEvents();
	   numberOfEventToBeProcessed = n_event;
	   
	   if(n_event>0)
	   { 
	      std::cout << "LBNERunManager::BeamOn() - Processing " << n_event << " events..." << std::endl;
	      DoEventLoop(n_event,macroFile,n_select);
	   }
	   else
	   {
	      std::cout << "LBNERunManager::BeamOn() - PROBLEM: THE NUMBER OF EVENTS TO BE PROCESSED IS NOT GREATER THAN 0." << std::endl;
	   }
	}
	else
	{
	   std::cout << std::endl;
	   std::cout << "*******************************************************" << std::endl;
	   std::cout << "LBNERunManager - BeamOn: It is NOT OK TO RUN. Quitting." << std::endl;
	   std::cout << "*******************************************************" << std::endl;
	std::cout << std::endl;
	}
	
	G4cout << G4endl;
	std::cout << "*******************************************************" << std::endl;
	std::cout << "*******************************************************" << std::endl;
	std::cout << "******LBNERunManager::BeamOn - Terminating Run...******" << std::endl;	 
	std::cout << "*******************************************************" << std::endl;
	
	RunTermination();
	std::cout << "*******************************************************" << std::endl;
	std::cout << "***LBNERunManager::BeamOn - ...Done Terminating Run.***" << std::endl;
	std::cout << "*******************************************************" << std::endl;
	std::cout << "*******************************************************" << std::endl;
	G4cout << G4endl;	 

	
	sWatch->Stop();
	G4cout << "LBNERunManager::BeamOn - Run Summary..." << G4endl;  	 
	G4cout << "   Processed "<< n_event << " particles in ";
	G4cout.precision(3);
	G4cout << sWatch->RealTime() << " s = " 
	       << sWatch->RealTime()/60. << " min " 
	       << sWatch->RealTime()/3600. << " hr " << G4endl;
	delete sWatch;
     }
     
     
  }
}

