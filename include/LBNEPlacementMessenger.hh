#ifndef LBNEPLACEMENTMESSENGER_H
#define LBNEPLACEMENTMESSENGER_H 1

#include <vector>

//#include "G4VUserVolumePlacement.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4PVPlacement.hh"
//
// Geant User Interface utility for handling the nominal paramtof the geometry. 
//
class LBNEPlacementMessenger: public G4UImessenger {

public:
  static LBNEPlacementMessenger* Instance();
  ~LBNEPlacementMessenger();
  void SetNewValue(G4UIcommand* command, G4String cmd); // Will set data in the singleton LBNEVolumePlacements
     
private:
   LBNEPlacementMessenger();
  static LBNEPlacementMessenger* fInstance;
    
//  G4UIdirectory*                fLBNEDir;
//  G4UIdirectory*                fdetDir;

  G4UIcmdWithADoubleAndUnit* fWaterLayerThickInHorn;
  G4UIcmdWithADoubleAndUnit* fHorn1Length;
  G4UIcmdWithADoubleAndUnit* fBaffleLength;
  G4UIcmdWithADoubleAndUnit* fBaffleInnerRadius;
  G4UIcmdWithADoubleAndUnit* fBaffleZPosition;
//
  G4UIcmdWithADoubleAndUnit* fTargetSLengthGraphite; 
  G4UIcmdWithADoubleAndUnit* fTargetLengthIntoHorn;
  G4UIcmdWithADoubleAndUnit* fTargetLengthOutsideHorn;
  G4UIcmdWithADoubleAndUnit* fTargetBerylCapThickness;
  G4UIcmdWithAString* fTargetMaterial;
  G4UIcmdWithADoubleAndUnit* fTargetDensity;
//
  G4UIcmdWithADouble* fHorn1RadialRescale;   
  G4UIcmdWithADouble* fHorn1LongRescale; 
  G4UIcmdWithAString* fHorn1InnerCondMat;
  G4UIcmdWithADoubleAndUnit* fHorn1RadialSafetyMargin; 
//  
  G4UIcmdWithADouble* fHorn2RadialRescale;   
  G4UIcmdWithADouble* fHorn2LongRescale; 
  G4UIcmdWithADoubleAndUnit* fHorn2LongPosition; // from the origin MCZero to the middle of Horn 2 
  G4UIcmdWithAString* fHorn2InnerCondMat;
//
  G4UIcmdWithADoubleAndUnit* fDecayPipeLength;
  G4UIcmdWithADoubleAndUnit* fDecayPipeRadius;
  G4UIcmdWithADoubleAndUnit* fDecayPipeLongPosition; // From the target 
  G4UIcmdWithADoubleAndUnit* fDecayPipeUpstreamWindowThickness;
  G4UIcmdWithAString* fDecayPipeGas; // Air or Helium supported 
  
   G4UIcmdWithAString* fAbsorberGDMLFilename;
 
  G4UIcmdWithABool* fInstallShield; // if true (default), we place roated chunk of steel surround Horns
  
  void SetMyUnitsAndConditions(G4UIcmdWithADoubleAndUnit *cmd, double value);
  
};
#endif
