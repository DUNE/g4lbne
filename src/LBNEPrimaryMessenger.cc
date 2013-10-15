#include "LBNEPrimaryMessenger.hh"

#include "LBNEPrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4ios.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "LBNERunManager.hh"

LBNEPrimaryMessenger::LBNEPrimaryMessenger(LBNEPrimaryGeneratorAction* RA)
  :fPrimaryAction (RA)
{
   LBNERunManager* theRunManager = dynamic_cast<LBNERunManager*>(G4RunManager::GetRunManager());
   
   if(theRunManager->GetVerboseLevel() > 0)
   {
      G4cout << "LBNEPrimaryMessenger Constructor Called." << G4endl;
   }
  fDirectory = new G4UIdirectory("/LBNE/generator/");

  fBeamOffsetXCmd = 
    new G4UIcmdWithADoubleAndUnit("/LBNE/generator/beamOffsetX",this);
  fBeamOffsetXCmd->SetGuidance("Set the X offset of the proton beam");
  fBeamOffsetXCmd->SetParameterName("beamOffsetX", false);
  fBeamOffsetXCmd->SetUnitCategory("Length");
  fBeamOffsetXCmd->SetDefaultValue (0.);
  fBeamOffsetXCmd->SetDefaultUnit ("mm");
  fBeamOffsetXCmd->SetUnitCandidates ("mm cm");
  
  fBeamOffsetYCmd = 
    new G4UIcmdWithADoubleAndUnit("/LBNE/generator/beamOffsetY",this);
  fBeamOffsetYCmd->SetGuidance("Set the Y offset of the proton beam");
  fBeamOffsetYCmd->SetParameterName("beamOffsetY", false);
  fBeamOffsetYCmd->SetUnitCategory("Length");
  fBeamOffsetYCmd->SetDefaultValue (0.);
  fBeamOffsetYCmd->SetDefaultUnit ("mm");
  fBeamOffsetYCmd->SetUnitCandidates ("mm cm");
 
  fBeamSigmaXCmd = 
    new G4UIcmdWithADoubleAndUnit("/LBNE/generator/beamSigmaX",this);
  fBeamSigmaXCmd->SetGuidance("Set the X Sigma of the proton beam");
  fBeamSigmaXCmd->SetParameterName("beamSigmaX", false);
  fBeamSigmaXCmd->SetUnitCategory("Length");
  fBeamSigmaXCmd->SetDefaultValue (fPrimaryAction->GetBeamSigmaX());
  fBeamSigmaXCmd->SetDefaultUnit ("mm");
  fBeamSigmaXCmd->SetUnitCandidates ("mm cm");
  
  fBeamSigmaYCmd = 
    new G4UIcmdWithADoubleAndUnit("/LBNE/generator/beamSigmaY",this);
  fBeamSigmaYCmd->SetGuidance("Set the Y Sigma of the proton beam");
  fBeamSigmaYCmd->SetParameterName("beamSigmaY", false);
  fBeamSigmaYCmd->SetUnitCategory("Length");
  fBeamSigmaYCmd->SetDefaultValue (fPrimaryAction->GetBeamSigmaY());
  fBeamSigmaYCmd->SetDefaultUnit ("mm");
  fBeamSigmaYCmd->SetUnitCandidates ("mm cm");
 
  fBeamMaxValXCmd = 
    new G4UIcmdWithADoubleAndUnit("/LBNE/generator/beamMaxValX",this);
  fBeamMaxValXCmd->SetGuidance("Set the maximum value in Y that a proton can have on target");
  fBeamMaxValXCmd->SetParameterName("beamMaxValX", false);
  fBeamMaxValXCmd->SetUnitCategory("Length");
  fBeamMaxValXCmd->SetDefaultValue (fPrimaryAction->GetBeamMaxValX());
  fBeamMaxValXCmd->SetDefaultUnit ("mm");
  fBeamMaxValXCmd->SetUnitCandidates ("mm cm");
  
  fBeamMaxValYCmd = 
    new G4UIcmdWithADoubleAndUnit("/LBNE/generator/beamMaxValY",this);
  fBeamMaxValYCmd->SetGuidance("Set the maximum value in Y that a proton can have on target");
  fBeamMaxValYCmd->SetParameterName("beamMaxValY", false);
  fBeamMaxValYCmd->SetUnitCategory("Length");
  fBeamMaxValYCmd->SetDefaultValue (fPrimaryAction->GetBeamMaxValY());
  fBeamMaxValYCmd->SetDefaultUnit ("mm");
  fBeamMaxValYCmd->SetUnitCandidates ("mm cm");
 
  fBeamThetaCmd =
    new G4UIcmdWithADoubleAndUnit("/LBNE/generator/beamTheta",this);
  fBeamThetaCmd->SetGuidance("Set the angle (theta) of the proton beam");
  fBeamThetaCmd->SetParameterName("beamTheta", false);
  fBeamThetaCmd->SetUnitCategory("Angle");
  
  fBeamPhiCmd =
    new G4UIcmdWithADoubleAndUnit("/LBNE/generator/beamPhi",this);
  fBeamPhiCmd->SetGuidance("Set the angle (phi) of the proton beam.");
  fBeamPhiCmd->SetParameterName("beamPhi", false);
  fBeamPhiCmd->SetUnitCategory("Angle");

  fCorrectForAngleCmd = 
    new G4UIcmdWithABool("/LBNE/generator/correctForAngle", this);
  fCorrectForAngleCmd->SetGuidance("If true, beam x/y position is corrected");
  fCorrectForAngleCmd->SetGuidance("to hit center of target using angle of");
  fCorrectForAngleCmd->SetGuidance("beam. Offsets in x and y specified via");
  fCorrectForAngleCmd->SetGuidance("messenger are still respected.");
  
  fProtonMomentum  = new G4UIcmdWithADoubleAndUnit("/LBNE/primary/protonMomentum",this);
  fProtonMomentum->SetGuidance("Primary proton momentum delivered by the Fermilab Main Injector ");
  fProtonMomentum->SetParameterName("protonMomentum",true);
  fProtonMomentum->SetDefaultValue (120.0*GeV);
  fProtonMomentum->SetDefaultUnit ("GeV");
  fProtonMomentum->SetUnitCandidates ("GeV");
  fProtonMomentum->AvailableForStates(G4State_Idle);
    
  fBeamOnTargetCmd = 
    new G4UIcmdWithABool("/LBNE/generator/beamOnTarget", this);
  fBeamOnTargetCmd->SetGuidance("If true, forces beam to hit the center");
  fBeamOnTargetCmd->SetGuidance("of target. Any x or y offsets supplied");
  fBeamOnTargetCmd->SetGuidance("via messenger are ignored");
  
  fBeamBetaFunctionX = new G4UIcmdWithADoubleAndUnit("/LBNE/primary/beamBetaFunctionX", this);
  fBeamBetaFunctionX->SetGuidance(
   "LBNE beam line X beta function at the the target (MCZERO) delived by the Fermilab Main Injector ");
  fBeamBetaFunctionX->SetParameterName("beamBetaFunctionX",true);
  fBeamBetaFunctionX->SetDefaultValue (64.842); // ugly, but the primary geenrator class not yet instantiated. 
  fBeamBetaFunctionX->SetDefaultUnit ("m");
  fBeamBetaFunctionX->SetUnitCandidates ("m");
  fBeamBetaFunctionX->AvailableForStates(G4State_Idle);
  
  fBeamBetaFunctionY = new G4UIcmdWithADoubleAndUnit("/LBNE/primary/beamBetaFunctionY", this);
  fBeamBetaFunctionX->SetGuidance(
   "LBNE beam line Y beta function at the the target (MCZERO) delived by the Fermilab Main Injector ");
  fBeamBetaFunctionY->SetParameterName("beamBetaFunctionY",true);
  fBeamBetaFunctionY->SetDefaultValue (64.842); // ugly, but the primary geenrator class not yet instantiated. 
  fBeamBetaFunctionY->SetDefaultUnit ("m");
  fBeamBetaFunctionY->SetUnitCandidates ("m");
  fBeamBetaFunctionY->AvailableForStates(G4State_Idle);

  fBeamEmittanceX = new G4UIcmdWithADoubleAndUnit("/LBNE/primary/beamEmittanceX", this);
  fBeamEmittanceX->SetGuidance(
   "LBNE beam line X Emittance, in pi mm mRad, Fermi units. ");
  fBeamEmittanceX->SetParameterName("beamEmittanceX",true);
  fBeamEmittanceX->SetDefaultValue (20.); // ugly, but the primary geenrator class not yet instantiated. 
  fBeamEmittanceX->SetDefaultUnit ("mm"); // a lie, but define a unit will only bring more confusion. 
  fBeamEmittanceX->SetUnitCandidates ("mm");
  fBeamEmittanceX->AvailableForStates(G4State_Idle);
  
  fBeamEmittanceY = new G4UIcmdWithADoubleAndUnit("/LBNE/primary/beamEmittanceY", this);
  fBeamEmittanceY->SetGuidance(
   "LBNE beam line Y Emittance, in pi mm mRad, Fermi units. ");
  fBeamEmittanceY->SetParameterName("beamEmittanceY",true);
  fBeamEmittanceY->SetDefaultValue (20.); // ugly, but the primary geenrator class not yet instantiated. 
  fBeamEmittanceY->SetDefaultUnit ("mm"); // a lie, but define a unit will only bring more confusion. 
  fBeamEmittanceY->SetUnitCandidates ("mm");
  fBeamEmittanceY->AvailableForStates(G4State_Idle);
  
    
  fUseGeantino  = new G4UIcmdWithoutParameter("/LBNE/primary/useGeantino",this);
  fUseGeantino->SetGuidance("Using a Geantino at the Primary, to study absorption");
  fUseGeantino->AvailableForStates(G4State_Idle);
    
  fUseMuonGeantino  = new G4UIcmdWithoutParameter("/LBNE/primary/useMuonGeantino",this);
  fUseMuonGeantino->SetGuidance("Using a muon at the Primary, to study absorption, with magnetic field effect ");
  fUseMuonGeantino->AvailableForStates(G4State_Idle);
  
  fGeantinoOpeningAngle  = new G4UIcmdWithADoubleAndUnit("/LBNE/primary/geantinoOpeningAngle",this);
  fGeantinoOpeningAngle->SetGuidance("Polar angle generating the geantino (or mu geantino)  ");
  fGeantinoOpeningAngle->SetParameterName("GeantinoOpeningAngle",true);
  fGeantinoOpeningAngle->SetDefaultValue (0.005*radian);
  fGeantinoOpeningAngle->SetDefaultUnit("radian");
  fGeantinoOpeningAngle->SetUnitCandidates("radian");
  fGeantinoOpeningAngle->AvailableForStates(G4State_Idle);
   
  fGeantinoOpeningAngleMin  = new G4UIcmdWithADoubleAndUnit("/LBNE/primary/geantinoOpeningAngleMin",this);
  fGeantinoOpeningAngleMin->SetGuidance("Minimum Polar angle generating the geantino (or mu geantino)  ");
  fGeantinoOpeningAngleMin->SetParameterName("GeantinoOpeningAngleMin",true);
  fGeantinoOpeningAngleMin->SetDefaultValue (0.);
  fGeantinoOpeningAngleMin->SetDefaultUnit("radian");
  fGeantinoOpeningAngleMin->SetUnitCandidates("radian");
  fGeantinoOpeningAngleMin->AvailableForStates(G4State_Idle);
   
  fGeantinoZOrigin  = new G4UIcmdWithADoubleAndUnit("/LBNE/primary/geantinoZOrigin",this);
  fGeantinoZOrigin->SetGuidance("Z origin  generating the geantino (or mu geantino) (in mm) ");
  fGeantinoZOrigin->SetParameterName("GeantinoOpeningAngle",true);
  fGeantinoZOrigin->SetDefaultValue (-515.);
  fGeantinoZOrigin->SetDefaultUnit ("mm");
  fGeantinoZOrigin->SetUnitCandidates ("mm cm m");
  fGeantinoZOrigin->AvailableForStates(G4State_Idle);
  
  fGeantinoZOriginSigma  = 
     new G4UIcmdWithADoubleAndUnit("/LBNE/primary/geantinoSigmaZOrigin",this);
  fGeantinoZOriginSigma->SetGuidance("Z origin  longitudinal spread generating the geantino (or mu geantino) (in mm) ");
  fGeantinoZOriginSigma->SetParameterName("GeantinoSigmaZOrigin",true);
  fGeantinoZOriginSigma->SetDefaultValue (100.);
  fGeantinoZOriginSigma->SetDefaultUnit ("mm");
  fGeantinoZOriginSigma->SetUnitCandidates ("mm cm m");
  fGeantinoZOriginSigma->AvailableForStates(G4State_Idle);

}

