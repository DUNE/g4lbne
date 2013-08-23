//#include <fstream>

#include "LBNEDetectorConstruction.hh"

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
#include "G4ExceptionSeverity.hh"

#include "G4VisExtent.hh"
#include "LBNEPlacementMessenger.hh"
#include "LBNESurveyor.hh"

//---------------------------------------------------------------------------// 
// Handles the tricky volume placements 
//---------------------------------------------------------------------------// 
LBNEVolumePlacements *LBNEVolumePlacements::fInstance = 0;

LBNEVolumePlacements *LBNEVolumePlacements::Instance() {
  if (fInstance == 0) fInstance = new LBNEVolumePlacements;
  return fInstance;
}

LBNEVolumePlacements::LBNEVolumePlacements() {

   fCheckVolumeOverLapWC = true;
   const G4double in = 2.54*cm;
   std::cerr << " LBNEVolumePlacements::LBNEVolumePlacements, Start btw, the inch is " << in << std::endl;
 
    // declaration of some constant which should not impact the physics at all, 
    // but are needed to build the G4 geometry. 
   fDecayPipeLength=250.0*m;
   fHorn1Length = 127.550*in; // Drawing 8875 - ME - 363093
   fHorn1UpstreamPlateLength = in*(5); //???????????//
   fHorn1DownstreamPlateLength = in*(5); //???????????//
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
 
  // No fTargetX0, Y0:  These would be surveyed point, part of the alignement 
  // realm, i.e. handled in the Surveyor class.
  fTargetFinHeight = 21.4*mm; // It includes half of the cooling tube, which will be inside the fins
                             // Drawing 7589-01-50-02 
  // The round edges (grinded 
//  fTargetFinWidth = 6.4*mm;
  fTargetFinWidth = 7.4*mm; // 6.4 -> 7.4 Per discussion with Jim and Alberto, July 25 2013. 
  fTargetFinLength = 20*mm; // 
  fTargetFinMaterial = G4String("Target"); // Only one type of target material 
  fTargetLengthOutsideHorn = fTargetSLength - fTargetLengthIntoHorn + fTargetFinLength; 
   // We add the fin that span the Usptream target and downstream target part. 
  
 //  fTargetNumberFins = 47; //  LBNE Doc 6100.. But we have to compute it from the graphite target length 
  // Since fTargetSLengthGraphite can be changed..Use the same algorithm.. 
  this->SegmentTarget(); 
  fTargetZOffsetStart = 32.2*mm; // From Russian drawing 7589-00-00-00 
  fTargetUpstrUpstrMargin = 1.0*cm; // Perhaps one ought to extend it is Upstream vacuum flange thicker. 
  fTargetUpstrDwnstrMargin = 1.0*mm; 
  fTargetUpstrPlateMaterial = std::string("Aluminum");
  fTargetUpstrPlateHoleRadius = (35.0/2.)*mm;//From Russian drawing 7589-00-00-00
  fTargetUpstrPlateOuterRadius = (214./2.)*mm; //From Russian drawing 7589-00-00-00
  fTargetUpstrPlateThick = 18.4*mm; // From Russian drawing 7589-00-00-00
  fTargetCanMaterial = std::string("Aluminum");
  fTargetCanInnerRadius = (155./2.)*mm; // From Russian drawing 7589-00-00-00
  fTargetCanOuterRadius = fTargetCanInnerRadius + 6.6*mm;
  fTargetCanLength = 9.80*in; // read out from my scree from LBNE Doc 6100, page 1
  fTargetDownstrCanFlangeThick = 0.85*in;
  fTargetDownstrCanFlangeMaterial = std::string("Aluminum"); 
  fTargetDownstrCanFlangeInnerRadius = (155.0/2.)*mm;
  fTargetDownstrCanFlangeOuterRadius = 120.0*mm; // Approximate
//  fTargetDownstrCanFlangeThick = (3.0/8.0)*in; ??? mistake See above 
  fTargetFlangeMaterial = std::string("Steel316"); 
  fTargetFlangeInnerRadius =  15.8*mm; // Approximate... +- 0.5 mm 
  fTargetFlangeOuterRadius= (8.24/2.)*in;;
  fTargetFlangeThick = 25.*mm;
  fTargetCTubeMaterial = std::string("Titanium"); 
  fTargetCTubeOuterRadius = (3. + 0.4)*mm ; // Doc db 6100 
  fTargetCTubeInnerRadius = 3.0*mm;
  fTargetCTubeUpstrLength = 9.621*in; // Length from the upstream bend to the fron the steel drawnstream flange  
  fTargetCTubeUpstrOffset = 2.32*in; // from the beam line tp to the center of the cooling tube. 
  fTargetCTubeUpstrRadCurve = 0.965*in;
  fTargetCTubeUpstrNumSegCurve = 12; // 30 degrees angles Should be good enough. 
  fTargetCTubeReturnHOffset = 0.25*in; // guess
  fTargetCTubeReturnLengthDownstr = 0.985*in - fTargetCTubeInnerRadius/2.0 - 0.050*mm;
  // A small correction for the tilt angle 
  fTargetCTubeReturnLengthDownstr -= fTargetCTubeReturnLengthDownstr*
                                          std::sin(fTargetCTubeReturnHOffset/fTargetCTubeReturnLengthDownstr);
  fTargetDistFlangeToTargetStart = 32.2*mm;
  fTargetCTubeReturnLengthUpstr = fTargetDistFlangeToTargetStart + 10.25*in; // from the upstream end of target to 
                                 // Upstr bend return 
  fTargetCTubeReturnLengthUpstrEnd = 6.604*in - fTargetDistFlangeToTargetStart;
  fTargetAlignRingMaterial = std::string("Steel316");
  fTargetAlignRingThick = 11.0*mm; // Accurate to +- 1 mm, from Russian drawing. Err on the excess side.  
  fTargetAlignRingInnerRadius = (18.0/2)*mm;
  fTargetAlignRingOuterRadius = (29.5/2)*mm;
  fTargetAlignRingCutAngle = 0.1735; // Assume atan(3.4 mm/fTargetAlignRingInnerRadius);
  fTargetAlignRingSpacing = 243.6*mm; // Such we can have 5 alignment rings over the entire distance. 
  fMaxNumAlignRings = 5; // assiming a target length of ~ 1 m. long 
  fTargetHeContTubeInnerRadius = 30.0*mm;
  fTargetHeContTubeThickness = 0.4*mm;
  fTargetHeContTubeLengthInHorn = fTargetLengthIntoHorn + fTargetSLengthDownstrEnd;
  fTargetHeContTubeLengthUpstr = 2.0*in + fTargetCanLength  + fTargetDownstrCanFlangeThick + 
                                 fTargetFlangeThick + fTargetDistFlangeToTargetStart;
  fTargetBerylDownstrWindowThick = 1.0*mm; // ???? pure guess 
  fTargetBerylDownstrWindowRadius = fTargetHeContTubeInnerRadius + fTargetHeContTubeThickness;
  fTargetBerylUpstrWindowThick = 1.0*mm; // pure guess 
  fTargetBerylUpstrWindowRadius = 0.8425*in/2.; // ???? Again, guessing... 
  // Section of Horn1 which is the logical volume of the target Upstream Assembly.
  // Drawing NUMI HORN#1, TRANSITION INNER TO OUTER Drawing number 8875. 112-MD-363097
  fTargetHorn1InnerRadsUpstr.resize(5);
  fTargetHorn1InnerRadsDownstr.resize(5);
  fTargetHorn1TransThick.resize(5);
  fTargetHorn1Lengths.resize(5);
  fTargetHorn1ZPositions.resize(5);
  fTargetHorn1InnerRadsUpstr[0] = 1.88*in;
  fTargetHorn1InnerRadsUpstr[1] = 2.28*in;
  fTargetHorn1InnerRadsUpstr[2] = 2.78*in; // It will a G4tubs, not G4Cons
  fTargetHorn1InnerRadsUpstr[3] = 4.49*in;
  fTargetHorn1InnerRadsUpstr[4] = 5.10*in;
  fTargetHorn1InnerRadsDownstr[0] = 1.573*in;
  fTargetHorn1InnerRadsDownstr[1] = 1.88*in;
  fTargetHorn1InnerRadsDownstr[2] = 4.49*in; // It will a G4tubs, not G4Cons This is the outer radius 
  fTargetHorn1InnerRadsDownstr[3] = 5.10*in;
  fTargetHorn1InnerRadsDownstr[4] = 5.53*in;
  fTargetHorn1TransThick[0] = (0.25*in)/std::cos(M_PI*45./180.);
  fTargetHorn1TransThick[4] = (0.25*in)/std::cos(M_PI*45./180.);
  fTargetHorn1TransThick[1] = (0.25*in)/std::cos(M_PI*60./180.);
  fTargetHorn1TransThick[3] = (0.25*in)/std::cos(M_PI*60./180.);
  fTargetHorn1TransThick[2] = fTargetHorn1InnerRadsDownstr[2] -   fTargetHorn1InnerRadsUpstr[2];
  fTargetHorn1Lengths[0] = 0.457*in;
  fTargetHorn1Lengths[4] = fTargetHorn1Lengths[0];
  fTargetHorn1Lengths[1] = 0.355*in;
  fTargetHorn1Lengths[3] = fTargetHorn1Lengths[1];
  fTargetHorn1Lengths[2] = 0.30*in; // Increase thickness  
  fTargetHorn1ZPositions[0] = -fTargetHorn1Lengths[0]/2.  - 2.5*mm ; // With respect to MCZero, or the dowstream end of Upstream Target Assembly
                                                          // push them bacward a bit to avoid overlaps 
  fTargetHorn1ZPositions[4] = fTargetHorn1ZPositions[0];
  fTargetHorn1ZPositions[1] = -1.0*(fTargetHorn1Lengths[0]  + fTargetHorn1Lengths[1]/2.) - 2.5*mm;
  fTargetHorn1ZPositions[3] = fTargetHorn1ZPositions[1];
  fTargetHorn1ZPositions[2] = -1.0*(fTargetHorn1Lengths[0]  + fTargetHorn1Lengths[1] + fTargetHorn1Lengths[2]/2.) - 5.0*mm; // 
   //
   // Basic algorithm to determine position of the target main daughter volumes 
   //
   // Details about the baffle 
   fBaffleInnerRadius = 6.5*mm; // Per discussion with Jim Hylen, July 22 2013 
   fBaffleOuterRadius = 28.3*mm; // from previous version and le010z185i_NumiDPHelium.input
   fBaffleLength = 1474.8*mm; // Drawing 8875.000-ME-363028
   fTargetAndBaffleLengthApprox = fBaffleLength + fTargetUpstrUpstrMargin  + fTargetUpstrPlateThick + fTargetCanLength 
	                   + fTargetDownstrCanFlangeThick + fTargetFlangeThick + 0.2*mm + 44.18*in + 
			   fTargetUpstrDwnstrMargin  +  fTargetDistFlangeToTargetStart + fTargetLengthOutsideHorn + 2.0*mm;
			    // NUMI Drawing 8875.000-ME-363028 with 2 mm margin of error. 
  fBaffleZPosition = -1.0e19; // to be determined one the Target Usptream segment is installed 
  fTargetZ0Upstr = (-1.0* (fTargetSLength - fTargetLengthIntoHorn))/2. ;
        // offset with respecto MCZERO of the center of target mother volume,  
  fTargetZ0Downstr = fTargetLengthIntoHorn/2. ;
  
  //
  
  
  fWaterLayerThickInHorns = 0.; 
  
//   std::cerr << " LBNEVolumePlacements::LBNEVolumePlacements, O.K. done " << std::endl;
  
}

