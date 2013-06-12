// #include "LBNESubVolume.hh" Independent from this. 
#include "LBNESurveyor.hh"
#include "Randomize.hh"

LBNESurveyorMessenger::LBNESurveyorMessenger():
fFullName(std::string("")),
fPoint(0),
fSetPositionFromToleranceCmd(0),
fToleranceCmd(0),
fPositionCmd(0)
{

}

LBNESurveyorMessenger::LBNESurveyorMessenger(LBNESurveyedPt* aPts, const G4String &aName, const G4String &description):
fFullName(aName),
fPoint(aPts),
fSetPositionFromToleranceCmd(0),
fToleranceCmd(0),
fPositionCmd(0)
{
  std::string cmdPathTop=std::string(fFullName) + std::string("/");

  std::string cmdPathLTol(cmdPathTop); cmdPathLTol += std::string("SetByTolerance");   
  fSetPositionFromToleranceCmd = new G4UIcmdWithABool(cmdPathLTol.c_str(), this );
  fSetPositionFromToleranceCmd->SetParameterName("SetByTolerance",true);
  fSetPositionFromToleranceCmd->SetDefaultValue(true);

// Units are always mm, default units in geant4. 

  std::string cmdPathTol(cmdPathTop); cmdPathTol += std::string("Tolerance");   
  fToleranceCmd = new G4UIcmdWith3Vector(cmdPathTol.c_str(), this );
  fToleranceCmd->SetParameterName("ToleranceX", "ToleranceY", "ToleranceZ", true);
  
  // Specific default values needs to be defined here, mechanical by mechanical elements..
  // based on the name.. 
  
  fToleranceCmd->SetDefaultValue(G4ThreeVector(1.0e-6, 1.0e-6, 1.0e-6)); // one nanometer. 

  std::string cmdPathPos(cmdPathTop); cmdPathPos += std::string("Position");   
  fPositionCmd = new G4UIcmdWith3Vector(cmdPathPos.c_str(), this );
  fPositionCmd->SetParameterName("PositionX", "PositionY", "PositionZ", true);
  fPositionCmd->SetDefaultValue(G4ThreeVector(0., 0., 0.));
//
  fPositionCmd->SetGuidance(description.c_str());
// Furthermore...

  if (aName.find("UpstreamRightPin") != std::string::npos) 
    fToleranceCmd->SetGuidance(
    " The elevation (Y) coordinate is not very accurate, however, the horizontal (X) was defined to 20 microns " ); 
  
}
LBNESurveyorMessenger::~LBNESurveyorMessenger() {

  //
  // The pointer to the Surveyed point is not owned by this messenger! 
  //
  delete fSetPositionFromToleranceCmd;
  delete fToleranceCmd;
  delete fPositionCmd;

}

void LBNESurveyorMessenger::SetNewValue(G4UIcommand* cmd, G4String newValue)
{
 if (cmd == fSetPositionFromToleranceCmd) {
    G4UIcmdWithABool *cmdB = reinterpret_cast <G4UIcmdWithABool*> (cmd);  
    fPoint->SetPositionByTolerance(cmdB->GetNewBoolValue(newValue.c_str()));
 }
 if (cmd == fToleranceCmd) {
   G4UIcmdWith3Vector *cmdB = reinterpret_cast <G4UIcmdWith3Vector*> (cmd);  
   fPoint->SetTolerance(cmdB->GetNew3VectorValue(newValue.c_str()));
 }
 if (cmd == fPositionCmd) {
   G4UIcmdWith3Vector *cmdB = reinterpret_cast <G4UIcmdWith3Vector*> (cmd);  
   fPoint->SetPosition(cmdB->GetNew3VectorValue(newValue.c_str()));
 }
}

LBNESurveyedPt::LBNESurveyedPt():
fName(""), 
fPositionSetByTolerance(false),
fTolerance(1.0e-6, 1.0e-6, 1.0e-6),
fPosition(0., 0., 0.) // messenger undefined if point is not named. 
{ } 

LBNESurveyedPt::LBNESurveyedPt(const std::string &aName):
fName(aName),
fPositionSetByTolerance(false),
fTolerance(1.0e-6, 1.0e-6, 1.0e-6),
fPosition(0., 0., 0.)
{
  defineMessenger();
}

LBNESurveyedPt::LBNESurveyedPt(const std::string &aName, const G4ThreeVector &tolerance) : 
fName(aName), 
fPositionSetByTolerance(true),
fTolerance(tolerance),
fPosition(0., 0., 0.)
{
  defineMessenger();
}

void LBNESurveyedPt::defineMessenger() {

  std::string descr("");
  std::string fullName("/LBNE/Surveyor/"); 
  fullName += fName;
  if (fName.find("Horn1") != std::string::npos) {
    if (fName.find("UpstreamLeftPin") != std::string::npos) 
      descr += std::string(" Surveyed Position of the upstream, left side alignment pin for Horn1 ") ;
    if (fName.find("UpstreamRightPin") != std::string::npos) 
      descr += std::string(" Surveyed Position of the upstream, side side alignment pin for Horn1 ") ;
    if (fName.find("DowstreamAnchor") != std::string::npos) 
      descr += std::string(" Surveyed Position downstream fixed point for Horn1 ") ;
  }
  fMessenger = LBNESurveyorMessenger(this, fullName, descr); 
}


void LBNESurveyedPt::SetPositionByTolerance(bool t) {
  fPositionSetByTolerance = t;
  if (!t) return;
  //
  // Set the position.  Assume a flat distribution within tolerance.. for now... 
  //
  for (size_t k=0; k!=3; ++k) fPosition[k] = 2.0*fTolerance[k]*(G4RandFlat::shoot() -0.5);
}  

LBNESurveyor* LBNESurveyor::fInstance = 0;
LBNESurveyor* LBNESurveyor::GetInstance() {
  if (fInstance == 0) fInstance = new LBNESurveyor();
  return fInstance;
}

LBNESurveyor::LBNESurveyor()
{
}
void LBNESurveyor::SetIt() { // Randomize again, if need be. 

  for(std::vector<LBNESurveyedPt>::iterator itPt=fData.begin(); itPt != fData.end(); ++itPt) {
    if (itPt->IsPositionSetByTolerance()) itPt->SetPositionByTolerance(true);
  }
  SetThings(); // a place holder for now.. 
  const double toleranceTarget= 0.1; // This parameter will most likely have a G4Messenger associated to it. 
  TieTargetSegments(toleranceTarget);
}
void LBNESurveyor::SetThings() { 
 // blank for now. 

}
bool  LBNESurveyor::TieTargetSegments(double toleranceTarget) {

 // standlone check on the surveyed geometry, prior to implement the G4 geometry. 

 return false; // Assume problematic for now..
}

