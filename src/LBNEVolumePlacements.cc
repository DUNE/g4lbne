//#include <fstream>

#include "LBNEDetectorConstruction.hh"
#include "LBNEDataInput.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UnitsTable.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Polycone.hh"
#include "G4Trap.hh"
#include "G4Cons.hh"
#include "G4Torus.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4PVReplica.hh"
#include "G4AssemblyVolume.hh"
#include "LBNEMagneticField.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PVPlacement.hh"
#include "G4RegionStore.hh"
#include "G4SolidStore.hh"
#include "G4GeometryManager.hh"
#include "G4FieldManager.hh"

#include "G4RunManager.hh"

#include "G4VisExtent.hh"

//---------------------------------------------------------------------------// 
// Handles the tricky volume placements 
//---------------------------------------------------------------------------// 
LBNEVolumePlacements *LBNEVolumePlacements::fInstance = 0;

LBNEVolumePlacements *LBNEVolumePlacements::Instance() {
  if (fInstance == 0) fInstance = new LBNEVolumePlacements;
  return fInstance;
}

LBNEVolumePlacements::LBNEVolumePlacements() {
  
  
    // declaration of some constant which should not impact the physics at all, 
    // but are needed to build the G4 geometry. 
   const G4double in = 2.54*cm;
   fDecayPipeLength=250.0*m;
   fTargetAndBaffleLengthApprox = in*(98.167 + 44.418); // NUMI Drawing 8875.000-ME-363028 
   fHorn1Length = 124.290*in;
   fHorn1UpstreamPlateLength = in*(5); //???????????//
   fHorn1DownstreamPlateLength = in*(5); //???????????//
   // Details
   fBaffleInnerRadius = 6.5*mm; // Per discussion with Jim Hylen, July 22 2013 
   fBaffleOuterRadius = 28.3*mm; // from previous version and le010z185i_NumiDPHelium.input
   fBaffleLength = 1474.8*mm; // Drawing 8875.000-ME-363028
   fBaffleZPosition = fTargetAndBaffleLengthApprox/2.
                       -1.0*(44.418*in - 1.256*in - 3.0*cm); 
		       // With respect to the center of the mother volume UpstreamTargetAssembly
//
// Target The first quantity is the target length as far as the beam is concerned 
//  Can be changed vis G4UIMessenger command 
  fTargetSLengthGraphite = 953.8*mm; // Russian drawing 7589-00-00-00 

  fTargetSLengthDownstrEnd = 59*mm;
  fTargetSLength = fTargetSLengthDownstrEnd + fTargetSLengthGraphite; // Russian drawing 7589-00-00-00 The 59 mm iscomputed as
    // 1022 + 23 - 32.2 - 953.8 // 32.2 is  fTargetDistFlangeToTargetStart
// 
// The following quantity can be changed via messenger command. 
// 
  fTargetLengthIntoHorn = 50.0*cm; //  LBNE Doc 6100, page 7 
  fTargetLengthOutsideHorn = fTargetSLength - fTargetLengthIntoHorn; 
  
  
  // No fTargetX0, Y0:  These would be surveyed point, part of the alignement 
  // realm, i.e. handled in the Surveyor class.
  fTargetFinHeight = 21.4*mm; // It includes half of the cooling tube, which will be inside the fins
                             // Drawing 7589-01-50-02 
  // The round edges (grinded 
//  fTargetFinWidth = 6.4*mm;
  fTargetFinWidth = 7.4*mm; // 6.4 -> 7.4 Per discussion with Jim and Alberto, July 25 2013. 
  fTargetFinLength = 20*mm; // 
  fTargetFinMaterial = G4String("Target"); // Only one type of target material 
//  fTargetNumberFins = 47; //  LBNE Doc 6100.. But we have to compute it from the graphite target length 
  // Since fTargetSLengthGraphite can be changed..Use the same algorithm.. 
  this->SegmentTarget(); 
  
  fTargetDowstreamBendLength = 0.9854*in; // barely readable on LBNE Doc 6100, figure 1
  fTargetUpstrUpstrMargin = 1.0*cm; // Perhaps one ought to extend it is Upstream vacuum flange thicker. 
  fTargetUpstrDwnstrMargin = 1.0*mm; // 
  fTargetUpstrPlateMaterial = std::string("Aluminum");
  fTargetUpstrPlateHoleRadius = (35.0/2.)*mm;//From Russian drawing 7589-00-00-00
  fTargetUpstrPlateOuterRadius = 214.*mm; //From Russian drawing 7589-00-00-00
  fTargetUpstrPlateThick = 18.4*mm; // From Russian drawing 7589-00-00-00
  fTargetCanMaterial = std::string("Aluminum");
  fTargetCanInnerRadius = (155./2.)*mm; // From Russian drawing 7589-00-00-00
  fTargetCanOuterRadius = fTargetCanInnerRadius + 6.6*mm;
  fTargetCanLength = 9.80*in; // read out from my scree from LBNE Doc 6100, page 1
  fTargetCanEndPlateThickness = 0.85*in;
  fTargetDownstrCanFlangeMaterial = std::string("Aluminum"); 
  fTargetDownstrCanFlangeInnerRadius = 13.8*mm;
  fTargetDownstrCanFlangeOuterRadius = fTargetCanInnerRadius; // Approximate
  fTargetDownstrCanFlangeThick = (3.0/8.0)*in;
  fTargetFlangeMaterial = std::string("Steel316"); 
  fTargetFlangeInnerRadius = fTargetDownstrCanFlangeInnerRadius ;
  fTargetFlangeOuterRadius= (8.24/2.)*in;;
  fTargetFlangeThick = 25.*mm;
  fTargetCTubeMaterial = std::string("Titanium"); 
  fTargetCTubeOuterRadius = (7.4 + 0.4)*mm ;
  fTargetCTubeInnerRadius = 7.4*mm;
  fTargetCTubeUpstrLength = 9.621*in; // Length from the upstream bend to the fron the steel drawnstream flange  
  fTargetCTubeUpstrOffset = 2.32*in; // from the beam line tp to the center of the cooling tube. 
  fTargetCTubeUpstrRadCurve = 0.965*in;
  fTargetCTubeUpstrNumSegCurve = 12; // 30 degrees angles Should be good enough. 
  fTargetCTubeReturnHOffset = 0.5*in; // guess
  fTargetCTubeReturnLengthDownstr = 0.985*in;
  fTargetDistFlangeToTargetStart = 32.2*mm;
  fTargetCTubeReturnLengthUpstr = fTargetDistFlangeToTargetStart + 10.25*in; // from the upstream end of target to 
                                 // Upstr bend return 
  fTargetCTubeReturnLengthUpstrEnd = 6.604*in - fTargetDistFlangeToTargetStart;
  fTargetAlignRingMaterial = std::string("Steel316");
  fTargetAlignRingThick = 0.25*in; //guess. 
  fTargetAlignRingInnerRadius = (18.0/2)*mm;
  fTargetAlignRingOuterRadius = (29.5/2)*mm;
  
  fTargetHeContTubeInnerRadius = 30.0*mm;
  fTargetHeContTubeThickness = 0.4*mm;
  fTargetHeContTubeLengthInHorn = fTargetLengthIntoHorn + fTargetSLengthDownstrEnd;
  fTargetHeContTubeLengthUpstr = 2.0*in + fTargetCanOuterLength  + fTargetDownstrCanFlangeThick + 
                                 fTargetFlangeThick + fTargetDistFlangeToTargetStart;
  fTargetBerylDownstrWindowThick = 1.0*mm; // ???? pure guess 
  fTargetBerylDownstrWindowRadius = fTargetHeContTubeInnerRadius + fTargetHeContTubeThickness;
  fTargetBerylUpstrWindowThick = 1.0*mm; // pure guess 
  fTargetBerylUpstrWindowRadius = 0.8425*in/2.; // ???? Again, guessing... 
				 
   //
   // Basic algorithm to determine position of the target main daughter volumes 
   //
  fTargetZ0Usptr = (-1.0* (fTargetSLength - fTargetLengthIntoHorn))/2. ;
        // offset with respecto MCZERO of the center of target mother volume,  
  fTargetZ0Downstr = fTargetLengthIntoHorn/2. ;
  
  fWaterLayerThickInHorns = 0.; 
   
}

