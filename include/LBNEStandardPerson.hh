//---------------------------------------------------------------------------// 
// $Id: LBNEStandardPerson.hh,v 1.1.2.3 2013/06/03 18:55:37 robj137 Exp $
//---------------------------------------------------------------------------// 

#ifndef LBNEStandardPerson_H
#define LBNEStandardPerson_H 

#include "LBNESubVolume.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

class G4LogicalVolume;

class LBNEStandardPerson : public LBNESubVolume
{

private:
  G4double      fHeadRadius;
  G4double      fHeight;
  G4double      fLegRadius;
  G4double      fLegLength;
  G4double      fTorsoRadius;
  G4double      fTorsoLength;

public:
  LBNEStandardPerson(G4String detName);
  ~LBNEStandardPerson();
  void ConstructSubvolume();

};

class LBNEStandardPersonMessenger: public LBNESubVolumeMessenger{
  
  public:
    LBNEStandardPersonMessenger(LBNESubVolume *subVolume);
    ~LBNEStandardPersonMessenger();
    void SetNewValue(G4UIcommand *cmd, G4String val);

  private:

    LBNEStandardPerson*      fStandardPerson;
};
#endif

