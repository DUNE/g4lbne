//----------------------------------------------------------------------
// LBNERunManager.cc
// $Id: LBNERunManager.cc,v 1.1.1.1.2.2 2013/08/20 22:57:03 lebrun Exp $
//----------------------------------------------------------------------

#include "LBNERunManager.hh"
#include "LBNEDataInput.hh"
#include "TStopwatch.h"
#include "TTime.h"
#include "LBNEPrimaryGeneratorAction.hh"
#include "G4GeometryManager.hh"
#include "G4StateManager.hh" 
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4DecayTable.hh"
#include "G4KL3DecayChannel.hh"
#include "G4StepLimiter.hh"
#include "G4ParticleTable.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4VEnergyLossProcess.hh"
#include "G4VModularPhysicsList.hh"
#include "G4PhysListFactory.hh"

//------------------------------------------------------------------------------------
LBNERunManager::LBNERunManager():
nEvents(50000),
fGeometryIntializedHere(false),
fPhysicsInitializedHere(false),
fMarsOrFlukaInputFileName(G4String("")),
fAsciiOutputFileName(G4String("")),
fNptOutputFileName(G4String("")),
fUseFluka(false),
fUseMars(false),
fCreateOutput(false),
fCreateAsciiOutput(false),
fVerboseLevel(1)

//:primaryGeneratorAction(0)
{
   if(fVerboseLevel > 0)
   {
      std::cout << "LBNERunManager Constructor Called." << std::endl;
   }
}
//------------------------------------------------------------------------------------
LBNERunManager::~LBNERunManager()
{
   if(fVerboseLevel > 0)
   {
      std::cout << "LBNERunManager Destructor Called." << std::endl;
   }

}
//------------------------------------------------------------------------------------

void LBNERunManager::InitializeGeometry() {
      if (fGeometryIntializedHere) return;
      std::cout << 
        "LBNERunManager InitializeGeometry: LBNE Beam line should be already constructed... " << std::endl;
//      G4GeometryManager::GetInstance()->CloseGeometry(); //
      G4RunManager::InitializeGeometry();
      std::cout << " ... Geometry has been closed. keep going...  " << std::endl;
      std::cout << " Geant4 is at stage " << G4StateManager::GetStateManager()->GetCurrentState() << std::endl;
      fGeometryIntializedHere = true;
}

