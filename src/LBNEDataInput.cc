//----------------------------------------------------------------------
//
//
// $Id: LBNEDataInput.cc,v 1.2 2013/01/31 19:23:30 loiacono Exp $
//----------------------------------------------------------------------

#include "LBNEDataInput.hh"
#include "LBNENumiDataInput.hh"
#include "G4ThreeVector.hh"
//#include "NumiHornSpiderSupport.hh"
#include "G4UserLimits.hh"
#include "LBNEKeyedInput.hh"
//#include "globals.hh"
#include <math.h>
#include <fstream>

static const G4double in=2.54*cm;

//Specify a file to read input from. If no file is given,
//default.input is used. If used, this function should be called
//before GetLBNEDataInput is called for the first time.
void LBNEDataInput::setInputFile(const G4String& filename) 
{
  char* directory = getenv("G4NUMI_INPUT_DIR");
  if(directory == NULL) fInputFileName = "";
  else fInputFileName = directory;
  fInputFileName += filename;
}

G4String LBNEDataInput::fInputFileName = "default.input";
LBNEDataInput* LBNEDataInput::fLBNEDataInput = 0;

//-------------------------------------------------------------------------------
//Obtain the instance of the singleton class LBNEDataInput. Constructs
//an instance if not already constructed.
LBNEDataInput* LBNEDataInput::GetLBNEDataInput()
{
    
    if (!fLBNEDataInput) {
        G4cout << "Constructing LBNEDataInput " << G4endl;
        fLBNEDataInput = new LBNEDataInput();   
	//fLBNEDataInput->printAll();
    }
    return fLBNEDataInput;
}
//-------------------------------------------------------------------------------
//Read in a file specified by the static variable
//fInputFileName. Creates an instance of LBNEKeyedInput, stored in the
//member variable keyedInput. This instance of LBNEKeyedInput is used
//to parse the data from the input file. A series of functions are
//called which read in data from keyedInput and store them as member
//variables of this LBNEDataInput object.
LBNEDataInput::LBNEDataInput()
   :fSimulation(""),
    fInputNtupleFileName(""),     
    fInputNtpTreeName(""),
    fOutputNtpFileName(""),
    //fBeamConfig(""),
    //fTargetConfig(""),
    //fIHornConfig(""),
    //fHornConfig(""),
    fGeometryTag(""),
    fRunNumber("0000"),
    fASCIIOutputFileName("asciiOut"),

    fKillTracking(true),
    //fUseCorrHornCurrent(true),    
    fOKToRun(false),

    fCreateOutput(false),
    fNImpWeightOn(true), 
    fUseFlukaInput(false),
    fUseMarsInput(false),
    fCreateASCIIOutput(false),
    fBeamOnTarget(false),
    fConstructTarget(true),

    fDebugLevel(1),
    fRunPeriod(-999),
    fNEvents(-1),

    fHornCurrent(0.0),
    fExtraFlukaNumiTargetZShift(0.*cm),
    fKillTrackingThreshold(0.05*GeV),
    fMaterialSigma(0.0),
    fStepLimit(0.0), 
    fTimeLimit(60.0*s),
    

    fSimulationVec(),

    fDefaultMaterial(0)
