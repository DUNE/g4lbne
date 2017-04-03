//This file defines the function
//LBNEDetectorConstruction::ConstructTarget.  It is a modified version
//of the LBNETarget.cc file from an old version of G4numi.  This
//function constructs the target and places it in TGAR (target area).

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

#include <sstream>


#include "LBNEDataInput.hh"

void LBNEDetectorConstruction::ConstructLBNETarget()
{
   LBNEDataInput *LBNEData = LBNEDataInput::GetLBNEDataInput();

  if(LBNEData->TargetNtarget <= 0) return;



  bool isBox = false;
  if(LBNEData->TargetShape == "BOX") isBox = true;
  else if(LBNEData->TargetShape == "TUBE") isBox = false;
  else G4Exception("TargetShape must be either 'BOX' or 'TUBE'","",FatalException,"");

  for(G4int i = 0; i < LBNEData->TargetNtarget; i++) 
  {  
     G4double TGT_l = LBNEData->TargetLength[i]/2.0;
     
    G4VSolid* TGT_solid;
    std::ostringstream solidName;
    solidName << "TGT" << (i+1) << "_solid";
    
    if(isBox) 
    {
       if( LBNEData->TargetNtarget > 1) G4Exception("layered box-shaped target not yet supported","",FatalException,"");
       //if(i > 0) G4Exception("layered box-shaped target not yet supported");
       TGT_solid = new G4Box(solidName.str(), LBNEData->TargetWidth[i]/2.0,
			     LBNEData->TargetHeight[i]/2.0, TGT_l);
    }
    else 
    {
      // 
      //is a tube. layers of the target are constructed as semi-hollow cylinders...
      //
      G4double lastR = 0.0;
      if(i > 0) lastR = LBNEData->TargetRadius[i-1];
      TGT_solid = new G4Tubs(solidName.str(), lastR, LBNEData->TargetRadius[i],
                             TGT_l, 0.0, 360.0*deg);
    }
    
    std::ostringstream lvName;
    lvName << "LVTarget" << (i+1);
    G4LogicalVolume* LVTarget=new G4LogicalVolume(TGT_solid,GetMaterial(LBNEData->TargetGEANTmat[i]),
						  lvName.str());
    
    //
    //the origin of rotation is at the begining of the volume (x0,y0,z0) and not at its center
    //
    G4RotationMatrix rotation(0,0,0);
    rotation.rotateX(atan(LBNEData->TargetDydz[i]));
    rotation.rotateY(atan(LBNEData->TargetDxdz[i]));

    G4ThreeVector targetPosition(LBNEData->TargetX0[i],LBNEData->TargetY0[i],LBNEData->TargetZ0[i]);
    targetPosition -= rotation*G4ThreeVector(0.,0.,-TGT_l);
    targetPosition -= G4ThreeVector(0.,0.,LBNEData->TargetAreaLength/2.+LBNEData->TargetAreaZ0);

    std::ostringstream pvpName;
    pvpName << "TGT" << (i+1);
    //G4PVPlacement *tgtplace = new G4PVPlacement(G4Transform3D(rotation,targetPosition),
    //					       pvpName.str(),LVTarget,TGAR,false,0);
    new G4PVPlacement(G4Transform3D(rotation,targetPosition),
		      pvpName.str(),LVTarget,TGAR,false,0);
/*
///////////////////////////
    G4RotationMatrix  tgt_obj_rot = tgtplace -> GetObjectRotationValue();
     G4cout << "tgt obj rotation = " <<  tgt_obj_rot(0,0) << " " << tgt_obj_rot(0,1) << " " << tgt_obj_rot(0,2) << " " << G4endl
	    << "                   " <<  tgt_obj_rot(0,1) << " " << tgt_obj_rot(1,1) << " " << tgt_obj_rot(1,2) << " " << G4endl
	    << "                   " <<  tgt_obj_rot(0,2) << " " << tgt_obj_rot(1,2) << " " << tgt_obj_rot(2,2) << " " << G4endl;

     G4ThreeVector  tgt_obj_trans = tgtplace -> GetObjectTranslation();
     G4cout << "tgt obj translation = " <<  tgt_obj_trans[0] << " " << tgt_obj_trans[1] << " " << tgt_obj_trans[2] << " " << G4endl;

////////////////////////////
  G4RotationMatrix  tgar_obj_rot = TGAR -> GetObjectRotationValue();
  G4ThreeVector  tgar_obj_trans = TGAR -> GetObjectTranslation();

  G4ThreeVector tgt_global_pos = tgar_obj_trans + tgt_obj_trans;
  G4cout << "target global center position = " <<  tgt_global_pos[0] << " " << tgt_global_pos[1] << " " << tgt_global_pos[2] << " " << G4endl;
  G4cout << "target global Z0, Zf = " << tgt_global_pos[2]-TGT_l << " " << tgt_global_pos[2]+TGT_l << G4endl;
  G4cout << G4endl;
*/

////////////////////////////
///////////////////////////////////
  
  }//end loop over target parts

  
  
  //
  //Place 2 exit volumes around target. Right now just assumes that the outer most 
  //TGT volume is the largest
  //
  G4int lastindex            = LBNEData->TargetNtarget-1;
  G4double tgtExit_thickness = 0.000002*m;
  G4double tgtInnerExit_outlength = LBNEData->TargetLength[lastindex] + tgtExit_thickness*2.0;
  G4double tgtInnerExit_inlength  = LBNEData->TargetLength[lastindex];
  G4double tgtOuterExit_outlength = tgtInnerExit_outlength + tgtExit_thickness*2.0;
  G4double tgtOuterExit_inlength  = tgtInnerExit_outlength;
  
  G4SubtractionSolid*  TgtInnerExit_solid    = 0;
  G4VSolid*            TgtInnerExit_outsolid = 0;
  G4VSolid*            TgtInnerExit_insolid  = 0;
  G4SubtractionSolid*  TgtOuterExit_solid    = 0;
  G4VSolid*            TgtOuterExit_outsolid = 0;
  G4VSolid*            TgtOuterExit_insolid  = 0;
  
  G4RotationMatrix  rotation    = G4RotationMatrix(0,0,0);
  G4ThreeVector     translation = G4ThreeVector(0,0,0);
  
  
  if(isBox) 
  {
     //if( LBNEData->TargetNtarget > 1) G4Exception("layered box-shaped target not yet supported");
     //std::cout << "Can't create target exit volumes for a box target" << std::endl;
     
     G4double tgtInnerExit_outhalfwidth  = (LBNEData->TargetWidth[lastindex])/2.  + tgtExit_thickness;
     G4double tgtInnerExit_outhalfheight = (LBNEData->TargetHeight[lastindex])/2. + tgtExit_thickness;
     G4double tgtInnerExit_inhalfwidth   = (LBNEData->TargetWidth[lastindex])/2.;
     G4double tgtInnerExit_inhalfheight  = (LBNEData->TargetHeight[lastindex])/2.;
     
     G4double tgtOuterExit_outhalfwidth  = tgtInnerExit_outhalfwidth + tgtExit_thickness;
     G4double tgtOuterExit_outhalfheight = tgtInnerExit_outhalfheight + tgtExit_thickness;
     G4double tgtOuterExit_inhalfwidth   = tgtInnerExit_outhalfwidth;
     G4double tgtOuterExit_inhalfheight  = tgtInnerExit_outhalfheight;
     
     TgtInnerExit_outsolid = new G4Box("TgtInnerExit_outsolid", tgtInnerExit_outhalfwidth, 
				       tgtInnerExit_outhalfheight, tgtInnerExit_outlength/2.0);
     TgtInnerExit_insolid  = new G4Box("TgtInnerExit_insolid", tgtInnerExit_inhalfwidth, 
				       tgtInnerExit_inhalfheight, tgtInnerExit_inlength/2.0);
     
     TgtOuterExit_outsolid = new G4Box("TgtOuterExit_outsolid", tgtOuterExit_outhalfwidth, 
				       tgtOuterExit_outhalfheight, tgtOuterExit_outlength/2.0);
     TgtOuterExit_insolid  = new G4Box("TgtOuterExit_insolid", tgtOuterExit_inhalfwidth, 
				       tgtOuterExit_inhalfheight, tgtOuterExit_inlength/2.0);
     
  }
  else // is tube
  {
     G4double tgtInnerExit_outradius = LBNEData->TargetRadius[lastindex] + tgtExit_thickness;
     G4double tgtInnerExit_inradius  = LBNEData->TargetRadius[lastindex];
     G4double tgtOuterExit_outradius = tgtInnerExit_outradius + tgtExit_thickness;
     G4double tgtOuterExit_inradius  = tgtInnerExit_outradius;
     
     TgtInnerExit_outsolid = new G4Tubs("TgtInnerExit_outsolid", 0.0,  tgtInnerExit_outradius, 
					tgtInnerExit_outlength/2.0, 0.*deg, 360.*deg);
     TgtInnerExit_insolid  = new G4Tubs("TgtInnerExit_intsolid", 0.0,  tgtInnerExit_inradius, 
					tgtInnerExit_inlength/2.0, 0.*deg, 360.*deg);
     
     TgtOuterExit_outsolid = new G4Tubs("TgtOuterExit_outsolid", 0.0,  tgtOuterExit_outradius, 
					tgtOuterExit_outlength/2.0, 0.*deg, 360.*deg);
     TgtOuterExit_insolid  = new G4Tubs("TgtOuterExit_intsolid", 0.0,  tgtOuterExit_inradius, 
					tgtOuterExit_inlength/2.0, 0.*deg, 360.*deg);
  }
  
  
  TgtInnerExit_solid = new G4SubtractionSolid("TgtInnerExit_solid", TgtInnerExit_outsolid, 
					      TgtInnerExit_insolid, G4Transform3D(rotation,translation));
  
  TgtOuterExit_solid = new G4SubtractionSolid("TgtOuterExit_solid", TgtOuterExit_outsolid, 
					      TgtOuterExit_insolid, G4Transform3D(rotation,translation));
  
  G4LogicalVolume*   TgtInnerExit_lv = new G4LogicalVolume(TgtInnerExit_solid, 
							   Air, "TgtInnerExit_lv", 0, 0, 0);
  
  G4LogicalVolume*   TgtOuterExit_lv = new G4LogicalVolume(TgtOuterExit_solid, 
							   Air, "TgtOuterExit_lv", 0, 0, 0);
     
  //
  //the origin of rotation is at the begining of the volume (x0,y0,z0) and not at its center
  //
  G4RotationMatrix targetrotation(0,0,0);
  targetrotation.rotateX(atan(LBNEData->TargetDydz[lastindex]));
  targetrotation.rotateY(atan(LBNEData->TargetDxdz[lastindex]));
  
  G4ThreeVector tgtInnerExitPosition(LBNEData->TargetX0[lastindex], 
				     LBNEData->TargetY0[lastindex], 
				     LBNEData->TargetZ0[lastindex]-tgtExit_thickness);
  
  G4ThreeVector tgtOuterExitPosition(LBNEData->TargetX0[lastindex], 
				     LBNEData->TargetY0[lastindex], 
				     LBNEData->TargetZ0[lastindex]- 2.0*tgtExit_thickness);
  
  tgtInnerExitPosition -= targetrotation*G4ThreeVector(0.0, 0.0, -(tgtInnerExit_outlength/2.0));
  tgtInnerExitPosition -= G4ThreeVector(0.0, 0.0, LBNEData->TargetAreaLength/2.0 + LBNEData->TargetAreaZ0);
  
  tgtOuterExitPosition -= targetrotation*G4ThreeVector(0.0, 0.0, -(tgtOuterExit_outlength/2.0));
  tgtOuterExitPosition -= G4ThreeVector(0.0, 0.0, LBNEData->TargetAreaLength/2.0 + LBNEData->TargetAreaZ0);
  
  //G4PVPlacement* TgtInnerExit_pv = 
  new G4PVPlacement(G4Transform3D(targetrotation,tgtInnerExitPosition), 
		    "TargetInnerExit", TgtInnerExit_lv, TGAR, false,0);
  //G4PVPlacement* TgtOuterExit_pv = 
  new G4PVPlacement(G4Transform3D(targetrotation,tgtOuterExitPosition), 
		    "TargetOuterExit", TgtOuterExit_lv, TGAR, false,0);
  
  //
  //done with target exit volumes
  // 
  
  
  
  if(LBNEData->GetSimulation() == "Target Tracking")
  {
     // begintarget end Plane for Target Tracking simulation
     
     //
     //Place plane volume at the end of the target. For Tracking.
     //This is only to be used when ONLY the TargetHall and Target is constructed and nothing else
     //otherwise volumes will overlap
     //Right now just assumes that all TGT volumes 
     //are the same length which is probably a good assumption just not general. Also assumes that 
     //the target is not rotated.
     G4double tgtEndPlane_length = 0.000001*m;
     
     //Make the width and height the same as the Target Hall
     G4double tgtEndPlane_halfwidth   = LBNEData->TargetAreaWidth/2.;
     G4double tgtEndPlane_halfheight  = LBNEData->TargetAreaHeight/2.;
     
     G4Box* tgtEndPlane_solid = new G4Box("TgtEndPlane_solid", tgtEndPlane_halfwidth, 
					  tgtEndPlane_halfheight, tgtEndPlane_length/2.);
     
     
     G4LogicalVolume* tgtEndPlane_lv = new G4LogicalVolume(tgtEndPlane_solid, He, "TgtEndPlane_lv", 0, 0, 0);
     //
     //the origin of rotation is at the begining of the volume (x0,y0,z0) and not at its center
     //
     G4RotationMatrix tgtEndPlane_rotation(0,0,0);
     tgtEndPlane_rotation.rotateX(atan(LBNEData->TargetDydz[lastindex]));
     tgtEndPlane_rotation.rotateY(atan(LBNEData->TargetDxdz[lastindex]));
     
     G4ThreeVector tgtEndPlane_position(LBNEData->TargetX0[lastindex], 
					LBNEData->TargetY0[lastindex], 
					LBNEData->TargetZ0[lastindex]+LBNEData->TargetLength[lastindex]+2.0*tgtExit_thickness);
     
     tgtEndPlane_position -= tgtEndPlane_rotation*G4ThreeVector(0.0, 0.0, -(tgtEndPlane_length/2.0));
     //position wrt the TGAR
     tgtEndPlane_position -= G4ThreeVector(0.0, 0.0, LBNEData->TargetAreaLength/2.0 + LBNEData->TargetAreaZ0);
	
     //position wrt the World
     //tgtEndPlane_position +=  G4ThreeVector(0.0, 0.0, LBNEData->TargetAreaLength/2.0 + LBNEData->TargetAreaZ0);
     
     //G4PVPlacement* tgtEndPlane_pl = 
     new G4PVPlacement(G4Transform3D(tgtEndPlane_rotation,tgtEndPlane_position), 
		       "TgtEndPlane", tgtEndPlane_lv, TGAR, false,0);
     
     
  }// end target end Plane for Target Tracking simulation



//
//This is for testing Dipole simulation
//
/*
////////////////////////////////////////
    //////////////////////////////////
  {
     //
     //construct TestLayer at Origin
     //
     
     G4double testlayer_halfwidth   = 0.001*m/2.;
     G4double testlayer_halfheight  = 0.001*m/2.;
     G4double testlayer_halflength  = LBNEData->TunnelZ0+LBNEData->TunnelLength/2.;
     G4VSolid* TestLayerOrigin_solid = new G4Box("TestLayerOrigin_solid", 
						 testlayer_halfwidth, 
						 testlayer_halfheight, 
						 testlayer_halflength);
     
     G4LogicalVolume* TestLayerOrigin_lv =new G4LogicalVolume(TestLayerOrigin_solid, He,
							      "TestLayerOrigin_lv", 0, 0, 0);

     G4RotationMatrix testlayer_rot(0,0,0);
     testlayer_rot.rotateY(-fDiploleBendwrtWorld);

     G4double target_hl =  LBNEData->TargetLength[0] / 2.0;

     //
     //testlayer_z is the location of the test layer in the target hall
     //
     G4double testlayer_z = LBNEData->TargetZ0[0] + target_hl;
	testlayer_z += -(LBNEData->TargetAreaZ0 + LBNEData->TargetAreaLength/2.)  - target_hl;
     G4ThreeVector testlayer_pos(0,0,testlayer_z);
     
     new G4PVPlacement(G4Transform3D(testlayer_rot,testlayer_pos),
		       "TestLayerOrigin",TestLayerOrigin_lv,TGAR,false,0);
  }
  {

     //
     //construct TestLayer at Dipole end
     //
     G4double dipole_z0 = 10.0*m;
     G4double dipole_l  = 2.0*m;
     G4double dipole_hl = dipole_l/2.;
     G4double dipole_zf = dipole_z0 + dipole_l;
     
     G4double testlayer_halfwidth   = 0.001*m/2.;
     G4double testlayer_halfheight  = 0.001*m/2.;
     //G4double testlayer_halflength  = LBNEData->TunnelZ0+LBNEData->TunnelLength/2. - dipole_zf;
G4double testlayer_halflength  = LBNEData->TunnelZ0+LBNEData->TunnelLength - dipole_zf;
     G4VSolid* TestLayerDipoleEnd_solid = new G4Box("TestLayerDipoleEnd_solid", 
						 testlayer_halfwidth, 
						 testlayer_halfheight, 
						 testlayer_halflength);
     
     G4LogicalVolume* TestLayerDipoleEnd_lv =new G4LogicalVolume(TestLayerDipoleEnd_solid, He,
								 "TestLayerDipoleEnd_lv", 0, 0, 0);


     //
     // Compute the location of the dipole end in the target hall
     // this is the testlayer_z
     //
     G4RotationMatrix testlayer_rot(0,0,0);
     testlayer_rot.rotateY(-fDiploleBendwrtDipole);

     G4double testlayer_z  = dipole_z0 + dipole_hl;
              testlayer_z += -(LBNEData->TargetAreaZ0 + LBNEData->TargetAreaLength/2.)  + dipole_hl;
     G4ThreeVector testlayer_pos(0,0,testlayer_z);
     
     new G4PVPlacement(G4Transform3D(testlayer_rot,testlayer_pos),
		       "TestLayerDipoleEnd",TestLayerDipoleEnd_lv,TGAR,false,0);
  }
     //////////////////////////////////
////////////////////////////////////////

*/

 

    G4cout << "Target Constructed" << G4endl;
}

 
