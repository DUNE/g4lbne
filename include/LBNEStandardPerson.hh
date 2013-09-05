//---------------------------------------------------------------------------// 
// $Id: LBNEStandardPerson.hh,v 1.1.2.4 2013/09/05 12:32:49 lebrun Exp $
//---------------------------------------------------------------------------// 

#ifndef LBNEStandardPerson_H
#define LBNEStandardPerson_H 

class G4LogicalVolume;

class LBNEStandardPerson
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
/*
** Obsolete..
class LBNEStandardPersonMessenger: public LBNESubVolumeMessenger{
  
  public:
    LBNEStandardPersonMessenger(LBNESubVolume *subVolume);
    ~LBNEStandardPersonMessenger();
    void SetNewValue(G4UIcommand *cmd, G4String val);

  private:

    LBNEStandardPerson*      fStandardPerson;
};
*/
#endif

