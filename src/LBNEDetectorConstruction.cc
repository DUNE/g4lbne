//----------------------------------------------------------------------
// $Id: LBNEDetectorConstruction.cc,v 1.3 2013/01/31 19:23:30 loiacono Exp $
//----------------------------------------------------------------------

#include <fstream>

#include "LBNEDetectorConstruction.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Polycone.hh"
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
#include "G4PVReplica.hh"
#include "G4AssemblyVolume.hh"
#include "LBNEMagneticField.hh"
#include "G4Transform3D.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"

#include "G4RegionStore.hh"
#include "G4SolidStore.hh"
#include "G4GeometryManager.hh"
#include "G4FieldManager.hh"

#include "G4RunManager.hh"

#include "G4VisExtent.hh"

//-------------------------------------------------------------------------
LBNEDetectorConstruction::LBNEDetectorConstruction()
{
   //This must Scan the input file     
   LBNEData = LBNEDataInput::GetLBNEDataInput();
   
   if(LBNEData->GetDebugLevel() > 0)
   { std::cout << "LBNEDetectorConstruction Constructor Called..." << std::endl; }
   
   
   if(!LBNEData)
   {
      std::cout << "********************************BIG PROBLEM*****************************" << std::endl; 
      std::cout << "********************************BIG PROBLEM*****************************" << std::endl; 
      std::cout << "********************************BIG PROBLEM*****************************" << std::endl; 
      std::cout << " LBNEData does not exist in LBNEDetectorConstruction Constructor." << std::endl;
      std::cout << "********************************BIG PROBLEM*****************************" << std::endl; 
      std::cout << "********************************BIG PROBLEM*****************************" << std::endl; 
      std::cout << "********************************BIG PROBLEM*****************************" << std::endl; 
      return;
   }
   

  G4int nhornparts = LBNEData -> GetTotalNumberOfHornParts();

  // Create Magnetic field Pointers for magnetic fields here***    
  //lbneMagField = new LBNEMagneticField(); 
  //lbneMagFieldIC = new LBNEMagneticFieldIC();
  //lbneMagFieldOC = new LBNEMagneticFieldOC();

  fHornBFieldVec.clear();
  fHornICBFieldVec.clear();
  fHornOCBFieldVec.clear();

  for(int npart = 1; npart < nhornparts+1; ++npart)
  {
     LBNEMagneticField* bf     = new LBNEMagneticField(); 
     LBNEMagneticFieldIC* bfic = new LBNEMagneticFieldIC();
     LBNEMagneticFieldOC* bfoc = new LBNEMagneticFieldOC();

     bf   -> SetHornCurrent(LBNEData->GetHornCurrentForHornPart(npart));
     bfic -> SetHornCurrent(LBNEData->GetHornCurrentForHornPart(npart));
     bfoc -> SetHornCurrent(LBNEData->GetHornCurrentForHornPart(npart));
     
     fHornBFieldVec.push_back(bf);
     fHornICBFieldVec.push_back(bfic);
     fHornOCBFieldVec.push_back(bfoc);
  }


  // create commands for interactive definition of the geometry
  detectorMessenger = new LBNEDetectorMessenger(this);

  DefineMaterials();

  if(LBNEData->GetDebugLevel() > 0)
  { std::cout << "...LBNEDetectorConstruction Constructor Complete." << std::endl; }
  

}
//-------------------------------------------------------------------------
LBNEDetectorConstruction::~LBNEDetectorConstruction()
{

   if(LBNEData->GetDebugLevel() > 0)
   { std::cout << "LBNEDetectorConstruction Destructor Called..." << std::endl; }

   for(unsigned int i = 0; i < fHornBFieldVec.size(); ++i)
   {
      delete fHornBFieldVec[i];
      delete fHornICBFieldVec[i];
      delete fHornOCBFieldVec[i];
   }

   fHornBFieldVec.clear();
   fHornICBFieldVec.clear();
   fHornOCBFieldVec.clear();

//   delete lbneMagField; 
//   delete lbneMagFieldIC;
//   delete lbneMagFieldOC; 
   delete LBNEData;
   DestroyMaterials();

   delete detectorMessenger;

   if(LBNEData->GetDebugLevel() > 0)
   { std::cout << "...LBNEDetectorConstruction Destructor Complete." << std::endl; }

}
//-------------------------------------------------------------------------
G4VPhysicalVolume* LBNEDetectorConstruction::Construct()
{
   G4cout << G4endl;
   G4cout << G4endl;
   G4cout << "********************************************************************" << G4endl;
   G4cout << "********************************************************************" << G4endl;
   G4cout << "LBNEDetectorConstruction::Construct Called. Constructing Detector..." << G4endl;
   G4cout << "********************************************************************" << G4endl;
   
        
  if (!LBNEData) 
  {
     G4cout << G4endl;
     G4cout << G4endl;
     G4cout << "********************************************************************" << G4endl;
     G4cout << "********************************************************************" << G4endl; 
     G4cout << "********************************************************************" << G4endl;
     G4cout << "********************************************************************" << G4endl;
     G4cout << "I am in LBNEDetectorConstruction::Construct() - this if statement should never be executed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!." << G4endl;
     G4cout << "********************************************************************" << G4endl;
     G4cout << "********************************************************************" << G4endl;
     G4cout << "********************************************************************" << G4endl;
     G4cout << "********************************************************************" << G4endl;
     G4cout << "********************************************************************" << G4endl;

     
/////////////////////////////////
     /*
      //Scan the input file     
      LBNEData = LBNEDataInput::GetLBNEDataInput();

      // Pointers for magnetic fields ***    
      lbneMagField = new LBNEMagneticField(); 
      lbneMagFieldIC = new LBNEMagneticFieldIC();
      lbneMagFieldOC = new LBNEMagneticFieldOC();

      DefineMaterials();
     */
/////////////////////////////////
  }


  G4Box* ROCK_solid = new G4Box("ROCK_solid",LBNEData->RockRadius,
                                LBNEData->RockRadius,LBNEData->RockHalfLen); 
  ROCK_log = new G4LogicalVolume(ROCK_solid, DoloStone,"ROCK_log",0,0,0); 
  ROCK_log->SetVisAttributes(G4VisAttributes::Invisible);
  ROCK = new G4PVPlacement(0,G4ThreeVector(),ROCK_log,"ROCK",0,false,0);
  


/*
  LBNEDetectorConstruction::ConstructTesting();
  

*/




  if(LBNEData->GetSimulation() == "Standard Neutrino Beam")
  {

     ConstructLBNEDecayPipe(); //temporarily swapped with ConstructTargetHall...
     ConstructLBNETargetHall();
     ConstructLBNEShielding(); //MUST call this BEFORE ConstructHorns
     ConstructLBNEHorns();

     if(LBNEData->GetConstructTarget())
     {
	if(LBNEData->TargetShape == "NUMI" || 
	   LBNEData->TargetShape == "numi" || 
	   LBNEData->TargetShape == "Numi" || 
	   LBNEData->TargetShape == "NuMI")
	{
	  G4cout << "CONSTRUCTING NUMI TARGET" << G4endl;
	  ConstructNUMITarget();
	  G4cout << "CONSTRUCTING NUMI TARGET" << G4endl;
	}
	else
	{
	   ConstructLBNETarget();
	   if(LBNEData->HPBaffle) ConstructLBNEBaffle();
	}
	
     }

     //ConstructLBNEHadronAbsorber(); 
     
     CheckOverlaps();

     LBNEDetectorConstruction::PrintDetectorGeometry();
     
  }
  else if(LBNEData->GetSimulation() == "Target Tracking")
  {
     
     ConstructLBNETargetHall();

     if(LBNEData->GetConstructTarget())
     {
	if(LBNEData->TargetShape == "NUMI" || 
	   LBNEData->TargetShape == "numi" || 
	   LBNEData->TargetShape == "Numi" || 
	   LBNEData->TargetShape == "NuMI")
	{
	   ConstructNUMITarget();
	}
	else
	{
	   ConstructLBNETarget();
	   if(LBNEData->HPBaffle) ConstructLBNEBaffle();
	}
	
     }

     
     CheckOverlaps();
  }
  else if(LBNEData->GetSimulation() == "Horn 1 Tracking" ||
	  LBNEData->GetSimulation() == "Horn 2 Tracking")
  {


     ConstructLBNEDecayPipe(); //temporarily swapped with ConstructTargetHall...
     ConstructLBNETargetHall();
     ConstructLBNEShielding(); //MUST call this BEFORE ConstructHorns
     ConstructLBNEHorns();

     if(LBNEData->GetConstructTarget())
     {
	if(LBNEData->TargetShape == "NUMI" || 
	   LBNEData->TargetShape == "numi" || 
	   LBNEData->TargetShape == "Numi" || 
	   LBNEData->TargetShape == "NuMI")
	{
	   ConstructNUMITarget();
	}
	else
	{
	   ConstructLBNETarget();
	   if(LBNEData->HPBaffle) ConstructLBNEBaffle();
	}
	
     }

     CheckOverlaps();
  }
  else
  {
     if((LBNEData->GetSimulation()).empty())
     {
	std::cout << std::endl;
	std::cout << "*********************************************************" << std::endl;
	std::cout << "LBNEDetectorConstruction::Construct() - Possible Problem: "
		  << "No Simulation Set. Did not construct any volumes." << std::endl;
	std::cout << "*********************************************************" << std::endl;
	std::cout << std::endl;
     }
     else
     {
	std::cout << std::endl;
	std::cout << "*********************************************************" << std::endl;
	std::cout << "LBNEDetectorConstruction::Construct() - Possible Problem: "
		  << "Unknown Simulation \"" << LBNEData->GetSimulation() 
		  << "\". Did not construct any volumes." << std::endl;
	std::cout << "*********************************************************" << std::endl;
	std::cout << std::endl;
     }
	
  }







  //Set Vis Attributes according to solid material (only for volumes not explicitly set)
  G4LogicalVolumeStore* lvStore=G4LogicalVolumeStore::GetInstance();
  lvStore=G4LogicalVolumeStore::GetInstance();
  for (size_t ii=0;ii<(*lvStore).size();ii++){   
    if ((*lvStore)[ii]->GetVisAttributes()==0) {
      G4String matName=(*lvStore)[ii]->GetMaterial()->GetName();
      (*lvStore)[ii]->SetVisAttributes(GetMaterialVisAttrib(matName));
    }
  }



  G4cout << "********************************************************************" << G4endl;
  G4cout << "...Detector Construction Completed." << G4endl;
  G4cout << "********************************************************************" << G4endl;
  G4cout << "********************************************************************" << G4endl;
  G4cout << G4endl;
  G4cout << G4endl;
  


  return ROCK;
}

