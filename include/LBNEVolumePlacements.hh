//---------------------------------------------------------------------------// 
// $Id: LBNEVolumePlacements.hh,v 1.1.2.5 2013/07/22 20:36:34 lebrun Exp $
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
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4PVPlacement.hh"

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
// Type of alignment study algorythms one could use 

enum LBNEVolumePlacements_AlignmentAlgo {eNominal, eFixed, eRandom}; 
//
// Geant User Interface utility. 
//
class LBNEPlacementMessenger: public G4UImessenger {

public:
  LBNEPlacementMessenger();
  ~LBNEPlacementMessenger();
  void SetNewValue(G4UIcommand* command, G4String cmd); // Will set data in the singleton LBNEVolumePlacements
     
private:
     
//  G4UIdirectory*                fLBNEDir;
//  G4UIdirectory*                fdetDir;

  G4UIcmdWithADoubleAndUnit* fWaterLayerThickInHorn;
  G4UIcmdWithADoubleAndUnit* fDecayPipeLength;
  G4UIcmdWithADoubleAndUnit* fHorn1Length;
  G4UIcmdWithADoubleAndUnit* fBaffleLength;
  G4UIcmdWithADoubleAndUnit* fBaffleInnerRadius;
  G4UIcmdWithADoubleAndUnit* fBaffleZPosition;
 
      
  };

//
// First, one more container to avoid the search through the map more then once 
//
class LBNEVolumePlacements;

struct LBNEVolumePlacementData {
  LBNEVolumePlacements_AlignmentAlgo fAlignmentModel;
  bool fIsPlaced;
  G4ThreeVector fPosition;
  G4RotationMatrix fRotation;
  bool fRotationIsUnitMatrix;
  std::vector<double> fParams; // sizes, or others.. 
  G4VPhysicalVolume *fMother; // Not Owned. G4 maintain this pointer.  Do not delete in the destructor of this class. 
  G4LogicalVolume* fCurrent; // Same 
};

class LBNEVolumePlacements
{

private:
  LBNEVolumePlacements();
  ~LBNEVolumePlacements();
  LBNEVolumePlacements (LBNEVolumePlacements const &); // not implemented 
  LBNEVolumePlacements& operator=(LBNEVolumePlacements const&); // not implemented 
  static LBNEVolumePlacements* fInstance;

public:
//
// Alignment algorithm See also LBNE Surveyor 
// eNominal:  the element on the beam line is placed perfectly, no misalignment 
// eFixed:  The placement occurs after picking up the surveyed value for one or more points 
//          rigidly attached to the placed volume in question. 
// eRandom: The survey point is understood as a tolerance figure, the actual placement will be taken randomly
//           assuming the actual placement occurs within +- 2 sigma, truncated gaussian. (to be negotiated )
   
  static LBNEVolumePlacements* Instance(); 
  
  void InitializeMessenger(); // Can't be in the constructor, inf. loop otherwise
  void Initialize(const G4LogicalVolume* matriarch); // Define only the top of the hierarchy
 
  LBNEVolumePlacementData* Create(const G4String &name);     											    
  
  G4VPhysicalVolume* PlaceFinal(const G4String &name, G4VPhysicalVolume *mother,
                               LBNEVolumePlacements_AlignmentAlgo model=eNominal);
			       
  // No change to either this data or the establish Geant4 geometry. 
  // 
  void TestVolumeOverlap(const G4String &name, G4VPhysicalVolume *mother) const;

  const LBNEVolumePlacementData* GetDetectorPlacementData(const G4String &name);

  inline double GetTargetHallZ() { return fTargetHallZ; }
  inline double GetDecayHallZ() { return fDecayHallZ; }
  inline double GetDecayPipeLength() { return fDecayPipeLength; }
  inline double GetAbsorberHallZ() { return fAbsorberHallZ; }
  inline double GetHorn1Length() { return fHorn1Length; }
  inline double GetBaffleLength() { return fBaffleLength; }
  inline double GetBaffleInnerRadius() { return fBaffleInnerRadius; }
  inline double GetBaffleZPosition() { return fBaffleZPosition; }
  
  // Interface to the Detector construction classes, or others..  
   
  inline void SetTotalLengthOfRock(double l) { fTotalLength = l;}
  
 // Interface to the Messenger command 
 
  inline void SetDecayPipeLength(double l) {fDecayPipeLength=l;}
  inline void SetTotalLength(double l) {fTotalLength=l;}
  inline void SetWaterLayerThickInHorns(double l) { fWaterLayerThickInHorns = l;}
  inline void SetHorn1Length(double l) { fHorn1Length = l;}
  inline void SetBaffleLength(double l) { fBaffleLength = l;}
  inline void SetBaffleInnerRadius(double r) { fBaffleInnerRadius = r;}
  inline void SetBaffleZPosition(double z) { fBaffleZPosition = z;}
  
 // LBNEDetector construction interface.. Just a pointer.. 
 
   
  
private:
  // GUI Interface  
  
  // Messenger
  G4UImessenger *fPlacementMessenger;

  // maps, to map the name of the detector to its placement, rotation, and the
  // logical volume in which it is placed.
  
  std::map<G4String, LBNEVolumePlacementData> fSubVolumes;

  // Useful lengths/dimensions, gotten from the messenger class, or set internally.  

  G4double fTotalLength;
  G4double fTargetHallZ;
  G4double fAbsorberHallZ;
  G4double fDecayHallZ;
  G4double fDecayPipeLength; // equal the above for now.. 
  G4double fDecayPipeRadius;
  G4double fWaterLayerThickInHorns;
  G4double fHorn1Length;
  G4double fTargetAndBaffleLengthApprox; 
  G4double fHorn1UpstreamPlateLength; //???????????//
  G4double fHorn1DownstreamPlateLength; //???????????//
  //
  G4double fDistMCZeroToACTRN1Pts; // See drawing 8875.112-MD-363097, with Jim Hylen annotation. 
  // 
  G4double fBaffleInnerRadius;
  G4double fBaffleOuterRadius;
  G4double fBaffleLength;
  G4double fBaffleZPosition; // With respect to 0. of TargetHallAndHorn1 center (MCZERO) 
  //
  // Store the mother volume at top of volume hierarchy, for book-keeping/debugging purposes. 
  
  const G4LogicalVolume* fTopLogicalVolume;
  //
  // This method was found in G4PVPlacement. It is a clone, where we just skip 
  // the error messages. 
  
  bool CheckOverlaps(const G4PVPlacement *phys, G4int nres, G4double eps, G4bool verbose) const; 
};

#endif

