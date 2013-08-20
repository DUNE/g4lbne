//----------------------------------------------------------------------
// LBNESteppingAction.cc
// $Id: LBNESteppingAction.cc,v 1.1.1.1.2.2 2013/08/20 22:57:03 lebrun Exp $
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


//----------------------------------------------------------------------
LBNESteppingAction::LBNESteppingAction()
{
  pRunManager=(LBNERunManager*)LBNERunManager::GetRunManager();
 fKillTrackingThreshold = 0.050*GeV;
 if (pRunManager->GetVerboseLevel() > 0) {
   std::cerr << " LBNESteppingAction::LBNESteppingAction called ... " << std::endl;
 }
 pMessenger = new LBNESteppingActionMessenger(this); 
}
//----------------------------------------------------------------------
LBNESteppingAction::~LBNESteppingAction()
{
 delete pMessenger;
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


   LBNESteppingAction::KillNonNuThresholdParticles(theStep);
      
   LBNESteppingAction::CheckDecay(theStep);
   if (fOutStudy.is_open()) StudyAbsorption(theStep);
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
void LBNESteppingAction::OpenAscii(const char *fname) {

   fOutStudy.open(fname);
   if (!fOutStudy.is_open()) {
     std::string descr("Can not open output file "); 
     descr += std::string(fname);
     G4Exception("LBNESteppingAction::OpenAscii", "I/O Problem ", FatalException, descr.c_str());
   }
   fOutStudy << 
    " id x y z ILDecayChan ILHorn1Neck ILHorn2Entr ILNCDecayChan ILNCHorn1Neck ILNCHorn2Entr ILAlHorn2Entr" << std::endl;
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
   if (prePtr == 0) {
     std::cerr << " No Pre point for step.. A ghost ???? Fatal... " << std::endl; exit(2);
   }
   if ( theTrack->GetNextVolume() == 0 ) {
       fOutStudy << " " << pRunManager->GetCurrentEvent()->GetEventID(); 
        for (size_t k=0; k!=3; k++) fOutStudy << " " << prePtr->GetPosition()[k];
	fOutStudy << " " << totalAbsDecayChan << " " <<  totalAbsHorn1Neck 
	          << " " << totalAbsHorn2Entr;
	fOutStudy << " " << waterAbsDecayChan << " " <<  waterAbsHorn1Neck 
	          << " " << waterAbsHorn2Entr << " " <<  alumAbsHorn2Entr << std::endl;
		  return;
   }		  
   //
   // I set the position of the geantino production vertex at Z=0.;
   //
   if ((std::abs(prePtr->GetPosition()[0]) < 1.0e-10) && (std::abs(prePtr->GetPosition()[1]) < 1.0e-10)) {
//     std::cerr << " Starting point, z = " << prePtr->GetPosition()[2] << std::endl;
     totalAbsDecayChan= 0.;
     totalAbsHorn1Neck=0.;
     totalAbsHorn2Entr=0.;
     waterAbsDecayChan= 0.;
     waterAbsHorn1Neck=0.;
     waterAbsHorn2Entr=0.;
     alumAbsHorn2Entr=0.;
     goneThroughHorn1Neck = false;
     goneThroughHorn2Entr = false;
   } 

   if(theStep->GetPreStepPoint()->GetPhysicalVolume() == NULL) return;
   const double ll = theStep->GetStepLength();
   G4StepPoint* postPtr = theStep->GetPostStepPoint();
   if (postPtr == NULL) {
       fOutStudy << " " << pRunManager->GetCurrentEvent()->GetEventID(); 
        for (size_t k=0; k!=3; k++) fOutStudy << " " << prePtr->GetPosition()[k];
	fOutStudy << " " << totalAbsDecayChan << " " <<  totalAbsHorn1Neck 
	          << " " << totalAbsHorn2Entr;
	fOutStudy << " " << waterAbsDecayChan << " " <<  waterAbsHorn1Neck 
	          << " " << waterAbsHorn2Entr << " " <<  alumAbsHorn2Entr << std::endl;
		  return;
   }		  
   if (postPtr->GetPosition()[2] > 830.) goneThroughHorn1Neck=true; // approximate... 
   if (postPtr->GetPosition()[2] > 6600.) goneThroughHorn2Entr=true; //truly approximate. 
   if (ll < 1.0e-10) return; 
   // Just print where we are now... 
   G4VPhysicalVolume *physVol = postPtr->GetPhysicalVolume();
   std::string vName(physVol->GetName());
   G4Material *material = postPtr->GetMaterial();
    
   if (pRunManager->GetCurrentEvent()->GetEventID() < 3) {
      const double r = std::sqrt(postPtr->GetPosition()[0]*postPtr->GetPosition()[0] + 
                                 postPtr->GetPosition()[1]*postPtr->GetPosition()[1]); 
      const double t = r/std::abs(postPtr->GetPosition()[2] + 515.25); // ZOrigin = -515.25
      // debugging only valid for Zorigin of -515.
      std::cerr << " r = " << r << " theta " << t <<  " z = " << postPtr->GetPosition()[2] << 
	      " In " << vName << " material " << material->GetName()
	      << " InterLength " << material->GetNuclearInterLength() << std::endl;  
   } 

   totalAbsDecayChan += ll/material->GetNuclearInterLength(); 
   if (vName.find("WaterLayer") != std::string::npos) waterAbsDecayChan += ll/material->GetNuclearInterLength(); 
   if (!goneThroughHorn1Neck) {
     totalAbsHorn1Neck += ll/material->GetNuclearInterLength(); 
     if (vName.find("WaterLayer") != std::string::npos) waterAbsHorn1Neck += ll/material->GetNuclearInterLength(); 
   }
   if (!goneThroughHorn2Entr) {
     totalAbsHorn2Entr += ll/material->GetNuclearInterLength(); 
     if (vName.find("WaterLayer") != std::string::npos) {
        waterAbsHorn2Entr += ll/material->GetNuclearInterLength(); 
     } else {
       if ((vName.find("PH01") != std::string::npos) && (material->GetName().find("Alumin") != std::string::npos))
         alumAbsHorn2Entr += ll/material->GetNuclearInterLength(); 
      }
   }
   if (vName.find("DPipe") !=  std::string::npos) {
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
