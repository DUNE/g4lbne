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
    LBNEVolumePlacementData *plDowstrHe = Create("Horn1TargetDownstrHelium");
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
 	if (copyNumber == (fMaxNumAlignRings -1)) {
	 // retract the last one a bit by 5 mm to place the return cooling pipe. 
	 posTmp[2] -= 10.0*mm;
	}				  
        new G4PVPlacement((G4RotationMatrix *) 0, 
	                            posTmp, infoTmpRLeft.fCurrent, G4String("TargetAlignmentRingLeft")+cNumStrStr.str(), 
				          vMHe->GetLogicalVolume(), false, copyNumberHere, fCheckVolumeOverLapWC);
        new G4PVPlacement((G4RotationMatrix *) 0, 
	                            posTmp, infoTmpRRight.fCurrent, G4String("TargetAlignmentRingRight")+ cNumStrStr.str(), 
				          vMHe->GetLogicalVolume(), false, copyNumberHere, fCheckVolumeOverLapWC);
       posTmp[2] += fTargetAlignRingSpacing;      
        copyNumber++; copyNumberHere++; 
        if ( copyNumber == fMaxNumAlignRings) break;
      }
    }
    // Deal with the first, split, target segment.  
    LBNEVolumePlacementData *infoTargSegFirst = 0;
    if (fTargetFinLengthSplitDwnstr > 0.3*mm) { 
      infoTargSegFirst = Create("Horn1TargetSegmentFirst");
      G4PVPlacement *vTargSegFirst = PlaceFinal("Horn1TargetSegmentFirst", vMHe);
      Create("Horn1TargetFinVertFirst");
      LBNEVolumePlacementData * plCoolingTubeFirst = Create("Horn1TargetCoolingTubeFirst");
      Create("Horn1TargetCoolingTubeFirstWater");
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
     }
    // Now place the previously defined standard target segment. Note: they already contain their cooling and 
    // real target. 
//    LBNEVolumePlacementData  *plHorn1TargetSegment = Create("Horn1TargetSegment");
    std::map<G4String, LBNEVolumePlacementData>::iterator itTargSeg = fSubVolumes.find(G4String("TargetUpstrDownstrSegment"));
    LBNEVolumePlacementData  *plTargetUpstrDownstrSegment= &itTargSeg->second;    
    
    double zCoordTmp = (infoTargSegFirst == 0) ? 
                       (-1.0*plDowstrHe->fParams[2]/2. + plTargetUpstrDownstrSegment->fParams[2]/2. + 0.005*mm) :
                        ( infoTargSegFirst->fPosition[2] + infoTargSegFirst->fParams[2]/2. + 
                       plTargetUpstrDownstrSegment->fParams[2]/2. + 0.002*mm);	       
    for (int iSeg=0; iSeg != fTargetNumFinsInHorn; iSeg++) { // Place with no misalignment
      posTmp[0] = 0.; posTmp[1] = 0.;
      posTmp[2] =  zCoordTmp;
//      std::cerr << " In Horn1, Positioning target segment " << iSeg  << " at Z = zCoord " << zCoordTmp << std::endl;
      std::ostringstream cNumStrStr; cNumStrStr << "_P" << iSeg;
         new G4PVPlacement((G4RotationMatrix *) 0, 
				  posTmp, plTargetUpstrDownstrSegment->fCurrent, 
				    G4String("Horn1TargetSegment")+cNumStrStr.str(), 
					vMHe->GetLogicalVolume(), false, iSeg+100, fCheckVolumeOverLapWC);
      zCoordTmp += (fTargetFinLength + fTargetFinSpacingLength);
    }
    // Now the end 
    LBNEVolumePlacementData *plCoolingTubeReturn = Create("Horn1TargetCoolingTubeDwnRetTit");
    posTmp[0] = 0.; posTmp[1] = 0.; 
    posTmp[2] = plDowstrHe->fParams[2]/2 - 
	       fTargetCTubeReturnDownstrThickWater - 1.5*fTargetCTubeReturnDownstrThickTitanium/2. - 2.0*mm;
	       //  Hopefully some roo to spare.. 			  
    new G4PVPlacement(&plCoolingTubeReturn->fRotation, 
	                            posTmp, plCoolingTubeReturn->fCurrent, 
				    G4String("Horn1TargetCoolingTubeDwnRetTit_PFront"), 
				          vMHe->GetLogicalVolume(), false, 0, fCheckVolumeOverLapWC);
    posTmp[2] = plDowstrHe->fParams[2]/2 -  0.5*fTargetCTubeReturnDownstrThickTitanium/2. - 1.3*mm;			  
    new G4PVPlacement(&plCoolingTubeReturn->fRotation, 
				  posTmp, plCoolingTubeReturn->fCurrent, 
				  G4String("Horn1TargetCoolingTubeDwnRetTit_PBack"), 
					vMHe->GetLogicalVolume(), false, 0, fCheckVolumeOverLapWC);
    
    Create("Horn1TargetCoolingTubeDwnRetWater");
    PlaceFinal("Horn1TargetCoolingTubeDwnRetWater", vMHe);

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
   fHorn1NeckZPosition = 30.315*in + fHorn1NeckLength/2.;  // Drawing 8875.112-MD  363105
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


