//---------------------------------------------------------------------------// 
// $Id: LBNEVolumePlacements.hh,v 1.1.2.1 2013/07/12 17:52:44 lebrun Exp $
//---------------------------------------------------------------------------// 

#ifndef LBNEVolumePlacement_H
#define LBNEVolumePlacement_H 1

#include <vector>

//#include "G4VUserVolumePlacement.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//---------------------------------------------------------------------------// 
// This class controls the placement of everything within the geometry. 

//class LBNEVolumePlacement : public G4VUserVolumePlacement
//
// The file G4VUserVolumePlacement.hh was found missing in my cvs instance of the package.
// Not sure what happened here. 
// In any event, do we need inheritance here? 
//
// Rephrasing the scope and goal for this class: a container that all hold 
// critical volume nominal placements in the package. 
// Real placements can be modifed before physical placement by correcting 
// for misalignments using the LBNESurveyor data.
// Thus, for a given subVolume, we might have two placements, two entries in the 
// in the map.  
//
// Benefits of this class are (i) a central repository of all nominal placement data. 
//  (ii) interface to the messenger to get nominal data from User's cards.  
//  (iii) interface with the Surveyor data class.
//
// The Dectetor construction handles the list of materials, and will use this repository 
// of placement 
//
// Key functionality : 
//
//  CheckPlacement 
//
// This is a singleton class 
//

//
// Paul L. G. Lebrun, July 2013 
//

//
// First, one more container to avoid the search through the map more then once 
//
struct LBNEVolumePlacementData {
  G4String detName;
  G4ThreeVector position;
  G4RotationMatrix rotation;
  const G4LogicalVolume*; // not owned. G4 maintain this pointer.  Do not delete upon destructor of this class. 
};

class LBNEVolumePlacements
{

private:
  LBNEVolumePlacements();
  LBNEVolumePlacements (LBNEVolumePlacements const &); // not implemented 
  LBNEVolumePlacements& operator=(LBNEVolumePlacements const&); // not implemented 
  static LBNEVolumePlacements* fInstance;

public:
  
  static LBNEVolumePlacements* Instance(); 
  
  void Initialize();

  G4ThreeVector GetDetectorPlacement(G4String detName, bool nominal);
  G4RotationMatrix GetDetectorRotation(G4String detName, bool nominal);
  G4RotationMatrix GetMotherVolumeName(G4String detName, bool nominal);

  void SetTargetHallLogical(G4LogicalVolume* vol) { fTargetHallLogical = vol; }
  void SetDecayHallLogical(G4LogicalVolume* vol) { fDecayHallLogical = vol; }
  void SetAbsorberHallLogical(G4LogicalVolume* vol) { fAbsorberHallLogical = vol; }

  void SetTargetHallZ(G4double val) { fTargetHallZ = val; }
  void SetDecayHallZ(G4double val) { fDecayHallZ = val; }
  void SetAbsorberHallZ(G4double val) { fAbsorberHallZ = val; }

private:

  // maps, to map the name of the detector to its placement, rotation, and the
  // logical volume in which it is placed.
  
  std::map<G4String, LBNEVolumePlacementData> fSubVolume;

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
  LBNEPlacementMessenger(LBNEVolumePlacements* );
  ~LBNEPlacementMessenger();
  void SetNewValue(G4UIcommand*, G4String);
     
private:
  LBNEVolumePlacements* LBNEDetector;
     
  G4UIdirectory*                LBNEDir;
  G4UIdirectory*                detDir;

      
  };

#endif

