//This file defines the function
//LBNEDetectorConstruction::ConstructTargetHall.  It is a modified
//version of the NumiTargetHall.cc file from an old version of G4numi.
//This function constructs the TGAR (target area) solid, which is used
//to contain everything in the target area.  It also constructs the
//blocks, which are (typically) used to make up the target hall
//shielding, the budal monitor, and the hadron absorber.  The hadron
//absorber is actually outside of the target hall and inside the decay
//pipe, so ConstructDecayPipe currently needs to be called before
//ConstructTargetHall.  Depending on its position, this function will
//decide if the parent of a block should be TGAR or TUNE (the decay
//tunnel).

#include "LBNEDetectorConstruction.hh"

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

void LBNEDetectorConstruction::ConstructLBNETargetHall()
{
   LBNEDataInput *LBNEData = LBNEDataInput::GetLBNEDataInput();

  //constructin TGAR (target area)...
  G4double TGAR_width=LBNEData->TargetAreaWidth/2.;
  G4double TGAR_height=LBNEData->TargetAreaHeight/2.;
  G4double TGAR_length=LBNEData->TargetAreaLength/2.;
  G4ThreeVector target_hall_position=G4ThreeVector(0,0,LBNEData->TargetAreaLength/2.+LBNEData->TargetAreaZ0);
  G4ThreeVector tunnel_position=G4ThreeVector(0,0,LBNEData->TunnelLength/2 + LBNEData->TunnelZ0);

  G4Box* TGAR_solid=new G4Box("TGAR_solid",TGAR_width,TGAR_height,TGAR_length);
  TGAR_log=new G4LogicalVolume(TGAR_solid,GetMaterial(LBNEData->TargetAreaGEANTmat),"TGAR_log",0,0,0);
  TGAR = new G4PVPlacement(0,target_hall_position,"TGAR",TGAR_log,ROCK,false,0);

////////////////////////////
  G4RotationMatrix  tgar_obj_rot = TGAR -> GetObjectRotationValue();
  G4cout << "TGAR obj rotation = " <<  tgar_obj_rot(0,0) << " " << tgar_obj_rot(0,1) << " " << tgar_obj_rot(0,2) << " " << G4endl
	 << "                    " <<  tgar_obj_rot(0,1) << " " << tgar_obj_rot(1,1) << " " << tgar_obj_rot(1,2) << " " << G4endl
	 << "                    " <<  tgar_obj_rot(0,2) << " " << tgar_obj_rot(1,2) << " " << tgar_obj_rot(2,2) << " " << G4endl;
  
  G4ThreeVector  tgar_obj_trans = TGAR -> GetObjectTranslation();
  G4cout << "TGAR obj translation = " <<  tgar_obj_trans[0] << " " << tgar_obj_trans[1] << " " << tgar_obj_trans[2] << " " << G4endl;

////////////////////////////

  G4cout << "Target Hall Constructed" << G4endl;
}

