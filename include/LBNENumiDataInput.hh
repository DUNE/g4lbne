//----------------------------------------------------------------------
//
// $Id: LBNENumiDataInput.hh,v 1.1 2013/01/31 19:26:03 loiacono Exp $
//----------------------------------------------------------------------

#ifndef LBNENumiDataInput_h
#define LBNENumiDataInput_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include <vector>
//#include "NumiHornSpiderSupport.hh"
#include "G4LogicalVolume.hh"

class G4Material;

typedef std::vector<G4double> vdouble_t;
typedef std::vector<G4int> vint_t;
typedef std::vector<G4String> vstring_t;
typedef std::vector<G4bool> vbool_t;
//typedef std::vector<NumiHornSpiderSupport> vNumiHornSpiderSupport_t;

class LBNENumiDataInput
{
public:
   LBNENumiDataInput();
   ~LBNENumiDataInput();
   
   static LBNENumiDataInput* GetLBNENumiDataInput();


   void SetTargetX0  (G4double val)     { TargetX0   = val;}
   void SetTargetY0  (G4double val)     { TargetY0   = val;}
   void SetTargetZ0  (G4double val)     { TargetZ0   = val;}
   void SetTargetDxdz(G4double val)     { TargetDxdz = val;}
   void SetTargetDydz(G4double val)     { TargetDydz = val;}

   void SetTargetAreaGEANTmat   (G4int val) { TargetAreaGEANTmat = val;}   
   void SetTargetAreaZ0      (G4double val) { TargetAreaZ0       = val;}   
   void SetTargetAreaLength  (G4double val) { TargetAreaLength   = val;}   
   void SetTargetAreaHeight  (G4double val) { TargetAreaHeight   = val;}   
   void SetTargetAreaWidth   (G4double val) { TargetAreaWidth    = val;}   

public:
   friend class LBNEDetectorConstruction;

private:

   static LBNENumiDataInput* fLBNENumiDataInput;

   //////////////////////////////
   //
   // Target Area
   G4int TargetAreaGEANTmat;

   G4double TargetAreaZ0; 
   G4double TargetAreaLength; 
   G4double TargetAreaHeight; 
   G4double TargetAreaWidth;

   //////////////////////////////
   //
   // Target
   G4bool TargetEndRounded;

   G4int TargetGEANTmat;
   G4int TargetSegmentNo;

   G4double TargetX0;
   G4double TargetY0;
   G4double TargetZ0;
   G4double TargetDxdz; 
   G4double TargetDydz;
   G4double TargetSLength; 
   G4double TargetSWidth;
   G4double TargetSHeight; 
   G4double TargetA;
   G4double TargetDensity;
   G4double TargetZ; 
   G4double TargetRL;
   G4double TargetSegmentPitch;
   G4double TargetCPGRadius;
   G4double TargetCPGPosition;

   //////////////////////////////
   //
   //Rings holding target and cooling pipes
   G4int NTgtRingN;

   vint_t TgtRingGeantMaterial;

   vdouble_t TgtRingZ0;
   vdouble_t TgtRingLength; 
   vdouble_t TgtRingRin;
   vdouble_t TgtRingRout;
   
   vstring_t TgtRingVolName;
   
   //////////////////////////////
   //
   //Budal
   G4double BudalX0;
   G4double BudalY0;
   G4double BudalZ0;
   G4double BudalDxdz;
   G4double BudalDydz;
   
   //////////////////////////////
   //
   //HPBaffle 
   G4int HPBaffle;
   G4int HPBaffleGEANTMat;
   
   G4double HPBaffleX0;
   G4double HPBaffleY0;
   G4double HPBaffleZ0;
   G4double HPBaffleDXDZ;
   G4double HPBaffleDYDZ;
   G4double HPBaffleLength;
   G4double HPBaffleRin;
   G4double HPBaffleRout;
   
   //////////////////////////////
   //
   //Cooling pipes
   G4int NCPipeN;

   vbool_t CPipeFilledWater;

   vint_t CPGeantMat;

   vdouble_t CPipeX0;
   vdouble_t CPipeY0;
   vdouble_t CPipeZ0;
   vdouble_t CPipeLength;
   vdouble_t CPipeRadiusIn;
   vdouble_t CPipeRadiusOut;
   vdouble_t CPipeWallThick;
   vdouble_t CPipeDXDZ; 
   vdouble_t CPipeDYDZ;
   vdouble_t CPipeCurvRad;
   vdouble_t CPipeOpenAng;
   vdouble_t CPipeCloseAng;

   vstring_t CPipeVolName;
   
   //////////////////////////////
   //
   //Container
   G4int NContainerN;
   
   vint_t CTubeGeantMat;

   vdouble_t CTubeZ0;
   vdouble_t CTubeLength;
   vdouble_t CTubeRin;
   vdouble_t CTubeRout;

   vstring_t CTubeVolName;
   
   
/*
  
  
  // Horn 1 & 2

   //-----------------
   //do these variables get used?!
   //
   G4int PhornNphorn;
   
   vdouble_t PhornZ1, PhornZ2;  
   vint_t  PhornNpoint;  
   vdouble_t PhornAin, PhornBin, PhornCin, PhornAout, PhornBout, PhornCout;
   vdouble_t  PhornROCin, PhornROCout;
   vdouble_t PhornThickFront, PhornThickEnd;  
   vdouble_t PhornX0, PhornY0, PhornZ0, PhornDXDZ, PhornDYDZ, PhornCurrent;
   vint_t PhornGEANTmat; 
   vstring_t PhornName;
   //-----------------
  
  // Flux Area
   G4int nNear,nFar;
   vdouble_t xdet_near,ydet_near,zdet_near;
   vdouble_t xdet_far,ydet_far,zdet_far;
   vstring_t det_near_name, det_far_name;

   G4double HornCurrent; //old
	
   // Horn 1

   //----------------
   //these variables are very important (Laura)
   //
   G4double Horn1X0, Horn1Y0, Horn1Z0;
   //----------------
   
   G4int NPHorn1OCN,NPHorn1ICN,NPHorn1EndN;
  
  vdouble_t PHorn1OCRout,PHorn1OCRin,PHorn1OCZ0;
  vdouble_t PHorn1ICZ0;
  vint_t PHorn1ICNpoint;

  vdouble_t PHorn1EndZ0,PHorn1EndLength,PHorn1EndRout,PHorn1EndRin;
  vint_t PHorn1EndGeantMat;
  vstring_t PHorn1EndVolName;

  G4int NHorn1SpiderSupportPlanesN,NHorn1SpidersPerPlaneN;
  vdouble_t Horn1SpiderSupportZ0;
  vNumiHornSpiderSupport_t Horn1SS;
  
   // Horn 2

   //----------------
   //these variables are very important (Laura)
   //
   G4double Horn2X0, Horn2Y0, Horn2Z0;
   //----------------
   
  G4int NPHorn2OCN,NPHorn2ICN,NPHorn2EndN;
  
  vdouble_t PHorn2OCRout,PHorn2OCRin,PHorn2OCZ0;
  vdouble_t PHorn2ICZ0;
  vint_t PHorn2ICNpoint;

  vdouble_t PHorn2EndZ0,PHorn2EndLength,PHorn2EndRout,PHorn2EndRin;
  vint_t PHorn2EndGeantMat;
  vstring_t PHorn2EndVolName;

  G4int NHorn2SpiderSupportPlanesN,NHorn2SpidersPerPlaneN;
  vdouble_t Horn2SpiderSupportZ0;
  vNumiHornSpiderSupport_t Horn2SS;
*/


};
#endif
