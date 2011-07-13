//
// LBNEImpWeight.hh
//

#ifndef LBNEImpWeight_H
#define LBNEImpWeight_H 1

#include "globals.hh"
#include "G4ThreeVector.hh"

class G4Track;

class LBNEImpWeight
{
  public:
    LBNEImpWeight();
    virtual ~LBNEImpWeight();

  static G4double CalculateImpWeight(const G4Track * aTrack);
  

};

#endif
