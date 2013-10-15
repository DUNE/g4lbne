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
  LBNEPrimaryGeneratorAction*   fPrimaryAction;
  G4UIdirectory*                fDirectory;
  G4UIcmdWithADoubleAndUnit*    fBeamOffsetXCmd;
  G4UIcmdWithADoubleAndUnit*    fBeamOffsetYCmd;
  G4UIcmdWithADoubleAndUnit*    fBeamSigmaXCmd;
  G4UIcmdWithADoubleAndUnit*    fBeamSigmaYCmd;
  G4UIcmdWithADoubleAndUnit*    fBeamMaxValXCmd;
  G4UIcmdWithADoubleAndUnit*    fBeamMaxValYCmd;
  G4UIcmdWithADoubleAndUnit*    fBeamThetaCmd;
  G4UIcmdWithADoubleAndUnit*    fBeamPhiCmd;
  G4UIcmdWithADoubleAndUnit*    fBeamBetaFunctionX;
  G4UIcmdWithADoubleAndUnit*    fBeamBetaFunctionY;
  G4UIcmdWithADoubleAndUnit*    fBeamEmittanceX;
  G4UIcmdWithADoubleAndUnit*    fBeamEmittanceY;
  G4UIcmdWithABool*             fCorrectForAngleCmd;
  G4UIcmdWithADoubleAndUnit*    fProtonMomentum;
  G4UIcmdWithABool*             fBeamOnTargetCmd;
  G4UIcmdWithoutParameter*      fUseGeantino;
  G4UIcmdWithoutParameter*      fUseMuonGeantino;
  G4UIcmdWithADoubleAndUnit*    fGeantinoOpeningAngle;
  G4UIcmdWithADoubleAndUnit*    fGeantinoOpeningAngleMin;
  G4UIcmdWithADoubleAndUnit*    fGeantinoZOrigin;
  G4UIcmdWithADoubleAndUnit*    fGeantinoZOriginSigma;
};

#endif
