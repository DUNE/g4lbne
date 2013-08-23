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
    posTmp[0] = plCoolingTubeReturn->fParams[0]; posTmp[1] = fTargetFinHeight/2.; 
    posTmp[2] = plCoolingTubeReturn->fParams[2];			  
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
