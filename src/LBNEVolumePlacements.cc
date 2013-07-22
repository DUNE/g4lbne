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
		       // With respect to the center of the mother volume UpstreamTargetHall
 
   //
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
  
  } else if (name == G4String("UpstreamTargetHall")) {
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
        fSubVolumes.find(G4String("UpstreamTargetHall"));
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
   
  fSubVolumes.insert(std::pair<G4String, LBNEVolumePlacementData>(name, info));
  return &(fSubVolumes.find(name)->second);
}

G4VPhysicalVolume* LBNEVolumePlacements::PlaceFinal(const G4String &name, G4VPhysicalVolume *mother,
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
    
    if (name == G4String("UpstreamTargetHall")) {
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
LBNEPlacementMessenger::~LBNEPlacementMessenger() {

  delete fWaterLayerThickInHorn;
  delete fDecayPipeLength;
  delete fHorn1Length;

}

LBNEPlacementMessenger::LBNEPlacementMessenger()
 {

//   std::cerr << " LBNEPlacementMessenger::LBNEPlacementMessenger, contructor, starts " << std::endl;
   LBNEVolumePlacements* volP=LBNEVolumePlacements::Instance();
   fDecayPipeLength  = new G4UIcmdWithADoubleAndUnit("/LBNE/det/decayPipeLength",this);
   fDecayPipeLength->SetGuidance("Length of the decay Pipe");
   fDecayPipeLength->SetParameterName("decayPipeLength",true);
   double value = volP->GetDecayPipeLength();
    fDecayPipeLength->SetDefaultValue (value);
   fDecayPipeLength->SetDefaultUnit ("m");
   fDecayPipeLength->SetUnitCandidates ("cm m");
   fDecayPipeLength->AvailableForStates(G4State_PreInit, G4State_Idle);

   fWaterLayerThickInHorn  = new G4UIcmdWithADoubleAndUnit("/LBNE/det/waterThickInHorn",this);
   fWaterLayerThickInHorn->SetGuidance("Water Thicknes on the inner conductor of the horn");
   fWaterLayerThickInHorn->SetParameterName("waterThickInHorn",true);
   fWaterLayerThickInHorn->SetDefaultValue (0.);
   fWaterLayerThickInHorn->SetDefaultUnit ("mm");
   fWaterLayerThickInHorn->SetUnitCandidates ("mm cm m");
   fWaterLayerThickInHorn->AvailableForStates(G4State_PreInit, G4State_Idle);
   { 
     fHorn1Length  = new G4UIcmdWithADoubleAndUnit("/LBNE/det/Horn1LengthLength",this);
     G4String guidance("Length of Horn1.\n  ");
     guidance += std::string(" The length of the outer conductor, excludes the upstream transition inner/outer, \n");
     guidance += std::string("  and flanges with bolts. If extended by user, it is assumed that conductor are extended.. " ); 
     guidance += std::string("  NUMI Horn1, FNAL Drawing number 8875. 112-ME-363092 " ); 
     fHorn1Length->SetGuidance(guidance);
     fHorn1Length->SetParameterName("Horn1LengthLength",true);
     double value = volP->GetHorn1Length(); //  
     
     fHorn1Length->SetDefaultValue (value);
     fHorn1Length->SetDefaultUnit ("m");
     fHorn1Length->SetUnitCandidates ("cm m");
     fHorn1Length->AvailableForStates(G4State_PreInit, G4State_Idle);
   }
    std::cerr << " LBNEPlacementMessenger::LBNEPlacementMessenger, contructor, Ends " << std::endl;
  
}
void LBNEPlacementMessenger::SetNewValue(G4UIcommand* command,  G4String newValue) {
   
   LBNEVolumePlacements* volP=LBNEVolumePlacements::Instance();
   if (command == fWaterLayerThickInHorn) {
     G4UIcmdWithADoubleAndUnit* cmdWD = dynamic_cast<G4UIcmdWithADoubleAndUnit*> (command);
     volP->SetWaterLayerThickInHorns(cmdWD->GetNewDoubleValue(newValue));
//     std::cerr << " Changed WaterLayerThickInHorns , o.k. " << std::endl;  // exit(2);
   }
   if (command == fDecayPipeLength) {
     G4UIcmdWithADoubleAndUnit* cmdWD = dynamic_cast<G4UIcmdWithADoubleAndUnit*> (command);
     volP->SetDecayPipeLength(cmdWD->GetNewDoubleValue(newValue));
   }
   if (command == fHorn1Length) {
     G4UIcmdWithADoubleAndUnit* cmdWD = dynamic_cast<G4UIcmdWithADoubleAndUnit*> (command);
     volP->SetHorn1Length(cmdWD->GetNewDoubleValue(newValue));
   }
    
   

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


