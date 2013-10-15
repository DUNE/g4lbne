//----------------------------------------------------------------------
// $Id
//----------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <iomanip>

#include "LBNEPrimaryGeneratorAction.hh"

#include <math.h>
#include <map>
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "G4UImanager.hh"

#include "LBNEPrimaryMessenger.hh"
#include "LBNERunManager.hh"
#include "LBNEParticleCode.hh"
#include "LBNEAnalysis.hh"


#include "TROOT.h"
#include <TFile.h>
#include <TTree.h>
#include <TLeaf.h>
#include <TSystem.h>

using namespace std;

LBNEPrimaryGeneratorAction::LBNEPrimaryGeneratorAction() :
    fRunManager(0),
    fPrimaryMessenger(0),
    fParticleGun(0),

    fInputFile(0),
    fInputTree(0),

    fProtonN(0),
    fNoOfPrimaries(0),
    fCurrentPrimaryNo(0),

    //fTunnelPos(0),
    fProtonMomentumMag(120.0*GeV),
    fProtonOrigin(0),
    fProtonMomentum(0),
    fProtonIntVertex(0),
    fParticleMomentum(0),
    fParticlePosition(0),

    fTgen(-99),
    fType(-99),
    fWeight(1.0),
    fCorrectForAngle(true),
    fBeamOnTarget(false),
    fBeamOffsetX(0.0),
    fBeamOffsetY(0.0),
    fBeamOffsetZ(-3.6*m), // beam divergence assumed to be very small... 
                          // If old way of defining the beam!.. See below. 
                          // Should place well in front of the baffle, 
			  // hopefully not too much air in the way.. 
    fBeamMaxValX(1.0*m),  // No truncation of the beam by default. 
    fBeamMaxValY(1.0*m), // No truncation of the beam by default. 
    fBeamSigmaX(1.3*mm),
    fBeamSigmaY(1.3*mm),
    fBeamAngleTheta(0.0),
    fBeamAnglePhi(0.0),
    fUseCourantSniderParams(true),
    fUseJustSigmaCoord(false),
    fBeamEmittanceX(20.), // In pi mm mRad (Fermi units of emittance) 
    fBeamEmittanceY(20.),
    fBeamBetaFunctionX(64.842), // in meter, assuming a 120 GeV beam 
    fBeamBetaFunctionY(64.842), // in meter, assuming a 120 GeV beam // John Jonstone, e-mail, Oct 11
    fBeamBetaFunctionAt120(64.842), // in meter, assuming a 120 GeV beam 
    fBeamBetaFunctionAt80(43.228), // in meter, assuming a 120 GeV beam 
    fBeamBetaFunctionAt60(32.421), // in meter, assuming a 120 GeV beam 
    
    fUseGeantino(false),
    fUseMuonGeantino(false),
    fZOriginGeantino(-515.), // Upstream of target.
    fSigmaZOriginGeantino(100.), // Upstream of target.
    fPolarAngleGeantino(.005),
    fPolarAngleGeantinoMin(0.)
{
   fRunManager       =(LBNERunManager*)LBNERunManager::GetRunManager();
   fPrimaryMessenger = new LBNEPrimaryMessenger(this);
   G4int n_particle = 1;
   fParticleGun = new G4ParticleGun(n_particle);
   //
   // Test the estimate fo the beta function 
   //
//   std::cerr << " Pz BetaFunc " << std::endl;
//   for (int iPz=0; iPz !=100; iPz++) 
//     std::cerr << " " << (50.0 + iPz*1.0) << " " <<
//               GetBetaFunctionvsBeamEnergy((50.0 + iPz*1.0)) << std:: endl;
//	       
   // Other CourantSnider functions set down below. 
}
//---------------------------------------------------------------------------------------
LBNEPrimaryGeneratorAction::~LBNEPrimaryGeneratorAction()
{
  delete fPrimaryMessenger;
  delete fParticleGun;
}
//---------------------------------------------------------------------------------------
void LBNEPrimaryGeneratorAction::SetProtonBeam()
{

   // Make sure we have a consistent way to define the beam 

   if ((fUseCourantSniderParams && fUseJustSigmaCoord) ||
       ((!fUseCourantSniderParams) && (!fUseJustSigmaCoord)) ) {
     G4Exception("LBNEPrimaryGeneratorAction::SetProtonBeam", " ", 
                FatalErrorInArgument,
    "Attempting to refdefined both CourantSnider parameters and beam spot size.  Please choose one!..."  );
   } 
   G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
 
  if (fUseGeantino) {
    fParticleGun->SetParticleDefinition(particleTable->FindParticle("geantino"));
  }
  else if (fUseMuonGeantino) {
    fParticleGun->SetParticleDefinition(particleTable->FindParticle("mu+"));
  }
  else fParticleGun->SetParticleDefinition(particleTable->FindParticle("proton"));
  
  const double eBeam = std::sqrt(fProtonMomentumMag*fProtonMomentumMag + 0.938272*GeV*0.938272*GeV); 
  const double pEkin = eBeam - 0.938272*GeV;
  fParticleGun->SetParticleEnergy(pEkin);
  // This needs to be implemented via data cards!!!
  G4ThreeVector beamPosition(0., 0., fBeamOffsetZ);
  
  fParticleGun->SetParticlePosition(beamPosition);
  G4ThreeVector beamDirection(1,0,0);
  beamDirection.setTheta(fBeamAngleTheta);
  beamDirection.setPhi(fBeamAnglePhi);
  G4cout << "Beam direction of " << beamDirection.X << " " <<
  beamDirection.Y << " " << beamDirection.Z << endl;
  fParticleGun->SetParticleMomentumDirection(beamDirection);
  
  fCurrentPrimaryNo=0;

  G4String spaces = "   ";
  std::cout << spaces << "Configuring the Proton beam..." << std::endl
	    << spaces << "   Momentum          = " << fParticleGun->GetParticleMomentum()/GeV << " GeV/c" << std::endl
	    << spaces << "   Kinetic Energy    = " << fParticleGun->GetParticleEnergy()/GeV << " GeV" << std::endl
	    << spaces << "   Beam Offset, X    = " << fBeamOffsetX/mm << " mm" << std::endl
	    << spaces << "   Beam Offset, Y    = " << fBeamOffsetY/mm << " mm" << std::endl
	    << spaces << "   Beam Offset, Z    = " << fBeamOffsetZ/m << " m" << std::endl
	    << spaces << "   Beam Sigma, X     = " << fBeamSigmaX/mm << " mm" << std::endl
	    << spaces << "   Beam Sigma, Y     = " << fBeamSigmaY/mm << " mm" << std::endl
	    << spaces << "   Beam Max. d|X|    = " << fBeamMaxValX/mm << " mm" << std::endl
	    << spaces << "   Beam Max, d|Y|    = " << fBeamMaxValY/mm << " mm" << std::endl
	    << spaces << "   Direction         = " << fParticleGun->GetParticleMomentumDirection() << std::endl;

  if (fUseCourantSniderParams) {
     
     const double gammaLorentz = eBeam/(0.938272*GeV);
     const double betaLorentz = std::sqrt(1.0 - 1.0/(gammaLorentz*gammaLorentz));
     const double betaGammaLorentz = gammaLorentz*betaLorentz;
     
     const double betaFuncStarX = (std::abs(fProtonMomentumMag - 120.*GeV) < 0.1) ? 
                              fBeamBetaFunctionX : 
			      GetBetaFunctionvsBeamEnergy(fProtonMomentumMag/GeV); // in meters, not mm 
     const double betaFuncStarY = (std::abs(fProtonMomentumMag - 120.*GeV) < 0.1) ? 
                              fBeamBetaFunctionY : 
			      GetBetaFunctionvsBeamEnergy(fProtonMomentumMag/GeV);
			         
     fBeamAlphaFunctionX = -(fBeamOffsetZ/m)/betaFuncStarX;
     fBeamAlphaFunctionY = -(fBeamOffsetZ/m)/betaFuncStarY;
     
     fBeamBetaFuncGenX = betaFuncStarX + ((fBeamOffsetZ/m)*(fBeamOffsetZ/m))/betaFuncStarX;// in meters
     fBeamBetaFuncGenY = betaFuncStarY + ((fBeamOffsetZ/m)*(fBeamOffsetZ/m))/betaFuncStarY;
     
     fBeamSigmaX = std::sqrt( 1.0e3 * fBeamBetaFuncGenX/m * fBeamEmittanceX/ (6.0*betaGammaLorentz)); // in mm 
     fBeamSigmaY = std::sqrt(  1.0e3 * fBeamBetaFuncGenY/m * fBeamEmittanceY/ (6.0*betaGammaLorentz));
     std::cout << spaces << "Re-Configuring the Proton beam using Courant-Snider parameters " << std::endl;
     std::cout << spaces << "  Beta function X at Point of creation " << fBeamBetaFuncGenX << " m " << std::endl;
     std::cout << spaces << "  Beta function Y at Point of creation " << fBeamBetaFuncGenY << " m " << std::endl;
     std::cout << spaces << "  Alpha function X at Point of creation " << fBeamAlphaFunctionX << std::endl;
     std::cout << spaces << "  Alpha function Y at Point of creation " << fBeamAlphaFunctionY << std::endl;
     std::cout << spaces << "  Sigma X at Point of creation " << fBeamSigmaX << " mm " << std::endl;
     std::cout << spaces << "  Sigma Y at Point of creation " << fBeamSigmaY << " mm " << std::endl;
     
  }
  
}



