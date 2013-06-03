#include "LBNESubVolume.hh"


LBNESubVolume::LBNESubVolume(G4String detName)
  : fPlacement(NULL), fRotation(), fSubVolumeLogical(NULL), 
    fSubVolumeName(detName), fMessenger(NULL) {
    G4cout << "constructing lbne subvolume " << detName << G4endl;
  fRotation = G4RotationMatrix::IDENTITY;
  //fRotation.setTheta(0);
  //fRotation.setPsi(0);
  //fRotation.setPhi(0);
  fPlacement = G4ThreeVector(0,0,0);
}


LBNESubVolume::~LBNESubVolume()
{;}

void LBNESubVolume::ConstructSubvolume()
{
G4cout << "constructing base class subvolume " << G4endl;
;
}
void LBNESubVolume::SetDefaults()
{;}

LBNESubVolumeMessenger::LBNESubVolumeMessenger(LBNESubVolume *subVolume)
{
  G4String subVolumeName = subVolume->GetPhysicalName();;
  fDirectoryName = "/LBNE/det/"+subVolumeName+"/";
  fDirectory = new G4UIdirectory(fDirectoryName);
}

LBNESubVolumeMessenger::~LBNESubVolumeMessenger()
{;}

void LBNESubVolumeMessenger::SetNewValue(G4UIcommand *cmd, G4String val){
; // for G4UIcommands that are common to all subvolumes
}
