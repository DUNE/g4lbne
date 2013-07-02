//----------------------------------------------------------------------
// $Id: LBNEVolumePlacement.cc,v 1.1.2.1 2013/07/02 13:12:21 robj137 Exp $
//----------------------------------------------------------------------

#include <fstream>

#include "LBNEDetectorConstruction.hh"
#include "LBNEDataInput.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UnitsTable.hh"

#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4PVReplica.hh"
#include "G4AssemblyVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"

#include "G4RegionStore.hh"
#include "G4SolidStore.hh"
#include "G4GeometryManager.hh"
#include "G4FieldManager.hh"

#include "G4RunManager.hh"

#include "G4VisExtent.hh"

#include <map>

//---------------------------------------------------------------------------// 
// Constructor, Destructor, and Initialization
//---------------------------------------------------------------------------// 

LBNEDetectorConstruction::LBNEDetectorConstruction()
{
  Initialize();
}


LBNEDetectorConstruction::~LBNEDetectorConstruction()
{
}

void LBNEDetectorConstruction::Initialize()
{
 
  // Useful measurements (these not to be the same as in
  // LBNEDetectorConstruction, and so obviously should only be set once!) FIXME
  // Also, I believe we want to keep the origin of the world to be the 
  fTargetHallZ = 28.2*m;
  
  // Volumes to place:

  // Decay Pipe
  fMotherVolumeMap["DecayPipe"] = fDecayHallLogical;
  fSubVolumeRotations["DecayPipe"] = G4RotationMatrix(0);
  fSubVolumeTranslation["DecayPipe"] = G4ThreeVector(0,0,0);
  
  // Hadron absorber
  
  // standard person
  // horn 2
  // horn 1
  // target



}


//-------------------------------------------------------------------------



