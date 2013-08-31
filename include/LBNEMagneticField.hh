
// $Id: LBNEMagneticField.hh,v 1.2.2.1 2013/08/31 20:41:09 lebrun Exp $
// --------------------------------------------------------------
// LBNEMagneticField.hh modified by Yuki 2004/7/16
// modified by Yuki 8/2/04
#ifndef LBNEMagneticField_H
#define LBNEMagneticField_H 1
#include <vector>

#include "globals.hh"
#include "G4MagneticField.hh"


class LBNEMagneticFieldHorn : public G4MagneticField
{
  public:
    explicit LBNEMagneticFieldHorn(bool isHorn1);

  virtual void GetFieldValue( const double Point[3], double *Bfield ) const;

  private:
   G4bool amHorn1;
   G4double fHornCurrent;
   // Data for the World coordinate transform to the Horn coordinate system. 
   // We linearize the tranformation, leave Z unchanged, except for a translation.
   G4bool coordinateSet;  
   G4double fXShift;
   G4double fYShift;
   G4double fXShiftSlope;
   G4double fYShiftSlope;
   G4double fZShiftDrawingCoordinate;
   // The outer radius of the conuctor is computed from the LBNEVolumePlacement Radial equations. 
   // in the coordinate system of the drawing 8875.112-MD-363104 and 383385
   G4double fZShiftUpstrWorldToLocal;
   G4double fZShiftDwnstrWorldToLocal; // Identical to one an other for Horn2, but diff is the logical volume 
                                       // is "UpstreamTargetAssembly" or "Horn1Hall". 
   // We now need to determine the radius where the current flows. 
   // We will call the utility meothds from the LBNEVolumePlacements class.  
   std::vector<size_t> fEqunIndices;
   std::vector<double> fZDCBegin;
   std::vector<double> fZDCEnd;
   G4double fNeckRadius;
   
   void setTheCoordinates();
   
  public:
    
   inline void SetHornCurrent(G4double ihorn) {fHornCurrent = ihorn;}
   inline G4double GetHornCurrent() const { return fHornCurrent;}
   
   void dumpField();

};

#endif