void LBNERunManager::InitializePhysics() {

  if (fPhysicsInitializedHere) return;
  
  G4String physListName = "QGSP_BERT"; // default
  G4PhysListFactory factory;
  G4VModularPhysicsList *phys = 0;
  if(factory.IsReferencePhysList(physListName)){
    phys = factory.GetReferencePhysList(physListName);
  } else {
    G4cout << "Couldn't find physics list name " << physListName << G4endl;
    G4cout << "Exiting" << G4endl;
    exit(0);
  }
  this->SetUserInitialization(phys);
  this->SetUserAction(new LBNEPrimaryGeneratorAction);

  G4RunManager::InitializePhysics();
   
  G4UImanager* UI = G4UImanager::GetUIpointer();
// ---> April 2006. D.Jaffe modifications to set K0L,K+,K- branching fractions

  //  K0L branching fractions from Table 3 of hep-ex/0512039
  // "Status of the Cabibbo Angle", E. Blucher et al.,
  // Proceedings of the CKM 2005 Workshop (WG1), UC San Diego, 15-18 March 2005
  // Average takes into account KTeV, KLOE, NA48 results
  // KTeV PRL 93, 181802. PR D70 092006
  // KLOE hep-ex/0408027, PL B632 (2006) 43
  // NA48 PL B602 (2004) 41
  
  // K+ branching fractions from PDG2005 update and section 3.2.1 of  hep-ex/0512039
  // for average Ke3 from BNL E865 (PRL 91, 261802) and NA48.
  // B(Ke3) = 5.14+-0.06 %  hep-ex/0512039
  // B(Ke3) = 4.93+-0.07 % PDG2005 (takes into account BNL and some much older results)
  
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  if (!particleTable->contains("kaon0L")) {
    std::cerr << " The Particle table does not know about kaon Long.. Something not initialized " << std::endl;
    exit(2);
  }
  G4cout<<G4endl;
  G4cout << "Setting correct branching fractions and form factors for "
         << "K0L, K+ and K- decays (MINOS-doc-1786)"<<G4endl;
  
  UI->ApplyCommand("/particle/select kaon0L");           // ----------- K0L
  UI->ApplyCommand("/particle/property/decay/select 0"); //3pi0
  UI->ApplyCommand("/particle/property/decay/br 19.72e-2");
  UI->ApplyCommand("/particle/property/decay/select 1"); //pi-,e+,nue
  UI->ApplyCommand("/particle/property/decay/br 20.20e-2");
  UI->ApplyCommand("/particle/property/decay/select 2"); //pi+,e-,nue
  UI->ApplyCommand("/particle/property/decay/br 20.20e-2");
  UI->ApplyCommand("/particle/property/decay/select 3"); //pi-,mu+,nue
  UI->ApplyCommand("/particle/property/decay/br 13.495e-2");
  UI->ApplyCommand("/particle/property/decay/select 4"); //pi+,mu-,nue
  UI->ApplyCommand("/particle/property/decay/br 13.495e-2");
  UI->ApplyCommand("/particle/property/decay/select 5"); //pi+,pi-,pi0
  UI->ApplyCommand("/particle/property/decay/br 12.53e-2");

  UI->ApplyCommand("/particle/select kaon+");            // ------------- K+
  UI->ApplyCommand("/particle/property/decay/select 0"); //mu,nu
  UI->ApplyCommand("/particle/property/decay/br 63.39e-2");
  UI->ApplyCommand("/particle/property/decay/select 1"); //pi,pi0
  UI->ApplyCommand("/particle/property/decay/br 21.03e-2");
  UI->ApplyCommand("/particle/property/decay/select 2"); //3pi
  UI->ApplyCommand("/particle/property/decay/br 5.59e-2");
  UI->ApplyCommand("/particle/property/decay/select 3"); //pi0,e,nu
  UI->ApplyCommand("/particle/property/decay/br 5.14e-2");
  UI->ApplyCommand("/particle/property/decay/select 4"); //pi0,mu,nu
  UI->ApplyCommand("/particle/property/decay/br 3.30e-2");
  UI->ApplyCommand("/particle/property/decay/select 5"); //pi0,pi0,pi
  UI->ApplyCommand("/particle/property/decay/br 1.757e-2");

  UI->ApplyCommand("/particle/select kaon-");            // ------------- K-
  UI->ApplyCommand("/particle/property/decay/select 0"); //mu,nu
  UI->ApplyCommand("/particle/property/decay/br 63.39e-2");
  UI->ApplyCommand("/particle/property/decay/select 1"); //pi,pi0
  UI->ApplyCommand("/particle/property/decay/br 21.03e-2");
  UI->ApplyCommand("/particle/property/decay/select 2"); //3pi
  UI->ApplyCommand("/particle/property/decay/br 5.59e-2");
  UI->ApplyCommand("/particle/property/decay/select 3"); //pi0,e,nu
  UI->ApplyCommand("/particle/property/decay/br 5.14e-2");
  UI->ApplyCommand("/particle/property/decay/select 4"); //pi0,mu,nu
  UI->ApplyCommand("/particle/property/decay/br 3.30e-2");
  UI->ApplyCommand("/particle/property/decay/select 5"); //pi0,pi0,pi
  UI->ApplyCommand("/particle/property/decay/br 1.757e-2");
  // <--- end of djaffe april06 additions						   

  // ---> April 2006. Adding Davids modifications here instead of changing 
  // G4KL3DecayChannel.cc
  G4ParticleTable::G4PTblDicIterator *ptiter=particleTable->GetIterator();

  G4double lamp = 2.81e-2;
  G4double lam0 = 1.64e-2;
  G4double xi_kp= 12.377 * (lam0 - lamp);
  G4double xi_k0= 11.713 * (lam0 - lamp);
  G4double pLambda=0.;
  G4double pXi0=0.;

  ptiter->reset();
  // G4cout <<G4endl;
  //  G4cout <<" Decay mode \t \t \t \t Lambda \t \t \t Xi0 "<<G4endl;
  while ((*ptiter)()){
    G4ParticleDefinition *particle = ptiter->value();
    if (particle==G4KaonPlus::KaonPlusDefinition()||
	particle==G4KaonMinus::KaonMinusDefinition()){
      G4DecayTable *decayTable=particle->GetDecayTable();
      for (G4int ii=0; ii<decayTable->entries(); ++ii){
	if (decayTable->GetDecayChannel(ii)->GetKinematicsName()=="KL3 Decay"){
	  G4KL3DecayChannel *decayChannel=(G4KL3DecayChannel *)decayTable->GetDecayChannel(ii);

	  //G4cout<<particle->GetParticleName();
	  for (G4int jj=0; jj<decayChannel->GetNumberOfDaughters();++jj){
	    //   if (jj==0) G4cout << " -> ";
	    //   else G4cout << " + ";
	    //   G4cout<<decayChannel->GetDaughter(jj)->GetParticleName();
	    if (decayChannel->GetDaughter(jj)==G4Positron::PositronDefinition()){
	      pLambda=lamp;
	      pXi0=xi_kp;
	    }
	    if (decayChannel->GetDaughter(jj)==G4Electron::ElectronDefinition()){
	      pLambda=lamp;
	      pXi0=xi_kp;
	    }
	    if (decayChannel->GetDaughter(jj)==G4MuonPlus::MuonPlusDefinition()){
	      pLambda=lamp;
	      pXi0=xi_kp;
	    }
	    if (decayChannel->GetDaughter(jj)==G4MuonMinus::MuonMinusDefinition()){
	      pLambda=lamp;
	      pXi0=xi_kp;
	    }
	  }//loop over daughters
	  // G4cout<<"\t \t" << decayChannel->GetDalitzParameterLambda() << " -> " << pLambda<< "  "
	  //	<<"\t"<< decayChannel->GetDalitzParameterXi() << " -> " << pXi0 <<G4endl ;
	  decayChannel->SetDalitzParameter(pLambda,pXi0);
	}//check if its KL3 Decay
      }//loop over decay channels
    }
    if (particle==G4KaonZeroLong::KaonZeroLongDefinition()){
      G4DecayTable *decayTable=particle->GetDecayTable();
      for (G4int ii=0; ii<decayTable->entries(); ++ii){
	if (decayTable->GetDecayChannel(ii)->GetKinematicsName()=="KL3 Decay"){
	  G4KL3DecayChannel *decayChannel=(G4KL3DecayChannel *)decayTable->GetDecayChannel(ii);
	  //	  G4cout<<particle->GetParticleName();
	  for (G4int jj=0; jj<decayChannel->GetNumberOfDaughters();++jj){
	    // if (jj==0) G4cout << " -> ";
	    // else G4cout << " + ";
	    // G4cout<<decayChannel->GetDaughter(jj)->GetParticleName();
	    if (decayChannel->GetDaughter(jj)==G4Positron::PositronDefinition()||
		decayChannel->GetDaughter(jj)==G4Electron::ElectronDefinition()){
	      pLambda=lamp;
	      pXi0=xi_k0;
	    }
	    if (decayChannel->GetDaughter(jj)==G4MuonPlus::MuonPlusDefinition()||
		decayChannel->GetDaughter(jj)==G4MuonMinus::MuonMinusDefinition()){
	      pLambda=lamp;
	      pXi0=xi_k0;
	    }
	  }//loop over daughters
	  //	  G4cout<<"\t \t" << decayChannel->GetDalitzParameterLambda() << " -> " << pLambda<< "  "
	  //	<<"\t"<< decayChannel->GetDalitzParameterXi() << " -> " << pXi0 <<G4endl ;

	  decayChannel->SetDalitzParameter(pLambda,pXi0);

	}//check if its KL3 Decay
      }
    } 
  }//loop over particles
  // done with kaon form factors
  // Setting the maximum step size to 1 cm
  G4ParticleTable* ptbl = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* piter = ptbl->GetIterator();
  G4StepLimiter* slim = new G4StepLimiter("StepLimiter");    
  
  piter->reset();
  while ( (*piter)() ) {
    G4ParticleDefinition* pdef = piter->value();
    G4ProcessManager* pmgr = pdef->GetProcessManager();
    
    // add user limit processes for steps and special cuts
    if ( pmgr ) {      
      pmgr->AddProcess( slim, -1, -1, 3);
    }
  }
  fPhysicsInitializedHere = true;
  G4RunManager::Initialize();
  
  std::cerr << " G4RunManager::Initialize invoked, state is now " 
            << G4StateManager::GetStateManager()->GetCurrentState() << std::endl;
  
  fLBNEEventAction = new LBNEEventAction;
  G4RunManager::SetUserAction(fLBNEEventAction); 
   
  fLBNESteppingAction = new LBNESteppingAction;
  G4RunManager::SetUserAction(fLBNESteppingAction);
  
  fLBNETrackingAction  = new LBNETrackingAction;
  G4RunManager::SetUserAction(fLBNETrackingAction);
  
  fLBNERunAction = new LBNERunAction;
  G4RunManager::SetUserAction(fLBNERunAction);
  
  fLBNEStackingAction = new LBNEStackingAction;
  G4RunManager::SetUserAction(fLBNEStackingAction);
   
   if (fVerboseLevel > 0) std::cerr << " All User Actions created and declared " << std::endl;
   
  if (!G4RunManager::ConfirmBeamOnCondition()) {
     std::cerr << " Expecting to be able to turn on the beam at this stage.  Not the case ! Fatal " << std::endl;
     exit(2);
   } else {
     std::cout << " Expecting to be able to turn on the beam at this stage " << std::endl;
   } 
 }

