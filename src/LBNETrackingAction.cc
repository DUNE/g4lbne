//----------------------------------------------------------------------
// LBNETrackingAction.cc
// $Id: LBNETrackingAction.cc,v 1.1.1.1.2.1 2013/08/20 22:57:03 lebrun Exp $
//----------------------------------------------------------------------


#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4Trajectory.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4EventManager.hh"

#include "LBNETrackInformation.hh"
#include "LBNETrackingAction.hh"
#include "LBNERunManager.hh"
#include "LBNEAnalysis.hh"
#include "LBNETrajectory.hh"
#include "LBNEPrimaryGeneratorAction.hh"
#include "LBNEEventAction.hh"


//--------------------------------------------------------------------------------------
LBNETrackingAction::LBNETrackingAction()
{
  pRunManager=(LBNERunManager*)LBNERunManager::GetRunManager();
  NPGA=(LBNEPrimaryGeneratorAction*)pRunManager->GetUserPrimaryGeneratorAction();
 // No message, fpTrackingManager pointer not set yet ..
}
//--------------------------------------------------------------------------------------
LBNETrackingAction::~LBNETrackingAction()
{
   if(fpTrackingManager->GetVerboseLevel() > 0)
   {
      std::cout << "LBNETrackingAction Destructor Called." << std::endl;
   }
}
//--------------------------------------------------------------------------------------
void LBNETrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
   if(fpTrackingManager->GetVerboseLevel() > 1)
   {
      G4int evtno = pRunManager->GetCurrentEvent()->GetEventID();
      std::cout << "Event " << evtno << ": LBNETrackingAction::PreUserTrackingAction() Called." << std::endl;
      if(fpTrackingManager->GetVerboseLevel() > 2)
      {
	 LBNETrackInformation* trackinfo=new LBNETrackInformation(); 
	 trackinfo -> Print(aTrack);
	 delete trackinfo;
      }
   }
      




      LBNETrackingAction::SetPreLBNETrackInformation(aTrack);
      
      //Store tracks in trajectory container
      fpTrackingManager->SetStoreTrajectory(true);
      fpTrackingManager->SetTrajectory(new LBNETrajectory(aTrack));
      
      LBNETrackingAction::AnalyzeIfNeutrino(aTrack);
   
   
   
  if(fpTrackingManager->GetVerboseLevel() > 1)
  {
     std::cout << "         LBNETrackingAction::PreUserTrackingAction() - done." << std::endl;
  }

}
//--------------------------------------------------------------------------------------
void LBNETrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
   if(fpTrackingManager->GetVerboseLevel() > 1)
   {
      G4int evtno = pRunManager->GetCurrentEvent()->GetEventID();
      std::cout << "Event " << evtno << ": LBNETrackingAction::PostUserTrackingAction() Called." << std::endl;
      if(fpTrackingManager->GetVerboseLevel() > 2)
      {
	 LBNETrackInformation* trackinfo=new LBNETrackInformation(); 
	 trackinfo -> Print(aTrack);
	 delete trackinfo;
      }
   }

   

   LBNETrackingAction::SetPostLBNETrackInformation(aTrack);


    if(fpTrackingManager->GetVerboseLevel() > 1)
    {
       std::cout << "         LBNETrackingAction::PostUserTrackingAction() - done." << std::endl;
    }


}





//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void LBNETrackingAction::SetPreLBNETrackInformation(const G4Track* aTrack)
{
   if(fpTrackingManager->GetVerboseLevel() > 3)
   {
      G4int evtno = pRunManager->GetCurrentEvent()->GetEventID();
      std::cout << "Event " << evtno 
		<< ": NumiTrackingAction::SetPreNumiTrackInformation() Called." 
		<< std::endl;
   }

   //set tgen (and weight for fluka nad mars input) 
   if (aTrack->GetTrackID()==1) 
   {   
      LBNETrackInformation* info = new LBNETrackInformation();
      LBNERunManager* theRunManager=dynamic_cast<LBNERunManager*>(G4RunManager::GetRunManager());
      if (theRunManager->GetUseFlukaInput() || theRunManager->GetUseMarsInput())
      {
	 info->SetTgen(NPGA->GetTgen());
	 info->SetNImpWt(NPGA->GetWeight());
      }
      else
      {
	 info->SetTgen(1);
	 info->SetNImpWt(1.);
      }
      G4Track* theTrack = (G4Track*)aTrack;
      theTrack->SetUserInformation(info);
   }
   

}

//--------------------------------------------------------------------------------------
void LBNETrackingAction::SetPostLBNETrackInformation(const G4Track* aTrack)
{
   if(fpTrackingManager->GetVerboseLevel() > 3)
   {
      G4int evtno = pRunManager->GetCurrentEvent()->GetEventID();
      std::cout << "Event " << evtno 
		<< ": NumiTrackingAction::SetPostNumiTrackInformation() Called." 
		<< std::endl;
   }

   // set tgen(secondary) = tgen(parent)+1
   LBNETrackInformation* info = (LBNETrackInformation*)(aTrack->GetUserInformation());
   if (info!=0) 
   {
      G4int tgen               = info->GetTgen();
      G4double nimpwt          = info->GetNImpWt();
      G4TrackVector* SecVector = fpTrackingManager->GimmeSecondaries();
      for (size_t ii=0;ii<(*SecVector).size();ii++)
      {
	 LBNETrackInformation* newinfo = new LBNETrackInformation();
	 newinfo->SetTgen(tgen+1); // set generation of daughter particles
	 newinfo->SetNImpWt(nimpwt); // set weight of the new track equal to parent weight
	(*SecVector)[ii]->SetUserInformation(newinfo);
      }
   }
}

//--------------------------------------------------------------------------------------
void LBNETrackingAction::AnalyzeIfNeutrino(const G4Track* aTrack)
{
   if(fpTrackingManager->GetVerboseLevel() > 3)
   {
      G4int evtno = pRunManager->GetCurrentEvent()->GetEventID();
      std::cout << "Event " << evtno 
		<< ": NumiTrackingAction::AnalyzeIfNeutrino() Called." 
		<< std::endl;
   }

  //if a particle is a neutrino then analyse and store in ntuple
  G4ParticleDefinition * particleDefinition = aTrack->GetDefinition();
  if ((particleDefinition == G4NeutrinoE::NeutrinoEDefinition())||
      (particleDefinition == G4NeutrinoMu::NeutrinoMuDefinition()) ||
      (particleDefinition == G4NeutrinoTau::NeutrinoTauDefinition()) ||
      (particleDefinition == G4AntiNeutrinoE::AntiNeutrinoEDefinition()) ||
      (particleDefinition == G4AntiNeutrinoMu::AntiNeutrinoMuDefinition()) ||
      (particleDefinition == G4AntiNeutrinoTau::AntiNeutrinoTauDefinition()))
    {
      LBNEAnalysis* analysis = LBNEAnalysis::getInstance();
      analysis->FillNeutrinoNtuple(*aTrack);
    }
}


