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

//  std::cerr << " LBNESurveyorMessenger::LBNESurveyorMessenger for " << aName <<  std::endl;
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
    
//   std::cerr << " LBNESurveyorMessenger::LBNESurveyorMessenger for " << aName <<  " done " <<  std::endl;
  
}
LBNESurveyorMessenger::LBNESurveyorMessenger(LBNESurveyorMessenger const &other): G4UImessenger(other) {
  std::cerr << " LBNESurveyorMessenger::LBNESurveyorMessenger  copy constructor invoked... " << std::endl;
  fFullName = other.fFullName;
  fPoint = other.fPoint; // back pointer to the point in question... Could be null is this messenger is blank. 
  fSetPositionFromToleranceCmd = other.fSetPositionFromToleranceCmd;
  fToleranceCmd = other.fToleranceCmd;
  fPositionCmd = other.fPositionCmd;
  std::cerr << " fPositionCmd " << (void *) fPositionCmd << " from " << (void *) other.fPositionCmd << std::endl;
  // Test assignments.. 
  fPositionCmd->SetDefaultValue(G4ThreeVector(1.0e-12, 1.0e-13, 1.0e-15));
  std::cerr << " O.K. ! ... " << std::endl;
}
LBNESurveyorMessenger& LBNESurveyorMessenger::operator= (LBNESurveyorMessenger const &other) {
  
  std::cerr << " LBNESurveyorMessenger::LBNESurveyorMessenger  operator =... " << std::endl;
  // This assignment operator seems broken.  Suspect the base class not working right 
  fFullName = other.fFullName;
  fPoint = other.fPoint; // back pointer to the point in question... Could be null is this messenger is blank. 
  fSetPositionFromToleranceCmd = other.fSetPositionFromToleranceCmd;
  fToleranceCmd = other.fToleranceCmd;
  fPositionCmd = other.fPositionCmd;
  std::cerr << " fPositionCmd " << (void *) fPositionCmd << " from " << (void *) other.fPositionCmd << std::endl;
  // Test assignments.. 
  fPositionCmd->SetDefaultValue(G4ThreeVector(1.0e-12, 1.0e-13, 1.0e-15));
  std::cerr << " O.K. ! ... " << std::endl;
  return *this;
}


LBNESurveyorMessenger::~LBNESurveyorMessenger() {
 std::cerr << " LBNESurveyorMessenger::~LBNESurveyorMessenger invoked " << std::endl;
  //
  // The pointer to the Surveyed point is not owned by this messenger! 
  //
//  delete fSetPositionFromToleranceCmd;
//  delete fToleranceCmd;
//  delete fPositionCmd;
// Can't delete them, these are not smart pointers. 

}

void LBNESurveyorMessenger::SetNewValue(G4UIcommand* cmd, G4String newValue)
{
   std::cerr << " LBNESurveyorMessenger::SetNewValue, Position, And quit ! " << std::endl; exit(2);
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
   std::cerr << " About to displace " << fPoint->GetName() << " by " << cmdB->GetNew3VectorValue(newValue.c_str()) << std::endl;
   fPoint->SetPosition(cmdB->GetNew3VectorValue(newValue.c_str()));
   std::cerr << " And quit " << std::endl; exit(2);
 }
}

LBNESurveyedPt::LBNESurveyedPt():
fName(""), 
fPositionSetByTolerance(false),
fTolerance(1.0e-6, 1.0e-6, 1.0e-6),
fPosition(0., 0., 0.) // messenger undefined if point is not named. 
{} 

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

  /*   
  // This does not work: the G4UIcommand does not appropriate copy constructors. 
  // or assignment operator...
  std::string descr("Surveyed Position of the ");
  std::string fullName("/LBNE/Surveyor/"); 
  fullName += fName;
  descr += fName;
  if (fName.find("Horn1") != std::string::npos) {
    if (fName.find("UpstreamLeftBall") != std::string::npos) 
      descr += std::string(" Surveyed Position of the upstream, left side alignment Ball for Horn1 ") ;
    if (fName.find("UpstreamRightBall") != std::string::npos) 
      descr += std::string(" Surveyed Position of the upstream, side side alignment Ball for Horn1 ") ;
    if (fName.find("DowstreamAnchor") != std::string::npos) 
      descr += std::string(" Surveyed Position downstream fixed point for Horn1 ") ;
  }
  if (fName.find("TargetCanister") != std::string::npos) {
    descr += std::string(", "); 
    if (fName.find("Upstream") != std::string::npos) descr += " Upstream, ";
    else if (fName.find("ownstream") != std::string::npos) descr += " Downstream, ";
    if (fName.find("Left") != std::string::npos) descr += " left side ";
    else if (fName.find("Right") != std::string::npos) descr += " right side ";
    descr += std::string("alignment pin for the target canister ") ;
  }
  
  fMessenger = LBNESurveyorMessenger(this, fullName, descr); 
  */
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
LBNESurveyor* LBNESurveyor::Instance() {
  if (fInstance == 0) fInstance = new LBNESurveyor();
  return fInstance;
}

