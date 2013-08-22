//#include <fstream>

#include "LBNEDetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UnitsTable.hh"
#include "LBNEPlacementMessenger.hh"

LBNEPlacementMessenger* LBNEPlacementMessenger::fInstance = 0;
LBNEPlacementMessenger* LBNEPlacementMessenger::Instance() {
  if (fInstance == 0) fInstance = new LBNEPlacementMessenger();
  return fInstance;
}

LBNEPlacementMessenger::~LBNEPlacementMessenger() {

//  delete fWaterLayerThickInHorn;  // They belong to 
//  delete fDecayPipeLength;
//  delete fHorn1Length;

}

LBNEPlacementMessenger::LBNEPlacementMessenger()
 {

//   std::cerr << " LBNEPlacementMessenger::LBNEPlacementMessenger, contructor, starts " << std::endl;
   LBNEVolumePlacements* volP=LBNEVolumePlacements::Instance();
   fDecayPipeLength  = new G4UIcmdWithADoubleAndUnit("/LBNE/det/decayPipeLength",this);
   fDecayPipeLength->SetGuidance("Length of the decay Pipe");
   fDecayPipeLength->SetParameterName("decayPipeLength",true);
   double value = volP->GetDecayPipeLength();
    fDecayPipeLength->SetDefaultValue (value);
   fDecayPipeLength->SetDefaultUnit ("m");
   fDecayPipeLength->SetUnitCandidates ("cm m");
   fDecayPipeLength->AvailableForStates(G4State_PreInit, G4State_Idle);

   fWaterLayerThickInHorn  = new G4UIcmdWithADoubleAndUnit("/LBNE/det/waterThickInHorn",this);
   fWaterLayerThickInHorn->SetGuidance("Water Thicknes on the inner conductor of the horn");
   fWaterLayerThickInHorn->SetParameterName("waterThickInHorn",true);
   fWaterLayerThickInHorn->SetDefaultValue (0.);
   fWaterLayerThickInHorn->SetDefaultUnit ("mm");
   fWaterLayerThickInHorn->SetUnitCandidates ("mm cm m");
   fWaterLayerThickInHorn->AvailableForStates(G4State_PreInit, G4State_Idle);
   { 
     fHorn1Length  = new G4UIcmdWithADoubleAndUnit("/LBNE/det/Horn1Length",this);
     G4String guidance("Length of Horn1.\n  ");
     guidance += std::string(" The length of the outer conductor, excludes the upstream transition inner/outer, \n");
     guidance += std::string("  and flanges with bolts. If extended by user, it is assumed that conductor are extended.. " ); 
     guidance += std::string("  NUMI Horn1, FNAL Drawing number 8875. 112-ME-363092 " ); 
     fHorn1Length->SetGuidance(guidance);
     fHorn1Length->SetParameterName("Horn1Length",true);
     double value = volP->GetHorn1Length(); //  
     
     SetMyUnitsAndConditions(fHorn1Length, value);
   }
   {
     fTargetSLengthGraphite = new G4UIcmdWithADoubleAndUnit("/LBNE/det/GraphiteTargetLength", this);
     G4String guidance("Length of the Graphite Target.\n  ");
     guidance += std::string(" Note: the length of the indiviudal fins are fixed to 2 cm. So, we will re-adjusted  \n");
     guidance += std::string(" abit to get an integer number of fins. For details, see LBNE-Doc-6100  " ); 
     fTargetSLengthGraphite->SetGuidance(guidance);
     fTargetSLengthGraphite->SetParameterName("GraphiteTargetLength",true);
     double value = volP->GetTargetSLengthGraphite(); //  
     
     SetMyUnitsAndConditions(fTargetSLengthGraphite, value);
     
   }
   {
     fTargetLengthIntoHorn = new G4UIcmdWithADoubleAndUnit("/LBNE/det/TargetLengthIntoHorn", this);
     G4String guidance("Length of the Graphite Target into the horn.\n  ");
     guidance += std::string(" More specifically, the length of graphite from MCZERO to the dowstream tip of graphite. \n");
     guidance += std::string(" FNAL Drawing number 8875. 112-ME-363092  " ); 
     fTargetLengthIntoHorn->SetGuidance(guidance);
     fTargetLengthIntoHorn->SetParameterName("TargetLengthIntoHorn",true);
     double value = volP->GetTargetLengthIntoHorn(); //  
     
     SetMyUnitsAndConditions(fTargetLengthIntoHorn, value);
     
   }
   
   
    std::cerr << " LBNEPlacementMessenger::LBNEPlacementMessenger, contructor, Ends " << std::endl;
  
}
// Just to avoid code bloat.. 

void LBNEPlacementMessenger::SetMyUnitsAndConditions(G4UIcmdWithADoubleAndUnit *cmd, double value) {

    cmd->SetDefaultValue (value);
    cmd->SetDefaultUnit ("m");
    cmd->SetUnitCandidates ("cm m");
    cmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}
void LBNEPlacementMessenger::SetNewValue(G4UIcommand* command,  G4String newValue) {
   
   LBNEVolumePlacements* volP=LBNEVolumePlacements::Instance();
   if (command == fWaterLayerThickInHorn) {
     G4UIcmdWithADoubleAndUnit* cmdWD = dynamic_cast<G4UIcmdWithADoubleAndUnit*> (command);
     volP->SetWaterLayerThickInHorns(cmdWD->GetNewDoubleValue(newValue));
//     std::cerr << " Changed WaterLayerThickInHorns , o.k. " << std::endl;  // exit(2);
   }
   if (command == fDecayPipeLength) {
     G4UIcmdWithADoubleAndUnit* cmdWD = dynamic_cast<G4UIcmdWithADoubleAndUnit*> (command);
     volP->SetDecayPipeLength(cmdWD->GetNewDoubleValue(newValue));
   }
   if (command == fHorn1Length) {
     G4UIcmdWithADoubleAndUnit* cmdWD = dynamic_cast<G4UIcmdWithADoubleAndUnit*> (command);
     volP->SetHorn1Length(cmdWD->GetNewDoubleValue(newValue));
   }
    
   if (command == fTargetSLengthGraphite) {
     G4UIcmdWithADoubleAndUnit* cmdWD = dynamic_cast<G4UIcmdWithADoubleAndUnit*> (command);
     volP->SetTargetSLengthGraphite(cmdWD->GetNewDoubleValue(newValue));
     volP->SegmentTarget();
   }
   if (command == fTargetLengthIntoHorn) {
     G4UIcmdWithADoubleAndUnit* cmdWD = dynamic_cast<G4UIcmdWithADoubleAndUnit*> (command);
     volP->SetTargetSLengthGraphite(cmdWD->GetNewDoubleValue(newValue));
     volP->SegmentTarget();
   }
   

}
