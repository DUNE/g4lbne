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
    // they have been already defined while dealing with the Usptream target portion 
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
   fHorn1IOTransLength *= fHorn1LongRescale;  
  for (size_t k=0; k != fHorn1UsptrOuterIOTransInnerLengths.size(); ++k)
      fHorn1UsptrOuterIOTransInnerLengths[k] *= fHorn1LongRescale; 
      
   for (size_t k=0; k != fHorn1UsptrOuterIOTransInnerPositions.size(); ++k)
      fHorn1UsptrOuterIOTransInnerPositions[k] *= fHorn1LongRescale; 
      
  for (size_t k=0; k != fTargetHorn1Lengths.size(); ++k) 
      fTargetHorn1Lengths[k] *= fHorn1LongRescale;
   for (size_t k=0; k != fTargetHorn1ZPositions.size(); ++k) 
      fTargetHorn1ZPositions[k] *= fHorn1LongRescale;  // This will be approximate...
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
  
  for (size_t k=0; k != fHorn1UsptrOuterIOTransInnerRads.size(); ++k)
      fHorn1UsptrOuterIOTransInnerRads[k] *= fHorn1RadialRescale; 
      
  for (size_t k=0; k != fTargetHorn1InnerRadsUpstr.size(); ++k) 
      fTargetHorn1InnerRadsUpstr[k] *= fHorn1RadialRescale;
  for (size_t k=0; k != fTargetHorn1InnerRadsUpstr.size(); ++k) 
      fTargetHorn1InnerRadsUpstr[k] *= fHorn1RadialRescale;
  for (size_t k=0; k != fTargetHorn1InnerRadsDownstr.size(); ++k) 
      fTargetHorn1InnerRadsDownstr[k] *= fHorn1RadialRescale;

  for (size_t k=0; k != fHorn1UsptrInnerRadsUpstr.size(); ++k) 
      fHorn1UsptrInnerRadsUpstr[k] *= fHorn1RadialRescale;
  for (size_t k=0; k != fHorn1UsptrInnerRadsUpstr.size(); ++k) 
      fHorn1UsptrInnerRadsUpstr[k] *= fHorn1RadialRescale;
//  for (size_t k=0; k != fHorn1UsptrOuterIOTransInnerRad.size(); ++k) 
//      fHorn1UsptrOuterIOTransInnerRad[k] *= fHorn1RadialRescale;     ???????????????????/
   
  for (size_t k=0; k != fHorn1UsptrInnerRadsOuterUpstr.size(); ++k)
    fHorn1UsptrInnerRadsOuterUpstr[k] *= fHorn1RadialRescale;
//  for (size_t k=0; k != fHorn1UpstrInnerRadsOuterDownstr.size(); ++k)
//    fHorn1UpstrInnerRadsOuterDownstr[k] *= fHorn1RadialRescale;
      
//  for (size_t k=0; k != fHorn1SectionInnerRads.size(); ++k) 
//        fHorn1SectionInnerRads[k] *= fHorn1RadialRescale; 
	
	 
//  fHorn1OuterTubeInnerRad  *= fHorn1RadialRescale; // Check variable names ... ??????????????????????
//  fHorn1OuterConnectorRad *= fHorn1RadialRescale;
//  fHorn1InnerConnectorRad *= fHorn1RadialRescale;
  
//  fHorn1InnerTopUpstrInnerRad *= fHorn1RadialRescale;
//  fHorn1InnerTopUpstrOuterRad *= fHorn1RadialRescale;
  
}  