//-------------------------------------------------------------------------
void LBNEDetectorConstruction::PrintDetectorGeometry()
{

   std::cout << std::endl;
   std::cout << "============================================================" << std::endl;
   std::cerr << "     LBNEDetectorConstruction::PrintDetectorGeometry() - ..." << std::endl;   
   std::cout << "============================================================" << std::endl;

   
   LBNEDetectorConstruction::PrintDetectorGeometry("Target Hall", "TGAR");

  
   //
   //Target Hall Shielding
   //
   std::cout << std::endl;
   std::cout << "**********************************************" << std::endl;
   std::cout << "Target Hall Shielding makes a \"hole\" with..." << std::endl
	     << "     xmin   = " << fTgtHallShield_closest_xminus/m << " m" << "\t"
	     << fTgtHallShield_closest_xminus/cm << " cm" << std::endl
	     << "     xmax   = " << fTgtHallShield_closest_xplus/m << " m" << "\t"
	     << fTgtHallShield_closest_xplus/cm << " cm" << std::endl
	     << "     width  = " << (fTgtHallShield_closest_xplus-fTgtHallShield_closest_xminus)/m 
	     << " m" << "\t"
	     << (fTgtHallShield_closest_xplus-fTgtHallShield_closest_xminus)/cm 
	     << " cm" << std::endl
	     << "     ymin   = " << fTgtHallShield_closest_yminus/m << " m" << "\t"
	     << fTgtHallShield_closest_yminus/cm << " cm" << std::endl
	     << "     ymax   = " << fTgtHallShield_closest_yplus/m << " m" << "\t"
	     << fTgtHallShield_closest_yplus/cm << " cm" << std::endl
	     << "     width  = " << (fTgtHallShield_closest_yplus-fTgtHallShield_closest_yminus)/m 
	     << " m" << "\t"
	     << (fTgtHallShield_closest_yplus-fTgtHallShield_closest_yminus)/cm 
	     << " cm" << std::endl;

   std::cout << "**********************************************" << std::endl;
   std::cout << std::endl;
   //end tgt hall shielding

   G4int ntgt = LBNEData->TargetNtarget;
   for(int i = 1; i < ntgt+1; ++i)
   {
      std::stringstream strtgt;
      strtgt << "TGT" << i;
      LBNEDetectorConstruction::PrintDetectorGeometry("Target", strtgt.str());
   }


   G4int nHorns      = LBNEData->NPhorns;
   vint_t hornNParts = LBNEData->PhornNParts;
   for(int ihorn = 1; ihorn <= nHorns; ++ihorn)
   {
      int nparts = hornNParts[ihorn-1];
      for(int ipart = 1; ipart <= nparts; ++ipart)
      {  
	 std::stringstream hornName;
	 hornName << "PH0" << ihorn << "-0" << ipart;
	 LBNEDetectorConstruction::PrintDetectorGeometry(hornName.str()+"_mother", 
	 						 hornName.str()+"_mother");
	 LBNEDetectorConstruction::PrintDetectorGeometry(hornName.str()+"_in",
							 hornName.str()+"_in");
	 LBNEDetectorConstruction::PrintDetectorGeometry(hornName.str()+"_IC",
							 hornName.str()+"_IC");
	 LBNEDetectorConstruction::PrintDetectorGeometry(hornName.str()+"_OC",
							 hornName.str()+"_OC");
	 
      }

      std::stringstream hornNeckName;
      hornNeckName << "PH0" << ihorn << "NeckPlane";
      LBNEDetectorConstruction::PrintDetectorGeometry(hornNeckName.str()+"In", 
						      hornNeckName.str()+"In");
      LBNEDetectorConstruction::PrintDetectorGeometry(hornNeckName.str()+"Out", 
						      hornNeckName.str()+"Out");
      std::stringstream hornEndName;
      hornEndName << "PH0" << ihorn << "EndPlane";
      LBNEDetectorConstruction::PrintDetectorGeometry(hornEndName.str(), 
						      hornEndName.str());
   }



   LBNEDetectorConstruction::PrintDetectorGeometry("Decay Pipe Tunnel", "TUNE");
   LBNEDetectorConstruction::PrintDetectorGeometry("Decay Pipe Shielding", "DPipeShield");
   LBNEDetectorConstruction::PrintDetectorGeometry("Decay Pipe Wall", "DPipeWall");
   LBNEDetectorConstruction::PrintDetectorGeometry("Decay Volume", "DPipeVol");
   LBNEDetectorConstruction::PrintDetectorGeometry("Decay Pipe Upstream Window", "UPWN");
   LBNEDetectorConstruction::PrintDetectorGeometry("Decay Pipe Downstream Window", "DNWN");


   std::cout << "============================================================" << std::endl;
   std::cerr << "          DONE Printing Detector Geometry" << std::endl;   
   std::cout << "============================================================" << std::endl;
   std::cout << std::endl;
}

