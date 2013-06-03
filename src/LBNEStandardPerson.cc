//---------------------------------------------------------------------------// 
// $Id: LBNEStandardPerson.cc,v 1.1.2.3 2013/06/03 18:55:37 robj137 Exp $
//---------------------------------------------------------------------------// 

#include <stdio.h>
#include <math.h>

//Local and Geant4
//#include "LBNEDetectorConstruction.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
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
#include "LBNEStandardPerson.hh"

LBNEStandardPerson::LBNEStandardPerson(G4String detName):LBNESubVolume(detName)
{
  fMessenger = new LBNEStandardPersonMessenger(this);
}

LBNEStandardPerson::~LBNEStandardPerson()
{
  delete fMessenger;
}


void LBNEStandardPerson::ConstructSubvolume()
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

  fHeadRadius = 10*cm;
  fHeight = 170*cm;
  fLegLength = 80*cm;
  fLegRadius = 15*cm;
  fTorsoLength = fHeight - fLegLength;
  fTorsoRadius = 18*cm;
  
  // Base Solids
  G4Sphere* headSolid = new G4Sphere("headSolid", 0, fHeadRadius, 
                                      0, 360*deg, 
                                      0, 180*deg);

  G4Tubs* torsoSolid = new G4Tubs("torsoSolid", 0, fTorsoRadius,
                                  fTorsoLength/2, 0, 360*deg);

  G4Tubs* legSolid = new G4Tubs("legSolid", 0, fLegRadius,
                                  fLegLength/2, 0, 360*deg);

  G4UnionSolid *humanSolid = new G4UnionSolid("humanSolid", legSolid,
                                              torsoSolid, 0,
                                              G4ThreeVector(0,0,fLegLength));
  humanSolid = new G4UnionSolid("humanSolid", humanSolid,
                                              headSolid, 0,
                                              G4ThreeVector(0,0,fHeadRadius +
                                              0.5*fTorsoLength+fLegLength));

  G4LogicalVolume *humanLogical =
    new G4LogicalVolume(humanSolid, G4Material::GetMaterial("Air"), 
                        "HumanLogical", 0,0,0);

  
  G4VisAttributes *Vis = new G4VisAttributes(G4Colour(0,0.3,0.3));
  humanLogical->SetVisAttributes(Vis);
  fSubVolumeLogical = humanLogical;
}

LBNEStandardPersonMessenger::LBNEStandardPersonMessenger(LBNESubVolume *subVolume)
  :LBNESubVolumeMessenger(subVolume) 
{
  fStandardPerson = (LBNEStandardPerson*)subVolume; 
}

LBNEStandardPersonMessenger::~LBNEStandardPersonMessenger()
{
}

void LBNEStandardPersonMessenger::SetNewValue(G4UIcommand* command, G4String val)
{
}
