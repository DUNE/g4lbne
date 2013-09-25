//---------------------------------------------------------------------------// 
// $Id: LBNEDetectorConstruction.cc,v 1.3.2.29 2013/09/25 22:58:30 lebrun Exp $
//---------------------------------------------------------------------------// 

#include <fstream>

#include "LBNEDetectorConstruction.hh"

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
#include "G4GDMLParser.hh"

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
  // Used only in placing the absorber.. 
  fBeamlineAngle = -101*mrad;

//  InitializeSubVolumes();  Obsolete. 
  InitializeMaterials();
  Initialize();
  fHasBeenConstructed = false; 
//  Construct(); Not yet!  Need to read the data card first... 
  fHornCurrent = 200.*ampere*1000; // in kA, defined via Detector GUImessenger if need be

}


LBNEDetectorConstruction::~LBNEDetectorConstruction()
{

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
  
}


//-------------------------------------------------------------------------

void LBNEDetectorConstruction::InitializeMaterials() {

  G4Element* elH  = new G4Element("Hydrogen","H" , 1., 1.01*g/mole);
  new G4Element("Helium","He" , 2., 4.003*g/mole);
  elC  = new G4Element("Carbon","C" , 6., 12.01*g/mole);
  elN  = new G4Element("Nitrogen","N" , 7., 14.01*g/mole);
  elO  = new G4Element("Oxygen"  ,"O" , 8., 16.00*g/mole); 
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

  new G4Material("Helium", 2, 4.0026*g/mole, 2.55*0.1785*kg/m3, kStateGas,
                          300*kelvin, 2.55*atmosphere); // to fill the canister.For the decay pipe, see below. 
			  
  new G4Material("HeliumTarget", 2, 4.0026*g/mole, 1.7436*0.1785*kg/m3, kStateGas,
                          350*kelvin, 1.36*atmosphere); //20 psi.  The factor of 1.7436 assume perfect gas. 
			  // density proportional to temperature and pression.  

  new G4Material("Aluminum", 13, 26.98*g/mole, 2.7*g/cm3);
  new G4Material("Argon", 18, 39.948*g/mole, 1.784*kg/m3, kStateGas,
                          300*kelvin, atmosphere);
  new G4Material("Lead", 82, 207.19*g/mole, 11.35*g/cm3);
  new G4Material("Iron", 26, 55.85*g/mole, 7.86999*g/cm3);

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
  
  new G4Material("Beryllium", 4, 9.0122*g/mole, 1.85*g/cm3); 
  
  G4Material *Mylar = new G4Material("Mylar", 1.397*g/cm3, 3);
  Mylar->AddElement(elC, 10);
  Mylar->AddElement(elH, 8);
  Mylar->AddElement(elO, 4);

}
//
// Declare the material for the target after the data cards have been read. 
// 
void LBNEDetectorConstruction::InitializeMaterialsPostPreIdle() {
  // In case they are different.. 
  G4String aNameT(fPlacementHandler->GetTargetMaterialName());
  if ((aNameT == G4String("Graphite")) || (aNameT == G4String("graphite")) ||
      (aNameT == G4String("Carbon")) || (aNameT == G4String("carbon"))) { 
  
    G4Material *Target = new G4Material( "Target", 
             fPlacementHandler->GetTargetDensity(), 3 ); //Carbon, air (Nitrogen and oxigen),
                                                                  // Assume density of POCO ZXF-5Q  
    Target->AddElement( elC,  0.99); 
    Target->AddElement( elN,  0.007); 
    Target->AddElement( elO,  0.003);
  } else if ((aNameT == G4String("Beryllium")) 
               || (aNameT == G4String("beryllium"))) {
     new G4Material("Target", 4, 9.0122*g/mole, 1.85*g/cm3); 
  } else if ((aNameT == G4String("Aluminium")) 
            || (aNameT == G4String("aluminium"))) {   
     new G4Material("Target", 13, 26.98*g/mole, 2.7*g/cm3);
  } else {
    G4String mess(" Non-standard material for the target: "); 
    mess += aNameT + G4String (" .  \n");
    mess += G4String("... Please upgrade the code after consultation with mechanical engineers\n.");
    G4Exception("LBNEDetectorConstruction::InitializeMaterialsPostPreIdle", 
                " ",  FatalErrorInArgument, mess.c_str());
  }
  
  G4String aNameG(fPlacementHandler->GetDecayPipeGas());
  if ((aNameG == G4String("Air")) || (aNameG == G4String("air"))) {
    G4Material *gas = new G4Material("DecayPipeGas" , 1.290*mg/cm3, 2);
    gas->AddElement(elN, 0.7);
    gas->AddElement(elO, 0.3);
  } else if ((aNameG == G4String("Helium")) || (aNameG == G4String("helium"))) {
      new G4Material("DecayPipeGas", 2, 4.0026*g/mole, 0.1785*kg/m3, kStateGas,
                          300*kelvin, 1.0*atmosphere);
  }  else {
    G4String mess(" Non-standard gas  : "); 
    mess += aNameG + G4String (" in the decay pipe .  \n");
    mess += G4String("... Please upgrade the code after consultation with mechanical engineers\n. ");
    G4Exception("LBNEDetectorConstruction::InitializeMaterialsPostPreIdle", 
                " ",  FatalErrorInArgument, mess.c_str());
  }
  
}
G4VisAttributes* LBNEDetectorConstruction::GetMaterialVisAttrib(G4String mName){
  G4VisAttributes* visAtt;
  if(mName == "Vacuum")  visAtt = new G4VisAttributes(false);
  else if(mName=="Aluminum") visAtt = new G4VisAttributes(G4Color(0.2, 0.8, 1));
  else if(mName=="Air") visAtt = new G4VisAttributes(G4Color(0.6,0.7,0.8));
  else if(mName=="Iron" || mName=="Slab_Stl") visAtt=new G4VisAttributes(G4Color(0.5,0.3,0));
  else if(mName=="Concrete") visAtt = new G4VisAttributes(G4Color(0.75,0.85,0.95));
  else visAtt = new G4VisAttributes(G4Color(1,0,0));
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
  
  this->InitializeMaterialsPostPreIdle(); 
  
  std::cout << " LBNEDetectorConstruction::Construct, Start !!! " << std::endl;
  std::cerr << " LBNEDetectorConstruction::Construct, Start !!! " << std::endl;
  
  fRockX = 60.0*m;
  fRockY = 60.0*m;
  fRockLength = 2.0*(fPlacementHandler->GetDecayPipeLength() + 50.*m); // Approximate and irrelevant, all rock anyways.
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
  LBNEVolumePlacementData *plDatUTA = fPlacementHandler->Create(G4String("UpstreamTargetAssembly"));
  std::cerr << " Placement data for volume UpstreamTargetAssembly, half length  " << plDatUTA->fParams[2]/2. << std::endl;
 
//   
  LBNEVolumePlacementData *plH1Dat = fPlacementHandler->Create(G4String("Horn1Hall"));
  
  G4VPhysicalVolume* targethorn1Phys = fPlacementHandler->PlaceFinal(G4String("TargetHallAndHorn1"), tunnel);
//
  
  G4PVPlacement* upstreamTargetAssPhys = 
    fPlacementHandler->PlaceFinal(G4String("UpstreamTargetAssembly"), targethorn1Phys); 
//
  fPlacementHandler->PlaceFinalUpstrTarget(upstreamTargetAssPhys);
    
  G4PVPlacement *vHorn1 = fPlacementHandler->PlaceFinal(G4String("Horn1Hall"), targethorn1Phys); 
//
// We will place the downstream part of the target in a container volume 
//
   fPlacementHandler->CreateHorn1TopLevelUpstr();
   G4PVPlacement *vUpstr = fPlacementHandler->PlaceFinal("Horn1TopLevelUpstr", vHorn1);

  fPlacementHandler->PlaceFinalDownstrTarget((G4PVPlacement*) vHorn1);

  fPlacementHandler->PlaceFinalHorn1((G4PVPlacement*) vHorn1, vUpstr);  

  LBNEVolumePlacementData *plH2Dat = fPlacementHandler->Create(G4String("Horn2Hall"));
  
  G4PVPlacement *vHorn2 = fPlacementHandler->PlaceFinal(G4String("Horn2Hall"), tunnel); 

  fPlacementHandler->PlaceFinalHorn2(vHorn2);
  
  // We now turn on the magentic fields 
  
  LBNEMagneticFieldHorn *fieldHorn1 = new LBNEMagneticFieldHorn(true);
  fieldHorn1->SetHornCurrent(fHornCurrent);
  G4FieldManager* aFieldMgr = new G4FieldManager(fieldHorn1); //create a local field		 
  aFieldMgr->SetDetectorField(fieldHorn1); //set the field 
  aFieldMgr->CreateChordFinder(fieldHorn1); //create the objects which calculate the trajectory
  const LBNEVolumePlacementData *plH1TopDat = 
    fPlacementHandler->Find("FieldHorn1", "Horn1TopLevelUpstr", "DetectorConstruction");
  plH1Dat->fCurrent->SetFieldManager(aFieldMgr,true); //attach the local field to logical volume
  plH1TopDat->fCurrent->SetFieldManager(aFieldMgr,true); // The upstream section of the horn as well 

  LBNEMagneticFieldHorn *fieldHorn2 = new LBNEMagneticFieldHorn(false);
  fieldHorn2->SetHornCurrent(fHornCurrent);
  G4FieldManager* aFieldMgr2 = new G4FieldManager(fieldHorn2); //create a local field		 
  aFieldMgr2->SetDetectorField(fieldHorn2); //set the field 
  aFieldMgr2->CreateChordFinder(fieldHorn2); //create the objects which calculate the trajectory
  plH2Dat->fCurrent->SetFieldManager(aFieldMgr2,true); //attach the local field to logical volume


// we forgot the baffle. 
  
   fPlacementHandler->Create(G4String("Baffle"));
// This will be a surveyed elements, but let us skip this step for now.    
   fPlacementHandler->PlaceFinal(G4String("Baffle"), upstreamTargetAssPhys);
//
// Place the decay Pipe Snout, which contains the window, in case we have Helium gas. 
//
   fPlacementHandler->Create(G4String("DecayPipeSnout")); // Now in Snout region 
   fPlacementHandler->PlaceFinalDecayPipeSnout((G4PVPlacement*) tunnel);
//   
// Place the decay pipe 
//   
   fPlacementHandler->Create(G4String("DecayPipeHall"));
   G4PVPlacement *vDecayPipe = fPlacementHandler->PlaceFinal(G4String("DecayPipeHall"), tunnel);
   fPlacementHandler->Create(G4String("DecayPipeConcrete"));
   fPlacementHandler->Create(G4String("DecayPipeOuterWall"));
   fPlacementHandler->Create(G4String("DecayPipeWall"));
   fPlacementHandler->Create(G4String("DecayPipeVolume"));
//   fPlacementHandler->Create(G4String("DecayPipeUpstrWindow")); // Now in Snout region 
   
   fPlacementHandler->PlaceFinal(G4String("DecayPipeConcrete"), vDecayPipe);
   fPlacementHandler->PlaceFinal(G4String("DecayPipeOuterWall"), vDecayPipe);
   fPlacementHandler->PlaceFinal(G4String("DecayPipeWall"), vDecayPipe);
   fPlacementHandler->PlaceFinal(G4String("DecayPipeVolume"), vDecayPipe);
   
   this->ConstructLBNEHadronAbsorber(tunnel);
   
  fHasBeenConstructed = true;
  
//  LBNERunManager *aRunManager=(LBNERunManager*)LBNERunManager::GetRunManager();
//  aRunManager->Initialize();
  
  return fRock;
}
void LBNEDetectorConstruction::ConstructLBNEHadronAbsorber(G4VPhysicalVolume *mother)
{

   const G4double in = 25.4*mm;

   G4cout << "Importing hadron absorber gdml file... " << G4endl;
   
   G4String filename(fPlacementHandler->GetAbsorberGDMLFilename());
   std::ifstream gdmlfile(filename.c_str());
   if (!gdmlfile.is_open()) {
      std::string mess(" AbsorberGDML file "); 
      mess += filename + G4String(" could not be found \n");
      G4Exception("LBNEDetectorConstruction::ConstructLBNEHadronAbsorber", " ", 
                    FatalErrorInArgument, mess.c_str());
      return; // perfunctory. 
      
    } else {
     gdmlfile.close();
    }
    const LBNEVolumePlacementData *plTunnel = 
      fPlacementHandler->Find("HadronAbsorber", "Tunnel", "ConstructLBNEHadronAbsorber");
    const LBNEVolumePlacementData *plDecayPipe = 
      fPlacementHandler->Find("HadronAbsorber", "DecayPipeHall", "ConstructLBNEHadronAbsorber");
   
     G4double CShld_length = 72*in;
     G4double zLocAbsorber = plDecayPipe->fPosition[2] + plDecayPipe->fParams[2]/2. + CShld_length + 1.0*m;
     std::cerr << "LBNEDetectorConstruction::ConstructLBNEHadronAbsorber zLocAbsorber " << zLocAbsorber << std::endl
               << " .... tunnel length " << plTunnel->fParams[2] << " size , X/Y "<<
	       plTunnel->fParams[0] << " / " << plTunnel->fParams[1] << std::endl;
     G4RotationMatrix *zrot=new G4RotationMatrix();
    
     G4double xo =  plDecayPipe->fPosition[2] + plDecayPipe->fParams[2]/2. + 10.25*12*in;
     G4double xp = (-6.0625*12*in - CShld_length/2.0);
     G4double yp = 0;
     G4double yo = -1*in; 
     G4double z_shld = xp*cos(fBeamlineAngle) - yp*sin(fBeamlineAngle) + xo;
     G4double y_shld = xp*sin(fBeamlineAngle) + yp*cos(fBeamlineAngle) + yo;
      
     G4ThreeVector tunnelPos = G4ThreeVector(0,0, zLocAbsorber);
     G4ThreeVector shldpos = G4ThreeVector(0, y_shld, z_shld); // to be checked 
     std::cerr << " shldpos " << shldpos << std::endl;
     G4GDMLParser parser;
     parser.Read( filename );
     G4LogicalVolume *aConcShld = parser.GetVolume( "Conc_SH" ); 
     G4LogicalVolume *aAHTop = parser.GetVolume( "AH_top" ); 
     G4LogicalVolume *aAHBack = parser.GetVolume( "AH_back" ); 
     G4LogicalVolume *aMuonAlk = parser.GetVolume( "AH_Muon_alk" ); 
     
     G4ThreeVector conc = parser.GetPosition( "Conc_SH_1inTOPpos" )+shldpos;	     
     G4ThreeVector top = parser.GetPosition( "AH_top_1inTOPpos" ) +shldpos;	    
     G4ThreeVector back = parser.GetPosition( "AH_back_1inTOPpos" )+shldpos;	     
     G4ThreeVector muon = parser.GetPosition( "AH_Muon_alk_1inTOPpos" )+shldpos;	 

     // rotate about beamline z-axis, zrot rotates about z-axis relative to local object 
     G4ThreeVector concRotZ( -conc.y(), conc.x(), conc.z() );
     G4ThreeVector topRotZ( -top.y(), top.x(), top.z() );
     G4ThreeVector backRotZ( -back.y(), back.x(), back.z() );
     G4ThreeVector muonRotZ( -muon.y(), muon.x(), muon.z() );

     new G4PVPlacement(zrot, concRotZ, "Conc_SH", aConcShld, mother, false, 0, true);
     new G4PVPlacement(zrot, topRotZ, "AH_top", aAHTop, mother, false, 0, true);
     new G4PVPlacement(zrot, backRotZ, "AH_back", aAHBack, mother, false, 0, true);
     new G4PVPlacement(zrot, muonRotZ, "AH_Muon_alk", aMuonAlk, mother, false, 0, true);

}

