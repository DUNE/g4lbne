
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
class LBNEDataInput;
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
  void SetBeamTheta(G4double theta) { fBeamAngleTheta = theta; }
  void SetBeamPhi(G4double phi) { fBeamAnglePhi = phi; }

private:

  LBNEDataInput*          fLBNEData;
  LBNERunManager*         fRunManager;
  LBNEPrimaryMessenger*   fPrimaryMessenger;
  
  G4ParticleGun*          fParticleGun;
  
  TFile *fInputFile;
  TTree *fInputTree;

  G4int fProtonN;  
  G4int fNoOfPrimaries;
  G4int fCurrentPrimaryNo;

  //G4ThreeVector fTunnelPos;

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
  G4double fBeamAngleTheta;
  G4double fBeamAnglePhi;
 
  G4bool fUseGeantino;
  G4bool fUseMuonGeantino;
  G4double fZOriginGeantino;
  G4double fPolarAngleGeantino;
   
   //double                  DoubleRand() {return 2*G4UniformRand()-1.;}
public: 

   inline void SetPolarAngleGeantino(double r) {fPolarAngleGeantino=r;}
   inline void SetUseGeantino(bool t) {fUseGeantino=t;}
   inline void SetUseMuonGeantino(bool t) {fUseMuonGeantino=t;}
   inline void SetZOriginGeantino(double v) {fZOriginGeantino=v;}
   
   inline bool GetUseMuonGeantino() const { return fUseMuonGeantino; }
   inline bool GetUseGeantino() const { return fUseGeantino; }
 
   
};

#endif


