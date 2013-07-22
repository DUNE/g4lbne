//
// LBNESteppingAction.hh
//

#ifndef LBNESteppingAction_H
#define LBNESteppingAction_H 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"
#include "LBNEDataInput.hh"

class G4EventManager;
class LBNEEventAction;
class LBNERunManager;

class LBNESteppingAction : public G4UserSteppingAction
{
  
 public:
  LBNESteppingAction();
  virtual ~LBNESteppingAction();
  
  virtual void UserSteppingAction(const G4Step*);

   void KillNonNuThresholdParticles(const G4Step * theStep);
   void CheckDecay(const G4Step * theStep);

   void CheckInHornEndPlane(const G4Step * theStep, G4int nhorn);

   void CheckInTgtEndPlane(const G4Step * theStep);
   void CheckInMuonDetectorPlane(const G4Step *theStep);
   

private:
   
   LBNEDataInput *fLBNEData;
   LBNERunManager *pRunManager;
   G4EventManager *EvtManager;
   LBNEEventAction *LBNEEvtAct;
   G4LogicalVolume *fDetMuonLogical;
  

};

#endif

