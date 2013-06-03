// LBNESubVolume.hh
//---------------------------------------------------------------------------// 

#ifndef LBNESubVolume_H
#define LBNESubVolume_H

#include "globals.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4UIdirectory.hh"
#include "G4UImessenger.hh"

class G4LogicalVolume;
//---------------------------------------------------------------------------// 
class LBNESubVolume
{
public:

  LBNESubVolume(G4String detName);
  virtual ~LBNESubVolume();
  
  virtual void ConstructSubvolume();
  inline G4LogicalVolume* GetSubVolumeLogical() { return fSubVolumeLogical; }
  inline G4LogicalVolume *GetLogicalVolume() { return fSubVolumeLogical; }

  void SetSubVolumeName( G4String aName ) { fSubVolumeName = aName; }
  void SetDefaults();
  G4RotationMatrix GetRotation() { return fRotation; }
  G4ThreeVector GetPlacement() { return fPlacement; }
  void FillPlacement(G4ThreeVector &vec) {
    vec.set(fPlacement.getX(),fPlacement.getY(),fPlacement.getZ());
  }
  void FillRotation(G4RotationMatrix &mat) {
    mat.setPhi(fRotation.getPhi());
    mat.setTheta(fRotation.getTheta());
    mat.setPsi(fRotation.getPsi());
  }
  G4ThreeVector GetPlacement(G4ThreeVector vec) {return fPlacement + vec; }
  G4String GetPhysicalName() { return fSubVolumeName; };

  void SetRotation(G4double phi, G4double theta, G4double psi) {
    fRotation.set(phi,theta,psi);
  }
  void SetPlacement(G4double x, G4double y, G4double z) { 
              fPlacement.set(x,y,z);
  }
  void SetPlacement(G4ThreeVector vec) { SetPlacement(vec.getX(), vec.getY(), vec.getZ()); }

protected:

  G4ThreeVector                 fPlacement;
  G4RotationMatrix              fRotation;
  G4LogicalVolume*              fSubVolumeLogical;
  G4String                      fSubVolumeName;
  G4UImessenger*                fMessenger;
};

class LBNESubVolumeMessenger: public G4UImessenger {
protected:
  LBNESubVolume *fSubVolume;
  G4UIdirectory *fDirectory;
  G4String fDirectoryName;
public:
  LBNESubVolumeMessenger(LBNESubVolume *subVolume);
  ~LBNESubVolumeMessenger();
  G4String GetDirectoryName() { return fDirectoryName; }
  G4UIdirectory *GetDirectory() { return fDirectory; }
  G4String GetCommandString(G4String cmd) {
    G4String cmdName = GetDirectoryName() + "/"+cmd;
    return cmdName;
  }
  void SetNewValue(G4UIcommand *cmd, G4String val);


}; 
#endif
