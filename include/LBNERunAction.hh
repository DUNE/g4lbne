//
// LBNERunAction.hh
//

#ifndef LBNERunAction_h
#define LBNERunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "LBNETrajectory.hh"

class G4Run;
class LBNERunActionMessenger;

class LBNERunAction : public G4UserRunAction
{
public:
  LBNERunAction();
  ~LBNERunAction();

  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);

   void CheckOKToRun();
  
private:
   int fVerboseLevel;
   LBNERunActionMessenger* runMessenger;
   
public:
   void SetDebugLevel(bool t) { fVerboseLevel = t; }  
   bool GetDebugLevel() const { return fVerboseLevel; }  
};

#endif

