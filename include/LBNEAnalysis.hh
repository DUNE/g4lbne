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
   void FillTrackingNtuple(const G4Track& track, LBNETrajectory* currTrajectory);
   void TrackThroughGeometry(const LBNETrajectory* TrackTrajectory);
   
   
   
   LBNETrajectory* GetParentTrajectory(G4int parentID);
   LBNETrajectory* GetTrajectory(G4int trackID);
   static LBNEAnalysis* getInstance();
   
      
   void SetCount(G4int count);
   G4int GetCount();
   void SetEntry(G4int entry);
   G4int GetEntry();
   
private:

   
private:
   static LBNEAnalysis* instance;
   
   G4double x;
   G4double y;
   G4double z;

   G4double noProtons;
   char nuNtupleFileName[50];

   //
   //need this or get a seg fault don't know why
   //
   std::map<int, int> code;
   //
   
   LBNEDataInput* LBNEData;

   TFile* fOutFile;
   TTree* fOutTree;

   
   TFile* nuNtuple;

   
      
   LBNEDataNtp_t *fLBNEOutNtpData;
   
   
   G4int fcount;
   G4int fentry;
   

   

};

#endif 
