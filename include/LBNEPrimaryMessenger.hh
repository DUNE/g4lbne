#ifndef LBNEPrimaryMessenger_h
#define LBNEPrimaryMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4ios.hh"
#include "LBNEPrimaryGeneratorAction.hh"

class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithoutParameter;

class LBNEPrimaryMessenger: public G4UImessenger
{
public:
  LBNEPrimaryMessenger(LBNEPrimaryGeneratorAction* );
  ~LBNEPrimaryMessenger();
  
  void SetNewValue(G4UIcommand* ,G4String );
 
private:
  LBNEPrimaryGeneratorAction*   PrimaryAction;
  G4ParticleTable*          particleTable;

};

#endif
