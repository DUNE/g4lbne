//----------------------------------------------------------------------
// LBNESteppingAction.cc
// $Id: LBNESteppingAction.cc,v 1.1.1.1.2.10 2013/09/16 22:30:59 lebrun Exp $
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
#include "G4TrackingManager.hh"

#include "LBNETrajectory.hh"
#include "LBNETrackInformation.hh"
#include "LBNEAnalysis.hh"
#include "LBNEEventAction.hh"
#include "LBNERunManager.hh"
#include "G4FieldManager.hh"
#include "G4Field.hh"
#include "LBNEVolumePlacements.hh"

//----------------------------------------------------------------------
LBNESteppingAction::LBNESteppingAction()
{
  pRunManager=(LBNERunManager*)LBNERunManager::GetRunManager();
 fKillTrackingThreshold = 0.050*GeV;
 if (pRunManager->GetVerboseLevel() > 0) {
   std::cerr << " LBNESteppingAction::LBNESteppingAction called ... " << std::endl;
 }
 pMessenger = new LBNESteppingActionMessenger(this); 
 fEvtIdPrevious = -1;
 fStudyGeantinoMode=G4String("none");
 fKeyVolumeForOutput=G4String("blank");
 fKeyVolumeForOutputTo=G4String("blank");
}
//----------------------------------------------------------------------
LBNESteppingAction::~LBNESteppingAction()
{
 if (fOutStudy.is_open()) fOutStudy.close();
 if (fOutStepStudy.is_open()) fOutStepStudy.close();
 delete pMessenger;
 // delete 
}

//----------------------------------------------------------------------
void LBNESteppingAction::UserSteppingAction(const G4Step * theStep)
{
   int verboseLevel = 
   G4EventManager::GetEventManager()->GetTrackingManager()->GetSteppingManager()->GetverboseLevel();
   if(verboseLevel > 3)
   {
      G4int evtno = pRunManager->GetCurrentEvent()->GetEventID();
      std::cout << "Event " << evtno << ": LBNESteppingAction::UserSteppingAction() Called." << std::endl;
   }

   if(pRunManager->GetCurrentEvent()->GetEventID() < -5) 
        this->dumpStepCheckVolumeAndFields(theStep); 

   LBNESteppingAction::KillNonNuThresholdParticles(theStep);
      
   LBNESteppingAction::CheckDecay(theStep);
   if (fOutStudy.is_open()) {
    if (fStudyGeantinoMode.find("Absorb") != std::string::npos) StudyAbsorption(theStep);
    if (fStudyGeantinoMode.find("Propa") != std::string::npos) StudyPropagation(theStep);
    if (fStudyGeantinoMode.find("PropCO") != std::string::npos) StudyCheckOverlap(theStep);
   }
}