LBNEVolumePlacements::~LBNEVolumePlacements() {
  delete fPlacementMessenger;
}
void LBNEVolumePlacements::InitializeMessenger() {
//   std::cerr << " LBNEVolumePlacements::Initialize, about to instantiate PlacementMessengers " << std::endl;
   fPlacementMessenger = new LBNEPlacementMessenger();
}
void LBNEVolumePlacements::Initialize(const G4LogicalVolume* matriarch) {

   fTopLogicalVolume = matriarch;
   std::cerr << " LBNEVolumePlacements::Initialize, about to instantiate PlacementMessengers " << std::endl;
   fPlacementMessenger = new LBNEPlacementMessenger();
   
}

LBNEVolumePlacementData* 
   LBNEVolumePlacements::Create(const G4String &name) {  

  // look for a pre-existing placement.  If there, fatal error, exit 
  LBNEVolumePlacementData info;
  info.fAlignmentModel = eNominal;
  info.fIsPlaced = false;
  info.fRotation = G4RotationMatrix(); // a bit of a waste of space, as, in most cases, not rotations are applied. 
  info.fRotationIsUnitMatrix= true;
  info.fPosition = G4ThreeVector(0., 0., 0.);
  info.fParams.resize(3, 0.);
  info.fMother = 0;
  std::string volumeName(name); // volumeName += std::string("_solid"); Not necessary 
  if (name == G4String("Tunnel")) {
    info.fParams[0] = 8*m - 2.0*cm;  // Note: the total volume is 60 m. wide => plenty enough rocks. 
    info.fParams[1] = 12*m - 2.0*cm; 
    info.fParams[2] = fTotalLength -2.0*cm;
    G4Box* hallBox = new G4Box(volumeName, info.fParams[0]/2., info.fParams[1]/2., info.fParams[2]/2. ); 
    info.fCurrent = new G4LogicalVolume(hallBox, G4Material::GetMaterial("Air"), name); 
  }
  if (name == G4String("TargetHallAndHorn1")) {
    std::map<G4String, LBNEVolumePlacementData>::const_iterator it = fSubVolumes.find(G4String("Tunnel"));
    if (it == fSubVolumes.end()) {
       std::cerr << " LBNEVolumePlacements::Create, internal error for " << name << " fatal " << std::endl;
      exit(2);
    }
    for (size_t k=0; k != 2; ++k) 
      info.fParams[k] = it->second.fParams[k] - 2.0*cm; 
    info.fParams[2] = fHorn1UpstreamPlateLength + fHorn1DownstreamPlateLength 
                      + fHorn1Length + 0.5 + fTargetAndBaffleLengthApprox  ;
    G4Box* hallBox = new G4Box(volumeName, info.fParams[0]/2., info.fParams[1]/2., info.fParams[2]/2. );
    info.fCurrent = new G4LogicalVolume(hallBox, G4Material::GetMaterial("Air"), volumeName); 
    info.fPosition[2] = 0.; // Definiing MCZERO, Drawing 8875. 112-MD-363097, annotated by jim Hylen
  
  } else if (name == G4String("UpstreamTargetAssembly")) {
    std::map<G4String, LBNEVolumePlacementData>::const_iterator it = 
        fSubVolumes.find(G4String("TargetHallAndHorn1"));
    if (it == fSubVolumes.end()) {
      std::cerr << " LBNEVolumePlacements::Create, internal error for " << name << " fatal " << std::endl;
      exit(2);
    }  
    for (size_t k=0; k != 2; ++k) 
      info.fParams[k] = it->second.fParams[k] - 0.5*cm; 
    info.fParams[2] = fTargetAndBaffleLengthApprox;
    G4Box* hallBox = new G4Box(volumeName, info.fParams[0]/2., info.fParams[1]/2., info.fParams[2]/2. );
    info.fCurrent = new G4LogicalVolume(hallBox, G4Material::GetMaterial("Air"), volumeName); 
    info.fPosition[2] = -fTargetAndBaffleLengthApprox/2.; //preliminary
//    std::cerr << " LBNEVolumePlacements::Create " << name << " half length " << info.fParams[2]/2. << std::endl;
      
  } else if (name == G4String("Horn1Hall")) {
    std::map<G4String, LBNEVolumePlacementData>::const_iterator it = 
      fSubVolumes.find(G4String("TargetHallAndHorn1"));
    if (it == fSubVolumes.end()) {
      std::cerr << " LBNEVolumePlacements::Create, internal error for " << name << " fatal " << std::endl;
      exit(2);
    }  
    for (size_t k=0; k != 2; ++k) 
      info.fParams[k] = it->second.fParams[k] - 0.5*cm; 
    info.fParams[2] = fHorn1UpstreamPlateLength + fHorn1Length + fHorn1DownstreamPlateLength;
    G4Box* hallBox = new G4Box(volumeName, info.fParams[0]/2., info.fParams[1]/2., info.fParams[2]/2. );
    info.fCurrent = new G4LogicalVolume(hallBox, G4Material::GetMaterial("Air"), volumeName); 
    info.fPosition[2] = fHorn1UpstreamPlateLength + fHorn1Length/2.; // preliminary. 
    std::cerr << " Checking stuff for " << name << " Mother volume size, x, y ,z "; 
      for (size_t k=0; k!= 3; k++)  std::cerr << it->second.fParams[k]/m << ", " ; std::cerr << std::endl;
    std::cerr << " This volume size, x, y ,z "; 
      for (size_t k=0; k!= 3; k++)  std::cerr << info.fParams[k]/m << ", " ; std::cerr << std::endl;
    std::cerr << " Tentatively placed at x, y ,z "; 
      for (size_t k=0; k!= 3; k++)  std::cerr << info.fPosition[k]/m << ", " ; std::cerr << std::endl;
  } else if (name == G4String("Baffle")) {
  // more cases here... a long list.. a too long of a  list? 
  // We now go one level down in the hierachy. Start from upstream 
    std::map<G4String, LBNEVolumePlacementData>::const_iterator it = 
        fSubVolumes.find(G4String("UpstreamTargetAssembly"));
    if (it == fSubVolumes.end()) {
      std::cerr << " LBNEVolumePlacements::Create, internal error for " << name << " fatal " << std::endl;
      exit(2);
    } 
    info.fParams[0] = fBaffleInnerRadius; 
    info.fParams[1] = fBaffleOuterRadius; 
    info.fParams[2] = fBaffleLength;
    G4Tubs* baffleTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
    info.fCurrent = new G4LogicalVolume(baffleTube, G4Material::GetMaterial("GraphiteBaffle"), volumeName); 
    info.fPosition[2] = fBaffleZPosition; //preliminary
  } 
  //
  // Target stuff.. Lots of it
  // 
  if (name.find("Target") != std::string:npos) {
    if (name.find("TargetUpstr") != std::string:npos) {
      std::map<G4String, LBNEVolumePlacementData>::const_iterator it = 
        fSubVolumes.find(G4String("UpstreamTargetAssembly"));
      if (it == fSubVolumes.end()) {
        std::cerr << " LBNEVolumePlacements::Create, internal error for " << name << " fatal " << std::endl;
        exit(2);
      } 
      if (name == G4String("TargetUpstrMTop")) { 
         // Complete container the target Canister and the portion of the target outside the horn 
	 // Subject to misalignment!!!
         info.fParams[1] = fTargetUpstrPlateOuterRadius + 2.0*cm; // mother volume, safety factor of 2 cm mm transversely   
         info.fParams[2] = fTargetUpstrUpstrMargin  + fTargetUpstrPlateThick + fTargetCanLength 
	                   + fTargetDownstrCanFlangeThick + fTargetFlangeThick + 0.2*mm +
			   fTargetUpstrDwnstrMargin  +  fTargetDistFlangeToTargetStart + fTargetLengthOutsideHorn;
         G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
         info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial("Air"), volumeName); 
         info.fPosition[2] = it->fParams[2]/2. -info.fParams[2]/2. - 0.01*mm; // Nominal: 
	           // Half length of UpstreamTargetAssembly - length of this  
       } 
      if (name == G4String("TargetUpstrM0")) { // Complete container for the target Canister and flange  
         info.fParams[1] = fTargetUpstrPlateOuterRadius + 1.0*cm; // mother volume, safety factor of 100 microns    
         info.fParams[2] = fTargetUpstrUpstrMargin  + fTargetUpstrPlateThick + fTargetCanLength 
	                   + fTargetDownstrCanFlangeThick + fTargetFlangeThick;
         G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
         info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial("HeliumTarget"), volumeName); 
         std::map<G4String, LBNEVolumePlacementData>::const_iterator itMTop = fSubVolumes.find(G4String("TargetUpstrMTop"));
         if (it == fSubVolumes.end()) {
          std::cerr << " LBNEVolumePlacements::Create, internal error for " << name << " fatal " << std::endl;
          exit(2);
         } 
         info.fPosition[2] = -1.0*itMTop->fParams[2]/2. + info.fParams[2]/2. + 0.01*mm; // 
       } 
       if (name.find("TargetUpstrUpstr") != std::string:npos) {
          std::map<G4String, LBNEVolumePlacementData>::const_iterator it = 
          fSubVolumes.find(G4String("TargetUpstrM0"));
          if (it == fSubVolumes.end()) {
            std::cerr << " LBNEVolumePlacements::Create, internal error for " << name << " fatal " << std::endl;
            exit(2);
          } 
          if (name == G4String("TargetUpstrUpstrPlate")) { // 
            info.fParams[0] = fTargetUpstrPlateHoleRadius; 
            info.fParams[1] = fTargetUpstrPlateOuterRadius; 
            info.fParams[2] = fTargetUpstrPlateThick ;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial("Aluminum"), volumeName); 
            info.fPosition[2] = -1.0*it->fParams[2]/2.0 + fTargetUpstrUpstrMargin + fTargetUpstrPlateThick/2.;
         } 
           if (name == G4String("TargetUpstrUpstrCan")) { // 
            info.fParams[0] = fTargetCanInnerRadius; 
            info.fParams[1] = fTargetCanOuterRadius; 
            info.fParams[2] = fTargetCanLength ;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(fTargetCanMaterial), volumeName); 
            info.fPosition[2] = -1.0*it->fParams[2]/2.0 + fTargetUpstrUpstrMargin +
	                        fTargetUpstrPlateThick + fTargetCanLength/2.0;
         } 
           if (name == G4String("TargetUpstrUpstrCanEndPlate")) { // 
            info.fParams[0] = fTargetUpstrPlateHoleRadius; 
            info.fParams[1] = fTargetUpstrPlateOuterRadius + 0.5*in; // ?? Guess  for the 1/2 inch extra. not critical 
            info.fParams[2] = fTargetCanEndPlateThickness;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(fTargetCanMaterial), volumeName); 
            info.fPosition[2] = -1.0*it->fParams[2]/2.0 + fTargetUpstrUpstrMargin +
	                        fTargetUpstrPlateThick + fTargetCanLength + fTargetCanEndPlateThickness/2.;
         } 
           if (name == G4String("TargetUpstrUpstrDwstrFlange")) { // 
            info.fParams[0] = fTargetFlangeInnerRadius; 
            info.fParams[1] = fTargetFlangeOuterRadius; 
            info.fParams[2] = fTargetFlangeThick ;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(fTargetFlangeMaterial), volumeName); 
            info.fPosition[2] = -1.0*it->fParams[2]/2.0 + fTargetUpstrUpstrMargin +
	                         fTargetUpstrPlateThick + fTargetCanLength + 
				 fTargetCanEndPlateThickness + fTargetFlangeThick/2.;
         }
           if (name == G4String("TargetUpstrUpstrCoolingTube")) { // 
            info.fParams[0] = 0.; // Solid block of copper, we'll define water below 
            info.fParams[1] = fTargetCTubeOuterRadius; 
            info.fParams[2] = fTargetCTubeUpstrLength ;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(fTargetCTubeMaterial), volumeName); 
	    // Leave the position blanks, as there will two copies (top and bottom.) 
	 }
           if (name == G4String("TargetUpstrUpstrCoolingTubeWater")) { // 
            info.fParams[0] = 0.; // Solid block of copper, we'll define water below 
            info.fParams[1] = fTargetCTubeInnerRadius; 
            info.fParams[2] = fTargetCTubeUpstrLength ;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("Water")), volumeName); 
	    // Leave the position blanks, as there will two copies (top and bottom.) 
	 }
	 /*
           if (name == G4String("TargetUpstrUpstrSupporBlock")) { // 
            info.fParams[0] = 25.0*mm; // A block of presumed steel to support cooling tubes. 
            info.fParams[1] = 50.0*mm; 
            info.fParams[2] = 11.0*mm;
            G4Tubs* aBox = new G4Box(volumeName, info.fParams[0]/2., info.fParams[1]/2, info.fParams[2]/2.);
            info.fCurrent = new G4LogicalVolume(aBox, G4Material::GetMaterial(std::string("steel316")), volumeName); 
	    info.fPosition[2] = -1.0*it->fParams[2]/2.0 + fTargetUpstrUpstrMargin +
	                         fTargetUpstrPlateThick + 140*mm;// Drawing 7589-00-00-00
	 }
	 */ // If implemented, need to split cooling tubes.  
           if (name == G4String("TargetUpstrUstrHorFin")) { // 
            info.fParams[0] = fTargetFinHeight; // X-Y inverted 
            info.fParams[1] = fTargetFinWidth; 
            info.fParams[2] = fTargetFinLength;
            G4Tubs* aBox = new G4Box(volumeName, info.fParams[0]/2., info.fParams[1]/2, info.fParams[2]/2.);
            info.fCurrent = new G4LogicalVolume(aBox, G4Material::GetMaterial(std::string("Target"), volumeName); 
	    info.fPosition[2] = -1.0*it->fParams[2]/2.0 + fTargetUpstrUpstrMargin +
	                         fTargetUpstrPlateThick + 140*mm;// Drawing 7589-00-00-00
	 }
	 // The following are probably too detailed, and too far away from axis. 
	 // Will not be used for now... 
           if (name == G4String("TargetUpstrUpstrCoolingTubeBendSection")) { // 
            info.fParams[0] = 0.; // Solid block of copper, we'll define water below 
            info.fParams[1] = fTargetCTubeOuterRadius; 
            info.fParams[2] = fTargetCTubeUpstrRadCurve/fTargetCTubeUpstrNumSegCurve - 0.1*mm; // 100 microns to avoid overlaps
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(fTargetCTubeMaterial), volumeName); 
	    // Leave the position blanks, as there will two copies (top and bottom.) 
	 }
           if (name == G4String("TargetUpstrUpstrCoolingTubeBendSectionWater")) { // 
            info.fParams[0] = 0.; // Solid block of copper, we'll define water below 
            info.fParams[1] = fTargetCTubeInnerRadius; 
            info.fParams[2] = fTargetCTubeUpstrRadCurve/fTargetCTubeUpstrNumSegCurve - 0.1*mm;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("Water")), volumeName); 
	    // Leave the position blanks, as there will two copies (top and bottom.) 
	 }
           if (name == G4String("TargetUpstrUpstrCoolingTubeFlangeSection")) { // 
            info.fParams[0] = 0.; // Solid block of copper, we'll define water below 
            info.fParams[1] = fTargetCTubeOuterRadius; 
            info.fParams[2] = fTargetFlangeThick;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(fTargetCTubeMaterial), volumeName); 
	    // Leave the position blanks, as there will be two copies (top and bottom.) 
	 }
           if (name == G4String("TargetUpstrUpstrCoolingTubeFlangeSectionWater")) { // 
            info.fParams[0] = 0.; // Solid block of copper, we'll define water below 
            info.fParams[1] = fTargetCTubeInnerRadius; 
            info.fParams[2] = fTargetFlangeThick;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("Water")), volumeName); 
	    // at (0.,0., 0.) 
	 }
                 
       } // TargetUpstrM0 as mother volume. 
       if (name == G4String("TargetUpstrM1")) { // Complete container volume in the upstream target hall 
                                                // to contain whatever if dowstream of the above flange 
						// and upstream of the Horn. 
         info.fParams[1] = fTargetHeContTubeInnerRadius + fTargetHeContTubeThickness+ 1.0*cm;
	        // mother volume, safety factor of 1. 0 cm  
         info.fParams[2] = fTargetUpstrDwnstrMargin  +  fTargetDistFlangeToTargetStart + fTargetLengthOutsideHorn; 
         G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
         info.fCurrent = new G4LogicalVolume(baffleTube, G4Material::GetMaterial(std::string("Air"), volumeName); 
         std::map<G4String, LBNEVolumePlacementData>::const_iterator itMTop = fSubVolumes.find(G4String("TargetUpstrMTop"));
         if (it == fSubVolumes.end()) {
          std::cerr << " LBNEVolumePlacements::Create, internal error for " << name << " fatal " << std::endl;
          exit(2);
         } 
         info.fPosition[2] = itMTop->fParams[2]/2. - info.fParams[2]/2. - 0.005*mm; // in TargetUpstrMTop 
       } 
       if (name.find("TargetUpstrDownstr") != std::string:npos) {
          std::map<G4String, LBNEVolumePlacementData>::const_iterator it = 
          fSubVolumes.find(G4String("TargetUpstrM1"));
          if (it == fSubVolumes.end()) {
            std::cerr << " LBNEVolumePlacements::Create, internal error for " << name << " fatal " << std::endl;
            exit(2);
          } 
          if (name == G4String("TargetUpstrDowstrHeContainer")) { // 
            info.fParams[0] = 0.; 
            info.fParams[1] = fTargetHeContTubeInnerRadius + fTargetHeContTubeThickness; 
            info.fParams[2] = fTargetUpstrDwnstrMargin/2.  +  fTargetDistFlangeToTargetStart + fTargetLengthOutsideHorn ;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("Beryllium")), volumeName); 
            info.fPosition[2] = 0.; // possibly tweak for the margin... 
         }
          if (name == G4String("TargetUpstrDowstrHelium")) { // 
            info.fParams[0] = 0.; 
            info.fParams[1] = fTargetHeContTubeInnerRadius; 
            info.fParams[2] = fTargetUpstrDwnstrMargin/4.  +  fTargetDistFlangeToTargetStart + fTargetLengthOutsideHorn ;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("HeliumTarget")), volumeName); 
            info.fPosition[2] = 0.; // possibly tweak for the margin... 
         }
          if (name == G4String("TargetUpstrDowstrCoolingTube")) { // 
            info.fParams[0] = 0.; 
            info.fParams[1] = fTargetCTubeOuterRadius; 
            info.fParams[2] = fTargetUpstrDwnstrMargin/4.  +  fTargetDistFlangeToTargetStart + fTargetLengthOutsideHorn ;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("Titanium")), volumeName); 
            info.fPosition[2] = fTargetFinHeight + 0.005*mm; // possibly tweak for the margin... 
         }
          if (name == G4String("TargetUpstrDowstrCoolingTubeWater")) { // 
            info.fParams[0] = 0.; 
            info.fParams[1] = fTargetCTubeInnerRadius; 
            info.fParams[2] = fTargetUpstrDwnstrMargin/8.  +  fTargetDistFlangeToTargetStart + fTargetLengthOutsideHorn ;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("Water")), volumeName); 
         }
          if (name == G4String("TargetUpstrDowstrCoolingTubeWater")) { // 
            info.fParams[0] = 0.; 
            info.fParams[1] = fTargetCTubeInnerRadius; 
            info.fParams[2] = fTargetUpstrDwnstrMargin/8.  +  fTargetDistFlangeToTargetStart + fTargetLengthOutsideHorn ;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("Water")), volumeName); 
         }
	 // 
       } // Downstream of the flange of the target canister. 
    } // Upstream Stuff 
    if (name == G4String("TargetFinVert")) { // 
            info.fParams[0] = fTargetFinWidth; 
            info.fParams[1] = fTargetFinHeight; 
            info.fParams[2] = fTargetFinLength ;
            G4Tubs* aBox = new G4Box(volumeName, info.fParams[0]/2, info.fParams[1]/2, info.fParams[2]/2.);
            info.fCurrent = new G4LogicalVolume(aBox, G4Material::GetMaterial(std::string("Target")), volumeName); 
    }
    if (name == G4String("TargetFinVertLast")) { // 
            info.fParams[0] = fTargetFinWidth; 
            info.fParams[1] = fTargetFinHeight; 
            info.fParams[2] = fTargetFinLengthSplitUpstr ;
            G4Tubs* aBox = new G4Box(volumeName, info.fParams[0]/2, info.fParams[1]/2, info.fParams[2]/2.);
            info.fCurrent = new G4LogicalVolume(aBox, G4Material::GetMaterial(std::string("Target")), volumeName);
 // despite being a single copy, do this in a separate method. 	    
    }
    // Target in Horn1 comes next... 
    
  } // End of Target declarations 
  //
   
  fSubVolumes.insert(std::pair<G4String, LBNEVolumePlacementData>(name, info));
  return &(fSubVolumes.find(name)->second);
}
// Place a volume, single copy. 
G4PVPlacement* LBNEVolumePlacements::PlaceFinal(const G4String &name, G4VPhysicalVolume *mother,
                               LBNEVolumePlacements_AlignmentAlgo model) {
			       
    G4LogicalVolume *vMother=mother->GetLogicalVolume();			       
    std::map<G4String, LBNEVolumePlacementData>::iterator it = fSubVolumes.find(name);
    if (it == fSubVolumes.end()) {
      std::cerr << " LBNEVolumePlacements::PlaceFinal, internal error for " 
                << name << " in mother volume " << vMother->GetName() << std::endl;
      std::cerr << " Such a volume is undefined.  Fatal.  " << std::endl;
      exit(2);
    }  
    LBNEVolumePlacementData &info=it->second;  
    info.fMother  =  mother; 
    std::cerr << " LBNEVolumePlacements::PlaceFinal, " << name << " half size " 
                << info.fParams[2]/2. << " tentative position " << info.fPosition[2] << std::endl;
    
    if (name == G4String("UpstreamTargetAssembly")) {
      // Special cases : not at the center of the mother volume.  It's downstream edge is the border 
      // 10 micron safety zone. 
      info.fPosition[2]=-info.fParams[2]/2. - 0.010*mm;
      std::cerr << " LBNEVolumePlacements::PlaceFinal, " << name << " half size " 
                << info.fParams[2]/2. << " position " << info.fPosition[2] << std::endl;
    }
    G4VPhysicalVolume *placement=0;
    G4String vpName(name); vpName += G4String("_P");
    if (info.fRotationIsUnitMatrix) 
        placement=new G4PVPlacement((G4RotationMatrix *) 0, 
	                             info.fPosition, info.fCurrent, vpName, vMother, false, 0);
    else    
        placement=new G4PVPlacement(&info.fRotation, info.fPosition, info.fCurrent, 
	                             vpName, vMother, false, 0);
    info.fIsPlaced = true;				     
    return placement;			       
}

