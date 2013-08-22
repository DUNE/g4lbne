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
    std::map<G4String, LBNEVolumePlacementData>::iterator itTargetUpstrM1 = fSubVolumes.find(G4String("TargetUpstrM1"));
    LBNEVolumePlacementData *plM1 = &itTargetUpstrM1 ->second;
    G4ThreeVector posTmp;
    posTmp[0] = 0.; // The alignment rings are always centered.. 
    posTmp[1] = 0.; // We start upstream to keep the separation among rings. 
    posTmp[2] = -1.0*plM1->fParams[2]/2.0 + infoTmpRLeft.fParams[2]/2. + 1.0*mm; // 1 mm spacing Left and right have the same thickness. 
    int copyNumber = 0;
    int copyNumberHere = 0;
    while (true) {
      if ( posTmp[2] < (plM1->fParams[2]/2.0 - 1.0*mm)) {
         posTmp[2] += fTargetAlignRingSpacing;      
         copyNumber++;
	 
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
        if ( copyNumber == (fMaxNumAlignRings-1)) break;
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
    std::map<G4String, LBNEVolumePlacementData>::iterator itTargSeg = fSubVolumes.find(G4String("TargetUpstrDownstrSegment"));
    LBNEVolumePlacementData  *plTargetUpstrDownstrSegment= &itTargSeg->second;    
    
    double zCoordTmp = infoTargSegFirst.fPosition[2] + infoTargSegFirst.fParams[2]/2. + 
                       plTargetUpstrDownstrSegment->fParams[2]/2. + 0.002*mm;	       
    for (int iSeg=0; iSeg != fTargetNumFinsInHorn; iSeg++) { // Place with no misalignment
      posTmp[0] = 0.; posTmp[1] = 0.;
      posTmp[2] += (fTargetFinLength + fTargetFinSpacingLength);
      posTmp[2] =  zCoordTmp;
      std::ostringstream cNumStrStr; cNumStrStr << "_P" << iSeg;
//      G4PVPlacement *vSeg = new G4PVPlacement((G4RotationMatrix *) 0, 
      G4PVPlacement((G4RotationMatrix *) 0, 
	                            posTmp, plTargetUpstrDownstrSegment->fCurrent, 
				      G4String("Horn1TargetSegment")+cNumStrStr.str(), 
				          vMHe->GetLogicalVolume(), false, iSeg, fCheckVolumeOverLapWC);
      zCoordTmp += (fTargetFinLength + fTargetFinSpacingLength);
    }
    // Now the end 
    LBNEVolumePlacementData *plCoolingTubeReturn = Create("Horn1TargetCoolingTubeHLast");
    posTmp[0] = 0.; posTmp[1] = fTargetFinHeight/2.; posTmp[2] = 0.;			  
    G4PVPlacement(&plCoolingTubeReturn->fRotation, 
	                            posTmp, plCoolingTubeReturn->fCurrent, 
				    G4String("Horn1TargetCoolingTubeHLast_PTop"), 
				          vMHe->GetLogicalVolume(), false, 0, fCheckVolumeOverLapWC);
    posTmp[1] = -1.0*fTargetFinHeight/2.;			  
    vTubeDown =  new G4PVPlacement(&plCoolingTubeReturn->fRotation, 
	                            posTmp, plCoolingTubeReturn->fCurrent, 
				    G4String("Horn1TargetCoolingTubeHLast_PTop"), 
				          vMHe->GetLogicalVolume(), false, 0, fCheckVolumeOverLapWC);
    
    PlaceFinal("Horn1TargetCoolingTubeHLastWater", vTubeDown);
    G4PVPlacement *vCL = PlaceFinal("Horn1TargetCoolingTubeVLast", vMHe);					  				  
    PlaceFinal("Horn1TargetCoolingTubeVLastWater", vCL);					  				  
    PlaceFinal("Horn1TargetDownstrHeContainerCap", vMHe);					  				  
}
