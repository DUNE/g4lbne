//----------------------------------------------------------------------
// $Id: LBNENumiTarget.cc,v 1.1.2.1 2013/08/09 22:43:58 lebrun Exp $
//----------------------------------------------------------------------

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
#include "LBNENumiDataInput.hh"

void LBNEDetectorConstruction::ConstructNUMITarget()
{

//
// All obsolete stuff. Keep here temporarely for documentation.. 

   LBNENumiDataInput *LBNENumiData = LBNENumiDataInput::GetLBNENumiDataInput();
   

   G4RotationMatrix rotation;
   G4ThreeVector translation; 
   G4String Sname,PVname,LVname,vol_name;
   G4double tgtGap=0.025*mm;
   char no[3];
   
   // Mother volume (TGAR) position
   G4ThreeVector target_hall_position=G4ThreeVector(0,0,LBNENumiData->TargetAreaLength/2.+LBNENumiData->TargetAreaZ0); 
   G4ThreeVector targetPosition=G4ThreeVector(LBNENumiData->TargetX0,LBNENumiData->TargetY0,LBNENumiData->TargetZ0);
   
   //Create Target Region
   //G4Region* TargetRegion = new G4Region("LBNE Target");
   
   //Create Mother Volume
   G4VSolid* sTargetMotherVol;
   G4VSolid* dummySolid;
   G4double mvGap=0.05*mm;
   sTargetMotherVol=new G4Tubs("sTargetMotherVol",0.,LBNENumiData->CTubeRout[0]+mvGap,LBNENumiData->CTubeLength[0]/2.+mvGap,0.,360.*deg);
   G4ThreeVector TargetMVOrigin=G4ThreeVector(0.,0.,LBNENumiData->CTubeZ0[0]+LBNENumiData->CTubeLength[0]/2.);
   for (G4int ii=1;ii<LBNENumiData->NContainerN;ii++){
      dummySolid=new G4Tubs("dummySolid",0.,LBNENumiData->CTubeRout[ii]+mvGap,LBNENumiData->CTubeLength[ii]/2.+mvGap,0.,360.*deg);
      translation=G4ThreeVector(0.,0.,LBNENumiData->CTubeZ0[ii]+LBNENumiData->CTubeLength[ii]/2.)-TargetMVOrigin;
      rotation=G4RotationMatrix(0.,0.,0.);
      sTargetMotherVol=new G4UnionSolid("sTargetMotherVol",sTargetMotherVol,dummySolid,G4Transform3D(rotation,translation));
   }    
   G4ThreeVector TargetMotherVolPosition=TargetMVOrigin-target_hall_position+targetPosition;
   //G4LogicalVolume* lvTargetMotherVol=new G4LogicalVolume(sTargetMotherVol,He,"lvTargetMother",0,0,0);
//   G4LogicalVolume* lvTargetMotherVol=new G4LogicalVolume(sTargetMotherVol,NumiTargetHelium,"lvTargetMother",0,0,0);
//   
//   G4VPhysicalVolume* pvTargetMotherVol=new G4PVPlacement(0,TargetMotherVolPosition,"pvTargetMother",lvTargetMotherVol,TGAR,false,0);
//   pvTargetMotherVol -> CheckOverlaps();
   
//   if(LBNENumiData->GetPrintGeometry())
//   {
      G4cout << "Z Position of target mother volume = " << TargetMotherVolPosition.z()/m << " m " << G4endl;
//   }
   
/*     
  //------------------------------------------------------------------------------------------------
  //Target
  //------------------------------------------------------------------------------------------------
  // Carbon Target
  
  //First create one segment
  G4double TGT_w=LBNENumiData->TargetSWidth/2.;
  G4double TGT_h=LBNENumiData->TargetSHeight/2.;
  G4double TGT_l=LBNENumiData->TargetSLength/2.; //these are 0.5*true dimension
  
  G4VSolid* TGT1_solid;
  G4Tubs* CPG_solid;
  if (LBNENumiData->TargetEndRounded)
  {
    TGT_l=TGT_l-LBNENumiData->TargetSWidth/2.;
    G4Box* TGT2_solid=new G4Box("TGT2_solid",TGT_w,TGT_h,TGT_l);
    G4Tubs* TGTRE_solid=new G4Tubs("TGTRE_solid",0.,TGT_w,TGT_h,0.,360.*deg); 
    rotation=G4RotationMatrix(0,0,0); rotation.rotateX(90.*deg);
    translation=G4ThreeVector(0,0,-TGT_l);
    TGT1_solid=new G4UnionSolid("TGT1_solid",TGT2_solid,TGTRE_solid,G4Transform3D(rotation,translation));
    translation=G4ThreeVector(0,0,TGT_l);
    TGT1_solid=new G4UnionSolid("TGT1_solid",TGT1_solid,TGTRE_solid,G4Transform3D(rotation,translation));
  }
  else
  { 
    TGT1_solid=new G4Box("TGT1_solid",TGT_w,TGT_h,TGT_l);
  }
  
  CPG_solid=new G4Tubs("CPG_solid",0.,LBNENumiData->TargetCPGRadius,2.*TGT_l,0.,360.*deg);
  rotation=G4RotationMatrix(0,0,0);
  translation=G4ThreeVector(0,LBNENumiData->TargetCPGPosition,0);
  G4SubtractionSolid* TGT_solid=new G4SubtractionSolid("TGT_solid",TGT1_solid,CPG_solid,G4Transform3D(rotation,translation));
  translation=G4ThreeVector(0,-LBNENumiData->TargetCPGPosition,0);
  TGT_solid=new G4SubtractionSolid("TGT_solid",TGT_solid,CPG_solid,G4Transform3D(rotation,translation));
  G4LogicalVolume* LVTargetFin=new G4LogicalVolume(TGT_solid,Target,"LVTargetFin",0,0,0);
  //Now create TargetSegmentNo of target fins and place them
  for (G4int ii=0;ii<LBNENumiData->TargetSegmentNo;ii++)
  {
     rotation=G4RotationMatrix(0,0,0);
     rotation.rotateX(atan(LBNENumiData->TargetDydz));
     rotation.rotateY(atan(LBNENumiData->TargetDxdz));
     // with this translation rotation axis is at the begining of the volume (x0,y0,z0) and not at its center
     translation=G4ThreeVector(-(sin(atan(LBNENumiData->TargetDxdz)))*cos(atan(LBNENumiData->TargetDydz))*LBNENumiData->TargetSLength/2.,(sin(atan(LBNENumiData->TargetDydz)))*LBNENumiData->TargetSLength/2.,(1-cos(atan(LBNENumiData->TargetDxdz))*cos(atan(LBNENumiData->TargetDydz)))*LBNENumiData->TargetSLength/2.);
     G4ThreeVector targetSegmentPosition=G4ThreeVector(0.,0.,LBNENumiData->TargetSLength/2.+(LBNENumiData->TargetSegmentPitch+LBNENumiData->TargetSLength)*ii)-TargetMVOrigin-translation;
     new G4PVPlacement(G4Transform3D(rotation,targetSegmentPosition),"TGT1",LVTargetFin,pvTargetMotherVol,false,0);
     
//     if(LBNENumiData->GetPrintGeometry())
//     {
	if(ii==0) G4cout << "Z0   Position of first target fin = " << (TargetMotherVolPosition.z() + (targetSegmentPosition.z()-LBNENumiData->TargetSLength/2.))/m << " m " << G4endl;
	if(ii==46)G4cout << "ZEnd Position of last target fin  = " << (TargetMotherVolPosition.z() + (targetSegmentPosition.z()+LBNENumiData->TargetSLength/2.) )/m << " m " << G4endl;
//     }
     
     
  }


  
  // Budal Monitor
  rotation=G4RotationMatrix(0,0,0);
  rotation.rotateX(atan(LBNENumiData->BudalDydz));
  rotation.rotateY(atan(LBNENumiData->BudalDxdz));
  rotation.rotateZ(90.*deg);
  // with this translation rotation axis is at the begining of the volume (x0,y0,z0) and not at its center
  translation=G4ThreeVector(-(sin(atan(LBNENumiData->BudalDxdz)))*cos(atan(LBNENumiData->BudalDydz))*LBNENumiData->TargetSLength/2.,(sin(atan(LBNENumiData->BudalDydz)))*LBNENumiData->TargetSLength/2.,(1-cos(atan(LBNENumiData->BudalDxdz))*cos(atan(LBNENumiData->BudalDydz)))*LBNENumiData->TargetSLength/2.);
  G4ThreeVector budalMonitorPosition=G4ThreeVector(LBNENumiData->BudalX0,LBNENumiData->BudalY0,LBNENumiData->BudalZ0+LBNENumiData->TargetSLength/2.)-TargetMVOrigin;
  G4LogicalVolume* LVBudalMonitor=new G4LogicalVolume(TGT1_solid,Target,"LVBudalMonitor",0,0,0);
  new G4PVPlacement(G4Transform3D(rotation,budalMonitorPosition),"BudalMonitor",LVBudalMonitor,pvTargetMotherVol,false,0);
  
  //Container
  G4LogicalVolume* lvContTube;
  G4VSolid* sContSolid;
  for (G4int ii=0;ii<LBNENumiData->NContainerN;ii++){
    vol_name=LBNENumiData->CTubeVolName[ii];
    sContSolid=new G4Tubs("ContSolid",LBNENumiData->CTubeRin[ii],LBNENumiData->CTubeRout[ii],LBNENumiData->CTubeLength[ii]/2.,0.,360.*deg);
    G4ThreeVector ContPosition=G4ThreeVector(0.,0.,LBNENumiData->CTubeZ0[ii]+LBNENumiData->CTubeLength[ii]/2.)-TargetMVOrigin;//target_hall_position+targetPosition;    
    lvContTube=new G4LogicalVolume(sContSolid,GetMaterial(LBNENumiData->CTubeGeantMat[ii]),vol_name.append("LV"),0,0,0);
    CNT[ii]=new G4PVPlacement(0,ContPosition,vol_name,lvContTube,pvTargetMotherVol,false,ii);
  }

  //Rings that hold target and cooling pipes
  G4LogicalVolume* lvTgtRing;
  G4Box* TGT_box_solid;
  G4VSolid* STgtRing;
  for (G4int ii=0;ii<LBNENumiData->NTgtRingN;ii++){
    vol_name=LBNENumiData->TgtRingVolName[ii];
    STgtRing = new G4Tubs("STgtRing",LBNENumiData->TgtRingRin[ii],LBNENumiData->TgtRingRout[ii],LBNENumiData->TgtRingLength[ii]/2.,0.,360.*deg);
    G4ThreeVector TgtRingPosition=G4ThreeVector(0.,0.,LBNENumiData->TgtRingZ0[ii]+LBNENumiData->TgtRingLength[ii]/2.)-TargetMVOrigin;
    //make holes for pipes
    CPG_solid=new G4Tubs("CPG_solid",0.,LBNENumiData->CPipeRadiusOut[0],2*LBNENumiData->TgtRingLength[ii],0.,360.*deg);
    rotation=G4RotationMatrix(0,0,0);
    translation=G4ThreeVector(0,LBNENumiData->CPipeY0[0],0);
    STgtRing = new G4SubtractionSolid("STgtRing",STgtRing,CPG_solid,G4Transform3D(rotation,translation));
    translation=G4ThreeVector(0,-LBNENumiData->CPipeY0[0],0);
    STgtRing = new G4SubtractionSolid("RingSolid",STgtRing,CPG_solid,G4Transform3D(rotation,translation));
    // clip off edges so that target fits inside
    G4double TGT_w=LBNENumiData->TargetSWidth/2.;
    G4double TGT_h=LBNENumiData->TargetSHeight/2.;
    G4double TGT_l=LBNENumiData->TargetSLength/2.; //these are 0.5*true dimension
    TGT_box_solid=new G4Box("TgtBox",1.1*TGT_w,1.025*TGT_h,TGT_l);
    STgtRing = new G4SubtractionSolid("RingSolid",STgtRing,TGT_box_solid);
    lvTgtRing=new G4LogicalVolume(STgtRing,GetMaterial(LBNENumiData->TgtRingGeantMaterial[ii]),vol_name.append("LV"),0,0,0);
    new G4PVPlacement(0,TgtRingPosition,vol_name,lvTgtRing,pvTargetMotherVol,false,0);
  }

  //Cooling pipes
  G4Tubs* CP_solid;
  G4Tubs* CP_water_solid;
  G4Torus* CPipe_tor;
  G4Torus* CPipeW_tor;
  for (G4int ii=0;ii<LBNENumiData->NCPipeN;ii++){
    sprintf(no,"%d",ii+1);
    vol_name="PV"; vol_name=vol_name.append(LBNENumiData->CPipeVolName[ii]);
    Sname="S";  Sname=Sname.append(LBNENumiData->CPipeVolName[ii]);
    LVname="LV"; LVname=LVname.append(LBNENumiData->CPipeVolName[ii]);
    PVname=""; PVname=PVname.append(LBNENumiData->CPipeVolName[ii]);
 
    if (LBNENumiData->CPipeCurvRad[ii]==0){
      CP_solid=new G4Tubs(Sname,LBNENumiData->CPipeRadiusIn[ii],LBNENumiData->CPipeRadiusOut[ii]-tgtGap,LBNENumiData->CPipeLength[ii]/2.-tgtGap,0.,360.*deg);
      LVCPipe[ii]=new G4LogicalVolume(CP_solid,GetMaterial(LBNENumiData->CPGeantMat[ii]),LVname,0,0,0);
      // Position the pipe
      rotation=G4RotationMatrix(0,0,0);
      rotation.rotateX(atan(LBNENumiData->CPipeDYDZ[ii]));
      rotation.rotateY(atan(LBNENumiData->CPipeDXDZ[ii]));
      translation=G4ThreeVector(-(sin(atan(LBNENumiData->CPipeDXDZ[ii])))*cos(atan(LBNENumiData->CPipeDYDZ[ii]))*LBNENumiData->CPipeLength[ii]/2.,(sin(atan(LBNENumiData->CPipeDYDZ[ii])))*LBNENumiData->CPipeLength[ii]/2.,(1-cos(atan(LBNENumiData->CPipeDXDZ[ii]))*cos(atan(LBNENumiData->CPipeDYDZ[ii])))*LBNENumiData->CPipeLength[ii]/2.);
      G4ThreeVector CPipe_position=G4ThreeVector(LBNENumiData->CPipeX0[ii],LBNENumiData->CPipeY0[ii],LBNENumiData->CPipeZ0[ii]+LBNENumiData->CPipeLength[ii]/2.)-TargetMVOrigin-translation;
      PVCPipe[ii]=new G4PVPlacement(G4Transform3D(rotation,CPipe_position),PVname,LVCPipe[ii],pvTargetMotherVol,false,0);
      if (LBNENumiData->CPipeFilledWater[ii]) {
	Sname=Sname.append("_water");
	LVname=LVname.append("_water");
	PVname=PVname.append("_water");
	if (LBNENumiData->CPipeRadiusIn[ii]==0) {
	  CP_water_solid=new 
	    G4Tubs(Sname,0.,LBNENumiData->CPipeRadiusOut[ii]-LBNENumiData->CPipeWallThick[ii],LBNENumiData->CPipeLength[ii]/2.-tgtGap,0.,360.*deg);
	}
	else {
	  CP_water_solid=new 
	    G4Tubs(Sname,LBNENumiData->CPipeRadiusIn[ii]+LBNENumiData->CPipeWallThick[ii],LBNENumiData->CPipeRadiusOut[ii]-LBNENumiData->CPipeWallThick[ii],LBNENumiData->CPipeLength[ii]/2.-tgtGap,0.,360.*deg);
	}
	LVCPipeW[ii]=new G4LogicalVolume(CP_water_solid,Water,LVname,0,0,0);
	G4VisAttributes * WaterPipeAtt=new G4VisAttributes(G4Colour(0.,0.,1.));
	LVCPipeW[ii]->SetVisAttributes(WaterPipeAtt);
	new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),PVname,LVCPipeW[ii],PVCPipe[ii],false,0);
      }
    }
    
    if (LBNENumiData->CPipeCurvRad[ii]!=0)
    {
      G4double dPhi=LBNENumiData->CPipeCloseAng[ii]-LBNENumiData->CPipeOpenAng[ii];
      /*                        
      if( (-LBNENumiData->CPipeOpenAng[ii]+LBNENumiData->CPipeCloseAng[ii]) >
	  LBNENumiData->CPipeOpenAng[ii])
      {
	dPhi=-2*LBNENumiData->CPipeOpenAng[ii]+LBNENumiData->CPipeCloseAng[ii]; 
      }
      else 
      {
	dPhi=360.*deg-2*LBNENumiData->CPipeOpenAng[ii]+LBNENumiData->CPipeCloseAng[ii]; 
        //there is a bug in G4Torus (geant4.6) visualization, 
        //so use this to visualize it; should be fixed in 4.7.x
      }
      */
/*
      CPipe_tor=new G4Torus(Sname,LBNENumiData->CPipeRadiusOut[ii]-LBNENumiData->CPipeWallThick[ii],LBNENumiData->CPipeRadiusOut[ii],LBNENumiData->CPipeCurvRad[ii],LBNENumiData->CPipeOpenAng[ii],dPhi);
      LVCPipe[ii]=new G4LogicalVolume(CPipe_tor,GetMaterial(LBNENumiData->CPGeantMat[ii]),LVname,0,0,0);

      // Position the pipe  
      rotation=G4RotationMatrix(0,0,0);
      rotation.rotateX(atan(LBNENumiData->CPipeDYDZ[ii]));
      rotation.rotateY(atan(LBNENumiData->CPipeDXDZ[ii]));
      translation=G4ThreeVector(-(sin(atan(LBNENumiData->CPipeDXDZ[ii])))*cos(atan(LBNENumiData->CPipeDYDZ[ii]))*LBNENumiData->CPipeLength[ii]/2.,(sin(atan(LBNENumiData->CPipeDYDZ[ii])))*LBNENumiData->CPipeLength[ii]/2.,(1-cos(atan(LBNENumiData->CPipeDXDZ[ii]))*cos(atan(LBNENumiData->CPipeDYDZ[ii])))*LBNENumiData->CPipeLength[ii]/2.);
      G4ThreeVector CPipe_position=G4ThreeVector(LBNENumiData->CPipeX0[ii],LBNENumiData->CPipeY0[ii],LBNENumiData->CPipeZ0[ii]+LBNENumiData->CPipeLength[ii]/2.)-TargetMVOrigin-translation;
      PVCPipe[ii]=new G4PVPlacement(G4Transform3D(rotation,CPipe_position),PVname,LVCPipe[ii],pvTargetMotherVol,false,0);
      
      if (LBNENumiData->CPipeFilledWater[ii]) {
	Sname=Sname.append("_water");
	LVname=LVname.append("_water");
	PVname=PVname.append("_water"); 
	CPipeW_tor=new G4Torus(Sname,0.,(LBNENumiData->CPipeRadiusOut[ii]-LBNENumiData->CPipeWallThick[ii])-tgtGap,LBNENumiData->CPipeCurvRad[ii],LBNENumiData->CPipeOpenAng[ii],dPhi);
	LVCPipeW[ii]=new G4LogicalVolume(CPipeW_tor,Water,LVname,0,0,0);
	G4VisAttributes * WaterPipeAtt=new G4VisAttributes(G4Colour(0.,0.,1.)); 
	LVCPipeW[ii]->SetVisAttributes(WaterPipeAtt);
	new G4PVPlacement(G4Transform3D(rotation,CPipe_position),PVname,LVCPipeW[ii],pvTargetMotherVol,false,0);
      }
    }
  }

*/

  G4cout << "Target Constructed" << G4endl;  


  G4cout << "Constructing NUMI Baffle..." << G4endl;  

  LBNEDetectorConstruction::ConstructNUMIBaffle();

  G4cout << "...NUMI Baffle Constructed." << G4endl;  

  
}