G4VPhysicalVolume* LBNEVolumePlacements::PlaceFinalUpstrTarget(G4VPhysicalVolume *mother,
                               LBNEVolumePlacements_AlignmentAlgo model) {
			       
    G4LogicalVolume *vMother=mother->GetLogicalVolume();			       
    std::map<G4String, LBNEVolumePlacementData>::iterator itM0 = fSubVolumes.find("UsptrTargetM0");
    if (it == fSubVolumes.end()) {
      std::cerr << " LBNEVolumePlacements::PlaceFinalUpstrTarget, internal error for " 
                << name << " in mother volume " << vMother->GetName() << std::endl;
      std::cerr << " Such a volume is undefined.  Fatal.  " << std::endl;
      exit(2);
    }  
    LBNEVolumePlacementData &info=it->second;  
    info.fMother  =  mother; 
    std::cerr << " LBNEVolumePlacements::PlaceFinalUpstrTarget, " << name << " half size " 
                << info.fParams[2]/2. << " tentative position " << info.fPosition[2] << std::endl;
    
    // 
    
    if (name == G4String("UpstreamTargetAssembly")) {
      // Special cases : not at the center of the mother volume.  It's downstream edge is the border 
      // 10 micron safety zone. 
      info.fPosition[2]=-info.fParams[2]/2. - 0.010*mm;
      std::cerr << " LBNEVolumePlacements::PlaceFinal, " << name << " half size " 
                << info.fParams[2]/2. << " position " << info.fPosition[2] << std::endl;
    }
    G4VPhysicalVolume *placement=0;
    G4String vpName(name); vpName += G4String("_P");
    if (info.fRotationIsUnitMatrix) 
        placement=new G4PVPlacement((G4RotationMatrix *) 0, 
	                             info.fPosition, info.fCurrent, vpName, vMother, false, 0);
    else    
        placement=new G4PVPlacement(&info.fRotation, info.fPosition, info.fCurrent, 
	                             vpName, vMother, false, 0);
    info.fIsPlaced = true;				     
    return placement;			       
}
			       
