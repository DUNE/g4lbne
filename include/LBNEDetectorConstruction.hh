//---------------------------------------------------------------------------// 
// $Id: LBNEDetectorConstruction.hh,v 1.5.2.15 2013/08/31 09:49:18 lebrun Exp $
//---------------------------------------------------------------------------// 

#ifndef LBNEDetectorConstruction_H
#define LBNEDetectorConstruction_H 1

#include <vector>

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "LBNEDecayPipe.hh"
#include "LBNEHadronAbsorber.hh"
#include "LBNEStandardPerson.hh"
//#include "LBNEDetectorMessenger.hh"
#include "LBNESubVolume.hh"
#include "LBNEVolumePlacements.hh"
#include "G4Element.hh"

class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4VisAttributes;

class LBNEMagneticField;
class LBNEMagneticFieldIC;
class LBNEMagneticFieldOC;

//class LBNESubVolume;
//class LBNEHadronAbsorber;
//class LBNEDecayPipe;
//class LBNETarget;
//class LBNEBaffle;
//class LBNEHornAssembly;

typedef std::vector<LBNESubVolume*> LBNEDet;

class LBNEDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  
  LBNEDetectorConstruction();
  ~LBNEDetectorConstruction();
 
  void Initialize();

  G4VPhysicalVolume* Construct();
  
  //void SetTargetZ0(G4double val);
  //void SetHornCurrent(G4double val);

  void CheckOverlaps();
    
  void UpdateGeometry(); // Obsolete..

  G4VPhysicalVolume* GetPhysicalVolume(G4String PVname);
  G4Material* GetMaterial(G4int matcode);
  void InitializeSubVolumes();
  void InitializeMaterials();
  void SetCheckOverlaps(bool val){ fCheckOverlaps = val; }
  void SetConstructTarget(bool val) { fConstructTarget = val; }
  void SetSimulationType(G4String val) { fSimulationType = val; }
  void SetBeamlineAngle(G4double angle) { fBeamlineAngle = angle; }

  bool HasBeenConstructed() const {return fHasBeenConstructed; }

private:

   bool fHasBeenConstructed; // The construct method has been called (only once per run...) 

  // Geometric instantiations
  LBNEVolumePlacements*                  fPlacementHandler;

  std::vector<LBNESubVolume*>        fSubVolumes;

  //LBNETarget                            *fTarget;
  //LBNEBaffle                            *fBaffle;
  //LBNEHornAssembly                      *fHornAssembly;
  LBNEDecayPipe*                        fDecayPipe;
  LBNEHadronAbsorber*                   fHadronAbsorber;
  LBNEStandardPerson*                   fStandardPerson;
  
  std::vector<LBNEMagneticField*>       fHornBFieldVec;
  std::vector<LBNEMagneticFieldIC*>     fHornICBFieldVec;
  std::vector<LBNEMagneticFieldOC*>     fHornOCBFieldVec;
 
  // Horn values
  std::vector<G4double> fHornCurrents;


  // Dimensions of (world-volume) pertinent features
  G4double fRockRadius;
  G4double fRockLength;
  G4double fRockX;
  G4double fRockY;
  G4double fRockZ;
  G4double fBeamlineAngle;
  G4double fDecayPipeWallThickness;
  G4double fTargetHallX;
  G4double fTargetHallY;
  G4double fTargetHallZ;
  G4double fDecayHallShieldingX;
  G4double fDecayHallShieldingY;
  G4double fDecayHallShieldingZ;
  G4double fDecayHallZ;
  G4double fAbsorberHallX;
  G4double fAbsorberHallY;
  G4double fAbsorberHallZ;
  // Flags that can be set by messenger
  bool fCheckOverlaps; // Obsolete, handled in LBNEVolumePlacements
  bool fConstructTarget;
  G4String fSimulationType;
  
  void PrintDetectorGeometry();
  void PrintDetectorGeometry(const G4String &desc, 
           const G4String &name);
  void GetWorldTransformation(G4VPhysicalVolume *physvol,
            G4RotationMatrix &WorldRotation,
            G4ThreeVector    &WorldTranslation);
  void PrintSolidDescription(const G4VSolid *solidvol,
           const G4LogicalVolume *logvol,
           G4RotationMatrix &WorldRotation,
           G4ThreeVector    &WorldTranslation);
//
// New v3.x construction.
//	   
  void ConstructUpstreamTarget(G4PVPlacement *phys); 

