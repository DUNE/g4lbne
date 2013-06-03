
//C++
#include <stdio.h>
#include <math.h>

//Local and Geant4
//#include "LBNEDetectorConstruction.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
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
#include "G4AssemblyVolume.hh"

#include "LBNEDataInput.hh"
#include "LBNEDecayPipe.hh"

LBNEDecayPipe::LBNEDecayPipe(G4String detName):LBNESubVolume(detName)
{
  fDecayPipeLength = 204.0*m;
  fDecayPipeRadius = 2.0*m;
  fDecayPipeWallThickness = 1.27*cm;
  fMessenger = new LBNEDecayPipeMessenger(this);
}

LBNEDecayPipe::~LBNEDecayPipe()
{
  delete fMessenger;
}


void LBNEDecayPipe::ConstructSubvolume()
{
  // Decay Pipe
  
  G4cout << "Constructing the decay pipe! " << G4endl;
  G4ThreeVector decayPipePosition = G4ThreeVector(0,0,0);

  G4Tubs* decayPipeSolid = new G4Tubs("DecayPipeSolid", 0, fDecayPipeRadius,
                                      fDecayPipeLength/2, 0, 360*deg);
  
  G4LogicalVolume* decayPipeLogical =  new G4LogicalVolume(decayPipeSolid,
                                            G4Material::GetMaterial("Slab_Stl"), 
                                            "DecayPipeLogical", 0,0,0);
  
  G4Tubs* decayVolumeSolid = new G4Tubs("DecayVolumeSolid", 0,
                                      fDecayPipeRadius-fDecayPipeWallThickness,
                                      fDecayPipeLength/2-fDecayPipeWinThickness,
                                      0, 360*deg);
  
  G4LogicalVolume* decayVolumeLogical =  new G4LogicalVolume(decayVolumeSolid,
                                                G4Material::GetMaterial("Air"), 
                                                "DecayVolumeLogical", 0,0,0);
  
  G4VPhysicalVolume* decayVolumePhysical;
  decayVolumePhysical = new G4PVPlacement(0,decayPipePosition,
                                          decayVolumeLogical,
                                          "DecayVolumePhysical",
                                          decayPipeLogical, 0,0,0);

  fSubVolumeLogical = decayPipeLogical;
}

LBNEDecayPipeMessenger::LBNEDecayPipeMessenger(LBNESubVolume *subVolume)
  :LBNESubVolumeMessenger(subVolume) 
{
  fDecayPipe = (LBNEDecayPipe*)subVolume; 

  fDecayPipeLengthCmd = 
    new G4UIcmdWithADoubleAndUnit(G4String(fDirectoryName+"pipeLength"),this);
  fDecayPipeLengthCmd->SetGuidance("Set the length of the decay pipe");
  fDecayPipeLengthCmd->SetUnitCategory("Length");

  fDecayPipeRadiusCmd = 
    new G4UIcmdWithADoubleAndUnit(G4String(fDirectoryName+"pipeRadius"),this);
  fDecayPipeRadiusCmd->SetGuidance("Set the length of the decay pipe");
  fDecayPipeRadiusCmd->SetUnitCategory("Radius");

  fDecayPipeWindowThicknessCmd = 
    new
    G4UIcmdWithADoubleAndUnit(G4String(fDirectoryName+"pipeWindowThickness"),this);
  fDecayPipeWindowThicknessCmd->SetGuidance("Set the length of the decay pipe");
  fDecayPipeWindowThicknessCmd->SetUnitCategory("WindowThickness");

  fDecayPipeWallThicknessCmd = 
    new G4UIcmdWithADoubleAndUnit(G4String(fDirectoryName+"pipeWallThickness"),this);
  fDecayPipeWallThicknessCmd->SetGuidance("Set the length of the decay pipe");
  fDecayPipeWallThicknessCmd->SetUnitCategory("WallThickness");

}

LBNEDecayPipeMessenger::~LBNEDecayPipeMessenger()
{
  delete fDecayPipeLengthCmd;
  delete fDecayPipeRadiusCmd;
  delete fDecayPipeWindowThicknessCmd;
  delete fDecayPipeWallThicknessCmd;
}

void LBNEDecayPipeMessenger::SetNewValue(G4UIcommand* command, G4String val)
{
  if(command == fDecayPipeLengthCmd){
    fDecayPipe->SetDecayPipeLength(fDecayPipeLengthCmd->GetNewDoubleValue(val));
  } else if(command == fDecayPipeRadiusCmd){
    fDecayPipe->SetDecayPipeRadius(fDecayPipeLengthCmd->GetNewDoubleValue(val));
  } else if(command == fDecayPipeWindowThicknessCmd){
    fDecayPipe->SetDecayPipeWindowThickness(fDecayPipeLengthCmd->GetNewDoubleValue(val));
  } else if(command == fDecayPipeWallThicknessCmd){
    fDecayPipe->SetDecayPipeThickness(fDecayPipeLengthCmd->GetNewDoubleValue(val));
  }
}