void LBNEVolumePlacements::TestVolumeOverlap(const G4String &name, G4VPhysicalVolume *mother) const {

//    G4PVPlacement *pvpMother= static_cast<G4PVPlacement *>(mother);
    G4LogicalVolume *vMother=mother->GetLogicalVolume();			       
    std::map<G4String, LBNEVolumePlacementData>::const_iterator it = fSubVolumes.find(name);
    if (it == fSubVolumes.end()) {
      std::cerr << " LBNEVolumePlacements::TestVolumeOverlap, internal error for " 
                << name << " Such a volume is undefined, fatal " << std::endl;
      exit(2);
    }  
    LBNEVolumePlacementData infoTmp(it->second); // deep copy 
    G4VPhysicalVolume *placement=0;
    const int numTrialsMax = 100;
    int numTrials = 0;
    if (name == G4String("Horn1Hall")) {
     // "blind" test: shift until we are o.k. 
      std::cerr << " LBNEVolumePlacements::TestVolumeOverlap for volume " << name 
                << " in mother volume " << vMother->GetName() << std::endl;
      while (numTrials < numTrialsMax) {
       
       // Special cases : not at the center of the mother volume.  It's downstream edge is the border 
      // 10 micron safety zone. 
       infoTmp.fPosition[2]= infoTmp.fParams[2]/2. + 0.010*mm - 5.0*mm ; // last 5 mm is a mistake... 
       infoTmp.fPosition[2] += (G4RandGauss::shoot(0., 3.0))*mm; // randomly trial 
       std::cerr << " ... z Offset = " << infoTmp.fPosition[2] - infoTmp.fParams[2]/2. 
                << " mm, half size " <<  infoTmp.fParams[2]/2. << " mm, Pos " << infoTmp.fPosition[2] <<  std::endl;
       // Ignore rotations for this tests 
       std::string vpName(name); vpName += std::string("_Ptmp");
//       std::cerr << " ... Before placing  " << vpName << " at Z = " << infoTmp.fPosition[2] << std::endl;
       G4PVPlacement *placement=new G4PVPlacement((G4RotationMatrix *) 0, infoTmp.fPosition, 
                                    infoTmp.fCurrent, vpName, vMother, false, 0, false);
//       std::cerr << " ... Physical Volume   " << placement->GetName() << " placed "  << std::endl;
       if (!this->CheckOverlaps(placement, 5000, 1.0e-4, false)) {
          std::cerr << " O.K., it fits ! " << std::endl;
          break;
       } else {
           std::cerr << " Nop, it does not fit, try again  " << std::endl;
           std::map<G4String, LBNEVolumePlacementData>::const_iterator itM = fSubVolumes.find(vMother->GetName());
           if (itM == fSubVolumes.end()) {
              std::cerr << " LBNEVolumePlacements::TestVolumeOverlap, internal error for " 
                << vMother->GetName() << " Such a volume is undefined, fatal " << std::endl;
              exit(2);
           }  
           std::cerr << " Check Mother volume length " << itM->second.fParams[2] << " mm " << std::endl;
           mother->GetLogicalVolume()->RemoveDaughter(placement);
	   
       }
       numTrials++;
     }
     // Since this is only a test, remove it; 
      mother->GetLogicalVolume()->RemoveDaughter(placement);
    }  
}   

