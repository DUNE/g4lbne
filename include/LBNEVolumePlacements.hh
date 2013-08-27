//---------------------------------------------------------------------------// 
// $Id: LBNEVolumePlacements.hh,v 1.1.2.17 2013/08/27 22:46:31 lebrun Exp $
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
  G4String fTypeName; // Box, tube, etc...
  G4VPhysicalVolume *fMother; // Not Owned. G4 maintain this pointer.  Do not delete in the destructor of this class. 
  G4LogicalVolume* fCurrent; // Same 
};

class LBNEHornRadialEquation  {

  public:
      LBNEHornRadialEquation(); // to be able to store in an stl vector. 
      LBNEHornRadialEquation(double rSqrtCoefficient, double zCoefficient, double rOffset, bool parabolic=true);
      double GetVal(double z) const ; 
      void test1() const; // Cross check for equation 1. Will generate G4Exception 
  
  private:
      static double inchDef;
      bool parabolic;
      double rCoeff;
      double zCoeff;
      double rOff;
      double rResc;
      double zResc;
      
  public:    
    inline double SetLongRescaleFactor(double r) {zResc = r;}
    inline double SetRadialRescaleFactor(double r) {rResc = r;}

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
  
  G4PVPlacement* PlaceFinal(const G4String &name, G4VPhysicalVolume *mother);
			       
  void PlaceFinalUpstrTarget(G4PVPlacement *mother);
  void PlaceFinalDownstrTarget(G4PVPlacement *mother);
 
  void PlaceFinalHorn1(G4PVPlacement *mother,  G4PVPlacement *motherDownstrTarget);
  			       
  // No change to either this data or the establish Geant4 geometry. 
  // 
  void TestVolumeOverlap(const G4String &name, G4VPhysicalVolume *mother) const;
  void PrintAll() const;

  const LBNEVolumePlacementData* GetDetectorPlacementData(const G4String &name);

  inline double GetTargetHallZ() const { return fTargetHallZ; }
  inline double GetDecayHallZ()  const { return fDecayHallZ; }
  inline double GetDecayPipeLength()  const { return fDecayPipeLength; }
  inline double GetAbsorberHallZ()  const { return fAbsorberHallZ; }
  inline double GetHorn1Length()  const { return fHorn1Length; }
  inline double GetBaffleLength() const  { return fBaffleLength; }
  inline double GetBaffleInnerRadius() const  { return fBaffleInnerRadius; }
  inline double GetBaffleZPosition()  const { return fBaffleZPosition; }
  
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
//
// Interface to the Messenger, Target stuff. 

  inline double GetTargetSLengthGraphite() const { return fTargetSLengthGraphite; }
  inline void SetTargetSLengthGraphite(double l) { fTargetSLengthGraphite = l; }
  inline double GetTargetLengthIntoHorn() const { return fTargetLengthIntoHorn; }
  inline void SetTargetLengthIntoHorn(double l) { fTargetLengthIntoHorn = l; }
//
// Interface to the Messenger, Horn1 parameters  
//
  inline void SetHorn1LongRescale(double r) {fHorn1LongRescale = r;}  
  inline void SetHorn1RadialRescale(double r) {fHorn1RadialRescale = r;}  
  inline void SetHorn2LongRescale(double r) {fHorn2LongRescale = r;}  
  inline void SetHorn2RadialRescale(double r) {fHorn2RadialRescale = r;}  
  inline double GetHorn2LongPosition() const { return fHorn2LongPosition; }
  inline void SetHorn2LongPosition(double l) { fHorn2LongPosition = l; }
  
  void SegmentTarget(); // Check the target segmentation. Assume fixed Fin size. 
  
  void RescaleHorn1Lengthwise();
  void RescaleHorn2Lengthwise();
  void RescaleHorn1Radially();
  void RescaleHorn2Radially();
  // Custom stuff for the upstream part of Horn1 
  
