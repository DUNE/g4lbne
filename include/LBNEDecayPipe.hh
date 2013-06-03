//---------------------------------------------------------------------------// 
// $Id: LBNEDecayPipe.hh,v 1.1.2.4 2013/06/03 18:52:47 robj137 Exp $
//---------------------------------------------------------------------------// 

#ifndef LBNEDecayPipe_H
#define LBNEDecayPipe_H 

#include "LBNESubVolume.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

class G4LogicalVolume;

class LBNEDecayPipe : public LBNESubVolume
{

private:
  G4double      fDecayPipeLength;
  G4double      fDecayPipeRadius;
  G4double      fDecayPipeWallThickness;
  G4double      fDecayPipeEWinThickness;
  G4double      fDecayPipeFWinThickness;
  G4double      fDecayPipeWinThickness;
  G4String      fMaterialName;

public:
  LBNEDecayPipe(G4String detName);
  ~LBNEDecayPipe();
  void ConstructSubvolume();

  void SetDecayPipeThickness(G4double val) { fDecayPipeWallThickness = val; }
  void SetDecayPipeRadius(G4double val) { fDecayPipeRadius = val; }
  void SetDecayPipeLength(G4double val) { fDecayPipeLength = val; }
  void SetDecayPipeWindowThickness(G4double val) {fDecayPipeWinThickness = val;}
  void SetMaterialName(G4String name) { fMaterialName = name; }

  G4double GetDecayPipeRadius() { return fDecayPipeRadius; }
  G4double GetDecayPipeLength() { return fDecayPipeLength; }
  G4String GetMaterialName() { return fMaterialName; }


};

class LBNEDecayPipeMessenger: public LBNESubVolumeMessenger{
  
  public:
    LBNEDecayPipeMessenger(LBNESubVolume *subVolume);
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

