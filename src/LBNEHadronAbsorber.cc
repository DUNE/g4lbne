
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
#include "LBNEHadronAbsorber.hh"

LBNEHadronAbsorber::LBNEHadronAbsorber(G4String detName):LBNESubDetector(detName)
{
  fMessenger = new LBNEHadronAbsorberMessenger(this);
}

LBNEHadronAbsorber::~LBNEHadronAbsorber()
{
  delete fMessenger;
}


void LBNEHadronAbsorber::ConstructSubdetector()
{
  // Hadron Absorber
  
  G4cout << "Constructing the Hadron Absorber" << G4endl;
  G4ThreeVector absorberPosition = G4ThreeVector(0,0,0);
  

//---------------------------------------------------------------------------// 
/*      
  So the absorber is a block. The outer block is concrete, the mid block is
  steel, and there's aluminum and i guess air in there somewhere as well. For
  construction, the outer volume will be air, and then nested shells for the
  concrete, steel, and aluminum.

*/
//---------------------------------------------------------------------------// 
//---------------------------------------------------------------------------// 
// Dimensions
//---------------------------------------------------------------------------// 

//---------------------------------------------------------------------------// 
  G4double eps = 1.0e-6*cm;

  fAirX = 12.5*m;
  fAirY = 12.5*m;
  fAirZ = 10.5*m;

  fConcreteX = 12.0*m;
  fConcreteY = 12.0*m;
  fConcreteZ = 10.0*m;
  
  fSteelX = 6.6*m;
  fSteelY = 6.6*m;
  fSteelZ = 6.6*m;
  
  fAlX = 2.0*m;
  fAlY = 2.0*m;
  fAlZ = 2.0*m;

  fCavityX = 15*cm;
  fCavityY = 15*cm;
  fCavityZ = 15*cm;

  // Displacements
  G4ThreeVector cavityDisplacement(0,0,-fAlZ/2 + fCavityZ/2 - eps);
  G4ThreeVector alumDisplacement(0,0,-fSteelZ/2 + fAlZ/2 - eps);
  G4ThreeVector steelDisplacement(0,0,-fConcreteZ/2 + fSteelZ/2 - eps);
  G4ThreeVector concreteDisplacement(0,0,-fAirZ/2 + fConcreteZ/2 - eps);

  // Placements
  G4ThreeVector concretePlacement(0,0,0);       
  G4ThreeVector steelPlacement(0,0, -fConcreteZ/2 + fSteelZ/2);
  G4ThreeVector alumPlacement(0,0,-fSteelZ/2 + fAlZ/2);
  
  // Base Solids
  G4Box* absorberAirSolid = new G4Box("absorberAirSolid", 
                                      fAirX/2, fAirY/2, fAirZ/2);
  
  G4Box *aluminumBlock = new G4Box("aluminumBlock", fAlX/2, fAlY/2, fAlZ/2);
                                                  

  G4Box *steelBlock = new G4Box("steelBlock", fSteelX/2, fSteelY/2, fSteelZ/2);

  G4Box *concreteBlock = new G4Box("concreteBlock", 
                                    fConcreteX/2, fConcreteY/2, fConcreteZ/2);


  G4Box *innerCavity = new G4Box("innerCavity",
                                  fCavityX/2,
                                  fCavityY/2,
                                  fCavityZ/2);

  // Boolean Solids (the actual concrete shell, steel shell, and aluminum shell)

  G4SubtractionSolid *aluminumShell = new G4SubtractionSolid("aluminumShell", 
                                                            aluminumBlock,
                                                            innerCavity,
                                                            0,
                                                            cavityDisplacement);

  G4SubtractionSolid *steelShell = new G4SubtractionSolid("steelShell", 
                                                            steelBlock,
                                                            aluminumBlock,
                                                            0,
                                                            alumDisplacement);

  G4SubtractionSolid *concreteShell = new G4SubtractionSolid("concreteShell", 
                                                            concreteBlock,
                                                            steelBlock,
                                                            0,
                                                            steelDisplacement);

  
  G4LogicalVolume *alumAbsorberLogical = 
    new G4LogicalVolume(aluminumShell, G4Material::GetMaterial("Aluminum"), 
                        "AluminumAbsorberLogical", 0,0,0);

  G4LogicalVolume *steelAbsorberLogical = 
    new G4LogicalVolume(steelShell, G4Material::GetMaterial("Slab_Stl"),
                        "SteelAbsorberLogical", 0,0,0);

  G4LogicalVolume *concreteAbsorberLogical = 
    new G4LogicalVolume(concreteShell, G4Material::GetMaterial("Concrete"),
                        "ConcreteAbsorberLogical", 0,0,0);

  G4LogicalVolume *hadronAbsorberLogical = 
    new G4LogicalVolume(absorberAirSolid, G4Material::GetMaterial("Air"),
                        "HadronAbsorberLogical", 0,0,0);

  
  // The geometry is more akin to nested measuring cups than nested Russian
  // Dolls, so a 'deep' nesting (air < aluminum < steel < concrete < air) isn't
  // practical, so the three shells get placed flat within an air volume.
  
  G4VPhysicalVolume* alumShellPhysical;
  G4VPhysicalVolume* steelShellPhysical;
  G4VPhysicalVolume* concreteShellPhysical;
  
  concreteShellPhysical = new G4PVPlacement(0, concretePlacement,
                                            concreteAbsorberLogical,
                                            "ConcreteAbsorberPhysical",
                                            hadronAbsorberLogical, 0,0,0);

  steelShellPhysical = new G4PVPlacement(0, steelPlacement,
                                            steelAbsorberLogical,
                                            "SteelAbsorberPhysical",
                                            concreteAbsorberLogical, 0,0,0);

  alumShellPhysical = new G4PVPlacement(0, alumPlacement,
                                            alumAbsorberLogical,
                                            "AluminumAbsorberPhysical",
                                            steelAbsorberLogical, 0,0,0);

  G4VisAttributes *Vis = new G4VisAttributes(G4Colour(0,0.3,0.3));
  hadronAbsorberLogical->SetVisAttributes(Vis);
  fSubDetectorLogical = hadronAbsorberLogical;
}

LBNEHadronAbsorberMessenger::LBNEHadronAbsorberMessenger(LBNESubDetector *subDetector)
  :LBNESubDetectorMessenger(subDetector) 
{
  fHadronAbsorber = (LBNEHadronAbsorber*)subDetector; 
}

LBNEHadronAbsorberMessenger::~LBNEHadronAbsorberMessenger()
{
}

void LBNEHadronAbsorberMessenger::SetNewValue(G4UIcommand* command, G4String val)
{
}