//----------------------------------------------------------------------
void LBNEDetectorConstruction::ConstructNUMIBaffle()
{ 
   LBNENumiDataInput *LBNENumiData = LBNENumiDataInput::GetLBNENumiDataInput();

   G4ThreeVector targetHallPosition=G4ThreeVector(0,0,LBNENumiData->TargetAreaLength/2.+LBNENumiData->TargetAreaZ0);
   
   // Baffle 
   G4double HPBRin    = LBNENumiData->HPBaffleRin;
   G4double HPBRout   = LBNENumiData->HPBaffleRout;
   G4double HPBlength = LBNENumiData->HPBaffleLength;
   G4Tubs* sBaffle    = new G4Tubs("sBaffle",HPBRin,HPBRout,HPBlength/2.,0.,360.*deg);
   G4LogicalVolume* lvBaffle = new G4LogicalVolume(sBaffle,Target,"lvBaffle",0,0,0);
   G4ThreeVector bafflePos   = G4ThreeVector(LBNENumiData->HPBaffleX0,
					     LBNENumiData->HPBaffleY0,
					     LBNENumiData->HPBaffleZ0+HPBlength/2.)-targetHallPosition;
   G4PVPlacement* pvbaffle = new G4PVPlacement(0,bafflePos,"pvBaffleMother",lvBaffle,TGAR,false,0);
//   pvbaffle -> CheckOverlaps();
   
/*
  G4cout << "Z0   Position of baffle = " << (bafflePos.z() - (HPBlength/2.) )/m << " m " << G4endl;
  G4cout << "Z    Position of baffle = " << (bafflePos.z())/m << " m " << G4endl;
  G4cout << "ZEnd Position of baffle = " << (bafflePos.z() + (HPBlength/2.) )/m << " m " << G4endl;

*/
}