//----------------------------------------------------------------------
//----------------------------------------------------------------------
void LBNESteppingAction::KillNonNuThresholdParticles(const G4Step * theStep)
{

   G4Track * theTrack = theStep->GetTrack();
   G4ParticleDefinition * particleDefinition = theTrack->GetDefinition();
   //
   //kill the track if it is below the kill tracking threshold and it is NOT a neutrino
   //
   if ( (theTrack->GetKineticEnergy() < fKillTrackingThreshold) &&
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

   std::string preStepPointName  = 
     theStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetName();
   std::string postStepPointName  = 
     theStep->GetPostStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetName();

   if (((preStepPointName == G4String("TargetHallAndHorn1")) && (postStepPointName == G4String("Tunnel"))) ||
       ((preStepPointName == G4String("Horn2Hall")) && (postStepPointName == G4String("Tunnel")))) 
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
   endName << "Horn1TopLevelDown";

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
void LBNESteppingAction::OpenAscii(const char *fname) {

   std::cerr << " LBNESteppingAction::OpenAscii Start with filename " << std::string(fname) << std::endl;
   fOutStudy.open(fname);
   if (!fOutStudy.is_open()) {
     std::string descr("Can not open output file "); 
     descr += std::string(fname);
     G4Exception("LBNESteppingAction::OpenAscii", "I/O Problem ", FatalException, descr.c_str());
   }
   if (fStudyGeantinoMode.find("Absorb") != std::string::npos) { 
     fOutStudy << 
    " id x y z ILDecayChan ILHorn1Neck ILHorn2Entr ILNCDecayChan ILNCHorn1Neck ILNCHorn2Entr ILAlHorn2Entr" << std::endl;
   } else if (fStudyGeantinoMode.find("Propa") != std::string::npos) {
     fOutStudy << " id x y z xo yo zo zPost step matPre matPost " << std::endl;
   } else if (fStudyGeantinoMode.find("PropCO") != std::string::npos) {
     fOutStudy << " id x y z xo yo zo step matPre matPost " << std::endl;
   }
   fOutStudy.flush();
   std::cerr << " LBNESteppingAction::OpenAscii " << std::string(fname) << std::endl;
}

void LBNESteppingAction::StudyAbsorption(const G4Step * theStep) {
//
//make sure we are dealing with a geantino, or a mu, to include lengthening of step due to curling in 
// B Field
//
   G4Track * theTrack = theStep->GetTrack();
   if (((theTrack->GetParticleDefinition()->GetParticleName()).find("geantino") == std::string::npos) && (
        ((theTrack->GetParticleDefinition()->GetParticleName()).find("mu+") == std::string::npos ))) return;

   

	
   G4StepPoint* prePtr = theStep->GetPreStepPoint();
   if (prePtr == 0) return;
/*
   if ( theTrack->GetNextVolume() == 0 ) {
       fOutStudy << " " << pRunManager->GetCurrentEvent()->GetEventID(); 
        for (size_t k=0; k!=3; k++) fOutStudy << " " << prePtr->GetPosition()[k];
	fOutStudy << " " << totalAbsDecayChan << " " <<  totalAbsHorn1Neck 
	          << " " << totalAbsHorn2Entr;
	fOutStudy << " " << waterAbsDecayChan << " " <<  waterAbsHorn1Neck 
	          << " " << waterAbsHorn2Entr << " " <<  alumAbsHorn2Entr << std::endl;
		  return;
   }		  
*/
   //
   // I set the position of the geantino production vertex at Z=0.;
   //
//   G4LogicalVolume *volPre = prePtr->GetPhysicalVolume()->GetLogicalVolume();
   if (fEvtIdPrevious  != pRunManager->GetCurrentEvent()->GetEventID() ) { 
     std::cerr << " Evt id " << 
           pRunManager->GetCurrentEvent()->GetEventID() <<
	      " Starting point, z = " << prePtr->GetPosition()[2] << std::endl;
     totalAbsDecayChan= 0.;
     totalAbsHorn1Neck=0.;
     totalAbsHorn2Entr=0.;
     waterAbsDecayChan= 0.;
     waterAbsHorn1Neck=0.;
     waterAbsHorn2Entr=0.;
     alumAbsHorn2Entr=0.;
     goneThroughHorn1Neck = false;
     goneThroughHorn2Entr = false;
     fEvtIdPrevious = pRunManager->GetCurrentEvent()->GetEventID();
     return;
   } 

   if(theStep->GetPreStepPoint()->GetPhysicalVolume() == NULL) return;
   const double ll = theStep->GetStepLength();
   G4StepPoint* postPtr = theStep->GetPostStepPoint();
/*
   if (postPtr == NULL) {
       fOutStudy << " " << pRunManager->GetCurrentEvent()->GetEventID(); 
        for (size_t k=0; k!=3; k++) fOutStudy << " " << prePtr->GetPosition()[k];
	fOutStudy << " " << totalAbsDecayChan << " " <<  totalAbsHorn1Neck 
	          << " " << totalAbsHorn2Entr;
	fOutStudy << " " << waterAbsDecayChan << " " <<  waterAbsHorn1Neck 
	          << " " << waterAbsHorn2Entr << " " <<  alumAbsHorn2Entr << std::endl;
		  return;
   } 		  
*/
  if (postPtr == NULL) return;
   G4VPhysicalVolume *physVol = postPtr->GetPhysicalVolume();
   std::string vName(physVol->GetName());
   G4Material *material = theTrack->GetMaterial();
    
   if (pRunManager->GetCurrentEvent()->GetEventID() < -3) {
      const double r = std::sqrt(postPtr->GetPosition()[0]*postPtr->GetPosition()[0] + 
                                 postPtr->GetPosition()[1]*postPtr->GetPosition()[1]); 
      const double t = r/std::abs(postPtr->GetPosition()[2] + 515.25); // ZOrigin = -515.25
      // debugging only valid for Zorigin of -515.
      std::cerr << " r = " << r << " theta " << t <<  " z = " << postPtr->GetPosition()[2] << 
	      " In " << vName << " material " << material->GetName()
	      << " InterLength " << material->GetNuclearInterLength() << std::endl;  
   } 
   if (postPtr->GetPosition()[2] > 500.) goneThroughHorn1Neck=true; // approximate... 
   if (postPtr->GetPosition()[2] > 6360.) goneThroughHorn2Entr=true; //truly approximate. 
   if (ll < 1.0e-10) return; 

   totalAbsDecayChan += ll/material->GetNuclearInterLength(); 
   if (vName.find("WaterLayer") != std::string::npos) waterAbsDecayChan += ll/material->GetNuclearInterLength(); 
   if (!goneThroughHorn1Neck) {
     totalAbsHorn1Neck += ll/material->GetNuclearInterLength(); 
     if (vName.find("WaterLayer") != std::string::npos) waterAbsHorn1Neck += ll/material->GetNuclearInterLength(); 
   }
   if (!goneThroughHorn2Entr) {
     totalAbsHorn2Entr += ll/material->GetNuclearInterLength();
//     if (theTrack->GetTrackLength() < (6000.0*mm)) 
//       std::cerr << " trackLength = " << theTrack->GetTrackLength() << " Z = " << postPtr->GetPosition()[2] << 
//         " Abs L " << totalAbsHorn2Entr << std::endl;
     
     if (vName.find("WaterLayer") != std::string::npos) {
        waterAbsHorn2Entr += ll/material->GetNuclearInterLength(); 
     } else {
       if ((vName.find("Horn1") != std::string::npos) && (material->GetName().find("Alumin") != std::string::npos))
         alumAbsHorn2Entr += ll/material->GetNuclearInterLength(); 
      }
   }
   if (vName.find("DecayPipe") !=  std::string::npos) {
        fOutStudy << " " << pRunManager->GetCurrentEvent()->GetEventID(); 
        for (size_t k=0; k!=3; k++) fOutStudy << " " << postPtr->GetPosition()[k];
	fOutStudy << " " << totalAbsDecayChan << " " <<  totalAbsHorn1Neck 
	          << " " << totalAbsHorn2Entr;
	fOutStudy << " " << waterAbsDecayChan << " " <<  waterAbsHorn1Neck 
	          << " " << waterAbsHorn2Entr << " " <<  alumAbsHorn2Entr << std::endl;
        theTrack->SetTrackStatus(fStopAndKill);
	return;
   } 
    
}
void LBNESteppingAction::StudyPropagation(const G4Step * theStep) {
//
//make sure we are dealing with a geantino... 
//
   G4Track * theTrack = theStep->GetTrack();
   if ((theTrack->GetParticleDefinition()->GetParticleName()).find("geantino") == std::string::npos) return;
   G4StepPoint* prePtr = theStep->GetPreStepPoint();
   if (prePtr == 0) return;
   G4StepPoint* postPtr = theStep->GetPostStepPoint();
   if (postPtr == 0) return;
   if (theStep->GetStepLength() < 0.1*mm) return;
   G4LogicalVolume *volPost = postPtr->GetPhysicalVolume()->GetLogicalVolume();
   G4LogicalVolume *volPre = prePtr->GetPhysicalVolume()->GetLogicalVolume();
   std::string volNamePost(volPost->GetName());
   std::string volNamePre(volPre->GetName());
   if ((volNamePost.find(fKeyVolumeForOutput.c_str()) != std::string::npos) || 
      (volNamePre.find(fKeyVolumeForOutput.c_str()) != std::string::npos)) {
       std::cout << " LBNESteppingAction::StudyPropagation, critical volume " << std::endl 
	  << ".. from " << volNamePre << " to " << volNamePost 
	             << " detected at " <<  prePtr->GetPosition() << " going to " << postPtr->GetPosition() << std::endl;
       const G4VTouchable *preHist = prePtr->GetTouchable();
       const G4VTouchable *postHist = postPtr->GetTouchable();
       const G4int nDepthPre = preHist->GetHistoryDepth();
       const G4int nDepthPost = postHist->GetHistoryDepth();
       std::cerr << " ....  History depth for pre point " <<  nDepthPre << " Post " << nDepthPost << std::endl;
       for (int k=0; k!=  std::max(nDepthPre, nDepthPost); k++) {
         if ((k <nDepthPre) && (k <nDepthPost))
	     std::cerr << " ............. Translation at depth, pre ... " << preHist->GetTranslation(k) 
	               << " Post " <<  postHist->GetTranslation(k) <<   std::endl;
	 else if (k <nDepthPre) 	       
	     std::cerr << " ..............Translation at depth, pre ... " << preHist->GetTranslation(k) << std::endl;
	 else  if (k <nDepthPost)     
	     std::cerr << " ..............Translation at depth, post ... " << postHist->GetTranslation(k) << std::endl;
       }
       std::cerr << " ------------------------------------------- " << std::endl;	     
   }   
   if (volPre->GetMaterial()->GetName() == volPost->GetMaterial()->GetName()) return;
   fOutStudy << " " << pRunManager->GetCurrentEvent()->GetEventID(); 
   for (int k=0; k != 3; k++) fOutStudy << " " << prePtr->GetPosition()[k];
   for (int k=0; k != 3; k++) fOutStudy << " " << postPtr->GetPosition()[k];
   fOutStudy << " " << postPtr->GetPosition()[2];
   fOutStudy << " " << theStep->GetStepLength();
   fOutStudy << " " << volPre->GetMaterial()->GetName();
   fOutStudy << " " << volPost->GetMaterial()->GetName();
   fOutStudy << std::endl;
   G4String vName(volPre->GetName());
   if (vName.find("DecayPipe") !=  std::string::npos) {
        theTrack->SetTrackStatus(fStopAndKill);
    }
}
void LBNESteppingAction::StudyCheckOverlap(const G4Step * theStep) {
//
//make sure we are dealing with a geantino... 
//
   G4Track * theTrack = theStep->GetTrack();
   if ((theTrack->GetParticleDefinition()->GetParticleName()).find("geantino") == std::string::npos) return;
   G4StepPoint* prePtr = theStep->GetPreStepPoint();
   if (prePtr == 0) return;
   G4StepPoint* postPtr = theStep->GetPostStepPoint();
   if (postPtr == 0) return;
   if (postPtr->GetPhysicalVolume() == 0) return;
   if (prePtr->GetPhysicalVolume() == 0) return;
   G4LogicalVolume *volPost = postPtr->GetPhysicalVolume()->GetLogicalVolume();
   G4LogicalVolume *volPre = prePtr->GetPhysicalVolume()->GetLogicalVolume();
   if (volPre == 0) return;
   if (volPost == 0) return;
   std::string volNamePost(volPost->GetName());
   std::string volNamePre(volPre->GetName());
//   std::cerr << " at Z = " << prePtr->GetPosition()[2] << ", " << volNamePre  
//             << " to " << postPtr->GetPosition()[2] << ", " << volNamePost  
//	     << "   ... From " << fKeyVolumeForOutput << " to " << fKeyVolumeForOutputTo << std::endl;
//   if (((volNamePost.find(fKeyVolumeForOutput.c_str()) != std::string::npos) || 
//      (volNamePre.find(fKeyVolumeForOutput.c_str()) != std::string::npos)) &&
//      ( (volNamePost.find(fKeyVolumeForOutputTo.c_str()) != std::string::npos) || 
//      (volNamePre.find(fKeyVolumeForOutputTo.c_str()) != std::string::npos))) {
   if ( (volNamePre.find(fKeyVolumeForOutput.c_str()) != std::string::npos) &&
        (volNamePost.find(fKeyVolumeForOutputTo.c_str()) != std::string::npos)) {
     fOutStudy << " " << pRunManager->GetCurrentEvent()->GetEventID(); 
     for (int k=0; k != 3; k++) fOutStudy << " " << prePtr->GetPosition()[k];
     for (int k=0; k != 3; k++) fOutStudy << " " << postPtr->GetPosition()[k];
     fOutStudy << " " << theStep->GetStepLength();
     fOutStudy << " " << volPre->GetMaterial()->GetName();
     fOutStudy << " " << volPost->GetMaterial()->GetName();
     fOutStudy << std::endl;
  }
  if (volNamePost.find("DecayPipe") !=  std::string::npos) {
        theTrack->SetTrackStatus(fStopAndKill);
  }
}

void LBNESteppingAction::dumpStepCheckVolumeAndFields(const G4Step * theStep) {

  int idEvt = pRunManager->GetCurrentEvent()->GetEventID();
  if (fEvtIdPrevious != idEvt) {
      if (fOutStepStudy.is_open()) fOutStepStudy.close();
      int idEvt = pRunManager->GetCurrentEvent()->GetEventID();
      std::ostringstream fNameStrStr; fNameStrStr << "./StepSudies_Evt" << idEvt << ".txt";
      std::string fNameStr(fNameStrStr.str());
      fOutStepStudy.open(fNameStr.c_str());
      fOutStepStudy << " x y z xp yp dpt bphi vName " << std::endl;
      fEvtIdPrevious = idEvt;
  }
  G4StepPoint* prePtr = theStep->GetPreStepPoint();
  if (theStep->GetPreStepPoint()->GetPhysicalVolume() == 0) return;
  if (prePtr == 0) return;
  G4Track * theTrack = theStep->GetTrack();
  if ( theTrack->GetNextVolume() == 0 ) {
    if (fOutStepStudy.is_open()) fOutStepStudy.close();
    return;
  }
  G4StepPoint* postPtr = theStep->GetPostStepPoint();
  if (postPtr == 0) {
    if (fOutStepStudy.is_open()) fOutStepStudy.close();
    return;
  } 
  for (size_t k=0; k !=3; k++) fOutStepStudy << " " << postPtr->GetPosition()[k];
  G4ThreeVector postVec = postPtr->GetMomentum();
  G4ThreeVector preVec = prePtr->GetMomentum();
  fOutStepStudy << " " << postVec[0]/postVec[2];
  fOutStepStudy << " " << postVec[1]/postVec[2];
  const double ptPre = std::sqrt(preVec[0]*preVec[0] + preVec[1]*preVec[1])/GeV;
  const double ptPost = std::sqrt(postVec[0]*postVec[0] + postVec[1]*postVec[1])/GeV;
  fOutStepStudy << " " << ptPost - ptPre;
  // Use our our utility to navigate to find the right field
  const G4Field *myField = 0;
  if ((postPtr->GetPosition()[2] > -500.) && (postPtr->GetPosition()[2] < 5000.)) {
    LBNEVolumePlacements *aPlacementHandler= LBNEVolumePlacements::Instance();
    const LBNEVolumePlacementData *pl = 
        aPlacementHandler->Find("FieldHorn1", "Horn1TopLevelUpstr", "DetectorConstruction");
    myField = pl->fCurrent->GetFieldManager()->GetDetectorField();
  } else if (postPtr->GetPosition()[2] < 10000.) {
    LBNEVolumePlacements *aPlacementHandler= LBNEVolumePlacements::Instance();
    const LBNEVolumePlacementData *pl = 
        aPlacementHandler->Find("FieldHorn1", "Horn2Hall", "DetectorConstruction");
    myField = pl->fCurrent->GetFieldManager()->GetDetectorField();
  }
   
  if (myField == 0) {
   fOutStepStudy << " " << 0.; 
  } else {
   double pos[3]; for (size_t k=0; k!= 3; ++k) pos[k] = postPtr->GetPosition()[k];
   double bf[3];
   myField->GetFieldValue(pos, &bf[0]);
   fOutStepStudy << " " << std::sqrt(bf[0]*bf[0] + bf[1]*bf[1])/tesla;
  }
  fOutStepStudy << " " << postPtr->GetPhysicalVolume()->GetLogicalVolume()->GetName() << std::endl; 
}
