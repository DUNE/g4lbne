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
   fDecayPipeLength->AvailableForStates(G4State_PreInit);
   {
   fDecayPipeLongPosition  = new G4UIcmdWithADoubleAndUnit("/LBNE/det/decayPipeLongPosition",this);
   fDecayPipeLongPosition->SetGuidance("Longitudinal Position of the entrance window of the decay pipe with respect to target");
   fDecayPipeLongPosition->SetParameterName("decayPipeLongPosition",true);
   double value = volP->GetDecayPipeLongPosition();
    fDecayPipeLongPosition->SetDefaultValue (value);
   fDecayPipeLongPosition->SetDefaultUnit ("m");
   fDecayPipeLongPosition->SetUnitCandidates ("m");
   fDecayPipeLongPosition->AvailableForStates(G4State_PreInit);
   }
   {
   fDecayPipeRadius  = new G4UIcmdWithADoubleAndUnit("/LBNE/det/decayPipeRadius",this);
   fDecayPipeRadius->SetGuidance("Radius of the decay Pipe");
   fDecayPipeRadius->SetParameterName("decayPipeRadius",true);
   double value = volP->GetDecayPipeRadius();
    fDecayPipeRadius->SetDefaultValue (value);
   fDecayPipeRadius->SetDefaultUnit ("m");
   fDecayPipeRadius->SetUnitCandidates ("cm m");
   fDecayPipeRadius->AvailableForStates(G4State_PreInit);
   }
   {
   fDecayPipeUpstreamWindowThickness  = new G4UIcmdWithADoubleAndUnit("/LBNE/det/decayPipeUsptreamWindowThickness",this);
   fDecayPipeUpstreamWindowThickness->SetGuidance("Thickness of the upstream window of the decay pipe.");
   fDecayPipeUpstreamWindowThickness->SetParameterName("decayPipeUpstreamWindowThickness",true);
   double value = volP->GetDecayPipeUpstrWindowThick();
    fDecayPipeUpstreamWindowThickness->SetDefaultValue (value);
   fDecayPipeUpstreamWindowThickness->SetDefaultUnit ("mm");
   fDecayPipeUpstreamWindowThickness->SetUnitCandidates ("cm");
   fDecayPipeUpstreamWindowThickness->AvailableForStates(G4State_PreInit);
   }
   {
   fDecayPipeGas  = new G4UIcmdWithAString("/LBNE/det/decayPipeGas",this);
   fDecayPipeGas->SetGuidance("Gas inside of the decay Pipe. Only two options so far, Air or Helium ");
   fDecayPipeGas->SetParameterName("decayPipeGas",true);
   fDecayPipeGas->SetDefaultValue (G4String("Air"));
   fDecayPipeGas->AvailableForStates(G4State_PreInit);
   }

   fWaterLayerThickInHorn  = new G4UIcmdWithADoubleAndUnit("/LBNE/det/waterThickInHorn",this);
   fWaterLayerThickInHorn->SetGuidance("Water Thicknes on the inner conductor of the horn");
   fWaterLayerThickInHorn->SetParameterName("waterThickInHorn",true);
   fWaterLayerThickInHorn->SetDefaultValue (0.);
   fWaterLayerThickInHorn->SetDefaultUnit ("mm");
   fWaterLayerThickInHorn->SetUnitCandidates ("mm cm m");
   fWaterLayerThickInHorn->AvailableForStates(G4State_PreInit);
   { 
     fHorn1Length  = new G4UIcmdWithADoubleAndUnit("/LBNE/det/Horn1Length",this);
     G4String guidance("Length of Horn1.\n  ");
     guidance += std::string(" The length of the outer conductor, excludes the upstream transition inner/outer, \n");
     guidance += std::string("  and flanges with bolts. If extended by user, it is assumed that conductor are extended.. " ); 
     guidance += std::string("  NUMI Horn1, FNAL Drawing number 8875. 112-ME-363092 " ); 
     guidance += std::string(" Obsolete data card, use /LBNE/det/Horn1LongRescale instead ");
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
     fTargetMaterial = new G4UIcmdWithAString("/LBNE/det/TargetMaterial", this);
     G4String guidance("Material for the target \n  ");
     guidance += std::string(" Note: Only Beryllium, Graphite and Aluminum accepted for now..   \n");
     guidance += std::string(" The physical length of the target is left unchanged upon use of this data card. \n  " );
     guidance += std::string(" Default value: graphite \n  " );      
     fTargetMaterial->SetGuidance(guidance);
     fTargetMaterial->SetParameterName("TargetMaterial",true);
     G4String value = volP->GetTargetMaterialName(); //  
     fTargetMaterial->SetDefaultValue(value);
   }
   {
     fTargetDensity = new G4UIcmdWithADoubleAndUnit("/LBNE/det/GraphiteTargetLength", this);
     G4String guidance("Density of the target. Only relevant for the graphite target. \n  ");
     fTargetDensity->SetGuidance(guidance);
     fTargetDensity->SetParameterName("GraphiteTargetLength",true);
     double value = volP->GetTargetDensity(); //  
     fTargetDensity->SetDefaultValue (value);
     fTargetDensity->SetDefaultUnit ("g/cm3");
     fTargetDensity->SetUnitCandidates ("g/cm3");
     fTargetDensity->AvailableForStates(G4State_PreInit);
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
     guidance += std::string(" FNAL Drawing number 8875. 112-ME-363096, 363097,... " ); 
     fHorn1RadialRescale->SetGuidance(guidance);
     fHorn1RadialRescale->SetParameterName("Horn1RadialRescale",true);
     fHorn1RadialRescale->SetDefaultValue(1.0);
    }
    { 
     fHorn1LongRescale = new G4UIcmdWithADouble("/LBNE/det/Horn1LongRescale", this);
     G4String guidance("A ratio between the actual lengths for this run over the nominal values for Horn1 \n  ");
     guidance += std::string(" FNAL Drawing number 8875. 112-ME-363096, 363097,...   " ); 
     guidance += std::string(" More specifically, all (excluding target elements in Horn1) will be rescale by  \n");
     guidance += std::string(" that factor. Suggested value for modification: no more than a few % change for start!..   \n");
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
  
   {
     fAbsorberGDMLFilename = new G4UIcmdWithAString("/LBNE/det/GDMLAbsorberFilename", this);
     G4String guidance("File name generated by MARS/ROOT describing the Hadron absorber at the end of the beam line. \n  ");
     guidance += std::string(" Path name can be relative, if the executable runs from the correct directory  \n");
     guidance += std::string(" Typically .../g4lbne \n  " );
     guidance += std::string(" Or, absolute i.e, /scratch/.../g4blne/gdml/lbne_absorber_112912.gdml \n  " );      
     guidance += std::string(" The translation of an (eventual) Unix environment variable is currently not implement \n  " );      
     fTargetMaterial->SetGuidance(guidance);
     fTargetMaterial->SetParameterName("GDMLAbsorberFilename",true);
     G4String value = volP->GetAbsorberGDMLFilename(); //  
     fTargetMaterial->SetDefaultValue(value);
   }  
    { 
     fInstallShield = new G4UIcmdWithABool("/LBNE/det/InstallShield", this);
     G4String guidance("If true (default) install beamline rotated rectangular chunck of steel surrounding the horns \n  ");
     fInstallShield->SetGuidance(guidance);
     fInstallShield->SetParameterName("InstallShieldAroundHorns",true);
     bool value = volP->GetDoInstallShield(); //  
     fInstallShield->SetDefaultValue(value);
   }
}
// Just to avoid code bloat.. 

