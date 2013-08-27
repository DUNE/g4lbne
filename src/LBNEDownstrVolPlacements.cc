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

#include "G4VisExtent.hh"
#include "LBNEPlacementMessenger.hh"
#include "LBNESurveyor.hh"

//---  Second file for geometry: the first one (LBNEVolumePlacements) was getting to long --// 

void LBNEVolumePlacements::PlaceFinalDownstrTarget(G4PVPlacement *mother) {
  
    Create("Horn1TargetDownstrHeContainer");
    G4PVPlacement *vMTop = PlaceFinal(std::string("Horn1TargetDownstrHeContainer"), mother);
    Create("Horn1TargetDownstrHelium");
    G4PVPlacement *vMHe = PlaceFinal(std::string("Horn1TargetDownstrHelium"), vMTop);
    // Deal with alignment ring located in the dowstream part of the target. We assume here that 
    // they have been already defined while dealing with the Upstream target portion 
    // First alignment ring, locate flush with the end plate (within 1 mm ) , left and right      
    std::map<G4String, LBNEVolumePlacementData>::iterator itTmpRLeft = fSubVolumes.find(G4String("TargetAlignmentRingLeft"));
    LBNEVolumePlacementData &infoTmpRLeft = itTmpRLeft->second;
    std::map<G4String, LBNEVolumePlacementData>::iterator itTmpRRight = fSubVolumes.find(G4String("TargetAlignmentRingRight"));
    LBNEVolumePlacementData &infoTmpRRight = itTmpRRight->second;
    std::map<G4String, LBNEVolumePlacementData>::iterator itM1 = fSubVolumes.find(G4String("TargetUpstrDownstrHelium"));
    LBNEVolumePlacementData *plM1 = &itM1 ->second;
    std::map<G4String, LBNEVolumePlacementData>::iterator itMother = fSubVolumes.find(G4String("Horn1TargetDownstrHelium"));
    LBNEVolumePlacementData *plMother = &itMother ->second;
    G4ThreeVector posTmp;
    posTmp[0] = 0.; // The alignment rings are always centered.. 
    posTmp[1] = 0.; // We start upstream to keep the separation among rings. 
    posTmp[2] = -1.0*plM1->fParams[2]/2.0 + infoTmpRLeft.fParams[2]/2. + 1.0*mm; // 1 mm spacing Left and right have the same thickness. 
    int copyNumber = 0;
    int copyNumberHere = 0;
    bool transitionDone = false;
    while (true) {
      if (!transitionDone) {
         posTmp[2] += fTargetAlignRingSpacing;      
         copyNumber++;
	 if (posTmp[2] > plM1->fParams[2]/2.0) {
	   transitionDone = true;
	   const double offset = posTmp[2] - plM1->fParams[2]/2.0;
	   posTmp[2] =  -1.0*plMother->fParams[2]/2.0 + infoTmpRLeft.fParams[2]/2. + offset ;
	 }
      } else {
        std::ostringstream cNumStrStr; cNumStrStr << "_P" << copyNumber;
        G4PVPlacement((G4RotationMatrix *) 0, 
	                            posTmp, infoTmpRLeft.fCurrent, G4String("TargetAlignmentRingLeft")+cNumStrStr.str(), 
				          vMHe->GetLogicalVolume(), false, copyNumberHere, fCheckVolumeOverLapWC);
        G4PVPlacement((G4RotationMatrix *) 0, 
	                            posTmp, infoTmpRRight.fCurrent, G4String("TargetAlignmentRingRight")+ cNumStrStr.str(), 
				          vMHe->GetLogicalVolume(), false, copyNumberHere, fCheckVolumeOverLapWC);
        posTmp[2] += fTargetAlignRingSpacing;      
        copyNumber++; copyNumberHere++; 
        if ( copyNumber == fMaxNumAlignRings) break;
      }
    }
    // Deal with the first, split, target segment. 				  
    Create("Horn1TargetSegmentFirst");
    G4PVPlacement *vTargSegFirst = PlaceFinal("Horn1TargetSegmentFirst", vMHe);
    Create("Horn1TargetFinVertFirst");
    LBNEVolumePlacementData * plCoolingTubeFirst = Create("Horn1TargetCoolingTubeFirst");
    Create("Horn1TargetCoolingTubeFirstWater");
    std::map<G4String, LBNEVolumePlacementData>::iterator itTargSegFirst = fSubVolumes.find(G4String("Horn1TargetSegmentFirst"));
    LBNEVolumePlacementData &infoTargSegFirst = itTargSegFirst->second;    
    posTmp[0] = 0.; posTmp[1] = fTargetFinHeight/2.; posTmp[2] = 0.;			  
    G4PVPlacement((G4RotationMatrix *) 0, 
	                            posTmp, plCoolingTubeFirst->fCurrent, 
				    G4String("Horn1TargetSegmentCoolingTubeFirst_PTop"), 
				          vTargSegFirst->GetLogicalVolume(), false, 0, fCheckVolumeOverLapWC);
    posTmp[1] = -1.0*fTargetFinHeight/2.;			  
    G4PVPlacement *vTubeDown =  new G4PVPlacement((G4RotationMatrix *) 0, 
	                            posTmp, plCoolingTubeFirst->fCurrent, 
				    G4String("Horn1TargetSegmentCoolingTubeFirst_PBottom"), 
				          vTargSegFirst->GetLogicalVolume(), false, 0, fCheckVolumeOverLapWC);
    PlaceFinal("Horn1TargetCoolingTubeFirstWater", vTubeDown);					  
    PlaceFinal("Horn1TargetFinVertFirst", vTargSegFirst);
    // Now place the reviously defined standard target segment. Note: they already contain their cooling and 
    // real target. 
//    LBNEVolumePlacementData  *plHorn1TargetSegment = Create("Horn1TargetSegment");
    std::map<G4String, LBNEVolumePlacementData>::iterator itTargSeg = fSubVolumes.find(G4String("TargetUpstrDownstrSegment"));
    LBNEVolumePlacementData  *plTargetUpstrDownstrSegment= &itTargSeg->second;    
    
    double zCoordTmp = infoTargSegFirst.fPosition[2] + infoTargSegFirst.fParams[2]/2. + 
                       plTargetUpstrDownstrSegment->fParams[2]/2. + 0.002*mm;	       
    for (int iSeg=0; iSeg != fTargetNumFinsInHorn; iSeg++) { // Place with no misalignment
      posTmp[0] = 0.; posTmp[1] = 0.;
      posTmp[2] =  zCoordTmp;
      std::cerr << " In Horn1, Positioning target segment " << iSeg  << " at Z = zCoord " << zCoordTmp << std::endl;
      std::ostringstream cNumStrStr; cNumStrStr << "_P" << iSeg;
         new G4PVPlacement((G4RotationMatrix *) 0, 
				  posTmp, plTargetUpstrDownstrSegment->fCurrent, 
				    G4String("Horn1TargetSegment")+cNumStrStr.str(), 
					vMHe->GetLogicalVolume(), false, iSeg+100, fCheckVolumeOverLapWC);
      zCoordTmp += (fTargetFinLength + fTargetFinSpacingLength);
    }
    // Now the end 
    LBNEVolumePlacementData *plCoolingTubeReturn = Create("Horn1TargetCoolingTubeHLast");
    posTmp[0] = plCoolingTubeReturn->fPosition[0]; posTmp[1] = fTargetFinHeight/2.; 
    posTmp[2] = plCoolingTubeReturn->fPosition[2];			  
    G4PVPlacement(&plCoolingTubeReturn->fRotation, 
	                            posTmp, plCoolingTubeReturn->fCurrent, 
				    G4String("Horn1TargetCoolingTubeHLast_PTop"), 
				          vMHe->GetLogicalVolume(), false, 0, fCheckVolumeOverLapWC);
    posTmp[1] = -1.0*fTargetFinHeight/2.;			  
    vTubeDown =  new G4PVPlacement(&plCoolingTubeReturn->fRotation, 
	                            posTmp, plCoolingTubeReturn->fCurrent, 
				    G4String("Horn1TargetCoolingTubeHLast_PTop"), 
				          vMHe->GetLogicalVolume(), false, 0, fCheckVolumeOverLapWC);
    
    Create("Horn1TargetCoolingTubeHLastWater");
    PlaceFinal("Horn1TargetCoolingTubeHLastWater", vTubeDown);
    Create("Horn1TargetCoolingTubeVLast");
    G4PVPlacement *vCL = PlaceFinal("Horn1TargetCoolingTubeVLast", vMHe);					  				  
    Create("Horn1TargetCoolingTubeVLastWater");
    PlaceFinal("Horn1TargetCoolingTubeVLastWater", vCL);					  				  
    Create("Horn1TargetDownstrHeContainerCap");
    PlaceFinal("Horn1TargetDownstrHeContainerCap", vMHe);					  				  
}
//
// Horn1 calls and utilities. 
//

