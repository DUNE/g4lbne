//----------------------------------------------------------------------
// LBNEStackingAction.cc
// $Id: LBNEStackingAction.cc,v 1.2.4.1 2013/07/22 15:06:59 robj137 Exp $
//----------------------------------------------------------------------

#include "LBNEStackingAction.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ios.hh"
#include "LBNEImpWeight.hh"
#include "LBNETrackInformation.hh"
#include "LBNEDataInput.hh"
#include "LBNETrajectory.hh"
#include "LBNEAnalysis.hh"
#include "LBNERunManager.hh"

//----------------------------------------------------------------------------------
LBNEStackingAction::LBNEStackingAction()
{ 
  LBNEData = LBNEDataInput::GetLBNEDataInput();
  pRunManager=(LBNERunManager*)LBNERunManager::GetRunManager();

  if(LBNEData->GetDebugLevel() > 0)
  {
     std::cout << "LBNEStackingAction Constructor Called." << std::endl;
  }
}

//----------------------------------------------------------------------------------
LBNEStackingAction::~LBNEStackingAction()
{

   if(LBNEData->GetDebugLevel() > 0)
   {
      std::cout << "LBNEStackingAction Destructor Called." << std::endl;
   }
}

//----------------------------------------------------------------------------------
G4ClassificationOfNewTrack LBNEStackingAction::ClassifyNewTrack(const G4Track * aTrack)
{
   
  G4ClassificationOfNewTrack classification = fUrgent;
  G4ParticleDefinition * particleType = aTrack->GetDefinition();

  if(LBNEData->GetDebugLevel() > 2)
  {
     G4int evtno = pRunManager->GetCurrentEvent()->GetEventID();
     std::cout << "Event " << evtno << ": LBNEStackingAction::ClassifyNewTrack for Particle " 
	       << "\"" << particleType->GetParticleName() << "\"" << " Called." << std::endl;
  }


  
  if(LBNEData->GetSimulation() == "Standard Neutrino Beam"){
     LBNEStackingAction::KillEMParticles(classification, aTrack);
     //LBNEStackingAction::KillltZeroPzParticles(classification, aTrack);
     LBNEStackingAction::KillThresholdParticles(classification, aTrack);
     LBNEStackingAction::KillOutOfWorldParticles(classification, aTrack);
     LBNEStackingAction::KillUnimportantParticles(classification, aTrack);
  }
  else if(LBNEData->GetSimulation() == "Muon Flux Measurement")
  {
     //LBNEStackingAction::KillEMParticles(classification, aTrack);
     //LBNEStackingAction::KillltZeroPzParticles(classification, aTrack);
     LBNEStackingAction::KillThresholdParticles(classification, aTrack);
     LBNEStackingAction::KillOutOfWorldParticles(classification, aTrack);
     LBNEStackingAction::KillUnimportantParticles(classification, aTrack);
  }
  else if(LBNEData->GetSimulation() == "Target Tracking")
  {
     LBNEStackingAction::KillEMParticles(classification, aTrack);
     //LBNEStackingAction::KillltZeroPzParticles(classification, aTrack);
     LBNEStackingAction::KillThresholdParticles(classification, aTrack);
     LBNEStackingAction::KillOutOfWorldParticles(classification, aTrack);
     LBNEStackingAction::KillUnimportantParticles(classification, aTrack);
  }
  else if(LBNEData->GetSimulation() == "Horn 1 Tracking" ||
	  LBNEData->GetSimulation() == "Horn 2 Tracking" )
  {
     LBNEStackingAction::KillEMParticles(classification, aTrack);
     //LBNEStackingAction::KillltZeroPzParticles(classification, aTrack);
     LBNEStackingAction::KillThresholdParticles(classification, aTrack);
     LBNEStackingAction::KillOutOfWorldParticles(classification, aTrack);
     LBNEStackingAction::KillUnimportantParticles(classification, aTrack);
  }
  else
  {
     std::cout << "LBNEStackingAction::ClassifyNewTrack() - PROBLEM: Don't know about the \"" 
	       << LBNEData->GetSimulation() << "\" Simulation" << std::endl;
  }
  
  
  
  
  return classification;
}
//----------------------------------------------------------------------------------
void LBNEStackingAction::NewStage() 
{
   if(LBNEData->GetDebugLevel() > 1)
   {
      G4int evtno = pRunManager->GetCurrentEvent()->GetEventID();
      std::cout << "Event " << evtno << ": LBNEStackingAction::NewStage Called." << std::endl;
   }


  // stackManager->ReClassify();
  //  return;
}
//----------------------------------------------------------------------------------  
void LBNEStackingAction::PrepareNewEvent()
{
   if(LBNEData->GetDebugLevel() > 1)
   {
      G4int evtno = pRunManager->GetCurrentEvent()->GetEventID();
      std::cout << "Event " << evtno << ": LBNEStackingAction::PrepareNewEvent Called." << std::endl;
   } 

}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void LBNEStackingAction::KillEMParticles(G4ClassificationOfNewTrack &classification, 
					 const G4Track * aTrack)
{

   G4ParticleDefinition * particleType = aTrack->GetDefinition();
   // Discard Gammas, Electrons, ...
   if ( (particleType==G4Gamma::GammaDefinition()       ||
        particleType==G4Electron::ElectronDefinition()  ||
        particleType==G4Positron::PositronDefinition() ) &&
	(classification != fKill) )
   {classification = fKill;}
   
}