void LBNEVolumePlacements::PlaceFinalHorn1(G4PVPlacement *mother, G4PVPlacement *motherHorn1Upstr) {

// 
// Start with upstream Inner to Out transition. This one is surveyable (as well as optionally rescaled. )
//
   const double in = 2.54*cm;
//
//   std::cerr << " Start testing PlaceFinalHorn1, mother Logical volume name " << mother->GetLogicalVolume()->GetName() << std::endl;
   fHorn1Equations[0].test1(); // this supposed to work 
//   std::cerr << " Test 1 passed " << std::endl;
//   fHorn1Equations[3].test1(); // This supposed to fail... 
   LBNEVolumePlacementData *plTrUpst = this->Create("Horn1IOTransCont");
   G4PVPlacement *vTrUpst = this->PlaceFinal("Horn1IOTransCont", motherHorn1Upstr);
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
     G4LogicalVolume *pCurrent = new G4LogicalVolume(aCons, G4Material::GetMaterial(std::string("Aluminum")), nameStr);
     G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.;  
     posTmp[2] = -1.0*(plTrUpst->fParams[2])/2. + fHorn1UpstrZPositions[k];			      
     new G4PVPlacement(	(G4RotationMatrix *) 0,	posTmp, pCurrent, nameStr + std::string("_P"), 
                        vTrUpst->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);
//     std::cerr << " Placing IOTrans Sub elem " << k << " at z = " << fHorn1UpstrZPositions[k] << " (abs), rel " << 
//       posTmp[2] << " length " << fHorn1UpstrLengths[k] << std::endl;
//     std::cerr << " RIUps " << fHorn1UpstrInnerRadsUpstr[k] << " ROUps " << fHorn1UpstrInnerRadsOuterUpstr[k];  			      
//     std::cerr << " RIDwn " << fHorn1UpstrInnerRadsDownstr[k] << " RODwn " << fHorn1UpstrInnerRadsOuterDownstr[k] << std::endl;  			      
   }
   
   for (size_t k=0; k!= fHorn1UpstrOuterIOTransInnerRads.size(); k++) {
     std::ostringstream nameStrStr; nameStrStr << "Horn1IOTransOuterPart" << k;
     G4String nameStr(nameStrStr.str());
     G4Tubs *aTubs = new G4Tubs(nameStr, fHorn1UpstrOuterIOTransInnerRads[k],
                                          fHorn1UpstrOuterIOTransInnerRads[k] + fHorn1UpstrOuterIOTransThicks[k],
	                              fHorn1UpstrOuterIOTransLengths[k]/2., 0., 360.0*deg);
     G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.;  
     posTmp[2] = -1.0*(plTrUpst->fParams[2])/2. + fHorn1UpstrOuterIOTransPositions[k];			      
     G4LogicalVolume *pCurrent = new G4LogicalVolume(aTubs, G4Material::GetMaterial(std::string("Aluminum")), nameStr);
     new G4PVPlacement(	(G4RotationMatrix *) 0,	posTmp, pCurrent, nameStr + std::string("_P"), 
                        vTrUpst->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	      
   }

   // O.K. Now the part of the Horn1 that surrounds the target. 
   // We already defined this mother volume. It is the infamous volume Horn1TopLevelUpstr
   // It is a cylinder (inner radius 0. ), or radius encompassing the inner and outer conductors. 
   
   G4PVPlacement *vUpst = motherHorn1Upstr;
   const LBNEVolumePlacementData *plHUpst = Find(G4String("InnerConductors"), "Horn1TopLevelUpstr", G4String("Create"));
  
// Start by checking possible radial overlap at the downstream end of the target. 
// The size and position of Horn1TopLevelUpstr and Horn1TargetDownstrHeContainer

  fZHorn1ACRNT1Shift = 3.316*in*fHorn1LongRescale; //  Drawing 8875.112-MD-363097

  this->CheckHorn1InnerConductAndTargetRadii();
   
  const double lengthInnerConductUpstr = fHorn1TopUpstrLength- fHorn1IOTransLength - 0.1*mm; // 100 microns safety margin 
  int numSubSect = GetNumberOfInnerHornSubSections(0, 0., lengthInnerConductUpstr, 10);
   // Fill with it one or more inner conductor conical section
   // We require a precision of 5 microns in the radius. 
   const double deltaZ = lengthInnerConductUpstr/numSubSect;
   for (int iSub=0; iSub != numSubSect; iSub++) { 
     //
      const double zzBegin = fZHorn1ACRNT1Shift + (iSub*deltaZ); // from the 
      const double zzEnd = zzBegin + deltaZ;
     std::ostringstream nameStrStr; nameStrStr << "Horn1UpstrSubSect" << iSub;
     G4String nameStr(nameStrStr.str());
// Smooth transition between equation 1 and 2 
     const double rMin1Eqn1 = fHorn1Equations[0].GetVal(zzBegin); // Equation 1 or 0
     const double rMin2Eqn1 = fHorn1Equations[0].GetVal(zzEnd);
     const double rMin1Eqn2 = fHorn1Equations[1].GetVal(zzBegin); // Equation 1 or 0
     const double rMin2Eqn2 = fHorn1Equations[1].GetVal(zzEnd);
     const double rMin1 = ((numSubSect - iSub -1)*rMin1Eqn1 + ((iSub+1)*rMin1Eqn2))/numSubSect;
     const double rMin2 = ((numSubSect - iSub -1)*rMin2Eqn1 + ((iSub+1)*rMin2Eqn2))/numSubSect;
//    std::cerr << " Inner radius for section " << nameStr << " At zzBegin " << zzBegin
//               << " rMin1 " << rMin1 << " rMin2 " << rMin2 << std::endl; 
     const double rMax1 = fHorn1Equations[5].GetVal(zzBegin) + fWaterLayerThickInHorns + 0.0025; 
       // Equation 6 (Drawing 8875.112-MD 363104)
     const double rMax2 = fHorn1Equations[5].GetVal(zzEnd) + fWaterLayerThickInHorns + 0.0025;     
     G4Cons *aCons = new G4Cons(nameStr, rMin1, rMax1,rMin2, rMax2,
	                              (deltaZ - 0.005*mm)/2., 0., 360.0*deg);
     G4LogicalVolume *pCurrent = new G4LogicalVolume(aCons, G4Material::GetMaterial(std::string("Aluminum")), nameStr);
     G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.;
     // plHUpst constain part of the Inner Outer Transition. Shift downtream by it's length 
     posTmp[2] = -1.0*(plHUpst->fParams[2])/2. + fHorn1IOTransLength + deltaZ/2. + iSub*deltaZ + 0.055*mm;			      
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
     const double rTmp1 = fHorn1Equations[5].GetVal( fZHorn1ACRNT1Shift + 15.0*mm - length/2.) 
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
   const double zLocFirstHangerDC = fHorn1LongRescale*(17.876 - 1.385)*in - lengthHangerRing/2.; // Drawing 8875.112-MD 363104
   const double zLocFirstHanger = zLocFirstHangerDC + fHorn1LongRescale*3.0*cm;
   // With respect to the starting point of Horn1TopLevelUpstr (assuming it is in the first section )
//   std::cerr << " zLocFirstHanger " << zLocFirstHanger << " in drawing coord. " << zLocFirstHangerDC << std::endl;
   const bool firstHangerinUpstrSect = (zLocFirstHanger < fHorn1TopUpstrLength);
     //Must have room for the whole piece.. Move it to the left if need be 
   const double zLocFirstHangerFull = zLocFirstHanger + lengthHangerRing/2. + 1.0*mm;
   double zLocTweakedFirstHanger = zLocFirstHanger;
   if (firstHangerinUpstrSect && (zLocFirstHangerFull > fHorn1TopUpstrLength)) {
     zLocTweakedFirstHanger -= (zLocFirstHangerFull - fHorn1TopUpstrLength);
   }
//   std::cerr << " zLocTweakedFirstHanger " << zLocTweakedFirstHanger << std::endl;
   if (firstHangerinUpstrSect) std::cerr << " First Spider Hanger is in Horn1Upstr section " << std::endl;
   if (!firstHangerinUpstrSect) std::cerr << " First Spider Hanger is in Horn1Downstr section " << std::endl;
   
   G4String nameStrFirstHanger("Horn1UpstrSpiderHanger");
   if ( firstHangerinUpstrSect ) {
	const double zPosCenterMotherVolume = -1.0*(plHUpst->fParams[2])/2. + zLocTweakedFirstHanger + lengthHangerRing/2. ;  			   
	const double zPosUpstrDrawingCoord = zLocTweakedFirstHanger - 0.5*lengthHangerRing - 3.0*cm*fHorn1LongRescale; 			   
        this->Horn1InstallSpiderHanger(nameStrFirstHanger, zPosUpstrDrawingCoord, 
	                                    zPosCenterMotherVolume, vUpst );			       
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
//   std::cerr << " zUpstreamCrazy3cm " << zUpstreamCrazy3cm << std::endl;
   // Used later... ZCoordinate translation from ZDrawing and the center of Horn1TopLevelDownstr
   const double zShiftDrawingDownstr = -1.0*(plHDwn->fParams[2])/2. - zUpstreamCrazy3cm;
   
   double lengthToTheNeck = fHorn1NeckZPosition - 
                          (fHorn1TopUpstrLength - 3.0*cm*fHorn1LongRescale)  - fHorn1NeckLength/2.;
			    // Stupid 3 cm coordinate shift. again..
  if (lengthToTheNeck < 0.) {
    std::ostringstream messStrStr; 
    messStrStr << "Inconsistent length to the neck of" << lengthToTheNeck << " Neck Position (drawing) "
               << fHorn1NeckZPosition << " fHorn1TopUpstrLength " <<  fHorn1TopUpstrLength;
    G4String messStr(messStrStr.str());
    G4Exception("LBNEVolumePlacements::PlaceFinalHorn1", " ",  FatalErrorInArgument, messStr.c_str());
  }
  //
  // Install the length of inner conductor, from downstream end of the target to Zdrawing 21.0888 inches 
  //
  const double z21p088 = fHorn1LongRescale*21.088*in; //Equation change
//  std::cerr << " ... Length to the neck " << lengthToTheNeck << std::endl;
  double aLengthPreNeck = 0.;
  double zLocPosIncremental = 0;
  if (z21p088 > zUpstreamCrazy3cm) {
     int numSubSect = GetNumberOfInnerHornSubSections(0, zUpstreamCrazy3cm, 
                                                      z21p088, 10); // These Z position are from the start of the inner conductor.   
     aLengthPreNeck =  z21p088 - zUpstreamCrazy3cm;
     const double deltaZ = aLengthPreNeck/numSubSect;
//     std::cerr << " ...... delta z to 21.0888 " << deltaZ << std::endl;
     for (int iSub = 0; iSub != numSubSect; iSub++) {					      
       const double zzBegin = zUpstreamCrazy3cm + iSub*deltaZ;
       const double zzEnd = zzBegin + deltaZ;
       std::ostringstream nameStrStr; nameStrStr << "Horn1DownstrPartM0SubSect" << iSub;
       G4String nameStr(nameStrStr.str());
       const double rMin1Eqn1 = fHorn1Equations[0].GetVal(zzBegin); // Equation 1 or 0
       const double rMin2Eqn1 = fHorn1Equations[0].GetVal(zzEnd);
       const double rMin1Eqn2 = fHorn1Equations[1].GetVal(zzBegin); // Equation 1 or 0
       const double rMin2Eqn2 = fHorn1Equations[1].GetVal(zzEnd);
       const double rMin1 = ((numSubSect - iSub -1)*rMin1Eqn1 + ((iSub+1)*rMin1Eqn2))/numSubSect;
       const double rMin2 = ((numSubSect - iSub -1)*rMin2Eqn1 + ((iSub+1)*rMin2Eqn2))/numSubSect;
       const double rMax1 = fHorn1Equations[5].GetVal(zzBegin) + fWaterLayerThickInHorns + 0.0025; 
       // Equation 6 (Drawing 8875.112-MD 363104)
       const double rMax2 = fHorn1Equations[5].GetVal(zzEnd) + fWaterLayerThickInHorns + 0.0025;     
       G4Cons *aCons = new G4Cons(nameStr, rMin1, rMax1,rMin2, rMax2,
	                              (deltaZ - 0.005*mm)/2., 0., 360.0*deg);
       G4LogicalVolume *pCurrent = new G4LogicalVolume(aCons, G4Material::GetMaterial(std::string("Aluminum")), nameStr);
       G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.;
       posTmp[2] = -1.0*(plHDwn->fParams[2])/2. + iSub*deltaZ + deltaZ/2.;
       zLocPosIncremental += deltaZ ;
       G4PVPlacement *vSub = new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
                        vDown->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	
			
      if (fWaterLayerThickInHorns	> 0.002*mm) {
       std::ostringstream nameStrStr; nameStrStr << "Horn1DownstrPartM0SubSect" << iSub << "Water";
       G4String nameStr(nameStrStr.str());
       G4Cons *aCons = new G4Cons(nameStr, rMax1 - fWaterLayerThickInHorns, rMax1-0.001*mm,
                                         rMax2 - fWaterLayerThickInHorns, rMax2-0.001*mm,
	                              (deltaZ - 0.0075*mm)/2., 0., 360.0*deg);
       G4LogicalVolume *pCurrent = new G4LogicalVolume(aCons, G4Material::GetMaterial(std::string("Water")), nameStr);
       G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.; posTmp[2] =0.;			      
       new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
                        vSub->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	
      }
    } // of the number of subsections in the upstream part of the neck region (zDrawing = 21.0888 inches) 
    lengthToTheNeck -= aLengthPreNeck;
  }
  //
  // Install the length of inner conductor, from either z = 21.0888 inches or zUpstreamCrazy3cm
  // to the neck 
  //
//  std::cerr << " ... Length to the neck after possible installation of pre Neck " << lengthToTheNeck << std::endl;
  const double zStartToNeck = (z21p088 > zUpstreamCrazy3cm) ? z21p088 : zUpstreamCrazy3cm;
  {
     int numSubSect = GetNumberOfInnerHornSubSections(0, zStartToNeck, 
                                                      fHorn1NeckZPosition, 10); // These Z position are from the start of the inner conductor.   
     const double deltaZ = lengthToTheNeck/numSubSect;
     for (int iSub = 0; iSub != numSubSect; iSub++) {					      
       const double zzBegin = zStartToNeck + iSub*deltaZ;
       const double zzEnd = zzBegin + deltaZ;
       std::ostringstream nameStrStr; nameStrStr << "Horn1DownstrPart0SubSect" << iSub;
       G4String nameStr(nameStrStr.str());
       const double rMin1 = fHorn1Equations[1].GetVal(zzBegin); // Equation 1
       const double rMin2 = fHorn1Equations[1].GetVal(zzEnd);
       const double rMax1 = fHorn1Equations[5].GetVal(zzBegin) + fWaterLayerThickInHorns + 0.0025; 
       // Equation 6 (Drawing 8875.112-MD 363104)
       const double rMax2 = fHorn1Equations[5].GetVal(zzEnd) + fWaterLayerThickInHorns + 0.0025;     
       G4Cons *aCons = new G4Cons(nameStr, rMin1, rMax1,rMin2, rMax2,
	                              (deltaZ - 0.10*mm)/2., 0., 360.0*deg);
       G4LogicalVolume *pCurrent = new G4LogicalVolume(aCons, G4Material::GetMaterial(std::string("Aluminum")), nameStr);
       G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.;
       posTmp[2] = zLocPosIncremental -1.0*(plHDwn->fParams[2])/2. + iSub*deltaZ + deltaZ/2.;
       G4PVPlacement *vSub = new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
                        vDown->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	
			
      if (fWaterLayerThickInHorns	> 0.002*mm) {
       std::ostringstream nameStrStr; nameStrStr << "Horn1DownstrPart0SubSect" << iSub << "Water";
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
    if (!firstHangerinUpstrSect) { // 
     //Must have room for the whole piece.. Move it to the right if need be 
      G4String nameStrFirstHanger("Horn1DownstrFirstSpiderHanger");
 			    // Stupid 3 cm coordinate shift. again..
       const double zLocPosM = zLocFirstHangerDC + zShiftDrawingDownstr + lengthHangerRing/2.;
      // of the mother volume. 
       this->Horn1InstallSpiderHanger( nameStrFirstHanger, zLocFirstHangerDC, zLocPosM,  vDown); 
    }
    // The first weld for this section. 
   {
     G4String nameStr("Horn1DownstrSubSect1Weld0");
     G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.;
     const double length = 24.0*mm; //Cover two real sections... 
     double zW = 21.0888*in*fHorn1LongRescale + length/2; // In Drawing coordinate system 
     posTmp[2] =  zW + zShiftDrawingDownstr + length/2.; // with respecto the upstr edge of Horn1TopLevelDownstr
     if ((posTmp[2] - 0.5*length) < -1.0*(plHDwn->fParams[2])/2.) {
        const double posTmpW = posTmp[2];
	posTmp[2] =-1.0*(plHDwn->fParams[2])/2.  + length/2. + 0.05*mm;	     
       std::cout << " Weld should be placed in the upstream section, but placed in the downstream one.. "
                 << std::endl << " shifted by "  << posTmpW - posTmp[2] << std::endl;
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
  } // From the downstream of the target to the neck, or from z=21.0888 to the neck  
   //
   // Now, the neck. Just a tube 
   //
   {
     G4String nameStr("Horn1Neck");
     const double zNeckDrawing = fHorn1LongRescale*(30.3150)*in; //start of the neck.. 
     const double rTmp1 = fHorn1RadialRescale*(0.709*in/2.); // Drawing 8875.112-MD 363105
     const double rTmp2 = fHorn1RadialRescale*(1.063*in/2.) + fWaterLayerThickInHorns + 0.025*mm; 
       // Drawing 8875.112-MD 363105
     fHorn1NeckInnerRadius = rTmp1; // For use in computing the magnetic field 
     fHorn1NeckOuterRadius = rTmp2; // For use in computing the magnetic field 
     const double length = fHorn1LongRescale*1.5680*in - 0.050*mm; // last term to absord 
        // small shifts in the upstream part.. 
     G4Tubs *aTubs = new G4Tubs(nameStr, rTmp1, rTmp2, 
	                           length/2.   , 0., 360.0*deg);
     G4LogicalVolume *pCurrent = new G4LogicalVolume(aTubs, G4Material::GetMaterial(std::string("Aluminum")), nameStr);
     G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.;
     posTmp[2] = zNeckDrawing  + zShiftDrawingDownstr + length/2. + 0.025*mm;			      
     G4PVPlacement* vSub = new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
                        vDown->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);
     if (fWaterLayerThickInHorns > 0.002*mm) {
       G4String nameStrW(nameStr); nameStrW += G4String("Water");
       G4Tubs *aTubsW = new G4Tubs(nameStrW, rTmp2-fWaterLayerThickInHorns-0.012*mm, rTmp2-0.012*mm, 
	                           length/2.   , 0., 360.0*deg);
       G4LogicalVolume *pCurrentW = new G4LogicalVolume(aTubsW, 
                             G4Material::GetMaterial(std::string("Water")), nameStrW);
       G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.; posTmp[2] =0.;			      
       new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrentW, nameStrW + std::string("_P"), 
                        vSub->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	
      }
   }				
    // The downstream part of the real section that has the neck.  
   {
     const double zStartDrawing =  fHorn1LongRescale*31.8827*in;
     const double zEndDrawing = fHorn1LongRescale*(41.0776)*in;
     fHorn1ZDEndNeckRegion = zEndDrawing;
     int numSubSect = GetNumberOfInnerHornSubSections(3, zStartDrawing, 
                                                      zEndDrawing, 10); // These Z position are from the start of the inner conductor.   
     const double deltaZ = (zEndDrawing - zStartDrawing)/numSubSect;
     for (int iSub = 0; iSub != numSubSect; iSub++) {					      
       const double zzBegin = zStartDrawing + iSub*deltaZ;
       const double zzEnd = zzBegin + deltaZ;
       std::ostringstream nameStrStr; nameStrStr << "Horn1DownstrPart1SubSect" << iSub;
       G4String nameStr(nameStrStr.str());
       const double rMin1 = fHorn1Equations[3].GetVal(zzBegin); 
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
     const double rTmp1 = fHorn1Equations[7].GetVal(zWW + length) + 0.150*mm + fWaterLayerThickInHorns;
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
     fHorn1ZEndIC = zEndDrawing; // For use in the Magnetic field class. 
     int numSubSect = GetNumberOfInnerHornSubSections(4, zStartDrawing, 
                                                      zEndDrawing, 10); // These Z position are from the start of the inner conductor.   
     const double deltaZ = (zEndDrawing - zStartDrawing)/numSubSect;
//     std::cerr << " Number of subsection for the downstream half of Horn1 " << numSubSect 
//                 << " deltaz " << deltaZ << std::endl;
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
       this->Horn1InstallSpiderHanger( nameStrFirstHanger, zLocDrawing, zLocPosM, vDown); 
       
       G4String nameStrSecondHanger("Horn1DownstrThirdSpiderHanger");
       zLocDrawing = fHorn1LongRescale*(80.9951 + 1.791)*in;
       zLocPosM = zLocDrawing + zShiftDrawingDownstr + 0.375*in*fHorn1LongRescale;
       this->Horn1InstallSpiderHanger( nameStrSecondHanger, zLocDrawing, zLocPosM, vDown); 
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
  //
  // The above equation to determine fHorn1TopUpstrLength looks wrong based on Geantino analysis
  // Why not simply:
  
  fHorn1TopUpstrLength  =  fTargetHeContTubeLengthInHorn + 7.0*mm;
  
//  std::cerr << "  CreateHorn1TopLevelUpstr : fHorn1IOTransLength " << fHorn1IOTransLength
//             <<  " fTargetHeContTubeLengthInHorn " << fTargetHeContTubeLengthInHorn 
//	     <<  " fHorn1TopUpstrLength	" << fHorn1TopUpstrLength << std::endl;		   
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

  const double in = 2.54*cm;   
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
			  
			  // We take the full length, the clash is at the downstream end. 
			  
  const double xOffL = plLargeRing->fPosition[0] + 
                          (plLargeRing->fParams[2]/2.)*plLargeRing->fRotation.xz();
			                                                              					    
  const double yOffL = plLargeRing->fPosition[1] + 
                          (plLargeRing->fParams[2]/2.)*plLargeRing->fRotation.yz();
			  // Same, at the downstream end. But this is only approximate. 
  const double xOff = xOffS - xOffL;// now translated to ref. frame of the large ring. 
  const double yOff = yOffS - yOffL;
  
  const double rSmall = plSmallRing->fParams[1]; // Outer radius
  // Compute the large radius based on equation 0, at the relevant Z 
  const double zCoordEnd  = fHorn1TopUpstrLength - 3.0*in*fHorn1LongRescale;
  const int eqNum = ((zCoordEnd/fHorn1LongRescale) < 21.0888*in) ? 0 : 1;
  const double rLarge = fHorn1Equations[eqNum].GetVal(zCoordEnd); // Equation 1;  // Inner Radius
//  std::cerr << " CheckHorn1InnerConductAndTargetRadii, zCoordEnd " 
//            << zCoordEnd << " rLarge " << rLarge << " rSmall " << rSmall << std::endl;
//  std::cerr << " .. xOffS " << 	xOffS << "  yOffS  " <<  yOffS 
//            << " xOffL " << xOffL  << " yOffL " << yOffL << std::endl;
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
//     const double phic = std::atan2(yOff, xOff); // extremum in phis . We don't care where it is in phi.. 
//     double rMaxDev = -1.0;
//     for (int iQ=0; iQ!=4; iQ++) { //. but which quadrant? (Should be able to figure out this more simply..) 
//        const double xCrit = rSmall*std::sin(phic);					     
//        const double yCrit = rSmall*std::cos(phic);
//	const double rr = std::sqrt(xCrit*xCrit + yCrit*yCrit);
//	if (rr > rMaxDev) rMaxDev = rr;
//    }
    const double  rMaxDev = std::sqrt(xOff*xOff + yOff*yOff);
//    std::cerr << " Misalignments detected in CheckHorn1InnerConductAndTargetRadii, phic " << phic 
//              << " rMaxDev  " << rMaxDev << std::endl;
    if ((rMaxDev + rSmall) < (rLarge - fHorn1RadialSafetyMargin)) return; // O.K> 
    std::ostringstream messStrStr; 
    messStrStr << messHeader << " Helium Tube radius " << rSmall
                                  << " Inner cond. radius " << rLarge << " \n";
    messStrStr << messHeader << " Tranvser Offset between the two rings  " << xOff << " / " << yOff << "\n";
    G4String messStr(messStrStr.str());
    G4Exception("LBNEVolumePlacements::CheckHorn1InnerConductAndTargetRadii", " ", 
                   FatalErrorInArgument, messStr.c_str()); 
  }  
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
//     std::cerr << " GetNumberOfInnerHornSubSections z1 " << z1 << " zEnd " << zEnd << 
//                     " r0 " << r0 << " r1 " << r1 << " rMid " << rMid << std::endl;
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
//
// 2nd argument is in drawing coordinate system, the upstream coordinate of the beginning of the ring
//  Third argument, in the reference frame of the mother volume, the long. center position 
//
void LBNEVolumePlacements::Horn1InstallSpiderHanger(const G4String &nameStrH, 
                                                    double zLocTweakedDC, double zPosMotherVolume, 
						   G4PVPlacement *vMother ) {

  const double in = 2.54*cm;
  G4String nameStr(nameStrH);nameStr += G4String("Ring");
  const double length = 0.750*in*fHorn1LongRescale;
  const int eqnNum = (zLocTweakedDC < (41.076*fHorn1LongRescale*in)) ? 5 : 7;
  const double zSignLength = (eqnNum == 5) ? -1.0 : 1.0; // to avoid collision with the inner conductor outer radius 
                                                         // at the upstream or downstream end 
  const double rTmp1 = fHorn1Equations[eqnNum].GetVal(zLocTweakedDC + length*zSignLength) 
                          + 0.0015*mm + fWaterLayerThickInHorns;
  const double rTmp2 = rTmp1 + 0.24*in; // Deduced from 363104 and equation 6
  G4Tubs *aTubs = new G4Tubs(nameStr, rTmp1, rTmp2, 
     				length/2.   , 0., 360.0*deg);
  G4LogicalVolume *pCurrent = new G4LogicalVolume(aTubs, G4Material::GetMaterial(std::string("Aluminum")), nameStr);
  G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.;
  posTmp[2] = zPosMotherVolume;  			   
  new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
  		     vMother->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);    
  
  //The connecting piece ring to the hangers.. There are three of them, at 120 degrees from each other. 
  
  G4String nameStr2(nameStrH); nameStr2 += G4String("Riser");
  const double heightRiser = 0.333*in - 0.020*mm;
  const double widthH = 1.5*in; // See drawing 8875.112-MD 363115
  const double thickH = 0.184*2*in; 
  G4Box *aBoxRiser = new G4Box(nameStr2, widthH/2., heightRiser/2.0, thickH/2.0);  
  G4LogicalVolume *pCurrentRiser = 
    new G4LogicalVolume(aBoxRiser, G4Material::GetMaterial(std::string("Aluminum")), nameStr2);
    
  G4String nameStr3(nameStrH); nameStr3 += G4String("Hanger");
  const double heightH = fHorn1OuterTubeInnerRad - rTmp2 - 1.0*mm - heightRiser;
  const double widthH2 = 1.0*in; // 363115 Note: we collapsed both hanger along the horizontal, transverse 
                                // direction. 
  const double thickH2 = 0.031*in; 
  G4Box *aBoxHanger = new G4Box(nameStr3, widthH2/2., heightH/2.0, thickH2/2.0);  
  G4LogicalVolume *pCurrentHanger = 
    new G4LogicalVolume(aBoxHanger, G4Material::GetMaterial(std::string("Aluminum")), nameStr3);
  
  for (int iRot=0; iRot != 3; iRot++) {
    std::ostringstream rnStrStr; rnStrStr << "_" << (iRot+1);
    G4String rnStr(rnStrStr.str());
  // Same Z position as above... 
    G4RotationMatrix * rMatPr = 0;
     if (iRot == 1) {
      rMatPr = new G4RotationMatrix; 
      rMatPr->rotateZ(2.0*M_PI/3.);
    } else if (iRot == 2) {
      rMatPr = new G4RotationMatrix; 
      rMatPr->rotateZ(-2.0*M_PI/3.);
    }
    
    const double dHRiser = rTmp2 + 0.010*mm + heightRiser/2.;
    posTmp[0] = 0.;  posTmp[1] = dHRiser;
    if (iRot != 0) {
      posTmp[0] = dHRiser*rMatPr->xy();
      posTmp[1] = dHRiser*rMatPr->yy();
    }
    if (iRot == 0) { 
       new G4PVPlacement(rMatPr, posTmp, pCurrentRiser, nameStr2 + std::string("_P") + rnStr, 
  		     vMother->GetLogicalVolume(), false, iRot, fCheckVolumeOverLapWC);
    } else {
       new G4PVPlacement(G4Transform3D(*rMatPr, posTmp), pCurrentRiser, nameStr2 + std::string("_P") + rnStr, 
  		     vMother->GetLogicalVolume(), false, iRot, fCheckVolumeOverLapWC);
    }
// Now the hanger it self 
    
    const double dHHanger = rTmp2 + 0.010*mm + 0.5*mm + heightRiser + heightH/2.;
    posTmp[0] = 0.;  posTmp[1] = dHHanger;
    if (iRot != 0) {
      posTmp[0] = dHHanger*rMatPr->xy();
      posTmp[1] = dHHanger*rMatPr->yy();
    }
  // Same Z position as above... 
    if (iRot == 0) { 
       new G4PVPlacement(rMatPr, posTmp, pCurrentHanger, nameStr3 + std::string("_P") + rnStr, 
  		     vMother->GetLogicalVolume(), false, iRot, fCheckVolumeOverLapWC);    
     } else {
       new G4PVPlacement(G4Transform3D(*rMatPr, posTmp), pCurrentHanger, nameStr3 + std::string("_P") + rnStr, 
  		     vMother->GetLogicalVolume(), false, iRot, fCheckVolumeOverLapWC);
     } 
 } // on the 120 degree symmetry point.
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
  
  fHorn2OuterTubeOuterRadMax =  37.0*in/2.0; // Drawing 8875.112-MD 363385  Need an other drawing. 
  fHorn2OuterTubeOuterRad =  31.30*in/2.0; // Drawing 8875.112-MD 363385  Need an other drawing. 
  fHorn2OuterTubeInnerRad =  29.25*in/2.0; 
  fHorn2LongPosition = 6600.0*mm; // Drawing received fropm Alberto M., who received from Rich Stefanek, we think. 
                               // From MCZero (our Zero in g4lbne ref. frame ), to the z=0. of drawing 8875.112Md 363383 
			       
  for(size_t k=0; k!= fHorn2PartsRadii.size(); ++k) 
    fHorn2PartsRadii[k] = fHorn2OuterTubeOuterRad + 1.0*in;
  
  fHorn2OffsetIOTr1 = 1.889*in; // Offset between the entrance of Horn2TopLevel - margin and Z=0, drawing... 
  fHorn2PartsLengths[0] = fHorn2OffsetIOTr1 + 5.861*in; // Drawing 8875.112-MD 363385
  fHorn2LengthNominal = 142.91*in; // Drawing 8875.112-MD 363382, Revision E
  
  fHorn2PartsRadii[0] = (33.75 + 1.0)*in/2. ; // Info only, take the outer Radius 

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
  fHorn2PartsRadii[6] = fHorn2OuterTubeOuterRadMax+ 1.0*in ; // info only. 
// add 1/2 inch margin for misalignment.
 
  fHorn2LengthMargin = 0.5*in;
   
  fHorn2Length = fHorn2LengthMargin; 
  for (size_t k=0; k!= fHorn2PartsLengths.size(); ++k) fHorn2Length +=fHorn2PartsLengths[k];
  fHorn2Length += fHorn2LengthMargin; 
//
//  std::cout << " Check of Horn2 lengths, Sum of indivuals parts + margin " << fHorn2Length 
//            << " nominal " << fHorn2LengthNominal << std::endl;
   
  int numSctTr = 3 + 2; // Number of tubes, otherwise. Approximate.. 
  //  Take 10 for the transition bulge, 2 for the outer flat part.
  const double radIOTrOut = 2.184*in; 
  const double radIOTrIn = 1.869*in; 
  const double radIOTrAv = 0.5*(radIOTrOut + radIOTrIn);
  const double deltaRadIOTr = radIOTrOut - radIOTrIn;
  const double thetaIOTRStart = std::abs(fHorn2OffsetIOTr1 - radIOTrOut)/radIOTrOut; // approximate.. 
  const double surfXSect = (M_PI - thetaIOTRStart)*(radIOTrOut*radIOTrOut - radIOTrIn*radIOTrIn);
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
  const double zMaxElemZeroDownstr = fHorn2OffsetIOTr1 - 0.010*mm;
  fHorn2UpstrOuterIOTransLengths[0] = zMaxElemZeroDownstr - zMaxElemZeroUpstr;
  fHorn2UpstrOuterIOTransPositions[0] = (zMaxElemZeroDownstr + zMaxElemZeroUpstr)/2.;
  fHorn2UpstrOuterIOTransLengths[2] = fHorn2UpstrOuterIOTransLengths[0];
  fHorn2UpstrOuterIOTransPositions[2] = fHorn2UpstrOuterIOTransPositions[0];
  const double heightTmp0 = surfXSectPart/fHorn2UpstrOuterIOTransLengths[0];
  fHorn2UpstrOuterIOTransRadsOne[0] = bigRadIOTr - heightTmp0/2. - radIOTrAv/sqrt(2.) ; 
  fHorn2UpstrOuterIOTransRadsTwo[0] = bigRadIOTr + heightTmp0/2. - radIOTrAv/sqrt(2.) ;
  fHorn2UpstrOuterIOTransRadsOne[2] = bigRadIOTr - heightTmp0/2. + radIOTrAv/sqrt(2.) ; 
  fHorn2UpstrOuterIOTransRadsTwo[2] = bigRadIOTr + heightTmp0/2. + radIOTrAv/sqrt(2.) ;
  // Top part. 
  fHorn2UpstrOuterIOTransLengths[3] = 0.45*in - 0.010*mm;
  fHorn2UpstrOuterIOTransPositions[3] = radIOTrOut + fHorn2UpstrOuterIOTransLengths[3]/2. + 0.005*mm;
  fHorn2UpstrOuterIOTransRadsOne[3] = bigRadIOTr + radIOTrIn ;
  fHorn2UpstrOuterIOTransRadsTwo[3] = bigRadIOTr + radIOTrOut;
  // Fange part 
  fHorn2UpstrOuterIOTransLengths[4] = 1.30*in - 0.010*mm;
  fHorn2UpstrOuterIOTransPositions[4] = radIOTrOut +  0.0075*mm + 
                                       fHorn2UpstrOuterIOTransLengths[3] + fHorn2UpstrOuterIOTransLengths[4]/2.;
  fHorn2UpstrOuterIOTransRadsOne[4] = bigRadIOTr + fHorn2UpstrOuterIOTransLengths[3]/2. + 0.005*mm ;
  fHorn2UpstrOuterIOTransRadsTwo[4] = 33.75*in/2;


  fHorn2InnerIOTransLength = 5.8610*in;
  
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
  LBNEHornRadialEquation eIO(10.9108, -0.1780, 0., false); fHorn2Equations.push_back(eIO); // 363385
 // The 2nd top lelvel section is the remaining part of that part, up to the end of the Horn1Hall Z location. 
 // This is just a tube. 
 //
    
 // Then, conical sections, all in a tube that has an inner radius of the neck, our radius 
 // 
 // To compute all that, we need to locate the target first.. 
 // So, done at construction time. 
 //
  
}
void LBNEVolumePlacements::RescaleHorn2Lengthwise() {
  for (size_t i=0; i!= fHorn2PartsLengths.size(); i++) 
       fHorn2PartsLengths[i] *= fHorn2LongRescale;       
  fHorn2Length *=  fHorn2LongRescale;
  for (size_t i=0; i!= fHorn2UpstrOuterIOTransLengths.size(); i++) 
      fHorn2UpstrOuterIOTransLengths[i] *= fHorn2LongRescale;
  for (size_t i=0; i!= fHorn2UpstrOuterIOTransPositions.size(); i++) 
      fHorn2UpstrOuterIOTransPositions[i] *= fHorn2LongRescale;
  fHorn2InnerIOTransLength *= fHorn2LongRescale;
  fHorn2LengthNominal *=  fHorn2LongRescale;      
}
  
void LBNEVolumePlacements::RescaleHorn2Radially() {
    for (size_t i=0; i!= fHorn2PartsRadii.size(); i++) 
       fHorn2PartsRadii[i] *= fHorn2RadialRescale;       
    fHorn2OuterTubeOuterRad *= fHorn2RadialRescale;
    fHorn2OuterTubeOuterRadMax *= fHorn2RadialRescale;
    fHorn2OuterTubeInnerRad *= fHorn2RadialRescale;
    for (size_t i=0; i!= fHorn2UpstrOuterIOTransRadsOne.size(); i++) 
      fHorn2UpstrOuterIOTransRadsOne[i] *= fHorn2RadialRescale;
    for (size_t i=0; i!= fHorn2UpstrOuterIOTransRadsTwo.size(); i++) 
      fHorn2UpstrOuterIOTransRadsTwo[i] *= fHorn2RadialRescale;
      
}

void LBNEVolumePlacements::PlaceFinalHorn2(G4PVPlacement *vH2Hall) {

  const double in = 2.54*cm;
  LBNEVolumePlacementData *plInfoH2Top = this->Create("Horn2TopLevel");
  G4PVPlacement *vH2 = PlaceFinal("Horn2TopLevel", vH2Hall);
  double zPosPart = -1.0*plInfoH2Top->fParams[2]/2 + fHorn2LengthMargin;
  
  
  const double zShiftDrawingIOTrans = fHorn2PartsLengths[0]/2 - fHorn2InnerIOTransLength;
  fHorn2DeltaZEntranceToZOrigin = zShiftDrawingIOTrans + fHorn2LengthMargin;
  
  double zStartDrawing =  0.; // definition of z=0 
//  std::cerr << " Horn2 Placements, zPostPart " << zPosPart << " Length of Horn2 Top level " << plInfoH2Top->fParams[2] 
//            << " zShiftDrawingIOTrans " << zShiftDrawingIOTrans << " 1rst part length " << fHorn2PartsLengths[0] << std::endl;
  fHorn2ZEqnChanges.clear();
  for (size_t kPart=0; kPart!= fHorn2PartsLengths.size(); kPart++) {
    std::ostringstream nStrStr; nStrStr << "Horn2Part" << kPart;
    G4String nStr(nStrStr.str());
    G4Tubs* tubsPart = new G4Tubs(nStr, 0., fHorn2PartsRadii[kPart], 
                                  (fHorn2PartsLengths[kPart]/2 - 0.002*mm ), 0., 360.0*deg);
    G4LogicalVolume *tubsL = new G4LogicalVolume(tubsPart, G4Material::GetMaterial("Air"), nStr);
    G4ThreeVector posTmp; posTmp[0] =0.; posTmp[1] =0.; 
    zPosPart += fHorn2PartsLengths[kPart]/2;
    posTmp[2] = zPosPart;
//    std::cerr << " At Horn2 big part " << kPart << "zPos before placing " << zPosPart << std::endl;
    G4PVPlacement *vPart = new G4PVPlacement((G4RotationMatrix *) 0, posTmp, tubsL, nStr + std::string("_P"), 
  		     vH2->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC); 
    zPosPart += fHorn2PartsLengths[kPart]/2;
    // Sub volumes for each part. 
    switch (kPart) {
      case 0: // The Inner Outer Transition piece 
      {
        //The inner part. 
        const double zEndDrawing = fHorn2InnerIOTransLength;
	fHorn2ZEqnChanges.push_back(zEndDrawing); // To be used in the magnetic field class.
        int numSubSect = GetNumberOfInnerHorn2SubSections(6, zStartDrawing, 
                                                      zEndDrawing, 10); // These Z position are from the start of the inner conductor.   
        const double deltaZ = (zEndDrawing - zStartDrawing)/numSubSect;
//        std::cerr << " Number of subsection for the IO transition of Horn2 " << numSubSect 
//                  << " deltaz " << deltaZ << std::endl;
       for (int iSub = 0; iSub != numSubSect; iSub++) {					      
         const double zzBegin = zStartDrawing + iSub*deltaZ;
         const double zzEnd = zzBegin + deltaZ;
         std::ostringstream nameStrStr; nameStrStr << "Horn2InnerPartIOTransSub" << iSub;
         G4String nameStr(nameStrStr.str());
         const double rMin1 = fHorn2Equations[6].GetVal(zzBegin); // Equation 1
         const double rMin2 = fHorn2Equations[6].GetVal(zzEnd);
         const double rMax1 = fHorn2Equations[10].GetVal(zzBegin) + fWaterLayerThickInHorns + 0.0025; 
         const double rMax2 = fHorn2Equations[10].GetVal(zzEnd) + fWaterLayerThickInHorns + 0.0025;     
         G4Cons *aCons = new G4Cons(nameStr, rMin1, rMax1,rMin2, rMax2,
	                              (deltaZ - 0.005*mm)/2., 0., 360.0*deg);
         G4LogicalVolume *pCurrent = new G4LogicalVolume(aCons, G4Material::GetMaterial(std::string("Aluminum")), nameStr);
         G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.;
         posTmp[2] = zzBegin  + zShiftDrawingIOTrans + deltaZ/2.;			      
         G4PVPlacement *vSub = new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
                        vPart->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	
			
         if (fWaterLayerThickInHorns > 0.002*mm) {
           std::ostringstream nameStrStr; nameStrStr << "Horn2InnerPartIOTransSub" << iSub << "Water";
           G4String nameStr(nameStrStr.str());
           G4Cons *aCons = new G4Cons(nameStr, rMax1 - fWaterLayerThickInHorns, rMax1-0.001*mm,
                                         rMax2 - fWaterLayerThickInHorns, rMax2-0.001*mm,
	                              (deltaZ - 0.0075*mm)/2., 0., 360.0*deg);
           G4LogicalVolume *pCurrent = new G4LogicalVolume(aCons, G4Material::GetMaterial(std::string("Water")), nameStr);
           G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.; posTmp[2] =0.;			      
           new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
                        vSub->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	
         }
       } // on the number of sub section of the Inner Outer transition 
       // Outer parts 
       for (size_t kOut=0; kOut != fHorn2UpstrOuterIOTransLengths.size(); kOut++) {
          std::ostringstream nStrStr; nStrStr << "Horn2IOTransOuterPart" << kOut;
          G4String nStr(nStrStr.str());
          G4Tubs* tubsPart = new G4Tubs(nStr, fHorn2UpstrOuterIOTransRadsOne[kOut], 
	                                      fHorn2UpstrOuterIOTransRadsTwo[kOut], 
					      fHorn2UpstrOuterIOTransLengths[kOut]/2., 0., 360.0*deg);
          G4LogicalVolume *tubsL = new G4LogicalVolume(tubsPart, G4Material::GetMaterial("Aluminum"), nStr);
          G4ThreeVector posTmp; posTmp[0] =0.; posTmp[1] =0.; 
	  posTmp[2] = - fHorn2PartsLengths[kPart]/2 + fHorn2UpstrOuterIOTransPositions[kOut];
          new G4PVPlacement((G4RotationMatrix *) 0, posTmp, tubsL, nStr + std::string("_P"), 
  		     vPart->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC); 
       }
       // The weld at the end. Shift it a bit upstream such that it does not interfere 
       {
          G4String nStr("Horn2IOTransWeld");
	  const double weldLength = 0.386*in;
	  const double zzEnd = fHorn2LongRescale*5.8610*in - weldLength - 0.050*mm;
	  const double radius = fHorn2Equations[10].GetVal(zzEnd) + fWaterLayerThickInHorns + 0.0025; 
	  const double thick = 0.0775*in; 
          G4Tubs* tubsPart = new G4Tubs(nStr, radius, (radius+thick), weldLength/2., 0., 360.0*deg);
          G4LogicalVolume *tubsL = new G4LogicalVolume(tubsPart, G4Material::GetMaterial("Aluminum"), nStr);
          G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] =0.; 
	  posTmp[2] =  fHorn2PartsLengths[kPart]/2 - weldLength/2. - 0.025*mm;
          new G4PVPlacement((G4RotationMatrix *) 0, posTmp, tubsL, nStr + std::string("_P"), 
  		     vPart->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC); 
        }
	zStartDrawing += fHorn2InnerIOTransLength; // for the next section 
      } // End of Transition piece 
      break;
      case 2: // The neck.  Must subdivide further, radial equation keeps changing.
       // There will be 5 parts .. Drawing 8875.112-MD 363387
       
      {
        const double lengthNeck = fHorn2LongRescale*2.411*in - 0.020*mm; // approximate. Equation not available in usual form 
        fHorn2NeckLength = lengthNeck; // to compute the magnetic field 
        const double zPosNeck = fHorn2LongRescale*(39.8193 - 29.800)*in; // with respect to the start of the mother volume 
	fHorn2NeckZPosition = zPosNeck;
        // We start by the neck 
	{
          G4String nStr("Horn2Part2Neck");
	  const double radiusInner =  fHorn2RadialRescale*3.071*in/2.;
	  const double radiusOuter = fHorn2RadialRescale*3.465*in/2. + fWaterLayerThickInHorns + 0.0025; 
	  fHorn2NeckOuterRadius = fHorn2RadialRescale*3.465*in/2.;
	  fHorn2NeckInnerRadius = radiusInner;
          G4Tubs* tubsPart = new G4Tubs(nStr, radiusInner, radiusOuter, lengthNeck/2., 0., 360.0*deg );
          G4LogicalVolume *tubsL = new G4LogicalVolume(tubsPart, G4Material::GetMaterial("Aluminum"), nStr);
          G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] =0.; 
	  posTmp[2] =  -1.0*fHorn2PartsLengths[kPart]/2  + zPosNeck;
          G4PVPlacement *vNeck = new G4PVPlacement((G4RotationMatrix *) 0, posTmp, tubsL, nStr + std::string("_P"), 
  		     vPart->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC); 
         if (fWaterLayerThickInHorns > 0.002*mm) {
           G4String nameStrW(nStr); nameStrW += std::string("Water");
           G4Tubs* tubsPartW = new G4Tubs(nameStrW, radiusOuter -0.001*mm - fWaterLayerThickInHorns , 
	                                  radiusOuter - 0.001*mm , 
					  lengthNeck/2. - 0.002*mm, 0., 360.0*deg);
           G4LogicalVolume *pCurrent = new G4LogicalVolume(tubsPartW, G4Material::GetMaterial(std::string("Water")), nameStrW);
           G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.; posTmp[2] =0.;			      
           new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStrW + std::string("_P"), 
                        vNeck->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	
         }
		     
        } // Neck part per se.  
        {
	  double zStartDrawingsSubP2[] = {29.800, 36.8888, 41.0248, 43.3660}; // Note the third number is compromise, corresponding 
	                                                                      // to the length of the neck going ~2 " to 2.41 "	   
	  double zEndDrawingsSubP2[] = {36.8888, 38.6138, 43.3660, 49.820};
	  size_t eqnInner[] = {6, 6, 7, 7};
	  size_t eqnOuter[] = {0, 1, 2 ,3}; 
         // Now loop on these 4 subparts.  (this gona a bit tedious...) 
	  for (size_t kSub=0; kSub!=4; kSub++) {
	    const double zStartDrawing = fHorn2LongRescale*zStartDrawingsSubP2[kSub]*in;
            const double zEndDrawing =  fHorn2LongRescale*zEndDrawingsSubP2[kSub]*in;
            fHorn2ZEqnChanges.push_back(zStartDrawing);
	    // Need to insert the neck one.. to get consistent set to compute the magnetic field. 
	    if (kSub == 1) fHorn2ZEqnChanges.push_back(zEndDrawing);
	    const double zShiftTmp = zStartDrawing - fHorn2LongRescale*zStartDrawingsSubP2[0]*in;
            int numSubSect = GetNumberOfInnerHorn2SubSections(eqnInner[kSub], zStartDrawing, zEndDrawing, 10);   
            const double deltaZ = (zEndDrawing - zStartDrawing)/numSubSect;
//            std::cerr << " Number of subsection for the middle part of Horn2 index " << kPart 
//	          << " Upstream of the neck,  is " << numSubSect 
//                  << " deltaz " << deltaZ << std::endl;
            for (int iSub = 0; iSub != numSubSect; iSub++) {					      
              const double zzBegin = zStartDrawing + iSub*deltaZ;
              const double zzEnd = zzBegin + deltaZ;
              std::ostringstream nameStrStr; 
	      nameStrStr << "Horn2InnerPart" << kPart << "NeckkSub" << kSub << "iSub" << iSub;
              G4String nameStr(nameStrStr.str());
              const double rMin1 = fHorn2Equations[eqnInner[kSub]].GetVal(zzBegin); // Equation 1
              const double rMin2 = fHorn2Equations[eqnInner[kSub]].GetVal(zzEnd);
              const double rMax1 = fHorn2Equations[eqnOuter[kSub]].GetVal(zzBegin) + fWaterLayerThickInHorns + 0.0025; 
              const double rMax2 = fHorn2Equations[eqnOuter[kSub]].GetVal(zzEnd) + fWaterLayerThickInHorns + 0.0025;     
//              std::cerr << " Pre/Post Neck, zzbegin " << zzBegin << " radii " << 
//	                 rMin1 << " / " << rMax1 << " / " << rMin2 <<" / " << rMax2 << std::endl;
              G4Cons *aCons = new G4Cons(nameStr, rMin1, rMax1,rMin2, rMax2,
	                              (deltaZ - 0.005*mm)/2., 0., 360.0*deg);
              G4LogicalVolume *pCurrent = new G4LogicalVolume(aCons, G4Material::GetMaterial(std::string("Aluminum")), nameStr);
              G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.;
              posTmp[2] = -fHorn2PartsLengths[kPart]/2 + iSub*deltaZ + deltaZ/2. + zShiftTmp;			      
              G4PVPlacement *vSub = new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
                        vPart->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	
			
             if (fWaterLayerThickInHorns > 0.002*mm) {
               G4String nameStrW(nameStr); nameStrW += std::string("Water");
               G4Cons *aCons = new G4Cons(nameStrW, rMax1 - fWaterLayerThickInHorns, rMax1-0.001*mm,
                                         rMax2 - fWaterLayerThickInHorns, rMax2-0.001*mm,
	                              (deltaZ - 0.0075*mm)/2., 0., 360.0*deg);
               G4LogicalVolume *pCurrent = new G4LogicalVolume(aCons, G4Material::GetMaterial(std::string("Water")), nameStrW);
               G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.; posTmp[2] =0.;			      
               new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStrW + std::string("_P"), 
                        vSub->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	
             }
        }
       } // Parts surrounding the neck. 
       // Weld Only the downstream one. The upstream part is comprise in the previous volume 
       {
          std::ostringstream nameStrStr; nameStrStr << "Horn2NeckPart" << kPart << "WeldDownstr";
          G4String nameStr(nameStrStr.str());
	  const double weldLength = 2.0 * 0.386*in; // double the length to include the part on the next volume 
	  const double zzEndForWeld = zEndDrawingsSubP2[3] + 0.50*mm; 
	  const double radius = fHorn2Equations[8].GetVal(zzEndForWeld) + fWaterLayerThickInHorns + 0.0025; 
	  const double thick = 0.0775*in; 
          G4Tubs* tubsPart = new G4Tubs(nameStr, radius, (radius+thick), weldLength/2., 0., 360.0*deg);
          G4LogicalVolume *tubsL = new G4LogicalVolume(tubsPart, G4Material::GetMaterial("Aluminum"), nameStr);
          G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] =0.; 
	  posTmp[2] =  fHorn2PartsLengths[kPart]/2 - weldLength - 0.025*mm;
          new G4PVPlacement((G4RotationMatrix *) 0, posTmp, tubsL, nameStr + std::string("_P"), 
  		     vPart->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC); 
        }
        zStartDrawing =  fHorn2LongRescale*49.820*in;
      } 
      break;
     } // end of case 2 
     case 6: // the downstream end piece. simple tubes. 
      {
        {
         G4String nameStr("Horn2DownstrPart6");
	 const double length = fHorn2LongRescale*(21.054 - 1.25)*in - 0.020*mm;
	 const double radInner = fHorn2RadialRescale*21.103*in/2.;
	 const double radOuter = fHorn2RadialRescale*21.853*in/2.;
         G4Tubs* tubs = new G4Tubs(nameStr, radInner, radOuter, length/2., 0., 360.0*deg);
         G4LogicalVolume *tubsL = new G4LogicalVolume(tubs, G4Material::GetMaterial("Aluminum"), nameStr);
         G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] =0.; 
	 posTmp[2] =  -fHorn2PartsLengths[kPart]/2 + length/2. + 0.0125*mm;
         new G4PVPlacement((G4RotationMatrix *) 0, posTmp, tubsL, nameStr + std::string("_P"), 
  		     vPart->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC); 
	}
	 // the flange/connector 
	  
         {
         G4String nameStr("Horn2DownstrPart6Flange");
	 const double length = fHorn2LongRescale*(1.25)*in - 0.020*mm;
	 const double radInner = fHorn2RadialRescale*21.103*in/2.;
	 const double radOuter = fHorn2RadialRescale*25.040*in/2.;
         G4Tubs* tubs = new G4Tubs(nameStr, radInner, radOuter, length/2., 0., 360.0*deg);
         G4LogicalVolume *tubsL = new G4LogicalVolume(tubs, G4Material::GetMaterial("Aluminum"), nameStr);
         G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] =0.; 
	 posTmp[2] =  fHorn2PartsLengths[kPart]/2 - length/2. - 0.0125*mm;
         new G4PVPlacement((G4RotationMatrix *) 0, posTmp, tubsL, nameStr + std::string("_P"), 
  		     vPart->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC); 
		     
	}
	break;     
      }
      default: 
      { // One set of conical sections. + weld at the end
        const double zEndDrawing = zStartDrawing + fHorn2PartsLengths[kPart];
	size_t eqnInner = 6; // kPart =1 
	size_t eqnOuter = 0;
	if (kPart == 3 ) {
	  eqnOuter = 3;
	  fHorn2ZEqnChanges.push_back(zStartDrawing);
	  eqnInner = 7;
	} else if (kPart == 4) {
	  eqnOuter = 4;
	  fHorn2ZEqnChanges.push_back(zStartDrawing);
	  eqnInner = 8;
	} else if (kPart == 5) {
	  eqnOuter = 5;
	  fHorn2ZEqnChanges.push_back(zStartDrawing);
	  eqnInner = 9;
	  fHorn2ZEqnChanges.push_back(zEndDrawing);
	}
        int numSubSect = GetNumberOfInnerHorn2SubSections(eqnOuter, zStartDrawing, 
                                                      zEndDrawing, 10);   
        const double deltaZ = (zEndDrawing - zStartDrawing)/numSubSect;
//        std::cerr << " Number of subsection for the generic part of Horn2 index " << kPart << " is " << numSubSect 
//                  << " deltaz " << deltaZ << std::endl;
	double zzEndForWeld = 0.;	  
       for (int iSub = 0; iSub != numSubSect; iSub++) {					      
         const double zzBegin = zStartDrawing + iSub*deltaZ;
         const double zzEnd = zzBegin + deltaZ;
	 zzEndForWeld = zzEnd;
         std::ostringstream nameStrStr; nameStrStr << "Horn2InnerPart" << kPart << "Sub" << iSub;
         G4String nameStr(nameStrStr.str());
         const double rMin1 = fHorn2Equations[eqnInner].GetVal(zzBegin); // Equation 1
         const double rMin2 = fHorn2Equations[eqnInner].GetVal(zzEnd);
         const double rMax1 = fHorn2Equations[eqnOuter].GetVal(zzBegin) + fWaterLayerThickInHorns + 0.0025; 
         const double rMax2 = fHorn2Equations[eqnOuter].GetVal(zzEnd) + fWaterLayerThickInHorns + 0.0025;     
         G4Cons *aCons = new G4Cons(nameStr, rMin1, rMax1,rMin2, rMax2,
	                              (deltaZ - 0.005*mm)/2., 0., 360.0*deg);
         G4LogicalVolume *pCurrent = new G4LogicalVolume(aCons, G4Material::GetMaterial(std::string("Aluminum")), nameStr);
         G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.;
         posTmp[2] = -fHorn2PartsLengths[kPart]/2 + iSub*deltaZ + deltaZ/2.;			      
         G4PVPlacement *vSub = new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
                        vPart->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	
			
         if (fWaterLayerThickInHorns > 0.002*mm) {
           G4String nameStrW(nameStr); nameStrW += std::string("Water");
           G4Cons *aCons = new G4Cons(nameStrW, rMax1 - fWaterLayerThickInHorns, rMax1-0.001*mm,
                                         rMax2 - fWaterLayerThickInHorns, rMax2-0.001*mm,
	                              (deltaZ - 0.0075*mm)/2., 0., 360.0*deg);
           G4LogicalVolume *pCurrent = new G4LogicalVolume(aCons, G4Material::GetMaterial(std::string("Water")), nameStrW);
           G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.; posTmp[2] =0.;			      
           new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStrW + std::string("_P"), 
                        vSub->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);	
         }
        
      }
        // The weld at the end. Shift it a bit upstream such that it does not interfere 
       {
          std::ostringstream nameStrStr; nameStrStr << "Horn2InnerPart" << kPart << "Weld";
          G4String nameStr(nameStrStr.str());
	  const double weldLength = 2.0 * 0.386*in; // double to include the next one.. Perhaps a bit oversized. 
	  if (kPart == 1) zzEndForWeld -= weldLength - 0.50*mm; // oversizing a bit by moving while computing the radius
	  else zzEndForWeld += 0.50*mm; 
	  const double radius = fHorn2Equations[eqnOuter].GetVal(zzEndForWeld) + fWaterLayerThickInHorns + 0.0025; 
	  const double thick = 0.0775*in; 
          G4Tubs* tubsPart = new G4Tubs(nameStr, radius, (radius+thick), weldLength/2., 0., 360.0*deg);
          G4LogicalVolume *tubsL = new G4LogicalVolume(tubsPart, G4Material::GetMaterial("Aluminum"), nameStr);
          G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] =0.; 
	  posTmp[2] =  fHorn2PartsLengths[kPart]/2 - weldLength/2. - 0.025*mm;
          new G4PVPlacement((G4RotationMatrix *) 0, posTmp, tubsL, nameStr + std::string("_P"), 
  		     vPart->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC); 
        }
     
      zStartDrawing += fHorn2PartsLengths[kPart]; // for the next section 
     }
    } //end of special cases. 
    // Outer tube (not for k=0 ) 
    if (kPart != 0) {
      std::ostringstream nStrStr; nStrStr << "Horn2OuterConductor" << kPart;
      G4String nStr(nStrStr.str());
      G4Tubs* tubsPart = new G4Tubs(nStr, fHorn2OuterTubeInnerRad, fHorn2OuterTubeOuterRad,
         (fHorn2PartsLengths[kPart]/2 - 0.020*mm ), 0., 360.0*deg);
      G4LogicalVolume *tubsL = new G4LogicalVolume(tubsPart, G4Material::GetMaterial("Air"), nStr);
      G4ThreeVector posTmp; posTmp[0] =0.; posTmp[1] =0.; posTmp[2] =0.; 
      new G4PVPlacement((G4RotationMatrix *) 0, posTmp, tubsL, nStr + std::string("_P"), 
  		     vPart->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC); 
    }
    // Eventual Hangers. only one set 
    if (kPart == 3) this->Horn2InstallSpiderHanger(nStr, vPart);
    
    // More 
  } // Loop over the parts, daughters of top level. 
  // Final flange downstream flange Approximate Drawing 363382-E1  Length is still a guess.. 
  G4String nStr("Horn2DownstrOuterFlange");
  G4Tubs* tubsOF1 = new G4Tubs(nStr, fHorn2OuterTubeOuterRad + 1.0*mm, 
                                fHorn2OuterTubeOuterRadMax-1.0*mm, 0.50*in, 0., 360.0*deg);
  G4LogicalVolume *tubsL = new G4LogicalVolume(tubsOF1, G4Material::GetMaterial("Aluminum"), nStr);
  G4ThreeVector posTmp; posTmp[0] =0.; posTmp[1] =0.;
  posTmp[2] = plInfoH2Top->fParams[2]/2 - fHorn2LongRescale*0.6*in; // approximate location
  new G4PVPlacement((G4RotationMatrix *) 0, posTmp, tubsL, nStr + std::string("_P"), 
  		     vH2->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);   
}