void LBNEVolumePlacements::RescaleHorn1Lengthwise() {

    fHorn1Length  *= fHorn1LongRescale; 
    fHorn1IOTransLength *= fHorn1LongRescale;
     
   for (size_t k=0; k != fHorn1UpstrLengths.size(); ++k) 
      fHorn1UpstrLengths[k] *=  fHorn1LongRescale; 
   for (size_t k=0; k != fHorn1UpstrZPositions.size(); ++k) 
      fHorn1UpstrZPositions[k] *=  fHorn1LongRescale; 
   
  for (size_t k=0; k != fHorn1UpstrOuterIOTransLengths.size(); ++k)
      fHorn1UpstrOuterIOTransLengths[k] *= fHorn1LongRescale; 
      
   for (size_t k=0; k != fHorn1UpstrOuterIOTransPositions.size(); ++k)
      fHorn1UpstrOuterIOTransPositions[k] *= fHorn1LongRescale; 
      
  for (size_t k=0; k != fTargetHorn1Lengths.size(); ++k) 
      fTargetHorn1Lengths[k] *= fHorn1LongRescale;
   for (size_t k=0; k != fTargetHorn1ZPositions.size(); ++k) 
      fTargetHorn1ZPositions[k] *= fHorn1LongRescale;  // This will be approximate...
      
  fHorn1TopUpstrLength *= fHorn1LongRescale;
  fHorn1TopDownstrLength *= fHorn1LongRescale;
  
  // Now we deal with the inner conductor radial equations.
  for (std::vector<LBNEHornRadialEquation>::iterator it=fHorn1Equations.begin();
        it != fHorn1Equations.end(); it++) it->SetLongRescaleFactor(fHorn1LongRescale);
	
  fHorn1NeckLength *= fHorn1LongRescale;
  fHorn1NeckZPosition *= fHorn1LongRescale;
  
//   for (size_t k=0; k != fHorn1SectionLengths.size(); ++k) 
//       fHorn1SectionLengths[k] *= fHorn1LongRescale;  
//  fHorn1OuterTubeInnerLength  *= fHorn1LongRescale; // ??????????????///
  fHorn1OuterConnectorLength *= fHorn1LongRescale;
  fHorn1InnerConnectorLength *= fHorn1LongRescale;
//  fHorn1InnerTopUpstrLength *= fHorn1LongRescale;  // ??????????? Check that all the rescale is consistently... 
//  fHorn1InnerTopDownstrLength  *= fHorn1LongRescale;
  
  
}

void LBNEVolumePlacements::RescaleHorn1Radially() { // Note: not all variables are rescale here, 
  // Some of this work will be done via the LBNEHornRadialEquation
  fHorn1IOTransInnerRad *= fHorn1RadialRescale;
  
  for (size_t k=0; k != fHorn1UpstrOuterIOTransInnerRads.size(); ++k)
      fHorn1UpstrOuterIOTransInnerRads[k] *= fHorn1RadialRescale; 
  for (size_t k=0; k != fHorn1UpstrOuterIOTransThicks.size(); ++k)
      fHorn1UpstrOuterIOTransThicks[k] *= fHorn1RadialRescale; 
      
  for (size_t k=0; k != fTargetHorn1InnerRadsUpstr.size(); ++k) 
      fTargetHorn1InnerRadsUpstr[k] *= fHorn1RadialRescale;
  for (size_t k=0; k != fTargetHorn1InnerRadsUpstr.size(); ++k) 
      fTargetHorn1InnerRadsUpstr[k] *= fHorn1RadialRescale;
  for (size_t k=0; k != fTargetHorn1InnerRadsDownstr.size(); ++k) 
      fTargetHorn1InnerRadsDownstr[k] *= fHorn1RadialRescale;

  for (size_t k=0; k != fHorn1UpstrInnerRadsUpstr.size(); ++k) 
      fHorn1UpstrInnerRadsUpstr[k] *= fHorn1RadialRescale;
  for (size_t k=0; k != fHorn1UpstrInnerRadsDownstr.size(); ++k) 
      fHorn1UpstrInnerRadsDownstr[k] *= fHorn1RadialRescale;
   
  for (size_t k=0; k != fHorn1UpstrInnerRadsOuterUpstr.size(); ++k)
    fHorn1UpstrInnerRadsOuterUpstr[k] *= fHorn1RadialRescale;
  for (size_t k=0; k != fHorn1UpstrInnerRadsOuterDownstr.size(); ++k)
    fHorn1UpstrInnerRadsOuterDownstr[k] *= fHorn1RadialRescale;
  
//  fHorn1TopUpstrInnerRad  *= fHorn1RadialRescale;  Recomputed in CreateHorn1TopLevelUpstr

  fHorn1OuterTubeInnerRad  *= fHorn1RadialRescale;; 
  fHorn1OuterTubeOuterRad  *= fHorn1RadialRescale;; 
  fHorn1TopUpstrOuterRad  *= fHorn1RadialRescale;
  
  fHorn1TopDownstrOuterRad  *= fHorn1RadialRescale; 
  // Now we deal with the inner conductor radial equations.
  for (std::vector<LBNEHornRadialEquation>::iterator it=fHorn1Equations.begin();
        it != fHorn1Equations.end(); it++) it->SetRadialRescaleFactor(fHorn1RadialRescale);
  
}  

void LBNEVolumePlacements::DeclareHorn1Dims() {
  
  const double in = 2.54*cm;
  fHorn1IOTransLength = 3.0*cm + 3.316*in + 0.005*mm; 
                          // Drawing 8875.112 -MD-363097 The 3 cm is the MCZERO offset, per verbal discussion 
                              // with J. Hylen. The 5 microns if to avoid G4 volume overlaps.  
			      
  fHorn1RadialSafetyMargin = 2.9*mm; // per agreement between Jim H. and Alberto M., Aug. 22 2013. 
  
  fHorn1IOTransInnerRad = 2.520*in/2. - fHorn1RadialSafetyMargin/2. ; // last term is the 
  fHorn1IOTransOuterRad = 16.250*in/2.;
  
  fHorn1UpstrInnerRadsUpstr.resize(4);
  fHorn1UpstrInnerRadsDownstr.resize(4);
  fHorn1UpstrInnerRadsOuterUpstr.resize(4);
  fHorn1UpstrInnerRadsOuterDownstr.resize(4);
  fHorn1UpstrLengths.resize(4);
  fHorn1UpstrZPositions.resize(4);
  
  fHorn1UpstrInnerRadsUpstr[0] = 1.572*in; 
  fHorn1UpstrInnerRadsOuterUpstr[0] = fHorn1UpstrInnerRadsUpstr[0] + 0.32*in; 
  fHorn1UpstrInnerRadsDownstr[0] = 1.41*in; 
  fHorn1UpstrInnerRadsOuterDownstr[0] = fHorn1UpstrInnerRadsDownstr[0] + 0.32*in; 
  fHorn1UpstrLengths[0] = 0.508*in - 0.010*mm;
  fHorn1UpstrZPositions[0] = fHorn1UpstrLengths[0]/2.; // With respect to the beginning of mother volume.  
  
  fHorn1UpstrInnerRadsUpstr[1] = fHorn1UpstrInnerRadsDownstr[0]; 
  fHorn1UpstrInnerRadsOuterUpstr[1] = fHorn1UpstrInnerRadsUpstr[1] + 0.32*in; 
  fHorn1UpstrInnerRadsDownstr[1] = 1.288*in; 
  fHorn1UpstrInnerRadsOuterDownstr[1] = fHorn1UpstrInnerRadsDownstr[1] + 0.3*in; 
  fHorn1UpstrLengths[1] = 0.639*in  - 0.010*mm;
  fHorn1UpstrZPositions[1] = fHorn1UpstrZPositions[0] + 0.005*mm + fHorn1UpstrLengths[0]/2 + fHorn1UpstrLengths[1]/2.;
  
  fHorn1UpstrInnerRadsUpstr[2] = fHorn1UpstrInnerRadsDownstr[1]; 
  fHorn1UpstrInnerRadsOuterUpstr[2] = fHorn1UpstrInnerRadsUpstr[2] + 0.28*in; 
  fHorn1UpstrInnerRadsDownstr[2] = 1.268*in; 
  fHorn1UpstrInnerRadsOuterDownstr[2] = fHorn1UpstrInnerRadsDownstr[2] + 0.118*in; 
  fHorn1UpstrLengths[2] = 2.835*in  - 0.010*mm; // Reduce a bit, too tight..
  fHorn1UpstrZPositions[2] = fHorn1UpstrZPositions[1] + 0.010*mm + fHorn1UpstrLengths[1]/2 + fHorn1UpstrLengths[2]/2.;
  
  fHorn1UpstrInnerRadsUpstr[3] = fHorn1UpstrInnerRadsDownstr[2]; 
  fHorn1UpstrInnerRadsOuterUpstr[2] = fHorn1UpstrInnerRadsUpstr[2] + 0.118*in; 
  fHorn1UpstrInnerRadsDownstr[3] =  fHorn1UpstrInnerRadsUpstr[3]; 
  fHorn1UpstrInnerRadsOuterDownstr[3] = fHorn1UpstrInnerRadsDownstr[3] + 0.20*in; 
  fHorn1UpstrLengths[3] = 0.40*in - 0.010*mm;
  fHorn1UpstrZPositions[3] = fHorn1UpstrZPositions[2] + 0.010*mm + fHorn1UpstrLengths[2]/2 + fHorn1UpstrLengths[3]/2.;
 //
 // These are elements, approximated as tubes, of the Inner Outer transition piece of Horn1 
 // 
  fHorn1UpstrOuterIOTransInnerRads.resize(4);
  fHorn1UpstrOuterIOTransThicks.resize(4);
  fHorn1UpstrOuterIOTransLengths.resize(4);
  fHorn1UpstrOuterIOTransPositions.resize(4);
  
  fHorn1UpstrOuterIOTransInnerRads[0] = 5.625*in;
  fHorn1UpstrOuterIOTransThicks[0] = 0.30*in;
  fHorn1UpstrOuterIOTransLengths[0] = 0.421*in;
  fHorn1UpstrOuterIOTransPositions[0] = fHorn1UpstrOuterIOTransLengths[0]/2. + 0.005*mm;

  fHorn1UpstrOuterIOTransInnerRads[1] = 5.763*in;
  fHorn1UpstrOuterIOTransThicks[1] = 0.32*in;
  fHorn1UpstrOuterIOTransLengths[1] = 0.421*in;
  fHorn1UpstrOuterIOTransPositions[1] = 
  fHorn1UpstrOuterIOTransLengths[0]+ fHorn1UpstrOuterIOTransLengths[1]/2. + 0.015*mm;
  
  fHorn1UpstrOuterIOTransInnerRads[2] = 5.763*in;
  fHorn1UpstrOuterIOTransThicks[2] = 0.4*in;
  fHorn1UpstrOuterIOTransLengths[2] = 0.75*in;
  fHorn1UpstrOuterIOTransPositions[2] = fHorn1UpstrOuterIOTransLengths[0] +
  fHorn1UpstrOuterIOTransLengths[1] + fHorn1UpstrOuterIOTransLengths[2]/2. + 0.050*mm;

  fHorn1UpstrOuterIOTransInnerRads[3] = 5.763*in;
  fHorn1UpstrOuterIOTransThicks[3] = 2.26*in;
  fHorn1UpstrOuterIOTransLengths[3] = fHorn1IOTransLength - 0.250*mm - 
    fHorn1UpstrOuterIOTransLengths[0] - fHorn1UpstrOuterIOTransLengths[1] - fHorn1UpstrOuterIOTransLengths[2];
    
  fHorn1UpstrOuterIOTransPositions[3] = fHorn1IOTransLength - fHorn1UpstrOuterIOTransLengths[3]/2. - 0.01*mm;;

// Outer dimension of the big tube that contains almost everything.. 
// Except the big connedtors rings downsream (max dim are 23.5 in 
     
  fHorn1OuterTubeOuterRad =  13.750*in/2.0;
  fHorn1OuterTubeInnerRad =  11.750*in/2.0; // checked with drawing 396094
 // 

  //
 // Now the top level sections. Only two of them, alignable. 
   fHorn1TopUpstrOuterRad = 16.250*in/2. +  5.0*mm; // safety 5 mm, big radius..// Drawing 8875.112-MD 363097
   fHorn1NeckLength = 1.568*in; // Drawing 8875.112-MD  363105
   fHorn1NeckZPosition = 30.135*in + fHorn1NeckLength/2.;  // Drawing 8875.112-MD  363105
      // In Z Drawing coordinates. 

   fHorn1TopDownstrLength  = 133.752*in; // We will have to subtract the upstream length, once we know it 
   // This include the downstream bulkhead. 
  // Other quantities defined in CreateHorn1TopLevelUpstr..
 // The first one goes up the downstream end of the IO transition peace 
 // and up the downstream end of the target. If the target is completly out of the horn, 
 // it is the part before the neck. (Drawing 8875.112-MD-363104)
 //  This is a tube, set by the inner conductor
 // radius at the end of the target.  
 // 
 // Load the equations Drawing 8875.112-MD 363104, 363105
//
  fHorn1Equations.clear();
  LBNEHornRadialEquation e1(1.975805, -0.055858, -0.078740); fHorn1Equations.push_back(e1);
  LBNEHornRadialEquation e2(1.818869, -0.055858, 0.); fHorn1Equations.push_back(e2);
  LBNEHornRadialEquation eBlank(0., 0.,  0.); fHorn1Equations.push_back(eBlank); // equation 3 not found on drawing. 
  LBNEHornRadialEquation e4(-5.619190, 0.180183, 0.); fHorn1Equations.push_back(e4); 
  LBNEHornRadialEquation e5(-5.462253, 0.180183, -0.078740); fHorn1Equations.push_back(e5); 
  LBNEHornRadialEquation e6(1.97805, -0.055858, 0.); fHorn1Equations.push_back(e6); 
  fHorn1Equations.push_back(eBlank); // equation 7 not found on drawing as well 
  LBNEHornRadialEquation e8(-5.462253, 0.180183, 0.); fHorn1Equations.push_back(e8); 
 
 // The 2nd top lelvel section is the remaining part of that part, up to the end of the Horn1Hall Z location. 
 // This is just a tube. 
 //
    
 // Then, conical sections, all in a tube that has an inner radius of the neck, our radius 
 // 
 // To compute all that, we need to locate the target first.. 
 // So, done at construction time. 
 //
  
}