LBNEDetectorMessenger::LBNEDetectorMessenger( LBNEDetectorConstruction* LBNEDet):LBNEDetector(LBNEDet)
{
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
   SetHornCurrent = new
     G4UIcmdWithADoubleAndUnit("/LBNE/det/seHornCurrent",this);
   SetHornCurrent->SetParameterName("Horn Current", false);
   SetHornCurrent->SetGuidance(" The current for the horn system. (Horn1 and Horn2 are in series.");
   SetHornCurrent ->SetDefaultValue(200.0); // CDR, March 2012
   SetHornCurrent->SetDefaultUnit("kA");
   SetHornCurrent->SetUnitCandidates("kA");
   SetHornCurrent->AvailableForStates(G4State_PreInit);
	
}

LBNEDetectorMessenger::~LBNEDetectorMessenger() 
{

   delete detDir;
   delete LBNEDir;
   delete ConstructTarget;
   delete SetBeamlineAngle;
   delete UpdateCmd;
   delete ConstructCmd;
   delete SetHornCurrent;
}


void LBNEDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{

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
   if (command == SetHornCurrent) {
     G4UIcmdWithADoubleAndUnit* cmdWD = dynamic_cast<G4UIcmdWithADoubleAndUnit*> (command);
     LBNEDetector->SetHornCurrent(cmdWD->GetNewDoubleValue(newValue));
   }
}
	
	
