//----------------------------------------------------------------------
// $Id
//----------------------------------------------------------------------

#include "LBNERunActionMessenger.hh"

#include "LBNERunAction.hh"
#include "LBNEDataInput.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4ios.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "G4RunManager.hh"

LBNERunActionMessenger::LBNERunActionMessenger(LBNERunAction* RA)
  :runAction (RA)
{
  LBNEData=LBNEDataInput::GetLBNEDataInput();

  //
  //LBNE/rndm
  //
  RndmDir = new G4UIdirectory("/LBNE/rndm/");
  RndmDir->SetGuidance("Rndm status control.");
  
  readRndmCmd = new G4UIcmdWithAString("/LBNE/rndm/read",this);
  readRndmCmd->SetGuidance("get rndm status from an external file.");
  readRndmCmd->SetParameterName("fileName",true);
  readRndmCmd->SetDefaultValue ("");
  readRndmCmd->AvailableForStates(G4State_PreInit,G4State_Idle);  

  showRndmCmd = new G4UIcmdWithoutParameter("/LBNE/rndm/show",this);
  showRndmCmd->SetGuidance("show rndm status.");
  showRndmCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  setRndmSeedCmd = new G4UIcmdWithAnInteger("/LBNE/rndm/setRndmSeed",this);
  setRndmSeedCmd->SetGuidance("set rndm seed.");
  setRndmSeedCmd->SetParameterName("rndmSeed",true);
  setRndmSeedCmd->SetDefaultValue (0);
  setRndmSeedCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  //

  //
  //LBNE/run
  //
  LBNERunDir = new G4UIdirectory("/LBNE/run/");
  LBNERunDir->SetGuidance("LBNE run management");


  SimulationName = new G4UIcmdWithAString("/LBNE/run/Simulation",this);
  SimulationName->SetGuidance("set external (fluka/mars) ntuple file name");
  SimulationName->SetParameterName("Simulation",true);
  SimulationName->AvailableForStates(G4State_PreInit,G4State_Idle);

  InputNtpTreeName = new G4UIcmdWithAString("/LBNE/run/InputNtpTreeName",this);
  InputNtpTreeName->SetGuidance("Set the name of the ROOT Tree in the Input Ntuple");
  InputNtpTreeName->SetParameterName("InputNtpTreeName",true);
  InputNtpTreeName->AvailableForStates(G4State_PreInit,G4State_Idle);

  InputNtpFileName = new G4UIcmdWithAString("/LBNE/run/InputNtpFileName",this);
  InputNtpFileName->SetGuidance("set input (fluka/mars) ntuple file name");
  InputNtpFileName->SetParameterName("extNtupleFileName",true);
  InputNtpFileName->SetDefaultValue (LBNEData->GetInputNtpFileName());
  InputNtpFileName->AvailableForStates(G4State_PreInit,G4State_Idle);

  DebugLevel = new G4UIcmdWithAnInteger("/LBNE/run/DebugLevel",this);
  DebugLevel->SetGuidance("Output some debugging info. Level sets the importance of the information printed.");
  DebugLevel->SetParameterName("DebugLevel",true);
  DebugLevel->SetDefaultValue (LBNEData->GetDebugLevel());
  DebugLevel->AvailableForStates(G4State_PreInit,G4State_Idle);

  setNEvents = new G4UIcmdWithAnInteger("/LBNE/run/NEvents",this);
  setNEvents->SetGuidance("Set the number of Events to process");
  setNEvents->SetParameterName("NEvents",true);
  setNEvents->SetDefaultValue (LBNEData->GetNEvents());
  setNEvents->AvailableForStates(G4State_PreInit,G4State_Idle);

  setRunID = new G4UIcmdWithAnInteger("/LBNE/run/setRunID",this);
  setRunID->SetGuidance("set run ID.");
  setRunID->SetParameterName("run ID number",true);
  setRunID->SetDefaultValue (0);
  setRunID->AvailableForStates(G4State_PreInit,G4State_Idle); 

  FlukaNumiTgtShiftCmd = new G4UIcmdWithADoubleAndUnit("/LBNE/run/AddExtraFlukaNumiTargetZShift",this);
  FlukaNumiTgtShiftCmd->SetGuidance("ONLY effects FLUKA05 Numi files. Add an extra shift in z to Fluka05 Numi Target files.");
  FlukaNumiTgtShiftCmd->SetParameterName("Size",false);
  FlukaNumiTgtShiftCmd->SetUnitCategory("Length");
  FlukaNumiTgtShiftCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  KillTrackingThreshold = new G4UIcmdWithADoubleAndUnit("/LBNE/run/KillTrackingThreshold",this);
  KillTrackingThreshold->SetGuidance("Sets Kill Tracking on or off");
  KillTrackingThreshold->SetParameterName("KillTrackingThreshold",true);
  KillTrackingThreshold->SetDefaultValue(LBNEData->GetKillTrackingThreshold());
  KillTrackingThreshold->AvailableForStates(G4State_PreInit,G4State_Idle);

  setStepLimit = new G4UIcmdWithADoubleAndUnit("/LBNE/run/setStepLimit",this);
  setStepLimit->SetGuidance("Maximum step size in magnetized horns");
  setStepLimit->SetParameterName("setStepLimit",true);
  setStepLimit->SetDefaultValue(0.0);
  setStepLimit->SetRange("setStepLimit>=0.");
  setStepLimit->SetUnitCategory("Length");
  setStepLimit->AvailableForStates(G4State_PreInit,G4State_Idle);

  
  useNImpWeight = new G4UIcmdWithABool("/LBNE/run/useNImpWeight",this);
  useNImpWeight->SetGuidance("use importance weighting (true/false)");
  useNImpWeight->SetParameterName("NImpWeight",true);
  useNImpWeight->SetDefaultValue (LBNEData->GetImpWeightOn());
  useNImpWeight->AvailableForStates(G4State_PreInit,G4State_Idle);

  useFlukaInput = new G4UIcmdWithABool("/LBNE/run/useFlukaInput",this);
  useFlukaInput->SetGuidance("use fluka input ntuple");
  useFlukaInput->SetParameterName("useFlukaInput",true);
  useFlukaInput->SetDefaultValue (LBNEData->GetUseFlukaInput());
  useFlukaInput->AvailableForStates(G4State_PreInit,G4State_Idle);

  useMarsInput = new G4UIcmdWithABool("/LBNE/run/useMarsInput",this);
  useMarsInput->SetGuidance("use mars input ntuple");
  useMarsInput->SetParameterName("useMarsInput",true);
  useMarsInput->SetDefaultValue (LBNEData->GetUseMarsInput());
  useMarsInput->AvailableForStates(G4State_PreInit,G4State_Idle);

  KillTracking=new G4UIcmdWithABool("/LBNE/run/KillTracking",this);
  KillTracking->SetGuidance("Sets Kill Tracking on or off");
  KillTracking->SetParameterName("KillTracking",true);
  KillTracking->SetDefaultValue(LBNEData->GetKillTracking());
  KillTracking->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  //


  //
  //LBNE/output/
  //
  LBNEOutputDir = new G4UIdirectory("/LBNE/output/");
  LBNEOutputDir->SetGuidance("LBNE output management");

  setNuNtupleFile = new G4UIcmdWithAString("/LBNE/output/OutputNtpFileName",this);
  setNuNtupleFile->SetGuidance("set output ntuple file name");
  setNuNtupleFile->SetParameterName("fileName",true);
  setNuNtupleFile->SetDefaultValue (LBNEData->GetOutputNtpFileName());
  setNuNtupleFile->AvailableForStates(G4State_PreInit,G4State_Idle);

  setASCIIFile = new G4UIcmdWithAString("/LBNE/output/setASCIIFile",this);
  setASCIIFile->SetGuidance("set ASCII file name");
  setASCIIFile->SetParameterName("fileName",true);
  setASCIIFile->SetDefaultValue (LBNEData->GetASCIIOutputFileName());
  setASCIIFile->AvailableForStates(G4State_PreInit,G4State_Idle);

  CreateOutput = new G4UIcmdWithABool("/LBNE/output/CreateOutput",this);
  CreateOutput->SetGuidance("Create Output Ntuple (true/false)");
  CreateOutput->SetParameterName("CreateOutput",true);
  CreateOutput->AvailableForStates(G4State_PreInit,G4State_Idle);

  outputASCIIFile = new G4UIcmdWithABool("/LBNE/output/outputASCIIFile",this);
  outputASCIIFile->SetGuidance("output ASCII file (true/false)");
  outputASCIIFile->SetParameterName("outputASCIIFile",true);
  outputASCIIFile->SetDefaultValue (LBNEData->GetCreateASCIIOutput());
  outputASCIIFile->AvailableForStates(G4State_PreInit,G4State_Idle);

  
  //


}

