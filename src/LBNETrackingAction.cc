//----------------------------------------------------------------------
// LBNETrackingAction.cc
// $Id: LBNETrackingAction.cc,v 1.1.1.1 2011/07/13 16:20:52 loiacono Exp $
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
#include "LBNEDataInput.hh"
#include "LBNEPrimaryGeneratorAction.hh"
#include "LBNEEventAction.hh"


//--------------------------------------------------------------------------------------
LBNETrackingAction::LBNETrackingAction()
{
  pRunManager=(LBNERunManager*)LBNERunManager::GetRunManager();
  NPGA=(LBNEPrimaryGeneratorAction*)pRunManager->GetUserPrimaryGeneratorAction();
  fLBNEData=LBNEDataInput::GetLBNEDataInput();

  if(fLBNEData->GetDebugLevel() > 0)
  {
     std::cout << "LBNETrackingAction Constructor Called." << std::endl;
  }
}
//--------------------------------------------------------------------------------------
LBNETrackingAction::~LBNETrackingAction()
{
   if(fLBNEData->GetDebugLevel() > 0)
   {
      std::cout << "LBNETrackingAction Destructor Called." << std::endl;
   }
}
//--------------------------------------------------------------------------------------
void LBNETrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
   if(fLBNEData->GetDebugLevel() > 1)
   {
      G4int evtno = pRunManager->GetCurrentEvent()->GetEventID();
      std::cout << "Event " << evtno << ": LBNETrackingAction::PreUserTrackingAction() Called." << std::endl;
      if(fLBNEData->GetDebugLevel() > 2)
      {
	 LBNETrackInformation* trackinfo=new LBNETrackInformation(); 
	 trackinfo -> Print(aTrack);
	 delete trackinfo;
      }
   }
      




   if(fLBNEData->GetSimulation() == "Standard Neutrino Beam")
   {
      LBNETrackingAction::SetPreLBNETrackInformation(aTrack);
      
      //Store tracks in trajectory container
      fpTrackingManager->SetStoreTrajectory(true);
      fpTrackingManager->SetTrajectory(new LBNETrajectory(aTrack));
      
      LBNETrackingAction::AnalyzeIfNeutrino(aTrack);
   }
   else if(fLBNEData->GetSimulation() == "Target Tracking" )
   {
      LBNETrackingAction::SetPreLBNETrackInformation(aTrack);
      //Store tracks in trajectory container
      fpTrackingManager->SetStoreTrajectory(true);
      fpTrackingManager->SetTrajectory(new LBNETrajectory(aTrack));
   }
   else if(fLBNEData->GetSimulation() == "Horn 1 Tracking" ||
	   fLBNEData->GetSimulation() == "Horn 2 Tracking" )
   {
      LBNETrackingAction::SetPreLBNETrackInformation(aTrack);
      //Store tracks in trajectory container
      fpTrackingManager->SetStoreTrajectory(true);
      fpTrackingManager->SetTrajectory(new LBNETrajectory(aTrack));
   }
   else
   {
      std::cout << "LBNETrackingAction::PreUserTrackingAction()- PROBLEM: Don't know about the \"" 
		<< fLBNEData->GetSimulation() << "\" Simulation" << std::endl;
   }
   
   
   
  if(fLBNEData->GetDebugLevel() > 1)
  {
     std::cout << "         LBNETrackingAction::PreUserTrackingAction() - done." << std::endl;
  }

}
//--------------------------------------------------------------------------------------
void LBNETrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
   if(fLBNEData->GetDebugLevel() > 1)
   {
      G4int evtno = pRunManager->GetCurrentEvent()->GetEventID();
      std::cout << "Event " << evtno << ": LBNETrackingAction::PostUserTrackingAction() Called." << std::endl;
      if(fLBNEData->GetDebugLevel() > 2)
      {
	 LBNETrackInformation* trackinfo=new LBNETrackInformation(); 
	 trackinfo -> Print(aTrack);
	 delete trackinfo;
      }
   }

   

   if(fLBNEData->GetSimulation() == "Standard Neutrino Beam")
   {
      LBNETrackingAction::SetPostLBNETrackInformation(aTrack);
   }
   else if(fLBNEData->GetSimulation() == "Target Tracking" )
   {
      LBNETrackingAction::SetPostLBNETrackInformation(aTrack);
   }
   else if(fLBNEData->GetSimulation() == "Horn 1 Tracking" ||
	   fLBNEData->GetSimulation() == "Horn 2 Tracking" )
   {
      LBNETrackingAction::SetPostLBNETrackInformation(aTrack);
   }
   else
   {
      std::cout << "LBNETrackingAction::PostUserTrackingAction()- PROBLEM: Don't know about the \"" 
		<< fLBNEData->GetSimulation() << "\" Simulation" << std::endl;
   }


    if(fLBNEData->GetDebugLevel() > 1)
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
   if(fLBNEData->GetDebugLevel() > 3)
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
      if (fLBNEData->GetUseFlukaInput() || fLBNEData->GetUseMarsInput())
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
   if(fLBNEData->GetDebugLevel() > 3)
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
   if(fLBNEData->GetDebugLevel() > 3)
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


