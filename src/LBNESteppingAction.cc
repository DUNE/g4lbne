//----------------------------------------------------------------------
// LBNESteppingAction.cc
// $Id: LBNESteppingAction.cc,v 1.1.1.1 2011/07/13 16:20:52 loiacono Exp $
//----------------------------------------------------------------------

//C++
#include <string>

#include "LBNESteppingAction.hh"

#include "G4ProcessManager.hh"
#include "G4SteppingManager.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4TrajectoryContainer.hh"
#include "G4RunManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Event.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4EventManager.hh"

#include "LBNETrajectory.hh"
#include "LBNETrackInformation.hh"
#include "LBNEDataInput.hh"
#include "LBNEAnalysis.hh"
#include "LBNEEventAction.hh"
#include "LBNERunManager.hh"


//----------------------------------------------------------------------
LBNESteppingAction::LBNESteppingAction()
{
  fLBNEData = LBNEDataInput::GetLBNEDataInput();
  pRunManager=(LBNERunManager*)LBNERunManager::GetRunManager();

  
}
//----------------------------------------------------------------------
LBNESteppingAction::~LBNESteppingAction()
{
}

//----------------------------------------------------------------------
void LBNESteppingAction::UserSteppingAction(const G4Step * theStep)
{

   if(fLBNEData->GetDebugLevel() > 3)
   {
      G4int evtno = pRunManager->GetCurrentEvent()->GetEventID();
      std::cout << "Event " << evtno << ": LBNESteppingAction::UserSteppingAction() Called." << std::endl;
   }





   if(fLBNEData->GetSimulation() == "Standard Neutrino Beam")
   {
      LBNESteppingAction::KillNonNuThresholdParticles(theStep);
      
      LBNESteppingAction::CheckDecay(theStep);
   }
   else if(fLBNEData->GetSimulation() == "Target Tracking" )
   {
      LBNESteppingAction::KillNonNuThresholdParticles(theStep);
      LBNESteppingAction::CheckDecay(theStep);
      LBNESteppingAction::CheckInTgtEndPlane(theStep);
   }
   else if(fLBNEData->GetSimulation() == "Horn 1 Tracking" )
   {
      LBNESteppingAction::KillNonNuThresholdParticles(theStep);
      LBNESteppingAction::CheckDecay(theStep);
      LBNESteppingAction::CheckInHornEndPlane(theStep, 1);
   }
   else if(fLBNEData->GetSimulation() == "Horn 2 Tracking" )
   {
      LBNESteppingAction::KillNonNuThresholdParticles(theStep);
      LBNESteppingAction::CheckDecay(theStep);
      LBNESteppingAction::CheckInHornEndPlane(theStep, 2);
   }
   else
   {
      std::cout << "LBNESteppingAction::UserSteppingAction() - PROBLEM: Don't know about the \"" 
		<< fLBNEData->GetSimulation() << "\" Simulation" << std::endl;
   }

   
}




//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
void LBNESteppingAction::KillNonNuThresholdParticles(const G4Step * theStep)
{

   G4Track * theTrack = theStep->GetTrack();
   G4ParticleDefinition * particleDefinition = theTrack->GetDefinition();
   //
   //kill the track if it is below the kill tracking threshold and it is NOT a neutrino
   //
   if (( fLBNEData->GetKillTracking() && 
	 theTrack->GetKineticEnergy() < fLBNEData->GetKillTrackingThreshold() ) &&
       (particleDefinition != G4NeutrinoE::NeutrinoEDefinition())&&
       (particleDefinition != G4NeutrinoMu::NeutrinoMuDefinition()) &&
       (particleDefinition != G4NeutrinoTau::NeutrinoTauDefinition()) &&
       (particleDefinition != G4AntiNeutrinoE::AntiNeutrinoEDefinition()) &&
       (particleDefinition != G4AntiNeutrinoMu::AntiNeutrinoMuDefinition()) &&
       (particleDefinition != G4AntiNeutrinoTau::AntiNeutrinoTauDefinition()))
   {
      theTrack->SetTrackStatus(fStopAndKill);
   }
   
}


