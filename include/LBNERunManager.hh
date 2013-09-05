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
   bool fGeometryIntializedHere;
   bool fPhysicsInitializedHere;
   LBNEEventAction *fLBNEEventAction;
   LBNESteppingAction *fLBNESteppingAction;
   LBNEStackingAction *fLBNEStackingAction ;
   LBNETrackingAction *fLBNETrackingAction;
   LBNERunAction *fLBNERunAction;
   
//   LBNEPrimaryGeneratorAction * primaryGeneratorAction;
  
   G4String fMarsOrFlukaInputFileName;
   G4String fAsciiOutputFileName;
   G4String fNptOutputFileName;
   bool fUseFluka;
   bool fUseMars;
   bool fCreateOutput;
   bool fCreateAsciiOutput;
   int fVerboseLevel;   
  
public:

   inline void SetNptInputFileName(G4String &aName) { fMarsOrFlukaInputFileName = aName; }
   
   inline LBNEPrimaryGeneratorAction* GetLBNEPrimaryGeneratorAction()
      { return dynamic_cast<LBNEPrimaryGeneratorAction*>(userPrimaryGeneratorAction); }
   
   inline LBNESteppingAction* GetLBNESteppingManager() 
      { return dynamic_cast<LBNESteppingAction*> (userSteppingAction); }
      
   inline void SetUseFlukaInput(bool t) {  fUseFluka = t; }
   inline void SetUseMarsInput(bool t) {  fUseMars = t; }
   
   inline bool GetUseFlukaInput() const {return  fUseFluka;} 
   inline bool GetUseMarsInput() const {return  fUseMars;} 
   inline G4String GetNptInputFileName() const {return fMarsOrFlukaInputFileName;}
   
   inline void SetOutputNtpFileName(G4String &aName) {fNptOutputFileName = aName;} 
   inline G4String  GetOutputNtpFileName() const {return fNptOutputFileName;} 
   inline void SetOutputASCIIFileName(G4String &aName) {fAsciiOutputFileName = aName;} 
   inline G4String  GetOutputASCIIFileName() const {return fAsciiOutputFileName;} 
   
   inline bool GetCreateOutput() const { return fCreateOutput; }
   inline void SetCreateOutput(bool t) { fCreateOutput = t;}
   inline bool GetCreateASCIIOutput() const { return fCreateAsciiOutput; }
   inline void SetCreateASCIIOutput(bool t) { fCreateAsciiOutput = t;}
   
   
// Why keeping two event counters ? One in the Base class (numberOfEventToProcessed) and nEvent here...  
//  Note available in v4.9.3...   
   inline void SetNumberOfEventsLBNE( int n ) {nEvents = n;}
   inline int GetNumberOfEventsLBNE() { return nEvents;}
      
   inline int GetVerboseLevel() const {return fVerboseLevel; }   
      
};

#endif