// Compute the number of segments and adjust the overall length, and the spacing such that 
// We still have a fixed number of 2 cm long pieces of graphite. 

void LBNEVolumePlacements::SegmentTarget() {

  fTargetFinSpacingLength = 0.3*mm; // Nominal value.. 
  double  targetNumFinsNominal = fTargetSLengthGraphite/(fTargetFinLength + fTargetFinSpacingLength); 
  double deltaNumFins =  targetNumFinsNominal - (int) targetNumFinsNominal; 
  fTargetNumFins = (int) targetNumFinsNominal;
  if (deltaNumFins > 0.5*fTargetFinLength) fTargetNumFins++;
  const double oldLength = fTargetSLengthGraphite
  fTargetSLengthGraphite = fTargetNumFins*fTargetFinLength + (fTargetNumFins-1)*fTargetFinLength; 
  fTargetFinSpacingLength = (fTargetSLengthGraphite  - fTargetNumberFins*fTargetFinLength)/(fTargetNumberOfFins - 1); //   
  if (std::abs(oldLength - fTargetSLengthGraphite) > 0.001*fTargetSLengthGraphite)
     std::cout << " LBNEVolumePlacements::segmentTarget: Warning: re-adjust the target length from " 
        oldLength <<  " to " << fTargetSLengthGraphite << " to get  an integer number of 2 cm long segments " << std::endl;
  // Compute the number of fins upstream and dowsntream of the break between target and horn1.
   
  double tempNFins = (fTargetLengthOutsideHorn - fTargetFinSpacingLength) /(fTargetFinLength + fTargetFinSpacingLength);
  fTargetNumFinsUpstr = (int) tempNFins;	
  fTargetFinLengthSplitUpstr = fTargetLengthOutsideHorn - fTargetFinSpacingLength - 
                               fTargetNumFinsUpstr*(fTargetFinLength + fTargetFinSpacingLength);
  fTargetFinLengthSplitDwnstr = fTargetFinLength - fTargetFinLengthSplitUpstr;			       
} 
  