void LBNEVolumePlacements::PlaceFinalHorn1(G4PVPlacement *mother, G4PVPlacement *motherDownstrTarget) {

// 
// Start with upstream Inner to Out transition. This one is surveyable (as well as optionally rescaled. )
//
   const double in = 2.54*cm;
//
   std::cerr << " Start testing PlaceFinalHorn1, mother Logical volume name " << mother->GetLogicalVolume()->GetName() << std::endl;
   fHorn1Equations[0].test1(); // this supposed to work 
   std::cerr << " Test 1 passed " << std::endl;
//   fHorn1Equations[3].test1(); // This supposed to fail... 
   LBNEVolumePlacementData *plTrUpst = this->Create("Horn1IOTransCont");
   G4PVPlacement *vTrUpst = this->PlaceFinal("Horn1IOTransCont", motherDownstrTarget);
   //
   // These sub-volumes will be never misaligned with respect to each other, or the container volume 
   // above.  So skip the VolumePlacement utilities. Code bloat here instead in the Create method.. Oh well.. 
   //
   for (size_t k=0; k!=fHorn1UpstrLengths.size(); k++) {
     std::ostringstream nameStrStr; nameStrStr << "Horn1IOTransInnerPart" << k;
     G4String nameStr(nameStrStr.str());
     G4Cons *aCons = new G4Cons(nameStr, fHorn1UpstrInnerRadsUpstr[k],fHorn1UpstrInnerRadsOuterUpstr[k],
                                         fHorn1UpstrInnerRadsDownstr[k],fHorn1UpstrInnerRadsOuterDownstr[k],
	                              fHorn1UpstrLengths[k]/2., 0., 360.0*deg);
     G4LogicalVolume *pCurrent = new G4LogicalVolume(aCons, G4Material::GetMaterial(std::string("Air")), nameStr);
     G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.;  
     posTmp[2] = -1.0*(plTrUpst->fParams[2])/2. + fHorn1UpstrZPositions[k];			      
     new G4PVPlacement(	(G4RotationMatrix *) 0,	posTmp, pCurrent, nameStr + std::string("_P"), 
                        vTrUpst->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	      
   }
   
   for (size_t k=0; k!= fHorn1UpstrOuterIOTransInnerRads.size(); k++) {
     std::ostringstream nameStrStr; nameStrStr << "Horn1IOTransOuterPart" << k;
     G4String nameStr(nameStrStr.str());
     G4Tubs *aTubs = new G4Tubs(nameStr, fHorn1UpstrOuterIOTransInnerRads[k],
                                          fHorn1UpstrOuterIOTransInnerRads[k] + fHorn1UpstrOuterIOTransThicks[k],
	                              fHorn1UpstrOuterIOTransLengths[k]/2., 0., 360.0*deg);
     G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.;  
     posTmp[2] = -1.0*(plTrUpst->fParams[2])/2. + fHorn1UpstrOuterIOTransPositions[k];			      
     G4LogicalVolume *pCurrent = new G4LogicalVolume(aTubs, G4Material::GetMaterial(std::string("Air")), nameStr);
     new G4PVPlacement(	(G4RotationMatrix *) 0,	posTmp, pCurrent, nameStr + std::string("_P"), 
                        vTrUpst->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	      
   }

   // O.K. Now the part of the Horn1 that surrounds the target. 
   // We already defined this mother volume. It is the infamous volume Horn1TopLevelUpstr
   // It is a cylinder (inner radius 0. ), or radius encompassing the inner and outer conductors. 
   
   G4PVPlacement *vUpst = motherDownstrTarget;
   const LBNEVolumePlacementData *plHUpst = Find(G4String("InnerConductors"), "Horn1TopLevelUpstr", G4String("Create"));
  
// Start by checking possible radial overlap at the downstream end of the target. 
// The size and position of Horn1TopLevelUpstr and Horn1TargetDownstrHeContainer

  const double zACRNT1Shift = 3.316*in*fHorn1LongRescale; //  Drawing 8875.112-MD-363097

  this->CheckHorn1InnerConductAndTargetRadii();
   
  const double lengthInnerConductUpstr = fHorn1TopUpstrLength- fHorn1IOTransLength - 0.1*mm; // 100 microns safety margin 
  int numSubSect = GetNumberOfInnerHornSubSections(0, 0., lengthInnerConductUpstr, 10);
   // Fill with it one or more inner conductor conical section
   // We require a precision of 5 microns in the radius. 
   const double deltaZ = lengthInnerConductUpstr/numSubSect;
   for (int iSub=0; iSub != numSubSect; iSub++) { 
     //
      const double zzBegin = zACRNT1Shift + (iSub*deltaZ); // from the 
      const double zzEnd = zzBegin + deltaZ;
     std::ostringstream nameStrStr; nameStrStr << "Horn1UpstrSubSect" << iSub;
     G4String nameStr(nameStrStr.str());
     const double rMin1 = fHorn1Equations[0].GetVal(zzBegin); // Equation 1
     const double rMin2 = fHorn1Equations[0].GetVal(zzEnd);
     const double rMax1 = fHorn1Equations[5].GetVal(zzBegin) + fWaterLayerThickInHorns + 0.0025; 
       // Equation 6 (Drawing 8875.112-MD 363104)
     const double rMax2 = fHorn1Equations[5].GetVal(zzEnd) + fWaterLayerThickInHorns + 0.0025;     
     G4Cons *aCons = new G4Cons(nameStr, rMin1, rMax1,rMin2, rMax2,
	                              (deltaZ - 0.005*mm)/2., 0., 360.0*deg);
     G4LogicalVolume *pCurrent = new G4LogicalVolume(aCons, G4Material::GetMaterial(std::string("Aluminum")), nameStr);
     G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.;
     // plHUpst constain part of the Inner Outer Transition. Shift downtream by it's length 
     posTmp[2] = -1.0*(plHUpst->fParams[2])/2. + fHorn1IOTransLength + deltaZ/2. + iSub*deltaZ;			      
     G4PVPlacement *vSub = new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
                        vUpst->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	
			
    if (fWaterLayerThickInHorns	> 0.002*mm) {
     std::ostringstream nameStrStr; nameStrStr << "Horn1UpstrSubSect" << iSub << "Water";
     G4String nameStr(nameStrStr.str());
     G4Cons *aCons = new G4Cons(nameStr, rMax1 - fWaterLayerThickInHorns, rMax1-0.001*mm,
                                         rMax2 - fWaterLayerThickInHorns, rMax2-0.001*mm,
	                              (deltaZ - 0.0075*mm)/2., 0., 360.0*deg);
     G4LogicalVolume *pCurrent = new G4LogicalVolume(aCons, G4Material::GetMaterial(std::string("Water")), nameStr);
     G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.; posTmp[2] =0.;			      
     new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
                        vSub->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	
    }
   }
    // on the number of subsections for the inner conductor, for the upstream part of Horn1 
   // Now add the welding joint between the most upstream part of the inner conductor and the Inner Outer transition section
   // Drawing Drawing 8875.112-MD 363104
   {
     G4String nameStr("Horn1UpstrSubSect0WeldUpstr");
     const double length = 12.0*mm; // Make it a bit shorter, it is rounded... 
     const double rTmp1 = fHorn1Equations[5].GetVal( zACRNT1Shift + 15.0*mm - length/2.) 
                              + 0.02*mm + fWaterLayerThickInHorns;
        // place it a little more detached..Also, the weld is on top of the layer of water.. Oh well.. 
      const double rTmp2 = rTmp1 + 1.8*mm; // 
     G4Tubs *aTubs = new G4Tubs(nameStr, rTmp1, rTmp2, 
	                           length/2.   , 0., 360.0*deg);
     G4LogicalVolume *pCurrent = new G4LogicalVolume(aTubs, G4Material::GetMaterial(std::string("Aluminum")), nameStr);
     G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.;
     posTmp[2] = -1.0*(plHUpst->fParams[2])/2. + fHorn1IOTransLength + length/2. + 15.0*mm;
    new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
                        vUpst->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	
   
   }
   // Now place the first support ring, and the spider hang, if room.
   const double lengthHangerRing = fHorn1LongRescale*0.750*in;
   const double zLocFirstHanger = fHorn1IOTransLength + fHorn1LongRescale*(17.876 - 1.385)*in - lengthHangerRing/2.;
   // With respect to the starting point of Horn1TopLevelUpstr 
   std::cerr << " zLocFirstHanger " << zLocFirstHanger << std::endl;
   const bool firstHangerinUpstrSect = (zLocFirstHanger < fHorn1TopUpstrLength);
     //Must have room for the whole piece.. Move it to the left if need be 
   const double zLocTweakedFirstHanger = std::min(zLocFirstHanger, 
                           (fHorn1TopUpstrLength - (0.375*in - 0.512*in)*fHorn1LongRescale - 3.0*mm));
			   // the 0.375 is the length if hanger ring, 0.512 the space for weld ant the end. 
			   
   std::cerr << " zLocTweakedFirstHanger " << zLocTweakedFirstHanger << std::endl;
   if (firstHangerinUpstrSect) std::cerr << " First Spider Hanger is in Horn1Upstr section " << std::endl;
   if (!firstHangerinUpstrSect) std::cerr << " First Spider Hanger is in Horn1Downstr section " << std::endl;
   
   G4String nameStrFirstHanger("Horn1UpstrSpiderHanger");
   if ( firstHangerinUpstrSect ) {
	const double zPosCenterMotherVolume = -1.0*(plHUpst->fParams[2])/2. + zLocTweakedFirstHanger + lengthHangerRing/2. ;  			   
	const double zPosCenterDrawingCoord = zLocTweakedFirstHanger - 3.0*cm*fHorn1LongRescale; 			   
        this->Horn1InstallSpiderHanger(nameStrFirstHanger, zPosCenterDrawingCoord, 
	                                    zPosCenterMotherVolume, plHUpst, vUpst );			       
   }
   // Outer tube 
   G4String nameStr("Horn1UpstrOutrTube");
   const double lengthOutT =  fHorn1TopUpstrLength - fHorn1IOTransLength -  1.0*mm;; // Make it a bit shorter, it is rounded... 
   G4Tubs *aTubs = new G4Tubs(nameStr, fHorn1OuterTubeInnerRad, 
                                        fHorn1OuterTubeOuterRad, lengthOutT/2.   , 0., 360.0*deg);
    G4LogicalVolume *pCurrent = new G4LogicalVolume(aTubs, G4Material::GetMaterial(std::string("Aluminum")), nameStr);
    G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.;
    posTmp[2] = -1.0*(plHUpst->fParams[2])/2. + fHorn1IOTransLength + lengthOutT/2. + 0.25*mm;			      
    new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
                        vUpst->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);
				
   // Outer Tube Upstream flange. See drawing  363094
   G4String nameStrOutFlUpstr("Horn1UpstrOutrTubeFlange");
   const double lengthOutFlUpstr = 1.0*in; //Not cleanly shown on drawing 363094 
   const double rTmpOutFlUpstrInner = fHorn1OuterTubeOuterRad + 0.1*mm;
   const double rTmpOutFlUpstrOuter = rTmpOutFlUpstrInner + 2.5*in; // Still a guess.. Probably a bit oversized.  
   aTubs = new G4Tubs(nameStrOutFlUpstr,  rTmpOutFlUpstrInner, rTmpOutFlUpstrOuter, lengthOutFlUpstr/2.0, 0., 360.0*deg);
   pCurrent = new G4LogicalVolume(aTubs, G4Material::GetMaterial(std::string("Aluminum")), nameStrOutFlUpstr);
   posTmp[0] = 0.; posTmp[1] = 0.;
   posTmp[2] = -1.0*(plHUpst->fParams[2])/2. + fHorn1IOTransLength + lengthOutT/2. + 1.0*mm;			      
   new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStrOutFlUpstr + std::string("_P"), 
                        vUpst->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);
			//
   // 
   // Now the dowstream section. 				
   //
   fHorn1TopDownstrLength -= (fHorn1TopUpstrLength - fHorn1IOTransLength) - 0.010*mm;
   LBNEVolumePlacementData *plHDwn = this->Create("Horn1TopLevelDownstr");
   G4PVPlacement *vDown = this->PlaceFinal("Horn1TopLevelDownstr", mother);
   //
   // Used later... ZCoordinate translation from ZDrawing and the center of Horn1TopLevelDownstr
   // Coordinate of the end of target container (that sets the length of the upstream section) and ZDrawing = 0. 
   const double zUpstreamCrazy3cm =  fHorn1TopUpstrLength -3.0*cm*fHorn1LongRescale;
   std::cerr << " zUpstreamCrazy3cm " << zUpstreamCrazy3cm << std::endl;
   // Used later... ZCoordinate translation from ZDrawing and the center of Horn1TopLevelDownstr
   const double zShiftDrawingDownstr = -1.0*(plHDwn->fParams[2])/2. - zUpstreamCrazy3cm;
   
   const double lengthToTheNeck = fHorn1NeckZPosition - 
                          (fHorn1TopUpstrLength - 3.0*cm*fHorn1LongRescale)  - fHorn1NeckLength/2.;
			  // Stupid 3 cm coordinate shift. 
  if (lengthToTheNeck < 0.) {
    std::ostringstream messStrStr; 
    messStrStr << "Inconsistent length to the neck of" << lengthToTheNeck << " Neck Position (drawing) "
               << fHorn1NeckZPosition << " fHorn1TopUpstrLength " <<  fHorn1TopUpstrLength;
    G4String messStr(messStrStr.str());
    G4Exception("LBNEVolumePlacements::PlaceFinalHorn1", " ",  FatalErrorInArgument, messStr.c_str());
  }
  //
  // Install the length of inner conductor, from downstream end of the target to the neck 
  //
  
  {
     int numSubSect = GetNumberOfInnerHornSubSections(0, zUpstreamCrazy3cm, 
                                                      fHorn1NeckZPosition, 10); // These Z position are from the start of the inner conductor.   
     const double deltaZ = lengthToTheNeck/numSubSect;
     for (int iSub = 0; iSub != numSubSect; iSub++) {					      
       const double zzBegin = zUpstreamCrazy3cm + iSub*deltaZ;
       const double zzEnd = zzBegin + deltaZ;
       std::ostringstream nameStrStr; nameStrStr << "Horn1DownstrPart0SubSect" << iSub;
       G4String nameStr(nameStrStr.str());
       const double rMin1 = fHorn1Equations[0].GetVal(zzBegin); // Equation 1
       const double rMin2 = fHorn1Equations[0].GetVal(zzEnd);
       const double rMax1 = fHorn1Equations[5].GetVal(zzBegin) + fWaterLayerThickInHorns + 0.0025; 
       // Equation 6 (Drawing 8875.112-MD 363104)
       const double rMax2 = fHorn1Equations[5].GetVal(zzEnd) + fWaterLayerThickInHorns + 0.0025;     
       G4Cons *aCons = new G4Cons(nameStr, rMin1, rMax1,rMin2, rMax2,
	                              (deltaZ - 0.005*mm)/2., 0., 360.0*deg);
       G4LogicalVolume *pCurrent = new G4LogicalVolume(aCons, G4Material::GetMaterial(std::string("Aluminum")), nameStr);
       G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.;
       posTmp[2] = -1.0*(plHDwn->fParams[2])/2. + iSub*deltaZ + deltaZ/2.;
       // The above is what I want. At least I think I do. A more succint way, for downstream volumes could be:
//	double zPosAlt = zzBegin + zShiftDrawingDownstr + deltaZ/2.; // Nicer form 
//       zPosAlt ?= posTmp[2] = zzBegin	+ zShiftDrawingDownstr + deltaZ/2.;
//       zShiftDrawingDownstr = posTmp[2] - zzBegin - deltaZ/2.	;	      
//       zShiftDrawingDownstr = -1.0*(plHDwn->fParams[2])/2. + iSub*deltaZ + deltaZ/2.  - zzBegin - deltaZ/2.	;	      
//       zShiftDrawingDownstr = -1.0*(plHDwn->fParams[2])/2. - zzBegin + iSub*deltaZ;
//       zShiftDrawingDownstr = -1.0*(plHDwn->fParams[2])/2. - zUpstreamCrazy3cm ;
	double zPosAlt = zzBegin + zShiftDrawingDownstr + deltaZ/2.;
	//
	// Check the silly 1D equations above.. 
	if (std::abs( zPosAlt - posTmp[2]) > 0.010) {
	  std::cerr << " Crap !" << std::endl; 
	  
	  exit(2);
	}  
       G4PVPlacement *vSub = new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
                        vDown->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	
			
      if (fWaterLayerThickInHorns	> 0.002*mm) {
       std::ostringstream nameStrStr; nameStrStr << "Horn1UpstrSubSect" << iSub << "Water";
       G4String nameStr(nameStrStr.str());
       G4Cons *aCons = new G4Cons(nameStr, rMax1 - fWaterLayerThickInHorns, rMax1-0.001*mm,
                                         rMax2 - fWaterLayerThickInHorns, rMax2-0.001*mm,
	                              (deltaZ - 0.0075*mm)/2., 0., 360.0*deg);
       G4LogicalVolume *pCurrent = new G4LogicalVolume(aCons, G4Material::GetMaterial(std::string("Water")), nameStr);
       G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.; posTmp[2] =0.;			      
       new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
                        vSub->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	
      }
    } // of the number of subsection to the neck 
    // Place the hanger, if not already done 
    if (!firstHangerinUpstrSect) { // Unlikely possiblity... Not Checked yet !!!!! 
     //Must have room for the whole piece.. Move it to the right if need be 
      G4String nameStrFirstHanger("Horn1DownstrFirstSpiderHanger");
       const double zLocDrawing = fHorn1TopUpstrLength - 3.0*in*fHorn1LongRescale 
                                 + lengthToTheNeck + 0.3750*in*fHorn1LongRescale;
       const double zLocPosM = -plHDwn->fParams[2]/2 + lengthToTheNeck + 0.3750*in*fHorn1LongRescale; // with respect to the center of 
       // of the mother volume. 
       this->Horn1InstallSpiderHanger( nameStrFirstHanger, zLocDrawing, zLocPosM,  plHDwn, vDown); 
    }
    // The first weld for this section. 
   {
     G4String nameStr("Horn1DownstrSubSect1Weld0");
     G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.;
     const double length = 24.0*mm; //Cover two real sections... 
     double zW = 21.0888*in*fHorn1LongRescale + length/2;
     posTmp[2] =  zW + zShiftDrawingDownstr + length/2.;
     if (posTmp[2] < -1.0*(plHDwn->fParams[2])/2.) {
        const double wrongZ = posTmp[2];
	zW = zUpstreamCrazy3cm + length	+ 2.0*mm;     
	posTmp[2] =-1.0*(plHDwn->fParams[2])/2.  + length + 0.1*mm;	     
       std::cerr << " Weld should be placed in the upstream section, but placed in the downstream one.. "
                 << std::endl << " shifted by "  << posTmp[2] - wrongZ << std::endl;
     } 
                    // Assuming it won't have to move because of the hanger ring
                                                   //  Could collide with it. Take nominal position, but to be checked.  
     const double rTmp1 = fHorn1Equations[5].GetVal(zW - length - 1.0*mm) 
                         + 0.015*mm + fWaterLayerThickInHorns;
        // place it a little more detached..The radius is estimated on the upstream side, biggest radius.
      const double rTmp2 = rTmp1 + 1.8*mm; // 
     G4Tubs *aTubs = new G4Tubs(nameStr, rTmp1, rTmp2, length/2.   , 0., 360.0*deg);
     G4LogicalVolume *pCurrent = new G4LogicalVolume(aTubs, G4Material::GetMaterial(std::string("Aluminum")), nameStr);
     new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
                        vDown->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	
   }
  } // From the downstream of the target to the neck. 
   //
   // Now, the neck. Just a tube 
   //
   {
     G4String nameStr("Horn1Neck");
     const double zNeckDrawing = fHorn1LongRescale*(30.3150)*in; //start of the neck.. 
     const double rTmp1 = fHorn1RadialRescale*(0.709*in/2.); // Drawing 8875.112-MD 363105
     const double rTmp2 = fHorn1RadialRescale*(1.063*in/2.); // Drawing 8875.112-MD 363105
     const double length = fHorn1LongRescale*1.5680*in - 0.050*mm; // last term to absord 
        // small shifts in the upstream part.. 
     G4Tubs *aTubs = new G4Tubs(nameStr, rTmp1, rTmp2, 
	                           length/2.   , 0., 360.0*deg);
     G4LogicalVolume *pCurrent = new G4LogicalVolume(aTubs, G4Material::GetMaterial(std::string("Aluminum")), nameStr);
     G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.;
     posTmp[2] = zNeckDrawing  + zShiftDrawingDownstr + length/2. + 0.025*mm;			      
     new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
                        vDown->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);
    }				
    // The downstream part of the real section that has the neck.  
   {
     const double zStartDrawing =  fHorn1LongRescale*31.8827*in;
     const double zEndDrawing = fHorn1LongRescale*(41.0776)*in;
     int numSubSect = GetNumberOfInnerHornSubSections(3, zStartDrawing, 
                                                      zEndDrawing, 10); // These Z position are from the start of the inner conductor.   
     const double deltaZ = (zEndDrawing - zStartDrawing)/numSubSect;
     for (int iSub = 0; iSub != numSubSect; iSub++) {					      
       const double zzBegin = zStartDrawing + iSub*deltaZ;
       const double zzEnd = zzBegin + deltaZ;
       std::ostringstream nameStrStr; nameStrStr << "Horn1DownstrPart1SubSect" << iSub;
       G4String nameStr(nameStrStr.str());
       const double rMin1 = fHorn1Equations[3].GetVal(zzBegin); // Equation 1
       const double rMin2 = fHorn1Equations[3].GetVal(zzEnd);
       const double rMax1 = fHorn1Equations[7].GetVal(zzBegin) + fWaterLayerThickInHorns + 0.0025; 
                 // Equation 6 (Drawing 8875.112-MD 363104)
       const double rMax2 = fHorn1Equations[7].GetVal(zzEnd) + fWaterLayerThickInHorns + 0.0025;     
       G4Cons *aCons = new G4Cons(nameStr, rMin1, rMax1,rMin2, rMax2,
	                              (deltaZ - 0.005*mm)/2., 0., 360.0*deg);
       G4LogicalVolume *pCurrent = new G4LogicalVolume(aCons, G4Material::GetMaterial(std::string("Aluminum")), nameStr);
       G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.;
       posTmp[2] = zzBegin   + zShiftDrawingDownstr + deltaZ/2.;			      
       G4PVPlacement *vSub = new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
                        vDown->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	
			
      if (fWaterLayerThickInHorns > 0.002*mm) {
       std::ostringstream nameStrStr; nameStrStr << "Horn1DownstrPart1SubSect" << iSub << "Water";
       G4String nameStr(nameStrStr.str());
       G4Cons *aCons = new G4Cons(nameStr, rMax1 - fWaterLayerThickInHorns, rMax1-0.001*mm,
                                         rMax2 - fWaterLayerThickInHorns, rMax2-0.001*mm,
	                              (deltaZ - 0.0075*mm)/2., 0., 360.0*deg);
       G4LogicalVolume *pCurrent = new G4LogicalVolume(aCons, G4Material::GetMaterial(std::string("Water")), nameStr);
       G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.; posTmp[2] =0.;			      
       new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
                        vSub->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	
      }
    } // of the number of subsection to the neck
    // The weld at the end  
   {
     G4String nameStr("Horn1DownstrPart1Weld1");
     const double zWW = fHorn1LongRescale*(41.0776)*in;; 
      const double length = 24.0*mm; //Cover two real sections... 
     const double rTmp1 = fHorn1Equations[7].GetVal(zWW + length) + 0.050*mm + fWaterLayerThickInHorns;
        // place it a little more detached..Also, the weld is on top of the layer of water.. Oh well.. 
      const double rTmp2 = rTmp1 + 1.8*mm; // 
     G4Tubs *aTubs = new G4Tubs(nameStr, rTmp1, rTmp2, 
	                           length/2.   , 0., 360.0*deg);
     G4LogicalVolume *pCurrent = new G4LogicalVolume(aTubs, G4Material::GetMaterial(std::string("Aluminum")), nameStr);
     G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.;
     posTmp[2] = zWW  + zShiftDrawingDownstr + length/2.;			      
     new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
                        vDown->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	
   }
  } // The downstream part horn1, starting downstream of the real section that has the neck
  // More Inner conductors, covering the drawings 8875.112-MD 363105 through 363109 included.
  // Radial equation 5 and 8 (indices 4 and 7 in our arrays) 
  // From ZDrawing 41.0576 to 117.126
  {
     const double zStartDrawing =  fHorn1LongRescale*41.108*in;
     const double zEndDrawing = fHorn1LongRescale*117.126*in;
     int numSubSect = GetNumberOfInnerHornSubSections(4, zStartDrawing, 
                                                      zEndDrawing, 10); // These Z position are from the start of the inner conductor.   
     const double deltaZ = (zEndDrawing - zStartDrawing)/numSubSect;
     std::cerr << " Number of subsection for the downstream half of Horn1 " << numSubSect 
                 << " deltaz " << deltaZ << std::endl;
     for (int iSub = 0; iSub != numSubSect; iSub++) {					      
       const double zzBegin = zStartDrawing + iSub*deltaZ;
       const double zzEnd = zzBegin + deltaZ;
       std::ostringstream nameStrStr; nameStrStr << "Horn1DownstrPart1SubSect" << iSub;
       G4String nameStr(nameStrStr.str());
       const double rMin1 = fHorn1Equations[4].GetVal(zzBegin); // Equation 1
       const double rMin2 = fHorn1Equations[4].GetVal(zzEnd);
       const double rMax1 = fHorn1Equations[7].GetVal(zzBegin) + fWaterLayerThickInHorns + 0.0025; 
          // Equation 6 (Drawing 8875.112-MD 363104)
       const double rMax2 = fHorn1Equations[7].GetVal(zzEnd) + fWaterLayerThickInHorns + 0.0025;     
       G4Cons *aCons = new G4Cons(nameStr, rMin1, rMax1,rMin2, rMax2,
	                              (deltaZ - 0.005*mm)/2., 0., 360.0*deg);
       G4LogicalVolume *pCurrent = new G4LogicalVolume(aCons, G4Material::GetMaterial(std::string("Aluminum")), nameStr);
       G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.;
       posTmp[2] = zzBegin  + zShiftDrawingDownstr + deltaZ/2.;			      
       G4PVPlacement *vSub = new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
                        vDown->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	
			
      if (fWaterLayerThickInHorns > 0.002*mm) {
       std::ostringstream nameStrStr; nameStrStr << "Horn1DownstrPart1SubSect" << iSub << "Water";
       G4String nameStr(nameStrStr.str());
       G4Cons *aCons = new G4Cons(nameStr, rMax1 - fWaterLayerThickInHorns, rMax1-0.001*mm,
                                         rMax2 - fWaterLayerThickInHorns, rMax2-0.001*mm,
	                              (deltaZ - 0.0075*mm)/2., 0., 360.0*deg);
       G4LogicalVolume *pCurrent = new G4LogicalVolume(aCons, G4Material::GetMaterial(std::string("Water")), nameStr);
       G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.; posTmp[2] =0.;			      
       new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
                        vSub->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	
      }
    } // of the number of subsection to the neck 
   {
     // Now the Hangers. (two of them.. ) 
     {
       G4String nameStrFirstHanger("Horn1DownstrSecondSpiderHanger");
       double zLocDrawing = zStartDrawing + fHorn1LongRescale*1.416*in;
       double zLocPosM = zLocDrawing + zShiftDrawingDownstr + 0.375*in*fHorn1LongRescale; // with respect to the center of 
       // of the mother volume. 
       this->Horn1InstallSpiderHanger( nameStrFirstHanger, zLocDrawing, zLocPosM,  plHDwn, vDown); 
       
       G4String nameStrSecondHanger("Horn1DownstrThirdSpiderHanger");
       zLocDrawing = fHorn1LongRescale*(80.9951 + 1.791)*in;
       zLocPosM = zLocDrawing + zShiftDrawingDownstr + 0.375*in*fHorn1LongRescale;
       this->Horn1InstallSpiderHanger( nameStrSecondHanger, zLocDrawing, zLocPosM,  plHDwn, vDown); 
     }
     // now a few welds.. 
     std::vector<double> zLocWelds(4,0.); // Drawing coordinate system
     zLocWelds[0] = fHorn1LongRescale*61.0464*in;
     zLocWelds[1] = fHorn1LongRescale*81.0151*in;
     zLocWelds[2] = fHorn1LongRescale*100.9839*in;
     zLocWelds[3] = fHorn1LongRescale*116.5*in; // Cheat a bit, place it upstream to make sure it does not overlap with the end
     for (size_t iW=0; iW !=zLocWelds.size(); iW++) { 
       std::ostringstream nameStrStr; nameStrStr << "Horn1DownstrPart1Weld" << iW+2;
       G4String nameStr(nameStrStr.str());
       const double length = 24.0*mm; //Cover two real sections... 
       const double zW = zLocWelds[iW];   
       const double rTmp1 = fHorn1Equations[7].GetVal(zW + length) + 0.015*mm + fWaterLayerThickInHorns;
       const double rTmp2 = rTmp1 + 1.8*mm; // 
       G4Tubs *aTubs = new G4Tubs(nameStr, rTmp1, rTmp2, 
	                           length/2.   , 0., 360.0*deg);
       G4LogicalVolume *pCurrent = new G4LogicalVolume(aTubs, G4Material::GetMaterial(std::string("Aluminum")), nameStr);
       G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.;
       posTmp[2] = zW + zShiftDrawingDownstr + length/2.;			      
       new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
                        vDown->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	
     }
   }
    {
   //Flange for the Inner Downstream, drawing 8875-112 363096 .. Two tubes
       // Upstream part 
       G4String nameStr("Horn1InnerDownstrFlangePart0");
       const double rTmp1 = fHorn1RadialRescale*(7.750*in/2.0);
       const double rTmp2 = fHorn1RadialRescale*(8.50*in/2.0);; // 
       const double length = fHorn1LongRescale*(12.244 - 1.10)*in - fHorn1LongRescale*12.0*mm; 
       // Subtract the 1/2 the length weld to avoid collision with the Horn1DownstrPart1Weld
       G4Tubs *aTubs = new G4Tubs(nameStr, rTmp1, rTmp2, length/2.   , 0., 360.0*deg);
       G4LogicalVolume *pCurrent = new G4LogicalVolume(aTubs, G4Material::GetMaterial(std::string("Aluminum")), nameStr);
       G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.;
       const double zDrawing = fHorn1LongRescale*(117.1126*in + 15.0*mm); // small shift to handle collisions 
       posTmp[2] = zDrawing + zShiftDrawingDownstr + length/2.;			      
       new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
                        vDown->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	
      }
     {
   //Flange per-se drawing 8875-112 363096 ..
       G4String nameStr("Horn1InnerDownstrFlangePart1");
       const double rTmp1 = fHorn1RadialRescale*7.750*in/2.0 + 1.0*mm;
       const double rTmp2 = fHorn1RadialRescale*11.271*in/2.0 + 1.0*mm; // 
       const double length = fHorn1LongRescale*(1.25)*in; // Add a bit for the connectors.
       G4Tubs *aTubs = new G4Tubs(nameStr, rTmp1, rTmp2, length/2.   , 0., 360.0*deg);
       G4LogicalVolume *pCurrent = new G4LogicalVolume(aTubs, G4Material::GetMaterial(std::string("Aluminum")), nameStr);
       G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.;
       const double zDrawing = fHorn1LongRescale*117.1126*in + fHorn1LongRescale*((12.244 - 1.10)*in + 15*mm); 
       posTmp[2] = zDrawing + zShiftDrawingDownstr + length/2.;			      
       new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
                        vDown->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	
      }
   
   
  }  // The downstream part of the real section that has the neck. 
  
  // The outer tube 
  {
       G4String nameStr("Horn1OuterTubeDowsntream");
       const double rTmp1 = fHorn1OuterTubeInnerRad;
       const double rTmp2 = fHorn1OuterTubeOuterRad; // 
       const double length = plHDwn->fParams[2] - 2.0*mm; 
       G4Tubs *aTubs = new G4Tubs(nameStr, rTmp1, rTmp2, length/2.   , 0., 360.0*deg);
       G4LogicalVolume *pCurrent = new G4LogicalVolume(aTubs, G4Material::GetMaterial(std::string("Aluminum")), nameStr);
       G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.; posTmp[2] = 0.; // at center...
       new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
                        vDown->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	
  }
  // The outer flange (downstream connector and bulk heads) Just a relatively thick tube. 
   {
       G4String nameStr("Horn1OuterTubeDowsntreamFlanges");
       const double rTmp1 = fHorn1OuterTubeOuterRad + 2.0*mm;
       const double rTmp2 = 23.5*in/2.; // 
       const double length = 3.0*in; 
       G4Tubs *aTubs = new G4Tubs(nameStr, rTmp1, rTmp2, length/2.   , 0., 360.0*deg);
       G4LogicalVolume *pCurrent = new G4LogicalVolume(aTubs, G4Material::GetMaterial(std::string("Aluminum")), nameStr);
       G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.; 
       posTmp[2] = plHDwn->fParams[2]/2. - fHorn1LongRescale*2.0*in; // Approximate..
       new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
                        vDown->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	
  }
 
  
}
LBNEVolumePlacementData* LBNEVolumePlacements::CreateHorn1TopLevelUpstr() { 

     // A bit special, case, we have to compute the inner conductor radius..
     // at the length of the target.
     
  const double in = 2.54*cm;   
  fHorn1TopUpstrLength  =  fTargetHeContTubeLengthInHorn -  // Not rescaled!!! 
                           fHorn1LongRescale*(3.0*cm - 3.316*in + 7.0*mm); 
			   // The last factor is a tweak, coming from the various safety marging and/or arbitrariness 
			   // in defining the geometry.  
  // The transition Inner to Outer is now included in the Horn1TopLevelUpstr volume!!
  fHorn1TopUpstrLength += fHorn1IOTransLength;
  std::cerr << "  CreateHorn1TopLevelUpstr : fHorn1IOTransLength " << fHorn1IOTransLength
             <<  " fTargetHeContTubeLengthInHorn " << fTargetHeContTubeLengthInHorn 
	     <<  " fHorn1TopUpstrLength	" << fHorn1TopUpstrLength << std::endl;		   
  double zEnd = fHorn1TopUpstrLength - fHorn1LongRescale*3.0*cm;  // Crappy convention again.. 
  if (zEnd > fHorn1LongRescale*30.3150*in) { // Z (drawing coordinate system) 
     std::ostringstream messStrStr; 
    messStrStr << " Helium Tube target hitting the neck! zEnd " << zEnd << "\n";
    G4String messStr(messStrStr.str());
    G4Exception("LBNEVolumePlacements::CreateHorn1TopLevelUpstr", " ", 
                   FatalErrorInArgument, messStr.c_str()); 
  }
  fHorn1TopUpstrInnerRad = fHorn1Equations[0].GetVal(zEnd) - 0.005*mm; // recaled radially before construction, if need be.
  return this->Create("Horn1TopLevelUpstr");
}
// Checking the possible radial overlap at the downstream end of the target. 
// The size and position of Horn1TopLevelUpstr and Horn1TargetDownstrHeContainer are fixed... 