LBNEPrimaryMessenger::~LBNEPrimaryMessenger()
{
  delete fBeamOffsetXCmd;
  delete fBeamOffsetYCmd;
  delete fBeamSigmaXCmd;
  delete fBeamSigmaYCmd;
  delete fBeamMaxValXCmd;
  delete fBeamMaxValYCmd;
  delete fBeamPhiCmd;
  delete fBeamThetaCmd;
  delete fCorrectForAngleCmd;
  delete fBeamOnTargetCmd;
  delete fDirectory;
  delete fUseGeantino;
  delete fProtonMomentum;
  delete fUseMuonGeantino;
  delete fGeantinoOpeningAngle;
  delete fGeantinoZOrigin;
  delete fGeantinoZOriginSigma;
}

void LBNEPrimaryMessenger::SetNewValue(G4UIcommand* cmd, G4String val)
{
   LBNERunManager* theRunManager = dynamic_cast<LBNERunManager*>(G4RunManager::GetRunManager());

   if(theRunManager->GetVerboseLevel() > 1)
   {
      G4cout << "LBNEPrimaryMessenger::SetNewValue - Setting Parameter value from input macro." << G4endl;
   }


   if(theRunManager->GetVerboseLevel() > 1)
   {
      G4cout << "LBNEPrimaryMessenger::SetNewValue - Done Setting parameter value." << G4endl;
   }

  if(cmd == fBeamOffsetXCmd){
    fPrimaryAction->SetBeamOffsetX(fBeamOffsetXCmd->GetNewDoubleValue(val));   
  }
  if(cmd == fBeamOffsetYCmd){
    fPrimaryAction->SetBeamOffsetY(fBeamOffsetYCmd->GetNewDoubleValue(val));   
  }
  if(cmd == fBeamSigmaXCmd){
    fPrimaryAction->SetBeamSigmaX(fBeamSigmaXCmd->GetNewDoubleValue(val)); 
    fPrimaryAction->SetUseJustSigmaCoord(true); 
  }
  if(cmd == fBeamSigmaYCmd){
    fPrimaryAction->SetBeamSigmaY(fBeamSigmaYCmd->GetNewDoubleValue(val));   
    fPrimaryAction->SetUseJustSigmaCoord(true); 
  }
  if(cmd == fBeamMaxValXCmd){
    fPrimaryAction->SetBeamMaxValX(fBeamMaxValXCmd->GetNewDoubleValue(val));   
  }
  if(cmd == fBeamMaxValYCmd){
    fPrimaryAction->SetBeamMaxValY(fBeamMaxValYCmd->GetNewDoubleValue(val));   
  }
  if(cmd == fBeamThetaCmd){
    fPrimaryAction->SetBeamTheta(fBeamThetaCmd->GetNewDoubleValue(val));   
  }
  if(cmd == fBeamPhiCmd){
    fPrimaryAction->SetBeamPhi(fBeamPhiCmd->GetNewDoubleValue(val));   
  }
  if(cmd == fCorrectForAngleCmd){
    fPrimaryAction->SetCorrectForAngle(fCorrectForAngleCmd->GetNewBoolValue(val));
  }
  if(cmd == fBeamOnTargetCmd){
    fPrimaryAction->SetBeamOnTarget(fBeamOnTargetCmd->GetNewBoolValue(val));
  }
  if (cmd == fGeantinoOpeningAngle) {
      G4UIcmdWithADoubleAndUnit* cmdWD = dynamic_cast<G4UIcmdWithADoubleAndUnit*> (cmd);
      fPrimaryAction->SetPolarAngleGeantino(cmdWD->GetNewDoubleValue(val));
  } else if (cmd ==  fGeantinoZOrigin ) {
      G4UIcmdWithADoubleAndUnit* cmdWD = dynamic_cast<G4UIcmdWithADoubleAndUnit*> (cmd);
      fPrimaryAction->SetZOriginGeantino( cmdWD->GetNewDoubleValue(val));   
  } else if (cmd ==  fGeantinoOpeningAngleMin ) {
      G4UIcmdWithADoubleAndUnit* cmdWD = dynamic_cast<G4UIcmdWithADoubleAndUnit*> (cmd);
      fPrimaryAction->SetPolarAngleGeantinoMin(cmdWD->GetNewDoubleValue(val));   
  } else if (cmd ==  fGeantinoZOriginSigma ) {
      G4UIcmdWithADoubleAndUnit* cmdWD = dynamic_cast<G4UIcmdWithADoubleAndUnit*> (cmd);
      fPrimaryAction->SetSigmaZOriginGeantino( cmdWD->GetNewDoubleValue(val));   
   } else if (cmd ==  fUseGeantino ) {
      if (fPrimaryAction->GetUseMuonGeantino()) {
        G4Exception("LBNEPrimaryMessenger", "Inconsistency in particle choice ", FatalException,
	              "Can't use both a muon geantino, and a geantino ");
      }
      fPrimaryAction->SetUseGeantino(true);
   } else if (cmd ==  fUseMuonGeantino ) {
      if (fPrimaryAction->GetUseGeantino()) {
        G4Exception("LBNEPrimaryMessenger", "Inconsistency in particle choice ", FatalException,
	              "Can't use both a muon geantino, and a geantino ");
      }
      fPrimaryAction->SetUseMuonGeantino(true);
   } else if (cmd ==  fProtonMomentum ) {
      G4UIcmdWithADoubleAndUnit* cmdWD = dynamic_cast<G4UIcmdWithADoubleAndUnit*> (cmd);
      const double pz =  cmdWD->GetNewDoubleValue(val);
      fPrimaryAction->SetProtonMomentum( pz);
      double bf = fPrimaryAction->GetBetaFunctionvsBeamEnergy(pz);
      fPrimaryAction->SetBeamBetaFunctionX(bf);
      fPrimaryAction->SetBeamBetaFunctionY(bf);
   }
   
   else if (cmd ==  fBeamBetaFunctionX ) {
      G4UIcmdWithADoubleAndUnit* cmdWD = dynamic_cast<G4UIcmdWithADoubleAndUnit*> (cmd);
      fPrimaryAction->SetBeamBetaFunctionX(cmdWD->GetNewDoubleValue(val));
      fPrimaryAction->SetUseCourantSniderParams(true);
   }
   else if (cmd ==  fBeamBetaFunctionY ) {
      G4UIcmdWithADoubleAndUnit* cmdWD = dynamic_cast<G4UIcmdWithADoubleAndUnit*> (cmd);
      fPrimaryAction->SetBeamBetaFunctionY(cmdWD->GetNewDoubleValue(val));
      fPrimaryAction->SetUseCourantSniderParams(true);
   }
   else if (cmd ==  fBeamEmittanceX ) {      
      G4UIcmdWithADoubleAndUnit* cmdWD = dynamic_cast<G4UIcmdWithADoubleAndUnit*> (cmd);
      fPrimaryAction->SetBeamEmittanceX(cmdWD->GetNewDoubleValue(val));
      fPrimaryAction->SetUseCourantSniderParams(true);
   }
   else if (cmd ==  fBeamEmittanceY ) {      
      G4UIcmdWithADoubleAndUnit* cmdWD = dynamic_cast<G4UIcmdWithADoubleAndUnit*> (cmd);
      fPrimaryAction->SetBeamEmittanceY(cmdWD->GetNewDoubleValue(val));
      fPrimaryAction->SetUseCourantSniderParams(true);
   }
   
}

