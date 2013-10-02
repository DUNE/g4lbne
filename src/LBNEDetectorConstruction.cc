//---------------------------------------------------------------------------// 
// $Id: LBNEDetectorConstruction.cc,v 1.3.2.32 2013/10/02 16:14:25 lebrun Exp $
//---------------------------------------------------------------------------// 

#include <fstream>
#include <vector>

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
  fRotBeamlineAngle.rotateX(-fBeamlineAngle);  

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
  fRockLength = fPlacementHandler->GetTotalLengthOfRock() + 4.0*cm; 
      // See LBNEVolumePlacements constructor. 
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
   
   if (fPlacementHandler->GetDoInstallShield()) { 
     this->ConstructLBNEShieldingHorn1(targethorn1Phys);
     this->ConstructLBNEShieldingHorn2(vHorn2);
     this->ConstructLBNEShieldingBetweenHorns(tunnel);
   }
   
   fHasBeenConstructed = true;
  
//  LBNERunManager *aRunManager=(LBNERunManager*)LBNERunManager::GetRunManager();
//  aRunManager->Initialize();
  
  return fRock;
}
void LBNEDetectorConstruction::ConstructLBNEHadronAbsorber(G4VPhysicalVolume *mother)
{

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
     G4GDMLParser parser;
     parser.Read( filename );
     G4LogicalVolume *topAbs = parser.GetVolume( "TOP" );
     // We dump the volume hierarchy.  Hoopefully not too deep, 
     /*
     for (int i=0; i != topAbs->GetNoDaughters(); ++i) {
        G4VPhysicalVolume *pVol = topAbs->GetDaughter(i);
        G4LogicalVolume *lVol = pVol->GetLogicalVolume();
	std::cerr << " Top level daughter # " << i << " Name " << lVol->GetName() 
	          << " at " << pVol->GetObjectTranslation() << std::endl;
        for (int ii=0; ii != lVol->GetNoDaughters(); ++ii) {
          G4VPhysicalVolume *pVol2 = lVol->GetDaughter(ii);
          G4LogicalVolume *lVol2 = pVol2->GetLogicalVolume();
	  std::cerr << "  .. 2nd level daughter # " << ii << " Name " << lVol2->GetName()
	   << " at " << pVol2->GetObjectTranslation() << std::endl;
         for (int iii=0; iii != lVol2->GetNoDaughters(); ++iii) {
           G4VPhysicalVolume *pVol3 = lVol2->GetDaughter(iii);
           G4LogicalVolume *lVol3 = pVol3->GetLogicalVolume();
	   std::cerr << "  ... 3rd level daughter # " << iii << " Name " << lVol3->GetName() 
	    << " at " << pVol3->GetObjectTranslation() << std::endl;
           for (int i4=0; i4 != lVol3->GetNoDaughters(); ++i4) {
            G4VPhysicalVolume *pVol4 = lVol3->GetDaughter(i4);
            G4LogicalVolume *lVol4 = pVol4->GetLogicalVolume();
	    std::cerr << "  .... 4rth level daughter # " << i4 << " Name " << lVol4->GetName()
	     << " at " << pVol4->GetObjectTranslation()  << std::endl;
            for (int i5=0; i5 != lVol4->GetNoDaughters(); ++i5) {
              G4VPhysicalVolume *pVol5 = lVol4->GetDaughter(i5);
              G4LogicalVolume *lVol5 = pVol5->GetLogicalVolume();
	      std::cerr << "  ..... 5rth level daughter # " << i5 << " Name " << lVol5->GetName() 
	      << " at " << pVol5->GetObjectTranslation()  << std::endl;
              for (int i6=0; i6 != lVol5->GetNoDaughters(); ++i6) {
                G4VPhysicalVolume *pVol6 = lVol5->GetDaughter(i6);
                G4LogicalVolume *lVol6 = pVol6->GetLogicalVolume();
	        std::cerr << "  ...... 6rth level daughter # " << i6 << " Name " << lVol6->GetName() << std::endl;
	      }
	    }
	  }
	}
      }
    }
*/	    
       
    
//     const G4Box *topSol = static_cast<const G4Box *>(topAbs->GetSolid());
//     const double marsTopWidth = topSol->GetYHalfLength();
//     const double marsTopHeight = topSol->GetXHalfLength();
//     const double marsTopLength = topSol->GetZHalfLength();
//     std::cerr << " Dimension of top level Hadron absorber MARS container, X " << topSol->GetXHalfLength() << 
//         " Y  "  << topSol->GetYHalfLength() <<   " Z  "  << topSol->GetZHalfLength() << std::endl;
//     std::cerr << " Number of daughters for TOP " << topAbs->GetNoDaughters() << std::endl;
     double maxHalfHeight = -1.0;
     double maxHalfWidth = -1.0;
     double maxHalfLength = -1.0;
     for (int i=0; i != topAbs->GetNoDaughters(); ++i) {
       G4VPhysicalVolume *pVol = topAbs->GetDaughter(i);
       G4LogicalVolume *lVol = pVol->GetLogicalVolume();
       std::cerr << " Daughther " << lVol->GetName();
       const G4Box *aBox = static_cast<const G4Box *>(lVol->GetSolid());
       G4ThreeVector loc = pVol->GetObjectTranslation();
       std::cerr << " at MARS coordinates " << loc[0] << ", " <<loc[1] << ", " << loc[2] << 
		     " zLength " << 2.0*aBox->GetZHalfLength() << std::endl;
       // Compute the maximum height, width.  Note the confusion about  X and Y X is up, vertical, in MArs  
       if ((std::abs(loc[2]) + aBox->GetZHalfLength()) > maxHalfLength)
           maxHalfLength = std::abs(loc[2]) + aBox->GetZHalfLength();
       if ((std::abs(loc[1]) + aBox->GetYHalfLength()) >  maxHalfWidth)
           maxHalfWidth = std::abs(loc[1]) + aBox->GetYHalfLength(); // Width is along X G4lbne orientation, which Y MARS 
       if ((std::abs(loc[0]) + aBox->GetXHalfLength()) >  maxHalfHeight)
           maxHalfHeight = std::abs(loc[0]) + aBox->GetXHalfLength();
	    // Height is along Y G4lbne orientation, which is negative X in MARS 
     }
     maxHalfHeight += 5.0*cm;
     maxHalfWidth += 5.0*cm;
     maxHalfLength += std::abs(maxHalfHeight*std::sin(fBeamlineAngle)) + 5.0*cm;;
     std::cerr << " Container volume for Hadron absorber, 1/2 width " 
	       << maxHalfWidth << " 1/2 Height " << maxHalfHeight 
	     << " 1/2 length " << maxHalfLength << std::endl;
     G4Box *aHABoxTop = new G4Box(G4String("HadronAbsorberTop"), maxHalfWidth, maxHalfHeight, maxHalfLength);
     G4LogicalVolume *aHATopL = 
        new G4LogicalVolume(aHABoxTop, G4Material::GetMaterial("Air"), G4String("HadronAbsorberTop"));
      const LBNEVolumePlacementData *plDecayPipe = 
         fPlacementHandler->Find(G4String("HadronAbsorber"), G4String("DecayPipeHall"), 
	                         G4String("LBNEDetectorConstruction::ConstructLBNEHadronAbsorber"));
     const double zzz = maxHalfLength + plDecayPipe->fParams[2]/2 + plDecayPipe->fPosition[2] + 
         std::abs(maxHalfHeight*std::sin(fBeamlineAngle)) +  5.0*cm;	
//     std::cerr << " half length Decay Pipe " << plDecayPipe->fParams[2]/2 
//               << " Position in tunnel (center) " << plDecayPipe->fPosition[2] 
//	       << " ZPosAbs " << zzz <<  std::endl;	 
     G4ThreeVector posTopHA(0., 0., zzz);
     new G4PVPlacement(&fRotBeamlineAngle, posTopHA, aHATopL, "HadronAbsorberTop", 
  		     mother->GetLogicalVolume(), false, 1, true);
     
     G4RotationMatrix *marsRot = new G4RotationMatrix;
     marsRot->rotateZ(-M_PI/2.);
     for (int i=0; i != topAbs->GetNoDaughters(); ++i) {
       G4VPhysicalVolume *pVol = topAbs->GetDaughter(i);
       G4LogicalVolume *lVol = pVol->GetLogicalVolume();
//       const G4Box *aBox = static_cast<const G4Box *>(lVol->GetSolid());
       G4ThreeVector loc = pVol->GetObjectTranslation();
//       const double yyy = loc[0]  - marsTopHeight + maxHalfHeight;  
       const double yyy = loc[0] ;  // Up to a sign!!! 
       const double xxx = loc[1]; // Y G4LBNE = -X Mars. Was centered in MARS, set to 0., o.k.
         // X G4LBNE = Y Mars. !! Not centered in Mars! Perhaps, ned a shift due to the 
        // the different size of the mother volume  
//       const double zzz = loc[2] - marsTopLength + maxHalfLength;
       const double zzz = loc[2] - 27.9*m; // Setting up this last shift with Geantino .
        //  Difference in lengths for mother volume.  
//       const double zzz = -maxHalfLength + aBox->GetZHalfLength() + 0.5*cm;
       G4ThreeVector posTmp(xxx, yyy, zzz);
//       std::cerr << " Placing volume " << lVol->GetName() << " at " << posTmp << " 1/2 sizes (G4 coord)  " 
//          << aBox->GetYHalfLength() << " , " << aBox->GetXHalfLength() << " , " 
//	  <<  aBox->GetZHalfLength() << std::endl;
//       std::cerr << "  .... Extend in Y " << posTmp[1] - aBox->GetXHalfLength() 
//                 << " to " << posTmp[1] + aBox->GetXHalfLength() << std::endl;
//       std::cerr << "  .... Extend in Z " << posTmp[2] - aBox->GetZHalfLength() 
//                 << " to " << posTmp[2] + aBox->GetZHalfLength() << std::endl;
       new G4PVPlacement(marsRot, posTmp, lVol, lVol->GetName() + std::string("_P"), aHATopL, false, 1, true);
     }
     //
     // We now clear the MARS top absorbers, simplify the geometry search 
     //
     topAbs->ClearDaughters();
}

