//
// LBNERunAction.hh
//

#ifndef LBNERunAction_h
#define LBNERunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "LBNETrajectory.hh"

class G4Run;
class LBNEDataInput;
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

   LBNEDataInput* fLBNEData;
   LBNERunActionMessenger* runMessenger;
};

#endif