//-------------------------------------------------------------------------
void LBNEDetectorConstruction::PrintDetectorGeometry(const G4String &desc, const G4String &name)
{

   std::cout << std::endl;
   std::cout << "**********************************************" << std::endl;
   std::cout << desc << "..." << std::endl;
   G4VPhysicalVolume* physvol = GetPhysicalVolume(name);
   G4String physvolname = "";
   if(physvol)
   {
      physvolname = physvol -> GetName();

      const G4LogicalVolume *logvol  = physvol -> GetLogicalVolume();
      const G4String &logvolname     = logvol -> GetName();
      
      const G4VSolid *solidvol       = logvol   -> GetSolid();  
      const G4String &solidvolname   = solidvol -> GetName();
      
      const G4LogicalVolume *mothervol = physvol -> GetMotherLogical();
      G4String mothervolname = "";
      mothervolname = "";
      if(mothervol)
      {
	 mothervolname = mothervol -> GetName();
      }
      else 
	 std::cout << "   Can't get mother volume for volume " << physvolname << std::endl; 
      
      
      std::cout << "   Logical Volume        = " << logvolname << std::endl;
      std::cout << "   Solid Volume          = " << solidvolname << std::endl;
      std::cout << "   Mother Logical Volume = " << mothervolname << std::endl;

      //
      //Find World coordinates
      //
      G4RotationMatrix WorldRotation(0,0,0);
      G4ThreeVector    WorldTranslation(0,0,0);
      LBNEDetectorConstruction::GetWorldTransformation(physvol,
						       WorldRotation,
						       WorldTranslation);
      //

      LBNEDetectorConstruction::PrintSolidDescription(solidvol, 
						      logvol,
						      WorldRotation, 
						      WorldTranslation);

   }
   else
   {
      std::cerr << "   PROBLEM: " << desc << " physical volume named " 
		<< physvolname << " doesn't exist" << std::endl;
   }
   std::cout << "**********************************************" << std::endl;
   std::cout << std::endl;
   
}



 //-------------------------------------------------------------------------
