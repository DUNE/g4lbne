//----------------------------------------------------------------------
//
//
// $Id: LBNENumiDataInput.cc,v 1.3 2013/08/17 17:25:31 ljf26 Exp $
//----------------------------------------------------------------------

//C++
#include <string>
#include <math.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdio.h>

#include "LBNENumiDataInput.hh"
#include "G4ThreeVector.hh"
//#include "NumiHornSpiderSupport.hh"
#include "G4UserLimits.hh"
#include "G4Material.hh"
#include "G4UnitsTable.hh"



static const G4double in = 2.54*cm;
static const G4double fTargetZ0_ref   = -0.35*m; //LE000 position
//static const G4double fHPBaffleZ0_ref = -2.53*m; //LE000 position
//static const G4double fBeamZ0_ref     = -4.0*m; //this is arbitrary right now
//Note: The baffle LE000 position was -3.04m. 
//This doesn't make sense. if the baffle
//is at -3.04 then the distance between the DS edge of the
//baffle and the US edge of the first Vertical target fin is 1.19m. 
//The Documentation, 
//http://www-numi.fnal.gov/numwork/tdh/TDH_V2_4.2.2-baffle.pdf, 
//says it should be 68cm

LBNENumiDataInput* LBNENumiDataInput::fLBNENumiDataInput = 0;

//---------------------------------------------------------------------------------
LBNENumiDataInput* LBNENumiDataInput::GetLBNENumiDataInput()
{
   if (!fLBNENumiDataInput) 
   {
      fLBNENumiDataInput = new LBNENumiDataInput();    
   }
   return fLBNENumiDataInput;
}

//---------------------------------------------------------------------------------
LBNENumiDataInput::LBNENumiDataInput()