LBNERunActionMessenger::~LBNERunActionMessenger()
{
   delete         RndmDir;
   delete         readRndmCmd;  
   delete         showRndmCmd;
   delete         setRndmSeedCmd;
   
   
   delete         LBNERunDir;
   delete         SimulationName;
   delete         InputNtpTreeName;
   delete         InputNtpFileName;
   delete         DebugLevel;
   delete         setNEvents;   
   delete         setRunID;
   delete         FlukaNumiTgtShiftCmd; 
   delete         KillTrackingThreshold;
   delete         setStepLimit;
   delete         useNImpWeight;
   delete         useFlukaInput;
   delete         useMarsInput;
   delete         KillTracking;


   delete         LBNEOutputDir;
   delete         setNuNtupleFile;
   delete         setASCIIFile;
   delete         CreateOutput;
   delete         outputASCIIFile;


}

void LBNERunActionMessenger::SetNewValue(G4UIcommand* command,G4String newValues)
{

   LBNEDataInput *LBNEData = LBNEDataInput::GetLBNEDataInput();
   
   //
   //LBNE/rndm
   //
   if (command == readRndmCmd)
   { 
      G4cout << "\n---> rndm status restored from file: " << newValues << G4endl;
      G4String rndmFile="rndm/";
      rndmFile.append(newValues);
      CLHEP::HepRandom::restoreEngineStatus(rndmFile);
   }   
   
   if (command == showRndmCmd)
   { 
      CLHEP::HepRandom::showEngineStatus();
   }  
   
   if (command == setRndmSeedCmd)
   { 
      CLHEP::HepRandom::setTheSeed(setRndmSeedCmd->GetNewIntValue(newValues));
   }
   //


   //
   //LBNE/run
   //
   if (command == SimulationName)
   {
      LBNEData->SetSimulation(newValues);
   }
   if (command == InputNtpTreeName)
   {
      LBNEData->SetInputNtpTreeName(newValues);
   }
   if (command == InputNtpFileName)
   {
      LBNEData->SetInputNtpFileName(newValues);
   }

   if (command == DebugLevel)
   {
      LBNEData->SetDebugLevel(DebugLevel->GetNewIntValue(newValues));
   }
   if (command == setNEvents)
   {
      LBNEData->SetNEvents(setNEvents->GetNewIntValue(newValues));
   }   
   if (command == setRunID)
   {
      G4RunManager* runManager = G4RunManager::GetRunManager();
      runManager->SetRunIDCounter(setRunID->GetNewIntValue(newValues));
   }

   if ( command == FlukaNumiTgtShiftCmd ) 
   {
      LBNEData->SetExtraFlukaNumiTargetZShift(FlukaNumiTgtShiftCmd->GetNewDoubleValue(newValues));
   }
   if (command== KillTrackingThreshold)
   {
      LBNEData->SetKillTrackingThreshold(KillTrackingThreshold->GetNewDoubleValue(newValues));
   }
   if (command == setStepLimit)
   {
      LBNEData->SetStepLimit(setStepLimit->GetNewDoubleValue(newValues));
   }  

   if (command == useNImpWeight)    
   { 
      LBNEData->SetNImpWeightOn(useNImpWeight->GetNewBoolValue(newValues)); 
   }
   if (command == useFlukaInput)         
   { 
      LBNEData->SetUseFlukaInput(useFlukaInput->GetNewBoolValue(newValues));
   }
   if (command == useMarsInput)          
   { 
      LBNEData->SetUseMarsInput(useMarsInput->GetNewBoolValue(newValues)); 
   }
   if (command== KillTracking)
   {
     LBNEData->SetKillTracking(KillTracking->GetNewBoolValue(newValues));
   }
  
   //
   
   //
   //LBNE/output
   //
   if (command == setNuNtupleFile)
   {
      LBNEData->SetOutputNtpFileName(newValues);
   }
   if (command == setASCIIFile)
   {
      LBNEData->SetASCIIOutputFileName(newValues);
   }

   if (command == CreateOutput)
   {
      LBNEData->SetCreateOutput(CreateOutput->GetNewBoolValue(newValues));
   }
   if (command == outputASCIIFile)
   {
      LBNEData->SetCreateASCIIOutput(outputASCIIFile->GetNewBoolValue(newValues));
   }

   //


  
  
  
}