void LBNEVolumePlacements::CheckHorn1InnerConductAndTargetRadii() {

  const LBNEVolumePlacementData *plSmallRing = this->Find(G4String("Check"), 
                                                    G4String("Horn1TargetDownstrHeContainer"), 
						    G4String("LBNEVolumePlacements::CheckHorn1InnerConductAndTargetRadii"));
  const LBNEVolumePlacementData *plLargeRing = this->Find(G4String("Check"), 
                                                    G4String("Horn1TopLevelUpstr"), 
						    G4String("LBNEVolumePlacements::CheckHorn1InnerConductAndTargetRadii"));
  const double xOffS = plSmallRing->fPosition[0] + 
                          (plSmallRing->fParams[2]/2.)*plSmallRing->fRotation.xz();	// Assumed misaligned once (withe respect to perfectly 
			                                                               // aligned (HORN1. ) 					    
  const double yOffS = plSmallRing->fPosition[1] + 
                          (plSmallRing->fParams[2]/2.)*plSmallRing->fRotation.yz();
			  
  const double xOffL = plLargeRing->fPosition[0] + 
                          (plLargeRing->fParams[2]/2.)*plLargeRing->fRotation.xz();
			                                                              					    
  const double yOffL = plLargeRing->fPosition[1] + 
                          (plLargeRing->fParams[2]/2.)*plLargeRing->fRotation.yz();
			  
  const double xOff = xOffS - xOffL;// now translated to ref. frame of the large ring. 
  const double yOff = yOffS - yOffL;
  
  const double rSmall = plSmallRing->fParams[1]; // Outer radius
  // Compute the large radius based on equation 0, at the relevant Z 
  const double zCoordEnd  = plLargeRing->fParams[2] - 3.0*cm*fHorn1LongRescale;
  
  const double rLarge = fHorn1Equations[0].GetVal(zCoordEnd); // Equation 1;  // Inner Radius
//  std::cerr << " CheckHorn1InnerConductAndTargetRadii, zCoordEnd " 
//            << zCoordEnd << " rLarge " << rLarge << std::endl;
  G4String messHeader(" The downstream end target is touching (or nearly touching)  the Horn1 inner conductor \n");
  
  if ((std::abs(xOff) < 0.002*mm) && (std::abs(yOff) < 0.002*mm)) {
    if (rSmall + fHorn1RadialSafetyMargin < rLarge) return;
    
    std::ostringstream messStrStr; 
    messStrStr << messHeader << " Helium Tube radius " << rSmall
                                  << " Inner cond. radius " << rLarge << " \n";
    messStrStr << messHeader << " Suggestion: Reduce fTargetLengthIntoHorn, currently set to  " << 
                                fTargetLengthIntoHorn <<  " mm \n";
    G4String messStr(messStrStr.str());
    G4Exception("LBNEVolumePlacements::CheckHorn1InnerConductAndTargetRadii", " ", 
                   FatalErrorInArgument, messStr.c_str()); 
  } else { // Misalignment
     const double phic = std::atan2(yOff, xOff); // extremum in phis .
     double rMax = -1.0;
     for (int iQ=0; iQ!=4; iQ++) { //. but which quadrant? (Should be able to figure out this more simply..) 
        const double xCrit = xOff + rSmall*std::sin(phic);					    
        const double yCrit = yOff + rSmall*std::cos(phic);
	const double rr = std::sqrt(xCrit*xCrit + yCrit*yCrit);
	if (rr > rMax) rMax = rr;
    }
    if (rMax < (rLarge - fHorn1RadialSafetyMargin)) return; // O.K> 
    std::ostringstream messStrStr; 
    messStrStr << messHeader << " Helium Tube radius " << rSmall
                                  << " Inner cond. radius " << rLarge << " \n";
    messStrStr << messHeader << " Tranvser Offset between the two rings  " << xOff << " / " << yOff << "\n";
    G4String messStr(messStrStr.str());
    G4Exception("LBNEVolumePlacements::CheckHorn1InnerConductAndTargetRadii", " ", 
                   FatalErrorInArgument, messStr.c_str()); 
  }  
}