LBNEVolumePlacements::~LBNEVolumePlacements() {
  delete fPlacementMessenger;
}
void LBNEVolumePlacements::InitializeMessenger() {
//   std::cerr << " LBNEVolumePlacements::Initialize, about to instantiate PlacementMessengers " << std::endl;
//   fPlacementMessenger = new LBNEPlacementMessenger();
// Why twice ?? 
}
void LBNEVolumePlacements::Initialize(const G4LogicalVolume* matriarch) {

   fTopLogicalVolume = matriarch;
   
}

LBNEVolumePlacementData* 
   LBNEVolumePlacements::Create(const G4String &name) {  

  // Check that it is not already defined. 

  std::map<G4String, LBNEVolumePlacementData>::const_iterator itDupl = fSubVolumes.find(name);
  if (itDupl != fSubVolumes.end()) {
      std::ostringstream mStrStr;
      mStrStr << " Volume named " << name << " Already defined. Fatal ";
      G4String mStr(mStrStr.str());
      G4Exception("LBNEVolumePlacements::Create", " ", FatalErrorInArgument, mStr.c_str()); 
    }

   const G4double in = 2.54*cm;
  // look for a pre-existing placement.  If there, fatal error, exit 
  LBNEVolumePlacementData info;
  info.fAlignmentModel = eNominal;
  info.fIsPlaced = false;
  info.fRotation = G4RotationMatrix(); // a bit of a waste of space, as, in most cases, not rotations are applied. 
  info.fRotationIsUnitMatrix= true;
  info.fPosition = G4ThreeVector(0., 0., 0.);
  info.fParams.resize(3, 0.);
  info.fMother = 0;
  info.fTypeName = G4String("Tubs"); // most common type of volume.. 
  std::string volumeName(name); // volumeName += std::string("_solid"); Not necessary 
  if (name == G4String("Tunnel")) {
    info.fParams[0] = 8*m - 2.0*cm;  // Note: the total volume is 60 m. wide => plenty enough rocks. 
    info.fParams[1] = 12*m - 2.0*cm; 
    info.fParams[2] = fTotalLength -2.0*cm;
    G4Box* hallBox = new G4Box(volumeName, info.fParams[0]/2., info.fParams[1]/2., info.fParams[2]/2. ); 
    info.fCurrent = new G4LogicalVolume(hallBox, G4Material::GetMaterial("Air"), name); 
    info.fTypeName = G4String("Box");
  }
  if (name == G4String("TargetHallAndHorn1")) {
    std::map<G4String, LBNEVolumePlacementData>::const_iterator it = fSubVolumes.find(G4String("Tunnel"));
    if (it == fSubVolumes.end()) {
     std::ostringstream mStrStr;
       mStrStr << " Internal error for " << name << " fatal " << std::endl;
      G4String mStr(mStrStr.str());
      G4Exception("LBNEVolumePlacements::Create", " ", FatalErrorInArgument, mStr.c_str()); 
    }
    for (size_t k=0; k != 2; ++k) 
      info.fParams[k] = it->second.fParams[k] - 2.0*cm; 
    info.fParams[2] = fHorn1UpstreamPlateLength + fHorn1DownstreamPlateLength 
                      + fHorn1Length + 1.0*cm + fTargetAndBaffleLengthApprox  ; // 1 cm longitudinally for spare?  
    G4Box* hallBox = new G4Box(volumeName, info.fParams[0]/2., info.fParams[1]/2., info.fParams[2]/2. );
    info.fCurrent = new G4LogicalVolume(hallBox, G4Material::GetMaterial("Air"), volumeName); 
    info.fPosition[2] = 0.; // Definiing MCZERO, Drawing 8875. 112-MD-363097, annotated by jim Hylen
    info.fTypeName = G4String("Box");
  
  } else if (name == G4String("UpstreamTargetAssembly")) {
    std::map<G4String, LBNEVolumePlacementData>::const_iterator it = 
        fSubVolumes.find(G4String("TargetHallAndHorn1"));
    if (it == fSubVolumes.end()) {
     std::ostringstream mStrStr;
       mStrStr << " Internal error for " << name << " Volume not found " << std::endl;
      G4String mStr(mStrStr.str());
      G4Exception("LBNEVolumePlacements::Create", " ", FatalErrorInArgument, mStr.c_str()); 
    }  
    for (size_t k=0; k != 2; ++k) 
      info.fParams[k] = it->second.fParams[k] - 0.5*cm; 
    info.fParams[2] = fTargetAndBaffleLengthApprox;
    G4Box* hallBox = new G4Box(volumeName, info.fParams[0]/2., info.fParams[1]/2., info.fParams[2]/2. );
    info.fCurrent = new G4LogicalVolume(hallBox, G4Material::GetMaterial("Air"), volumeName); 
    info.fPosition[2] =-it->second.fParams[2]/2. + fTargetAndBaffleLengthApprox/2.; 
    info.fTypeName = G4String("Box");
//    std::cerr << " LBNEVolumePlacements::Create " << name << " half length " << info.fParams[2]/2. << std::endl;
      
  } else if (name == G4String("Baffle")) {
  // more cases here... a long list.. a too long of a  list? 
  // We now go one level down in the hierachy. Start from upstream 
    std::map<G4String, LBNEVolumePlacementData>::const_iterator it = 
        fSubVolumes.find(G4String("UpstreamTargetAssembly"));
    if (it == fSubVolumes.end()) {
     std::ostringstream mStrStr;
       mStrStr << " Internal error for " << name 
               << " Volume UpstreamTargetAssembly not found " << std::endl;
      G4String mStr(mStrStr.str());
      G4Exception("LBNEVolumePlacements::Create", " ", FatalErrorInArgument, mStr.c_str()); 
    } 
    info.fParams[0] = fBaffleInnerRadius; 
    info.fParams[1] = fBaffleOuterRadius; 
    info.fParams[2] = fBaffleLength;
    G4Tubs* baffleTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
    info.fCurrent = new G4LogicalVolume(baffleTube, G4Material::GetMaterial("GraphiteBaffle"), volumeName); 
    info.fPosition[2] = -it->second.fParams[2]/2. + fBaffleLength/2.0 + 0.5*mm; 
  } 
  //
  // Target stuff.. Lots of it
  // 
  if (name.find("Target") != std::string::npos) {
    if (name.find("TargetUpstr") != std::string::npos) {
      std::map<G4String, LBNEVolumePlacementData>::const_iterator it = 
        fSubVolumes.find(G4String("UpstreamTargetAssembly"));
      if (it == fSubVolumes.end()) {
     std::ostringstream mStrStr;
       mStrStr << " Internal error for " << name << " Mother Volume Target not found " << std::endl;
      G4String mStr(mStrStr.str());
      G4Exception("LBNEVolumePlacements::Create", " ", FatalErrorInArgument, mStr.c_str()); 
      } 
      if (name == G4String("TargetUpstrMTop")) { 
         // Complete container the target Canister and the portion of the target outside the horn 
	 // Subject to misalignment!!!
         info.fParams[1] =  10.*in; // mother volume, must hold the Inner/Outer Horn1 transition. 
         info.fParams[2] = fTargetUpstrUpstrMargin  + fTargetUpstrPlateThick + fTargetCanLength 
	                   + fTargetDownstrCanFlangeThick + fTargetFlangeThick + 0.2*mm +
			   fTargetUpstrDwnstrMargin  +  fTargetDistFlangeToTargetStart + fTargetLengthOutsideHorn;
         G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
         info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial("Air"), volumeName); 
         info.fPosition[2] = it->second.fParams[2]/2. -info.fParams[2]/2. - 0.01*mm; // Nominal: 
	           // Half length of UpstreamTargetAssembly - length of this  
       } 
      if (name == G4String("TargetUpstrM0")) { // Complete container for the target Canister and flange  
         info.fParams[1] = fTargetDownstrCanFlangeOuterRadius + 1.0*cm; 
         info.fParams[2] = fTargetUpstrUpstrMargin  + fTargetUpstrPlateThick + fTargetCanLength 
	                   + fTargetDownstrCanFlangeThick + fTargetFlangeThick;
         G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
         info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial("HeliumTarget"), volumeName); 
         std::map<G4String, LBNEVolumePlacementData>::const_iterator itMTop = fSubVolumes.find(G4String("TargetUpstrMTop"));
         if (itMTop == fSubVolumes.end()) {
           std::ostringstream mStrStr;
           mStrStr << " Internal error for " << name << " Mother Volume TargetUpstrM0 not found " << std::endl;
           G4String mStr(mStrStr.str());
           G4Exception("LBNEVolumePlacements::Create", " ", FatalErrorInArgument, mStr.c_str()); 
         } 
         info.fPosition[2] = -1.0*itMTop->second.fParams[2]/2. + info.fParams[2]/2. + 0.01*mm; // 
       } 
       if (name.find("TargetUpstrUpstr") != std::string::npos) {
          std::map<G4String, LBNEVolumePlacementData>::const_iterator it = 
          fSubVolumes.find(G4String("TargetUpstrM0"));
          if (it == fSubVolumes.end()) {
           std::ostringstream mStrStr;
           mStrStr << " Internal error for " << name << " Mother Volume TargetUpstrUpstr not found " << std::endl;
           G4String mStr(mStrStr.str());
           G4Exception("LBNEVolumePlacements::Create", " ", FatalErrorInArgument, mStr.c_str()); 
          } 
          if (name == G4String("TargetUpstrUpstrPlate")) { // 
            info.fParams[0] = fTargetUpstrPlateHoleRadius; 
            info.fParams[1] = fTargetUpstrPlateOuterRadius; 
            info.fParams[2] = fTargetUpstrPlateThick ;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial("Aluminum"), volumeName); 
            info.fPosition[2] = -1.0*it->second.fParams[2]/2.0 + fTargetUpstrUpstrMargin + fTargetUpstrPlateThick/2.;
         } 
           if (name == G4String("TargetUpstrUpstrCan")) { // 
            info.fParams[0] = fTargetCanInnerRadius; 
            info.fParams[1] = fTargetCanOuterRadius; 
            info.fParams[2] = fTargetCanLength ;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(fTargetCanMaterial), volumeName); 
            info.fPosition[2] = -1.0*it->second.fParams[2]/2.0 + fTargetUpstrUpstrMargin +
	                        fTargetUpstrPlateThick + fTargetCanLength/2.0;
         } 
           if (name == G4String("TargetUpstrUpstrCanEndPlate")) { // 
            info.fParams[0] = fTargetCanInnerRadius; 
            info.fParams[1] = fTargetCanOuterRadius + 0.5*in; // ?? Guess  for the 1/2 inch extra. not critical 
            info.fParams[2] = fTargetDownstrCanFlangeThick;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(fTargetCanMaterial), volumeName); 
            info.fPosition[2] = -1.0*it->second.fParams[2]/2.0 + fTargetUpstrUpstrMargin +
	                        fTargetUpstrPlateThick + fTargetCanLength + fTargetDownstrCanFlangeThick/2.;
