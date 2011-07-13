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

void LBNEDetectorConstruction::ConstructLBNEShielding()
{

   LBNEDataInput *LBNEData = LBNEDataInput::GetLBNEDataInput();
   
   fTgtHallShield_closest_yplus  =  999.0*m;
   fTgtHallShield_closest_yminus = -999.0*m;
   fTgtHallShield_closest_xplus  =  999.0*m;
   fTgtHallShield_closest_xminus = -999.0*m;

   G4ThreeVector target_hall_position=G4ThreeVector(0,0,LBNEData->TargetAreaLength/2.+LBNEData->TargetAreaZ0);
   G4ThreeVector tunnel_position=G4ThreeVector(0,0,LBNEData->TunnelLength/2 + LBNEData->TunnelZ0);

  //constructing each of the blocks...
  for (G4int ii=0;ii<LBNEData->BlockNblock;ii++){
    char no[4];
    sprintf(no,"%d",ii+1);
    G4String vol_name="BLK_solid";
    vol_name.append(no);
    BLK_solid[ii] = new G4Box(vol_name,LBNEData->BlockHdx[ii],LBNEData->BlockHdy[ii],LBNEData->BlockLength[ii]/2.);
    vol_name="BLK_log";
    vol_name.append(no);
    G4Material* material=GetMaterial(LBNEData->BlockGeantMaterial[ii]);
    BLK_log[ii] = new G4LogicalVolume(BLK_solid[ii],material,vol_name,0,0,0);
    vol_name="BLK";
    vol_name.append(no);
    G4ThreeVector block_position(LBNEData->BlockX0[ii],LBNEData->BlockY0[ii],LBNEData->BlockZ0[ii]+LBNEData->BlockLength[ii]/2.);
    
    //determine based on position if TGAR or TUNE should be the parent volume
    G4String motherVol;
    if((LBNEData->BlockZ0[ii]>=LBNEData->TargetAreaZ0) &&
       (LBNEData->BlockZ0[ii]<=(LBNEData->TargetAreaZ0 + LBNEData->TargetAreaLength)))
    {
      motherVol = "TGAR";
      block_position -= target_hall_position;

      
      //
      //determine the closest approach of the target hall shielding to the axis
      //so that can place the horn neck and end tracking volumes in such that they
      //don't overlap the target hall shielding
      //
      //the if statement here is to not include blocks that don't extend past the target
      // i.e. the budal monitor which is BLK14
      //
      if(LBNEData->BlockZ0[ii]+LBNEData->BlockLength[ii] > 0.0)
	 LBNEDetermineTargetHallShieldingClosestApproach(ii);


    }
    else 
    {
      motherVol = "TUNE";
      block_position -= tunnel_position;
    }

    G4RotationMatrix rotation(0,0,0);
    rotation.rotateX(atan(LBNEData->BlockDydz[ii]));
    rotation.rotateY(atan(LBNEData->BlockDxdz[ii]));

    new G4PVPlacement(G4Transform3D(rotation,block_position),vol_name,BLK_log[ii],GetPhysicalVolume(motherVol),false,0);
  }

  G4cout << "Target Hall Shielding Blocks Constructed" << G4endl;
}

void LBNEDetectorConstruction::LBNEDetermineTargetHallShieldingClosestApproach(G4int ii)
{
   LBNEDataInput *LBNEData = LBNEDataInput::GetLBNEDataInput();

   G4double x1 = LBNEData->BlockX0[ii] + LBNEData->BlockHdx[ii];
   G4double x2 = LBNEData->BlockX0[ii] - LBNEData->BlockHdx[ii];

   if(x1 == 0 || x2 == 0)
   {
      fTgtHallShield_closest_xplus  = 0.0;
      fTgtHallShield_closest_xminus = 0.0;
   }

   if(x1 < 0.0) 
   { if(x1 > fTgtHallShield_closest_xminus) fTgtHallShield_closest_xminus = x1; }
   if(x2 < 0.0)
   { if(x2 > fTgtHallShield_closest_xminus) fTgtHallShield_closest_xminus = x2; }
   if(x1 > 0.0)
   { if(x1 < fTgtHallShield_closest_xplus)  fTgtHallShield_closest_xplus = x1; }
   if(x2 > 0.0)
   { if(x2 < fTgtHallShield_closest_xplus)  fTgtHallShield_closest_xplus = x2; }

   if(fTgtHallShield_closest_xplus == 0 || fTgtHallShield_closest_xminus == 0)
   {
      std::cout << "LBNEDetectorConstruction::DetermineTargetHallShieldingClosestApproach - " << std::endl;
      std::cout << "      PROBLEM: Closest approach in X of Target Hall Shielding is 0. This shouldn't happen" << std::endl;
   }
   

   G4double y1 = LBNEData->BlockY0[ii] + LBNEData->BlockHdy[ii];
   G4double y2 = LBNEData->BlockY0[ii] - LBNEData->BlockHdy[ii];

   if(y1 == 0 || y2 == 0)
   {
      fTgtHallShield_closest_yplus  = 0.0;
      fTgtHallShield_closest_yminus = 0.0;
   }

   if(y1 < 0.0) 
   { if(y1 > fTgtHallShield_closest_yminus) fTgtHallShield_closest_yminus = y1; }
   if(y2 < 0.0)
   { if(y2 > fTgtHallShield_closest_yminus) fTgtHallShield_closest_yminus = y2; }
   if(y1 > 0.0)
   { if(y1 < fTgtHallShield_closest_yplus)  fTgtHallShield_closest_yplus = y1; }
   if(y2 > 0.0)
   { if(y2 < fTgtHallShield_closest_yplus)  fTgtHallShield_closest_yplus = y2; }

   if(fTgtHallShield_closest_yplus == 0 || fTgtHallShield_closest_yminus == 0)
   {
      std::cout << "LBNEDetectorConstruction::DetermineTargetHallShieldingClosestApproach - " << std::endl;
      std::cout << "      PROBLEM: Closest approach in Y of Target Hall Shielding is 0. This shouldn't happen" << std::endl;
   }

}
