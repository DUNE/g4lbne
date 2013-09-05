#ifndef LBNERunActionMessenger_h
#define LBNERunActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4ios.hh"

class LBNERunAction;
class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithoutParameter;

class LBNERunActionMessenger: public G4UImessenger
{
public:
  LBNERunActionMessenger(LBNERunAction* );
  ~LBNERunActionMessenger();
  
  void SetNewValue(G4UIcommand* ,G4String );
 
private:

   LBNERunAction*              runAction;
   //
   //NuMI/rndm
   //
   G4UIdirectory*              RndmDir;

   G4UIcmdWithAString*         readRndmCmd;  
   
   G4UIcmdWithoutParameter*    showRndmCmd;
   
   G4UIcmdWithAnInteger*       setRndmSeedCmd;
   //

   //
   //NuMI/run
   //
   G4UIdirectory*              LBNERunDir;
   
   G4UIcmdWithAString*         SimulationName;
   G4UIcmdWithAString*         InputNtpTreeName;
   G4UIcmdWithAString*         InputNtpFileName;
   
   G4UIcmdWithAnInteger*       DebugLevel;
   G4UIcmdWithAnInteger*       setNEvents;   
   G4UIcmdWithAnInteger*       setRunID;
   
   G4UIcmdWithADoubleAndUnit*  FlukaNumiTgtShiftCmd; 
   G4UIcmdWithADoubleAndUnit*  KillTrackingThreshold;
   G4UIcmdWithADoubleAndUnit*  setStepLimit;

   G4UIcmdWithABool*           useNImpWeight;
   G4UIcmdWithABool*           useFlukaInput;
   G4UIcmdWithABool*           useMarsInput;
   G4UIcmdWithABool*           KillTracking;
   G4UIcmdWithABool*           GenerateOutputNTuple;

   //


   //
   //NuMI/output/
   //
   G4UIdirectory*              LBNEOutputDir;

   G4UIcmdWithAString*         setNuNtupleFile; 
   G4UIcmdWithAString*         setASCIIFile; 

   G4UIcmdWithABool*           CreateOutput;
   G4UIcmdWithABool*           outputASCIIFile;


   //

   
      
};

#endif