/*fSimulation(""),
    fInputNtpTreeName(""),
    fOKToRun(false),
    fCheckOverlaps(false),
    fCreateOutput(false),
    fDebugLevel(0)
*/
{
  
  if(fLBNEDataInput) G4Exception("LBNEDataInput constructed twice.","",FatalException,"");
  
  G4cout << G4endl;
  G4cout << G4endl;
  G4cout << "********************************************************************" << G4endl;
  G4cout << "********************************************************************" << G4endl;
  G4cout << "LBNEDataInput Initializing..." << G4endl;
  G4cout << "********************************************************************" << G4endl;
  
  
  preInit();
  readFile();


  //readFile();
  //preInit();
  initHornStandard();

  initBeam();       // must come before initTarget
  initRock();       // must come before initTargetArea
  initTargetArea(); // must come before initTarget
  initTarget();     // must come before initBaffle
  initBaffle();
  initTunnel();
  initShield();
  initDecayPipe();
  initBlocks();
  initHadrBox();
  initHorns();
  initDetectors();


  if(keyedInput != NULL) delete keyedInput;

  G4cout << "********************************************************************" << G4endl;
   G4cout << "...LBNEDataInput Initialization Complete." << G4endl;
   G4cout << "********************************************************************" << G4endl;
   G4cout << "********************************************************************" << G4endl;
   G4cout << G4endl;
   G4cout << G4endl;
}
//-------------------------------------------------------------------------------
//reads in keyedInput
void LBNEDataInput::readFile() 
{
  keyedInput = NULL;
  if(!fInputFileName.isNull()) {
    G4cout << "Reading gnumi-formatted data from " << fInputFileName << G4endl;
    keyedInput = new LBNEKeyedInput();
    keyedInput->readFile(fInputFileName);
  }
}
//-------------------------------------------------------------------------------
void LBNEDataInput::preInit() 
{
   //
   //For Fluka05 files run for the NuMI target
   //there is a shift of the "z" variable by -35cm
   //must shift it to 0+TargetZ0 in NumiPrimaryGeneratorAction
   //when setting up the particle beam so need to apply an
   //extra +35cm shift to get it first to 0cm
   //IF YOU NEED THIS (i.e. if you are running with one 
   //of these type of fluka files as input) YOU MUST SET
   //NuMI/run/AddExtraFlukaNumiTargetZShift 35 cm 
   //IN YOUR MACRO!  
   //
   fExtraFlukaNumiTargetZShift = 0.*cm;


   //
   //Fill Simulation Vector with the list of Possible simulations
   //
   fSimulationVec.push_back("Standard Neutrino Beam");
   fSimulationVec.push_back("Target Tracking");
   fSimulationVec.push_back("Horn 1 Tracking");
   fSimulationVec.push_back("Horn 2 Tracking");


/*


  debugOn = false;
  NImpWeightOn = true; 
  createNuNtuple = createHadmmNtuple = createASCII = createBXDRAW = false;
  useFlukaInput = useMarsInput = false;
  //can set useMuonInupt and useMuonBeam from macro
  useMuonInput = useMuonBeam = false;
  fNEvents = -1;
  useTestBeam = false;
  useDecayPipeSelect = false;
  KillTracking = true; // false for ahimmel
  testTheta = M_PI/6.;
  
  StepLimit = 0.0;
  extNtupleFileName=""; //fluka or mars or muon ntuple with particles coming
                        //  of the target
  //Set the energy threshold for 'killing' particles
  KillTrackingThreshold = 0.05*GeV; //for defaut neutrino MC g4numi 
  //KillTrackingThreshold = 0.001*GeV; //for muon beam MC
  
  
  nuNtupleName    = "nuNtuple"; //base name for output files
  //can set hadmmNtupleDir and hadmmNtupleName from macro
  hadmmNtupleDir  = "./";
  hadmmNtupleName = "";
  asciiName       = "asciiOut";
  bxdrawName      = "bxdrawOut";
  RunNumber       = "0000";
  geometry        = "";
  
  // Denotes the change in sigma to the
  // rock density and muon alcove wall location
  materialSigma   = 0;
*/


}
//---------------------------------------------------------------------------------
void LBNEDataInput::PrintSetup()
{
   G4String spaces = "   ";
   std::cout << std::endl;
   std::cout << spaces << "NumiDataInput::Print() - Printing..." << std::endl;
   std::cout << spaces << spaces << "fSimulation           = " << fSimulation << std::endl;
   std::cout << spaces << spaces << "fGeometryTag          = " << fGeometryTag  << std::endl;
   std::cout << spaces << spaces << "fRunNumber            = " << fRunNumber    << std::endl;
   std::cout << spaces << spaces << "fMaterialSigma        = " << fMaterialSigma       << std::endl;
   std::cout << spaces << spaces << "fStepLimit            = " << fStepLimit/cm << " cm" << std::endl;
   std::cout << spaces << spaces << "fTimeLimit            = " << fTimeLimit/s << " s"   << std::endl;
   std::cout << spaces << spaces << "fExtraFlukaNumiTargetZShift  = " << fExtraFlukaNumiTargetZShift/cm << " cm" << std::endl;
   std::cout << spaces << spaces << "fDebugLevel           = " << fDebugLevel      << std::endl;
   std::cout << spaces << spaces << "fNEvents              = " << fNEvents         << std::endl;
   std::cout << spaces << spaces << "fNImpWeightOn         = " << fNImpWeightOn       << std::endl;
   std::cout << spaces << spaces << "fKillTracking         = " << fKillTracking << std::endl;
   if(fKillTracking)
   {  std::cout << spaces << spaces << spaces << "fKillTrackingThreshold   = " << fKillTrackingThreshold/GeV << " GeV" << std::endl;}
   std::cout << spaces << spaces << "fUseFlukaInput        = " << fUseFlukaInput      << std::endl;
   std::cout << spaces << spaces << "fUseMarsInput         = " << fUseMarsInput       << std::endl;
   if(fUseFlukaInput || fUseMarsInput)
   {  std::cout << spaces << spaces << spaces << "fInputNtupleFileName  = " << fInputNtupleFileName << std::endl;
      std::cout << spaces << spaces << spaces << "fInputNtpTreeName     = " << fInputNtpTreeName << std::endl;}
   std::cout << spaces << spaces << "fCreateOutput         = " << fCreateOutput       << std::endl;
   if(fCreateOutput)
   {  std::cout << spaces << spaces << spaces << "fOutputNtpFileName    = " << fOutputNtpFileName << std::endl;}
   std::cout << spaces << spaces << "fCreateASCIIOutput    = " << fCreateASCIIOutput  << std::endl;
   if(fCreateASCIIOutput)
   {  std::cout << spaces << spaces << spaces << "fASCIIOutputFileName  = " << fASCIIOutputFileName  << std::endl;}
   std::cout << spaces << spaces << "fOKToRun              = " << fOKToRun            << std::endl;   

   
   std::cout << std::endl;
   
}

