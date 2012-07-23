#include "LBNEDetectorConstruction.hh"

#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4BREPSolidPCone.hh"
#include "G4Polycone.hh"
#include "G4FieldManager.hh"

#include "G4SubtractionSolid.hh"
#include "G4UserLimits.hh"

#include "LBNEMagneticField.hh"

#include "LBNEDataInput.hh"

//-----------------------------------------------------------------------------
void LBNEDetectorConstruction::ConstructLBNEHorns()
{
   LBNEDataInput *LBNEData = LBNEDataInput::GetLBNEDataInput();

   //
   //Determine the length, height and x and y positions of the
   //horn neck and end tracking planes from info from the Target hall shielding
   //
   fTrackingPlane_halfwidth  = (fTgtHallShield_closest_xplus - fTgtHallShield_closest_xminus)/2.0;
   fTrackingPlane_halfheight = (fTgtHallShield_closest_yplus - fTgtHallShield_closest_yminus)/2.0;
   fTrackingPlane_X0         = (fTgtHallShield_closest_xplus + fTgtHallShield_closest_xminus)/2.0;
   fTrackingPlane_Y0         = (fTgtHallShield_closest_yplus + fTgtHallShield_closest_yminus)/2.0;

   G4int nHorns      = LBNEData-> GetNumberOfHorns();//NPhorns;
   vint_t hornNParts = LBNEData-> GetVectorOfNHornParts();//PhornNParts;
   
   for(int ihorn = 0; ihorn < nHorns; ++ihorn)
   {
      int jstart = 1;
      for(int i = 0; i < ihorn; ++i)
      {
	 jstart += hornNParts[i];
      } 
      jstart -= 1;

      //std::cout << "ihorn = " << ihorn << " nparts = " << hornNParts[ihorn] << " jstart = " << jstart << std::endl;

      ConstructLBNEHorn(ihorn+1, hornNParts[ihorn], jstart);
   }

   
   
}

