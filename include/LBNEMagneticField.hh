
// $Id: LBNEMagneticField.hh,v 1.2.2.8 2013/12/24 20:30:46 lebrun Exp $
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
   // Parameter for the systematic uncertainty due to skin depth effect for the inner conductor. 
   // varies between 0. (default, current density is uniform between inner IC radius and outer IC radius), 
   // to 0.99, where all the current flows in only 1% of the outer radii..  
   G4double fSkinDepthInnerRad;
   G4double  fSkinDepthIIRInv;
   mutable std::ofstream fOutTraj;
   
   
   void fillTrajectories(const double Point[3], double bx, double by) const;
   // 
   // Implement the Skin depth for the Inner conductors. 
   // References:  Zarko's Thesis (MINOS Document 5694-v1), section 5.2.4, p150 - 156)
   // DeltaR : difference from the radial coordinate of the track and the inner radius of the inner conductor.
   //
   // Note: Approximate, because this reasoning is based on a cylindical (not tubular) conductor.
   //
   inline double getNormCurrDensityInnerC(double deltaR, double rOut) const {
     if (deltaR < 0.) return 0.;
     const double xby2 = fSkinDepthIIRInv*deltaR/2.; // x must be less < 1, otherwise Taylor expansion fails.. 
     const double xby2Out = fSkinDepthIIRInv*rOut/2.; // x must be less < 1, otherwise Taylor expansion fails.. 
     // keep only 3 terms.. 
     const double ber = 1.0 - std::pow(xby2, 4.)/4. + std::pow(xby2, 8.)/576.;
     const double bei = std::pow(xby2, 2.) - std::pow(xby2, 6.)/36 + std::pow(xby2, 10.)/14400.;
     const double beNSq = ber*ber + bei*bei;
     const double berOut = 1.0 - std::pow(xby2Out, 4.)/4. + std::pow(xby2Out, 8.)/576.;
     const double beiOut = std::pow(xby2Out, 2.) - std::pow(xby2Out, 6.)/36 + std::pow(xby2Out, 10.)/14400.;
     const double beNSqOut = berOut*berOut + beiOut*beiOut;
     return std::sqrt(beNSq/beNSqOut);
   }
   
  public:
    
   inline void SetHornCurrent(G4double ihorn) {fHornCurrent = ihorn;}
   inline G4double GetHornCurrent() const { return fHornCurrent;}
   inline void SetSkinDepthInnerRad(G4double f) {fSkinDepthInnerRad = f; fSkinDepthIIRInv= std::sqrt(2.0)/f;}
   
   void dumpField() const;

};

#endif

