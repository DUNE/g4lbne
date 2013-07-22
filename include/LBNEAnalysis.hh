//
// LBNEAnalysis.hh
//
// Modified Jul 2005 by A. MArino to make data_t and hadmmtuple_t classes

#ifndef LBNEANALYSIS_HH
#define LBNEANALYSIS_HH

#include "globals.hh"
#include "LBNETrajectory.hh"
#include "LBNEDataInput.hh"

//root
#include "TSystem.h"

//G4
#include "G4ios.hh"
#include "G4TrajectoryContainer.hh"

#include <map>

class G4ParticleDefinition;
class G4Step;
class TFile;
class TTree;
class G4Track;
class LBNEDataNtp_t;


const int kMaxP = 10000;
class LBNEAnalysis
{
public:

   LBNEAnalysis();
   ~LBNEAnalysis();
   
   //void book();
   //void finish();
   G4bool CreateOutput();
   void CloseOutput();
   void FillNeutrinoNtuple(const G4Track& track);
   void FillMuonData(const G4Step& aStep);
   void FillTrackingNtuple(const G4Track& track, LBNETrajectory* currTrajectory);
   void TrackThroughGeometry(const LBNETrajectory* TrackTrajectory);
   
   
   
   LBNETrajectory* GetParentTrajectory(G4int parentID);
   LBNETrajectory* GetTrajectory(G4int trackID);
   static LBNEAnalysis* getInstance();
   
   void ResetEvent();   
   void FillEvent();   
   void IncrementEvent(const G4Track *theTrack);
   void SetCount(G4int count);
   G4int GetCount();
   void SetEntry(G4int entry);
   G4int GetEntry();
   
private:
   static LBNEAnalysis* instance;
   
   G4double x;
   G4double y;
   G4double z;
   int fTrackID[kMaxP];
   int fParticlePDG[kMaxP];
   double fNImpWt[kMaxP];
   double fParticleX[kMaxP];
   double fParticleY[kMaxP];
   double fParticleZ[kMaxP];
   double fParticleEnergy[kMaxP];
   double fParticleMass[kMaxP];
   double fParticleDX[kMaxP];
   double fParticleDY[kMaxP];
   double fParticleDZ[kMaxP];
   int fNParticles;
   G4double noProtons;
   char nuNtupleFileName[50];

   //
   //need this or get a seg fault don't know why
   //
   map<int, int> code;
   //
   
   LBNEDataInput* LBNEData;

   TFile* fOutFile;
   TTree* fOutTree;
   TTree* fMuonTree;
   
   TFile* nuNtuple;

   
      
   LBNEDataNtp_t *fLBNEOutNtpData;
   LBNEDataNtp_t *fMuonData;
   
   G4int fcount;
   G4int fentry;
   

   

};

#endif 