//-----------------------------------------------------------------------------
void LBNEDetectorConstruction::ConstructLBNEHorn(G4int nhorn, G4int nparts, G4int jstart)
{

   LBNEDataInput *LBNEData = LBNEDataInput::GetLBNEDataInput();
   

   // Mother Volume position
   G4ThreeVector target_hall_position=G4ThreeVector(0,0,LBNEData->TargetAreaLength/2.+LBNEData->TargetAreaZ0);
   G4Material* material;
   
   //Horns
   G4double z[20];
   G4double rmin[20];
   G4double rmax[20];



   G4int partcounter = 0;
   // Create mother volumes 
   for (G4int jj = jstart; jj < nparts+jstart ; ++jj)
   {
      ++partcounter;
      G4String vol_name = ""; 

      char no[3];
      sprintf(no,"%d",jj+1);
      std::stringstream phHornName;
      phHornName << "PH0" << nhorn << "-0" << partcounter;





      //std::cout << "nhorn = " << nhorn << " nparts+jstart = " << nparts+jstart << " jstart = " << jstart << " jj = " << jj << std::endl;

      //std::cout << "Horn " << nhorn << " Mother Part 0" << partcounter << std::endl;

      G4double rminmin = 10.0*m;
      for (G4int ii=0;ii<LBNEData->PhornNpoint[jj]+1;ii++)
      {
	 z[ii]=((LBNEData->PhornZ2[jj]-LBNEData->PhornZ1[jj])/LBNEData->PhornNpoint[jj]*ii);
	 rmin[ii]=LBNEphornRgivenZ(LBNEData->PhornAin[jj],LBNEData->PhornBin[jj],LBNEData->PhornCin[jj],z[ii]+LBNEData->PhornZ1[jj]);
	 rmax[ii]=LBNEData->PhornROCout[jj];
	 
	 if(rmin[ii] < rminmin) rminmin = rmin[ii]; 

          
	 //std::cout << "z[" << ii << "] = " << z[ii] 
	//	   << " rmin[" << ii << "] = " << rmin[ii] 
	//	   << " rmax[" << ii << "] = " << rmax[ii] 
	//	   << " rminmin = " << rminmin << std::endl;
	  
      }
      
      std::cout << "Constructing " << phHornName.str() << "..."<< std::endl;
      vol_name = phHornName.str() + "_mother_solid";
      
      Horn_PM[jj]= new G4Polycone(vol_name,0.,360.*deg,LBNEData->PhornNpoint[jj]+1,z,rmin,rmax);
      
      vol_name = phHornName.str() + "_mother_lv";
      Horn_PM_lv[jj]=new G4LogicalVolume(Horn_PM[jj],Air,vol_name,0,0,0);      

      vol_name = phHornName.str() + "_mother";
      PHORN[jj]=new G4PVPlacement(0,G4ThreeVector(0,0,LBNEData->PhornZ0[jj]+LBNEData->PhornZ1[jj])-target_hall_position, 
				  vol_name,Horn_PM_lv[jj],TGAR,false,0);
      




      


      //
      // Inner conductor
      //
      std::cout << "Horn " << nhorn << " Inner Conductor " << std::endl;
      for (G4int ii=0;ii<LBNEData->PhornNpoint[jj]+1;ii++)
      {
	 z[ii]=((LBNEData->PhornZ2[jj]-LBNEData->PhornZ1[jj])/LBNEData->PhornNpoint[jj]*ii);
	 rmin[ii]=LBNEphornRgivenZ(LBNEData->PhornAin[jj],LBNEData->PhornBin[jj],LBNEData->PhornCin[jj],z[ii]+LBNEData->PhornZ1[jj]);
	 rmax[ii]=LBNEphornRgivenZ(LBNEData->PhornAout[jj],LBNEData->PhornBout[jj],LBNEData->PhornCout[jj],z[ii]+LBNEData->PhornZ1[jj]);

             	 
		 //std::cout << "z[" << ii << "] = " << z[ii] 
		   //   << " rmin[" << ii << "] = " << rmin[ii] 
		     // << " rmax[" << ii << "] = " << rmax[ii] << std::endl;
	     
      }
      
      vol_name = phHornName.str() + "_IC_solid";
      
      //G4BREPSolidPCone* Horn_in=new G4BREPSolidPCone(vol_name,0.,360.*deg,LBNEData->PhornNpoint[jj]+1,0.,z,rmin,rmax);
      G4VSolid* Horn_in= new G4Polycone(vol_name,0.,360.*deg,LBNEData->PhornNpoint[jj]+1,z,rmin,rmax);
      material=GetMaterial(LBNEData->PhornGEANTmat[jj]);
      vol_name = phHornName.str() + "_IC_lv";
      G4LogicalVolume* Horn_in_lv=new G4LogicalVolume(Horn_in,material,vol_name,0,0,0);
      // Magnetic field
////////////////////////////////
//      G4FieldManager* FieldMgr = new G4FieldManager(lbneMagFieldIC); //create a local field		 
      G4FieldManager* FieldMgr = new G4FieldManager(fHornICBFieldVec[jj]); //create a local field		 
      FieldMgr->SetDetectorField(fHornICBFieldVec[jj]); //set the field 
      FieldMgr->CreateChordFinder(fHornICBFieldVec[jj]); //create the objects which calculate the trajectory
      Horn_in_lv->SetFieldManager(FieldMgr,true); //attach the local field to logical volume
	 
      vol_name = phHornName.str() + "_IC";
      
      //G4PVPlacement* PI0_place = new G4PVPlacement(0,G4ThreeVector(0,0,0), vol_name,Horn_in_lv,PHORN[jj],false,0); 
      new G4PVPlacement(0,G4ThreeVector(0,0,0), vol_name,Horn_in_lv,PHORN[jj],false,0); 
      
      //	 
      // end IC
      //
      
      


      //
      // Outer conductor
      //
      std::cout << "Horn " << nhorn << " Outer Conductor " << std::endl;
      vol_name = phHornName.str() + "_OC_solid";
      G4Tubs* Horn_out=new 
	 G4Tubs(vol_name,LBNEData->PhornROCin[jj],LBNEData->PhornROCout[jj],(LBNEData->PhornZ2[jj]-LBNEData->PhornZ1[jj])/2.,0.,360.*deg);
      material=GetMaterial(LBNEData->PhornGEANTmat[jj]);
  
          
      //std::cout << "jj = " << jj << " outerconductor in rad = " 
	//	<< LBNEData->PhornROCin[jj] << " outerconductor out rad = " 
	//	<< LBNEData->PhornROCout[jj] << std::endl;
      
      
      vol_name = phHornName.str() + "_OC_lv";
      G4LogicalVolume* Horn_out_lv=new G4LogicalVolume(Horn_out,material,vol_name,0,0,0);
      // Magnetic field
/////////////////////////////////////////////
      //G4FieldManager* FieldMgr2 = new G4FieldManager(lbneMagFieldOC); //create a local field
      G4FieldManager* FieldMgr2 = new G4FieldManager(fHornOCBFieldVec[jj]);
      FieldMgr2->SetDetectorField(fHornOCBFieldVec[jj]); //set the field 
      FieldMgr2->CreateChordFinder(fHornOCBFieldVec[jj]); //create the objects which calculate the trajectory
      Horn_out_lv->SetFieldManager(FieldMgr2,true); //attach the local field to logical volume
      
      vol_name = phHornName.str() + "_OC";
      
      //G4PVPlacement* PO0_place = new G4PVPlacement(0,G4ThreeVector(0,0,((LBNEData->PhornZ2[jj]-LBNEData->PhornZ1[jj])/2.)), 
      //						   vol_name,Horn_out_lv,PHORN[jj],false,0);
      new G4PVPlacement(0,G4ThreeVector(0,0,((LBNEData->PhornZ2[jj]-LBNEData->PhornZ1[jj])/2.)), 
			vol_name,Horn_out_lv,PHORN[jj],false,0);
      
      //
      //end OC
      //


	 
      //
      // Inside horns (field part)
      //
      std::cout << "Horn " << nhorn << " Inside " << std::endl;
      for (G4int kk=0;kk<LBNEData->PhornNpoint[jj]+1;kk++)
      {
	 rmin[kk]=LBNEphornRgivenZ(LBNEData->PhornAout[jj],LBNEData->PhornBout[jj],LBNEData->PhornCout[jj], z[kk]+LBNEData->PhornZ1[jj]);
	 rmax[kk]=LBNEData->PhornROCin[jj];

	 //std::cout << "z[" << kk << "] = " << z[kk] 
	//	   << " rmin[" << kk << "] = " << rmin[kk] 
	//	   << " rmax[" << kk << "] = " << rmax[kk] << std::endl;
      }
      vol_name = phHornName.str() + "_in_solid";
      //G4BREPSolidPCone* Horn_inside=new G4BREPSolidPCone(vol_name,0.,360.*deg,LBNEData->PhornNpoint[jj]+1,0.,z,rmin,rmax);
      G4VSolid* Horn_inside= new G4Polycone(vol_name,0.,360.*deg,LBNEData->PhornNpoint[jj]+1,z,rmin,rmax);	 
      vol_name = phHornName.str() + "_in_lv";
      G4LogicalVolume* Horn_inside_lv=new G4LogicalVolume(Horn_inside,Vacuum,vol_name,0,0,0);
      // Magnetic field  
      //G4FieldManager* FieldMgr3 = new G4FieldManager(lbneMagField); //create a local field      
      G4FieldManager* FieldMgr3 = new G4FieldManager(fHornBFieldVec[jj]); //create a local field      
      FieldMgr3->SetDetectorField(fHornBFieldVec[jj]); //set the field 
      FieldMgr3->CreateChordFinder(fHornBFieldVec[jj]); //create the objects which calculate the trajectory 
      Horn_inside_lv->SetFieldManager(FieldMgr3,true); //attach the local field to logical volume
      
      Horn_inside_lv->SetUserLimits(new G4UserLimits(10.*cm));
      
      vol_name = phHornName.str() + "_in";
      //vol_name="PF0";
      //vol_name.append(no);
      //G4PVPlacement* PF0_place = new G4PVPlacement(0,G4ThreeVector(0,0,0), vol_name,Horn_inside_lv,PHORN[jj],false,0);
      new G4PVPlacement(0,G4ThreeVector(0,0,0), vol_name,Horn_inside_lv,PHORN[jj],false,0);
      
      //
      //end inside the horn (field part)
      //


      
      
      
      //
      //Create 2 Horn Neck Tracking volumes around the neck of the horn 
      //PhornNeckZ1 and Z2 are positive
      //
      if( !(LBNEData->PhornNeckZ1[jj] < 0.0) && !(LBNEData->PhornNeckZ2[jj] < 0.0))
      {
	 
	 std::cout << "Horn " << nhorn << " Neck Tracking Volume " << std::endl;
         //
         //used to tell what Part of the horn is the neck in LBNEAnalysis
         //
	 LBNEData->fHornNeckPartName.push_back(phHornName.str());
	 
	 //
	 //outer neck plane
	 //
	 G4double hornNeckPlaneOut_length = LBNEData->PhornNeckZ2[jj]-LBNEData->PhornNeckZ1[jj];	    
	 //G4double hornNeckPlaneOut_length = 0.1*m; //for visualization need to make the thickness larger 	    
	 //Make the width and height the "hole" that the Target shielding makes in the Target Hall
	 G4double hornNeckPlaneOut_halfwidth   = fTrackingPlane_halfwidth;
	 G4double hornNeckPlaneOut_halfheight  = fTrackingPlane_halfheight;
	 
	 std::stringstream phNeckName;
	 phNeckName << "PH0" << nhorn;
	 vol_name = phNeckName.str() + "NeckPlaneOut_solid";
	 //vol_name = phHornName.str() + "NeckPlaneOut_solid";
	 G4Box* hornNeckPlaneOut_solid = new G4Box(vol_name, hornNeckPlaneOut_halfwidth, 
	    					   hornNeckPlaneOut_halfheight, hornNeckPlaneOut_length/2.);
	 
	 //
	 //need to subtract the horn itself from the plane
	 //
	 G4double z_neck;
	 G4double rmin_neck;
	 G4double rmax_neck;
	 
	 z_neck=((LBNEData->PhornNeckZ2[jj]-LBNEData->PhornNeckZ1[jj]));
	 rmin_neck=LBNEphornRgivenZ(LBNEData->PhornAin[jj],LBNEData->PhornBin[jj],LBNEData->PhornCin[jj],z_neck+LBNEData->PhornZ1[jj]);
	 rmax_neck=LBNEData->PhornROCout[jj];
	 
         
	 //std::cout << "z  = " << z_neck << " rmin = " << rmin_neck << " rmax = " << rmax_neck << std::endl;
	 
	 
	 G4Tubs* horn_solid=new 
	    G4Tubs("HORNSOLID",0.,rmax_neck,hornNeckPlaneOut_length,0.,360.*deg);
	 
	 G4RotationMatrix  subrotation    = G4RotationMatrix(0,0,0);
	 //G4ThreeVector     subtranslation = G4ThreeVector(0,0,0);
	 G4ThreeVector     subtranslation = G4ThreeVector(LBNEData->PhornX0[jj]-fTrackingPlane_X0,LBNEData->PhornY0[jj]-fTrackingPlane_Y0,0);
	 G4SubtractionSolid*  hornNeckPlaneOut_subsolid    = 0;
	 
	 vol_name = phNeckName.str() + "NeckPlaneOut_subsolid";
	 //vol_name = phHornName.str() + "NeckPlaneOut_subsolid";
	 hornNeckPlaneOut_subsolid = new G4SubtractionSolid(vol_name, 
							    hornNeckPlaneOut_solid, horn_solid, 
							    G4Transform3D(subrotation,subtranslation));
	 
	 vol_name = phNeckName.str() + "NeckPlaneOut_lv";
	 //vol_name = phHornName.str() + "NeckPlaneOut_lv";
	 G4LogicalVolume* hornNeckPlaneOut_lv = new G4LogicalVolume(hornNeckPlaneOut_subsolid,He,vol_name,0,0,0);
	 //HornNeckPlaneOutLogVec.push_back(hornNeckPlaneOut_lv);
	 
	 vol_name = phNeckName.str() + "NeckPlaneOut";
	 //vol_name = phHornName.str() + "NeckPlaneOut";
	 //this is the position of the NeckPlaneOut volume wrt the target hall
	 G4ThreeVector  hornNeckPlaneOut_position =  G4ThreeVector(fTrackingPlane_X0,
								   fTrackingPlane_Y0,
								   LBNEData->PhornZ0[jj]+LBNEData->PhornNeckZ1[jj])-target_hall_position;
	 
	 //std::cout << "x = " <<  hornNeckPlaneOut_position.x() 
	//	   << " y = " <<  hornNeckPlaneOut_position.y() 
	//	   << " z = " << hornNeckPlaneOut_position.z() << std::endl;
	 

	 //G4PVPlacement* hornNeckPlaneOut = 
	 new G4PVPlacement(0, hornNeckPlaneOut_position, vol_name, hornNeckPlaneOut_lv, TGAR,false,0); 
	 //HornNeckPlaneOutPhysVec.push_back(hornNeckPlaneOut);
	 
	 
	 
	 
	 
	 //
	 //inner neck plane
	 //
	 
	 G4double z_neckin[20];
	 G4double rmin_neckin[20];
	 G4double rmax_neckin[20];
	 
	 for (G4int ii=0;ii<2;ii++)
	 {
	    z_neckin[ii]=((LBNEData->PhornNeckZ2[jj]-LBNEData->PhornNeckZ1[jj])/1.0*ii);
	    rmax_neckin[ii]=LBNEphornRgivenZ(LBNEData->PhornAin[jj],LBNEData->PhornBin[jj],LBNEData->PhornCin[jj],z[ii]+LBNEData->PhornZ1[jj]);
	    rmin_neckin[ii]=0.0;
	    	    
	    
	    //std::cout << "z_neckin[" << ii << "] = " << z_neckin[ii] 
		 //     << " rmin_neckin[" << ii << "] = " << rmin_neckin[ii] 
		 //     << " rmax_neckin[" << ii << "] = " << rmax_neckin[ii] << std::endl;
	    
	 }
	 
	 vol_name = phNeckName.str() + "NeckPlaneIn_solid";
	 //vol_name = phHornName.str() + "NeckPlaneIn_solid";
	 //G4Polycone* hornNeckPlaneIn_solid = new G4Polycone(vol_name,0.,360.*deg,2,0.,z_neckin,rmin_neckin,rmax_neckin);
	 G4Polycone* hornNeckPlaneIn_solid = new G4Polycone(vol_name,0.,360.*deg,2,z_neckin,rmin_neckin,rmax_neckin);
	 
	 
	 vol_name = phNeckName.str() + "NeckPlaneIn_lv";
	 //vol_name = phHornName.str() + "NeckPlaneIn_lv";
	 G4LogicalVolume* hornNeckPlaneIn_lv = new G4LogicalVolume(hornNeckPlaneIn_solid,He,vol_name,0,0,0);
	 
	 //this is the position of the NeckPlaneIn volume wrt the target hall
	 //vol_name = phHornName.str() + "NeckPlaneIn";
	 vol_name = phNeckName.str() + "NeckPlaneIn";
	 G4ThreeVector  hornNeckPlaneIn_position =  G4ThreeVector(LBNEData->PhornX0[jj],
								  LBNEData->PhornY0[jj],
								  LBNEData->PhornZ0[jj]+LBNEData->PhornNeckZ1[jj])-target_hall_position;
	 
	 //G4PVPlacement* hornNeckPlaneIn_pl = 
	 new	G4PVPlacement(0, hornNeckPlaneIn_position, vol_name, hornNeckPlaneIn_lv, TGAR,false,0); 
	 
	 
	 
	 
      }
      //
      //end horn neck tracking volumes 
      //


      
      
      
      
      //
      // Front and End cover
      //
      if (LBNEData->PhornThickFront[jj]!=0)
      {
	 std::stringstream phFrontName;
	 phFrontName << "PH0" << nhorn;
	 vol_name = phFrontName.str() + "_frontcap_solid";
	 //vol_name="PC0";
	 //vol_name.append(no);
	 G4double rin=rmin[0];
	 G4double rout=LBNEData->PhornROCout[jj];
	 G4Tubs* Horn_front_cover=new 
	    G4Tubs(vol_name,rin,rout,(LBNEData->PhornThickFront[jj])/2.,0.,360.*deg);
	 material=GetMaterial(LBNEData->PhornGEANTmat[jj]);
	 vol_name = phFrontName.str() + "_frontcap_lv";
	 G4LogicalVolume* Horn_fc_lv=new G4LogicalVolume(Horn_front_cover,material,vol_name,0,0,0);
	 G4ThreeVector PC_position=G4ThreeVector(0,0,LBNEData->PhornZ0[jj]+LBNEData->PhornZ1[jj]-(LBNEData->PhornThickFront[jj])/2.)
	    -target_hall_position;
	 vol_name = phFrontName.str() + "_frontcap";
	 //G4PVPlacement* PC0_place = new G4PVPlacement(0,PC_position, vol_name,Horn_fc_lv,TGAR,false,0);
	 new G4PVPlacement(0,PC_position, vol_name,Horn_fc_lv,TGAR,false,0);
	 
      }

      

      if (LBNEData->PhornThickEnd[jj]!=0)
      {
	 std::stringstream phEndName;
	 phEndName << "PH0" << nhorn;
	 vol_name = phEndName.str() + "_endcap_solid";
	 //vol_name="PC0";
	 //vol_name.append(no);
	 G4double rin=rmin[LBNEData->PhornNpoint[jj]];
	 G4double rout=LBNEData->PhornROCout[jj];
	 G4Tubs* Horn_end_cover=new 
	    G4Tubs(vol_name,rin,rout,(LBNEData->PhornThickEnd[jj])/2.,0.,360.*deg);
	 material=GetMaterial(LBNEData->PhornGEANTmat[jj]);
	 vol_name = phEndName.str() + "_endcap_lv";	    
	 G4LogicalVolume* Horn_ec_lv=new G4LogicalVolume(Horn_end_cover,material,vol_name,0,0,0);
	 G4ThreeVector PC_position=G4ThreeVector(0,0,LBNEData->PhornZ0[jj]+LBNEData->PhornZ2[jj]+(LBNEData->PhornThickEnd[jj])/2.)
	    -target_hall_position;
	 vol_name = phEndName.str() + "_endcap";
	 //G4PVPlacement* PC0_place = new G4PVPlacement(0,PC_position, vol_name,Horn_ec_lv,TGAR,false,0);
	 new G4PVPlacement(0,PC_position, vol_name,Horn_ec_lv,TGAR,false,0);
	 
	 


	 
	 //
	 //Place plane volume at the end of the horn. For Tracking.
	 //This can only be used when no Target hall shielding is constructed.
	 //otherwise volumes will overlap
	 //
	 G4double hornEndPlane_length = 0.000001*m;
	 
	 //Make the width and height the "hole" that the shielding
	 // makes in the target hall
	 G4double hornEndPlane_halfwidth   = fTrackingPlane_halfwidth;
	 G4double hornEndPlane_halfheight  = fTrackingPlane_halfheight;
	 
	 vol_name = phEndName.str() + "EndPlane_solid";
	 G4Box* hornEndPlane_solid = new G4Box(vol_name, hornEndPlane_halfwidth, 
					       hornEndPlane_halfheight, hornEndPlane_length/2.);
	 
	 vol_name = phEndName.str() + "EndPlane_lv";
	 G4LogicalVolume* hornEndPlane_lv = new G4LogicalVolume(hornEndPlane_solid, He, vol_name, 0, 0, 0);
	 
	 G4ThreeVector hornEndPlane_position = PC_position + G4ThreeVector(fTrackingPlane_X0,fTrackingPlane_Y0,(LBNEData->PhornThickEnd[jj]/2.)+(hornEndPlane_length/2.));
	 
	 vol_name = phEndName.str() + "EndPlane";
	 //G4PVPlacement* hornEndPlane_pl = new G4PVPlacement(0,hornEndPlane_position, vol_name,hornEndPlane_lv,TGAR,false,0);
	 new G4PVPlacement(0,hornEndPlane_position, vol_name,hornEndPlane_lv,TGAR,false,0);
	 //end horn end plane tracking volume

	 
      }
      //
      //end front and end cover
      //
      	 
	 
   }


   



   G4cout << "Horn " << nhorn << " constructed" << G4endl;

}



G4double LBNEDetectorConstruction::LBNEphornRgivenZ(G4double a, G4double b, G4double c, G4double z)
{
   G4double r;
   if (b!=0)
   {
      if (a > 0) { r = sqrt((a-z)/b)+c;}
      else     { r = sqrt((a+z)/b)+c;}
   }
   else { r=c; }
   
  return r;
}