// Clone for Horn1.. Could avoid a bit of code bloat, but that's not much... 
int LBNEVolumePlacements::GetNumberOfInnerHorn2SubSections(size_t eqn, double z1, double z2, int nMax) const {

   int numSubSect = 1;
   double zEnd = z2;
   const double zLengthTotal = z2 - z1;
   double zLength = zLengthTotal;
   while (true) { 
     const double r0 = fHorn2Equations[eqn].GetVal(z1);
     const double r1 = fHorn2Equations[eqn].GetVal(zEnd);
     const double rMid = fHorn2Equations[eqn].GetVal((z1 + zEnd)/2.);
//     std::cerr << " GetNumberOfInnerHorn2SubSections z1 " << z1 << " zEnd " << zEnd << 
//                     " r0 " << r0 << " r1 " << r1 << " rMid " << rMid << std::endl;
     if (std::abs(rMid - 0.5*(r0+r1)) < 0.050*mm) break; // 50 microns good enough for Gov. work. 
                                                         // meachanical tolerance is one 5 mills, (127 microns)
     zLength /=2.;
     zEnd = z1 + zLength;
     numSubSect++;
     if (numSubSect > nMax) {
       G4Exception("LBNEVolumePlacements::PlaceFinalHorn2", " ", FatalErrorInArgument, " Crazy subdivision! ");
       break;
     }
   }
   return numSubSect;
}
// Clone from Horn1 .. But simpler, less arguments because there is one set of such contraptions. 