void LBNEPlacementMessenger::SetMyUnitsAndConditions(G4UIcmdWithADoubleAndUnit *cmd, double value) {

    cmd->SetDefaultValue (value);
    cmd->SetDefaultUnit ("m");
    cmd->SetUnitCandidates ("cm m");
    cmd->AvailableForStates(G4State_PreInit);
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
   if (command == fDecayPipeRadius) {
     G4UIcmdWithADoubleAndUnit* cmdWD = dynamic_cast<G4UIcmdWithADoubleAndUnit*> (command);
     volP->SetDecayPipeRadius(cmdWD->GetNewDoubleValue(newValue));
   }
   if (command == fDecayPipeLongPosition) {
     G4UIcmdWithADoubleAndUnit* cmdWD = dynamic_cast<G4UIcmdWithADoubleAndUnit*> (command);
     volP->SetDecayPipeLongPosition(cmdWD->GetNewDoubleValue(newValue));
   }
   if (command == fDecayPipeUpstreamWindowThickness) {
     G4UIcmdWithADoubleAndUnit* cmdWD = dynamic_cast<G4UIcmdWithADoubleAndUnit*> (command);
     volP->SetDecayPipeUpstrWindowThick(cmdWD->GetNewDoubleValue(newValue));
   }
   if (command == fDecayPipeGas) {
     volP->SetDecayPipeGas(newValue);
   }
   if (command == fHorn1Length) {
     G4UIcmdWithADoubleAndUnit* cmdWD = dynamic_cast<G4UIcmdWithADoubleAndUnit*> (command);
     G4Exception("LBNEPlacementMessenger::SetNewValue ", " ", FatalErrorInArgument,
           " Obsolete data card, use Horn1LongRescale instead ");
     volP->SetHorn1Length(cmdWD->GetNewDoubleValue(newValue));
   }
    
   if (command == fTargetSLengthGraphite) {
     G4UIcmdWithADoubleAndUnit* cmdWD = dynamic_cast<G4UIcmdWithADoubleAndUnit*> (command);
     volP->SetTargetSLengthGraphite(cmdWD->GetNewDoubleValue(newValue));
     volP->SegmentTarget();
   }
   if (command == fTargetDensity) {
     G4UIcmdWithADoubleAndUnit* cmdWD = dynamic_cast<G4UIcmdWithADoubleAndUnit*> (command);
     volP->SetTargetDensity(cmdWD->GetNewDoubleValue(newValue));
   }
   if (command == fTargetMaterial) {
     std::cout << " !!! You have chosen to use " << newValue << " as target material " << std::endl;
     volP->SetTargetMaterialName(newValue);
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
   if (command == fAbsorberGDMLFilename) {
          volP->SetAbsorberGDMLFilename(newValue);
   }

}
