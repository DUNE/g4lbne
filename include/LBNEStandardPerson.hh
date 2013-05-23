#ifndef LBNEStandardPerson_H
#define LBNEStandardPerson_H 

#include "LBNESubDetector.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

class G4LogicalVolume;

class LBNEStandardPerson : public LBNESubDetector
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
  void ConstructSubdetector();

};

class LBNEStandardPersonMessenger: public LBNESubDetectorMessenger{
  
  public:
    LBNEStandardPersonMessenger(LBNESubDetector *subDetector);
    ~LBNEStandardPersonMessenger();
    void SetNewValue(G4UIcommand *cmd, G4String val);

  private:

    LBNEStandardPerson*      fStandardPerson;
};
#endif

