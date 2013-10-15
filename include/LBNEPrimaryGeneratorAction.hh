
#ifndef LBNEPrimaryGeneratorAction_h
#define LBNEPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "Randomize.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class LBNEPrimaryMessenger;
class TFile;
class TTree;
class LBNERunManager;


class LBNEPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
 public:
  LBNEPrimaryGeneratorAction();
  ~LBNEPrimaryGeneratorAction();

  void GeneratePrimaries(G4Event* anEvent);
  void GenerateG4ProtonBeam(G4Event* anEvent);
  void GenerateBeamFromInput(G4Event* anEvent);
  void Geantino(G4Event* anEvent);

  G4ParticleGun* GetParticleGun() {return fParticleGun;};
  void SetProtonBeam();
  
  G4bool OpenNtuple(G4String ntupleName);
  void CloseNtuple();

  G4int GetNoOfPrimaries()                { return fNoOfPrimaries; }    

  //
  // Primary proton information when using G4Pbeam
  //
  G4ThreeVector GetProtonOrigin()         { return fProtonOrigin; }
  G4ThreeVector GetProtonMomentum()       { return fProtonMomentum; }
  G4ThreeVector GetProtonIntVertex()      { return fProtonIntVertex; }
    
  //
  // Info about a particle leaving the target (when using external ntuple)
  //
  G4ThreeVector GetParticlePosition()     { return fParticlePosition; }
  G4ThreeVector GetParticleMomentum()     { return fParticleMomentum; }
  G4double GetWeight()                    { return fWeight; }
  G4int GetTgen()                         { return fTgen; }
  G4int GetParticleType()                 { return fType; }

  G4int GetProtonNumber()              { return fProtonN; }
  
  void SetCorrectForAngle(G4bool aBool) { fCorrectForAngle = aBool; }
  void SetBeamOnTarget(G4bool aBool) { fBeamOnTarget = aBool; }
  void SetBeamOffsetX(G4double x) { fBeamOffsetX = x; }
  void SetBeamOffsetY(G4double y) { fBeamOffsetY = y; }
  void SetBeamSigmaX(G4double x) { fBeamSigmaX = x; }
  void SetBeamSigmaY(G4double y) { fBeamSigmaY = y; }
  void SetBeamMaxValX(G4double x) { fBeamMaxValX = x; }
  void SetBeamMaxValY(G4double y) { fBeamMaxValY = y; }
  void SetBeamTheta(G4double theta) { fBeamAngleTheta = theta; }
  void SetBeamPhi(G4double phi) { fBeamAnglePhi = phi; }
  void SetProtonMomentum(G4double p) {fProtonMomentumMag = p;}
  
  inline void SetUseCourantSniderParams(G4bool t) {fUseCourantSniderParams = t;}
  inline void SetUseJustSigmaCoord(G4bool t) {fUseJustSigmaCoord = t;}
  
  inline G4bool GetUseCourantSniderParams() { return fUseCourantSniderParams;}
  inline G4bool GetUseJustSigmaCoord() {return fUseJustSigmaCoord;}
  
  inline void SetBeamEmittanceX(G4double t) {fBeamEmittanceX = t;}
  inline void SetBeamEmittanceY(G4double t) {fBeamEmittanceY = t;}
  inline void SetBeamBetaFunctionX(G4double t) {fBeamBetaFunctionX = t;}
  inline void SetBeamBetaFunctionY(G4double t) {fBeamBetaFunctionY = t;}
  
  inline double GetBeamSigmaX() const { return fBeamSigmaX; }
  inline double GetBeamSigmaY() const { return fBeamSigmaY; }
  inline double GetBeamOffsetX() const { return fBeamOffsetX; }
  inline double GetBeamOffsetY() const { return fBeamOffsetY; }
  inline double GetBeamMaxValX() const { return fBeamMaxValX; }
  inline double GetBeamMaxValY() const { return fBeamMaxValY; }
  
  inline double GetBeamEmittanceX() const { return fBeamEmittanceX;} 
  inline double GetBeamBetaFunctionX() const { return fBeamBetaFunctionX;}
  
  inline double GetBeamEmittanceY() const { return fBeamEmittanceY;} 
  inline double GetBeamBetaFunctionY() const { return fBeamBetaFunctionY;}
  
