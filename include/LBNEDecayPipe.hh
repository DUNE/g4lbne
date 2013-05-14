#ifndef LBNEDecayPipe_H
#define LBNEDecayPipe_H 

#include "LBNESubDetector.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

class G4LogicalVolume;

class LBNEDecayPipe : public LBNESubDetector
{

private:
  G4double      fDecayPipeLength;
  G4double      fDecayPipeRadius;
  G4double      fDecayPipeWallThickness;
  G4double      fDecayPipeEWinThickness;
  G4double      fDecayPipeFWinThickness;
  G4double      fDecayPipeWinThickness;

public:
  LBNEDecayPipe(G4String detName);
  ~LBNEDecayPipe();
  void ConstructSubdetector();

  void SetDecayPipeThickness(G4double val) { fDecayPipeWallThickness = val; }
  void SetDecayPipeRadius(G4double val) { fDecayPipeRadius = val; }
  void SetDecayPipeLength(G4double val) { fDecayPipeLength = val; }
  void SetDecayPipeWindowThickness(G4double val) {fDecayPipeWinThickness = val;}

  G4double GetDecayPipeRadius() { return fDecayPipeRadius; }
  G4double GetDecayPipeLength() { return fDecayPipeLength; }
};

class LBNEDecayPipeMessenger: public LBNESubDetectorMessenger{
  
  public:
    LBNEDecayPipeMessenger(LBNESubDetector *subDetector);
    ~LBNEDecayPipeMessenger();
    void SetNewValue(G4UIcommand *cmd, G4String val);

  private:
    G4UIcmdWithADoubleAndUnit*  fDecayPipeLengthCmd;
    G4UIcmdWithADoubleAndUnit*  fDecayPipeRadiusCmd;
    G4UIcmdWithADoubleAndUnit*  fDecayPipeWindowThicknessCmd;
    G4UIcmdWithADoubleAndUnit*  fDecayPipeWallThicknessCmd;

    LBNEDecayPipe*      fDecayPipe;
};
#endif

