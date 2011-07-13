#ifndef LBNEDetectorMessenger_H
#define LBNEDetectorMessenger_H 1

#include "globals.hh"
#include "G4UImessenger.hh"

class LBNEDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;

class LBNEDetectorMessenger: public G4UImessenger {

public:
   LBNEDetectorMessenger(LBNEDetectorConstruction* );
   ~LBNEDetectorMessenger();
   
   void SetNewValue(G4UIcommand*, G4String);
   
private:
   LBNEDetectorConstruction* LBNEDetector;
   
   G4UIdirectory*              LBNEDir;
   G4UIdirectory*              detDir;


   G4UIcmdWithABool*           ConstructTarget;


   G4UIcmdWithoutParameter*    UpdateCmd;
};

#endif
