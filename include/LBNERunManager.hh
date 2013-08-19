//
// LBNERunManager.hh
//

#ifndef LBNERunManager_h
#define LBNERunManager_h 1

#include "G4RunManager.hh"
#include "LBNERunAction.hh"
#include "LBNEEventAction.hh"
#include "LBNETrackingAction.hh"
#include "LBNESteppingAction.hh"
#include "LBNEStackingAction.hh"

class LBNEDataInput;
#include "LBNEPrimaryGeneratorAction.hh"

class LBNERunManager : public G4RunManager
{
public:
   LBNERunManager();
   virtual ~LBNERunManager();
   
   virtual void InitializeGeometry();
   virtual void InitializePhysics();
   virtual void BeamOn(G4int n_event,const char* macroFile=0,G4int n_select=-1);
   inline G4int GetNumberOfEvents(){
      return numberOfEventToBeProcessed;
   }
   
   G4int nEvents;
   
protected:
   LBNEDataInput* LBNEData;
   bool fGeometryIntializedHere;
   bool fPhysicsInitializedHere;
   LBNEEventAction *fLBNEEventAction;
   LBNESteppingAction *fLBNESteppingAction;
   LBNEStackingAction *fLBNEStackingAction ;
   LBNETrackingAction *fLBNETrackingAction;
   LBNERunAction *fLBNERunAction;
   
//   LBNEPrimaryGeneratorAction * primaryGeneratorAction;
   
public:
   
   inline LBNEPrimaryGeneratorAction* GetLBNEPrimaryGeneratorAction()
      { return dynamic_cast<LBNEPrimaryGeneratorAction*>(userPrimaryGeneratorAction); }
   
};

#endif