void LBNEDetectorConstruction::ConstructLBNEShieldingHorn1(G4VPhysicalVolume *mother) {

   const double in = 25.4*mm;
//
// Install steel shielding around the Horn1 and the target. 
//
// Based on Docdb 5339, page 30 and 31. 
// See also drawing 2251.000-ME-487105 
// 
// We simply install steel, assumed low carbon...
//
   const G4String nameM = mother->GetLogicalVolume()->GetName();
   const LBNEVolumePlacementData *plTop = 
         fPlacementHandler->Find(G4String("ShieldingHorn1"), nameM, 
	                         G4String("LBNEDetectorConstruction::ConstructLBNEShieldingHorn1"));
				 
   const double horn1HeightAtMCZERO = 66.7*in;				 
//
// Bottom 
//
   G4String bName = G4String("ShieldingHorn1Bottom"); 
   const double bWidth = 208*in;
   const double bHeight = 72.0*in;
   const double bLength = plTop->fParams[2] - 2.*bHeight*std::abs(std::sin(fBeamlineAngle)) - 5.0*cm;
   G4Box *bBox = new G4Box(bName, bWidth/2., bHeight/2., bLength/2.);
   G4LogicalVolume *bLVol = new G4LogicalVolume(bBox, G4Material::GetMaterial(std::string("Slab_Stl")), bName); 
   G4ThreeVector posTmp(0., 0., 0.); 
   posTmp[1] = -horn1HeightAtMCZERO  - bHeight/2.; 
   // MCZERO is G4 coordinate 0.0, the center of tunnel, so the above number needs to be corrected for the 
   // shift in the center of the mother volume  
   const double zShift = -1.0*plTop->fPosition[2];
   const double yCorr = -1.0*zShift*std::sin(fBeamlineAngle);
   posTmp[1]  -= yCorr;
//   std::cerr << "ConstructLBNEShieldingHorn1 .... "<< bName << " zShift bottom plate " << zShift 
//	     << " thus, y position " << posTmp[1] << " coorection " << yCorr << std::endl;
//	     
   new G4PVPlacement(&fRotBeamlineAngle, posTmp, bLVol, bName + std::string("_P"),
                       mother->GetLogicalVolume(), false, 1, true);
//
// Sides 
//
   G4String sName = G4String("ShieldingHorn1Side"); 
   const double sWidth = 72.0*in;
   const double sHeight = 270.8*in - 72.0*in - 4.0*cm; // oversize a bit, but no matter.. 
   const double sLength = plTop->fParams[2] - 2.*sHeight*std::abs(std::sin(fBeamlineAngle)) - 5.0*cm;
   G4Box *sBox = new G4Box(sName, sWidth/2., sHeight/2., sLength/2.);
   G4LogicalVolume *sLVol = new G4LogicalVolume(sBox, G4Material::GetMaterial(std::string("Slab_Stl")), sName); 
   posTmp[0] = (-32.0 - 36.0)*in;
   posTmp[1] = sHeight/2. - horn1HeightAtMCZERO + 2.0*cm; 
   // MCZERO is G4 coordinate 0.0, the center of tunnel, so the above number needs to be corrected for the 
   // shift in the center of the mother volume  
   posTmp[1]  -= yCorr;
//   std::cerr << "ConstructLBNEShieldingHorn1 .... "<< bName << " zShift side plate, left  " << zShift 
//	     << " thus, y position " << posTmp[1] << " coorection " << yCorr 
//	     << std::endl << " .......  X = " << posTmp[0] << " Y " << posTmp[1] << std::endl;
	     
   new G4PVPlacement(&fRotBeamlineAngle, posTmp, sLVol, sName + std::string("_LeftP"),
                       mother->GetLogicalVolume(), false, 1, true);
   posTmp[0] = (+32.0 + 36.0)*in;
//   std::cerr << "ConstructLBNEShieldingHorn1 .... "<< bName << " zShift side plate, Right  " << zShift 
//	     << " thus, y position " << posTmp[1] << " coorection " << yCorr 
//	     << std::endl << " .......  X = " << posTmp[0] << " Y " << posTmp[1] << std::endl;
	     
   new G4PVPlacement(&fRotBeamlineAngle, posTmp, sLVol, sName + std::string("_RightP"),
                       mother->GetLogicalVolume(), false, 2, true);
//
// Top    
}

void LBNEDetectorConstruction::ConstructLBNEShieldingHorn2(G4PVPlacement *mother) {

}

void LBNEDetectorConstruction::ConstructLBNEShieldingBetweenHorns(G4VPhysicalVolume *mother) {


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
	
	
