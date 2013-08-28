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
    {
     fHorn1RadialRescale = new G4UIcmdWithADouble("/LBNE/det/Horn1RadialRescale", this);
     G4String guidance("A ratio between the actual radii for this run over the nominal values for Horn1 \n  ");
     guidance += std::string(" More specifically, all (excluding target elements in Horn1) will be rescale by  \n");
     guidance += std::string(" that factor. Suggested value for modification: no more than 105 for a start!..   \n");
     guidance += std::string(" FNAL Drawing number 8875. 112-ME-363xxx  " ); 
     fHorn1RadialRescale->SetGuidance(guidance);
     fHorn1RadialRescale->SetParameterName("Horn1RadialRescale",true);
     fHorn1RadialRescale->SetDefaultValue(1.0);
    }
    { 
     fHorn1LongRescale = new G4UIcmdWithADouble("/LBNE/det/Horn1LongRescale", this);
     G4String guidance("A ratio between the actual lengths for this run over the nominal values for Horn1 \n  ");
     guidance += std::string(" More specifically, all (excluding target elements in Horn1) will be rescale by  \n");
     guidance += std::string(" that factor. Suggested value for modification: no more than 105 for a start!..   \n");
     guidance += std::string(" FNAL Drawing number 8875. 112-ME-363xxx  " ); 
     fHorn1LongRescale->SetGuidance(guidance);
     fHorn1LongRescale->SetParameterName("Horn1LongRescale",true);
     fHorn1LongRescale->SetDefaultValue(1.0);
   }
    {
     fHorn2RadialRescale = new G4UIcmdWithADouble("/LBNE/det/Horn2RadialRescale", this);
     G4String guidance("A ratio between the actual radii for this run over the nominal values for Horn2 \n  ");
     guidance += std::string(" More specifically, all (excluding target elements in Horn2) will be rescale by  \n");
     guidance += std::string(" that factor. Suggested value for modification: no more than 105 for a start!..   \n");
     guidance += std::string(" FNAL Drawing number 8875.112 MD-363383 " ); 
     fHorn2RadialRescale->SetGuidance(guidance);
     fHorn2RadialRescale->SetParameterName("Horn2RadialRescale",true);
     fHorn2RadialRescale->SetDefaultValue(1.0);
    }
    { 
     fHorn2LongRescale = new G4UIcmdWithADouble("/LBNE/det/Horn2LongRescale", this);
     G4String guidance("A ratio between the actual lengths for this run over the nominal values for Horn2 \n  ");
     guidance += std::string(" More specifically, all (excluding target elements in Horn2) will be rescale by  \n");
     guidance += std::string(" that factor. Suggested value for modification: no more than 105 for a start!..   \n");
     guidance += std::string(" FNAL Drawing number 8875.112 MD-363383 " ); 
     fHorn2LongRescale->SetGuidance(guidance);
     fHorn2LongRescale->SetParameterName("Horn2LongRescale",true);
     fHorn2LongRescale->SetDefaultValue(1.0);
   }
    { 
     fHorn2LongPosition = new G4UIcmdWithADoubleAndUnit("/LBNE/det/Horn2LongPosition", this);
     G4String guidance("The Z position of Horn2 with respect to the entrance of Horn1 (a.k.a MCZero) \n  ");
     fHorn2LongPosition->SetGuidance(guidance);
     fHorn2LongPosition->SetParameterName("Horn2LongPosition",true);
     double value = volP->GetHorn2LongPosition(); //  
     
     SetMyUnitsAndConditions(fHorn2LongPosition, value);
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
     volP->SetTargetLengthIntoHorn(cmdWD->GetNewDoubleValue(newValue));
     volP->SegmentTarget();
   }
   if (command == fHorn1RadialRescale) {
     G4UIcmdWithADouble* cmdWD = dynamic_cast<G4UIcmdWithADouble*> (command);
     volP->SetHorn1RadialRescale(cmdWD->GetNewDoubleValue(newValue));
     volP->RescaleHorn1Radially();
   }
   if (command == fHorn1LongRescale) {
     G4UIcmdWithADouble* cmdWD = dynamic_cast<G4UIcmdWithADouble*> (command);
     volP->SetHorn1LongRescale(cmdWD->GetNewDoubleValue(newValue));
     volP->RescaleHorn1Lengthwise();
   }
   if (command == fHorn2RadialRescale) {
     G4UIcmdWithADouble* cmdWD = dynamic_cast<G4UIcmdWithADouble*> (command);
     volP->SetHorn2RadialRescale(cmdWD->GetNewDoubleValue(newValue));
     volP->RescaleHorn2Radially();
   }
   if (command == fHorn2LongRescale) {
     G4UIcmdWithADouble* cmdWD = dynamic_cast<G4UIcmdWithADouble*> (command);
     volP->SetHorn2LongRescale(cmdWD->GetNewDoubleValue(newValue));
     volP->RescaleHorn2Lengthwise();
   }
    if (command == fHorn2LongPosition) {
     G4UIcmdWithADoubleAndUnit* cmdWD = dynamic_cast<G4UIcmdWithADoubleAndUnit*> (command);
     volP->SetHorn2LongPosition(cmdWD->GetNewDoubleValue(newValue));
   }
  

}