//-------------------------------------------------------------------------------
void LBNEDataInput::initHornStandard() 
{
  //
  //Horns
  //
  hrnmat = 9;   // Al
  hrnmatcr =31; // CT852

}

//-------------------------------------------------------------------------------
void LBNEDataInput::initBeam(G4double beamz0) 
{
  G4float beam_x_dir = -999.0;
  G4float beam_y_dir = -999.0;
  G4float beam_z_dir = -999.0;
  
  G4double beam_x_pos = -999.0;
  G4double beam_y_pos = -999.0;
  G4double beam_z_pos = beamz0;
  if(beam_z_pos == beam_x_pos)
  {  
     get(beam_z_pos, "BeamZ0", m); 
  }
  get(beam_x_pos, "BeamX0", m);
  get(beam_y_pos, "BeamY0", m);

  
  get(fProtonMomentum, "BeamP", GeV);
  get(fBeamSigmaX, "BeamSigx", m);
  get(fBeamSigmaY, "BeamSigy", m);
  fBeamDirection = G4ThreeVector(beam_x_dir,beam_y_dir,beam_z_dir);
  fBeamPosition = G4ThreeVector(beam_x_pos,beam_y_pos,beam_z_pos);

  get(fBeamSigDx, "BeamSigdx");
  get(fBeamSigDy, "BeamSigdy");
  get(fBeamMaxDx, "BeamMaxdx");
  get(fBeamMaxDy, "BeamMaxdy");

  get(fBeamOnTarget, "BeamOnTarget");
  
  fProtonKineticEnergy= sqrt(pow((.938*GeV),2)+pow(fProtonMomentum,2))-0.938*GeV;
}
//-------------------------------------------------------------------------------
void LBNEDataInput::initRock() {
  get(RockRadius, "RockRadius", m);
  get(RockHalfLen, "RockHalfLen", m);
  get(RockDensity, "RockDensity", g/cm3);
}
//-------------------------------------------------------------------------------
void LBNEDataInput::initTargetArea() {
  get(TargetAreaZ0,       "TargetAreaZ0",     m);
  get(TargetAreaLength,   "TargetAreaLength", m);
  get(TargetAreaHeight,   "TargetAreaHeight", m);
  get(TargetAreaWidth,    "TargetAreaWidth",  m);
  get(TargetAreaGEANTmat, "TargetAreaGEANTmat");
}