void LBNEVolumePlacements::Horn2InstallSpiderHanger(const G4String &nameStrH, G4PVPlacement *vMother ) {

  const double in = 2.54*cm;
  G4String nameStr(nameStrH);nameStr += G4String("Ring");
  // Drawing 8875.112-MD 363388 and 363405
  const double length = 0.750*in*fHorn2LongRescale;
  const int eqnNum = 3;
  const double zLocDrawing = fHorn2LongRescale*60.4681*in;
  const double rTmp1 = fHorn2Equations[eqnNum].GetVal(zLocDrawing) + 0.015*mm + fWaterLayerThickInHorns;
  const double rTmp2 = fHorn2RadialRescale*11.625*in + fWaterLayerThickInHorns + 0.015*mm; // 
  G4Tubs *aTubs = new G4Tubs(nameStr, rTmp1, rTmp2, length/2.   , 0., 360.0*deg);
  G4LogicalVolume *pCurrent = new G4LogicalVolume(aTubs, G4Material::GetMaterial(std::string("Aluminum")), nameStr);
  G4ThreeVector posTmp; posTmp[0] = 0.; posTmp[1] = 0.;
  posTmp[2] = fHorn2LongRescale*in*(-0.0180); // computed from drawing 8875.112MD-363388 			   
  new G4PVPlacement((G4RotationMatrix *) 0, posTmp, pCurrent, nameStr + std::string("_P"), 
  		     vMother->GetLogicalVolume(), false, 1, fCheckVolumeOverLapWC);    
  
  //The connecting piece ring to the hangers.. There are three of them, at 120 degrees from each other. 
  
  G4String nameStr2(nameStrH); nameStr2 += G4String("Riser");
  const double heightRiser = 0.333*in - 0.020*mm;
  const double widthH = 1.5*in; // See drawing 8875.112-MD 363115
  const double thickH = 0.184*2*in; 
  G4Box *aBoxRiser = new G4Box(nameStr2, widthH/2., heightRiser/2.0, thickH/2.0);  
  G4LogicalVolume *pCurrentRiser = 
    new G4LogicalVolume(aBoxRiser, G4Material::GetMaterial(std::string("Aluminum")), nameStr2);
    
  G4String nameStr3(nameStrH); nameStr3 += G4String("Hanger");
  const double heightH = fHorn2OuterTubeInnerRad - rTmp2 - 1.0*mm - heightRiser;
  const double widthH2 = 1.0*in; // 363115 Note: we collapsed both hanger along the horizontal, transverse 
                                // direction. 
  const double thickH2 = 0.031*in; 
  G4Box *aBoxHanger = new G4Box(nameStr3, widthH2/2., heightH/2.0, thickH2/2.0);  
  G4LogicalVolume *pCurrentHanger = 
    new G4LogicalVolume(aBoxHanger, G4Material::GetMaterial(std::string("Aluminum")), nameStr3);
  
  for (int iRot=0; iRot != 3; iRot++) {
    std::ostringstream rnStrStr; rnStrStr << "_" << (iRot+1);
    G4String rnStr(rnStrStr.str());
  // Same Z position as above... 
    G4RotationMatrix * rMatPr = 0;
     if (iRot == 1) {
      rMatPr = new G4RotationMatrix; 
      rMatPr->rotateZ(2.0*M_PI/3.);
    } else if (iRot == 2) {
      rMatPr = new G4RotationMatrix; 
      rMatPr->rotateZ(-2.0*M_PI/3.);
    }
    
    const double dHRiser = rTmp2 + 0.010*mm + heightRiser/2.;
    posTmp[0] = 0.;  posTmp[1] = dHRiser;
    if (iRot != 0) {
      posTmp[0] = dHRiser*rMatPr->xy();
      posTmp[1] = dHRiser*rMatPr->yy();
    }
    if (iRot == 0) { 
       new G4PVPlacement(rMatPr, posTmp, pCurrentRiser, nameStr2 + std::string("_P") + rnStr, 
  		     vMother->GetLogicalVolume(), false, iRot, fCheckVolumeOverLapWC);
    } else {
       new G4PVPlacement(G4Transform3D(*rMatPr, posTmp), pCurrentRiser, nameStr2 + std::string("_P") + rnStr, 
  		     vMother->GetLogicalVolume(), false, iRot, fCheckVolumeOverLapWC);
    }
// Now the hanger it self 
    
    const double dHHanger = rTmp2 + 0.010*mm + 0.5*mm + heightRiser + heightH/2.;
    posTmp[0] = 0.;  posTmp[1] = dHHanger;
    if (iRot != 0) {
      posTmp[0] = dHHanger*rMatPr->xy();
      posTmp[1] = dHHanger*rMatPr->yy();
    }
  // Same Z position as above... 
    if (iRot == 0) { 
       new G4PVPlacement(rMatPr, posTmp, pCurrentHanger, nameStr3 + std::string("_P") + rnStr, 
  		     vMother->GetLogicalVolume(), false, iRot, fCheckVolumeOverLapWC);    
     } else {
       new G4PVPlacement(G4Transform3D(*rMatPr, posTmp), pCurrentHanger, nameStr3 + std::string("_P") + rnStr, 
  		     vMother->GetLogicalVolume(), false, iRot, fCheckVolumeOverLapWC);
     } 
 } // on the 120 degree symmetry point.
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