//      ????? info.fParams[2] = fTargetUpstrUpstrMargin  + fTargetUpstrPlateThick + fTargetCanLength // For M0
//	                   + fTargetDownstrCanFlangeThick + fTargetFlangeThick;
         } 
           if (name == G4String("TargetUpstrUpstrDwstrFlange")) { // 
            info.fParams[0] = fTargetFlangeInnerRadius; 
            info.fParams[1] = fTargetFlangeOuterRadius; 
            info.fParams[2] = fTargetFlangeThick ;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(fTargetFlangeMaterial), volumeName); 
            info.fPosition[2] = -1.0*it->second.fParams[2]/2.0 + fTargetUpstrUpstrMargin +
	                         fTargetUpstrPlateThick + fTargetCanLength + 
				 fTargetDownstrCanFlangeThick + fTargetFlangeThick/2.;
         }
           if (name == G4String("TargetUpstrUpstrCoolingTube")) { // 
            info.fParams[0] = 0.; // Solid tube of copper, we'll define water below 
            info.fParams[1] = fTargetCTubeOuterRadius; 
            info.fParams[2] = fTargetCTubeUpstrLength ;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(fTargetCTubeMaterial), volumeName); 
	    // Leave the position blanks, as there will two copies (top and bottom.) 
	 }
           if (name == G4String("TargetUpstrUpstrCoolingTubeWater")) { // 
            info.fParams[0] = 0.; 
            info.fParams[1] = fTargetCTubeInnerRadius; 
            info.fParams[2] = fTargetCTubeUpstrLength ;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("Water")), volumeName); 
	    // Leave the position blanks, as there will two copies (top and bottom.) 
	 }
           if (name == G4String("TargetUpstrUpstrSupportBlockTopLeft")) { // 
            info.fParams[0] = 15.0*mm; // A block of presumed steel to support cooling tubes. +- 2 mm or so. 
            info.fParams[1] = 10.0*mm; 
            info.fParams[2] = 10.0*mm;
            G4Box* aBox = new G4Box(volumeName, info.fParams[0]/2., info.fParams[1]/2, info.fParams[2]/2.);
            info.fCurrent = new G4LogicalVolume(aBox, G4Material::GetMaterial(std::string("Steel316")), volumeName); 
	    info.fPosition[0] = -1.0;// Drawing 7589-00-00-00  Approximate. +- 1 mm 
	    info.fPosition[1] = 22.5;// Drawing 7589-00-00-00
	    info.fPosition[2] = -1.0*it->second.fParams[2]/2.0 + fTargetUpstrUpstrMargin +
	                         fTargetUpstrPlateThick + 140.*mm;// Drawing 7589-00-00-00
            info.fTypeName = G4String("Box");
	 }
           if (name == G4String("TargetUpstrUpstrSupportBlockBottomLeft")) { // 
            info.fParams[0] = 15.0*mm; // A block of presumed steel to support cooling tubes. +- 2 mm or so.  
            info.fParams[1] = 10.0*mm; 
            info.fParams[2] = 11.0*mm;
            G4Box* aBox = new G4Box(volumeName, info.fParams[0]/2., info.fParams[1]/2, info.fParams[2]/2.);
            info.fCurrent = new G4LogicalVolume(aBox, G4Material::GetMaterial(std::string("Steel316")), volumeName); 
	    info.fPosition[0] = -1.0*mm;// Drawing 7589-00-00-00
	    info.fPosition[1] = -20.0;// Drawing 7589-00-00-00
	    info.fPosition[2] = -1.0*it->second.fParams[2]/2.0 + fTargetUpstrUpstrMargin +
	                         fTargetUpstrPlateThick + 140*mm;// Drawing 7589-00-00-00
            info.fTypeName = G4String("Box");
	 }
           if (name == G4String("TargetUpstrUpstrSupportBlockRight")) { // 
            info.fParams[0] = 10.0*mm; // A block of presumed steel to support cooling tubes. +- 2 mm or so.  
            info.fParams[1] = 15.0*mm; 
            info.fParams[2] = 11.0*mm;
            G4Box* aBox = new G4Box(volumeName, info.fParams[0]/2., info.fParams[1]/2, info.fParams[2]/2.);
            info.fCurrent = new G4LogicalVolume(aBox, G4Material::GetMaterial(std::string("Steel316")), volumeName); 
	    info.fPosition[0] = 14.0;// Drawing 7589-00-00-00
	    info.fPosition[1] = 7.5;// Drawing 7589-00-00-00
	    info.fPosition[2] = -1.0*it->second.fParams[2]/2.0 + fTargetUpstrUpstrMargin +
	                         fTargetUpstrPlateThick + 140*mm;// Drawing 7589-00-00-00
            info.fTypeName = G4String("Box");
	 }
            if (name == G4String("TargetUpstrUpstrSupportRod")) { // 
            info.fParams[0] = 0.; // 
            info.fParams[1] = 12.*mm; // A bit oversize, the upstream part is thicker.. 
            info.fParams[2] = 163.0*mm; // Approx to ~ 3 mm. 
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.0*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("Steel316")), volumeName); 
            // See special implementation due to multiple copies.  These rods will not be "surveyable"
	    
	 }
            if (name == G4String("TargetUpstrUpstrSupportSleeve")) { // 
            info.fParams[0] = fTargetFlangeInnerRadius - 0.2*mm; // 
            info.fParams[1] = info.fParams[0] + 3.0*mm; //  Approx to 500 microns
            info.fParams[2] = 72.0*mm;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.0*deg );
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("Steel316")), volumeName); 
	    info.fPosition[2] =-1.0*it->second.fParams[2]/2.0 + fTargetUpstrUpstrMargin +
	                         fTargetUpstrPlateThick + fTargetCanLength 
				 + fTargetDownstrCanFlangeThick - info.fParams[2]/2. -0.1*mm;
	 }
          if (name == G4String("TargetUpstrUstrHorFin")) { // 
            info.fParams[0] = fTargetFinHeight; // X-Y inverted 
            info.fParams[1] = fTargetFinWidth; 
            info.fParams[2] = fTargetFinLength;
            G4Box* aBox = new G4Box(volumeName, info.fParams[0]/2., info.fParams[1]/2, info.fParams[2]/2.);
            info.fCurrent = new G4LogicalVolume(aBox, G4Material::GetMaterial(std::string("Target")), volumeName); 
	    info.fPosition[2] = -1.0*it->second.fParams[2]/2.0 + fTargetUpstrUpstrMargin +
	                         fTargetUpstrPlateThick + 140*mm;// Drawing 7589-00-00-00
            info.fTypeName = G4String("Box");
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
            info.fParams[0] = 0.; 
            info.fParams[1] = fTargetCTubeOuterRadius; 
            info.fParams[2] = fTargetFlangeThick;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(fTargetCTubeMaterial), volumeName); 
	    // Leave the position blanks, as there will be two copies (top and bottom.) 
	 }
           if (name == G4String("TargetUpstrUpstrCoolingTubeFlangeSectionWater")) { // 
            info.fParams[0] = 0.; 
            info.fParams[1] = fTargetCTubeInnerRadius; 
            info.fParams[2] = fTargetFlangeThick;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("Water")), volumeName); 
	    // at (0.,0., 0.) 
	 }
                 
       } // UpstreamTargetAssembly as mother volume. 
       
       if (name == G4String("TargetUpstrM1")) { // Complete container volume in the upstream target hall 
                                                // to contain whatever if dowstream of the above flange 
						// and upstream of the Horn. 
	 // The out radius is set by the components of the Horn1 that infringe upstream of MCZERO.
	 // That is, the Inner to Outer transition piece 
	 double maxRadIOHorn1 = -1.0;
	 for (size_t iR = 0; iR != fTargetHorn1InnerRadsDownstr.size(); iR++) 
	     if (fTargetHorn1InnerRadsDownstr[iR] > maxRadIOHorn1) maxRadIOHorn1 = fTargetHorn1InnerRadsDownstr[iR];
	 for (size_t iR = 0; iR != fTargetHorn1InnerRadsUpstr.size(); iR++) 
	     if (fTargetHorn1InnerRadsUpstr[iR] > maxRadIOHorn1) maxRadIOHorn1 = fTargetHorn1InnerRadsUpstr[iR];
	 maxRadIOHorn1 += 2.0*cm;    
         info.fParams[1] = std::max(maxRadIOHorn1, (fTargetHeContTubeInnerRadius + fTargetHeContTubeThickness+ 1.0*cm));
	        // mother volume, safety factor of 1. 0 cm  
         info.fParams[2] = fTargetUpstrDwnstrMargin  +  fTargetDistFlangeToTargetStart + fTargetLengthOutsideHorn - fTargetFinLength; 
         G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
         info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("Air")), volumeName); 
         std::map<G4String, LBNEVolumePlacementData>::const_iterator itMTop = fSubVolumes.find(G4String("TargetUpstrMTop"));
         if (it == fSubVolumes.end()) {
          std::ostringstream mStrStr;
          mStrStr << " Internal error for " << name << " Volume TargetUpstrMTop not found " << std::endl;
          G4String mStr(mStrStr.str());
          G4Exception("LBNEVolumePlacements::Create", " ", FatalErrorInArgument, mStr.c_str()); 
         } 
         info.fPosition[2] = itMTop->second.fParams[2]/2. - info.fParams[2]/2. - 0.005*mm; // in TargetUpstrMTop 
       } 
       if (name.find("TargetUpstrDownstr") != std::string::npos) {
          std::map<G4String, LBNEVolumePlacementData>::const_iterator it = 
          fSubVolumes.find(G4String("TargetUpstrM1"));
          if (it == fSubVolumes.end()) {
            std::ostringstream mStrStr;
            mStrStr << " Internal error for " << name << " Volume TargetUpstrM1 not found " << std::endl;
            G4String mStr(mStrStr.str());
            G4Exception("LBNEVolumePlacements::Create", " ", FatalErrorInArgument, mStr.c_str()); 
           } 
          if (name == G4String("TargetUpstrDownstrHeContainer")) { // 
            info.fParams[0] = 0.; 
            info.fParams[1] = fTargetHeContTubeInnerRadius + fTargetHeContTubeThickness; 
            info.fParams[2] = it->second.fParams[2]- .005*mm;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("Beryllium")), volumeName); 
            info.fPosition[2] = 0.; // possibly tweak for the margin... 
         }
          if (name == G4String("TargetUpstrDownstrHelium")) { // 
            std::map<G4String, LBNEVolumePlacementData>::const_iterator it = 
            fSubVolumes.find(G4String("TargetUpstrDownstrHeContainer"));
             if (it == fSubVolumes.end()) {
              std::ostringstream mStrStr;
              mStrStr << " Internal error for " << name << " Volume TargetUpstrDownstrHeContainer not found " << std::endl;
              G4String mStr(mStrStr.str());
              G4Exception("LBNEVolumePlacements::Create", " ", FatalErrorInArgument, mStr.c_str()); 
            } 
            info.fParams[0] = 0.; 
            info.fParams[1] = fTargetHeContTubeInnerRadius; 
            info.fParams[2] = it->second.fParams[2]- .005*mm;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("HeliumTarget")), volumeName); 
            info.fPosition[2] = 0.; // possibly tweak for the margin... 
         }
          if (name == G4String("TargetUpstrDownstrCoolingTube")) { // 
            info.fParams[0] = 0.; 
            info.fParams[1] = fTargetCTubeOuterRadius; 
            info.fParams[2] = fTargetFinLength+ 0.9*fTargetFinSpacingLength ;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("Titanium")), volumeName); 
            info.fPosition[2] = fTargetFinHeight + 0.005*mm; // possibly tweak for the margin... 
         }
          if (name == G4String("TargetUpstrDownstrCoolingTubeWater")) { // 
            info.fParams[0] = 0.; 
            info.fParams[1] = fTargetCTubeInnerRadius; 
            info.fParams[2] = fTargetFinLength+ 0.9*fTargetFinSpacingLength ;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("Water")), volumeName); 
         }
          if (name == G4String("TargetUpstrDownstrSegment")) { // 
            info.fParams[0] = fTargetFinWidth + 0.2*mm; 
            info.fParams[1] = fTargetFinHeight + 2.0*fTargetCTubeOuterRadius + 0.2*mm; 
            info.fParams[2] = fTargetFinLength+ 0.95*fTargetFinSpacingLength ;
            G4Box* aBox = new G4Box(volumeName, info.fParams[0]/2., info.fParams[1]/2., info.fParams[2]/2.);
            info.fCurrent = new G4LogicalVolume(aBox, G4Material::GetMaterial(std::string("HeliumTarget")), volumeName); 
            info.fTypeName = G4String("Box");
         }
          if (name == G4String("TargetUpstrDownstrCoolingTubeLast")) { // 
            info.fParams[0] = 0.; 
            info.fParams[1] = fTargetCTubeOuterRadius; 
            info.fParams[2] = fTargetFinLengthSplitUpstr - 0.025*mm;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("Titanium")), volumeName); 
            info.fPosition[2] = fTargetFinHeight + 0.005*mm; // possibly tweak for the margin... 
         }
          if (name == G4String("TargetUpstrDownstrCoolingTubeLastWater")) { // 
            info.fParams[0] = 0.; 
            info.fParams[1] = fTargetCTubeInnerRadius; 
            info.fParams[2] = fTargetFinLengthSplitUpstr - 0.03*mm;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("Water")), volumeName); 
         }
          if (name == G4String("TargetUpstrDownstrSegmentLast")) { // 
            info.fParams[0] = fTargetFinWidth + 0.2*mm; 
            info.fParams[1] = fTargetFinHeight + 2.0*fTargetCTubeOuterRadius + 0.2*mm; 
            info.fParams[2] = fTargetFinLengthSplitUpstr - 0.01*mm;
            G4Box* aBox = new G4Box(volumeName, info.fParams[0]/2., info.fParams[1]/2., info.fParams[2]/2.);
            info.fCurrent = new G4LogicalVolume(aBox, G4Material::GetMaterial(std::string("HeliumTarget")), volumeName); 
             info.fTypeName = G4String("Box");
        }
	 // 
       } // Downstream of the flange of the target canister. 
    } // Upstream Stuff 
    //
    // Bits and pieces for the target itself. 
    //
    if (name == G4String("TargetFinVert")) { // 
            info.fParams[0] = fTargetFinWidth; 
            info.fParams[1] = fTargetFinHeight - 2.0*fTargetCTubeOuterRadius - 0.1*mm; 
            info.fParams[2] = fTargetFinLength ;
            G4Box* aBox = new G4Box(volumeName, info.fParams[0]/2, info.fParams[1]/2, info.fParams[2]/2.);
            info.fCurrent = new G4LogicalVolume(aBox, G4Material::GetMaterial(std::string("Target")), volumeName); 
            info.fTypeName = G4String("Box");
    }
    if (name == G4String("TargetFinHorizontal")) { // only one of those, fixed size  
            info.fParams[0] = 62.0*mm; 
            info.fParams[1] = fTargetFinWidth; 
            info.fParams[2] = fTargetFinLength ;
            G4Box* aBox = new G4Box(volumeName, info.fParams[0]/2, info.fParams[1]/2, info.fParams[2]/2.);
            info.fCurrent = new G4LogicalVolume(aBox, G4Material::GetMaterial(std::string("Target")), volumeName); 
	    info.fPosition[0] = -info.fParams[0]/2. + 10.0*mm; // Russian drawing 7589-00-00 
            std::map<G4String, LBNEVolumePlacementData>::const_iterator it = 
                                    fSubVolumes.find(G4String("TargetUpstrM0"));
            if (it == fSubVolumes.end()) {
              std::ostringstream mStrStr;
              mStrStr << " Internal error for " << name << " Volume TargetUpstrM0 not found " << std::endl;
              G4String mStr(mStrStr.str());
              G4Exception("LBNEVolumePlacements::Create", " ", FatalErrorInArgument, mStr.c_str()); 
	    }
	    info.fPosition[0] = -info.fParams[0]/2.0 + 10.*mm;
	    info.fPosition[1] = 0.;
	    info.fPosition[2] = -1.0*it->second.fParams[2]/2.0 + fTargetUpstrUpstrMargin +
	                         fTargetUpstrPlateThick + fTargetCanLength + fTargetDownstrCanFlangeThick
				 - 100.0*mm; // Russian drawing 7589-00-00 
            info.fTypeName = G4String("Box");
      } 
    if (name == G4String("TargetFinVertUpstrLast")) { // 
            info.fParams[0] = fTargetFinWidth; 
            info.fParams[1] = fTargetFinHeight - 2.0*fTargetCTubeOuterRadius - 0.1*mm; 
            info.fParams[2] = fTargetFinLengthSplitUpstr  - 0.02*mm; // 20 micron safety. 
            G4Box* aBox = new G4Box(volumeName, info.fParams[0]/2, info.fParams[1]/2, info.fParams[2]/2.);
            info.fCurrent = new G4LogicalVolume(aBox, G4Material::GetMaterial(std::string("Target")), volumeName);
            info.fTypeName = G4String("Box");
 // despite being a single copy, do the placement in a separate method. 	    
    }
    
    if (name == G4String("TargetAlignmentRingLeft")) { // 
	    info.fParams.resize(5);
            info.fParams[0] = fTargetAlignRingInnerRadius;
            info.fParams[1] = fTargetAlignRingOuterRadius; 
            info.fParams[2] = fTargetAlignRingThick;
	    info.fParams[3] = M_PI/2. + fTargetAlignRingCutAngle ;
	    info.fParams[4] = M_PI - fTargetAlignRingCutAngle ;
            G4Tubs* aTubs = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2.,
	                           info.fParams[3],   info.fParams[4]  );
            info.fCurrent = new G4LogicalVolume(aTubs, G4Material::GetMaterial(std::string("Steel316")), volumeName);
 // Multiple copies.. 
 	    
    }
    if (name == G4String("TargetAlignmentRingRight")) { // 
	    info.fParams.resize(5);
            info.fParams[0] = fTargetAlignRingInnerRadius;
            info.fParams[1] = fTargetAlignRingOuterRadius; 
            info.fParams[2] = fTargetAlignRingThick;
	    info.fParams[3] = M_PI + fTargetAlignRingCutAngle;
	    info.fParams[4] = 2.0*M_PI -  fTargetAlignRingCutAngle;
            G4Tubs* aTubs = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2.,
	                           info.fParams[3],   info.fParams[4]  );
            info.fCurrent = new G4LogicalVolume(aTubs, G4Material::GetMaterial(std::string("Steel316")), volumeName);
 // Multiple copies.. 
 	    
    } 
  // Things that are physically part of Horn1, but placed in at the dowstream end of the target volume 
  // These volumes are surrounding the 
   if (name == G4String("TargetHorn1TransInnerOuterCont")) { // Approximate!... 
            info.fParams[0] = fTargetHorn1InnerRadsDownstr[0] - 0.2*mm;
            info.fParams[1] = fTargetHorn1InnerRadsDownstr[4] + fTargetHorn1TransThick[4] + 1.*mm; 
            info.fParams[2] = -1.0*fTargetHorn1ZPositions[2] + 0.3*in + 5.0*mm; // few mm safety... 
            G4Tubs* aTubs = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2.,
	                           0., 360.0*deg);
            info.fCurrent = new G4LogicalVolume(aTubs, G4Material::GetMaterial(std::string("Air")), volumeName);
	    // Pick volume Upstream TargetAssembly. 
            std::map<G4String, LBNEVolumePlacementData>::const_iterator it = 
                                    fSubVolumes.find(G4String("TargetUpstrM1"));
            if (it == fSubVolumes.end()) {
              std::ostringstream mStrStr;
              mStrStr << " Internal error for " << name << " Volume TargetUpstrM1 not found " << std::endl;
              G4String mStr(mStrStr.str());
              G4Exception("LBNEVolumePlacements::Create", " ", FatalErrorInArgument, mStr.c_str()); 
	    }
	    info.fPosition[2] = 1.0*it->second.fParams[2]/2.0  - info.fParams[2]/2 - 0.25*mm; // Appro to a fraction of mm. 
    }
    if (name.find("TargetHorn1TransInnerOuterPart") != std::string::npos) { // Still even more approximate!... 
            size_t nameIndex= name.find("TargetHorn1TransInnerOuterPart") + name.length() - 1;
	    std::string nameIndexStr(name.substr(nameIndex));
	    std::istringstream nameIndexStrStr(nameIndexStr); 
	    int iPart = -1;
	    nameIndexStrStr >> iPart;
	    std::cerr << " from LBNEVolumePlacements::Create, for " << name << " Part number " << iPart << std::endl;
	    if (iPart != 2) {
	      info.fParams.resize(5); 
              info.fParams[0] = fTargetHorn1InnerRadsUpstr[iPart];
              info.fParams[2] = fTargetHorn1InnerRadsDownstr[iPart]; 
              info.fParams[1] = fTargetHorn1InnerRadsUpstr[iPart] + fTargetHorn1TransThick[iPart];
              info.fParams[3] = fTargetHorn1InnerRadsDownstr[iPart] + fTargetHorn1TransThick[iPart];
	      info.fParams[4] = fTargetHorn1Lengths[iPart];
              G4Cons* aCons = new G4Cons(volumeName, info.fParams[0], info.fParams[1], info.fParams[2], info.fParams[3],
	                              info.fParams[4]/2., 0., 360.0*deg);
              info.fCurrent = new G4LogicalVolume(aCons, G4Material::GetMaterial(std::string("Aluminum")), volumeName);
              info.fTypeName = G4String("Cons");
	    } else { 
              info.fParams[0] = fTargetHorn1InnerRadsUpstr[iPart];
              info.fParams[1] = fTargetHorn1InnerRadsUpstr[iPart] + fTargetHorn1TransThick[iPart];
	      info.fParams[2] = fTargetHorn1Lengths[iPart];
              G4Tubs* aTubs = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2.,
	                           0., 360.0*deg);
              info.fCurrent = new G4LogicalVolume(aTubs, G4Material::GetMaterial(std::string("Air")), volumeName);
	    }
	    // Pick container volume . 
            std::map<G4String, LBNEVolumePlacementData>::const_iterator it = 
                                    fSubVolumes.find(G4String("TargetHorn1TransInnerOuterCont"));
            if (it == fSubVolumes.end()) {
              std::ostringstream mStrStr;
              mStrStr << " Internal error for " << name 
	              << " Volume TargetHorn1TransInnerOuterContTargetUpstrM0 not found " << std::endl;
              G4String mStr(mStrStr.str());
              G4Exception("LBNEVolumePlacements::Create", " ", FatalErrorInArgument, mStr.c_str()); 
	    }
	    info.fPosition[2] = 1.0*it->second.fParams[2]/2.0  + fTargetHorn1ZPositions[iPart]; // Position were given with respect to MCZero
    }
  } // End of Target declarations 
  //
  // Beginning of Horn1 declarations 
  //
  if (name.find("Horn1") == 0) {
    if (name == G4String("Horn1Hall")) {
       std::map<G4String, LBNEVolumePlacementData>::const_iterator itMother = 
         fSubVolumes.find(G4String("TargetHallAndHorn1"));
       if (itMother == fSubVolumes.end()) {
         std::cerr << " LBNEVolumePlacements::Create, internal error for " << name << " fatal " << std::endl;
              std::ostringstream mStrStr;
              mStrStr << " Internal error for " << name << " Volume Horn1Hall not found " << std::endl;
              G4String mStr(mStrStr.str());
              G4Exception("LBNEVolumePlacements::Create", " ", FatalErrorInArgument, mStr.c_str()); 
       }  
       std::map<G4String, LBNEVolumePlacementData>::const_iterator itTargetAssembly = 
         fSubVolumes.find(G4String("UpstreamTargetAssembly"));
       if (itTargetAssembly == fSubVolumes.end()) {
            std::cerr << " LBNEVolumePlacements::Create, internal error for " << name << " fatal " << std::endl;
            std::ostringstream mStrStr;
            mStrStr << " Internal error for " << name << " Volume UpstreamTargetAssembly not found " << std::endl;
            G4String mStr(mStrStr.str());
            G4Exception("LBNEVolumePlacements::Create", " ", FatalErrorInArgument, mStr.c_str()); 
       }  
       for (size_t k=0; k != 2; ++k) 
        info.fParams[k] = itMother->second.fParams[k] - 0.5*cm; 
        info.fParams[2] = fHorn1UpstreamPlateLength + fHorn1Length + fHorn1DownstreamPlateLength;
        G4Box* hallBox = new G4Box(volumeName, info.fParams[0]/2., info.fParams[1]/2., info.fParams[2]/2. );
        info.fCurrent = new G4LogicalVolume(hallBox, G4Material::GetMaterial("Air"), volumeName); 
        info.fPosition[2] = -1.0*itMother->second.fParams[2]/2. + itTargetAssembly->second.fParams[2] + info.fParams[2]/2. + 0.020*mm;
     
    } 
    // Note: to optimize the geometry, we place the downstream end of the target into the 
    // horn1. Target is not a type, nor misplaced in the information flow. 
    
    if (name ==  G4String("Horn1TargetDownstrHeContainer")) {
    
        std::map<G4String, LBNEVolumePlacementData>::const_iterator itMother = 
         fSubVolumes.find(G4String("Horn1Hall"));
       if (itMother == fSubVolumes.end()) {
              std::ostringstream mStrStr;
              mStrStr << " Internal error for " << name << " Horn1Hall not found " << std::endl;
              G4String mStr(mStrStr.str());
              G4Exception("LBNEVolumePlacements::Create", " ", FatalErrorInArgument, mStr.c_str()); 
       }  
   
       info.fParams[0] = 0.; 
       info.fParams[1] = fTargetHeContTubeInnerRadius + fTargetHeContTubeThickness; 
       info.fParams[2] = fTargetHeContTubeLengthInHorn + 0.020*mm;
       G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
       info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("Beryllium")), volumeName); 
       info.fPosition[2] = -itMother->second.fParams[2]/2. + info.fParams[2]/2. + 0.002*mm; //
       // This is a surveyed volume, tweaked in PlaceFinal  
     }
     if (name ==  G4String("Horn1TargetDownstrHelium")) {
    
        std::map<G4String, LBNEVolumePlacementData>::const_iterator itMother = 
         fSubVolumes.find(G4String("Horn1TargetDownstrHeContainer"));
       if (itMother == fSubVolumes.end()) {
              std::ostringstream mStrStr;
              mStrStr << " Internal error for " << name 
	               << " Volume Horn1TargetDownstrHeContainer not found " << std::endl;
              G4String mStr(mStrStr.str());
              G4Exception("LBNEVolumePlacements::Create", " ", FatalErrorInArgument, mStr.c_str()); 
       }  
   
       info.fParams[0] = 0.; 
       info.fParams[1] = fTargetHeContTubeInnerRadius ; 
       info.fParams[2] = fTargetHeContTubeLengthInHorn + 0.010*mm;
       G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
       info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("HeliumTarget")), volumeName); 
       info.fPosition[2] = 0.; //
       // That was is a surveyed volume, tweaked in PlaceFinal         
    }   
    if (name == G4String("Horn1TargetFinVertFirst")) { // 
            info.fParams[0] = fTargetFinWidth; 
            info.fParams[1] = fTargetFinHeight - 2.0*fTargetCTubeOuterRadius - 0.1*mm; 
            info.fParams[2] = fTargetFinLengthSplitDwnstr  - 0.02*mm; // 20 micron safety. 
            G4Box* aBox = new G4Box(volumeName, info.fParams[0]/2, info.fParams[1]/2, info.fParams[2]/2.);
            info.fCurrent = new G4LogicalVolume(aBox, G4Material::GetMaterial(std::string("Target")), volumeName);
            info.fTypeName = G4String("Box");
 // despite being a single copy, do the placement in a separate method. 	    
    }
    if (name == G4String("Horn1TargetCoolingTubeFirst")) { // 
            info.fParams[0] = 0.; 
            info.fParams[1] = fTargetCTubeOuterRadius; 
            info.fParams[2] = fTargetFinLengthSplitDwnstr - 0.025*mm;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("Titanium")), volumeName); 
            info.fPosition[2] = fTargetFinHeight + 0.005*mm; // possibly tweak for the margin... 
    }
    if (name == G4String("Horn1TargetCoolingTubeFirstWater")) { // 
            info.fParams[0] = 0.; 
            info.fParams[1] = fTargetCTubeInnerRadius; 
            info.fParams[2] = fTargetFinLengthSplitDwnstr - 0.03*mm;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("Water")), volumeName); 
    }
    if (name == G4String("Horn1TargetSegmentFirst")) { //
            std::map<G4String, LBNEVolumePlacementData>::const_iterator itMother = 
            fSubVolumes.find(G4String("Horn1TargetDownstrHelium"));
            if (itMother == fSubVolumes.end()) {
              std::ostringstream mStrStr;
              mStrStr << " Internal error for " << name 
	              << " Volume Horn1TargetDownstrHelium not found " << std::endl;
              G4String mStr(mStrStr.str());
              G4Exception("LBNEVolumePlacements::Create", " ", FatalErrorInArgument, mStr.c_str()); 
             }  
            info.fParams[0] = fTargetFinWidth + 0.2*mm; 
            info.fParams[1] = fTargetFinHeight + 2.0*fTargetCTubeOuterRadius + 0.2*mm; 
            info.fParams[2] = fTargetFinLengthSplitDwnstr - 0.01*mm;
            G4Box* aBox = new G4Box(volumeName, info.fParams[0]/2., info.fParams[1]/2., info.fParams[2]/2.);
            info.fCurrent = new G4LogicalVolume(aBox, G4Material::GetMaterial(std::string("HeliumTarget")), volumeName); 
            info.fTypeName = G4String("Box");
	    info.fPosition[2] = -1.0*itMother->second.fParams[2]/2. + info.fParams[2]/2. + 0.010*mm;
    }
    if (name == G4String("Horn1TargetSegment")) { // A duplicate of the Upstream segment.  Geatn4 does not see the daughter volume 
            // above ???
            info.fParams[0] = fTargetFinWidth + 0.2*mm; 
            info.fParams[1] = fTargetFinHeight + 2.0*fTargetCTubeOuterRadius + 0.2*mm; 
            info.fParams[2] = fTargetFinLength+ 0.95*fTargetFinSpacingLength ;
            G4Box* aBox = new G4Box(volumeName, info.fParams[0]/2., info.fParams[1]/2., info.fParams[2]/2.);
            info.fCurrent = new G4LogicalVolume(aBox, G4Material::GetMaterial(std::string("HeliumTarget")), volumeName); 
            info.fTypeName = G4String("Box");
    }
    if (name == G4String("Horn1TargetCoolingTubeHLast")) { // 
            std::map<G4String, LBNEVolumePlacementData>::const_iterator itMother = 
            fSubVolumes.find(G4String("Horn1TargetDownstrHelium"));
            if (itMother == fSubVolumes.end()) {
              std::ostringstream mStrStr;
              mStrStr << " Internal error for " << name 
	              << " Volume Horn1TargetDownstrHelium not found " << std::endl;
              G4String mStr(mStrStr.str());
              G4Exception("LBNEVolumePlacements::Create", " ", FatalErrorInArgument, mStr.c_str()); 
             }  
            info.fParams[0] = 0.; 
            info.fParams[1] = fTargetCTubeOuterRadius; 
            info.fParams[2] = fTargetCTubeReturnLengthDownstr;
            G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
            info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("Titanium")), volumeName); 
            info.fPosition[0] = fTargetCTubeReturnHOffset/2.0;
            info.fPosition[2] =
	       itMother->second.fParams[2]/2 - fTargetCTubeOuterRadius - 15.0*mm - fTargetCTubeReturnLengthDownstr/2.0;
	       // ?????????????????????????? To be checked with  the drawing... // 
	    // Two different Vetical locations (up and down) 
	    info.fRotation.rotateY(fTargetCTubeReturnHOffset/(info.fParams[2]+2.0*mm)); // approximate.	
	    info.fRotationIsUnitMatrix = false; 		 
	        ; // possibly tweak for the margin... 
    }
    if (name == G4String("Horn1TargetCoolingTubeVLast")) { // 
          
            std::map<G4String, LBNEVolumePlacementData>::const_iterator itMother = 
            fSubVolumes.find(G4String("Horn1TargetDownstrHelium"));
            if (itMother == fSubVolumes.end()) {
              std::ostringstream mStrStr;
              mStrStr << " Internal error for " << name 
	              << " Volume Horn1TargetDownstrHelium not found " << std::endl;
              G4String mStr(mStrStr.str());
              G4Exception("LBNEVolumePlacements::Create", " ", FatalErrorInArgument, mStr.c_str()); 
             }  
            std::map<G4String, LBNEVolumePlacementData>::const_iterator itH = 
            fSubVolumes.find(G4String("Horn1TargetCoolingTubeHLast"));
            if (itH == fSubVolumes.end()) {
              std::ostringstream mStrStr;
              mStrStr << " Internal error for " << name 
	              << " Volume Horn1TargetCoolingTubeHLast not found " << std::endl;
              G4String mStr(mStrStr.str());
              G4Exception("LBNEVolumePlacements::Create", " ", FatalErrorInArgument, mStr.c_str()); 
             }  
           info.fParams[0] = 0.; 
           info.fParams[1] = fTargetCTubeOuterRadius; 
           info.fParams[2] = fTargetFinHeight;
           G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
           info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("Titanium")), volumeName); 
           info.fPosition[0] = fTargetCTubeReturnHOffset;
           info.fPosition[2] = 	itMother->second.fParams[2]/2 - fTargetCTubeOuterRadius - 10.0*mm  ;
	    // Two different Vetical locations (up and down) 
	   info.fRotation.rotateX(M_PI/2.); 	info.fRotationIsUnitMatrix = false;
	        ; // possibly tweak for the margin... 
    }
    if (name == G4String("Horn1TargetCoolingTubeHLastWater")) { // 
          std::map<G4String, LBNEVolumePlacementData>::const_iterator itMother = 
          fSubVolumes.find(G4String("Horn1TargetCoolingTubeHLast"));
          if (itMother == fSubVolumes.end()) {
              std::ostringstream mStrStr;
              mStrStr << " Internal error for " << name 
	              << " Horn1TargetCoolingTubeHLast not found " << std::endl;
              G4String mStr(mStrStr.str());
              G4Exception("LBNEVolumePlacements::Create", " ", FatalErrorInArgument, mStr.c_str()); 
           }  
           info.fParams[0] = 0.; 
           info.fParams[1] = fTargetCTubeInnerRadius; 
           info.fParams[2] = itMother->second.fParams[2] - 0.050*mm;
           G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
           info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("Water")), volumeName); 
    }
    if (name == G4String("Horn1TargetCoolingTubeVLastWater")) { // 
          std::map<G4String, LBNEVolumePlacementData>::const_iterator itMother = 
          fSubVolumes.find(G4String("Horn1TargetCoolingTubeVLast"));
          if (itMother == fSubVolumes.end()) {
              std::ostringstream mStrStr;
              mStrStr << " Internal error for " << name 
	              << " Volume Horn1TargetCoolingTubeVLast not found " << std::endl;
              G4String mStr(mStrStr.str());
              G4Exception("LBNEVolumePlacements::Create", " ", FatalErrorInArgument, mStr.c_str()); 
           }  
           info.fParams[0] = 0.; 
           info.fParams[1] = fTargetCTubeInnerRadius; 
           info.fParams[2] = itMother->second.fParams[2] - 0.050*mm;
           G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
           info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("Water")), volumeName); 
    }
    
     if (name == G4String("Horn1TargetDownstrHeContainerCap")) { // 
             std::map<G4String, LBNEVolumePlacementData>::const_iterator itMother = 
            fSubVolumes.find(G4String("Horn1TargetDownstrHelium"));
            if (itMother == fSubVolumes.end()) {
              std::ostringstream mStrStr;
              mStrStr << " Internal error for " << name 
	              << " Volume Horn1TargetDownstrHelium not found " << std::endl;
              G4String mStr(mStrStr.str());
              G4Exception("LBNEVolumePlacements::Create", " ", FatalErrorInArgument, mStr.c_str()); 
             }  
          info.fParams[0] = 0.; 
           info.fParams[1] = fTargetHeContTubeInnerRadius-0.010*mm; 
           info.fParams[2] = 0.5*mm; // Docdb 6100
           G4Tubs* aTube = new G4Tubs(volumeName, info.fParams[0], info.fParams[1], info.fParams[2]/2., 0., 360.*deg);
           info.fCurrent = new G4LogicalVolume(aTube, G4Material::GetMaterial(std::string("Beryllium")), volumeName); 
	   info.fPosition[2] = itMother->second.fParams[2]/2 - info.fParams[2] - 0.5*mm;
    }
   
    
  
  } // End of Horn1 
   
  fSubVolumes.insert(std::pair<G4String, LBNEVolumePlacementData>(name, info));
  return &(fSubVolumes.find(name)->second);
}
// Place a volume, single copy. 
G4PVPlacement* LBNEVolumePlacements::PlaceFinal(const G4String &name, G4VPhysicalVolume *mother) {
			       
    G4LogicalVolume *lMother=mother->GetLogicalVolume();			       
    std::map<G4String, LBNEVolumePlacementData>::iterator it = fSubVolumes.find(name);
    if (it == fSubVolumes.end()) {
      std::ostringstream mStrStr;
      mStrStr << " Internal error for " << name << " Mother Volume not found " << std::endl;
      G4String mStr(mStrStr.str());
      G4Exception("LBNEVolumePlacements::PlaceFinal", " ", FatalErrorInArgument, mStr.c_str()); 
    }  
    std::string surveyedPtName(name);
    // Special case, nomenclature confusion.. Otherwise, name Surveyed point would get too long. 
    if (name == G4String("UpstreamTargetAssembly")) surveyedPtName = std::string("Canister");
    if (name == G4String("TargetUpstrDownstrHeContainer")) surveyedPtName = std::string("HeTube");
    if (name == G4String("Horn1TargetDownstrHeContainer")) surveyedPtName = std::string("HeTube");
    
    LBNEVolumePlacementData &info=it->second;
    info.fMother  =  mother; 
    LBNESurveyor* theSurvey = LBNESurveyor::Instance();
    std::vector<double> posA(3,0.); 
    for (size_t k=0; k!=3; ++k) posA[k] = info.fPosition[k]; 
    std::cerr << " LBNEVolumePlacements::PlaceFinal, Prior to alignment " << name << " half size " 
                << info.fParams[2]/2. << " position X " << info.fPosition[0] 
		 << " Y " << info.fPosition[1] << " Z  " << info.fPosition[2] << std::endl;
    if (theSurvey->size() != 0) {
      G4ThreeVector deltaUpstrLeft; 
      G4ThreeVector deltaUpstrRight; 
      G4ThreeVector deltaDownstrLeft; 
      G4ThreeVector deltaDownstrRight; 
      // This code could be optimize a bit, by removing extra vector copies.. 
      for(std::vector<LBNESurveyedPt>::const_iterator itSurv = theSurvey->begin(); 
            itSurv != theSurvey->end(); itSurv++) {
	    if (itSurv->GetName().find(surveyedPtName) == std::string::npos) continue;
	      if (itSurv->GetName().find("Upstr") !=  std::string::npos) {
	        if (itSurv->GetName().find("Left") !=  std::string::npos) deltaUpstrLeft = itSurv->GetPosition(); 
		else if (itSurv->GetName().find("Right") !=  std::string::npos) deltaUpstrRight = itSurv->GetPosition(); 
	      } else if (itSurv->GetName().find("Downstr") !=  std::string::npos) {
	        if (itSurv->GetName().find("Left") !=  std::string::npos) deltaDownstrLeft = itSurv->GetPosition(); 
		else if (itSurv->GetName().find("Right") !=  std::string::npos) deltaDownstrRight = itSurv->GetPosition(); 
	      } 
       }
      std::vector<double> deltaUpstr(3, 0.); 
      std::vector<double> deltaDownstr(3, 0.); 
      std::vector<double> deltaSlopes(2, 0.); 
      for (size_t k=0; k != 3; ++k)  {
         if ((std::abs(deltaUpstrLeft[k]) > 2.0e-3*mm) && (std::abs(deltaUpstrLeft[k]) > 2.0e-3*mm)) 
	   deltaUpstr[k] = 0.5*(deltaUpstrLeft[k] + deltaUpstrRight[k]);
	 else if (std::abs(deltaUpstrLeft[k]) > 2.0e-3*mm)  deltaUpstr[k] = deltaUpstrLeft[k];
	 else if (std::abs(deltaUpstrRight[k]) > 2.0e-3*mm)  deltaUpstr[k] = deltaUpstrRight[k];
         if ((std::abs(deltaDownstrLeft[k]) > 2.0e-3*mm) && (std::abs(deltaDownstrLeft[k]) > 2.0e-3*mm)) 
	   deltaDownstr[k] = 0.5*(deltaDownstrLeft[k] + deltaDownstrRight[k]);
	 else if (std::abs(deltaDownstrLeft[k]) > 2.0e-3*mm)  deltaDownstr[k] = deltaDownstrLeft[k];
	 else if (std::abs(deltaDownstrRight[k]) > 2.0e-3*mm)  deltaDownstr[k] = deltaDownstrRight[k];
	 // Special case for the Helium tube that contains the target segments: 
	 // Only the dowstream measurements make sense 
	 if ((name == "TargetUpstrDownstrHeContainer") || (name == "Horn1TargetDownstrHeContainer"))  { 
	   if (std::abs(deltaUpstr[k]) >  2.0e-3*mm) {
	     std::cerr << " LBNEVolumePlacements::PlaceFinal The upstream section of the He Container is misaligned." << std::endl; 
	     std::cerr << "   This makes little sense from a mechanical point of view. " << std::endl; 
	     std::cerr << "   Suggested action: misaling the target canister instead. " << std::endl;
	     std::cerr << "   Meanwhile, setting the deltaUpstream to 0. " << std::endl;
	     deltaUpstr[k] = 0.;
	   }
	   if (k != 2) {
	      deltaSlopes[k] = deltaDownstr[k]/(fTargetHeContTubeLengthUpstr + fTargetHeContTubeLengthInHorn);
	      if (name == "TargetUpstrDownstrHeContainer") {
	        info.fPosition[k] += 0.5 * fTargetHeContTubeLengthUpstr * deltaSlopes[k];
	      } else if (name == "Horn1TargetDownstrHeContainer") {
	        info.fPosition[k] += 0.5 * fTargetHeContTubeLengthInHorn * deltaSlopes[k]; 
              } else { 
	        info.fPosition[k] += 0.5*(deltaUpstr[k] + deltaDownstr[k]);
                deltaSlopes[k] = (deltaDownstr[k] - deltaUpstr[k])/info.fParams[2]; 
	      }
	   }
	 }
      }
      if ((std::abs(deltaSlopes[0]) > 2.0e-9) || (std::abs(deltaSlopes[1]) > 2.0e-9)) { 
        info.fRotationIsUnitMatrix = false;
        info.fRotation.rotateX(deltaSlopes[0]);	
        info.fRotation.rotateY(deltaSlopes[1]);	
      }
    } // End of alignment. 
    std::cerr << " LBNEVolumePlacements::PlaceFinal, " << name << " half size " 
                << info.fParams[2]/2. << " position X " << info.fPosition[0] 
		 << " Y " << info.fPosition[1] << " Z  " << info.fPosition[2] << std::endl;
    std::map<G4String, LBNEVolumePlacementData>::iterator itMother = 
       fSubVolumes.find(lMother->GetName());
    if (itMother != fSubVolumes.end()) {
       LBNEVolumePlacementData &infoMother=itMother->second;
      std::cerr << " .... Params for mother ";
      for (size_t k=0; k!= infoMother.fParams.size(); k++) std::cerr << " " << infoMother.fParams[k] << ",";
      std::cerr << " . " << std::endl;

    }
    G4PVPlacement *placement=0;
    G4String vpName(name); vpName += G4String("_P");
    if (info.fRotationIsUnitMatrix) 
        placement=new G4PVPlacement((G4RotationMatrix *) 0, 
	                             info.fPosition, info.fCurrent, vpName, lMother, false, 0, fCheckVolumeOverLapWC);
    else    
        placement=new G4PVPlacement(&info.fRotation, info.fPosition, info.fCurrent, 
	                             vpName, lMother, false, 0, fCheckVolumeOverLapWC);
    info.fIsPlaced = true;
    // Optionally, we could test for overlap after installing each new volume here. 				     
    return placement;			       
}