void LBNEVolumePlacements::RescaleHorn2Lengthwise() {
  
}
  
void LBNEVolumePlacements::RescaleHorn2Radially() {
  
  
}
//
// Subdivide a conical section such that the radial equations are correct within a 
// a tolerance of 5 microns. We assume that the number of subdivisions
// estimated at the upstream end of the section  will 
// be adequate over the entire section. 
//
int LBNEVolumePlacements::GetNumberOfInnerHornSubSections(size_t eqn, double z1, double z2, int nMax) const {

   int numSubSect = 1;
   double zEnd = z2;
   const double zLengthTotal = z2 - z1;
   double zLength = zLengthTotal;
   while (true) { 
     const double r0 = fHorn1Equations[eqn].GetVal(z1);
     const double r1 = fHorn1Equations[eqn].GetVal(zEnd);
     const double rMid = fHorn1Equations[eqn].GetVal((z1 + zEnd)/2.);
     std::cerr << " GetNumberOfInnerHornSubSections z1 " << z1 << " zEnd " << zEnd << 
                     " r0 " << r0 << " r1 " << r1 << " rMid " << rMid << std::endl;
     if (std::abs(rMid - 0.5*(r0+r1)) < 0.050*mm) break; // 50 microns good enough for Gov. work. 
                                                         // meachanical tolerance is one 5 mills, (127 microns)
     zLength /=2.;
     zEnd = z1 + zLength;
     numSubSect++;
     if (numSubSect > nMax) {
       G4Exception("LBNEVolumePlacements::PlaceFinalHorn1", " ", FatalErrorInArgument, " Crazy subdivision! ");
       break;
     }
   }
   return numSubSect;
}
void LBNEVolumePlacements::Horn1InstallSpiderHanger(const G4String &nameStrH, 
                                                    double zLocTweaked, double zPosMotherVolume, 
						    const LBNEVolumePlacementData *plInfo, 
						   G4PVPlacement *vMother ) {

  const double in = 2.54*cm;
  G4String nameStr(nameStrH);nameStr += G4String("Ring");
  const double length = 0.750*in*fHorn1LongRescale;
  const int eqnNum = (zLocTweaked < (41.076*fHorn1LongRescale*in)) ? 5 : 7;
  const double zSignLength = (eqnNum == 5) ? -1.0 : 1.0; // to avoid collision with the inner conductor. 
  const double rTmp1 = fHorn1Equations[eqnNum].GetVal(zLocTweaked + length*zSignLength) 
                          + 0.0015*mm + fWaterLayerThickInHorns;
  const double rTmp2 = rTmp1 + 0.24*in; // Deduced from 363104 and equation 6
  G4Tubs *aTubs = new G4Tubs(nameStr, rTmp1, rTmp2, 
     				length/2.   , 0., 360.0*deg);
  G4LogicalVolume *pCurrent = new G4LogicalVolume(aTubs, G4Material::GetMaterial(std::string("Aluminum")), nameStr);
  G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.;
  posTmp[2] = zPosMotherVolume;  			   
  new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
  		     vMother->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);    
  
  //The connecting piece ring to hanger..  
  G4String nameStr2(nameStrH); nameStr2 += G4String("Riser");
  const double heightRiser = 0.333*in;
  const double widthH = 1.5*in; // See drawing 8875.112-MD 363115
  const double thickH = 0.184*2*in; 
  G4Box *aBox = new G4Box(nameStr2, widthH/2., heightRiser/2.0, thickH/2.0);  
  pCurrent = new G4LogicalVolume(aBox, G4Material::GetMaterial(std::string("Aluminum")), nameStr2);
  posTmp[0] = 0.; posTmp[1] = rTmp2 + 0.5*mm + heightRiser/2.;
  // Same Z position as above... 
  new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr2 + std::string("_P"), 
  		     vMother->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);    
