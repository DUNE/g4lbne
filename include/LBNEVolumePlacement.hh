//---------------------------------------------------------------------------// 
// $Id: LBNEVolumePlacement.hh,v 1.1.2.1 2013/07/02 13:12:21 robj137 Exp $
//---------------------------------------------------------------------------// 

#ifndef LBNEVolumePlacement_H
#define LBNEVolumePlacement_H 1

#include <vector>

#include "G4VUserVolumePlacement.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//---------------------------------------------------------------------------// 
// This class controls the placement of everything within the geometry. 

class LBNEVolumePlacement : public G4VUserVolumePlacement
{

private:
  LBNEVolumePlacement();
  static LBNEVolumePlacement* fInstance;

public:
  
  
  void Initialize();

  G4ThreeVector GetDetectorPlacement(G4String detName);
  G4RotationMatrix GetDetectorRotation(G4String detName);
  G4RotationMatrix GetMotherVolumeName(G4String detName);

  void SetTargetHallLogical(G4LogicalVolume* vol) { fTargetHallLogical = vol; }
  void SetDecayHallLogical(G4LogicalVolume* vol) { fDecayHallLogical = vol; }
  void SetAbsorberHallLogical(G4LogicalVolume* vol) { fAbsorberHallLogical = vol; }

  void SetTargetHallZ(G4double val) { fTargetHallZ = val; }
  void SetDecayHallZ(G4double val) { fDecayHallZ = val; }
  void SetAbsorberHallZ(G4double val) { fAbsorberHallZ = val; }

private:

  // maps, to map the name of the detector to its placement, rotation, and the
  // logical volume in which it is placed.
  std::map<G4String, G4ThreeVector> fSubVolumeTranslations;
  std::map<G4String, G4RotationMatrix> fSubVolumeRotations;
  std::map<G4String, const G4LogicalVolume> fMotherVolumeMap;

  // Useful lengths/dimensions

  G4double fTargetHallZ;
  G4double fAbsorberHallZ;
  G4double fDecayHallZ;


  // Pointers to the mother logical volume halls

  G4LogicalVolume* fTargetHallLogical;
  G4LogicalVolume* fDecayHallLogical;
  G4LogicalVolume* fAbsorberHallLogical;
  // General (required) parameters

  // Placements of subvolumes (within relevant Hall coordinates)
  // Target Hall

  // Decay Hall
  G4ThreeVector fDecayPipePlacement;
  
  // Absorber Hall
  G4ThreeVector fHadronAbsorberPlacement;

  
  
  
  // Messenger
  G4UImessenger *fPlacementMessenger;

  
};

class LBNEPlacementMessenger: public G4UImessenger {

public:
  LBNEPlacementMessenger(LBNEVolumePlacement* );
  ~LBNEPlacementMessenger();
  void SetNewValue(G4UIcommand*, G4String);
     
private:
  LBNEVolumePlacement* LBNEDetector;
     
  G4UIdirectory*                LBNEDir;
  G4UIdirectory*                detDir;

      
  };

#endif