//----------------------------------------------------------------------
void LBNESteppingAction::CheckDecay(const G4Step * theStep)
{
   G4Track * theTrack = theStep->GetTrack();
   G4ParticleDefinition * particleDefinition = theTrack->GetDefinition();
  

  // Check if the Pi+, Pi-, K+, K-, K0L, mu+ or mu- decayed and set Ndecay code:
  // 1  K0L -> nu_e pi- e+
  // 2  K0L -> anti_nu_e pi+ e-
  // 3  K0L -> nu_mu pi- mu+
  // 4  K0L -> anti_nu_mu pi+ mu-
  // 5  K+  -> nu_mu mu+
  // 6  K+  -> nu_e pi0 e+
  // 7  K+  -> nu_mu pi0 mu+
  // 8  K-  -> anti_nu_mu mu-
  // 9  K-  -> anti_nu_e pi0 e-
  // 10 K-  -> anti_nu_mu pi0 mu-
  // 11 mu+ -> anti_nu_mu nu_e e+
  // 12 mu- -> nu_mu anti_nu_e e-
  // 13 pi+ -> nu_mu mu+
  // 14 pi- -> anti_nu_mu mu-



  

  if (theStep->GetPostStepPoint()->GetProcessDefinedStep() != NULL)
  {
     G4int decay_code=0;
     if (theStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="Decay")
     {
	G4int nSecAtRest = fpSteppingManager->GetfN2ndariesAtRestDoIt();
	G4int nSecAlong  = fpSteppingManager->GetfN2ndariesAlongStepDoIt();
	G4int nSecPost   = fpSteppingManager->GetfN2ndariesPostStepDoIt();
	G4int nSecTotal  = nSecAtRest+nSecAlong+nSecPost;
	G4TrackVector* secVec = fpSteppingManager->GetfSecondary();
	
	if (particleDefinition==G4PionPlus::PionPlusDefinition())
        {
           for (size_t partno=(*secVec).size()-nSecTotal;partno<(*secVec).size();partno++)
           {
	      if ((*secVec)[partno]->GetDefinition()->GetParticleName()=="nu_mu")
                 decay_code=13;
           }
	}
	if (particleDefinition==G4PionMinus::PionMinusDefinition())
        {
           for (size_t partno=(*secVec).size()-nSecTotal;partno<(*secVec).size();partno++)
           {
	      if ((*secVec)[partno]->GetDefinition()->GetParticleName()=="anti_nu_mu")
                 decay_code=14;
           }
	}
	if (particleDefinition==G4KaonPlus::KaonPlusDefinition())
        {
           for (size_t partno=(*secVec).size()-nSecTotal;partno<(*secVec).size();partno++)
           {
	      if ((*secVec)[partno]->GetDefinition()->GetParticleName()=="nu_mu")
              {if (nSecTotal==2) decay_code=5;
		if (nSecTotal==3) decay_code=7;}
	      if ((*secVec)[partno]->GetDefinition()->GetParticleName()=="nu_e")
                 decay_code=6;
           }
	}
	if (particleDefinition==G4KaonMinus::KaonMinusDefinition())
        {
           for (size_t partno=(*secVec).size()-nSecTotal;partno<(*secVec).size();partno++)
           {
              if ((*secVec)[partno]->GetDefinition()->GetParticleName()=="anti_nu_mu")
	      {if (nSecTotal==2) decay_code=8;
                 if (nSecTotal==3) decay_code=10;}
              if ((*secVec)[partno]->GetDefinition()->GetParticleName()=="anti_nu_e")
                 decay_code=9;
           }
	}
	if (particleDefinition==G4KaonZeroLong::KaonZeroLongDefinition())
        {
           for (size_t partno=(*secVec).size()-nSecTotal;partno<(*secVec).size();partno++)
	    {
               if ((*secVec)[partno]->GetDefinition()->GetParticleName()=="nu_e")
                  decay_code=1;
               if ((*secVec)[partno]->GetDefinition()->GetParticleName()=="anti_nu_e")
                  decay_code=2;
               if ((*secVec)[partno]->GetDefinition()->GetParticleName()=="nu_mu")
                  decay_code=3;
               if ((*secVec)[partno]->GetDefinition()->GetParticleName()=="anti_nu_mu")
                  decay_code=4;	    
	    }
	}
	if (particleDefinition==G4MuonPlus::MuonPlusDefinition())
        {
           for (size_t partno=(*secVec).size()-nSecTotal;partno<(*secVec).size();partno++)
           {
	      if ((*secVec)[partno]->GetDefinition()->GetParticleName()=="anti_nu_mu")
                 decay_code=11;	
           }
	}
	if (particleDefinition==G4MuonMinus::MuonMinusDefinition())
        {
           for (size_t partno=(*secVec).size()-nSecTotal;partno<(*secVec).size();partno++)
           {
	      if ((*secVec)[partno]->GetDefinition()->GetParticleName()=="nu_mu")
                 decay_code=12;	
           }
        }
	
	LBNETrackInformation* oldinfo=(LBNETrackInformation*)(theTrack->GetUserInformation()); 
	if (oldinfo!=0)
        {
           oldinfo->SetDecayCode(decay_code);                                                      
	  theTrack->SetUserInformation(oldinfo); 
	}
	else
        {
           LBNETrackInformation* newinfo=new LBNETrackInformation(); 
	  newinfo->SetDecayCode(decay_code);                                                       
	  theTrack->SetUserInformation(newinfo); 
	}
     }
  }
}