//----------------------------------------------------------------------------------
void LBNEStackingAction::KillltZeroPzParticles(G4ClassificationOfNewTrack& classification, 
					     const G4Track * aTrack)
{
  //Discard particles with pz<0
   G4ThreeVector momentum=aTrack->GetMomentumDirection();
   if (momentum[2]<0 && (classification != fKill)) {classification = fKill;}
   
}

//----------------------------------------------------------------------------------
void LBNEStackingAction::KillThresholdParticles(G4ClassificationOfNewTrack& classification, 
						const G4Track * aTrack)
{
   G4ParticleDefinition * particleType = aTrack->GetDefinition();
   //Discard particles with kinetic energy < KillTracking Threshold.GeV (that are not neutrinos)
   if ((particleType!=G4NeutrinoE::NeutrinoEDefinition())&&
       (particleType!=G4NeutrinoMu::NeutrinoMuDefinition())&&
       (particleType!=G4NeutrinoTau::NeutrinoTauDefinition())&&
       (particleType!=G4AntiNeutrinoE::AntiNeutrinoEDefinition())&&
       (particleType!=G4AntiNeutrinoMu::AntiNeutrinoMuDefinition())&&
       (particleType!=G4AntiNeutrinoTau::AntiNeutrinoTauDefinition()))
   {
      G4double energy = aTrack->GetKineticEnergy();
      if (( LBNEData->GetKillTracking() && energy < LBNEData->GetKillTrackingThreshold() ) &&
	  (classification != fKill))
      {classification = fKill;} 
   }
   
   
}


//----------------------------------------------------------------------------------
void LBNEStackingAction::KillOutOfWorldParticles(G4ClassificationOfNewTrack& classification, 
						 const G4Track * aTrack)
{

   //check if track is inside world (some neutral particles make huge jumps from horns (field part) and
   // end up decaying in ~infinity which occasionaly causes g4numi to crash
   G4ThreeVector position=aTrack->GetPosition();
   if ((classification != fKill) &&
       ((sqrt(position[0]*position[0]+position[1]*position[1])>LBNEData->GetRockRadius())||
	position[2]>LBNEData->GetRockHalfLen()))
   {
      if (LBNEData->GetDebugLevel() > 2)
      {
	 G4cout <<"LBNEStackingAction: Killing Out of World Particle" <<G4endl;
	 G4cout << "   Particle type: "<<aTrack->GetDefinition()->GetParticleName()
		<< " ; Parent ID: "<<aTrack->GetParentID()
		<< " ;  Kinetic Energy = "<<aTrack->GetKineticEnergy()/GeV<<" GeV"<<G4endl;
	 G4cout << "   Position (m) = "<<position/m<<G4endl;
      }
      classification =fKill;
   }
   
   
}

//----------------------------------------------------------------------------------
void LBNEStackingAction::KillUnimportantParticles(G4ClassificationOfNewTrack& classification, 
						  const G4Track * aTrack)
{

  //If importance weighting is on:
  if (LBNEData->GetImpWeightOn() && (classification != fKill))
  {
    LBNETrackInformation* trackInfo=(LBNETrackInformation*)(aTrack->GetUserInformation());  
    if (trackInfo!=0) 
    {
       G4double Nimpweight=LBNEImpWeight::CalculateImpWeight(aTrack);
       if(Nimpweight==0.) { classification = fKill; } 
       else               { trackInfo->SetNImpWt(Nimpweight); }
    }
  }

}