LBNESurveyor::LBNESurveyor()
{
  
//  std::cerr << " LBNESurveyor::LBNESurveyor, start ... " << std::endl;
  fAllMessenger = new LBNEAllSurveyorMessenger();
  SetThings();
}
void LBNESurveyor::SetIt() { // Randomize, perhaps.. Not commissioned.. 
/*
  for(std::vector<LBNESurveyedPt>::iterator itPt=fData.begin(); itPt != fData.end(); ++itPt) {
    if (itPt->IsPositionSetByTolerance()) itPt->SetPositionByTolerance(false);
  }
  SetThings(); // a place holder for now.. 
  const double toleranceTarget= 0.1; // This parameter will most likely have a G4Messenger associated to it. 
//  TieTargetSegments(toleranceTarget);
*/
}
void LBNESurveyor::SetThings() { 

//
// Setting real aligned placements by tolerance, i.e. assign random position of objects, was 
// viewed as "strange and not really needed" by Jim Hylen and Tom Junk. So, leave on the back burner
// and install a few points for only for the target, and Horn1 
//

   AddPoint(std::string("UpstreamLeftPinTargetCanister"));
   AddPoint(std::string("UpstreamRightPinTargetCanister"));
   AddPoint(std::string("DownstreamLeftPinTargetCanister"));
   AddPoint(std::string("DownstreamRightPinTargetCanister"));
//
   AddPoint(std::string("UpstreamLeftPinTargetHeTube"));
   AddPoint(std::string("UpstreamRightPinTargetHeTube"));
   AddPoint(std::string("DownstreamLeftPinTargetHeTube"));
   AddPoint(std::string("DownstreamRightPinTargetHeTube"));

   AddPoint(std::string("UpstreamLeftBallHorn1"));
   AddPoint(std::string("UpstreamRightBallHorn1"));
   AddPoint(std::string("DownstreamLeftBallHorn1"));
   AddPoint(std::string("DownstreamRightBallHorn1"));

   AddPoint(std::string("UpstreamLeftBallHorn2"));
   AddPoint(std::string("UpstreamRightBallHorn2"));
   AddPoint(std::string("DownstreamLeftBallHorn2"));
   AddPoint(std::string("DownstreamRightBallHorn2"));
   
   AddPoint(std::string("UpstreamLeftDecayPipe"));
   AddPoint(std::string("UpstreamRightDecayPipe"));
   AddPoint(std::string("DownstreamLeftDecayPipe"));
   AddPoint(std::string("DownstreamRightDecayPipe"));
   
   fAllMessenger->defineAllCommds();
   
}
LBNEAllSurveyorMessenger::LBNEAllSurveyorMessenger():amDefined(false) {

  fSurvDir = new G4UIdirectory("/LBNE/Surveyor/");
  fSurvDir->SetGuidance("Implement controlled misalignments ");
  
}
LBNEAllSurveyorMessenger::~LBNEAllSurveyorMessenger() {

   delete fSurvPosUpstreamLeftPinTargetCanister;
   delete fSurvPosUpstreamRightPinTargetCanister;
   delete fSurvPosDownstreamLeftPinTargetCanister;
   delete fSurvPosDownstreamRightPinTargetCanister;
   delete fSurvPosUpstreamLeftPinTargetHeTube;
   delete fSurvPosUpstreamRightPinTargetHeTube;
   delete fSurvPosDownstreamLeftPinTargetHeTube;
   delete fSurvPosDownstreamRightPinTargetHeTube;
   delete fSurvPosUpstreamLeftBallHorn1;
   delete fSurvPosUpstreamRightBallHorn1;
   delete fSurvPosDownstreamLeftBallHorn1;
   delete fSurvPosDownstreamRightBallHorn1;
   delete fSurvPosUpstreamLeftBallHorn2;
   delete fSurvPosUpstreamRightBallHorn2;
   delete fSurvPosDownstreamLeftBallHorn2;
   delete fSurvPosDownstreamRightBallHorn2;
   delete fSurvPosUpstreamLeftDecayPipe;
   delete fSurvPosUpstreamRightDecayPipe;
   delete fSurvPosDownstreamLeftDecayPipe;
   delete fSurvPosDownstreamRightDecayPipe;
   delete fSurvDir;
   
}
void LBNEAllSurveyorMessenger::defineAllCommds() {

   if (amDefined) return;
  fSurvPosUpstreamLeftPinTargetCanister = new G4UIcmdWith3Vector("/LBNE/Surveyor/PosUpstreamLeftPinTargetCanister", this);
  fSurvPosUpstreamRightPinTargetCanister = new G4UIcmdWith3Vector("/LBNE/Surveyor/PosUpstreamRightPinTargetCanister", this);
  fSurvPosDownstreamLeftPinTargetCanister = new G4UIcmdWith3Vector("/LBNE/Surveyor/PosDownstreamLeftPinTargetCanister", this);
  fSurvPosDownstreamRightPinTargetCanister = new G4UIcmdWith3Vector("/LBNE/Surveyor/PosDownstreamRightPinTargetCanister", this);
  fSurvPosUpstreamLeftPinTargetHeTube = new G4UIcmdWith3Vector("/LBNE/Surveyor/PosUpstreamLeftPinTargetHeTube", this);
  fSurvPosUpstreamRightPinTargetHeTube = new G4UIcmdWith3Vector("/LBNE/Surveyor/PosUpstreamRightPinTargetHeTube", this);
  fSurvPosDownstreamLeftPinTargetHeTube = new G4UIcmdWith3Vector("/LBNE/Surveyor/PosDownstreamLeftPinTargetHeTube", this);
  fSurvPosDownstreamRightPinTargetHeTube = new G4UIcmdWith3Vector("/LBNE/Surveyor/PosDownstreamRightPinTargetHeTube", this);
  fSurvPosUpstreamLeftBallHorn1 = new G4UIcmdWith3Vector("/LBNE/Surveyor/PosUpstreamLeftBallHorn1", this);
  fSurvPosUpstreamRightBallHorn1 = new G4UIcmdWith3Vector("/LBNE/Surveyor/PosUpstreamRightBallHorn1", this);
  fSurvPosDownstreamLeftBallHorn1 = new G4UIcmdWith3Vector("/LBNE/Surveyor/PosDownstreamLeftBallHorn1", this);
  fSurvPosDownstreamRightBallHorn1 = new G4UIcmdWith3Vector("/LBNE/Surveyor/PosDownstreamRightBallHorn1", this);
  fSurvPosUpstreamLeftBallHorn2 = new G4UIcmdWith3Vector("/LBNE/Surveyor/PosUpstreamLeftBallHorn2", this);
  fSurvPosUpstreamRightBallHorn2 = new G4UIcmdWith3Vector("/LBNE/Surveyor/PosUpstreamRightBallHorn2", this);
  fSurvPosDownstreamLeftBallHorn2 = new G4UIcmdWith3Vector("/LBNE/Surveyor/PosDownstreamLeftBallHorn2", this);
  fSurvPosDownstreamRightBallHorn2 = new G4UIcmdWith3Vector("/LBNE/Surveyor/PosDownstreamRightBallHorn2", this);
  fSurvPosUpstreamLeftDecayPipe = new G4UIcmdWith3Vector("/LBNE/Surveyor/PosUpstreamLeftDecayPipe", this);
  fSurvPosUpstreamRightDecayPipe = new G4UIcmdWith3Vector("/LBNE/Surveyor/PosUpstreamRightDecayPipe", this);
  fSurvPosDownstreamLeftDecayPipe = new G4UIcmdWith3Vector("/LBNE/Surveyor/PosDownstreamLeftDecayPipe", this);
  fSurvPosDownstreamRightDecayPipe = new G4UIcmdWith3Vector("/LBNE/Surveyor/PosDownstreamRightDecayPipe", this);
 
  fSurvPosUpstreamLeftPinTargetCanister->SetDefaultValue(G4ThreeVector(1.0e-6*mm, 1.0e-6*mm, 1.0e-6*mm));
  fSurvPosUpstreamRightPinTargetCanister->SetDefaultValue(G4ThreeVector(1.0e-6*mm, 1.0e-6*mm, 1.0e-6*mm));
  fSurvPosDownstreamLeftPinTargetCanister->SetDefaultValue(G4ThreeVector(1.0e-6*mm, 1.0e-6*mm, 1.0e-6*mm));
  fSurvPosDownstreamRightPinTargetCanister->SetDefaultValue(G4ThreeVector(1.0e-6*mm, 1.0e-6*mm, 1.0e-6*mm));
  fSurvPosUpstreamLeftPinTargetHeTube->SetDefaultValue(G4ThreeVector(1.0e-6*mm, 1.0e-6*mm, 1.0e-6*mm));
  fSurvPosUpstreamRightPinTargetHeTube->SetDefaultValue(G4ThreeVector(1.0e-6*mm, 1.0e-6*mm, 1.0e-6*mm));
  fSurvPosDownstreamLeftPinTargetHeTube->SetDefaultValue(G4ThreeVector(1.0e-6*mm, 1.0e-6*mm, 1.0e-6*mm));
  fSurvPosDownstreamRightPinTargetHeTube->SetDefaultValue(G4ThreeVector(1.0e-6*mm, 1.0e-6*mm, 1.0e-6*mm));
  fSurvPosUpstreamLeftBallHorn1->SetDefaultValue(G4ThreeVector(1.0e-6*mm, 1.0e-6*mm, 1.0e-6*mm));
  fSurvPosUpstreamRightBallHorn1->SetDefaultValue(G4ThreeVector(1.0e-6*mm, 1.0e-6*mm, 1.0e-6*mm));
  fSurvPosDownstreamLeftBallHorn1->SetDefaultValue(G4ThreeVector(1.0e-6*mm, 1.0e-6*mm, 1.0e-6*mm));
  fSurvPosDownstreamRightBallHorn1->SetDefaultValue(G4ThreeVector(1.0e-6*mm, 1.0e-6*mm, 1.0e-6*mm));
  fSurvPosUpstreamLeftBallHorn2->SetDefaultValue(G4ThreeVector(1.0e-6*mm, 1.0e-6*mm, 1.0e-6*mm));
  fSurvPosUpstreamRightBallHorn2->SetDefaultValue(G4ThreeVector(1.0e-6*mm, 1.0e-6*mm, 1.0e-6*mm));
  fSurvPosDownstreamLeftBallHorn2->SetDefaultValue(G4ThreeVector(1.0e-6*mm, 1.0e-6*mm, 1.0e-6*mm));
  fSurvPosDownstreamRightBallHorn2->SetDefaultValue(G4ThreeVector(1.0e-6*mm, 1.0e-6*mm, 1.0e-6*mm));
  fSurvPosUpstreamLeftDecayPipe->SetDefaultValue(G4ThreeVector(1.0e-6*mm, 1.0e-6*mm, 1.0e-6*mm));
  fSurvPosUpstreamRightDecayPipe->SetDefaultValue(G4ThreeVector(1.0e-6*mm, 1.0e-6*mm, 1.0e-6*mm));
  fSurvPosDownstreamLeftDecayPipe->SetDefaultValue(G4ThreeVector(1.0e-6*mm, 1.0e-6*mm, 1.0e-6*mm));
  fSurvPosDownstreamRightDecayPipe->SetDefaultValue(G4ThreeVector(1.0e-6*mm, 1.0e-6*mm, 1.0e-6*mm));
  
  fSurvPosUpstreamLeftPinTargetCanister->AvailableForStates(G4State_PreInit);
  fSurvPosUpstreamRightPinTargetCanister->AvailableForStates(G4State_PreInit);
  fSurvPosDownstreamLeftPinTargetCanister->AvailableForStates(G4State_PreInit);
  fSurvPosDownstreamRightPinTargetCanister->AvailableForStates(G4State_PreInit);
  fSurvPosUpstreamLeftPinTargetHeTube->AvailableForStates(G4State_PreInit);
  fSurvPosUpstreamRightPinTargetHeTube->AvailableForStates(G4State_PreInit);
  fSurvPosDownstreamLeftPinTargetHeTube->AvailableForStates(G4State_PreInit);
  fSurvPosDownstreamRightPinTargetHeTube->AvailableForStates(G4State_PreInit);
  fSurvPosUpstreamLeftBallHorn1->AvailableForStates(G4State_PreInit);
  fSurvPosUpstreamRightBallHorn1->AvailableForStates(G4State_PreInit);
  fSurvPosDownstreamLeftBallHorn1->AvailableForStates(G4State_PreInit);
  fSurvPosDownstreamRightBallHorn1->AvailableForStates(G4State_PreInit);
  fSurvPosUpstreamLeftBallHorn2->AvailableForStates(G4State_PreInit);
  fSurvPosUpstreamRightBallHorn2->AvailableForStates(G4State_PreInit);
  fSurvPosDownstreamLeftBallHorn2->AvailableForStates(G4State_PreInit);
  fSurvPosDownstreamRightBallHorn2->AvailableForStates(G4State_PreInit);
  fSurvPosUpstreamLeftDecayPipe->AvailableForStates(G4State_PreInit);
  fSurvPosUpstreamRightDecayPipe->AvailableForStates(G4State_PreInit);
  fSurvPosDownstreamLeftDecayPipe->AvailableForStates(G4State_PreInit);
  fSurvPosDownstreamRightDecayPipe->AvailableForStates(G4State_PreInit);
  
  amDefined=true; 
  
}
void LBNEAllSurveyorMessenger::SetNewValue(G4UIcommand* cmd,G4String newValues) {

  LBNESurveyor *theSurv = LBNESurveyor::Instance();
  if (cmd == fSurvPosUpstreamLeftPinTargetCanister) {
    G4UIcmdWith3Vector *cmdT = dynamic_cast<G4UIcmdWith3Vector*>(cmd);
    theSurv->setPointPosition(G4String("UpstreamLeftPinTargetCanister"), 
                                       cmdT->GetNew3VectorValue(newValues.c_str())); 
    return;				       
  }
  if (cmd == fSurvPosUpstreamRightPinTargetCanister) {
    G4UIcmdWith3Vector *cmdT = dynamic_cast<G4UIcmdWith3Vector*>(cmd);
    theSurv->setPointPosition(G4String("UpstreamRightPinTargetCanister"), 
                                       cmdT->GetNew3VectorValue(newValues.c_str())); 
    return;
  }
  if (cmd == fSurvPosDownstreamLeftPinTargetCanister) {
    G4UIcmdWith3Vector *cmdT = dynamic_cast<G4UIcmdWith3Vector*>(cmd);
    theSurv->setPointPosition(G4String("DownstreamLeftPinTargetCanister"), 
                                       cmdT->GetNew3VectorValue(newValues.c_str())); 
    return;				       
  }
  if (cmd == fSurvPosDownstreamRightPinTargetCanister) {
    G4UIcmdWith3Vector *cmdT = dynamic_cast<G4UIcmdWith3Vector*>(cmd);
    theSurv->setPointPosition(G4String("DownstreamRightPinTargetCanister"), 
                                       cmdT->GetNew3VectorValue(newValues.c_str())); 
    return;
  }
 // 
  if (cmd == fSurvPosUpstreamLeftPinTargetHeTube) {
    G4UIcmdWith3Vector *cmdT = dynamic_cast<G4UIcmdWith3Vector*>(cmd);
    theSurv->setPointPosition(G4String("UpstreamLeftPinTargetHeTube"), 
                                       cmdT->GetNew3VectorValue(newValues.c_str())); 
    return;				       
  }
  if (cmd == fSurvPosUpstreamRightPinTargetHeTube) {
    G4UIcmdWith3Vector *cmdT = dynamic_cast<G4UIcmdWith3Vector*>(cmd);
    theSurv->setPointPosition(G4String("UpstreamRightPinTargetHeTube"), 
                                       cmdT->GetNew3VectorValue(newValues.c_str())); 
    return;
  }
  if (cmd == fSurvPosDownstreamLeftPinTargetHeTube) {
    G4UIcmdWith3Vector *cmdT = dynamic_cast<G4UIcmdWith3Vector*>(cmd);
    theSurv->setPointPosition(G4String("DownstreamLeftPinTargetHeTube"), 
                                       cmdT->GetNew3VectorValue(newValues.c_str())); 
    return;				       
  }
  if (cmd == fSurvPosDownstreamRightPinTargetHeTube) {
    G4UIcmdWith3Vector *cmdT = dynamic_cast<G4UIcmdWith3Vector*>(cmd);
    theSurv->setPointPosition(G4String("DownstreamRightPinTargetHeTube"), 
                                       cmdT->GetNew3VectorValue(newValues.c_str())); 
    return;
  }
  //
  
  if (cmd == fSurvPosUpstreamLeftBallHorn1) {
    G4UIcmdWith3Vector *cmdT = dynamic_cast<G4UIcmdWith3Vector*>(cmd);
    theSurv->setPointPosition(G4String("UpstreamLeftBallHorn1"), 
                                       cmdT->GetNew3VectorValue(newValues.c_str())); 
    return;				       
  }
  if (cmd == fSurvPosUpstreamRightBallHorn1) {
    G4UIcmdWith3Vector *cmdT = dynamic_cast<G4UIcmdWith3Vector*>(cmd);
    theSurv->setPointPosition(G4String("UpstreamRightBallHorn1"), 
                                       cmdT->GetNew3VectorValue(newValues.c_str())); 
    return;
  }
  if (cmd == fSurvPosDownstreamLeftBallHorn1) {
    G4UIcmdWith3Vector *cmdT = dynamic_cast<G4UIcmdWith3Vector*>(cmd);
    theSurv->setPointPosition(G4String("DownstreamLeftBallHorn1"), 
                                       cmdT->GetNew3VectorValue(newValues.c_str())); 
    return;				       
  }
  if (cmd == fSurvPosDownstreamRightBallHorn1) {
    G4UIcmdWith3Vector *cmdT = dynamic_cast<G4UIcmdWith3Vector*>(cmd);
    theSurv->setPointPosition(G4String("DownstreamRightBallHorn1"), 
                                       cmdT->GetNew3VectorValue(newValues.c_str())); 
    return;
  }
 // 
  
  if (cmd == fSurvPosUpstreamLeftBallHorn2) {
    G4UIcmdWith3Vector *cmdT = dynamic_cast<G4UIcmdWith3Vector*>(cmd);
    theSurv->setPointPosition(G4String("UpstreamLeftBallHorn2"), 
                                       cmdT->GetNew3VectorValue(newValues.c_str())); 
    return;				       
  }
  if (cmd == fSurvPosUpstreamRightBallHorn2) {
    G4UIcmdWith3Vector *cmdT = dynamic_cast<G4UIcmdWith3Vector*>(cmd);
    theSurv->setPointPosition(G4String("UpstreamRightBallHorn2"), 
                                       cmdT->GetNew3VectorValue(newValues.c_str())); 
    return;
  }
  if (cmd == fSurvPosDownstreamLeftBallHorn2) {
    G4UIcmdWith3Vector *cmdT = dynamic_cast<G4UIcmdWith3Vector*>(cmd);
    theSurv->setPointPosition(G4String("DownstreamLeftBallHorn2"), 
                                       cmdT->GetNew3VectorValue(newValues.c_str())); 
    return;				       
  }
  if (cmd == fSurvPosDownstreamRightBallHorn2) {
    G4UIcmdWith3Vector *cmdT = dynamic_cast<G4UIcmdWith3Vector*>(cmd);
    theSurv->setPointPosition(G4String("DownstreamRightBallHorn2"), 
                                       cmdT->GetNew3VectorValue(newValues.c_str())); 
    return;
  }
 // 
  
  if (cmd == fSurvPosUpstreamLeftDecayPipe) {
    G4UIcmdWith3Vector *cmdT = dynamic_cast<G4UIcmdWith3Vector*>(cmd);
    theSurv->setPointPosition(G4String("UpstreamLeftDecayPipe"), 
                                       cmdT->GetNew3VectorValue(newValues.c_str())); 
    return;				       
  }
  if (cmd == fSurvPosUpstreamRightDecayPipe) {
    G4UIcmdWith3Vector *cmdT = dynamic_cast<G4UIcmdWith3Vector*>(cmd);
    theSurv->setPointPosition(G4String("UpstreamRightDecayPipe"), 
                                       cmdT->GetNew3VectorValue(newValues.c_str())); 
    return;
  }
  if (cmd == fSurvPosDownstreamLeftDecayPipe) {
    G4UIcmdWith3Vector *cmdT = dynamic_cast<G4UIcmdWith3Vector*>(cmd);
    theSurv->setPointPosition(G4String("DownstreamLeftDecayPipe"), 
                                       cmdT->GetNew3VectorValue(newValues.c_str())); 
    return;				       
  }
  if (cmd == fSurvPosDownstreamRightDecayPipe) {
    G4UIcmdWith3Vector *cmdT = dynamic_cast<G4UIcmdWith3Vector*>(cmd);
    theSurv->setPointPosition(G4String("DownstreamRightDecayPipe"), 
                                       cmdT->GetNew3VectorValue(newValues.c_str())); 
    return;
  }
 // 
  

}
