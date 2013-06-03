//---------------------------------------------------------------------------// 
// $Id: LBNEHadronAbsorber.hh,v 1.1.2.3 2013/06/03 18:52:47 robj137 Exp $
//---------------------------------------------------------------------------// 

#ifndef LBNEHadronAbsorber_H
#define LBNEHadronAbsorber_H 

#include "LBNESubVolume.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

class G4LogicalVolume;

class LBNEHadronAbsorber : public LBNESubVolume
{

private:
  G4double      fAlX;
  G4double      fAlY;
  G4double      fAlZ;
  G4double      fSteelX;
  G4double      fSteelY;
  G4double      fSteelZ;
  G4double      fCavityX;
  G4double      fCavityY;
  G4double      fCavityZ;
  G4double      fConcreteX;
  G4double      fConcreteY;
  G4double      fConcreteZ;
  G4double      fAirX;
  G4double      fAirY;
  G4double      fAirZ;

public:
  LBNEHadronAbsorber(G4String detName);
  ~LBNEHadronAbsorber();
  void ConstructSubvolume();

};

class LBNEHadronAbsorberMessenger: public LBNESubVolumeMessenger{
  
  public:
    LBNEHadronAbsorberMessenger(LBNESubVolume *subVolume);
    ~LBNEHadronAbsorberMessenger();
    void SetNewValue(G4UIcommand *cmd, G4String val);

  private:

    LBNEHadronAbsorber*      fHadronAbsorber;
};
#endif

