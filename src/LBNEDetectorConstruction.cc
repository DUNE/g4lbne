//----------------------------------------------------------------------
// $Id: LBNEDetectorConstruction.cc,v 1.3.2.1 2013/05/14 10:37:50 robj137 Exp $
//----------------------------------------------------------------------

#include <fstream>

#include "LBNEDetectorConstruction.hh"
#include "LBNEDataInput.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UnitsTable.hh"

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
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"

#include "G4RegionStore.hh"
#include "G4SolidStore.hh"
#include "G4GeometryManager.hh"
#include "G4FieldManager.hh"

#include "G4RunManager.hh"

#include "G4VisExtent.hh"

//-------------------------------------------------------------------------
// Constructor, Destrutor, and Initialization
//---------------------------------------------------------------------------// 

LBNEDetectorConstruction::LBNEDetectorConstruction()
{
  InitializeSubVolumes();
  
  Initialize();
  fDetectorMessenger = new LBNEDetectorMessenger(this);
  //DefineMaterials();
}


LBNEDetectorConstruction::~LBNEDetectorConstruction()
{
  for(unsigned int i = 0; i < fHornBFieldVec.size(); ++i)
  {
     delete fHornBFieldVec[i];
     delete fHornICBFieldVec[i];
     delete fHornOCBFieldVec[i];
  }

  fHornBFieldVec.clear();
  fHornICBFieldVec.clear();
  fHornOCBFieldVec.clear();

  //DestroyMaterials();

  //for(size_t i = 0; i< fSubDetectors.size(); i++){
  //  delete fSubDetectors[i];
  //}

  delete fDetectorMessenger;
}

void LBNEDetectorConstruction::InitializeSubVolumes()
{
  fDecayPipe = new LBNEDecayPipe("DecayPipe");
  /*
  fTarget = new LBNETarget();
  fBaffle = new LBNEBaffle();
  fHornAssembly = new LBNEHornAssembly();
  fHadronAbsorber = new LBNEHadronAbsorber();
  fTarget->SetDefaults();
  fBaffle->SetDefaults();
  fHornAssembly->SetDefaults();
  fDecayPipe->SetDefaults();
  fHadronAbsorber->SetDefaults();
  */
  fSubDetectors.clear();
  fBeamlineAngle = 0*mrad;
}

void LBNEDetectorConstruction::Initialize()
{
  // Set standard (and safe) values for class variables
  fSimulationType = "Standard Neutrino Beam"; 
  fCheckOverlaps = false;
  fConstructTarget = true;
  
  fHornBFieldVec.clear();
  fHornICBFieldVec.clear();
  fHornOCBFieldVec.clear();
  /*
  for(int npart = 1; npart < nhornparts+1; ++npart)
  {
     LBNEMagneticField* bf     = new LBNEMagneticField(); 
     LBNEMagneticFieldIC* bfic = new LBNEMagneticFieldIC();
     LBNEMagneticFieldOC* bfoc = new LBNEMagneticFieldOC();

     bf   -> SetHornCurrent(fHornCurrents[npart-1]);
     bfic -> SetHornCurrent(fHornCurrents[npart-1]);
     bfoc -> SetHornCurrent(fHornCurrents[npart-1]);
     
     fHornBFieldVec.push_back(bf);
     fHornICBFieldVec.push_back(bfic);
     fHornOCBFieldVec.push_back(bfoc);
  }
*/
}


//-------------------------------------------------------------------------

