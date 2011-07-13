//----------------------------------------------------------------------
//$Id: LBNEBaffle.cc,v 1.1.1.1 2011/07/13 16:20:52 loiacono Exp $
//----------------------------------------------------------------------


#include "LBNEDetectorConstruction.hh"

#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

#include "LBNEDataInput.hh"

void LBNEDetectorConstruction::ConstructLBNEBaffle()
{ 

   LBNEDataInput *LBNEData = LBNEDataInput::GetLBNEDataInput();
  //G4ThreeVector targetHallPosition=G4ThreeVector(0,0,LBNEData->TargetAreaLength/2.+LBNEData->TargetAreaZ0);
  
  // Baffle 
  G4double HPBRin=LBNEData->HPBaffleRin;
  G4double HPBRout=LBNEData->HPBaffleRout;
  G4double HPBlength=LBNEData->HPBaffleLength;
  G4Tubs* sBaffle=new G4Tubs("sBaffle",HPBRin,HPBRout,HPBlength/2.,0.,360.*deg);
  G4LogicalVolume* lvBaffle=new G4LogicalVolume(sBaffle,GetMaterial(LBNEData->HPBaffleGEANTMat),"lvBaffle",0,0,0);

  //G4ThreeVector bafflePos=G4ThreeVector(LBNEData->HPBaffleX0,LBNEData->HPBaffleY0,LBNEData->HPBaffleZ0+HPBlength/2.)-targetHallPosition;

  G4RotationMatrix rotation;
  rotation.rotateX(atan(LBNEData->HPBaffleDYDZ));
  rotation.rotateY(atan(LBNEData->HPBaffleDXDZ));

  G4ThreeVector bafflePos(LBNEData->HPBaffleX0,LBNEData->HPBaffleY0,LBNEData->HPBaffleZ0);
  bafflePos -= rotation*G4ThreeVector(0., 0., -HPBlength/2.);
  bafflePos -= G4ThreeVector(0,0,LBNEData->TargetAreaLength/2.+LBNEData->TargetAreaZ0);

  new G4PVPlacement(G4Transform3D(rotation,bafflePos),"Baffle",lvBaffle,TGAR,false,0);
}

/*
    G4ThreeVector targetPosition(LBNEData->TargetX0[i],LBNEData->TargetY0[i],LBNEData->TargetZ0[i]);
    targetPosition -= rotation*G4ThreeVector(0.,0.,-TGT_l);
    targetPosition -= G4ThreeVector(0.,0.,LBNEData->TargetAreaLength/2.+LBNEData->TargetAreaZ0);
*/