void LBNEVolumePlacements::DeclareHorn1Dims() {
  
  const double in = 2.54*cm;
  fHorn1IOTransLength = 3.0*cm + 3.316*in + 0.005*mm; 
                          // Drawing 8875.112 -MD-363097 The 3 cm is the MCZERO offset, per verbal discussion 
                              // with J. Hylen. The 5 microns if to avoid G4 volume overlaps.  
			      
  fHorn1RadialSafetyMargin = 2.9*mm; // per agreement between Jim H. and Alberto M., Aug. 22 2013. 
  
  fHorn1IOTransInnerRad = 2.520*in/2. - fHorn1RadialSafetyMargin/2. ; // last term is the 
  fHorn1IOTransOuterRad = 16.250*in/2.;
  
  fHorn1UsptrInnerRadsUpstr.resize(4);
  fHorn1UpstrInnerRadsDownstr.resize(4);
  fHorn1UsptrInnerRadsOuterUpstr.resize(4);
  fHorn1UpstrInnerRadsOuterDownstr.resize(4);
  fHorn1UpstrLengths.resize(4);
  fHorn1UpstrZPositions.resize(4);
  
  fHorn1UsptrInnerRadsUpstr[0] = 1.572*in; 
  fHorn1UsptrInnerRadsOuterUpstr[0] = fHorn1UsptrInnerRadsUpstr[0] + 0.32*in; 
  fHorn1UpstrInnerRadsDownstr[0] = 1.41*in; 
  fHorn1UpstrInnerRadsOuterDownstr[0] = fHorn1UpstrInnerRadsDownstr[0] + 0.32*in; 
  fHorn1UpstrLengths[0] = 0.508*in - 0.010*mm;
  fHorn1UpstrZPositions[0] = fHorn1UpstrLengths[0]/2.; // With respect to the beginning of mother volume.  
  
  fHorn1UsptrInnerRadsUpstr[1] = fHorn1UpstrInnerRadsDownstr[0]; 
  fHorn1UsptrInnerRadsOuterUpstr[1] = fHorn1UsptrInnerRadsUpstr[1] + 0.32*in; 
  fHorn1UpstrInnerRadsDownstr[1] = 1.288*in; 
  fHorn1UpstrInnerRadsOuterDownstr[1] = fHorn1UpstrInnerRadsDownstr[1] + 0.3*in; 
  fHorn1UpstrLengths[1] = 0.639*in  - 0.010*mm;
  fHorn1UpstrZPositions[1] = fHorn1UpstrZPositions[0] + 0.010*mm + fHorn1UpstrLengths[1]/2.;
  
  fHorn1UsptrInnerRadsUpstr[2] = fHorn1UpstrInnerRadsDownstr[1]; 
  fHorn1UsptrInnerRadsOuterUpstr[2] = fHorn1UsptrInnerRadsUpstr[2] + 0.28*in; 
  fHorn1UpstrInnerRadsDownstr[2] = 1.268*in; 
  fHorn1UpstrInnerRadsOuterDownstr[2] = fHorn1UpstrInnerRadsDownstr[2] + 0.118*in; 
  fHorn1UpstrLengths[2] = 3.323*in  - 0.010*mm;
  fHorn1UpstrZPositions[2] = fHorn1UpstrZPositions[1] + 0.010*mm + fHorn1UpstrLengths[2]/2.;
  
  fHorn1UsptrInnerRadsUpstr[3] = fHorn1UpstrInnerRadsDownstr[2]; 
  fHorn1UsptrInnerRadsOuterUpstr[2] = fHorn1UsptrInnerRadsUpstr[2] + 0.118*in; 
  fHorn1UpstrInnerRadsDownstr[3] =  fHorn1UsptrInnerRadsUpstr[3]; 
  fHorn1UpstrInnerRadsOuterDownstr[3] = fHorn1UpstrInnerRadsDownstr[3] + 0.20*in; 
  fHorn1UpstrLengths[3] = 0.410*in - 0.010*mm;
  fHorn1UpstrZPositions[3] = fHorn1UpstrZPositions[2] + 0.010*mm + fHorn1UpstrLengths[3]/2.;
  
  
 //
 // Now the top level sections. Only two of them, alignable. 
 // 
 // The first one goes up the downstream end of the IO transition peace 
 // and up the downstream end of the target. If the target is completly out of the horn, 
 // it is the part before the neck. (Drawing 8875.112-MD-363104) This is a tube, set by the 
 // 
 // 
 // The 2nd top lelvel section is the remaining part of that part, up to the end of the Horn1Hall Z location. 
 // This is just a tube. 
 //
 
 // Then, conical sections, all in a tube that has an inner radius of the neck, our radius 
 // 
 // To compute all that, we need to locate the target first.. 
 // So, done at construction time. 
 //
  
}


void LBNEVolumePlacements::PlaceFinalHorn1(G4PVPlacement *mother) {
//
// Start with upstream Inner to Out transition. This one is surveyable (as well as optionally rescaled. )
//
   LBNEVolumePlacementData *plTrUpst = this->Create("fHorn1IOTransCont");
   G4PVPlacement *vTrUpst = this->PlaceFinal("fHorn1IOTransCont", mother);


}
void LBNEVolumePlacements::RescaleHorn2Lengthwise() {
  
}
  
void LBNEVolumePlacements::RescaleHorn2Radially() {
  
  
}