// Now the hanger it self 
  G4String nameStr3(nameStrH);nameStr3 += G4String("Hanger");
  const double heightH = fHorn1OuterTubeInnerRad - rTmp2 - 1.0*mm - heightRiser;
  const double widthH2 = 1.0*in; // 363115 Note: we collapsed both hanger along the horizontal, transverse 
                                // direction. 
  const double thickH2 = 0.031*in; 
  aBox = new G4Box(nameStr, widthH2/2., heightH/2.0, thickH2/2.0);  
  pCurrent = new G4LogicalVolume(aBox, G4Material::GetMaterial(std::string("Aluminum")), nameStr3);
  posTmp[0] = 0.; posTmp[1] = rTmp2 + 0.5*mm + heightRiser + heightH/2.;
  // Same Z position as above... 
  new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr3 + std::string("_P"), 
  		     vMother->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);    
}
//
// The set of equations that determine the radii of the inner conductor sections
//
//
// Horn2 code starts here... 
void LBNEVolumePlacements::DeclareHorn2Dims() {
  
  const double in = 2.54*cm;
  fHorn2RadialRescale = 1.0;
  fHorn2LongRescale = 1.0;
  
  fHorn2PartsLengths.resize(7);
  fHorn2PartsRadii.resize(fHorn2PartsLengths.size());
  
  fHorn2OuterTubeOuterRad =  33.75*in/2.0; // Drawing 8875.112-MD 363385  Need an other drawing. 
  fHorn2OuterTubeInnerRad =  29.25*in/2.0; 
  fHorn2LongPosition = 6600.0; // Drawing received fropm Alberto M., who received from Rich Stefanek, we think. 
                               // From MCZero (our Zero in g4lbne ref. frame ), to the z=0. of drawing 8875.112Md 363383 
			       
  for(size_t k=0; k!= fHorn2PartsLengths.size(); ++k) 
    fHorn2PartsRadii[0] = fHorn2OuterTubeOuterRad + 1.0*in;
  
  const double offsetIOTr1 = 1.889*in; 
  fHorn2PartsLengths[0] = (offsetIOTr1 + 5.861)*in; // Drawing 8875.112-MD 363385

  fHorn2PartsLengths[1] = (24.061)*in; // Drawing 8875.112-MD 363386
//  fHorn2PartsRadii[1] = (19.385 + 1.0)*in ; // Info only, take the outer Radius 
  
  fHorn2PartsLengths[2] = (20.092)*in; // Drawing 8875.112-MD 363387
//  fHorn2PartsRadii[2] = (10.70 + 1.0)*in ; // 
  
  fHorn2PartsLengths[3] = (20.061)*in; // Drawing 8875.112-MD 363388
//   fHorn2PartsRadii[3] = (13.293 + 1.0)*in ; // info only. 

  fHorn2PartsLengths[4] = (24.030)*in; // Drawing 8875.112-MD 363389
//   fHorn2PartsRadii[4] = (17.755 + 1.0)*in ; // info only. 

  fHorn2PartsLengths[5] = (24.325)*in; // Drawing 8875.112-MD 363390
//   fHorn2PartsRadii[4] = (21.341 + 1.0)*in ; // info only. 

  fHorn2PartsLengths[6] = (21.054)*in; // Drawing 8875.112-MD 363390
//   fHorn2PartsRadii[4] = (25.308 + 1.0)*in ; // info only. 
// add 1/2 inch margin for misalignment.
 
  fHorn2LengthMargin = 0.5*in;
   
  fHorn2Length = fHorn2LengthMargin; 
  for (size_t k=0; k!= fHorn2PartsLengths.size(); ++k) fHorn2Length +=fHorn2PartsLengths[k];
  fHorn2Length += fHorn2LengthMargin; 
   
  int numSctTr = 3 + 2; // Number of tubes, otherwise. Approximate.. 
  //  Take 10 for the transition bulge, 2 for the outer flat part.
  const double radIOTrOut = 2.184*in; 
  const double radIOTrIn = 1.869*in; 
  const double radIOTrAv = 0.5*(radIOTrOut + radIOTrIn);
  const double deltaRadIOTr = radIOTrOut - radIOTrIn;
  const double thetaIOTRStart = std::abs(offsetIOTr1 - radIOTrOut)/radIOTrOut; // approximate.. 
  const double surfXSect = M_PI - (thetaIOTRStart)*(radIOTrOut*radIOTrOut - radIOTrIn*radIOTrIn);
  const double surfXSectPart = surfXSect/(numSctTr-2);
  const double bigRadIOTr = 12.756*in;
  // Valid only for 
  fHorn2UpstrOuterIOTransLengths.resize(numSctTr);
  fHorn2UpstrOuterIOTransRadsOne.resize(numSctTr);
  fHorn2UpstrOuterIOTransRadsTwo.resize(numSctTr);
  fHorn2UpstrOuterIOTransPositions.resize(numSctTr);
  
  fHorn2UpstrOuterIOTransLengths[1] = deltaRadIOTr - 0.010*mm; 
  fHorn2UpstrOuterIOTransPositions[1] = deltaRadIOTr/2.; // with respect to the start of the mother volume   
  const double heightTmp1 = surfXSectPart/fHorn2UpstrOuterIOTransLengths[1];
  fHorn2UpstrOuterIOTransRadsOne[1] = bigRadIOTr - heightTmp1/2.;
  fHorn2UpstrOuterIOTransRadsTwo[1] = bigRadIOTr + heightTmp1/2.;
  const double zMaxElemZeroUpstr = deltaRadIOTr + 0.010*mm;
  const double zMaxElemZeroDownstr = offsetIOTr1 - 0.010*mm;
  fHorn2UpstrOuterIOTransLengths[0] = zMaxElemZeroDownstr - zMaxElemZeroUpstr;
  fHorn2UpstrOuterIOTransPositions[0] = (zMaxElemZeroDownstr + zMaxElemZeroUpstr)/2.;
  fHorn2UpstrOuterIOTransLengths[2] = fHorn2UpstrOuterIOTransLengths[0];
  fHorn2UpstrOuterIOTransPositions[2] = fHorn2UpstrOuterIOTransPositions[0];
  const double heightTmp0 = surfXSectPart/fHorn2UpstrOuterIOTransLengths[0];
  fHorn2UpstrOuterIOTransRadsOne[0] = bigRadIOTr - heightTmp1/2. - radIOTrAv/sqrt(2.) ; 
  fHorn2UpstrOuterIOTransRadsTwo[0] = bigRadIOTr + heightTmp1/2. - radIOTrAv/sqrt(2.) ;
  fHorn2UpstrOuterIOTransRadsOne[2] = bigRadIOTr - heightTmp1/2. + radIOTrAv/sqrt(2.) ; 
  fHorn2UpstrOuterIOTransRadsTwo[2] = bigRadIOTr + heightTmp1/2. + radIOTrAv/sqrt(2.) ;
  // Top part. 
  fHorn2UpstrOuterIOTransLengths[3] = 0.45*in - 0.010*mm;
  fHorn2UpstrOuterIOTransPositions[3] = radIOTrOut + fHorn2UpstrOuterIOTransLengths[3]/2. + 0.005*mm;
  fHorn2UpstrOuterIOTransRadsOne[3] = bigRadIOTr + radIOTrIn ;
  fHorn2UpstrOuterIOTransRadsTwo[3] = bigRadIOTr + radIOTrOut;
  // Fange part 
  fHorn2UpstrOuterIOTransLengths[4] = 1.30*in - 0.010*mm;
  fHorn2UpstrOuterIOTransPositions[4] = radIOTrOut + 
                                       fHorn2UpstrOuterIOTransLengths[3] + fHorn2UpstrOuterIOTransLengths[4]/2.;
  fHorn2UpstrOuterIOTransRadsOne[4] = bigRadIOTr + fHorn2UpstrOuterIOTransLengths[3]/2. + 0.005*mm ;
  fHorn2UpstrOuterIOTransRadsTwo[4] = fHorn2OuterTubeOuterRad;
  
 // 
 // Load the equations Drawing 8875.112-MD 36385 - 363092
//
  fHorn2Equations.clear();
  LBNEHornRadialEquation e1(114.73006, -2.91414, 0.); fHorn2Equations.push_back(e1);
  LBNEHornRadialEquation e2(22.68402, -0.54203, 0., false); fHorn2Equations.push_back(e2);
  LBNEHornRadialEquation e3(-10.63139, 0.30058, 0., false); fHorn2Equations.push_back(e3); // linear. 
  LBNEHornRadialEquation e4(-56.92263, 1.44583, 0.); fHorn2Equations.push_back(e4); 
  LBNEHornRadialEquation e5(0.12835, 0.0932, 0., false); fHorn2Equations.push_back(e5); 
  LBNEHornRadialEquation e6(1.93227, 0.07398, 0., false); fHorn2Equations.push_back(e6); // 363390
  LBNEHornRadialEquation e7(114.73007, -2.91414, -0.11811);  fHorn2Equations.push_back(e7); // equation 7 found 0n 363386
                                                                        // equation 7 on 363385 differs sliggtly.. in the noise... 
  LBNEHornRadialEquation e8(-56.92263, 1.44583, -0.11811); fHorn2Equations.push_back(e8); 
  LBNEHornRadialEquation e9(0.01064, 0.0932, 0., false); fHorn2Equations.push_back(e9); 
  LBNEHornRadialEquation e10(1.81456, 0.07398, 0., false); fHorn2Equations.push_back(e10); // 363389
 
 // The 2nd top lelvel section is the remaining part of that part, up to the end of the Horn1Hall Z location. 
 // This is just a tube. 
 //
    
 // Then, conical sections, all in a tube that has an inner radius of the neck, our radius 
 // 
 // To compute all that, we need to locate the target first.. 
 // So, done at construction time. 
 //
  
}
double LBNEHornRadialEquation::inchDef = 2.54*cm;