G4VPhysicalVolume* LBNEDetectorConstruction::Construct() {
  G4Element* N  = new G4Element("Nitrogen","N" , 7., 14.01*g/mole);
  G4Element* O  = new G4Element("Oxygen"  ,"O" , 8., 16.00*g/mole);  
  G4Material* Air = new G4Material("Air"  , 1.290*mg/cm3, 2);
  Air->AddElement(N, 0.7);
  Air->AddElement(O, 0.3);
  fRockX = 60.0*m;
  fRockY = 60.0*m;
  fRockLength = 300.0*m;
  G4Box* ROCK_solid = new G4Box("ROCK_solid",fRockX/2, fRockY/2, fRockLength/2);
  G4LogicalVolume *RockLogical = new G4LogicalVolume(ROCK_solid, Air,"RockLogical",0,0,0); 
  //RockLogical->SetVisAttributes(G4VisAttributes::Invisible);
  ROCK = new G4PVPlacement(0,G4ThreeVector(),RockLogical,"ROCK",0,false,0);
  
  G4cout << "SIMULATION IS " << fSimulationType << G4endl;
  // First create the Target Hall, Pipe Hall, and Absorber Hall, and then
  // connect them together.

  G4double eps = 1e-6*cm;
  G4double concreteWidth= 2*m; // Shouldn't matter.

  fTargetHallX = 7.671*m;
  fTargetHallY = 11.862*m;
  fTargetHallZ = 28.2*m;

  fDecayPipeLength = fDecayPipe->GetDecayPipeLength();
  fDecayPipeRadius = fDecayPipe->GetDecayPipeRadius();
  fPipeHallShieldingX = 15.4*m;
  fPipeHallShieldingY = 15.4*m;
  fPipeHallShieldingZ = fDecayPipe->GetDecayPipeLength();
  fPipeHallLength = fDecayPipeLength;
  fAbsorberHallX = 10*m;
  fAbsorberHallY = 22*m;
  fAbsorberHallZ = 14*m;
  //fBeamlineAngle = 101*mrad;
  // First the outer concrete shell solids...
  G4Box *TargetHallConcreteSolid = new G4Box("TargetHallConcreteSolid",
                                             (fTargetHallX+concreteWidth*2)/2,
                                             (fTargetHallY+concreteWidth*2)/2,
                                             (fTargetHallZ+concreteWidth*2)/2);
  G4Box *PipeHallConcreteSolid = new G4Box("PipeHallConcreteSolid", 
                                            fPipeHallShieldingX/2,
                                            fPipeHallShieldingY/2,
                                            fPipeHallShieldingZ/2);
                                    
  G4Box *AbsorberHallConcreteSolid = new G4Box("AbsorberHallSolid",
                                     (fAbsorberHallX+concreteWidth*2)/2,
                                     (fAbsorberHallY+concreteWidth*2)/2,
                                     (fAbsorberHallZ+concreteWidth*2)/2);
  // And now the actual target 'halls'
  G4Box *TargetHallSolid = new G4Box("TargetHallSolid", 
                                      fTargetHallX/2,
                                      fTargetHallY/2,
                                      fTargetHallZ/2);

  G4Tubs *PipeHallSolid = new G4Tubs("PipeHallSolid", 0,fDecayPipeRadius+eps,
                                      fDecayPipeLength/2+eps, 0*deg, 360*deg);
                                    
  G4Box *AbsorberHallSolid = new G4Box("AbsorberHallSolid",
                                        fAbsorberHallX/2,
                                        fAbsorberHallY/2,
                                        fAbsorberHallZ/2);

  G4ThreeVector PipeTranslation(0,0,fPipeHallLength/2+fTargetHallZ/2);
  G4ThreeVector
  AbsorberTranslation(0,0,fAbsorberHallZ/2+fPipeHallLength+fTargetHallZ/2);
  G4RotationMatrix AbsorberRotation;
  AbsorberRotation.rotateY(fBeamlineAngle);
  G4cout << "Rotating beamline by " << fBeamlineAngle/mrad << " mrad" << G4endl;
  G4Transform3D transformAbsorber(AbsorberRotation,AbsorberTranslation);
  
  G4UnionSolid *HallConcreteSolid = new G4UnionSolid("TargetPipeHallConcreteSolid", 
                                             TargetHallConcreteSolid,
                                             PipeHallConcreteSolid,
                                             0, PipeTranslation);

  HallConcreteSolid = new G4UnionSolid("HallConcreteSolid", 
                               HallConcreteSolid,
                               AbsorberHallConcreteSolid,transformAbsorber);
                               
  G4UnionSolid *HallSolid = new G4UnionSolid("TargetPipeHallSolid", 
                                             TargetHallSolid,
                                             PipeHallSolid,
                                             0, PipeTranslation);

  HallSolid = new G4UnionSolid("HallSolid", 
                               HallSolid,
                               AbsorberHallSolid,transformAbsorber);


  G4LogicalVolume *hallConcreteLogical; 
  G4LogicalVolume *hallLogical; 
  
  //FIXME this needs to be concrete (duh)
  hallConcreteLogical = new G4LogicalVolume(HallConcreteSolid, G4Material::GetMaterial("Air"),
                                    "hallConcreteLogical",0,0,0);

  hallLogical = new G4LogicalVolume(HallSolid, G4Material::GetMaterial("Air"),
                                    "hallLogical",0,0,0);

  G4VPhysicalVolume *hallPlacement;
  G4VPhysicalVolume *hallConcretePlacement;
  hallConcretePlacement = new G4PVPlacement(0,-0.5*AbsorberTranslation,
                                            hallConcreteLogical,
                                            "hallConcretePlacement",
                                            RockLogical,false,0);
                                                    
  hallPlacement = new G4PVPlacement(0,G4ThreeVector(0,0,0),hallLogical,
                                    "hallPlacement",
                                    hallConcreteLogical,false,0);
                             

  // Now to calculate useful positions within the hall
  G4double hallLength = fTargetHallZ + fPipeHallLength + fAbsorberHallZ;
  G4cout << " Total hall length is " << hallLength/m << " m long" << G4endl;
  G4double decayPipePosition = 0.5*fTargetHallZ + fPipeHallLength/2;
  fDecayPipe->SetPlacement(0,0,decayPipePosition);
  //fDecayPipe->SetPlacement(0,0,-targetHallZ);
  //Center of the tube portion = 

    // Individual geometries within all halls:
    // baffle
    // target
    // horn 1
    // horn 2
    // decay pipe (with windows)
    // absorber

  // is baffle necessary????

  //fSubDetectors.push_back(fTarget);

  if( fSimulationType == "Standard Neutrino Beam" ||
      fSimulationType == "Horn 1 Tracking" ||
      fSimulationType == "Horn 2 Tracking") {
    // Then we need everything!
    //fSubDetectors.push_back(fBaffle);
    //fSubDetectors.push_back(fHornAssembly);
    fSubDetectors.push_back(fDecayPipe);
    //fSubDetectors.push_back(fHadronAbsorber);
  } else if(fSimulationType == "Target Tracking") {
    // FIXME .. anyting aside from not adding everythin but the target?
  } else { // unknown simulation type
    if((fSimulationType).empty()){
      std::cout << std::endl;
      std::cout << "*******************************************" << std::endl;
      std::cout << "LBNEDetectorConstruction::Construct() - Possible Problem: "
	  << "Constructing the Target volume and that's it." << std::endl;
      std::cout << "*******************************************" << std::endl;
      std::cout << std::endl;
      //Leaving for now, but will ditch once get the intiailize argument done
    } else { // FIXME Unknown simulation?
	std::cout << std::endl;
	std::cout << "*******************************************" << std::endl;
	std::cout << "LBNEDetectorConstruction::Construct() - Problem: "
    	  << "Unknown Simulation \'"<< fSimulationType << "\'"
	  << "Constructing the Target volume and that's it." << std::endl;
	std::cout << "*******************************************" << std::endl;
	std::cout << std::endl;
    }
  } 
  G4cout << "Trying to process subdetector " << G4endl;
  for(unsigned int i = 0; i<fSubDetectors.size(); i++){
    
    LBNESubDetector *subDetector = fSubDetectors[i];
    //LBNEDecayPipe *subDetector = (LBNEDecayPipe*)fSubDetectors[i];
    subDetector->ConstructSubdetector();
    G4ThreeVector detPlacement;
    subDetector->FillPlacement(detPlacement);
    G4RotationMatrix detRotation;
    subDetector->FillRotation(detRotation);
    
    G4Transform3D transform(detRotation,detPlacement);
    new G4PVPlacement(transform,
                      subDetector->GetLogicalVolume(),
                      subDetector->GetPhysicalName(),
                      hallLogical,
                      false, 0,false);
  }
  //Set Vis Attributes according to solid material 
  // (only for volumes not explicitly set)
  G4LogicalVolumeStore* lvStore=G4LogicalVolumeStore::GetInstance();
  lvStore=G4LogicalVolumeStore::GetInstance();
  /*
  for (size_t ii=0;ii<(*lvStore).size();ii++){   
    if ((*lvStore)[ii]->GetVisAttributes()==0) {
      G4String matName=(*lvStore)[ii]->GetMaterial()->GetName();
      (*lvStore)[ii]->SetVisAttributes(GetMaterialVisAttrib(matName));
    }
  }
  */
  //return hallPlacement;
  return ROCK;
}

