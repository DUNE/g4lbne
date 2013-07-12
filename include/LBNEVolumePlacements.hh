//---------------------------------------------------------------------------// 
// $Id: LBNEVolumePlacements.hh,v 1.1.2.2 2013/07/12 20:28:26 lebrun Exp $
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
// Benefits of this class are (i) a central repository of all 
//   nominal and tried placements data. 
//  (ii) interface to the messenger to get nominal data from User's cards.  
//  (iii) interface with the Surveyor data class, in case misalignment are studied
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
  bool isNominal;
  bool isPlaced;
  G4String detName;
  G4ThreeVector position;
  G4RotationMatrix rotation;
  const G4LogicalVolume* mother; // Owned. But G4 maintain this pointer.  Do not delete in the destructor of this class. 
  const G4LogicalVolume* current; 
};

class LBNEVolumePlacements
{

private:
  LBNEVolumePlacements();
  LBNEVolumePlacements (LBNEVolumePlacements const &); // not implemented 
  LBNEVolumePlacements& operator=(LBNEVolumePlacements const&); // not implemented 
  static LBNEVolumePlacements* fInstance;

public:
  
  enum AlignmentAlgo {eNominal, eRandom, eFixed}; 
  
  static LBNEVolumePlacements* Instance(); 
  
  void Initialize(const G4LogicalVolume* matriarch); // Define only the 
  
  // By Locate, we mean that a definite G4PVPlacement occurs if and only if the alingment is simply nominal  
  G4PVPlacement*  CreateLogicalAndLocate(const G4String &name,  G4PVPlacement *inPlaceMother, AlignmentAlgo model);
  
  G4PVPlacement* PlaceFinal(const G4String &name, bool nominal=false );

  const LBNEVolumePlacementData* GetDetectorPlacementData(const G4String &name, bool nominal=true);

  inline double GetTargetHallZ() { return fTargetHallZ; }
  inline double GetDecayHallZ() { return fDecayHallZ; }
  inline double GetDecayPipeLength() { return fDecayPipeLength; }
  inline double GetAbsorberHallZ() { return fAbsorberHallZ; }
 
 // Interface to the Messenger command 
 
  inline void SetDecayPipeLength(double l) {fDecayPipeLength=l;}
  inline void SetWaterLayerThickInHorns(double l) { fWaterLayerThickInHorns = l;}
  
private:

  // maps, to map the name of the detector to its placement, rotation, and the
  // logical volume in which it is placed.
  
  std::map<G4String, LBNEVolumePlacementData> fSubVolume;

  // Useful lengths/dimensions

  G4double fTargetHallZ;
  G4double fAbsorberHallZ;
  G4double fDecayHallZ;
  G4double fDecayPipeLength; // equal the above for now.. 
  G4double fDecayPipeRadius;
  G4double fWaterLayerThickInHorns;
  
  // Messenger
  G4UImessenger *fPlacementMessenger;

  
};

class LBNEPlacementMessenger: public G4UImessenger {

public:
  LBNEPlacementMessenger();
  ~LBNEPlacementMessenger();
  void SetNewValue(G4UIcommand*, G4String); // Will set data in the singleton LBNEVolumePlacements
     
private:
     
  G4UIdirectory*                LBNEDir;
  G4UIdirectory*                detDir;

  G4UICommand* WaterLayerThickInHorn;
  G4UICommand* DecayPipeLength;
  
      
  };

#endif