//
// Currently a clone of G4PVPlacement::CheckOverlaps.  But we plan to upgrade it 
// to handle volume that interesct each other at tiny surfaces. 
// 
bool LBNEVolumePlacements::CheckOverlaps(const G4PVPlacement *plVol, G4int res, G4double tol, G4bool verbose) const {

  if (res<=0) { return false; }

  G4VSolid* solid = plVol->GetLogicalVolume()->GetSolid();
  G4LogicalVolume* motherLog = plVol->GetMotherLogical();
  if (!motherLog) { return false; }

  G4VSolid* motherSolid = motherLog->GetSolid();

  if (verbose)
  {
    G4cout << "Checking overlaps for volume " << plVol->GetName() << " ... ";
  }

  // Create the transformation from daughter to mother
  //
  G4AffineTransform Tm( plVol->GetRotation(), plVol->GetTranslation() );

  for (G4int n=0; n<res; n++)
  {
    // Generate a random point on the solid's surface
    //
    G4ThreeVector point = solid->GetPointOnSurface();

    // Transform the generated point to the mother's coordinate system
    //
    G4ThreeVector mp = Tm.TransformPoint(point);

    // Checking overlaps with the mother volume
    //
    if (motherSolid->Inside(mp)==kOutside)
    {
      G4double distin = motherSolid->DistanceToIn(mp);
      if (distin > tol)
      {
        if (verbose) G4cout << G4endl;
        if (verbose) G4cout << "WARNING - G4PVPlacement::CheckOverlaps()" << G4endl
               << "          Overlap is detected for volume "
               << plVol->GetName() << G4endl
               << "          with its mother volume "
               << motherLog->GetName() << G4endl
               << "          at mother local point " << mp << ", "
               << "overlapping by at least: " << G4BestUnit(distin, "Length")
               << G4endl;
        if (verbose) G4Exception("G4PVPlacement::CheckOverlaps()", "InvalidSetup",
                    JustWarning, "Overlap with mother volume !");
        return true;
      }
    }

    // Checking overlaps with each 'sister' volume
    //
    for (G4int i=0; i<motherLog->GetNoDaughters(); i++)
    {
      G4VPhysicalVolume* daughter = motherLog->GetDaughter(i);

      if (daughter == plVol) { continue; }

      // Create the transformation for daughter volume and transform point
      //
      G4AffineTransform Td( daughter->GetRotation(),
                            daughter->GetTranslation() );
      G4ThreeVector md = Td.Inverse().TransformPoint(mp);

      G4VSolid* daughterSolid = daughter->GetLogicalVolume()->GetSolid();
      if (daughterSolid->Inside(md)==kInside)
      {
        G4double distout = daughterSolid->DistanceToOut(md);
        if (distout > tol)
        {
         if (verbose)  G4cout << G4endl;
         if (verbose)  G4cout << "WARNING - G4PVPlacement::CheckOverlaps()" << G4endl
                 << "          Overlap is detected for volume "
                 << plVol->GetName() << G4endl
                 << "          with " << daughter->GetName() << " volume's"
                 << G4endl
                 << "          local point " << md << ", "
                 << "overlapping by at least: " << G4BestUnit(distout,"Length")
                 << G4endl;
         if (verbose)  G4Exception("G4PVPlacement::CheckOverlaps()", "InvalidSetup",
                      JustWarning, "Overlap with volume already placed !");
          return true;
        }
      }

      // Now checking that 'sister' volume is not totally included and
      // overlapping. Do it only once, for the first point generated
      //
      if (n==0)
      {
        // Generate a single point on the surface of the 'sister' volume
        // and verify that the point is NOT inside the current volume

        G4ThreeVector dPoint = daughterSolid->GetPointOnSurface();

        // Transform the generated point to the mother's coordinate system
        // and finally to current volume's coordinate system
        //
        G4ThreeVector mp2 = Td.TransformPoint(dPoint);
        G4ThreeVector ms = Tm.Inverse().TransformPoint(mp2);

        if (solid->Inside(ms)==kInside)
        {
          if (verbose)  G4cout << G4endl;
          if (verbose)  G4cout << "WARNING - G4PVPlacement::CheckOverlaps()" << G4endl
                  << "          Overlap is detected for volume "
                  << plVol->GetName() << G4endl
                  << "          apparently fully encapsulating volume "
                  << daughter->GetName() << G4endl
                  << "          at the same level !" << G4endl;
          if (verbose)  G4Exception("G4PVPlacement::CheckOverlaps()", "InvalidSetup",
                       JustWarning, "Overlap with volume already placed !");
          return true;
        }
      }
    }
  }

  if (verbose)
  {
    G4cout << "OK! " << G4endl;
  }

  return false;



}