private:

  LBNERunManager*         fRunManager;
  LBNEPrimaryMessenger*   fPrimaryMessenger;
  
  G4ParticleGun*          fParticleGun;
  
  TFile *fInputFile;
  TTree *fInputTree;

  G4int fProtonN;  
  G4int fNoOfPrimaries;
  G4int fCurrentPrimaryNo;

  //G4ThreeVector fTunnelPos;

  G4double fProtonMomentumMag;
  G4ThreeVector fProtonOrigin;
  G4ThreeVector fProtonMomentum;
  G4ThreeVector fProtonIntVertex;
  G4ThreeVector fParticleMomentum;
  G4ThreeVector fParticlePosition;

  G4int    fTgen;
  G4int    fType; 
  G4double fWeight;
  
  G4bool   fCorrectForAngle;
  G4bool   fBeamOnTarget;
  G4double fBeamOffsetX;
  G4double fBeamOffsetY;
  G4double fBeamOffsetZ;
  G4double fBeamMaxValX;
  G4double fBeamMaxValY;
  G4double fBeamSigmaX;
  G4double fBeamSigmaY;
  G4double fBeamAngleTheta;
  G4double fBeamAnglePhi;
  
  G4bool fUseCourantSniderParams; // new way to set the Proton beam paramters 
  G4bool fUseJustSigmaCoord;// Old way One and only of these flags should be set to true. 
  
  G4double fBeamEmittanceX; // 95 % value
  G4double fBeamEmittanceY;
  G4double fBeamBetaFunctionX; // in meters. Assumed to be at the waist point, (beta*)  
  G4double fBeamBetaFunctionY;
  G4double fBeamBetaFunctionAt120; // in meters 
  G4double fBeamBetaFunctionAt80; // in meters 
  G4double fBeamBetaFunctionAt60;
  
  // We are at a waste, so aplha = 0 and gamma = 1/beta (or close to it. ) 
  // Except that we generate the beam upstream. So we need the alpha 
  
  G4double fBeamAlphaFunctionX; // The alha at the place where the beam is generated. 
  G4double fBeamAlphaFunctionY; // The alha at the place where the beam is generated. 
  G4double fBeamBetaFuncGenX; // Actually used at the place where the beam is generated 
  G4double fBeamBetaFuncGenY; 
 
  G4bool fUseGeantino;
  G4bool fUseMuonGeantino;
  G4double fZOriginGeantino;
  G4double fSigmaZOriginGeantino;
  G4double fPolarAngleGeantino;
  G4double fPolarAngleGeantinoMin;
   
   //double                  DoubleRand() {return 2*G4UniformRand()-1.;}
public: 

   inline void SetPolarAngleGeantino(double r) {fPolarAngleGeantino=r;}
   inline void SetPolarAngleGeantinoMin(double r) {fPolarAngleGeantinoMin=r;}
   inline void SetUseGeantino(bool t) {fUseGeantino=t;}
   inline void SetUseMuonGeantino(bool t) {fUseMuonGeantino=t;}
   inline void SetZOriginGeantino(double v) {fZOriginGeantino=v;}
   inline void SetSigmaZOriginGeantino(double v) {fSigmaZOriginGeantino=v;}
   
   inline bool GetUseMuonGeantino() const { return fUseMuonGeantino; }
   inline bool GetUseGeantino() const { return fUseGeantino; }
   
   double GetBetaFunctionvsBeamEnergy(double pz); 
                         // quadratic interpolation, based on the e-mail from 
                                     // John Jostone. 
   
};

#endif


