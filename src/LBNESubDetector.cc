#include "LBNESubDetector.hh"


LBNESubDetector::LBNESubDetector(G4String detName)
  : fPlacement(NULL), fRotation(), fSubDetectorLogical(NULL), 
    fSubDetectorName(detName), fMessenger(NULL) {
    G4cout << "constructing lbne subdetector " << detName << G4endl;
  fRotation = G4RotationMatrix::IDENTITY;
  //fRotation.setTheta(0);
  //fRotation.setPsi(0);
  //fRotation.setPhi(0);
  fPlacement = G4ThreeVector(0,0,0);
}


LBNESubDetector::~LBNESubDetector()
{;}

void LBNESubDetector::ConstructSubdetector()
{
G4cout << "constructing base class subdetector " << G4endl;
;
}
void LBNESubDetector::SetDefaults()
{;}

LBNESubDetectorMessenger::LBNESubDetectorMessenger(LBNESubDetector *subDetector)
{
  G4String subDetectorName = subDetector->GetPhysicalName();;
  fDirectoryName = "/LBNE/det/"+subDetectorName+"/";
  fDirectory = new G4UIdirectory(fDirectoryName);
}

LBNESubDetectorMessenger::~LBNESubDetectorMessenger()
{;}

void LBNESubDetectorMessenger::SetNewValue(G4UIcommand *cmd, G4String val){
; // for G4UIcommands that are common to all subdetectors
}
