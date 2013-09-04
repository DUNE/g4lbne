//----------------------------------------------------------------------
//LBNEMagneticField 
// $Id
//----------------------------------------------------------------------

#include "LBNEMagneticField.hh"

#include "G4VPhysicalVolume.hh"
#include "G4ios.hh"
#include "G4TransportationManager.hh"
#include "LBNEVolumePlacements.hh"
#include "LBNESurveyor.hh"
#include "Randomize.hh"

//magnetic field between conductors ====================================================

LBNEMagneticFieldHorn::LBNEMagneticFieldHorn(bool isOne) : 
amHorn1(isOne),
fHornCurrent(0.),
fCoordinateSet(false),  
fShift(2, 0.),
fShiftSlope(2,0.),
fZShiftDrawingCoordinate(-1.0e13),
fZShiftUpstrWorldToLocal(-1.0e13),
fEffectiveLength(0.),
fHornNeckOuterRadius(-1.0e13),
fHornNeckInnerRadius(-1.0e13),
fOuterRadius(1.0e9),
fOuterRadiusEff(1.0e9),
fSkinDepth(4.1*mm)
{
 // 
 // We rely on the Volume Placement utility and the surveyor data to compute the coordinate transfers. 
 // Two distinct ones:
 //
 //  (i) Going from G4 World to the local coordinate system.  This is done in two steps: first a Z shift, fixed.
 //         Key variable: fZShiftUpstrWorldToLocal
 // second, a linearized rotation due misalignement, in both X and Y. 
 //
 //  (ii) A Z translation to go from G4 World the 8875.112 MD 363xxx drawing coordinate system, such that 
 //      we can used the exact parabolic equations (Outer radius one)
 //    fZShiftDrawingCoordinate 
 //
 // This strategy has the drawback of the danger of making mistakes in the code below, but has 
 // has the advantage of less coordinate transforms. (inverse one done in the G4 Propagation. )
 // It is assumed here that the misalignment are small, i.e., cos^2(tilt) ~ 0.  
// 
// Having the field manager assigned to the top level of the Horn volume instead of just 
// the conductor polycones volumes, and the volume in between the two conductors, 
//  allows us to add fringe fields if need be, and avoid creating the volumes in between 
// the conductors..
//
  const LBNEVolumePlacements *aPlacementHandler = LBNEVolumePlacements::Instance();
// Radial equation interface.  Take the outer equations..
  
  fEqnIndicesInner.clear();
  fEqnIndicesOuter.clear();
  fZDCBegin.clear();
  fZDCEnd.clear();
  if(amHorn1) { // Use the Placement data to get the relevant Z coordinate to compute the distance to the inner or outer conductor.  
     const double zCDNeckStart = aPlacementHandler->GetHorn1NeckZPosition() - aPlacementHandler->GetHorn1NeckLength()/2.;
     const double zCDNeckEnd = aPlacementHandler->GetHorn1NeckZPosition() + aPlacementHandler->GetHorn1NeckLength()/2.;
     const double zCDFirstRing = 0.544*zCDNeckStart; // transition from drawing equation 1 to 2 .
                                                   // Drawing 8875.112-MD 363104
     fZDCBegin.push_back(0.); fZDCEnd.push_back(zCDFirstRing); 
     fEqnIndicesInner.push_back(0); fEqnIndicesOuter.push_back(5);
     fZDCBegin.push_back(zCDFirstRing); fZDCEnd.push_back(zCDNeckStart); 
     fEqnIndicesInner.push_back(1); fEqnIndicesOuter.push_back(5);
     fZDCBegin.push_back(zCDNeckStart); fZDCEnd.push_back(zCDNeckEnd);
     fEqnIndicesInner.push_back(99); fEqnIndicesOuter.push_back(99); // No equations, use the radius
     fHornNeckOuterRadius = aPlacementHandler->GetHorn1NeckOuterRadius(); 
     fHornNeckInnerRadius = aPlacementHandler->GetHorn1NeckInnerRadius(); 
     const double zEndNR = aPlacementHandler->GetHorn1ZDEndNeckRegion();
     fZDCBegin.push_back(zCDNeckEnd); fZDCEnd.push_back(zEndNR); 
     fEqnIndicesInner.push_back(3); fEqnIndicesOuter.push_back(7);
     const double zEnd = aPlacementHandler->GetHorn1ZEndIC();
     fZDCBegin.push_back(zEndNR); fZDCEnd.push_back(zEnd);
     fEqnIndicesInner.push_back(4); fEqnIndicesOuter.push_back(7);
     fEffectiveLength = aPlacementHandler->GetHorn1EffectiveLength();
     fOuterRadius = aPlacementHandler->GetHorn1OuterTubeOuterRad();
  } else {
     const LBNEVolumePlacementData *plDat = aPlacementHandler->Find("Bfield", 
                            "Horn2TopLevel", "LBNEMagneticFieldHorn::LBNEMagneticFieldHorn");
//     std::cerr << " Number of equation changes " << aPlacementHandler->GetHorn2ZEqnChangeNumEqn() << std::endl;
//     for (size_t k=0; k != aPlacementHandler->GetHorn2ZEqnChangeNumEqn(); k++) {
//       std::cerr << " Horn2 zone " << k << " at z " 
//                 << aPlacementHandler->GetHorn2ZEqnChange(k)/25.4 << std::endl;
//     }
     fEffectiveLength = plDat->fParams[2];
     const double z1 = 	aPlacementHandler->GetHorn2ZEqnChange(0);	    
     fZDCBegin.push_back(0.); fZDCEnd.push_back(z1); 
     fEqnIndicesInner.push_back(6); fEqnIndicesOuter.push_back(10); //IO trasnsition, Part 1
      
     const double z2 = 	aPlacementHandler->GetHorn2ZEqnChange(1);	    
     fZDCBegin.push_back(z1); fZDCEnd.push_back(z2); 
     fEqnIndicesInner.push_back(6);  fEqnIndicesOuter.push_back(0); // Getting to the neck region

     const double z3 = 	aPlacementHandler->GetHorn2ZEqnChange(2);	    
     fZDCBegin.push_back(z2); fZDCEnd.push_back(z3); 
     fEqnIndicesInner.push_back(6);  fEqnIndicesOuter.push_back(0); // Getting to the neck, neck region 

     const double z4 = 	aPlacementHandler->GetHorn2ZEqnChange(3);	    
     fZDCBegin.push_back(z3); fZDCEnd.push_back(z4); 
     fEqnIndicesInner.push_back(6);  fEqnIndicesOuter.push_back(1); // Just before the neck

     const double z5 = 	aPlacementHandler->GetHorn2ZEqnChange(4);	    
     fZDCBegin.push_back(z4); fZDCEnd.push_back(z5);     
     fEqnIndicesInner.push_back(99); fEqnIndicesOuter.push_back(99); //The neck, fixe radius
     fHornNeckOuterRadius = aPlacementHandler->GetHorn2NeckOuterRadius();
     fHornNeckInnerRadius = aPlacementHandler->GetHorn2NeckInnerRadius();

     const double z6 = 	aPlacementHandler->GetHorn2ZEqnChange(5);	    
     fZDCBegin.push_back(z5); fZDCEnd.push_back(z6); 
     fEqnIndicesInner.push_back(7);  fEqnIndicesOuter.push_back(2); // Just after the neck

     const double z7 = 	aPlacementHandler->GetHorn2ZEqnChange(6);	    
     fZDCBegin.push_back(z6); fZDCEnd.push_back(z7); 
     fEqnIndicesInner.push_back(7);  fEqnIndicesOuter.push_back(3); // moving along positive Z, neck region 

      const double z8 =  aPlacementHandler->GetHorn2ZEqnChange(7);  // 	Next physical section     
     fZDCBegin.push_back(z7); fZDCEnd.push_back(z8); 
     fEqnIndicesInner.push_back(7);  fEqnIndicesOuter.push_back(3);

      const double z9 =  aPlacementHandler->GetHorn2ZEqnChange(8);  // 	    
     fZDCBegin.push_back(z8); fZDCEnd.push_back(z9); 
     fEqnIndicesInner.push_back(8);  fEqnIndicesOuter.push_back(4);

      const double z10 =  aPlacementHandler->GetHorn2ZEqnChange(9);	    
     fZDCBegin.push_back(z9); fZDCEnd.push_back(z10); 
     fEqnIndicesInner.push_back(9);  fEqnIndicesOuter.push_back(5);
     
     fOuterRadius = aPlacementHandler->GetHorn2OuterTubeOuterRad();

  }
  fOuterRadiusEff = fOuterRadius - 2.0*fSkinDepth; // skin depth at 0.43 kHz 
  // Z coordinate change not yet initialize, done at the first track.
/*
  std::cerr << " Table of Z position and equations for ";
  if (amHorn1) std::cerr << " Horn1 " ;
  else  std::cerr << " Horn2 " ;
  std::cerr << std::endl <<  " Z-Start      Z-End      Innner Eqn    Outer Eqn rIC thick" << std::endl;
  for(size_t k=0; k!= fZDCBegin.size(); k++) {
    std::cerr << " " << fZDCBegin[k] << " " << fZDCEnd[k] 
              << " " << fEqnIndicesInner[k] << " " << fEqnIndicesOuter[k];
    const double zMid = 0.5 *( fZDCBegin[k] + fZDCEnd[k]);
    double rOut = fHornNeckOuterRadius; 
    double rIn =  fHornNeckInnerRadius; 
    if (fEqnIndicesInner[k]  != 99) {
      rOut = (amHorn1)  ? aPlacementHandler->GetConductorRadiusHorn1(zMid, fEqnIndicesOuter[k]) :
    	      aPlacementHandler->GetConductorRadiusHorn2(zMid, fEqnIndicesOuter[k]);
      rIn = (amHorn1)  ? aPlacementHandler->GetConductorRadiusHorn1(zMid, fEqnIndicesInner[k]) :
    	      aPlacementHandler->GetConductorRadiusHorn2(zMid, fEqnIndicesInner[k]);
    }
    std::cerr << " " << rIn << " " << rOut - rIn  << std::endl;
  }
*/
//
// Use now the survey data (simulated for now.. ) to establish the coordinate transform..  
//
  LBNESurveyor *theSurvey= LBNESurveyor::Instance();
  std::string hornIndexStr = (amHorn1) ? std::string("1") : std::string("2"); 
  std::string upstrStr("Upstream");std::string downstrStr("Downstream");
  std::string leftStr("Left"); std::string rightStr("Right"); 
  std::string ballStr("BallHorn");
  std::vector<LBNESurveyedPt>::const_iterator itUpLeft=
    theSurvey->GetPoint(G4String(upstrStr + leftStr + ballStr + hornIndexStr));
  std::vector<LBNESurveyedPt>::const_iterator itUpRight=
    theSurvey->GetPoint(G4String(upstrStr + rightStr + ballStr + hornIndexStr));
  std::vector<LBNESurveyedPt>::const_iterator itDwLeft=
    theSurvey->GetPoint(G4String(downstrStr + leftStr + ballStr + hornIndexStr));
  std::vector<LBNESurveyedPt>::const_iterator itDwRight=
    theSurvey->GetPoint(G4String(downstrStr + rightStr + ballStr + hornIndexStr));
  const G4ThreeVector pUpLeft = itUpLeft->GetPosition();
  const G4ThreeVector pUpRight = itUpRight->GetPosition();
  const G4ThreeVector pDwLeft = itDwLeft->GetPosition();
  const G4ThreeVector pDwRight = itDwRight->GetPosition();
  for (size_t k=0; k!=2; k++) {
    fShift[k] = -0.50*(pUpLeft[k] + pUpRight[k]); // minus sign comes from the global to local. 
                                                  // Transverse shoft at the upstream  entrance of Horn1 
    fShiftSlope[k] = 0.5*(pUpLeft[k] + pUpRight[k] -  pDwLeft[k] - pDwRight[k])/fEffectiveLength; 
  }
  
}

