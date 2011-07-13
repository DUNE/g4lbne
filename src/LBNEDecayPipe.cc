
//C++
#include <stdio.h>
#include <math.h>

//Local and Geant4
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

void LBNEDetectorConstruction::ConstructLBNEDecayPipe()
{

   LBNEDataInput *LBNEData = LBNEDataInput::GetLBNEDataInput();
//
//This is for testing tunnel rotation
//
/*
   G4RotationMatrix tunnel_rot(0,0,0);
   G4double phi = ComputeTunnelRotation(tunnel_rot);
   G4double xshift = (LBNEData->TunnelLength/2.)*sin(phi);
   
   G4cout << "************************" << G4endl;
   G4cout << "TUNNEL rotation = " <<  tunnel_rot(0,0) << " " << tunnel_rot(0,1) << " " << tunnel_rot(0,2) << " " << G4endl
	  << "                  " <<  tunnel_rot(0,1) << " " << tunnel_rot(1,1) << " " << tunnel_rot(1,2) << " " << G4endl
	  << "                  " <<  tunnel_rot(0,2) << " " << tunnel_rot(1,2) << " " << tunnel_rot(2,2) << " " << G4endl;
   G4cout << "************************" << G4endl;
*/

// Tunnel
  G4double l=LBNEData->TunnelLength/2.;
  G4double r=LBNEData->TunnelRadius;

  G4ThreeVector tunnel_position=G4ThreeVector(0,0,l+LBNEData->TunnelZ0);


  G4Tubs* TUNE_solid = new G4Tubs("TUNE_solid",0.,r,l,0,360.*deg);

  G4LogicalVolume* TUNE_log = new G4LogicalVolume(TUNE_solid,GetMaterial(LBNEData->TunnelGEANTmat),"TUNE_log",0,0,0);
  //TUNE_log->SetVisAttributes(G4VisAttributes::Invisible);



  pvTUNE = new G4PVPlacement(0,tunnel_position,"TUNE",TUNE_log,ROCK,false,0);

//
//This is for testing tunnel rotation
//
/*
  //G4ThreeVector tunnel_position_temp = G4ThreeVector(0,0,0);
  //tunnel_position_temp -= tunnel_rot*G4ThreeVector(0,0,-LBNEData->TunnelZ0-LBNEData->TunnelLength/2.);
  G4double dipole_z0 = 10.*m;
  G4double dipole_l = 2.*m;
  G4double dipole_zf = dipole_z0+dipole_l;
  G4double tunnel_dipole_dist = LBNEData->TunnelZ0 - dipole_zf + LBNEData->TunnelLength/2.;
  G4ThreeVector tunnel_position_temp = G4ThreeVector(0,0,dipole_zf);  
  tunnel_position_temp += tunnel_rot*G4ThreeVector(0,0,tunnel_dipole_dist);
  

  pvTUNE = new G4PVPlacement(G4Transform3D(tunnel_rot,tunnel_position_temp),"TUNE",TUNE_log,ROCK,false,0);
*/
  


////////////////////////////
  G4RotationMatrix  dp_obj_rot = pvTUNE -> GetObjectRotationValue();
  G4cout << "Decay Pipe obj rotation = " <<  dp_obj_rot(0,0) << " " << dp_obj_rot(0,1) << " " << dp_obj_rot(0,2) << " " << G4endl
	 << "                          " <<  dp_obj_rot(0,1) << " " << dp_obj_rot(1,1) << " " << dp_obj_rot(1,2) << " " << G4endl
	 << "                          " <<  dp_obj_rot(0,2) << " " << dp_obj_rot(1,2) << " " << dp_obj_rot(2,2) << " " << G4endl;
  
  G4ThreeVector  dp_obj_trans = pvTUNE -> GetObjectTranslation();
  G4cout << "Decay Pipe obj translation = " <<  dp_obj_trans[0] << " " << dp_obj_trans[1] << " " << dp_obj_trans[2] << " " << G4endl;

////////////////////////////

  // Shielding
  l=LBNEData->ShieldLength/2.;
  G4double r_in=LBNEData->ShieldRin;
  G4double r_out=LBNEData->ShieldRout;
  G4ThreeVector sc01_position=G4ThreeVector(LBNEData->ShieldX0,LBNEData->ShieldY0,LBNEData->ShieldZ0+l)-tunnel_position;

  G4Tubs* SC01_solid = new G4Tubs("SC01_solid",r_in,r_out,l,0,360.*deg);
  G4LogicalVolume* SC01_log = new G4LogicalVolume(SC01_solid,GetMaterial(LBNEData->ShieldGEANTmat),"SC01_log",0,0,0);
  G4VPhysicalVolume* SC01 = new G4PVPlacement(0,sc01_position,"DPipeShield",SC01_log,pvTUNE,false,0);
  SC01 -> CheckOverlaps();

  std::cout << "DP Shielding (mother is TUNE) : rin = " << r_in << " r_out = " << r_out << " material = " << GetMaterial(LBNEData->ShieldGEANTmat)->GetName()<< std::endl;

  // Decay Pipe
  l=LBNEData->DecayPipeLength/2.;
  r=LBNEData->DecayPipeRadius;
  G4ThreeVector decay_pipe_position=G4ThreeVector(0,0,LBNEData->DecayPipeZ0+l)-tunnel_position;

  G4Tubs* DPIP_solid = new G4Tubs("DPIP_solid",0.,r,l,0,360.*deg);
  G4LogicalVolume* DPIP_log = new G4LogicalVolume(DPIP_solid,GetMaterial(LBNEData->DecayPipeGEANTmat),"DPIP_log",0,0,0);
  G4VPhysicalVolume* DPIP = new G4PVPlacement(0,decay_pipe_position,"DPipeWall",DPIP_log,pvTUNE,false,0);

  std::cout << "DecayPipe (mother is TUNE) : rin = 0" << " r_out = " << r << " material = " << GetMaterial(LBNEData->DecayPipeGEANTmat)->GetName()<< std::endl;

  // Decay Volume
  l=(LBNEData->DecayPipeLength-LBNEData->DecayPipeFWinThick-LBNEData->DecayPipeEWinThick)/2.;
  r=LBNEData->DecayPipeRadius-LBNEData->DecayPipeWallThick;
  G4ThreeVector decay_volume_position(0,0,(LBNEData->DecayPipeFWinThick-LBNEData->DecayPipeEWinThick)/2.);

  G4Tubs* DVOL_solid = new G4Tubs("DVOL_solid",0.,r,l,0,360.*deg);
  G4LogicalVolume* DVOL_log = new G4LogicalVolume(DVOL_solid,GetMaterial(LBNEData->DecayPipeFillGeantMat),"DVOL_log",0,0,0);
  new G4PVPlacement(0,decay_volume_position,"DPipeVol",DVOL_log,DPIP,false,0);

std::cout << "DecayPipe volume (mother is DecayPipe) : rin = 0" << " r_out = " << r << " material = " << GetMaterial(LBNEData->DecayPipeFillGeantMat)->GetName()<< std::endl;

  // Upstream window
  l=LBNEData->DecayPipeFWinThick/2.;
  r=LBNEData->DecayPipeRadius;
  G4ThreeVector upwn_position=G4ThreeVector(0,0,-LBNEData->DecayPipeLength/2.+l);

  G4Tubs* UPWN_solid = new G4Tubs("UPWN_solid",0.,r,l,0,360.*deg);
  G4LogicalVolume* UPWN_log = new G4LogicalVolume(UPWN_solid,GetMaterial(LBNEData->DecayPipeFWinmat),"UPWN_log",0,0,0);
  new G4PVPlacement(0,upwn_position,"UPWN",UPWN_log,DPIP,false,0);

  // Downstream window
  l=LBNEData->DecayPipeEWinThick/2.;
  r=LBNEData->DecayPipeRadius;
  G4ThreeVector dnwn_position=G4ThreeVector(0,0,LBNEData->DecayPipeLength/2.-l);

  G4Tubs* DNWN_solid = new G4Tubs("DNWN_solid",0.,r,l,0,360.*deg);
  G4LogicalVolume* DNWN_log = new G4LogicalVolume(DNWN_solid,GetMaterial(LBNEData->DecayPipeEWinmat),"DNWN_log",0,0,0);
  new G4PVPlacement(0,dnwn_position,"DNWN",DNWN_log,DPIP,false,0);




  G4cout << "Decay Pipe Constructed" <<G4endl;
}