  LBNEVolumePlacementData* CreateHorn1TopLevelUpstr();
  
  
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
  G4double fHorn1DownstreamPlateLength; // Bulkhead, guess work at this point 
  //
  G4double fDistMCZeroToACTRN1Pts; // See drawing 8875.112-MD-363097, with Jim Hylen annotation. 
  // 
  G4double fBaffleInnerRadius;
  G4double fBaffleOuterRadius;
  G4double fBaffleLength;
  G4double fBaffleZPosition; // With respect to 0. of TargetHallAndHorn1 center (MCZERO) 
  //
  // Target description.  Variable name 
  // borrowed (inspired, actually) from LBNENumiDataInput, when applicable. 
  //
  G4double fTargetSLength; // total length from first fin Beryllium window end cap. 
  G4double fTargetSLengthGraphite;// total length for the graphite target per-se. **Settable via Messenger. 
  G4double fTargetLengthIntoHorn;
                       // offset with respecto MCZERO, and/or the transition between UpstreamTarget Hall and Horn1 Hall
		       // **Settable via Messenger. 
  G4double fTargetLengthOutsideHorn; // defined from the above.
  G4double fTargetSLengthDownstrEnd;
  G4double fTargetZOffsetStart; 
  // No fTargetX0, Y0:  These would be surveyed point, part of the alignement 
  // realm, i.e. handled in the Surveyor class.
  G4double fTargetFinWidth;
  G4double fTargetFinHeight;
  G4double fTargetFinLength; // 
  G4int fTargetNumFins;
  G4int fTargetNumFinsUpstr;
  G4int fTargetNumFinsInHorn;
  G4double fTargetFinLengthSplitUpstr; // 
  G4double fTargetFinLengthSplitDwnstr; // 
  G4String fTargetFinMaterial; // 
  G4double fTargetFinSpacingLength; // Averaged over the whole length
  G4double fTargetUpstrUpstrMargin; 
  G4double fTargetUpstrDwnstrMargin; 
  G4String fTargetUpstrPlateMaterial;
  G4double fTargetUpstrPlateHoleRadius;
  G4double fTargetUpstrPlateOuterRadius;
  G4double fTargetUpstrPlateThick;
  G4String fTargetCanMaterial;
  G4double fTargetCanInnerRadius;
  G4double fTargetCanOuterRadius;
  G4double fTargetCanLength;
  G4String fTargetDownstrCanFlangeMaterial;
  G4double fTargetDownstrCanFlangeOuterRadius;
  G4double fTargetDownstrCanFlangeInnerRadius;
  G4double fTargetDownstrCanFlangeThick;
  G4String fTargetFlangeMaterial;
  G4double fTargetFlangeInnerRadius;
  G4double fTargetFlangeOuterRadius;
  G4double fTargetFlangeThick;
  G4String fTargetCTubeMaterial;
  G4double fTargetCTubeOuterRadius;
  G4double fTargetCTubeInnerRadius;
  G4double fTargetCTubeUpstrLength;
  G4double fTargetCTubeUpstrOffset;
  G4double fTargetCTubeUpstrRadCurve;
  G4double fTargetCTubeReturnLengthUpstr;
  G4int fTargetCTubeUpstrNumSegCurve;
  G4double fTargetCTubeReturnHOffset;
  G4double fTargetCTubeReturnLengthDownstr;
  G4double fTargetCTubeReturnLengthUstr;
  G4double fTargetCTubeReturnLengthUpstrEnd;
  G4double fTargetDistFlangeToTargetStart;
  G4double fTargetAlignRingThick;
  G4double fTargetAlignRingInnerRadius;
  G4double fTargetAlignRingOuterRadius;
  G4double fTargetAlignRingCutAngle;
  G4String fTargetAlignRingMaterial;
  G4double fTargetAlignRingSpacing;
  G4int fMaxNumAlignRings;
  G4double fTargetBerylDownstrWindowThick;
  G4double fTargetBerylDownstrWindowRadius;
  G4double fTargetBerylUpstrWindowThick;
  G4double fTargetBerylUpstrWindowRadius;
  G4double fTargetHeContTubeThickness;
  G4double fTargetHeContTubeInnerRadius;
  G4double fTargetHeContTubeLengthUpstr;
  G4double fTargetHeContTubeLengthInHorn;
  //
  // derived quantities
  G4double fTargetZ0; // offset with respecto MCZERO 
  G4double fTargetZ0Upstr; // For the two sections of the target. 
  G4double fTargetZ0Downstr; // For the two sections of the target. 
  G4double fTargetSWidth;
  G4double fTargetSHeight;
  //
  // Variables that are part physicall part of Horn1 but located logically in G4, in UpstreamTargetAssembly. 
  // We decompose the Inner to Outer conductor transition into 5 section of cones. 
  //
  std::vector<G4double> fTargetHorn1InnerRadsUpstr;
  std::vector<G4double> fTargetHorn1InnerRadsDownstr;
  std::vector<G4double> fTargetHorn1TransThick;
  std::vector<G4double> fTargetHorn1Lengths;
  std::vector<G4double> fTargetHorn1ZPositions;
  //
  // Horn1, excluding the target elements in side the Horn1. 
  //
  // First, a rescaling factor with respect to current (NUMI based) design
  //
  G4double fHorn1RadialRescale;
  G4double fHorn1LongRescale;
  G4double fHorn1RadialSafetyMargin;
  
  G4double fHorn1IOTransLength; // Transition Inner to Outer conductors. A container volume (TUBS)
  G4double fHorn1IOTransInnerRad; // Surveyable!. But physically attachached to the top level section. 
  G4double fHorn1IOTransOuterRad;
  
  std::vector<G4double> fHorn1UpstrInnerRadsUpstr;
  std::vector<G4double> fHorn1UpstrInnerRadsDownstr; 
  std::vector<G4double> fHorn1UpstrInnerRadsOuterUpstr;
  std::vector<G4double> fHorn1UpstrInnerRadsOuterDownstr; 
  std::vector<G4double> fHorn1UpstrLengths;
  std::vector<G4double> fHorn1UpstrZPositions;