G4VPhysicalVolume* LBNEDetectorConstruction::GetPhysicalVolume(G4String PVname)
{
  G4PhysicalVolumeStore* PVStore=G4PhysicalVolumeStore::GetInstance();
  for (size_t ii=0;ii<(*PVStore).size();ii++){
    if ((*PVStore)[ii]->GetName()==PVname) return (*PVStore)[ii];
  }
  G4cout << "LBNEDetectorConstruction: Volume "<<PVname<< " is not in Physical Volume Store"<<G4endl;
  return 0;
}




//-------------------------------------------------------------------------
void LBNEDetectorConstruction::GetWorldTransformation(G4VPhysicalVolume *physvol,
						      G4RotationMatrix &WorldRotation,
						      G4ThreeVector    &WorldTranslation)
{
   G4VPhysicalVolume* daughter = physvol;
   G4LogicalVolume* mother     = physvol->GetMotherLogical();
   G4AffineTransform  theTransformationFromPhysVolToWorld = G4AffineTransform();
   G4PhysicalVolumeStore* thePhysVolStore =G4PhysicalVolumeStore::GetInstance();
   while (mother)
   {
      theTransformationFromPhysVolToWorld *=
	 G4AffineTransform(daughter->GetFrameRotation(),daughter->GetObjectTranslation());
      for ( unsigned int i=0; i< thePhysVolStore->size();i++)
      {
	 if ((*thePhysVolStore)[i]->GetLogicalVolume() == mother)
	 {
	    daughter = (*thePhysVolStore)[i];
	    mother =daughter->GetMotherLogical();
	    break;
	 }//;              
      }
   }
   WorldRotation    = theTransformationFromPhysVolToWorld.NetRotation();
   WorldTranslation = theTransformationFromPhysVolToWorld.NetTranslation();
   
}