//-------------------------------------------------------------------------------
void LBNEDataInput::initTarget() 
{
  //All of the Target inputs are now being used.

  fConstructTarget = true;

  getConstructFlag(TargetNtarget, "TargetNtarget");
  if(TargetNtarget <= 0) G4Exception("TargetNtarget must be at least one","",FatalException,"");

  get(TargetShape, "TargetShape");


  if(TargetShape == "NUMI" || 
     TargetShape == "numi" || 
     TargetShape == "Numi" || 
     TargetShape == "NuMI")
  {
     if (TargetNtarget != 1)
     {
	G4Exception("TargetNtarget must be 1 for the Numi target","",FatalException,"");
     }
     LBNEDataInput::initNUMITarget();
  }
  else
  {
     get(TargetX0,     "TargetX0",     TargetNtarget, m);
     get(TargetY0,     "TargetY0",     TargetNtarget, m);
     get(TargetZ0,     "TargetZ0",     TargetNtarget, m);
     get(TargetDxdz,   "TargetDxdz",   TargetNtarget);
     get(TargetDydz,   "TargetDydz",   TargetNtarget);
     get(TargetLength, "TargetLength", TargetNtarget, m);
     
     if(TargetShape == "BOX") 
     {
	get(TargetWidth,  "TargetWidth",  TargetNtarget, m);
	get(TargetHeight, "TargetHeight", TargetNtarget, m);
     }
     else if(TargetShape == "TUBE") 
     {
	get(TargetRadius, "TargetRadius", TargetNtarget, m);
     }
     else 
     {
	G4Exception("TargetShape must be either 'BOX', 'TUBE', or 'NUMI'","",FatalException,"");
     }

     get(TargetA,        "TargetA",         TargetNtarget, g/mole);
     get(TargetZ,        "TargetZ",         TargetNtarget);
     get(TargetDensity,  "TargetDensity",   TargetNtarget, g/cm3);
     get(TargetGEANTmat, "TargetGEANTmat",  TargetNtarget);
  }


}

//-------------------------------------------------------------------------------
void LBNEDataInput::initNUMITarget() 
{
   G4cout << G4endl;
   G4cout << "****************************************************************" << G4endl;
   G4cout << "LBNEDataInput::initNUMITarget() - Initializing NUMI Target..." << G4endl;

   //
   //Set NUMI Target Numbers
   //

   G4double TgtZ0BaffleDownZDist = 680*mm; //see notes in LBNENumiDataInput
   G4double HPBaffleLength       = 1.5*m;
   G4double BaffleBeamDist       = 10*cm; //arbitrary

   TargetA.push_back(12.01*g/mole);
   TargetZ.push_back(6.);
   TargetDensity.push_back(1.78*g/cm3); //1.815*g/cm3;//1.754*g/cm3;
   TargetGEANTmat.push_back(18);
   
   //
   //Read In Target Z0 but nothing but Z0 works right now 
   //!!!!!!This is all that may be set for the NUMI target!!!!!!
   //


//   getConstructFlag(TargetNtarget, "TargetNtarget");
   get(TargetX0,     "TargetX0",     TargetNtarget, m);
   get(TargetY0,     "TargetY0",     TargetNtarget, m);
   get(TargetZ0,     "TargetZ0",     TargetNtarget, m);
   get(TargetDxdz,   "TargetDxdz",   TargetNtarget);
   get(TargetDydz,   "TargetDydz",   TargetNtarget);
   
   LBNENumiDataInput *LBNENumiData = LBNENumiDataInput::GetLBNENumiDataInput();
   LBNENumiData -> SetTargetX0  (TargetX0[0]);
   LBNENumiData -> SetTargetY0  (TargetY0[0]);
   LBNENumiData -> SetTargetZ0  (TargetZ0[0]);
   LBNENumiData -> SetTargetDxdz(TargetDxdz[0]);
   LBNENumiData -> SetTargetDydz(TargetDydz[0]);
   //
   //Target Area Must have been read in before now
   //
   LBNENumiData -> SetTargetAreaZ0       (TargetAreaZ0);
   LBNENumiData -> SetTargetAreaLength   (TargetAreaLength);
   LBNENumiData -> SetTargetAreaHeight   (TargetAreaHeight);
   LBNENumiData -> SetTargetAreaWidth    (TargetAreaWidth);
   LBNENumiData -> SetTargetAreaGEANTmat (TargetAreaGEANTmat);


   //set proton beam location to be BaffleBeamDist in front of baffle
   G4double beamz0 = TargetZ0[0] - TgtZ0BaffleDownZDist -  HPBaffleLength - BaffleBeamDist;
   initBeam(beamz0); 


   G4cout << "LBNEDataInput::initNUMITarget() - Done initializing NUMI Target." << G4endl;
   G4cout << "****************************************************************" << G4endl;
   G4cout << G4endl;

}