//----------------------------------------------------------------------
void LBNESteppingAction::CheckInHornEndPlane(const G4Step * theStep, G4int nhorn)
{

   G4Track * theTrack = theStep->GetTrack();
   
   if(theStep->GetPreStepPoint()->GetPhysicalVolume() == NULL) return;

   std::string preStepPointName  = theStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();

   std::stringstream endName;
   endName << "PH0" << nhorn << "EndPlane";

   if (preStepPointName.find(endName.str()) != std::string::npos)
   {
      theTrack->SetTrackStatus(fStopAndKill);
      G4VTrajectory* currTrajectory = 0;
      currTrajectory = (G4EventManager::GetEventManager()->GetTrackingManager()->GimmeTrajectory());

      if(!currTrajectory) std::cout << "Current trajectory is invalid" << std::endl;

      LBNETrajectory* currLBNETrajectory = 0;
      currLBNETrajectory = dynamic_cast<LBNETrajectory*>(currTrajectory);

      if(!currLBNETrajectory) std::cout << "Cast failed"<< std::endl;

      LBNETrajectory* newtrajectory = 0;
      newtrajectory = new LBNETrajectory(*currLBNETrajectory);
      if(!newtrajectory) std::cout << "new trajectory is invalid" << std::endl;
            
      newtrajectory ->AppendStep(theStep);

      LBNEAnalysis* analysis = LBNEAnalysis::getInstance();
      analysis->FillTrackingNtuple(*theTrack, newtrajectory);
      
      delete newtrajectory;
   }


}

//----------------------------------------------------------------------
void LBNESteppingAction::CheckInTgtEndPlane(const G4Step * theStep)
{

   G4Track * theTrack = theStep->GetTrack();
   
   if(theStep->GetPreStepPoint()->GetPhysicalVolume() == NULL) return;

   std::string preStepPointName  = theStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();

   std::stringstream endName;
   endName << "TgtEndPlane";

   if (preStepPointName.find(endName.str()) != std::string::npos)
   {
      theTrack->SetTrackStatus(fStopAndKill);
      G4VTrajectory* currTrajectory = 0;
      currTrajectory = (G4EventManager::GetEventManager()->GetTrackingManager()->GimmeTrajectory());

      if(!currTrajectory) std::cout << "Current trajectory is invalid" << std::endl;

      LBNETrajectory* currLBNETrajectory = 0;
      currLBNETrajectory = dynamic_cast<LBNETrajectory*>(currTrajectory);

      if(!currLBNETrajectory) std::cout << "Cast failed"<< std::endl;

      LBNETrajectory* newtrajectory = 0;
      newtrajectory = new LBNETrajectory(*currLBNETrajectory);
      if(!newtrajectory) std::cout << "new trajectory is invalid" << std::endl;
            
      newtrajectory ->AppendStep(theStep);

      LBNEAnalysis* analysis = LBNEAnalysis::getInstance();
      analysis->FillTrackingNtuple(*theTrack, newtrajectory);
      
      delete newtrajectory;
   }


}