//
//LBNE stuff
  void ConstructLBNEBaffle();
  void ConstructLBNEDecayPipe();
  void ConstructLBNEHadronAbsorber(G4VPhysicalVolume* vPhys);
  void ConstructLBNEShielding();
  void LBNEDetermineTargetHallShieldingClosestApproach(G4int ii);
  G4double fTgtHallShield_closest_yplus;
  G4double fTgtHallShield_closest_yminus;
  G4double fTgtHallShield_closest_xplus;
  G4double fTgtHallShield_closest_xminus;

  void ConstructLBNETargetHall();
  void ConstructLBNETarget();

  void ConstructLBNEHorns();
  G4double fTrackingPlane_halfwidth;
  G4double fTrackingPlane_halfheight;
  G4double fTrackingPlane_X0;
  G4double fTrackingPlane_Y0;
  void ConstructLBNEHorn(G4int nhorn, G4int nparts, G4int jstart);
  G4double LBNEphornRgivenZ(G4double a, G4double b, G4double c, G4double z);
//
//


//
//Specifically Exact Numi Target
//This automatically constructs the NUMI baffle and budal monitor
   void ConstructNUMITarget(); //budal mon is constructed in this function
   void ConstructNUMIBaffle();
//
//

   ///use for testing gometry stuff
   void ConstructTesting();
  

   void InitializeMaterialsPostPreIdle();

 
//
  G4int GetMaterialCode(const G4String &matName);
  G4Material* GetMaterial(G4String matName);
  G4VisAttributes* GetMaterialVisAttrib(G4String matName);
  void DestroyMaterials();
  
  // Messenger
  G4UImessenger *fDetectorMessenger;
  // elements pointers
  G4Element *elC;
  G4Element *elN;
  G4Element *elO;
  
  // Materials
  G4Material* Vacuum;
  G4Material* DecayPipeVacuum;
  G4Material* NumiDecayPipeHelium;
  G4Material* Air;
  G4Material* Water;
  G4Material* He;
  G4Material* Beryllium;
  G4Material* C;
  G4Material* Al;
  G4Material* Ar;
  G4Material* Pb;
  G4Material* Fe;
  G4Material* CT852;
  G4Material* Steel316;
  G4Material* Titanium;
  G4Material* Concrete;
  G4Material* Shotcrete;
  G4Material* Rebar_Concrete;
  G4Material* Target;
  G4Material* HeliumTarget;
  G4Material* DolomiteRock;
  G4Material* DoloStone;
  G4Material* MaqShale;
  G4Material* Chert;
  G4Material* Pyrite;
  G4Material* MaqSiltstone;
  G4Material* var_Al;
  G4Material* var_Stl;
  G4Material* Slab_Stl;
  G4Material* Blu_Stl;
  G4Material* n1018_Stl;
  G4Material* A500_Stl;
  G4Material* M1018_Stl;
  G4Material* Alumina;
  G4Material* HeGas;
  G4Material* Drywall;
  G4Material* Paraffin;
  G4Material* graphiteBaffle;
  G4Material* DefaultMaterial;



  // Logical volumes
  //
  G4LogicalVolume* ROCK_log;
  G4LogicalVolume* TRGT_lv;
  // G4LogicalVolume* lvTUNE;
  G4LogicalVolume* BLK_log[100]; 
  G4LogicalVolume* CShld_log[16];
  G4LogicalVolume* TGAR_log;
  G4LogicalVolume* Horn_PM_lv[8];
  G4LogicalVolume* LVCPipe[20];
  G4LogicalVolume* LVCPipeW[20];
  G4LogicalVolume* HadrBox_log;
  G4LogicalVolume* ShldBox_log;

  // Physical volumes
  //
  G4VPhysicalVolume* fRock;
  G4VPhysicalVolume* fRvTUNE;
  G4VPhysicalVolume* TGAR;
  G4VPhysicalVolume* TRGT;
  G4VPhysicalVolume* PHORN[8];
  G4VPhysicalVolume* PVCPipe[20];
  G4VPhysicalVolume* CNT[20];
  G4VPhysicalVolume* HadrBox;
  G4VPhysicalVolume* ShldBox;
  
  //Solids
  //
  G4VSolid* BLK_solid[100];
  G4VSolid* CShld_solid[16];
  G4VSolid* Horn_PM[8];
};
#endif