  std::vector<G4double> fHorn1UpstrOuterIOTransInnerRads; 
  std::vector<G4double> fHorn1UpstrOuterIOTransThicks; 
  std::vector<G4double> fHorn1UpstrOuterIOTransLengths; 
  std::vector<G4double> fHorn1UpstrOuterIOTransPositions; 

  G4double fHorn1TopUpstrLength; // Upstream part of the inner conductor, container volume (TUBS), Surveyed. 
  G4double fHorn1TopUpstrInnerRad; // This volume envelopes the target. 
  G4double fHorn1TopUpstrOuterRad;

  G4double fHorn1TopDownstrLength; // Do part of the inner conductor, container volume (TUBS), Surveyed. 
  G4double fHorn1TopDownstrOuterRad;
  G4double fHorn1NeckLength;
  G4double fHorn1NeckZPosition; // from the start of Horn1TopLevelUpstr
                                              
  // The outer tube
  
  G4double fHorn1OuterTubeInnerRad; 
  G4double fHorn1OuterTubeOuterRad; 
  
  G4double fHorn1OuterConnectorRad;  // rescaled radially as well. 
  G4double fHorn1OuterConnectorThick;
  G4double fHorn1OuterConnectorLength;
  G4double fHorn1OuterConnectorPosition;

  G4double fHorn1InnerConnectorRad; // dwonstream end connectors 
  G4double fHorn1InnerConnectorThick;
  G4double fHorn1InnerConnectorLength;
  G4double fHorn1InnerConnectorPosition;
  
  std::vector<LBNEHornRadialEquation> fHorn1Equations;
  //
  // ==========================================================
  // 
  // Horn2 
  //
  G4double fHorn2RadialRescale;
  G4double fHorn2LongRescale;
  G4double fHorn2LongPosition; // with data cards... 
  //
  G4double fHorn2Length; // Top level volume, surveyable. and rescal-able. 
  G4double fHorn2Radius;
  // We will subdivide Horn2 along the parts on Drawing 8875.112MD - 363383
  //  
  // Part 1 (index 0 in the array below) : Transition Inner to outer conductor . Drawing 363382
  // Part2.... Up to 7. 
  //
  std::vector<G4double> fHorn2PartsLengths; 
  std::vector<G4double> fHorn2PartsRadii;
  
  G4double fHorn2LengthMargin;   
  //
  // We approximate the upstream inner to outer transition (part 1) as a bunch tubs 
  //
  std::vector<G4double> fHorn2UpstrOuterIOTransRadsOne; 
  std::vector<G4double> fHorn2UpstrOuterIOTransRadsTwo; 
  std::vector<G4double> fHorn2UpstrOuterIOTransLengths; 
  std::vector<G4double> fHorn2UpstrOuterIOTransPositions; // with respect to the start of the mother volume
  //
  // And one cone.  
  //
  G4double fHorn2InnerRMinUpstr;
  G4double fHorn2InnerRMaxUpstr;
  G4double fHorn2InnerRMinDownstr;
  G4double fHorn2InnerRMaxDownstr;
  
  std::vector<LBNEHornRadialEquation> fHorn2Equations;

  // The outer tube
  
  G4double fHorn2OuterTubeInnerRad; 
  G4double fHorn2OuterTubeOuterRad; 
  
  G4double fHorn2OuterConnectorRad;  // rescaled radially as well. 
  G4double fHorn2OuterConnectorThick;
  G4double fHorn2OuterConnectorLength;
  G4double fHorn2OuterConnectorPosition;

  G4double fHorn2InnerConnectorRad; // dwonstream end connectors 
  G4double fHorn2InnerConnectorThick;
  G4double fHorn2InnerConnectorLength;
  G4double fHorn2InnerConnectorPosition;
  
  // a flag to check the geometry as it is constructed. 
  
  bool fCheckVolumeOverLapWC;
  
  // Store the mother volume at top of volume hierarchy, for book-keeping/debugging purposes. 
  
  const G4LogicalVolume* fTopLogicalVolume;
  //
  const LBNEVolumePlacementData* Find(const G4String &name, const char *motherName, const char *method);
  
  //
  // This method was found in G4PVPlacement. It is a clone, where we just skip 
  // the error messages.   
  bool CheckOverlaps(const G4PVPlacement *phys, G4int nres, G4double eps, G4bool verbose) const; 
  //
  // More intialization done here (too keep the constructor code of reasonable length.. )
  //
  void DeclareHorn1Dims(); 
  void CheckHorn1InnerConductAndTargetRadii(); // A check prior to the CheckOverlap of Geant4.  In principle, 
                                               // simpler and therefore more exact or reliable. 
					       // If fail, issues a fatal G4Exception. 
  int GetNumberOfInnerHornSubSections(size_t eqn, double z1, double z2, int nMax) const;
   
  void Horn1InstallSpiderHanger(const G4String &name, double zFromStartInnerConduct, double zPos,
                                const LBNEVolumePlacementData *plInfo,  G4PVPlacement *vMother );					       
  void Horn1PlaceAWeld(const G4String &name, double z, 
                                const LBNEVolumePlacementData *plInfo,  G4PVPlacement *vMother );					       
  void DeclareHorn2Dims(); 
};

#endif

