//---------------------------------------------------------------------------// 
// $Id: LBNEDetectorConstruction.cc,v 1.3.2.16 2013/08/22 00:10:24 lebrun Exp $
//---------------------------------------------------------------------------// 

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
#include "LBNEVolumePlacements.hh"
#include "LBNEDetectorMessenger.hh"
#include "LBNERunManager.hh"

#include "G4RunManager.hh"

#include "G4VisExtent.hh"

//---------------------------------------------------------------------------// 
// Constructor, Destructor, and Initialization
//---------------------------------------------------------------------------// 

LBNEDetectorConstruction::LBNEDetectorConstruction()
{
  fPlacementHandler = LBNEVolumePlacements::Instance(); // Minimal setup for the Placement algorithm. 
  fDetectorMessenger = new LBNEDetectorMessenger(this);
//
  // temporary.. Need a better place. 
  fBeamlineAngle = -101*mrad;

//  InitializeSubVolumes();  Obsolete. 
  InitializeMaterials();
  Initialize();
  fHasBeenConstructed = false; 
//  Construct(); Not yet!  Need to read the data card first... 
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

  DestroyMaterials();

  //for(size_t i = 0; i< fSubVolumes.size(); i++){
  //  delete fSubVolumes[i];
  //}

  delete fDetectorMessenger;
//  delete fPlacementHandler; A static struct now, no point deleting it 
}

 // Obsolete.