void LBNEMagneticFieldHorn::GetFieldValue(const double Point[3],double *Bfield) const
{

  
   G4double current = fHornCurrent/ampere/1000.;
//   std::cerr << " LBNEMagneticFieldHorn::GetFieldValue current " << current << std::endl;
   for (size_t k=0; k!=3; ++k) Bfield[k] = 0.;
   // Intialization of the coordinate transform. 
   const LBNEVolumePlacements *aPlacementHandler = LBNEVolumePlacements::Instance();
   if (!fCoordinateSet) {

     G4Navigator* numinavigator=new G4Navigator(); //geometry navigator
     G4Navigator* theNavigator=G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
     numinavigator->SetWorldVolume(theNavigator->GetWorldVolume());
     G4ThreeVector Position=G4ThreeVector(Point[0],Point[1],Point[2]); 
     G4VPhysicalVolume* myVolume = numinavigator->LocateGlobalPointAndSetup(Position);
     G4TouchableHistoryHandle aTouchable = numinavigator->CreateTouchableHistoryHandle();
     G4ThreeVector localPosition = aTouchable->GetHistory()->GetTopTransform().TransformPoint(Position);
     delete numinavigator;
     G4String vName = myVolume->GetLogicalVolume()->GetName();
     if (amHorn1 ) {
       if (vName != G4String("Horn1Hall") || (localPosition[2] > 0.)) return; // not there yet.., or getting by the back side.
	fZShiftUpstrWorldToLocal = Point[2]; // to correct for the misalignment, via the first coordinate transform
	 // Note: because the logival Horn1 container volume is split into to of them, 
	 // the offset for the field might a bit different than for the actual volume. This is a small correction. 
	 // What matter most the field is the tilt, which is correct. 
        fZShiftDrawingCoordinate = Point[2] - aPlacementHandler->GetHorn1DeltaZEntranceToZOrigin(); // Check the sign ! 
     } else {
        if (vName != G4String("Horn2TopLevel") || (localPosition[2] > 0.)) return; 
 	fZShiftUpstrWorldToLocal = Point[2]; // to correct for the misalignment, via the first coordinate transform
        fZShiftDrawingCoordinate = Point[2] - aPlacementHandler->GetHorn2DeltaZEntranceToZOrigin();
     }
     fCoordinateSet = true;
//     std::cerr << " Coordinate transform, Z shifts at Z World  " << Point[2] << " in  " << vName << std::endl;
//     std::cerr << " fZShiftUpstrWorldToLocal " << 
//                    fZShiftUpstrWorldToLocal << " fZShiftDrawingCoordinate " << fZShiftDrawingCoordinate << std::endl;
//      if (!amHorn1) { std::cerr << " And quit  !!!! " << std::endl; exit(2); }
//      this->dumpField();
   } // Initialization of Z coordinate transform 
   if (!fCoordinateSet) return;
   std::vector<double> ptTrans(2,0.);
   const double zLocal = Point[2] - fZShiftUpstrWorldToLocal; // from the start of the relevant volume 
   for (size_t k=0; k != 2; k++) 
     ptTrans[k] = Point[k] + fShift[k] + zLocal*fShiftSlope[k]; 
   const double r = std::sqrt(ptTrans[0]*ptTrans[0] + ptTrans[1]*ptTrans[1]); 
   if (r > fOuterRadius) return;
   if ( r < 1.0e-3) return; // The field should go smoothly to zero at the center of the horm 

   double magBField = current / (5.*r/cm)/10*tesla; //B(kG)=i(kA)/[5*r(cm)], 1T=10kG
   
   if ( r > fOuterRadiusEff) {
     const double dr = r - fOuterRadiusEff;
     const double attLength  = std::exp(-dr/fSkinDepth);
     magBField *= attLength;
   } else { 
     size_t kSelZ = fEqnIndicesInner.size();
     const double zLocD = Point[2] - fZShiftDrawingCoordinate;
     for (size_t k=0; k != fEqnIndicesInner.size(); ++k) {
       if (zLocD < fZDCBegin[k]) break;
       if (zLocD > fZDCEnd[k]) continue; // They are Z ordered.. 
       kSelZ = k; 
       break;
     }
     if (kSelZ ==  fEqnIndicesInner.size()) return;
     double radIC = fHornNeckInnerRadius;
     double radOC = fHornNeckOuterRadius;
     if (fEqnIndicesInner[kSelZ] != 99) {
       radIC = (amHorn1) ? aPlacementHandler->GetConductorRadiusHorn1(zLocD,fEqnIndicesInner[kSelZ] ) : 
                        aPlacementHandler->GetConductorRadiusHorn2(zLocD, fEqnIndicesInner[kSelZ]);
       radOC = (amHorn1) ? aPlacementHandler->GetConductorRadiusHorn1(zLocD,fEqnIndicesOuter[kSelZ] ) : 
                        aPlacementHandler->GetConductorRadiusHorn2(zLocD, fEqnIndicesOuter[kSelZ]);
     }
     if ((radIC < 1.0e-3) || (radOC < 1.0e-3)) {
       std::ostringstream mStrStr;
       mStrStr << " Wrong equation index at Z " << Point[2] << std::endl;
       G4String mStr(mStrStr.str());
      G4Exception("LBNEMagneticFieldHorn::GetFieldValue", " ",  FatalErrorInArgument, mStr.c_str()); 
      return; 
     }
     if (r < radIC) return; // zero field region (Amps law). 
     // 
     if ((r > radIC) && (r < radOC)) {
      const double surfCyl = radOC*radOC - radIC*radIC;
      magBField *= (r*r - radIC*radIC)/surfCyl; // Assume uniform current density and apply Amps law. 
     }
     Bfield[0] = -magBField*ptTrans[1]/r;
     Bfield[1] = magBField*ptTrans[0]/r;
//     std::cerr << " Field region at Z " << Point[2] << " r = " << r 
//	       << " radIC " << radIC << " radOC " 
//	       << radOC << "zLocD " << zLocD << " magBField " 
//	       <<  magBField/tesla << " Bx " << Bfield[0]/tesla << std::endl;
   }
}
void LBNEMagneticFieldHorn::dumpField() const {
   const LBNEVolumePlacements *aPlacementHandler = LBNEVolumePlacements::Instance();
  std::string fName = (amHorn1) ? std::string("./FieldMapHorn1.txt") : std::string("./FieldMapHorn2.txt");
  std::ofstream fOut(fName.c_str());
  fOut << " z zd r bphi " << std::endl;
  double zStart = -500.;
  double zEnd = 4000.;
  double rMax = fOuterRadius + 50.0*mm;
  if (!amHorn1) { zStart = 6000.; zEnd = 11000.; }
  const int numZStep = 1000;
  const int numRStep= 200;
  const double zStep = (zEnd-zStart)/numZStep;
  const double rStep = rMax/numRStep;
  double point[3];
  for (int iZ=0; iZ !=numZStep; iZ++) { 
    double z = zStart +  iZ*zStep;
    point[2] = z;
    const double zLocD = point[2] - fZShiftDrawingCoordinate;
    double radIC = fHornNeckInnerRadius;
    size_t kSelZ = fEqnIndicesInner.size();
    for (size_t k=0; k != fEqnIndicesInner.size(); ++k) {
       if (zLocD < fZDCBegin[k]) break;
       if (zLocD > fZDCEnd[k]) continue; // They are Z ordered.. 
       kSelZ = k; 
       break;
    }
    if (kSelZ ==  fEqnIndicesInner.size()) continue;
    if (fEqnIndicesInner[kSelZ] != 99) {
       radIC = (amHorn1) ? aPlacementHandler->GetConductorRadiusHorn1(zLocD,fEqnIndicesInner[kSelZ] ) : 
                        aPlacementHandler->GetConductorRadiusHorn2(zLocD, fEqnIndicesInner[kSelZ]);
    }
    double r = radIC - 0.25*mm;
    // inside the conductor... 
    while (r < rMax)  {
      const double phi = 2.0*M_PI*G4RandFlat::shoot();
      point[0] = r*std::cos(phi);
      point[1] = r*std::sin(phi);
      double bf[3];
      this->GetFieldValue(point, &bf[0]);
      fOut << " " << z << "  " << zLocD << " " << r << " " 
           << std::sqrt(bf[0]*bf[0] + bf[1]*bf[1])/tesla << std::endl;
      if (r < (radIC + 5.0*mm)) r += 0.1*mm;
      else  r += rStep;
    }
  } 
  fOut.close();
}  