//---------------------------------------------------------------------------------------
G4bool LBNEPrimaryGeneratorAction::OpenNtuple(G4String ntupleName)
{
   G4String message("Input Ntuple"); 
   message += ntupleName + G4String("Not yet supported"); 
   G4Exception("LBNEPrimaryGeneratorAction::OpenNtuple", " ", 
                FatalErrorInArgument, message.c_str());
    /*		
   fCurrentPrimaryNo=0;
   
   G4bool fIsOpen=false;
   fInputFile=new TFile(ntupleName,"READ");
   if (!fInputFile->IsZombie())
   {
      
      //fInputTree=(TTree*)(fInputFile->Get("h3"));
      fInputTree=(TTree*)(fInputFile->Get((fLBNEData->GetInputNtpTreeName()).c_str()));
   
      if(!fInputTree)
      {
	 //G4cout<<"LBNEPrimaryGeneratorAction: Can't find tree "<< G4endl;
	 G4cout<<"LBNEPrimaryGeneratorAction: Can't find tree with name "
	       << fLBNEData->GetInputNtpTreeName() << G4endl;
      }

      fCurrentPrimaryNo=0;
      G4int entries = fInputTree->GetEntries();
      if(fLBNEData->GetNEvents() > 0 && fLBNEData->GetNEvents() < entries)
	 fNoOfPrimaries = fLBNEData->GetNEvents();
      else   
	 fNoOfPrimaries = fInputTree->GetEntries();
      
      
     fIsOpen=true;
   }
   else
   {
      G4cout<<"LBNEPrimaryGeneratorAction: Input (fluka/mars) root file doesn't exist"
            <<"   Aborting run"<<G4endl;
      fIsOpen=false;
   }
   return fIsOpen;
   */
   return false;
}
//---------------------------------------------------------------------------------------
void LBNEPrimaryGeneratorAction::CloseNtuple()
{

   if(!fInputFile) return;

   if(fInputFile && fInputFile -> IsOpen())
   {
      fInputFile->Close();
      if(fInputFile->IsOpen())
      {
	 std::cout << "   PROBLEM: Failed to close Input Ntuple " << fInputFile -> GetName() << std::endl;
      }
      else 
      {
	 std::cout << "   Sucessfully closed Input Ntuple " << fInputFile -> GetName() << std::endl;
      }
   }

   std::cout << "LBNEPrimaryGeneratorAction::CloseNtuple() - " << std::endl;
   std::cout << "   Used " << fProtonN << " protons from input file" << endl;




   fCurrentPrimaryNo=0;
}

