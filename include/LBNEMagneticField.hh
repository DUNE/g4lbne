
// $Id: LBNEMagneticField.hh,v 1.2.2.6 2013/09/09 20:01:49 lebrun Exp $
// --------------------------------------------------------------
// LBNEMagneticField.hh modified by Yuki 2004/7/16
// modified by Yuki 8/2/04
#ifndef LBNEMagneticField_H
#define LBNEMagneticField_H 1
#include <fstream>
#include <vector>

#include "globals.hh"
#include "G4MagneticField.hh"
#include <fstream>

class LBNEMagneticFieldHorn : public G4MagneticField
{
  public:
    explicit LBNEMagneticFieldHorn(bool isHorn1);
    ~LBNEMagneticFieldHorn();
    
  virtual void GetFieldValue( const double Point[3], double *Bfield ) const;

  private:
   G4bool amHorn1;
   G4double fHornCurrent;
   // Data for the World coordinate transform to the Horn coordinate system. 
   // We linearize the tranformation, leave Z unchanged, except for a translation.
   mutable G4bool fCoordinateSet; // set when we got the first track entering the logical volume for which  the field manager is defined  
   std::vector<double> fShift;
   std::vector<double> fShiftSlope;
   mutable G4double fZShiftDrawingCoordinate;
   // The outer radius of the conuctor is computed from the LBNEVolumePlacement Radial equations. 
   // in the coordinate system of the drawing 8875.112-MD-363104 and 383385
   mutable G4double fZShiftUpstrWorldToLocal; // The Zcoordinate in world system of the entrance of the volume for which the field is defined. 
                                      // For Horn1, this volue is named "Horn1TopLevelUpstr" 
				      //(or Horn1IOTransCont, which start 2 microns downstream of it... 
				      // and Horn1Hall
				      // For Horn2, it is Horn2TopLevel. 
   G4double fEffectiveLength; // The length assumed to compute the coordinate change. 
   G4double fHornNeckOuterRadius; // Copy of the VolumePlacement Neck Outer Radius, rescaled if need be.. 				        
   G4double fHornNeckInnerRadius; // Copy of the VolumePlacement Neck Outer Radius, rescaled if need be.. 				        
   G4double fOuterRadius;
   G4double fOuterRadiusEff; // the effective outer radius, physical outer radius - 2.0*skindepth at 0.43 kHz 
   G4double fSkinDepth; // at (1./(2.3 ms))
   // We now need to determine the radius where the current flows. 
   // We will call the utility meothds from the LBNEVolumePlacements class.  
   std::vector<size_t> fEqnIndicesOuter;
   std::vector<size_t> fEqnIndicesInner;
   std::vector<double> fZDCBegin;
   std::vector<double> fZDCEnd;
   G4double fNeckRadius;
   mutable std::ofstream fOutTraj;
   
   
   void fillTrajectories(const double Point[3], double bx, double by) const;
   
  public:
    
   inline void SetHornCurrent(G4double ihorn) {fHornCurrent = ihorn;}
   inline G4double GetHornCurrent() const { return fHornCurrent;}
   
   void dumpField() const;

};

#endif