//-------------------------------------------------------------------------
void LBNEDetectorConstruction::PrintSolidDescription(const G4VSolid *solidvol,
						     const G4LogicalVolume *logvol,
						     G4RotationMatrix &,
						     G4ThreeVector    &WorldTranslation)
{

   //
   //Describe volume
   //
   if(!solidvol)
   {
      std::cerr << "LBNEDetectorConstruction::PrintSolidDescription - " 
		<< "Invalid solid volume pointer" << std::endl;
      return;
   }

   if(!logvol)
   {
      std::cerr << "LBNEDetectorConstruction::PrintSolidDescription - " 
		<< "Invalid logical volume pointer" << std::endl;
      return;
   }

   G4Material *material  = logvol -> GetMaterial();
   G4String materialName = "";
   G4int materialCode    = -99;
   if(material)
   {
      materialName = material -> GetName();
      materialCode = GetMaterialCode(materialName);
   }
   
   std::cout << "   Describing Solid Volume... " << std::endl;
   
   const G4Tubs*      solidvol_tub   = dynamic_cast<const G4Tubs*>(solidvol);
   const G4Box*       solidvol_box   = dynamic_cast<const G4Box*>(solidvol);
   const G4Polycone*  solidvol_pcone = dynamic_cast<const G4Polycone*>(solidvol);

   //
   //check for a Field
   //
   bool bfieldExists = false;
   double hCurrent = -999.0;
   const G4FieldManager *fldmgr = logvol -> GetFieldManager();
   if(fldmgr)
   {
      if(fldmgr -> DoesFieldExist())
      {
	 const G4Field* fld = fldmgr -> GetDetectorField();

	 const LBNEMagneticField*   hornbfield   = dynamic_cast<const LBNEMagneticField*>(fld);
	 const LBNEMagneticFieldIC* hornbfieldic = dynamic_cast<const LBNEMagneticFieldIC*>(fld);
	 const LBNEMagneticFieldOC* hornbfieldoc = dynamic_cast<const LBNEMagneticFieldOC*>(fld);

	 if(hornbfield)
	 {
	    hCurrent = hornbfield -> GetHornCurrent();
	    bfieldExists = true;
	 }
	 else if(hornbfieldic)
	 {
	    hCurrent = hornbfieldic -> GetHornCurrent();
	    bfieldExists = true;
	 }
	 else if(hornbfieldoc)
	 {
	    hCurrent = hornbfieldoc -> GetHornCurrent();
	    bfieldExists = true;
	 }

      }

   }
   //done checking for field
   
   if(solidvol_pcone)
   {
      std::cout << "Volume is a G4Polycone" << std::endl;
      std::cout << "Material Name = " << materialName << std::endl
		<< "Material Code = " << materialCode << std::endl;
      
      G4double zmax = -9999999.0;
      G4double zmin =  9999999.0;
      G4double rmax = -9999999.0;
      G4double rmin =  9999999.0;

      G4int numrzcorn = solidvol_pcone -> GetNumRZCorner();
      for(G4int icorn = 0; icorn < numrzcorn; ++icorn)
      {
	 G4double r = (solidvol_pcone -> GetCorner(icorn)).r;
	 G4double z = (solidvol_pcone -> GetCorner(icorn)).z;

	 if(r > rmax) rmax = r;
	 if(r < rmin) rmin = r;
	 if(z > zmax) zmax = z;
	 if(z < zmin) zmin = z;
	 
	 std::cout << "Corner " << icorn << " (r, z)" << std::endl
		   << "         (" << r/m << ",\t" << z/m << ") m" << std::endl
		   << "         (" << r/cm << ",\t" << z/cm << ") cm" << std::endl;
      }

      std::cout << std::endl;
      std::cout << "(zmin, zmax) = (" << zmin/m << ",\t" << zmax/m << ") m" << std::endl;
      std::cout << "               (" << zmin/cm << ",\t" << zmax/cm << ") cm" << std::endl;
      std::cout << "(rmin, rmax) = (" << rmin/m << ",\t" << rmax/m << ") m" << std::endl;
      std::cout << "               (" << rmin/cm << ",\t" << rmax/cm << ") cm" << std::endl;
      std::cout << std::endl;      
      std::cout << "Full Length = " << (zmax-zmin)/m << " m\t" << (zmax-zmin)/cm << "cm " << std::endl;

      if(bfieldExists)
	 std::cout << "Has a Magnetic Field with Current = " << hCurrent/ampere/1000. << " kA."<< std::endl;            	 
   }
   else if(solidvol_tub) 
   {
      
      G4double inrad       = solidvol_tub -> GetInnerRadius();
      G4double outrad      = solidvol_tub -> GetOuterRadius();
      G4double zhalflength = solidvol_tub -> GetZHalfLength();
      std::cout << "Volume is a G4Tubs" << std::endl;
      std::cout << "Material Name = " << materialName << std::endl
		<< "Material Code = " << materialCode << std::endl;
      std::cout << "inrad         = "  << inrad/m  << " m, " 
		<< "\t" << inrad/cm  << " cm, " 
		<< "\t" << inrad/mm  << " mm" << std::endl;
      std::cout << "outrad        = " << outrad/m << " m, " 
		<< "\t" << outrad/cm << " cm, " 
		<< "\t" << outrad/mm << " mm" << std::endl;
      std::cout << "half length z = " << zhalflength/m << " m, " 
		<< "\t" << zhalflength/cm << " cm, " 
		<< "\t" << zhalflength/mm << " mm" << std::endl;
      std::cout << "Full length z = " << zhalflength*2./m << " m, " 
		<< "\t" << zhalflength*2./cm << " cm, " 
		<< "\t" << zhalflength*2./mm << " mm" << std::endl;
      
      std::cout << "World Z0       = " << (WorldTranslation.z()-zhalflength)/m << " m" << "\t"  
		<< (WorldTranslation.z()-zhalflength)/cm << " cm" << "\t"
		<< (WorldTranslation.z()-zhalflength)/cm << " mm" << std::endl;

      
      std::cout << "World X Center = " << WorldTranslation.x()/m << " m" 
		<< "\t" << WorldTranslation.x()/cm << " cm" 
		<< "\t" << WorldTranslation.x()/mm << " mm" << std::endl; 
      std::cout << "World Y Center = " << WorldTranslation.y()/m << " m" 
		<< "\t" << WorldTranslation.y()/cm << " cm" 
		<< "\t" << WorldTranslation.y()/mm << " mm" << std::endl; 
      std::cout << "World Z (min, center, max) = (" << (WorldTranslation.z()-zhalflength)/m 
		<< ", " << WorldTranslation.z()/m 
		<< ", " << (WorldTranslation.z()+zhalflength)/m << ") m" << std::endl;
      std::cout << "                             (" << (WorldTranslation.z()-zhalflength)/cm 
		<< ", " << WorldTranslation.z()/cm 
		<< ", " << (WorldTranslation.z()+zhalflength)/cm << ") cm" << std::endl;
      std::cout << "                             (" << (WorldTranslation.z()-zhalflength)/mm 
		<< ", " << WorldTranslation.z()/mm 
		<< ", " << (WorldTranslation.z()+zhalflength)/mm << ") mm" << std::endl;

      if(bfieldExists)
	 std::cout << "Has a Magnetic Field with Current = " << hCurrent/ampere/1000. << " kA."<< std::endl;
      
      
   }
   else if(solidvol_box) 
   {
      
      G4double xhalflength = solidvol_box -> GetXHalfLength();
      G4double yhalflength = solidvol_box -> GetYHalfLength();
      G4double zhalflength = solidvol_box -> GetZHalfLength();
      
      std::cout << "Volume is a G4Box" << std::endl;
      std::cout << "Material Name = " << materialName << std::endl 
		<< "Material Code = " << materialCode << std::endl;
      std::cout << "half length x = " << xhalflength/m << " m, " 
		<< "\t" << xhalflength/cm << " cm, " 
		<< "\t" << xhalflength/mm << " mm" << std::endl;
      std::cout << "half length y = " << yhalflength/m << " m, " 
		<< "\t" << yhalflength/cm << " cm, " 
		<< "\t" << yhalflength/mm << " mm" << std::endl;
      std::cout << "half length z = " << zhalflength/m << " m, " 
		<< "\t" << zhalflength/cm << " cm, " 
		<< "\t" << zhalflength/mm << " mm" << std::endl;
      std::cout << "Full length z = " << zhalflength*2./m << " m, " 
		<< "\t" << zhalflength*2./cm << " cm, " 
		<< "\t" << zhalflength*2./mm << " mm" << std::endl;

      std::cout << "World Z0                   = " << (WorldTranslation.z()-zhalflength)/m << " m" << "\t"  
		<< (WorldTranslation.z()-zhalflength)/cm << " cm" << "\t"
		<< (WorldTranslation.z()-zhalflength)/cm << " mm" << std::endl;
      
      std::cout << "World X (min, center, max) = (" << (WorldTranslation.x()-xhalflength)/m 
		<< ", " << WorldTranslation.x()/m 
		<< ", " << (WorldTranslation.x()+xhalflength)/m << ") m" << std::endl;
      std::cout << "                             (" << (WorldTranslation.x()-xhalflength)/cm 
		<< ", " << WorldTranslation.x()/cm 
		<< ", " << (WorldTranslation.x()+xhalflength)/cm << ") cm" << std::endl;
      std::cout << "                             (" << (WorldTranslation.x()-xhalflength)/mm 
		<< ", " << WorldTranslation.x()/mm 
		<< ", " << (WorldTranslation.x()+xhalflength)/mm << ") mm" << std::endl; 
      std::cout << "World Y (min, center, max) = (" << (WorldTranslation.y()-yhalflength)/m 
		<< ", " << WorldTranslation.y()/m 
		<< ", " << (WorldTranslation.y()+yhalflength)/m << ") m" << std::endl;
      std::cout << "                             (" << (WorldTranslation.y()-yhalflength)/cm 
		<< ", " << WorldTranslation.y()/cm 
		<< ", " << (WorldTranslation.y()+yhalflength)/cm << ") cm" << std::endl;
      std::cout << "                             (" << (WorldTranslation.y()-yhalflength)/mm 
		<< ", " << WorldTranslation.y()/mm 
		<< ", " << (WorldTranslation.y()+yhalflength)/mm << ") mm" << std::endl; 
      std::cout << "World Z (min, center, max) = (" << (WorldTranslation.z()-zhalflength)/m 
		<< ", " << WorldTranslation.z()/m 
		<< ", " << (WorldTranslation.z()+zhalflength)/m << ") m" << std::endl;
      std::cout << "                             (" << (WorldTranslation.z()-zhalflength)/cm 
		<< ", " << WorldTranslation.z()/cm 
		<< ", " << (WorldTranslation.z()+zhalflength)/cm << ") cm" << std::endl;
      std::cout << "                             (" << (WorldTranslation.z()-zhalflength)/mm 
		<< ", " << WorldTranslation.z()/mm 
		<< ", " << (WorldTranslation.z()+zhalflength)/mm << ") mm" << std::endl;

      if(bfieldExists)
	 std::cout << "Has a Magnetic Field with Current = " << hCurrent/ampere/1000. << " kA."<< std::endl;            
   }
   else
   {
      
      solidvol -> DumpInfo();
      if(bfieldExists)
	 std::cout << "Has a Magnetic Field with Current = " << hCurrent << " kA."<< std::endl;
   }
   
   //
   
}

