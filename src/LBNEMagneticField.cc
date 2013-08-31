//----------------------------------------------------------------------
//LBNEMagneticField 
// $Id
//----------------------------------------------------------------------

#include "LBNEMagneticField.hh"

#include "G4VPhysicalVolume.hh"
#include "G4ios.hh"
#include "G4TransportationManager.hh"
#include "LBNEVolumePlacements.hh"

//magnetic field between conductors ====================================================

LBNEMagneticFieldHorn::LBNEMagneticFieldHorn(bool isOne) : 
amHorn1(isOne),
fHornCurrent(0.),
coordinateSet(false),  
fXShift(0.),
fYShift(0.),
fXShiftSlope(0.),
fYShiftSlope(0.),
fZShiftDrawingCoordinate(-1.0e13),
fZShiftUpstrWorldToLocal(-1.0e13),
fZShiftDwnstrWorldToLocal(-1.0e13)    
{
  const LBNEVolumePlacements *fPlacementHandler = LBNEVolumePlacements::Instance();
  const double aHornLongRescale = (amHorn1) ? 
      fPlacementHandler->GetHorn1LongRescale() : fPlacementHandler->GetHorn2LongRescale() ;
// Radial equation interface.  Take the outer equations..
  
  fEqunIndices.clear();
  fZDCBegin.clear();
  fZDCEnd.clear();
/*
 ???????????????/ Fill the stuff here.. 
  if(amHorn1) {
     fEqunIndices.push_back(
  }
*/
}

void LBNEMagneticFieldHorn::GetFieldValue(const double Point[3],double *Bfield) const
{

  
   G4double current = fHornCurrent/ampere/1000.;
   /*
//   G4double current = LBNEData->GetHornCurrent()/ampere/1000.;

  //G4cout << "LBNEData->HornCurrent = " << LBNEData->HornCurrent/ampere << " A" << G4endl;
  //G4cout << "current = " << current << G4endl;


  
  G4double radius = sqrt(Point[0]*Point[0]+Point[1]*Point[1]);    
  G4double B = current / (5.*radius/cm)/10*tesla;  //B(kG)=i(kA)/[5*r(cm)], 1T=10kG
  Bfield[0] = -B*Point[1]/radius;
  Bfield[1] = B*Point[0]/radius;
  Bfield[2] = 0.;

*/  
}