LBNEHornRadialEquation::LBNEHornRadialEquation(double rc, double zc, double rOff, bool isParabolic):
parabolic(isParabolic),
rCoeff(rc),
zCoeff(zc),
rOff(rOff),
rResc(1.0),
zResc(1.0)
{ ; } 

double LBNEHornRadialEquation::GetVal(double z) const {
  // z = 0 above is by arbitrary choice the Z coordinate of the starting point of the Horn1TopLevelUpstr logical volume
  // We shift the drawing coordinate system
 // const double zD = z + 3.2852*inchDef*zResc; // Only valid for Horn1 !!!!! 
// By definition, drawing 8875.000-ME-363028 (Z=0 is ACTRNT)
 
 const double zR = z/inchDef; // back in inches. 
 const double argR = rCoeff + zR*zCoeff;
 if (argR < 0.) {
    std::ostringstream mStrStr; mStrStr << " Negative argument, z = " << z; 
    G4String mStr(mStrStr.str());
    G4Exception("LBNEHornRadialEquation::GetVal", " ", FatalErrorInArgument, mStr.c_str());
  } 
 const double radius = parabolic ? (std::sqrt(argR) + rOff) : argR;
 return radius*zResc*inchDef;
}

void LBNEHornRadialEquation::test1() const {

  //Test at the end of first section of Horn1 (8875.112-MD 363104) 
  const double argZ = (3.2645 + 17.876 - 0.031);
  const double rTest = this->GetVal(argZ*25.4*mm*zResc);
  std::cerr << " LBNEHornRadialEquation::test1, argZ " << argZ << "  rTest (mm) " << rTest <<  std::endl;
  std::cerr << " inchDef " << inchDef << " zCoeff " << zCoeff << " rCoeff " << rCoeff << " rOff " << rOff << std::endl;
  const double delta = 2.0*rTest - 1.6326*25.4*mm*rResc; // Actually, the drawing says 1.6 !!! mistake of 700 microns. 
  std::cerr << " delta (mm) " << delta << " zResc " << zResc << " rResc " << rResc <<  std::endl;
  if (std::abs(delta) > 0.127*mm) { // 5 mill tolerance 
    G4Exception("LBNEHornRadialEquation::test1", " ", FatalErrorInArgument,
                " Horn1 Equation 0 inconsistent with drawing 363104"); 
  } 
}