/*
G4double LBNEDetectorConstruction::ComputeTunnelRotation(G4RotationMatrix &tunnel_rot)
{
//this needs to be changed to be general 
   G4double dipole_z0 = 10.0*m;
   G4double dipole_l  = 2.0*m;
   G4double tunnel_z0 = LBNEData->TunnelZ0;
   G4double tunnel_hl = LBNEData->TunnelLength/2.0;

   //
   // The dipole end is the rotation axis but the rotation
   // must be with respect to the mother volume of the tunnel 
   // which is the world. So must compute the rotation with respect
   // to the world coordinate system.
   //

   //
   // Compute distance of tunnel center from the dipole end 
   //
   G4double dipole_zf = dipole_z0 + dipole_l;
   G4double tunnel_dipole_dist = tunnel_z0 - dipole_zf + tunnel_hl;

   //
   //Compute the bending angle of the dipole.
   //Assume that want pions with (total)energy of 5GeV optimally bent.
   //formula -> E [GeV] = (0.29979/theta) * Integral B*dl [Tm/rad] 
   //
   G4double E_pi = 120.0*GeV; 
   G4double dipole_B = 100.*tesla;   
   //Note: G4 converts the Energy to MeV but we need it in GeV so divide by 1000.
   G4double theta = (0.29979/(E_pi/1000.)) * dipole_B*dipole_l;

   G4cout << "THETA = " << theta << G4endl;

   //
   //theta is the rotation angle about the rotation axis which is the
   //end of the dipole. But need the rotation axis to be the origin of the world.
   //we need to know the corresponding bend angle with respect to the world, 
   //call it phi. 
   //To start...
   //We have an Isosceles triangle with tunnel_dipole_dist as the two equal sides
   //and theta the apex angle. Need to compute the opposite side. Call it s.
   // using the law of cosines and a 1/2 angle trig identity...
   //
   G4double s = 2.0*tunnel_dipole_dist * sin(theta/2.0);
   
   //
   //with respect to the world, we have an isoceles triangle with the 
   //two equal sides = tunnel_z0+tunnel_hl and the opposite side is s.
   //phi is the apex angle. Using the above formula and solving for phi
   //gives...
   //
   G4double phi = 2.0 * asin( s / (2.0*(tunnel_z0+tunnel_hl)) );

   //tunnel_rot.rotateY(-phi);
tunnel_rot.rotateY(-theta);

   fDiploleBendwrtWorld = phi;

   fDiploleBendwrtDipole = theta;

   return phi;
   
}
*/