//-------------------------------------------------------------------------
void LBNEDetectorConstruction::CheckOverlaps()
{


   G4PhysicalVolumeStore* PVStore=G4PhysicalVolumeStore::GetInstance();
   for (size_t ii=0;ii<(*PVStore).size();ii++)
   {
      
      
      
      G4VPhysicalVolume *physvol  = (*PVStore)[ii];
      

      physvol -> CheckOverlaps();
   }
   
   
   /*
   //G4PhysicalVolumeStore* PVStore=G4PhysicalVolumeStore::GetInstance();
   PVStore=G4PhysicalVolumeStore::GetInstance();
   for (size_t ii=0;ii<(*PVStore).size();ii++)
   {
   
      
      G4VPhysicalVolume *physvol  = (*PVStore)[ii];
      const G4String &physvolname = physvol -> GetName();
      
      std::cout << std::endl;
      std::cout << "//////////////////////////////////////////////////////////////" << std::endl;
      std::cout << "**************************************************************" << std::endl;
      std::cout << "Printing Info for Physical Volume " << physvolname << "..." << std::endl;
      
      const G4LogicalVolume *logvol  = physvol -> GetLogicalVolume();
      const G4String &logvolname     = logvol -> GetName();
      
      const G4VSolid *solidvol       = logvol   -> GetSolid();  
      const G4String &solidvolname   = solidvol -> GetName();
      
      const G4LogicalVolume *mothervol = physvol -> GetMotherLogical();
      G4String mothervolname = "";
      mothervolname = "";
      if(mothervol)
      {
	 mothervolname = mothervol -> GetName();
      }
      else 
	 std::cout << "Can't get mother volume for volume " << physvolname << std::endl; 
      
      
      std::cout << "   Logical Volume        = " << logvolname << std::endl;
      std::cout << "   Solid Volume          = " << solidvolname << std::endl;
      std::cout << "   Mother Logical Volume = " << mothervolname << std::endl;
      
      //
      //Find World coordinates
      //
      G4RotationMatrix WorldRotation(0,0,0);
      G4ThreeVector    WorldTranslation(0,0,0);
      LBNEDetectorConstruction::GetWorldTransformation(physvol,
						       WorldRotation,
						       WorldTranslation);
      //
      
      //
      //Describe volume
      //
      
      std::cout << "   Describing Solid Volume... " << std::endl;
            
      const G4Tubs* solidvol_tub = dynamic_cast<const G4Tubs*>(solidvol);
      const G4Box*  solidvol_box = dynamic_cast<const G4Box*>(solidvol);
      
      if(solidvol_tub) 
      {
	 
	G4double inrad       = solidvol_tub -> GetInnerRadius();
	G4double outrad      = solidvol_tub -> GetOuterRadius();
	G4double zhalflength = solidvol_tub -> GetZHalfLength();
	std::cout << "Volume is a G4Tubs" << std::endl;
	std::cout << "inrad         = "  << inrad/m  << " m, " 
		  << "\t" << inrad/cm  << " cm, " 
		  << "\t" << inrad/mm  << " mm" << std::endl;
	std::cout << "outrad        = " << outrad/m << " m, " 
		  << "\t" << outrad/cm << " cm, " 
		  << "\t" << outrad/mm << " mm" << std::endl;
	std::cout << "half length z = " << zhalflength/m << " m, " 
		  << "\t" << zhalflength/cm << " cm, " 
		  << "\t" << zhalflength/mm << " mm" << std::endl;

	std::cout << "World X Center = " << WorldTranslation.x()/m << " m" 
		  << "\t" << WorldTranslation.x()/cm << " cm" 
		  << "\t" << WorldTranslation.x()/mm << " mm" << std::endl; 
	std::cout << "World Y Center = " << WorldTranslation.y()/m << " m" 
		  << "\t" << WorldTranslation.y()/cm << " cm" 
		  << "\t" << WorldTranslation.y()/mm << " mm" << std::endl; 
	std::cout << "World Z (min, center, max) = (" << (WorldTranslation.z()-zhalflength)/m 
		  << ", " << WorldTranslation.z()/m 
		  << ", " << (WorldTranslation.z()+zhalflength)/m << ") m" << std::endl;
	std::cout << "                             (" << (WorldTranslation.z()-zhalflength)/cm 
		  << ", " << WorldTranslation.z()/cm 
		  << ", " << (WorldTranslation.z()+zhalflength)/cm << ") cm" << std::endl;
	std::cout << "                             (" << (WorldTranslation.z()-zhalflength)/mm 
		  << ", " << WorldTranslation.z()/mm 
		  << ", " << (WorldTranslation.z()+zhalflength)/mm << ") mm" << std::endl;

	
     }
     if(solidvol_box) 
     {

	G4double xhalflength = solidvol_box -> GetXHalfLength();
	G4double yhalflength = solidvol_box -> GetYHalfLength();
	G4double zhalflength = solidvol_box -> GetZHalfLength();

	
	std::cout << "half length x = " << xhalflength/m << " m, " 
		  << "\t" << xhalflength/cm << " cm, " 
		  << "\t" << xhalflength/mm << " mm" << std::endl;
	std::cout << "half length y = " << yhalflength/m << " m, " 
		  << "\t" << yhalflength/cm << " cm, " 
		  << "\t" << yhalflength/mm << " mm" << std::endl;
	std::cout << "half length z = " << zhalflength/m << " m, " 
		  << "\t" << zhalflength/cm << " cm, " 
		  << "\t" << zhalflength/mm << " mm" << std::endl;

	std::cout << "World X (min, center, max) = (" << (WorldTranslation.x()-xhalflength)/m 
		  << ", " << WorldTranslation.x()/m 
		  << ", " << (WorldTranslation.x()+xhalflength)/m << ") m" << std::endl;
	std::cout << "                             (" << (WorldTranslation.x()-xhalflength)/cm 
		  << ", " << WorldTranslation.x()/cm 
		  << ", " << (WorldTranslation.x()+xhalflength)/cm << ") cm" << std::endl;
	std::cout << "                             (" << (WorldTranslation.x()-xhalflength)/mm 
		  << ", " << WorldTranslation.x()/mm 
		  << ", " << (WorldTranslation.x()+xhalflength)/mm << ") mm" << std::endl; 
	std::cout << "World Y (min, center, max) = (" << (WorldTranslation.y()-yhalflength)/m 
		  << ", " << WorldTranslation.y()/m 
		  << ", " << (WorldTranslation.y()+yhalflength)/m << ") m" << std::endl;
	std::cout << "                             (" << (WorldTranslation.y()-yhalflength)/cm 
		  << ", " << WorldTranslation.y()/cm 
		  << ", " << (WorldTranslation.y()+yhalflength)/cm << ") cm" << std::endl;
	std::cout << "                             (" << (WorldTranslation.y()-yhalflength)/mm 
		  << ", " << WorldTranslation.y()/mm 
		  << ", " << (WorldTranslation.y()+yhalflength)/mm << ") mm" << std::endl; 
	std::cout << "World Z (min, center, max) = (" << (WorldTranslation.z()-zhalflength)/m 
		  << ", " << WorldTranslation.z()/m 
		  << ", " << (WorldTranslation.z()+zhalflength)/m << ") m" << std::endl;
	std::cout << "                             (" << (WorldTranslation.z()-zhalflength)/cm 
		  << ", " << WorldTranslation.z()/cm 
		  << ", " << (WorldTranslation.z()+zhalflength)/cm << ") cm" << std::endl;
	std::cout << "                             (" << (WorldTranslation.z()-zhalflength)/mm 
		  << ", " << WorldTranslation.z()/mm 
		  << ", " << (WorldTranslation.z()+zhalflength)/mm << ") mm" << std::endl;



     }
     else
     {

	solidvol -> DumpInfo();
     }
     std::cout << std::endl;
   
     //
     

     std::cout << "**************************************************************" << std::endl;
     std::cout << "//////////////////////////////////////////////////////////////" << std::endl;
     std::cout << std::endl;

     
  }
  */

}

//-------------------------------------------------------------------------
void LBNEDetectorConstruction::UpdateGeometry()
{

  // Clean old geometry
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  //G4RegionStore::GetInstance()->Clean();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  DefineMaterials();
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());

}

