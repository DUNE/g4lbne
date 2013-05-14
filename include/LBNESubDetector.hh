// LBNESubDetector.hh
//---------------------------------------------------------------------------// 

#ifndef LBNESubDetector_H
#define LBNESubDetector_H

#include "globals.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4UIdirectory.hh"
#include "G4UImessenger.hh"

class G4LogicalVolume;
//---------------------------------------------------------------------------// 
class LBNESubDetector
{
public:

  LBNESubDetector(G4String detName);
  virtual ~LBNESubDetector();
  
  virtual void ConstructSubdetector();
  inline G4LogicalVolume* GetSubDetectorLogical() { return fSubDetectorLogical; }
  inline G4LogicalVolume *GetLogicalVolume() { return fSubDetectorLogical; }

  void SetSubDetectorName( G4String aName ) { fSubDetectorName = aName; }
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
  G4String GetPhysicalName() { return fSubDetectorName; };

  void SetPlacement(G4double x, G4double y, G4double z) { 
              fPlacement.set(x,y,z);
  }
  void SetPlacement(G4ThreeVector vec) { SetPlacement(vec.getX(), vec.getY(), vec.getZ()); }

protected:

  G4ThreeVector                 fPlacement;
  G4RotationMatrix              fRotation;
  G4LogicalVolume*              fSubDetectorLogical;
  G4String                      fSubDetectorName;
  G4UImessenger*                fMessenger;
};

class LBNESubDetectorMessenger: public G4UImessenger {
protected:
  LBNESubDetector *fSubDetector;
  G4UIdirectory *fDirectory;
  G4String fDirectoryName;
public:
  LBNESubDetectorMessenger(LBNESubDetector *subDetector);
  ~LBNESubDetectorMessenger();
  G4String GetDirectoryName() { return fDirectoryName; }
  G4UIdirectory *GetDirectory() { return fDirectory; }
  G4String GetCommandString(G4String cmd) {
    G4String cmdName = GetDirectoryName() + "/"+cmd;
    return cmdName;
  }
  void SetNewValue(G4UIcommand *cmd, G4String val);


}; 
#endif
