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

#include "G4RegionStore.hh"
#include "G4SolidStore.hh"
#include "G4GeometryManager.hh"
#include "G4FieldManager.hh"

#include "G4RunManager.hh"

#include "G4VisExtent.hh"

//---------------------------------------------------------------------------// 
// Handles the tricky volume placements 
//---------------------------------------------------------------------------// 
LBNEVolumePlacements::LBNEVolumePlacements() {
    
    fTargetHallZ = 28.2*m;
    fTargetHallZ = 0.; // ???????????
}

LBNEVolumePlacements::CreateLogicalAndLocate(const G4String &name,  G4PVPlacement *inPlaceMother, AlignmentAlgo model) {

  // look for a pre-existing placement.  If there, fatal error, exit 
  

}

LBNEPlacementMessenger::LBNEPlacementMessenger() {

   LBNEVolumePlacements* volP=LBNEVolumePlacements::Instance();
   
   DecayPipeLength  = new G4UIcmdWithADoubleAndUnit("/LBNE/det/decayPipeLength",this);
   DecayPipeLength->SetGuidance("Length of the decay Pipe");
   DecayPipeLength->SetParameterName("decayPipeLength",true);
   double value = 250.;
   volP->SetDecayPipeLength(value);
   DecayPipeLength->SetDefaultValue (value);
   DecayPipeLength->SetDefaultUnit ("m");
   DecayPipeLength->SetUnitCandidates ("cm m");
   DecayPipeLength->AvailableForStates(G4State_PreInit, G4State_Idle);

   WaterLayerThickInHorn  = new G4UIcmdWithADoubleAndUnit("/LBNE/det/waterThickInHorn",this);
   WaterLayerThickInHorn->SetGuidance("Water Thicknes on the inner conductor of the horn");
   WaterLayerThickInHorn->SetParameterName("waterThickInHorn",true);
   WaterLayerThickInHorn->SetDefaultValue (0.);
   WaterLayerThickInHorn->SetDefaultUnit ("mm");
   WaterLayerThickInHorn->SetUnitCandidates ("mm cm m");
   WaterLayerThickInHorn->AvailableForStates(G4State_PreInit, G4State_Idle);
}
LBNEPlacementMessenger::SetNewValue(G4UIcommand* command,G4String newValue) {
   
   LBNEVolumePlacements* volP=LBNEVolumePlacements::Instance();
   if (command == WaterLayerThickInHorn) {
     G4UIcmdWithADoubleAndUnit* cmdWD = dynamic_cast<G4UIcmdWithADoubleAndUnit*> (command);
     volP->SetWaterLayerThickInHorns(cmdWD->GetNewDoubleValue(newValue));
//     std::cerr << " Changed WaterLayerThickInHorns , o.k. " << std::endl;  // exit(2);
   }
   if (command == DecayPipeLength) {
     G4UIcmdWithADoubleAndUnit* cmdWD = dynamic_cast<G4UIcmdWithADoubleAndUnit*> (command);
     volP->SetDecayPipeLength(cmdWD->GetNewDoubleValue(newValue));
   }
    
   

}