double LBNEPrimaryGeneratorAction::GetBetaFunctionvsBeamEnergy(double pz){
 
                         // quadratic interpolation, based on the e-mail from 
                                     // John Jostone. 

 // Define and slove a set of quadratic equation. 
 const double betaSecond80 = (fBeamBetaFunctionAt120 + 2.0*fBeamBetaFunctionAt60 - 
                                   3.0*fBeamBetaFunctionAt80)/2400.;
 const double betaPrime80 = (1.0/40.)*(fBeamBetaFunctionAt120 - fBeamBetaFunctionAt80 - betaSecond80*1600.); 
 
 const double dp = pz - 80.;
 return (fBeamBetaFunctionAt80 + betaPrime80*dp + betaSecond80*dp*dp); 
}
//---------------------------------------------------------------------------------------
void LBNEPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
   int verboseLevel = G4EventManager::GetEventManager()->GetVerboseLevel();
   if(verboseLevel > 1)
   {
      std::cout << "Event " << anEvent->GetEventID() << ": LBNEPrimaryGeneratorAction::GeneratePrimaries() Called." << std::endl;
   }
   
   
   G4int totNoPrim=fRunManager->GetNumberOfEvents();
   if (totNoPrim>20)
   {
      if (fCurrentPrimaryNo%(totNoPrim/20)==0)
	 G4cout<<"Processing particles #: "
	       <<fCurrentPrimaryNo<<" to "<< fCurrentPrimaryNo+(totNoPrim/20) <<G4endl;
   }
   
   {
/* 
      if (fLBNEData->GetUseFlukaInput() || fLBNEData->GetUseMarsInput()) 
      {
	 LBNEPrimaryGeneratorAction::GenerateBeamFromInput(anEvent);
      }
*/      
      if (fUseGeantino || fUseMuonGeantino ) {
        LBNEPrimaryGeneratorAction::Geantino(anEvent);
      }
      else
      {
	 LBNEPrimaryGeneratorAction::GenerateG4ProtonBeam(anEvent);
      }
      
   }   
   
   //fCurrentPrimaryNo++;
   ++fCurrentPrimaryNo;
}