void LBNEVolumePlacements::PlaceFinalUpstrTarget(G4PVPlacement *mother) {
			       
    // Define in LBNEDetectorConstruction..    
    //Start by defining the top level of the target container volume, which includes the canister (TargetUpstrM0) 
   // and the target section which is outside Horn1 (TargetUpstrM1)
   
    LBNEVolumePlacementData *plMTop = Create("TargetUpstrMTop");
    G4PVPlacement *vMTop = PlaceFinal(std::string("TargetUpstrMTop"), mother);
    std::cerr << " TargetUpstrMTop  created and placed ... " << std::endl;
    LBNEVolumePlacementData *plM0 = Create("TargetUpstrM0");
    std::cerr << " TargetUpstrM0  created ... " << std::endl;
    LBNEVolumePlacementData *plM1 = Create("TargetUpstrM1");
    std::cerr << " TargetUpstrM1  created ... " << std::endl;
    G4PVPlacement *vM0 = PlaceFinal(std::string("TargetUpstrM0"), vMTop);
    std::cerr << " TargetUpstrM0  placed ... " << std::endl;
    G4PVPlacement *vM1 = PlaceFinal(std::string("TargetUpstrM1"), vMTop);
    std::string tUpUp("TargetUpstrUpstr");
    Create(tUpUp + std::string("Plate"));
    Create(tUpUp + std::string("Can"));
    Create(tUpUp + std::string("CanEndPlate"));
    Create(tUpUp + std::string("DwstrFlange"));
    Create(tUpUp + std::string("CoolingTube"));
    Create(tUpUp + std::string("CoolingTubeWater"));
    Create(tUpUp + std::string("SupportBlockTopLeft"));
    Create(tUpUp + std::string("SupportBlockBottomLeft"));
    Create(tUpUp + std::string("SupportBlockRight"));
    Create(tUpUp + std::string("SupportSleeve"));
    // None of these are surveyable, position is fixed. 
    PlaceFinal(tUpUp + std::string("Plate"), vM0);
    PlaceFinal(tUpUp + std::string("Can"), vM0);
    PlaceFinal(tUpUp + std::string("CanEndPlate"), vM0);
    PlaceFinal(tUpUp + std::string("DwstrFlange"), vM0);
    // Cooling tubes: two copies. This breaks the Placement data model. Never mind, assume here that misalignment 
    // of these cooling tube has negligible effect on the physics. 
    
    G4String nameTmp5(tUpUp + std::string("CoolingTube"));
    std::map<G4String, LBNEVolumePlacementData>::iterator itTmp = fSubVolumes.find(nameTmp5);
    LBNEVolumePlacementData &infoTmp = itTmp->second;
    G4ThreeVector posTmp;
    posTmp[0] = 0.;
    posTmp[1] = 0.5*fTargetFinHeight;
    posTmp[2] = -1.0*plM0->fParams[2]/2.0 + fTargetUpstrUpstrMargin +
	                         fTargetUpstrPlateThick + fTargetCanLength + 
				 fTargetDownstrCanFlangeThick + fTargetFlangeThick - infoTmp.fParams[2]/2 - 1.0*mm;
    G4String vpNameTmp5(nameTmp5);				 			 
    std::cerr << " Position for " << vpNameTmp5+G4String("_PTop") << " X = " 
                                  << posTmp[0] << " Y " << posTmp[1] << " Z " << posTmp[2] << std::endl;  
    G4PVPlacement *vCoolingTubeTop = new G4PVPlacement((G4RotationMatrix *) 0, 
	                            posTmp, infoTmp.fCurrent, vpNameTmp5+G4String("_PTop"),
				     vM0->GetLogicalVolume(), false, 0, fCheckVolumeOverLapWC );
    posTmp[1] = -0.5*fTargetFinHeight;
    G4PVPlacement *vCoolingTubeBottom = new G4PVPlacement((G4RotationMatrix *) 0, 
	                            posTmp, infoTmp.fCurrent, vpNameTmp5+std::string("_PBottom"),
				     vM0->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);
    
    PlaceFinal(tUpUp + std::string("CoolingTubeWater"), vCoolingTubeTop);
    PlaceFinal(tUpUp + std::string("CoolingTubeWater"), vCoolingTubeBottom);
    PlaceFinal(tUpUp + std::string("SupportBlockTopLeft"), vM0);
    PlaceFinal(tUpUp + std::string("SupportBlockBottomLeft"), vM0);
    PlaceFinal(tUpUp + std::string("SupportBlockRight"), vM0);
    PlaceFinal(tUpUp + std::string("SupportSleeve"), vM0);
    // We need three support/alignment rods. Identical volume, but at different locations. 
    std::string nameTmp10(tUpUp + std::string("SupportRod"));
    Create(nameTmp10);
    itTmp = fSubVolumes.find(nameTmp10);
    infoTmp = itTmp->second;
    // Top Left alignment/support rod. 
    posTmp[0] = -20.5*mm; // Again, accurate to +- 1 mm or so. 
    posTmp[1] = 24.0*mm;
    posTmp[2] = -1.0*plM0->fParams[2]/2.0 + fTargetUpstrUpstrMargin +
	                         fTargetUpstrPlateThick + fTargetCanLength + 
				 fTargetDownstrCanFlangeThick - infoTmp.fParams[2]/2 - 1.0*mm;
    G4String vpNameTmp10(nameTmp10);				 			 
    G4PVPlacement *vRodTopLeft = new G4PVPlacement((G4RotationMatrix *) 0, 
	                            posTmp, infoTmp.fCurrent, vpNameTmp10+std::string("_PTopLeft"), 
				            vM0->GetLogicalVolume(), false, 0, fCheckVolumeOverLapWC);
    posTmp[0] = -15.5*mm; // Again, accurate to +- 1 mm or so. 
    posTmp[1] = -36.0*mm;
    G4PVPlacement *vRodBottomLeft = new G4PVPlacement((G4RotationMatrix *) 0, 
	                            posTmp, infoTmp.fCurrent, vpNameTmp10+std::string("_PBottomLeft"),
				     vM0->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);
    posTmp[0] = 33.0*mm; // Again, accurate to +- 1 mm or so. 
    posTmp[1] = 5.0*mm;
    G4PVPlacement *vRodRight = new G4PVPlacement((G4RotationMatrix *) 0, 
	                            posTmp, infoTmp.fCurrent, vpNameTmp10+std::string("_PRight"), 
				    vM0->GetLogicalVolume(), false, 2, fCheckVolumeOverLapWC);
    
//  Horizontal fin for beam alignment. Most important thing in the canister, in terms of material budget 

    std::string nameTmp11("TargetFinHorizontal");
    Create(nameTmp11);
    PlaceFinal(nameTmp11, vM0);
    
    // Done with the target canister..Now assemble the upstream part of the target it self. (Upstream of the horn1)  
    std::string tUpDown("TargetUpstrDownstr"); 
    Create(tUpDown + std::string("HeContainer")); 
    G4PVPlacement *vHeTube = PlaceFinal(tUpDown + std::string("HeContainer"), vM1); // Surveyable 
    Create(tUpDown + std::string("Helium")); 
    G4PVPlacement *vHelium = PlaceFinal(tUpDown + std::string("Helium"), vHeTube); // Fixed 
    
    // First alignment ring, locate flush with the end plate (within 1 mm ) , left and right      
    Create(G4String("TargetAlignmentRingLeft"));
    std::map<G4String, LBNEVolumePlacementData>::iterator itTmpRLeft = fSubVolumes.find(G4String("TargetAlignmentRingLeft"));
    LBNEVolumePlacementData &infoTmpRLeft = itTmpRLeft->second;
    Create(G4String("TargetAlignmentRingRight"));
    std::map<G4String, LBNEVolumePlacementData>::iterator itTmpRRight = fSubVolumes.find(G4String("TargetAlignmentRingRight"));
    LBNEVolumePlacementData &infoTmpRRight = itTmpRRight->second;

    posTmp[0] = 0.; // The alignment rings are always centered.. 
    posTmp[1] = 0.;
    posTmp[2] = -1.0*plM1->fParams[2]/2.0 + infoTmpRLeft.fParams[2]/2. + 1.0*mm; // 1 mm spacing Left and right have the same thickness. 
    int copyNumber = 0;
    while (true) {
      std::ostringstream cNumStrStr; cNumStrStr << "_P" << copyNumber;
      G4PVPlacement((G4RotationMatrix *) 0, 
	                            posTmp, infoTmpRLeft.fCurrent, G4String("TargetAlignmentRingLeft")+cNumStrStr.str(), 
				          vHelium->GetLogicalVolume(), false, copyNumber, fCheckVolumeOverLapWC);
      G4PVPlacement((G4RotationMatrix *) 0, 
	                            posTmp, infoTmpRRight.fCurrent, G4String("TargetAlignmentRingRight")+ cNumStrStr.str(), 
				          vHelium->GetLogicalVolume(), false, copyNumber, fCheckVolumeOverLapWC);
      posTmp[2] += fTargetAlignRingSpacing;      
      copyNumber++;
      if ( posTmp[2] > (plM1->fParams[2]/2.0 - 1.0*mm)) break;
    }				  
    //
    // Define the 3 elementry volumes in a given 
    LBNEVolumePlacementData *plTargetCoolingTube = Create(G4String("TargetUpstrDownstrCoolingTube"));    
    LBNEVolumePlacementData  *plTargetCoolingTubeWater = Create(G4String("TargetUpstrDownstrCoolingTubeWater"));
    LBNEVolumePlacementData *plTargetFin = Create(G4String("TargetFinVert"));
    G4String nameTgtUpDownSeg("TargetUpstrDownstrSegment");
    LBNEVolumePlacementData *plTargetUpstrDownstrSegment = Create(nameTgtUpDownSeg);
    G4PVPlacement *vSeg = 0;
    const double zCoordTmp = -1.0*plM1->fParams[2]/2.0 + fTargetZOffsetStart;
    for (int iSeg=0; iSeg != fTargetNumFinsUpstr-1; iSeg++) { // Place with no misalignment,  The last segment is placed below  
      posTmp[0] = 0.; posTmp[1] = 0.;
      posTmp[2] =  zCoordTmp
                  + plTargetUpstrDownstrSegment->fParams[2]/2. + iSeg*(fTargetFinLength + fTargetFinSpacingLength);
      std::ostringstream cNumStrStr; cNumStrStr << "_P" << iSeg;
      vSeg = new G4PVPlacement((G4RotationMatrix *) 0, 
	                            posTmp, plTargetUpstrDownstrSegment->fCurrent, 
				      nameTgtUpDownSeg+cNumStrStr.str(), 
				          vHelium->GetLogicalVolume(), false, copyNumber, fCheckVolumeOverLapWC);
//      std::cerr << " Placing target at Zs " << 	posTmp[2] << std::endl;			  
    }
    posTmp[0] = 0.; posTmp[1] = fTargetFinHeight/2.; posTmp[2] =	0.;			  
    G4PVPlacement *vTubeUp =  new G4PVPlacement((G4RotationMatrix *) 0, 
	                            posTmp, plTargetCoolingTube->fCurrent, 
				      nameTgtUpDownSeg+G4String("CoolingTubeUp_PTop"), 
				          vSeg->GetLogicalVolume(), false, 0, fCheckVolumeOverLapWC);
					  
    posTmp[1] = -1.0*fTargetFinHeight/2.;			  
    G4PVPlacement *vTubeDown =  new G4PVPlacement((G4RotationMatrix *) 0, 
	                            posTmp, plTargetCoolingTube->fCurrent,
				     nameTgtUpDownSeg+ G4String("CoolingTubeUp_PBottom"), 
				          vSeg->GetLogicalVolume(), false, 0, fCheckVolumeOverLapWC);
    posTmp[0] = 0.; posTmp[1] = 0.; posTmp[2] = 0.;			  
    G4PVPlacement *vTubeWater = new G4PVPlacement((G4RotationMatrix *) 0, 
	                            posTmp, plTargetCoolingTubeWater->fCurrent,
				    nameTgtUpDownSeg+ G4String("CoolingTubeWater_P"), 
				          vTubeUp->GetLogicalVolume(), false, 0, fCheckVolumeOverLapWC);
      
    G4PVPlacement *vTargetFin = new G4PVPlacement((G4RotationMatrix *) 0, 
	                            posTmp, plTargetFin->fCurrent, nameTgtUpDownSeg+G4String("TargetFinVert_P"), 
				          vSeg->GetLogicalVolume(), false, 0, fCheckVolumeOverLapWC);
    // Repeat the last few mantra for the last segments 
    LBNEVolumePlacementData *plTargetCoolingTubeLast = Create(G4String("TargetUpstrDownstrCoolingTubeLast"));    
    LBNEVolumePlacementData *plTargetCoolingTubeWaterLast = Create(G4String("TargetUpstrDownstrCoolingTubeLastWater"));
    LBNEVolumePlacementData *plTargetFinLast = Create(G4String("TargetFinVertUpstrLast"));
    G4String nameTgtUpDownSegLast("TargetUpstrDownstrSegmentLast");
    LBNEVolumePlacementData *plTargetUpstrDownstrSegmentLast = Create(nameTgtUpDownSegLast);
    posTmp[0] = 0.; posTmp[1] = 0.;
    
    posTmp[2] = zCoordTmp + plTargetUpstrDownstrSegmentLast->fParams[2]/2. + 
		  (fTargetNumFinsUpstr-1)*(fTargetFinLength + fTargetFinSpacingLength);
    vSeg = new G4PVPlacement((G4RotationMatrix *) 0, 
	                            posTmp, plTargetUpstrDownstrSegmentLast->fCurrent, 
				      G4String("TargetUpstrDowstrSegmentLast"),
				          vHelium->GetLogicalVolume(), false, 0, fCheckVolumeOverLapWC);
    posTmp[0] = 0.; posTmp[1] = fTargetFinHeight/2.; posTmp[2] = 0.;			  
    vTubeUp =  new G4PVPlacement((G4RotationMatrix *) 0, 
	                            posTmp, plTargetCoolingTubeLast->fCurrent, 
				    G4String("TargetUpstrDownstrCoolingTubeUpLast_PTop"), 
				          vSeg->GetLogicalVolume(), false, 0, fCheckVolumeOverLapWC);
					  
    posTmp[1] = -1.0*fTargetFinHeight/2.;			  
    vTubeDown =  new G4PVPlacement((G4RotationMatrix *) 0, 
	                            posTmp, plTargetCoolingTubeLast->fCurrent, 
				    G4String("TargetUpstrDownstrCoolingTubeUpLast_PBottom"), 
				          vSeg->GetLogicalVolume(), false, 0, fCheckVolumeOverLapWC);
    posTmp[0] = 0.; posTmp[1] = 0.; posTmp[2] = 0.;			  
    vTubeWater = new G4PVPlacement((G4RotationMatrix *) 0, 
	                            posTmp, plTargetCoolingTubeWaterLast->fCurrent, 
				    nameTgtUpDownSeg+G4String("CoolingTubeWater_P"), 
				          vTubeUp->GetLogicalVolume(), false, 0, fCheckVolumeOverLapWC);
    vTargetFin = new G4PVPlacement((G4RotationMatrix *) 0, 
	                            posTmp, plTargetFinLast->fCurrent, G4String("TargetFinVertLast_P"), 
				          vSeg->GetLogicalVolume(), false, 0, fCheckVolumeOverLapWC);
   					  				  
    // End for the segment of the target which is outside Horn1
    
    // We now have to include at the end of VM1 the parts of the transition Inner to Outer from Horn1. 
    // No misalignment implemented, dimensions are too crude to start with, material too far from the 
    // beam line anyway. 
    G4String nameHorn1IO("TargetHorn1TransInnerOuter");
    G4String nameHorn1IOCont(nameHorn1IO + G4String("Cont"));
    LBNEVolumePlacementData *plHorn1IOCont = Create(nameHorn1IOCont);
    G4PVPlacement *vHorn1IOCont = PlaceFinal(nameHorn1IOCont, vM1);
    for (size_t iPartIO = 0; iPartIO != fTargetHorn1InnerRadsUpstr.size(); iPartIO++) {
      std::ostringstream nameStrStr; nameStrStr << nameHorn1IO << "Part" << iPartIO;
      G4String nameStr(nameStrStr.str());
      LBNEVolumePlacementData *plHorn1IOCont = Create(nameStr);
      PlaceFinal(nameStr, vHorn1IOCont);
    }        
}