void LBNERunManager::BeamOn(G4int n_event,const char* macroFile,G4int n_select)
{
  G4bool runOn(true);
  G4bool cond = ConfirmBeamOnCondition();
  if(cond)
  {
     if(runOn)
     {
	TStopwatch *sWatch=new TStopwatch();
	sWatch->Start(true);
	// Over write the number of events with our own
	numberOfEventToBeProcessed = nEvents;
	
	//
	//RunInitializtion() calls a lot of functions one of which is 
	//LBNERunAction::BeginOfRunAction(). Put all run Initiliation code here
	//like opening the Output Ntuple and checking to make sure that
	//the simulation is setup correctly and it is OK to Run.
	//
	G4cout << G4endl;
	std::cout << "********************************************************" << std::endl;
	std::cout << "********************************************************" << std::endl;
	std::cout << "*****LBNERunManager::BeamOn - With "<<nEvents << " Events " << std::endl;	 
	std::cout << "********************************************************" << std::endl;
	G4RunManager::BeamOn(nEvents, macroFile, n_select);
	std::cout << "********************************************************" << std::endl;
	std::cout << "***LBNERunManager::BeamOn - ...Done ***" << std::endl;
	std::cout << "********************************************************" << std::endl;
	std::cout << "********************************************************" << std::endl;
	G4cout << G4endl;	 
	sWatch->Stop();
	G4cout << "LBNERunManager::BeamOn - Run Summary..." << G4endl;  	 
	G4cout << "   Processed "<< nEvents << " beam particles in ";
	G4cout.precision(3);
	G4cout << sWatch->RealTime() << " s = " 
	       << sWatch->RealTime()/60. << " min " 
	       << sWatch->RealTime()/3600. << " hr " << G4endl;
	delete sWatch;
 
     }
   }
}