void LBNEDetectorConstruction::InitializeSubVolumes()
{
//  fDecayPipe = new LBNEDecayPipe("DecayPipe");
// fHadronAbsorber = new LBNEHadronAbsorber("HadronAbsorber");
//  fStandardPerson = new LBNEStandardPerson("StandardPerson");
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
 // fSubVolumes.clear();
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

void LBNEDetectorConstruction::InitializeMaterials() {

  G4Element* elH  = new G4Element("Hydrogen","H" , 1., 1.01*g/mole);
  G4Element* elHe = new G4Element("Helium","He" , 2., 4.003*g/mole);
  G4Element* elC  = new G4Element("Carbon","C" , 6., 12.01*g/mole);
  G4Element* elN  = new G4Element("Nitrogen","N" , 7., 14.01*g/mole);
  G4Element* elO  = new G4Element("Oxygen"  ,"O" , 8., 16.00*g/mole); 
  G4Element* elNa = new G4Element("Sodium"  ,"Na" , 11., 22.99*g/mole); 
  G4Element* elMg = new G4Element("Magnesium"  ,"Mg" , 12., 24.305*g/mole); 
  G4Element* elAl = new G4Element("Aluminum"  ,"Al" , 13., 26.98*g/mole); 
  G4Element* elSi = new G4Element("Silicon"  ,"Si" , 14., 28.09*g/mole); 
  G4Element* elP  = new G4Element("Phosphorus"  ,"P" , 15., 30.974*g/mole); 
  G4Element* elS  = new G4Element("Sulfur"  ,"S" , 16., 32.065*g/mole); 
  G4Element* elK  = new G4Element("Potassium"  ,"K" , 19., 39.1*g/mole); 
  G4Element* elCa = new G4Element("Calcium"  ,"Ca" , 20., 40.09*g/mole); 
  G4Element* elTi = new G4Element("Titanium"  ,"Ti" , 22., 47.867*g/mole); 
  G4Element* elCr = new G4Element("Chromium"  ,"Cr" , 24., 51.9961*g/mole); 
  G4Element* elMn = new G4Element("Manganese"  ,"Mn" , 25., 54.938*g/mole); 
  G4Element* elFe = new G4Element("Iron"  ,"Fe" , 26., 55.85*g/mole); 
  G4Element* elNi = new G4Element("Nickel"  ,"Ni" , 28., 58.6934*g/mole); 
  G4Element* elCu = new G4Element("Copper"  ,"Cu" , 29., 63.546*g/mole); 
  G4Element* elHg = new G4Element("Mercury"  ,"Hg" , 80., 200.59*g/mole); 
  G4Element* elMo = new G4Element("Molybdenum"  ,"Mo" , 42., 95.96*g/mole); 


  Air = new G4Material("Air"  , 1.290*mg/cm3, 2);
  Air->AddElement(elN, 0.7);
  Air->AddElement(elO, 0.3);
  
  CT852 = new G4Material("CT852", 7.75*g/cm3, 10); 
  CT852->AddElement(elC,  0.001); 
  CT852->AddElement(elSi, 0.008); 
  CT852->AddElement(elMn, 0.008); 
  CT852->AddElement(elCr, 0.13); 
  CT852->AddElement(elS,  0.00025); 
  CT852->AddElement(elP,  0.0003); 
  CT852->AddElement(elTi, 0.002); 
  CT852->AddElement(elCu, 0.003); 
  CT852->AddElement(elNi, 0.006); 
  CT852->AddElement(elFe, 0.84145); 

  Steel316 = new G4Material("Steel316", 8.0*g/cm3, 9); 
  // Reference: Google search, found Anderson Schumake company.  
  Steel316->AddElement(elC,  0.015); 
  Steel316->AddElement(elSi, 0.005); 
  Steel316->AddElement(elMn, 0.008); 
  Steel316->AddElement(elMo, 0.01); 
  Steel316->AddElement(elCr, 0.17); 
  Steel316->AddElement(elS,  0.00015); 
  Steel316->AddElement(elP,  0.0003); 
  Steel316->AddElement(elNi, 0.12); 
  Steel316->AddElement(elFe, 0.6716); 
  
  Titanium = new G4Material("Titanium", 22, 47.867*g/mole, 4.506*g/cm3);
  
 // ASTM836 steel
  Slab_Stl = new G4Material("Slab_Stl", 7.8416*g/cm3, 6);
  Slab_Stl->AddElement(elC,  0.001);
  Slab_Stl->AddElement(elSi, 0.001);
  Slab_Stl->AddElement(elMn, 0.004);
  Slab_Stl->AddElement(elFe, 0.982);
  Slab_Stl->AddElement(elNi, 0.01);
  Slab_Stl->AddElement(elCu, 0.002);

  // BluBlock Steel
  Blu_Stl = new G4Material("Blu_Stl", 7.25*g/cm3, 1);
  Blu_Stl->AddElement(elFe, 1.0);

  Water = new G4Material("Water", 1.0*g/cm3, 2);
  Water->AddElement(elH, 2);
  Water->AddElement(elO, 1);

  Vacuum = new G4Material("Vacuum", 2.376e-15*g/cm3,1,kStateGas,300.*kelvin,2.0e-7*bar);
  Vacuum->AddMaterial(Air, 1.);

  G4Material* Helium = new G4Material("Helium", 2, 4.0026*g/mole, 0.1785*kg/m3, kStateGas,
                          300*kelvin, 2.55*atmosphere);
  G4Material* HeliumTarget = new G4Material("HeliumTarget", 2, 4.0026*g/mole, 1.7436*0.1785*kg/m3, kStateGas,
                          350*kelvin, 1.36*atmosphere); //20 psi.  The factor of 1.7436 assume perfect gas. 
			  // density proportional to temperature and pression.  

  G4Material* Aluminum = new G4Material("Aluminum", 13, 26.98*g/mole, 2.7*g/cm3);
  G4Material* Argon = new G4Material("Argon", 18, 39.948*g/mole, 1.784*kg/m3, kStateGas,
                          300*kelvin, atmosphere);
  G4Material* Lead = new G4Material("Lead", 82, 207.19*g/mole, 11.35*g/cm3);
  G4Material* Iron = new G4Material("Iron", 26, 55.85*g/mole, 7.86999*g/cm3);

  Concrete = new G4Material("Concrete", 2.03*g/cm3, 10);
  Concrete->AddElement( elH,  0.01);
  Concrete->AddElement( elO,  0.529);
  Concrete->AddElement( elNa, 0.016);
  Concrete->AddElement( elHg, 0.002);
  Concrete->AddElement( elAl, 0.034);
  Concrete->AddElement( elSi, 0.337);
  Concrete->AddElement( elK,  0.013);
  Concrete->AddElement( elCa, 0.044);
  Concrete->AddElement( elFe, 0.014);
  Concrete->AddElement( elC,  0.001);
  
  G4Material *rockMat = new G4Material( "rockMat", 2.78*g/cm3, 4 ); //CaMg(CO3)2
  rockMat->AddElement( elCa, 1);
  rockMat->AddElement( elMg, 1); 
  rockMat->AddElement( elC,  2); 
  rockMat->AddElement( elO,  6); 

  G4Material *graphiteBaffle = new G4Material( "GraphiteBaffle", 1.78*g/cm3, 3 ); //Carbon, air (Nitrogen and oxigen) 
  graphiteBaffle->AddElement( elC,  0.99); // 
  graphiteBaffle->AddElement( elN,  0.007); //  
  graphiteBaffle->AddElement( elO,  0.003); // 

  // In case they are different.. 

  G4Material *Target = new G4Material( "Target", 1.78*g/cm3, 3 ); //Carbon, air (Nitrogen and oxigen),
                                                                  // Assume density of POCO ZXF-5Q  
  Target->AddElement( elC,  0.99); 
  Target->AddElement( elN,  0.007); 
  Target->AddElement( elO,  0.003);
  G4Material *Beryllium = new G4Material("Beryllium", 4, 9.0122*g/mole, 1.85*g/cm3); 
  
}


G4VisAttributes* LBNEDetectorConstruction::GetMaterialVisAttrib(G4String mName){
  G4VisAttributes* visAtt;
  if(mName == "Vacuum")  visAtt = new G4VisAttributes(false);
  if(mName=="Aluminum") visAtt = new G4VisAttributes(G4Color(0.2, 0.8, 1));
  if(mName=="Air") visAtt = new G4VisAttributes(G4Color(0.6,0.7,0.8));
  if(mName=="Iron" || mName=="Slab_Stl") visAtt=new G4VisAttributes(G4Color(0.5,0.3,0));
  if(mName=="Concrete") visAtt = new G4VisAttributes(G4Color(0.75,0.85,0.95));
  if(!visAtt) visAtt = new G4VisAttributes(G4Color(1,0,0));
  return visAtt;
}

void LBNEDetectorConstruction::DestroyMaterials()
{
  // Destroy all allocated elements and materials
  size_t i;
  G4MaterialTable* matTable = (G4MaterialTable*)G4Material::GetMaterialTable();
  for(i=0;i<matTable->size();i++)
  { delete (*(matTable))[i]; }
  matTable->clear();
  G4ElementTable* elemTable = (G4ElementTable*)G4Element::GetElementTable();
  for(i=0;i<elemTable->size();i++)
  { delete (*(elemTable))[i]; }
  elemTable->clear();
}

G4VPhysicalVolume* LBNEDetectorConstruction::Construct() {


  if (fHasBeenConstructed) {
     std::cerr << " WARNING: LBNEDetectorConstruction::Construct, already done, skip " << std::endl;
     return fRock;
  }
  std::cout << " LBNEDetectorConstruction::Construct, Start !!! " << std::endl;
  std::cerr << " LBNEDetectorConstruction::Construct, Start !!! " << std::endl;
  
  double decayPipeLength = fPlacementHandler->GetDecayPipeLength();  
  fRockX = 60.0*m;
  fRockY = 60.0*m;
  fRockLength = decayPipeLength + 50.*m; // Approximate and irrelevant, all rock anyways.
  fPlacementHandler->SetTotalLength(fRockLength);    
  G4Box* ROCK_solid = new G4Box("ROCK_solid",fRockX/2, fRockY/2, fRockLength/2);
  G4LogicalVolume *RockLogical = 
            new G4LogicalVolume(ROCK_solid,
                                G4Material::GetMaterial("Concrete"),
                                "RockLogical",0,0,0); 
  fPlacementHandler->Initialize(RockLogical); // sort of a noop for now..				
  //RockLogical->SetVisAttributes(G4VisAttributes::Invisible);
  fRock = new G4PVPlacement(0,G4ThreeVector(),RockLogical,"ROCK",0,false,0);
  
  // First create the Target Hall, Pipe Hall, and Absorber Hall, and then
  // connect them together.
  fPlacementHandler->SetTotalLengthOfRock(fRockLength);
  fPlacementHandler->Create(G4String("Tunnel")); // Material is rock oversized.. 
  G4VPhysicalVolume* tunnel = fPlacementHandler->PlaceFinal(G4String("Tunnel"), fRock ); // like Rock, oversized. Air. 
  fPlacementHandler->Create(G4String("TargetHallAndHorn1"));
// 
// Before placing the container volume for the target region + horn1, define these two volumes, 
// as these two are adjacent. The boundary is "coordinate zero.", respecting older convention. 
//
  LBNEVolumePlacementData *plDat = fPlacementHandler->Create(G4String("UpstreamTargetAssembly"));
  std::cerr << " Placement data for volume UpstreamTargetAssembly, half length  " << plDat->fParams[2]/2. << std::endl;
//   
  fPlacementHandler->Create(G4String("Horn1Hall"));
  G4VPhysicalVolume* targethorn1Phys = fPlacementHandler->PlaceFinal(G4String("TargetHallAndHorn1"), tunnel);
//
//   
  std::cerr << " LBNEDetectorConstruction::Construct, about to place-final UsptreamTargetAssembly" << std::endl;
  std::cerr << " Dump of the volume placements so far " << std::endl;
//  fPlacementHandler->PrintAll();
  
  G4PVPlacement* upstreamTargetAssPhys = 
    fPlacementHandler->PlaceFinal(G4String("UpstreamTargetAssembly"), targethorn1Phys); 
//
  fPlacementHandler->PlaceFinalUpstrTarget(upstreamTargetAssPhys);
  
  fPlacementHandler->PlaceFinalDownstrTarget((G4PVPlacement*) targethorn1Phys);

  
  // Just test random error in positioning along the Z axis
//  fPlacementHandler->TestVolumeOverlap(G4String("Horn1Hall"), targethorn1Phys);
  
  fPlacementHandler->PlaceFinal(G4String("Horn1Hall"), targethorn1Phys); 
  

  fPlacementHandler->Create(G4String("Horn2Hall"));
//  fPlacementHandler->PlaceFinal(G4String("Horn2Hall"), tunnel); // to be done... 
  
  // We now will do the details for the traget and Horn1 
  
   fPlacementHandler->Create(G4String("Baffle"));
// This will be a surveyed elements, but let us skip this step for now.    
   fPlacementHandler->PlaceFinal(G4String("Baffle"), upstreamTargetAssPhys);
   
//   std::cerr << " And quit for now .. " << std::endl; exit(2);
/*

  // ???????????????????? Everything downstream of this need to be adapted. 					     
  
  G4double eps = 1e-7*m; // 0.1 micron


  fDecayPipeLength = fDecayPipe->GetDecayPipeLength();
  fDecayPipeRadius = fDecayPipe->GetDecayPipeRadius();
  fDecayHallZ = fDecayPipeLength;
  fPlacementHandler->SetDecayHallZ(fDecayPipeLenth);
  fAbsorberHallX = 10*m;
  fAbsorberHallY = 22*m;
  fAbsorberHallZ = 14*m;
  
  G4Box *TargetHallSolid = new G4Box("TargetHallSolid", 
                                      fTargetHallX/2,
                                      fTargetHallY/2,
                                      fTargetHallZ/2);

  G4Tubs *DecayHallPreSolid = new G4Tubs("DecayHallPreSolid", 0,fDecayPipeRadius+eps,
                                      fDecayPipeLength/2+5*m, 0*deg, 360*deg);

  G4Box *AbsorberHallSolid = new G4Box("AbsorberHallSolid",
                                        fAbsorberHallX/2,
                                        fAbsorberHallY/2,
                                        fAbsorberHallZ/2);
  
  G4RotationMatrix AbsorberRotation;
  AbsorberRotation.rotateX(fBeamlineAngle);
  G4Transform3D DecayHallXform( AbsorberRotation,
                                G4ThreeVector(0,0,fDecayHallZ/2 +
                                fAbsorberHallZ/2));
  G4SubtractionSolid *DecayHallSolid = new G4SubtractionSolid("DecayHallSolid",
  DecayHallPreSolid, AbsorberHallSolid, DecayHallXform);
  G4ThreeVector targetHallPosition(0,0,-fDecayHallZ/2 - fTargetHallZ/2-eps);
  G4ThreeVector decayHallPosition(0,0,5*m);
  G4double offset = 6*m;
  G4ThreeVector
  absorberHallPosition(0,offset*cos(fBeamlineAngle),fDecayHallZ/2 +
  fAbsorberHallZ/2+eps+5*m + offset*sin(fBeamlineAngle));
  
  G4ThreeVector
  AbsorberTranslation(0,0,fAbsorberHallZ/2+fDecayHallZ+fTargetHallZ/2);
  G4cout << "Rotating beamline by " << fBeamlineAngle/mrad << " mrad" << G4endl;
  G4Transform3D transformAbsorber(AbsorberRotation,absorberHallPosition);
  
  G4LogicalVolume *targetHallLogical = 
      new G4LogicalVolume(TargetHallSolid, G4Material::GetMaterial("Air"), 
                          "targetHallLogical", 0,0,0);

  G4LogicalVolume *decayHallLogical = 
      new G4LogicalVolume(DecayHallSolid, G4Material::GetMaterial("Air"), 
                          "decayHallLogical", 0,0,0);
  
  G4LogicalVolume *absorberHallLogical = 
      new G4LogicalVolume(AbsorberHallSolid, G4Material::GetMaterial("Air"), 
                          "absorberHallLogical", 0,0,0);
  
  // Register the mother hall logical volumes with the placement handler
  fPlacementHandler->SetTargetHallLogical(targetHallLogical);
  fPlacementHandler->SetDecayHallLogical(targetHallLogical);
  fPlacementHandler->SetAbsorberHallLogical(targetHallLogical);
  
  
  G4PVPlacement* targetHallPhysical = 
      new G4PVPlacement(0, targetHallPosition, targetHallLogical,
                        "targetHallPlacement", RockLogical, false, 0);

  G4PVPlacement* decayHallPhysical = 
      new G4PVPlacement(0, decayHallPosition, decayHallLogical,
                        "decayHallPlacement", RockLogical, false, 0);

  G4PVPlacement* absorberHallPhysical = 
      new G4PVPlacement(transformAbsorber, absorberHallLogical,
                        "absorberHallPlacement", RockLogical, false, 0);

                             

  // Now to calculate useful positions within the hall
  G4double hallLength = fTargetHallZ + fDecayHallZ + fAbsorberHallZ;
  G4cout << " Total hall length is " << hallLength/m << " m long" << G4endl;
  G4double decayPipePosition = 0.5*fTargetHallZ + fDecayHallZ/2;
  fHadronAbsorber->SetRotation(0,-fBeamlineAngle, 0);
  //fHadronAbsorber->SetPlacement(0,0,0.5*(fTargetHallZ+2*fDecayHallZ+fAbsorberHallZ));
  //fStandardPerson->SetPlacement(0,
  //                              0.9*m-fAbsorberHallY/2,
   //                             0.5*fTargetHallZ + fDecayHallZ + 0.5*fAbsorberHallZ + 5*m);
  fStandardPerson->SetRotation(0,-fBeamlineAngle+90*deg,0);
    // Individual geometries within all halls:
    // baffle
    // target
    // horn 1
    // horn 2
    // decay pipe (with windows)
    // absorber

  // is baffle necessary????

  //fSubVolumes.push_back(fTarget);

  if( fSimulationType == "Standard Neutrino Beam" ||
      fSimulationType == "Horn 1 Tracking" ||
      fSimulationType == "Horn 2 Tracking") {
    // Then we need everything!
    //fSubVolumes.push_back(fBaffle);
    //fSubVolumes.push_back(fHornAssembly);
    //fSubVolumes.push_back(fDecayPipe);
    //fSubVolumes.push_back(fHadronAbsorber);
    fSubVolumes.push_back(fStandardPerson);
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
  G4cout << "Trying to process subvolume " << G4endl;
  for(unsigned int i = 0; i<fSubVolumes.size(); i++){
    
    LBNESubVolume *subVolume = fSubVolumes[i];
    subVolume->ConstructSubvolume();
    G4ThreeVector detPlacement;
    subVolume->FillPlacement(detPlacement);
    G4RotationMatrix detRotation;
    subVolume->FillRotation(detRotation);
    
    G4Transform3D transform(detRotation,detPlacement);
    new G4PVPlacement(transform,
                      subVolume->GetLogicalVolume(),
                      subVolume->GetPhysicalName(),
                      decayHallLogical,
                      false, 0,false);
  }
  //Set Vis Attributes according to solid material 
  // (only for volumes not explicitly set)
  G4LogicalVolumeStore* lvStore=G4LogicalVolumeStore::GetInstance();
  lvStore=G4LogicalVolumeStore::GetInstance();
  for (size_t ii=0;ii<(*lvStore).size();ii++){   
    if ((*lvStore)[ii]->GetVisAttributes()==0) {
      G4String mName=(*lvStore)[ii]->GetMaterial()->GetName();
      (*lvStore)[ii]->SetVisAttributes(GetMaterialVisAttrib(mName));
    }
  }
  */
  //
  // Invoking here the G4RunManager Initialize method. 
  //
  
  fHasBeenConstructed = true;
  
//  LBNERunManager *aRunManager=(LBNERunManager*)LBNERunManager::GetRunManager();
//  aRunManager->Initialize();
  
  return fRock;
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
   ConstructTarget->SetParameterName("constructTarget", false);
   ConstructTarget->SetGuidance("Target construction on/off"); 
   ConstructTarget->SetParameterName("constructTarget",true); 
   ConstructTarget->AvailableForStates(G4State_PreInit,G4State_Idle);
   
   SetBeamlineAngle = new
   G4UIcmdWithADoubleAndUnit("/LBNE/det/setBeamlineAngle",this);
   SetBeamlineAngle->SetParameterName("angle", false);
   SetBeamlineAngle->SetGuidance("Set the angle of the beamline");
   
   UpdateCmd = new G4UIcmdWithoutParameter("/LBNE/det/update",this);
   UpdateCmd->SetGuidance("Update or Construct LBNE geometry. Same difference ");
   UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
   UpdateCmd->SetGuidance("if you changed geometrical value(s).");
   UpdateCmd->AvailableForStates(G4State_PreInit);
   
   ConstructCmd = new G4UIcmdWithoutParameter("/LBNE/det/construct",this);
   ConstructCmd->SetGuidance("Construct LBNE geometry. Should be one and only time ");
   ConstructCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
   ConstructCmd->SetGuidance("if you changed geometrical value(s).");
   ConstructCmd->AvailableForStates(G4State_PreInit);
  
   new G4UnitDefinition("kiloampere" , "kA", "Electric current", 1000.*ampere);
        
        

        
	
}

LBNEDetectorMessenger::~LBNEDetectorMessenger() 
{

   delete detDir;
   delete LBNEDir;
   delete ConstructTarget;
   delete SetBeamlineAngle;
   delete UpdateCmd;
   delete ConstructCmd;
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
  
  if ( command == UpdateCmd ) 
   {
      LBNEDetector->Construct();
      return;
   }
  if ( command == ConstructCmd ) 
   {
      LBNEDetector->Construct();
      return;
   }
}
	
	