//-------------------------------------------------------------------------------
void LBNEDataInput::initBaffle() {
  getConstructFlag(HPBaffle, "HPBaffle");
  if(!HPBaffle) return;

  if(TargetShape == "NUMI" || 
     TargetShape == "numi" || 
     TargetShape == "Numi" || 
     TargetShape == "NuMI")
 {
    
    G4cout << "*****************************************************"<< G4endl;
    G4cout << "LBNEDataInput::initBaffle() - The Baffle is automatically" << G4endl;
    G4cout << " constructed when choosing to simulate the NUMI Target." << G4endl;
    G4cout << "*****************************************************"<< G4endl;
    G4cout << G4endl;
    return;
 }
  
  get(HPBaffleGEANTMat, "HPBaffleGEANTMat");
  get(HPBaffleX0,       "HPBaffleX0", m);
  get(HPBaffleY0,       "HPBaffleY0", m);
  get(HPBaffleZ0,       "HPBaffleZ0", m);
  get(HPBaffleDXDZ,     "HPBaffleDXDZ");
  get(HPBaffleDYDZ,     "HPBaffleDYDZ");
  get(HPBaffleLength,   "HPBaffleLength", m);
  get(HPBaffleRin,      "HPBaffleHoleR", m);
  get(HPBaffleRout,     "HPBaffleRadius", m);
}
//-------------------------------------------------------------------------------
void LBNEDataInput::initTunnel() {
  get(TunnelZ0, "TunnelZ0", m);
  get(TunnelRadius, "TunnelRadius", m);
  get(TunnelLength, "TunnelLength", m);
  get(TunnelA, "TunnelA");
  get(TunnelZ, "TunnelZ");
  get(TunnelGEANTmat, "TunnelGEANTmat");
  BeamAngle      = 0.05835; // in radians.
}
//-------------------------------------------------------------------------------
void LBNEDataInput::initShield() {
  getFirst(ShieldX0, "ShieldX0", m);
  getFirst(ShieldY0, "ShieldY0", m);
  getFirst(ShieldZ0, "ShieldZ0", m);
  getFirst(ShieldDxdz, "ShieldDxdz");
  getFirst(ShieldDydz, "ShieldDydz");
  getFirst(ShieldLength, "ShieldLength", m);
  getFirst(ShieldRout, "ShieldRout", m);
  getFirst(ShieldRin, "ShieldRin", m);
  getFirst(ShieldGEANTmat, "ShieldGEANTmat");
}
//-------------------------------------------------------------------------------
void LBNEDataInput::initDecayPipe() {
  get(DecayPipeZ0, "DecayPipeZ0", m);
  get(DecayPipeRadius, "DecayPipeRadius", m);
  get(DecayPipeLength, "DecayPipeLength", m);
  get(DecayPipeFWinThick, "DecayPipeFWinThick", m);
  get(DecayPipeEWinThick, "DecayPipeEWinThick", m);
  get(DecayPipeWallThick, "DecayPipeWallThick", m);
  get(DecayPipeA, "DecayPipeA");
  get(DecayPipeZ, "DecayPipeZ");
  get(DecayPipeGEANTmat, "DecayPipeGEANTmat");
  get(DecayPipeFWinmat, "DecayPipeFWinmat");
  get(DecayPipeEWinmat, "DecayPipeEWinmat");
  get(DecayPipeFillGeantMat, "DecayPipeFillGeantMat");
}
//-------------------------------------------------------------------------------
void LBNEDataInput::initBlocks() 
{
  get(BlockNblock, "BlockNblock");

  get(BlockX0, "BlockX0", BlockNblock, m);
  get(BlockY0, "BlockY0", BlockNblock, m);
  get(BlockZ0, "BlockZ0", BlockNblock, m);
  get(BlockDxdz, "BlockDxdz", BlockNblock);
  get(BlockDydz, "BlockDydz", BlockNblock);
  get(BlockLength, "BlockLength", BlockNblock, m);
  get(BlockHdx, "BlockHdx", BlockNblock, m);
  get(BlockHdy, "BlockHdy", BlockNblock, m);
  get(BlockGeantMaterial, "BlockGeantmat", BlockNblock);
}
//-------------------------------------------------------------------------------
void LBNEDataInput::initHadrBox() {
  // Hadron Box Dimensions
  // 55.75' is the surveyed distance between the
  // downstream wall of Muon Alcove 1 and the
  // nominal center of the Decay Pipe.
  HadrBox_width = 324*.0254*m;
  HadrBox_height = 6.6294*m;
  HadrBox_length = 55.75*12*in-(4*12*in+2.24*in+9*12*in+9*in);
}
//-------------------------------------------------------------------------------
void LBNEDataInput::initHorns() 
{
   get(NPhorns,     "NPhorns");
   if(NPhorns <= 0) G4Exception("NPhorns must be greater than zero","",FatalException,"");
   get(PhornNParts, "PhornNParts", NPhorns);

   get(PhornNphorn, "PhornNphorn");
   if(PhornNphorn <= 0) G4Exception("PhornNphorn must be greater than zero","",FatalException,"");

   get(PhornZ1,       "PhornZ1",         PhornNphorn, m);
   get(PhornZ2,       "PhornZ2",         PhornNphorn, m);
   get(PhornNeckZ1,   "PhornNeckZ1",     PhornNphorn, m);
   get(PhornNeckZ2,   "PhornNeckZ2",     PhornNphorn, m);
   get(PhornNpoint,   "PhornNpoint",     PhornNphorn);
   get(PhornAin,      "PhornAin",        PhornNphorn, cm);
   get(PhornBin,      "PhornBin",        PhornNphorn, 1.0/cm);
   get(PhornCin,      "PhornCin",        PhornNphorn, cm);
   get(PhornAout,     "PhornAout",       PhornNphorn, cm);
   get(PhornBout,     "PhornBout",       PhornNphorn, 1.0/cm);
   get(PhornCout,     "PhornCout",       PhornNphorn, cm);
   get(PhornROCin,    "PhornROCin",      PhornNphorn, m);
   get(PhornROCout,   "PhornROCout",     PhornNphorn, m);
   get(PhornThickFront, "PhornThickFront", PhornNphorn, m);
   get(PhornThickEnd,   "PhornThickEnd",   PhornNphorn, m);
   get(PhornX0,       "PhornX0",         PhornNphorn, m);
   get(PhornY0,       "PhornY0",         PhornNphorn, m);
   get(PhornZ0,       "PhornZ0",         PhornNphorn, m);
   get(PhornDXDZ,     "PhornDXDZ",       PhornNphorn);
   get(PhornDYDZ,     "PhornDYDZ",       PhornNphorn);
   get(PhornCurrent,  "PhornCurrent",    PhornNphorn, (1E6)*ampere);
   get(PhornGEANTmat, "PhornGEANTmat",   PhornNphorn);
   
   fHornCurrent = PhornCurrent[0];


     
}
//-------------------------------------------------------------------------------
void LBNEDataInput::initDetectors() 
{
   //Near & Far Detector location
  nNear=5;
  nFar=3;
  G4double xdetNear[]    = { 0.       ,  0.      ,    11.50,     0.         ,     25.84   };
  G4double ydetNear[]    = { 0.       ,  0.      ,    -3.08,     0.         ,     78.42   };
  G4double zdetNear[]    = {1000.     ,1036.49   ,   1000.97,   1030.99     ,    745.25   };
//  G4String detNameNear[] = {"LBNE", "Minos",        "Nova", "Minerva"  , "MiniBooNE" };
  G4double xdetFar[]     = { 0.       , 0.        , 11040.   };
  G4double ydetFar[]     = { 0.       , 0.        , -4200.   };
  G4double zdetFar[]     = { 1297000. , 735340.  , 810450.   };
//  G4String detNameFar[]  = {"LBNE",  "Minos" , "Nova"};

  get(xdetNear[0], "FluxAreaX0near");
  get(ydetNear[0], "FluxAreaY0near");
  get(zdetNear[0], "FluxAreaZ0near");
  
  get(xdetFar[0], "FluxAreaX0far");
  get(ydetFar[0], "FluxAreaY0far");
  get(zdetFar[0], "FluxAreaZ0far");

  for(G4int ii=0;ii<nNear;ii++){
    xdet_near.push_back(xdetNear[ii]*m);
    ydet_near.push_back(ydetNear[ii]*m);
    zdet_near.push_back(zdetNear[ii]*m);
  }
  for(G4int ii=0;ii<nFar;ii++){
    xdet_far.push_back(xdetFar[ii]*m);
    ydet_far.push_back(ydetFar[ii]*m);
    zdet_far.push_back(zdetFar[ii]*m);
  }
}

