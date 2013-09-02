
// $Id: LBNEMagneticField.hh,v 1.2.2.2 2013/09/02 09:35:03 lebrun Exp $
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
   mutable G4bool coordinateSet; // set when we got the first track entering the logical volume for which  the field manager is defined  
   std::vector<double> fShift;
   std::vector<double> fShiftSlope;
   mutable G4double fZShiftDrawingCoordinate;
   // The outer radius of the conuctor is computed from the LBNEVolumePlacement Radial equations. 
   // in the coordinate system of the drawing 8875.112-MD-363104 and 383385
   mutable G4double fZShiftUpstrWorldToLocal; // The Zcoordinate in world system of the entrance of the volume for which the field is defined. 
                                      // For Horn1, this volue is named "Horn1TopLevelUpstr" (or Horn1IOTransCont, which start 2 microns downstream
				      // of it... 
				      // For Horn2, it is Horn2TopLevel. 
   G4double fEffectiveLength; // The length assumed to compute the coordinate change. 
   G4double fHornNeckRadius; // Copy of the VolumePlacement Neck Outer Radius, rescaled if need be.. 				        
   G4double fOuterRadius;
   // We now need to determine the radius where the current flows. 
   // We will call the utility meothds from the LBNEVolumePlacements class.  
   std::vector<size_t> fEqnIndices;
   std::vector<double> fZDCBegin;
   std::vector<double> fZDCEnd;
   G4double fNeckRadius;
      
  public:
    
   inline void SetHornCurrent(G4double ihorn) {fHornCurrent = ihorn;}
   inline G4double GetHornCurrent() const { return fHornCurrent;}
   
   void dumpField();

};

#endif

