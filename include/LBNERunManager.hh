//
// LBNERunManager.hh
//

#ifndef LBNERunManager_h
#define LBNERunManager_h 1

#include "G4RunManager.hh"

class LBNEDataInput;
#include "LBNEPrimaryGeneratorAction.hh"

class LBNERunManager : public G4RunManager
{
public:
   LBNERunManager();
   virtual ~LBNERunManager();
   
   virtual void BeamOn(G4int n_event,const char* macroFile=0,G4int n_select=-1);
   inline G4int GetNumberOfEvents(){
      return numberOfEventToBeProcessed;
   }
   
   G4int nEvents;
   
protected:
   LBNEDataInput* LBNEData;
   
//   LBNEPrimaryGeneratorAction * primaryGeneratorAction;
   
public:
   
   inline LBNEPrimaryGeneratorAction* GetLBNEPrimaryGeneratorAction()
      { return dynamic_cast<LBNEPrimaryGeneratorAction*>(userPrimaryGeneratorAction); }
   
};

#endif