//Some of these "get" functions just call keyedInput->get. This
//indirection is used in case if at some point in the future someone
//wants to make a new way to receive input. Then, only these "get"
//functions will need to be updated.
//-------------------------------------------------------------------------------
template<class T>
void LBNEDataInput::get(T& buffer, const G4String& name)
{
  keyedInput->get(name, buffer);
}
//-------------------------------------------------------------------------------
template<class T>
void LBNEDataInput::get(std::vector<T>& buffer, const G4String& name,
                        G4int expectedLength)
{
  keyedInput->get(name, buffer, expectedLength);
}

//-------------------------------------------------------------------------------
//This function is used to get the first element of an array with the
//given name from input. It expects the array to exist and have at
//least one value.
template<class T>
void LBNEDataInput::getFirst(T& buffer, const G4String& name) {
  std::vector<T> myVector;
  get(myVector, name);
  if(myVector.size() == 0) {
    G4String message = "expected non-empty array: ";
    message.append(name);
    G4Exception(message,"",FatalException,"");
  }
  else {
    buffer = myVector[0];
  }
}

//-------------------------------------------------------------------------------
//This function gets a double like usual, and then multiplies it by
//the given scalar.
void LBNEDataInput::get(G4double& buffer, const G4String& name,
			G4double scalar)
{
  get(buffer, name);
  buffer *= scalar;
}