LBNEDetectorMessenger::LBNEDetectorMessenger( LBNEDetectorConstruction* LBNEDet):LBNEDetector(LBNEDet)
{

   LBNEDataInput *LBNEData = LBNEDataInput::GetLBNEDataInput();
   
   if(LBNEData->GetDebugLevel() > 0)
   {
      G4cout << "LBNEDetectorMessenger Constructor Called." << G4endl;
   }
   
   LBNEDir = new G4UIdirectory("/LBNE/");
   LBNEDir->SetGuidance("UI commands for detector geometry");
   
   detDir = new G4UIdirectory("/LBNE/det/");
   detDir->SetGuidance("detector control");
   
   
   ConstructTarget = new G4UIcmdWithABool("/LBNE/det/constructTarget",this); 
   ConstructTarget->SetGuidance("Target construction on/off"); 
   ConstructTarget->SetParameterName("constructTarget",true); 
   ConstructTarget->AvailableForStates(G4State_PreInit,G4State_Idle);
   
   SetBeamlineAngle = new
   G4UIcmdWithADoubleAndUnit("/LBNE/det/setBeamlineAngle",this);
   SetBeamlineAngle->SetGuidance("Set the angle of the beamline");
   
   //UpdateCmd = new G4UIcmdWithoutParameter("/LBNE/det/update",this);
   //UpdateCmd->SetGuidance("Update LBNE geometry.");
   //UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
   //UpdateCmd->SetGuidance("if you changed geometrical value(s).");
   //UpdateCmd->AvailableForStates(G4State_Idle);
   
   
	new G4UnitDefinition("kiloampere" , "kA", "Electric current", 1000.*ampere);
        
        

        
	
}

LBNEDetectorMessenger::~LBNEDetectorMessenger() 
{

   delete detDir;
   delete LBNEDir;
   delete ConstructTarget;
   delete SetBeamlineAngle;
   //delete UpdateCmd;
}


void LBNEDetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{

   LBNEDataInput *LBNEData = LBNEDataInput::GetLBNEDataInput();

  if(LBNEData->GetDebugLevel() > 0){
    G4cout << "LBNEDetectorMessenger::SetNewValue - Setting Parameter value from input macro." << G4endl;
   }

   
  if ( command == ConstructTarget ) {
    LBNEData->SetConstructTarget(ConstructTarget->GetNewBoolValue(newValue));
  }

  if (command == SetBeamlineAngle){
    LBNEDetector->SetBeamlineAngle(SetBeamlineAngle->GetNewDoubleValue(newValue));
  }
   /*
   if ( command == UpdateCmd ) 
   {
      LBNEDetector->UpdateGeometry();
      return;
   }
   */
}
	
	
