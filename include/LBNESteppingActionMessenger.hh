#ifndef LBNESteppingActionMessenger_h
#define LBNESteppingActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4ios.hh"

class LBNESteppingAction;
class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithoutParameter;

class LBNESteppingActionMessenger: public G4UImessenger
{
public:
  LBNESteppingActionMessenger(LBNESteppingAction* );
  ~LBNESteppingActionMessenger();
  
  void SetNewValue(G4UIcommand* ,G4String );
 
private:

   LBNESteppingAction*              SteppingAction;

   //
   //NuMI/rndm
   //
   //
   //
   G4UIdirectory*              StepDir;
   
   G4UIcmdWithAString*         SimulationNameCmd;
   //
   G4UIcmdWithAString*         OutputASCIIFileNameCmd;


   //

   
      
};

#endif