//-------------------------------------------------------------------------------
//This function gets an array of doubles like usual, and then
//multiplies them by the given scalar.
void LBNEDataInput::get(vdouble_t& buffer, const G4String& name,
                        G4int expectedLength, G4double scalar)
{
  get(buffer, name, expectedLength);
  for(size_t i = 0; i < buffer.size(); i++) buffer[i] *= scalar;
}

//-------------------------------------------------------------------------------
//This function is the same as the other getFirst function, but after
//the first array element is retrieved, it is multiplied by the given
//scalar.
void LBNEDataInput::getFirst(G4double& buffer, const G4String& name,
                             G4double scalar)
{
  getFirst(buffer, name);
  buffer *= scalar;
}



//-------------------------------------------------------------------------------
//Works like a regular get function, except the variable does not have
//to be found in the input file. If it is not found, the value of
//zero/false will be used. This function can be used with an int or a
//bool as type T.
template<class T>
void LBNEDataInput::getConstructFlag(T& buffer, const G4String& name) {
  if(keyedInput->contains(name)) keyedInput->get(name, buffer);
  else buffer = 0;
}
//-------------------------------------------------------------------------------
LBNEDataInput::~LBNEDataInput()
{  
}
//---------------------------------------------------------------------------------
void LBNEDataInput::ApplyStepLimits(G4LogicalVolume *vol)
{
  if (fStepLimit == 0.0) return;
  vol->SetUserLimits(new G4UserLimits(fStepLimit));
}

//---------------------------------------------------------------------------------
void LBNEDataInput::ApplyTimeLimits(G4LogicalVolume *vol)
{
  if (fTimeLimit == 0.0) return;
  vol->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,fTimeLimit));
}

//-------------------------------------------------------------------------------
//Helper macros for printAll. The macro PRINT# will input # variable
//names and print out each variable name and value.

#define PRINT1(a) print(#a, a);
#define PRINT2(a,b) PRINT1(a) print(#b, b);
#define PRINT3(a,b,c) PRINT2(a,b) print(#c, c);
#define PRINT4(a,b,c,d) PRINT3(a,b,c) print(#d, d);
#define PRINT5(a,b,c,d,e) PRINT4(a,b,c,d) print(#e, e);
#define PRINT6(a,b,c,d,e,f) PRINT5(a,b,c,d,e) print(#f, f);
#define PRINT7(a,b,c,d,e,f,g) PRINT6(a,b,c,d,e,f) print(#g, g);