{

   G4cout << "LBNENumiDataInput Constructor Called" << G4endl;
   if(fLBNENumiDataInput) {
       G4Exception("LBNENumiDataInput constructed twice.","",FatalException,"");
   }
    
   // 
   // Target
   //=======================================================================
   //
   TargetX0           = 0.0;
   TargetY0           = 0.0;
//  TargetZ0           = -0.35*m + TargetConfigZ;
   TargetZ0           = 0.0;
   TargetDxdz         = 0.0; // doesn't
   TargetDydz         = 0.0; // work properly yet
   TargetSLength      = 20.*mm;
   TargetSWidth       = 7.40E-03*m;
   TargetSHeight      = 18.0E-03*m;
   TargetCPGRadius    = 3.2*mm; // Cooling pipe groove
   TargetCPGPosition  = 10.7*mm;
   TargetEndRounded   = true;
   TargetSegmentNo    = 47;
   TargetSegmentPitch = 0.3*mm;
   TargetA            = 12.01*g/mole;
   TargetZ            = 6.;
   TargetDensity      = 1.78*g/cm3; //1.815*g/cm3;//1.754*g/cm3;
   TargetRL           = 25.692;
   TargetGEANTmat     = 18;
   //
   //end Target
   //----------------------------------------------------   

   
   //
   //Budal Monitor
   //this moves with the target -Laura
   //=======================================================================
   G4double TgtZ0BudalCENTERZDist = 157.3*mm;
   BudalX0 = 0.0;
   BudalY0 = 0.0;
   BudalZ0 = TargetZ0 - TgtZ0BudalCENTERZDist - TargetSLength/2.0;
   //BudalZ0 = -16.72*cm;
   BudalDxdz = 0.0;
   BudalDydz = 0.0;
   //   
   //end Budal Monitor
   //----------------------------------------------------   

   //
   //Baffle
   //this moves with the target -Laura
   //=======================================================================   
   //HPBaffleLength     =  1.20*m;
   HPBaffleLength     =  1.50*m; //Why was this 1.2m?!
   HPBaffleRin        =  6.5*mm;
   HPBaffleRout       =  3.*cm;
   HPBaffleGEANTMat   =  18;
   HPBaffleX0         =  0.00;
   HPBaffleY0         =  0.00;
   G4double TgtZ0BaffleDownZDist = 680*mm; //see note below
   HPBaffleZ0         = TargetZ0 - TgtZ0BaffleDownZDist -  HPBaffleLength;
   //HPBaffleZ0         = -3.04*m + TargetConfigZ; //for flugg
   //This doesn't make sense. 
   //If the baffle is at -3.04 then the distance between the DS edge of the
   //baffle and the US edge of the first Vertical target fin is 1.19m. 
   //The Documentation, 
   //http://www-numi.fnal.gov/numwork/tdh/TDH_V2_4.2.2-baffle.pdf, 
   //says it should be 68cm   
   HPBaffleDXDZ       =  0.0;
   HPBaffleDYDZ       =  0.0;
   //
   //end baffle
   //----------------------------------------------------   
   


   //
   //Cooling pipes
   //=======================================================================   
   NCPipeN = 19;
   //=======================================================================
   // CPipeDXDZ=-99999 and CPipeDYDZ=0 puts curved part in z-y plane
   //=======================================================================
   G4int CPGeantMat_[]        = {10     ,   10   ,10     , 10     , 10    , 10    ,  10   , 10    , 10     , 10           ,10            , 31     , 31      ,10        ,10        ,  10     ,  10     , 10      ,10  };
   G4bool CPipeFilledWater_[] = { true  , true   ,true   , true   ,  true , true  ,false  ,true   , false  , true         ,true          , true   ,true     ,true      ,true      , true    , true    , true    , true};
   G4double CPipeX0_[]        = {0.     ,0.      ,0.     , 0.     ,   0.  ,0.     , 0.    , 0.    ,  0.    , 0.           ,0.            , 0      , 0       ,0         ,0         , 0       , 0       , 0       , 0};
   G4double CPipeY0_[]        = {1.05e-2,-1.05e-2,1.05e-2,-1.05e-2,3.5e-2 ,-3.5e-2, 0.    , 0.    ,  0.    , 1.05e-2      ,-1.05e-2      , 5.95e-2, -5.95e-2,5.95e-2   ,-5.95e-2  , 5.95e-2 , -5.95e-2,5.95e-2  ,-5.95e-2};
   G4double CPipeZ0_[]        = {-0.275 , -0.275 ,-0.30  ,-0.30   , -.3001,-.3001 ,.969   , .9695 , .9735  , 0.955        ,0.955         , -0.215 , -0.215  ,-.071     ,-.071     ,-0.287   ,-0.287   , -.30    , -.30};
   G4double CPipeDXDZ_[]      = {    0  ,  0     , 0     ,  0     ,-99999 ,-99999 , 0     ,  0    , 0      , 0            ,0             , 0.     , 0.      ,0.        ,0         , 0.      , 0.      , 0.      , 0.}; 
   G4double CPipeDYDZ_[]      = {    0  ,  0     , 0     ,  0     , 0     ,0      , 0     ,  0    , 0      , 0            ,0             , 0.     , 0.      ,0.        ,0         , 0.      , 0.      , 0.      , 0.};
   G4double CPipeLength_[]    = {1.230  , 1.230  , .025  , .025   , 0     ,0      , 5e-4  ,  4e-3 , 3e-3   , 14.e-3      ,14.e-3         , 14.4e-2, 14.4e-2 ,2e-2      ,2e-2      , 7.2e-2      ,7.2e-2  , 13e-3   ,13e-3};
   G4double CPipeRadiusOut_[] = {3e-3   , 3e-3   ,3e-3   ,3e-3    , 3e-3  ,3e-3   ,13.52e-3,14.1e-3,14.1e-3, 3.4e-3       , 3.4e-3       , 5e-3   , 5e-3    ,4e-3      ,4e-3      , 7.5e-3      , 7.5e-3 , 3e-3    ,3e-3};
   G4double CPipeRadiusIn_[]  = {   0.  ,    0.  ,   0.  ,   0.   , 0.    ,   0.  ,7.52e-3,7.3e-3,7.3e-3   , 0.           , 0.           , 0      , 0       ,0         ,0         , 0           , 0        , 0.      ,0.}; 
   //CPipeRadiusIn is not the inner pipe radius - use radiusOut and wallthickness for that
   G4double CPipeWallThick_[] = {4e-4   ,4e-4    ,4e-4   ,4e-4    ,4e-4   ,4e-4   , 0     ,1.6e-3 , 0      , 0.8e-3       , 0.8e-3       , 1.e-3  , 1.e-3   ,1e-3      ,1e-3      , 1.1e-3      , 1.1e-3  , 4e-4    , 4e-4}; 
   G4double CPipeCurvRad_[]   = {0      , 0      , 0     , 0      ,2.45e-2,2.45e-2, 0     ,   0   , 0      , 0            , 0            , 0      , 0       ,0         ,0         , 0           , 0        , 0       , 0}; // straight tube if this is 0;
   G4double CPipeOpenAng_[]   = {0      , 0      , 0     , 0      ,90.    ,90.    , 0     ,   0   , 0      , 0            , 0            , 0      , 0       ,0         ,0         , 0           , 0        , 0       , 0   };
   G4double CPipeCloseAng_[]  = {0      , 0      , 0     , 0      ,270.   ,270.   , 0     ,   0   , 0      , 0            , 0            , 0      , 0       ,0         ,0         , 0           , 0        , 0       , 0  };
   G4String CPipeVolName_[]   = {"Pipe1","Pipe2" ,"Pipe3","Pipe4" ,"Pipe5","Pipe6","Pipe7","Pipe8","Pipe9" ,"PipeAdapter1","PipeAdapter2","PipeC1","PipeC2" ,"PipeEndT","PipeEndB","PipeBellowT","PipeBellowB"  ,"Pipe1tp","Pipe2btm" };
   
   for (G4int ii=0;ii<NCPipeN;ii++)
   {
      CPGeantMat.push_back(CPGeantMat_[ii]);
      CPipeFilledWater.push_back(CPipeFilledWater_[ii]);
      CPipeX0.push_back(CPipeX0_[ii]*m);
      CPipeY0.push_back(CPipeY0_[ii]*m);    
      CPipeZ0.push_back(CPipeZ0_[ii]*m);
      CPipeDXDZ.push_back(CPipeDXDZ_[ii]);
      CPipeDYDZ.push_back(CPipeDYDZ_[ii]);
      CPipeLength.push_back(CPipeLength_[ii]*m);
      CPipeRadiusOut.push_back(CPipeRadiusOut_[ii]*m);
      CPipeRadiusIn.push_back(CPipeRadiusIn_[ii]*m);
      CPipeWallThick.push_back(CPipeWallThick_[ii]*m);
      CPipeCurvRad.push_back(CPipeCurvRad_[ii]*m);
      CPipeOpenAng.push_back(CPipeOpenAng_[ii]*deg);
      CPipeCloseAng.push_back(CPipeCloseAng_[ii]*deg);
      CPipeVolName.push_back(CPipeVolName_[ii]);
   }
   
   //Container
   //=======================================================================
   // Z0 with respect to the first target fin
   NContainerN=21;
   
   G4double CTubeZ0_[]     ={-.42181, -.421556, -.41555, -.35855 , -0.3522 ,-.3332 ,-0.0802, -0.123  ,-0.109  ,-0.0572 ,-0.0492 , -0.04  ,-0.042 ,-.022 , -0.013   , -0.0035  ,0.011     , 0.011       ,  0.011      , 0.9785 , 0.9896  };
   G4double CTubeLength_[] ={.254e-3, 6.006e-3, 5.7e-2 , 6.35e-3 , 19e-3   , 253e-3, 23e-3 , 11e-2   , 66e-3  ,8e-3   , 17e-3  , 6e-3   ,0.02   ,15e-3  , 24e-3    , 14.5e-3  , 0.9785   ,1e-6         , 0.9786      ,5e-4    , 1e-6};
   G4double CTubeRin_[]    ={  0.   , 1.27e-2 , 1.7e-2 , 22.22e-3, 17.5e-3 , 77e-3 , 77e-3 , 14.6e-3 , 16.e-3 , 74e-3 , 24e-3  , 18.5e-3,16e-3  ,16e-3  , 14.6e-3  , 16e-3    , 14.6e-3  ,15.101e-3    , 15.1e-3     , 0.0    , 0.};
   G4double CTubeRout_[]   ={22e-3  , 34.67e-3, 1.9e-2 , 34.67e-3, 107e-3  , 83e-3 , 120e-3, 16.0e-3 , 21.4e-3,106e-3 , 106e-3 , 24e-3  ,18.5e-3,18.5e-3, 16e-3    , 16.4e-3  , 15e-3    , .35         , 15.101e-3   ,14.15e-3, 15.1e-3  };
   G4int CTubeGeantMat_[]  ={   5   ,  10     ,   10   , 10      , 10      , 10    , 10    ,  31     , 10     , 10    , 10     ,  10    , 10    ,10     , 10       , 9        , 9        ,15           ,  15         ,  5     ,  15};
   G4String CTubeVolName_[]={"BeUp1", "BeUp2" , "Added", "BeUp3" , "BFront", "Body", "BEnd","CerTube", "Conn1","CLid1", "CLid2", "Conn2","Conn3","Tube1a" ,"Tube1b", "AlTube1", "AlTube2","TGTExitCyl1","TGTExitCyl2", "BeDW" ,"TGTExitTop"};
   
   for (G4int ii=0;ii<NContainerN;ii++)
   {
      CTubeZ0.push_back(CTubeZ0_[ii]*m);
      CTubeLength.push_back(CTubeLength_[ii]*m);
      CTubeRin.push_back(CTubeRin_[ii]*m);
      CTubeRout.push_back(CTubeRout_[ii]*m);
      CTubeGeantMat.push_back(CTubeGeantMat_[ii]);
      CTubeVolName.push_back(CTubeVolName_[ii]);
   }
   
   //Rings holding target and cooling pipes
   NTgtRingN = 5;
   G4double TgtRingZ0_[]         = {-0.03   ,  0.212   , 0.455    , 0.697   , 0.939 };
   G4double TgtRingLength_[]     = {8e-3    ,  8e-3    , 8e-3     , 8e-3    , 8e-3 };
   G4double TgtRingRin_[]        = {9.5e-3  , 9.5e-3   , 9.5e-3   , 9.5e-3  , 9.5e-3};
   G4double TgtRingRout_[]       = {14.55e-3, 14.55e-3 , 14.55e-3 , 14.55e-3, 14.55e-3};
   G4int TgtRingGeantMaterial_[] = {9       , 9        , 9        , 9       , 9 };
   G4String TgtRingVolName_[]    = {"Ring1" , "Ring2"  , "Ring3"  , "Ring4" , "Ring5" };
   
   for (G4int ii=0;ii<NTgtRingN;ii++)
   {
      TgtRingZ0.push_back(TgtRingZ0_[ii]*m);
      TgtRingLength.push_back(TgtRingLength_[ii]*m);
      TgtRingRin.push_back(TgtRingRin_[ii]*m);
      TgtRingRout.push_back(TgtRingRout_[ii]*m);
      TgtRingGeantMaterial.push_back(TgtRingGeantMaterial_[ii]);
      TgtRingVolName.push_back(TgtRingVolName_[ii]);
   }
   
   
   /*  
   
   //
   //Horns
   //
   Horn1X0 = 0.0*cm;
   Horn1Y0 = 0.0*cm;
   Horn1Z0 = 3.0*cm;
   
   Horn2X0 = 0.0*cm;
   Horn2Y0 = 0.0*cm;
   Horn2Z0 = 10.0*m;
   
   
   
   
   NPHorn2EndN=3;
   
   G4double PHorn2EndZ0_[]     ={135.861        ,137.611     ,139.486};
   if (jCompare) {
      PHorn2EndZ0_[0] = 118.11;
      PHorn2EndZ0_[1] = 119.86;
      PHorn2EndZ0_[2] = 122.048;	
   }
   G4double PHorn2EndLength_[] ={1.75           ,2.188       ,.625};
   G4double PHorn2EndRin_[]    ={12.719         ,12.532      ,11.};
   G4double PHorn2EndRout_[]   ={14.405         ,14.469      ,12.532};
   G4int PHorn2EndGeantMat_[]  ={31             ,  9         ,  9 };  
   G4String PHorn2EndVolName_[]={"PHorn2InsRing","PHorn2CPB1","PHorn2CPB2"};
   
   for (G4int ii=0;ii<NPHorn2EndN;ii++){
      PHorn2EndZ0.push_back(PHorn2EndZ0_[ii]*in);
      PHorn2EndLength.push_back(PHorn2EndLength_[ii]*in);
      PHorn2EndRin.push_back(PHorn2EndRin_[ii]*in);
      PHorn2EndRout.push_back(PHorn2EndRout_[ii]*in);
      if(airhrn){
	 PHorn2EndGeantMat.push_back(15);
      }
      else{
	 PHorn2EndGeantMat.push_back(PHorn2EndGeantMat_[ii]);
      }
      PHorn2EndVolName.push_back(PHorn2EndVolName_[ii]);
   }
   
   NPHorn1EndN=3;
   G4double PHorn1EndZ0_[]     ={126.092        ,127.842     ,129.718};
   if (jCompare) {
      PHorn1EndZ0_[0] = 118.11;
      PHorn1EndZ0_[1] = 119.86;
      PHorn1EndZ0_[2] = 122.048;	
   }
   G4double PHorn1EndLength_[] ={1.75           ,2.188       ,.624};
   G4double PHorn1EndRin_[]    ={6.00           ,5.815       ,4.25};
   G4double PHorn1EndRout_[]   ={7.687          ,7.75        ,5.815};
   G4int PHorn1EndGeantMat_[]  ={31             ,  9         ,  9   };
   G4String PHorn1EndVolName_[]={"PHorn1InsRing","PHorn1CPB1","PHorn1CPB2"};
   
   for (G4int ii=0;ii<NPHorn1EndN;ii++){
      PHorn1EndZ0.push_back(PHorn1EndZ0_[ii]*in);
      PHorn1EndLength.push_back(PHorn1EndLength_[ii]*in);
      PHorn1EndRin.push_back(PHorn1EndRin_[ii]*in);
      PHorn1EndRout.push_back(PHorn1EndRout_[ii]*in);
      if(airhrn){ 
	 PHorn1EndGeantMat.push_back(15);
      }
      else{
	 PHorn1EndGeantMat.push_back(PHorn1EndGeantMat_[ii]);
      }
      PHorn1EndVolName.push_back(PHorn1EndVolName_[ii]);
   }
   
   //Spider supports for Horn1
   NHorn1SpiderSupportPlanesN=3;
   NHorn1SpidersPerPlaneN=3;
   Horn1SpiderSupportZ0.push_back(19.261*in);
   Horn1SpiderSupportZ0.push_back(42.862*in);
   Horn1SpiderSupportZ0.push_back(82.444*in);
   
   NumiHornSpiderSupport dummy=NumiHornSpiderSupport();
   dummy.stripW=.5*in;
   dummy.stripH=3.685*in;
   dummy.stripL=.076*in;
   dummy.topW=1.437*in;
   dummy.topH=.108*in;
   dummy.topL=.4*in;
   dummy.bottomW=1.437*in;
   dummy.bottomH=.542*in;
   dummy.bottomL=.4*in;
   dummy.bottomThickMid=.3*in;  //Thickness of bottom part in the middle
   dummy.bottomR=1.188*in;
   dummy.ceramicRodR=.491*in;
   
   Horn1SS.push_back(dummy);
   
   dummy.stripW=.5*in;
   dummy.stripH=3.123*in;
   dummy.stripL=.063*in;
   dummy.topW=1.437*in;
   dummy.topH=.108*in;
   dummy.topL=.4*in;
   dummy.bottomW=1.437*in;
   dummy.bottomH=.454*in;
   dummy.bottomL=.4*in;
   dummy.bottomThickMid=.3*in;  //Thickness of bottom part in the middle
   dummy.bottomR=1.75*in;
   dummy.ceramicRodR=.491*in;
   
   Horn1SS.push_back(dummy);
   
   dummy.stripW=.5*in;
   dummy.stripH=1.56*in;
   dummy.stripL=.031*in;
   dummy.topW=1.437*in;
   dummy.topH=.108*in;
   dummy.topL=.4*in;
   dummy.bottomW=1.437*in;
   dummy.bottomH=.379*in;
   dummy.bottomL=.4*in;
   dummy.bottomThickMid=.3*in;  //Thickness of bottom part in the middle
   dummy.bottomR=3.313*in;
   dummy.ceramicRodR=.491*in;
   
   Horn1SS.push_back(dummy);
   
   //Spider supports for Horn2
   NHorn2SpiderSupportPlanesN=1;
   NHorn2SpidersPerPlaneN=3;
   Horn2SpiderSupportZ0.push_back(59.809*in);
   
   dummy.stripW=.5*in;
   dummy.stripH=7.033*in;
   dummy.stripL=.125*in;
   dummy.topW=1.437*in;
   dummy.topH=.108*in;
   dummy.topL=.4*in;
   dummy.bottomW=1.437*in;
   dummy.bottomH=.345*in;
   dummy.bottomL=.4*in;
   dummy.bottomThickMid=.3*in;  //Thickness of bottom part in the middle
   dummy.bottomR=5.812*in;
   dummy.ceramicRodR=.491*in;
   
   Horn2SS.push_back(dummy);
   //---------------------------------------------------------
   */   
   
      G4cout << "********************************************************************" << G4endl;
     G4cout << "...LBNENumiDataInput Initialization Complete." << G4endl;
     G4cout << "********************************************************************" << G4endl;
     G4cout << "********************************************************************" << G4endl;
     G4cout << G4endl;
     G4cout << G4endl;
     
   
}
//---------------------------------------------------------------------------------
LBNENumiDataInput::~LBNENumiDataInput()
{
}