//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
void LBNEPrimaryGeneratorAction::GenerateG4ProtonBeam(G4Event* anEvent)
{

   

// If nothing else is set, use a proton beam
    G4double x0 = 0.;
    G4double y0 = 0.; 
    G4double z0 = 0.;
    
    do 
    {
       x0 = G4RandGauss::shoot(0.0, fBeamSigmaX);
    }
    while(std::abs(x0) > fBeamMaxValX);
    do 
    {
       y0 = G4RandGauss::shoot(0.0, fBeamSigmaY);
    }
    while(std::abs(y0) > fBeamMaxValY);
    
    const double xFromEmitt = x0;
    const double yFromEmitt = y0;
    z0 = fBeamOffsetZ;

    if(!fBeamOnTarget){
      // b.c. if fBeamOnTarget, all offsets are ignored.
      x0 += fBeamOffsetX;
      y0 += fBeamOffsetY;
    }

    G4double dx=0.;
    G4double dy=0.; 
    G4double dz=0.;
    
    if(fabs(fBeamAngleTheta) > 1e-4){
      dx += sin(fBeamAngleTheta)*cos(fBeamAnglePhi);
      dy += sin(fBeamAngleTheta)*sin(fBeamAnglePhi);
    }
    dz = std::sqrt(1.0 - (dx*dx + dy*dy));
    G4ThreeVector direction(dx, dy, dz);
        
    if(fCorrectForAngle || fBeamOnTarget){
       x0 += (dx/dz)*z0;
       y0 += (dy/dz)*z0;
    }
    double phaseX = 0.;
    double phaseY = 0.;
    if (fUseCourantSniderParams) { // The tails in the dx, dy distribution not quite Gaussian, 
                                   // but this is small error. 
        phaseX = 2.0*M_PI*G4RandFlat::shoot();
	const double amplX = xFromEmitt/std::cos(phaseX);
	dx += (std::sin(phaseX)*amplX - fBeamAlphaFunctionX*xFromEmitt)/(fBeamBetaFuncGenX*m); // in radian (mm/mm) 
        phaseY = 2.0*M_PI*G4RandFlat::shoot();
	const double amplY = yFromEmitt/std::cos(phaseY);
	dy += (std::sin(phaseY)*amplY - fBeamAlphaFunctionY*yFromEmitt)/(fBeamBetaFuncGenY*m);
    }

//    if (std::abs(y0) > 5.0) {
//      std::cerr << " !!!!!.... Anomalously large y0 " << y0 << " dy " << dy << " Phase y " << phaseY << std::endl;
//    }

    fProtonN = fCurrentPrimaryNo;
    fProtonOrigin   = G4ThreeVector(x0, y0, z0);
    fProtonMomentum = G4ThreeVector(dx*fProtonMomentumMag, dy*fProtonMomentumMag, dz*fProtonMomentumMag);
    fProtonIntVertex = G4ThreeVector(-9999.,-9999.,-9999.);
    fWeight=1.; //for primary protons set weight and tgen
    fTgen=0;
    
    fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
    fParticleGun->SetParticleMomentumDirection(direction);
    //
    // Backdoor to create lots of beam particle, records data on where we are on the target.. 
    //
    // G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    // fParticleGun->SetParticleDefinition(particleTable->FindParticle("geantino"));
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
//
// Used to begug geometry, and/or, Radiography of the detector. 
//
void LBNEPrimaryGeneratorAction::Geantino(G4Event* anEvent)
{

   
    
// If nothing else is set, use a proton beam
    
    const double dr = fPolarAngleGeantinoMin + 
                      (fPolarAngleGeantino - fPolarAngleGeantinoMin)*G4RandFlat::shoot();
    const double dPhi = 2.0*M_PI*G4RandFlat::shoot();
//    const double dPhi = -M_PI/2.; // Shooting up, test Hadron Absorber. 
    const double dx = dr*std::cos(dPhi);
    const double dy = dr*std::sin(dPhi);
    const double dz = sqrt(1.0 - (dx*dx + dy*dy));
    G4ThreeVector direction(dx, dy, dz);

//
// Store it a proton internally to lbne d/s  ... Messy: 
    fProtonN = fCurrentPrimaryNo;
    
    fTgen=0;
    const double x =  G4RandGauss::shoot(0.0, 1.3);  
    const double y =  G4RandGauss::shoot(0.0, 1.3);  
// Back door to study effect of overlap
//    const double dPhiPos = 2.0*M_PI*G4RandFlat::shoot();
//    const double x =  G4RandGauss::shoot(0.0, 1.3) + 223.18 * std::sin(dPhiPos);  
//    const double y =  G4RandGauss::shoot(0.0, 1.3) + 223.18 * std::cos(dPhiPos);  
    const double z =  fZOriginGeantino +  G4RandGauss::shoot(0.0, fSigmaZOriginGeantino);  
    fParticleGun->SetParticlePosition(G4ThreeVector(x, y, z));
    fParticleGun->SetParticleMomentumDirection(direction);
    if (fUseMuonGeantino) fParticleGun->SetParticleEnergy(fProtonMomentumMag); // back door use of the proton momentum data card. 
    fParticleGun->GeneratePrimaryVertex(anEvent);
}



//---------------------------------------------------------------------------------------
void LBNEPrimaryGeneratorAction::GenerateBeamFromInput(G4Event* anEvent)
{
    /*
     Fluka and Mars input variables:
     FLUKA                           MARS                   
     -----------------------------------------------------------------------------------------------
     TYPE                            TYPE                                      - type of particle (see LBNEAnalysis::GetParticleName())
     X, Y, Z                         X,Y,Z                                     - particle coordinates
     PX, PY, PZ                      PX, PY, PZ                                - particle momentum
     WEIGHT                          WEIGHT                                    - particle weight
     GENER                           GENER                                     - particle generation
     PROTVX, PROTVY, PROTVZ          PVTXX, PVTXY, PVTXZ                       - proton interaction vertex 
     PROTX, PROTY, PROTZ             PROTX, PROTY, PROTZ                       - proton initial coordinates
     PROTPX, PROTPY, PROTPZ          PROTPX, PROTPY, PROTPZ                    - proton initial momentum
     MOMPX,MOMPY,MOMPZ               PPX, PPY, PPZ                             - ???
     MOMTYPE                         PTYPE                                     - ???
     */

   G4Exception("LBNEPrimaryGeneratorAction::GenerateBeamFromInput", " ", 
                FatalErrorInArgument, " Input Ntuple not yet supported ");
      //
    //Need to create a new Gun each time
    //so Geant v4.9 doesn't complain
    //about momentum not match KE
    //
     if(fParticleGun){ delete fParticleGun; fParticleGun = 0;}
     fParticleGun = new G4ParticleGun(1);
     fParticleGun->SetParticleEnergy(0.0*GeV);
    
    G4double x0,y0,z0,px,py,pz;
    G4String particleName;
    fInputTree->GetEntry(fCurrentPrimaryNo);

    fProtonN = fInputTree->GetLeaf("event")->GetValue();
    
    x0 = fInputTree->GetLeaf("x")->GetValue()*cm;
    y0 = fInputTree->GetLeaf("y")->GetValue()*cm;
    z0 = fBeamOffsetZ; // fixed translation!!! Input file not yet supported anyways... 
//    z0 = fInputTree->GetLeaf("z")->GetValue()*cm+fLBNEData->GetTargetZ0(0)+fLBNEData->GetExtraFlukaNumiTargetZShift();
    //z0 = fInputTree->GetLeaf("z")->GetValue()*cm+fLBNEData->TargetZ0+fLBNEData->GetExtraFlukaNumiTargetZShift();
    
    px = fInputTree->GetLeaf("px")->GetValue()*GeV;
    py = fInputTree->GetLeaf("py")->GetValue()*GeV;
    pz = fInputTree->GetLeaf("pz")->GetValue()*GeV;
    
    fParticlePosition=G4ThreeVector(x0,y0,z0);
    fParticleMomentum=G4ThreeVector(px,py,pz);
    
    fWeight = fInputTree->GetLeaf("weight")->GetValue();
    fType = G4int(fInputTree->GetLeaf("type")->GetValue());
    fTgen = G4int(fInputTree->GetLeaf("gener")->GetValue());
    particleName=LBNEParticleCode::AsString(LBNEParticleCode::IntToEnum(fType));
    fProtonOrigin   = G4ThreeVector(fInputTree->GetLeaf("protx")->GetValue()*cm,
                                    fInputTree->GetLeaf("proty")->GetValue()*cm,
                                    fInputTree->GetLeaf("protz")->GetValue()*cm);
    fProtonMomentum = G4ThreeVector(fInputTree->GetLeaf("protpx")->GetValue()*cm,
                                    fInputTree->GetLeaf("protpy")->GetValue()*cm,
                                    fInputTree->GetLeaf("protpz")->GetValue()*cm);
    
/*
    if (fLBNEData->GetUseMarsInput()){
      fProtonIntVertex = G4ThreeVector(fInputTree->GetLeaf("pvtxx")->GetValue()*cm,
                                       fInputTree->GetLeaf("pvtxy")->GetValue()*cm,
                                       fInputTree->GetLeaf("pvtxz")->GetValue()*cm);
    }
    else if (fLBNEData->GetUseFlukaInput()){
      fProtonIntVertex = G4ThreeVector(fInputTree->GetLeaf("protvx")->GetValue()*cm,
                                       fInputTree->GetLeaf("protvy")->GetValue()*cm,
                                       fInputTree->GetLeaf("protvz")->GetValue()*cm);
    }
*/    
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    fParticleGun->SetParticleDefinition(particleTable->FindParticle(particleName));
    //G4double mass=particleTable->FindParticle(particleName)->GetPDGMass();
    
    //fParticleGun->SetParticleEnergy(sqrt(mass*mass+px*px+py*py+pz*pz)-mass);
    fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
    fParticleGun->SetParticleMomentum(G4ThreeVector(px,py,pz));
    fParticleGun->GeneratePrimaryVertex(anEvent);

}