//-------------------------------------------------------------------------------
//printAll is used to print all of the member variable names and
//values for this LBNEDataInput object.
void LBNEDataInput::printAll() {
  G4cout << G4endl;
  G4cout << "Printing LBNEDataInput..." << G4endl;
  G4cout << "-------------------------" << G4endl;
  
//  PRINT4(NImpWeightOn);
//  PRINT4(createHadmmNtuple, createASCII, createASCII, useFlukaInput);
//  PRINT5(useMarsInput, useMuonBeam, useMuonInput, solidMuMons, fNEvents);
//  PRINT4(KillTracking, nuNtupleName, hadmmNtupleName, hadmmNtupleDir);
//  PRINT6(asciiName, RunNumber, geometry, StepLimit, createBXDRAW, useTestBeam);
//  PRINT3(useDecayPipeSelect, bxdrawName, testTheta);

//  PRINT6(useMacro, NZpoint, Zpoint, zpNtupleName, createZpNtuple, raytracing);

//  PRINT7(airhrn, vacuumworld, jCompare, g3Chase, hrnmat, hrnmatcr, hallmat);

//  PRINT4(protonMomentum, beamSigmaX, beamSigmaY, protonKineticEnergy);
//  PRINT4(materialSigma, KillTrackingThreshold, beamPosition, beamDirection);
  
  PRINT3(RockRadius, RockHalfLen, RockDensity);

  PRINT4(TargetAreaZ0, TargetAreaLength, TargetAreaHeight, TargetAreaWidth);
  PRINT1(TargetAreaGEANTmat);

  PRINT1(TargetNtarget);
  PRINT5(fConstructTarget, TargetShape, TargetX0, TargetY0, TargetZ0);
  PRINT5(TargetDxdz, TargetDydz, TargetLength, TargetRadius, TargetWidth);
  PRINT4(TargetHeight, TargetA, TargetDensity, TargetZ);
  PRINT1(TargetGEANTmat);

  PRINT5(HPBaffle, HPBaffleGEANTMat, HPBaffleX0, HPBaffleY0, HPBaffleZ0);
  PRINT4(HPBaffleDXDZ, HPBaffleDYDZ, HPBaffleLength, HPBaffleRin);
  PRINT1(HPBaffleRout);

  PRINT5(TunnelZ0, TunnelRadius, TunnelLength, TunnelA, TunnelZ);
  PRINT2(TunnelGEANTmat, BeamAngle);
  
  PRINT4(DecayPipeZ0, DecayPipeRadius, DecayPipeLength, DecayPipeFWinThick);
  PRINT4(DecayPipeEWinThick, DecayPipeWallThick, DecayPipeA, DecayPipeZ);
  PRINT3(DecayPipeGEANTmat, DecayPipeFWinmat, DecayPipeEWinmat);

  PRINT6(ShieldX0, ShieldY0, ShieldZ0, ShieldDxdz, ShieldDydz, ShieldLength);
  PRINT3(ShieldRout, ShieldRin, ShieldGEANTmat);

  PRINT6(BlockNblock, BlockX0, BlockY0, BlockZ0, BlockDxdz, BlockDydz);
  PRINT4(BlockLength, BlockHdx, BlockHdy, BlockGeantMaterial);

  PRINT3(HadrBox_width, HadrBox_height, HadrBox_length);

  PRINT6(PhornNphorn, PhornZ1, PhornZ2, PhornNpoint, PhornAin, PhornBin);
  PRINT6(PhornCin, PhornAout, PhornBout, PhornCout, PhornROCin, PhornROCout);
  PRINT6(PhornThickFront, PhornThickEnd, PhornX0, PhornY0, PhornZ0, PhornDXDZ);
  PRINT3(PhornDYDZ, PhornCurrent, PhornGEANTmat); //, PhornName);

  PRINT7(nNear, nFar, xdet_near, ydet_near, zdet_near, xdet_far, ydet_far);
  PRINT1(zdet_far);

  PRINT1(fHornCurrent);

  G4cout << "-------------------------" << G4endl;
}

//helper functions for printAll...

//-------------------------------------------------------------------------------
template<class T>
void LBNEDataInput::print(const G4String& name, const T& value) {
  G4cout << "    " << name << ": " << value << G4endl;
}

//-------------------------------------------------------------------------------
template<class T>
void LBNEDataInput::print(const G4String& name, const std::vector<T>& value) {
  G4cout << "    " << name << ": [ ";
  for(size_t i = 0; i < value.size(); i++) G4cout << value[i] << ", ";
  G4cout << " ]" << G4endl;
}


//-------------------------------------------------------------------------------
//possible simulations that are currently enabled are below
void LBNEDataInput::SetSimulation(G4String &sim) 
{ 
   //
   // In NumiRunAction the fSimulation is Checked and fOKToRun 
   // is set to true/false. Then in NumiRunManager fOKToRun is used 
   // to determine whether or not to run.
   //
   //

   G4bool knownSim = false;
   
   for(vstring_t::const_iterator sit = fSimulationVec.begin(); sit != fSimulationVec.end(); ++sit)
   {
      if (sim == *sit) { knownSim = true; break;}
   }

   if(knownSim)
   {
      fSimulation = sim;
   }
   else
   { 
      std::cout << "LBNEDataInput::SetSimulation - PROBLEM: Simulation "
		<< "\"" << sim << "\"" << " unknown. Currently "
		<< "implemented simulations are " << std::endl;

      for(vstring_t::const_iterator sit = fSimulationVec.begin(); sit != fSimulationVec.end(); ++sit)
      {
	 std::cout << "     " << *sit << std::endl;
      }
      std::cout << std::endl;
   }
   
}