void LBNEVolumePlacements::PrintAll() const {

 std::cout << "======================================================================================" << std::endl;
 std::cout << " Dump of all the Placed Volumes " << std::endl;
 for (std::map<G4String, LBNEVolumePlacementData>::const_iterator it= fSubVolumes.begin(); it!=fSubVolumes.end();
                              it++) {
			      
    const LBNEVolumePlacementData &info = it->second;
    std::cout << " SubVolume " << it->first << " of type " << info.fTypeName;
    if (info.fMother == 0) std::cout << " has no assigned mother volume" << std::endl;
    else std::cout << " is in logical volume " << info.fMother->GetLogicalVolume()->GetName() << std::endl;
    std::cout << " .... Params Values ";
    for (std::vector<double>::const_iterator ip=info.fParams.begin(); ip!=info.fParams.end(); ip++) 
      std::cout << " " << (*ip) << ", ";
    std::cout << std::endl;  
    std::cout << " .... Position " << info.fPosition;
    std::cout << std::endl;  
    if (!info.fRotationIsUnitMatrix) {
     std::cout << " .......... Has a rotation matrix ";
     info.fRotation.print(std::cout);
    }  
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    
  }
  std::cout << "======================================================================================" << std::endl;
}
			       
void LBNEVolumePlacements::TestVolumeOverlap(const G4String &name, G4VPhysicalVolume *mother) const {

//    G4PVPlacement *pvpMother= static_cast<G4PVPlacement *>(mother);
    G4LogicalVolume *vMother=mother->GetLogicalVolume();			       
    std::map<G4String, LBNEVolumePlacementData>::const_iterator it = fSubVolumes.find(name);
    if (it == fSubVolumes.end()) {
      std::ostringstream mStrStr;
      mStrStr << " Internal error for " << name << " Volume not found " << std::endl;
      G4String mStr(mStrStr.str());
      G4Exception("LBNEVolumePlacements::TestVolumeOverlap", " ", FatalErrorInArgument, mStr.c_str()); 
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
              std::ostringstream mStrStr;
              mStrStr << " Internal error for " << name << " Volume not found " << std::endl;
              G4String mStr(mStrStr.str());
              G4Exception("LBNEVolumePlacements::TestVolumeOverlap", " ", FatalErrorInArgument, mStr.c_str()); 
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
  fTargetNumFins = (int) (targetNumFinsNominal + 0.5);
  double deltaNumFins =  targetNumFinsNominal - (double) fTargetNumFins; 
  if (deltaNumFins > 0.5) fTargetNumFins++;
  if (deltaNumFins < -0.5) fTargetNumFins--;
//  std::cout << " LBNEVolumePlacements::SegmentTarget, total number of segments " << fTargetNumFins << 
//               " Delta Num fins " << deltaNumFins << std::endl;
  const double oldLength = fTargetSLengthGraphite;
  fTargetSLengthGraphite = fTargetNumFins*fTargetFinLength + (fTargetNumFins-1)*fTargetFinSpacingLength; 
  fTargetFinSpacingLength = (fTargetSLengthGraphite  - fTargetNumFins*fTargetFinLength)/(fTargetNumFins - 1); //   
  if (std::abs(oldLength - fTargetSLengthGraphite) > 0.001*fTargetSLengthGraphite)
     std::cout << " LBNEVolumePlacements::segmentTarget: Warning: re-adjust the target length from " <<
        oldLength <<  " to " << fTargetSLengthGraphite << " to get  an integer number of 2 cm long segments " << std::endl;
  // Compute the number of fins upstream and dowsntream of the break between target and horn1.
   
  double tempNFins = (fTargetLengthOutsideHorn - fTargetFinSpacingLength) /(fTargetFinLength + fTargetFinSpacingLength);
  fTargetNumFinsUpstr = (int) tempNFins;
  fTargetNumFinsInHorn = fTargetNumFins - fTargetNumFinsUpstr; //Note one fin will be split...  	
//  std::cerr << "  Num Fins Ups /Downstr " << fTargetNumFinsUpstr << " / " << fTargetNumFinsInHorn << std::endl;
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


