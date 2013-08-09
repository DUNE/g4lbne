#ifndef LBNESurveyor_H
#define LBNESurveyor_H 
/*
** This class, and it's subclasses (SurveyedPt), is an attempt at describing the 
** the process of surveying the LBNE target and neutrino beam elements. 
** It is not mean to be a general tool: Qunatities are specific to the LBNE beam layout. 
**
** The main class is a singleton. It can be refered by the LBNESubVolume class, when it comes to place 
** a sub volume. Unlike the SubVolume class, this class does not actively place volume in the hierarchy. 
** It simply manages the key surveyed points along the beam line. 
** 
** It is expected to be interface to a LBNESurveyorMessenger, derived class from G4Messenger class 
** to set things up.
**
** The constructor of this class (private, as it is a singleton class) simply sets all position to 
** (0. 0. 0.) and tolerance to a rediculously small number (i.e., by default things are perfectly aligned.) 
**
** Intended use: This would be a two step process:
**
**   1.  Prior to instantiatiated the G4 geometry, the container class LBNESurveyor is instantiated. 
**  Surveyed Points are added, one by one, by name.  This could be done in the LBNESurveyor contructor, 
** or else where in this package, provided that the G4Placement are not yet done. The goal is to 
** provide a survey "log book" 
**
**  2. Based on the above logbook, as one construct the G4 geometry, one enquire about the relative coordinates
** of the surveyed points, relative to the nominal base line. From this points, a rotation matrix and vector displacement 
** for the relevant volume can be computed, either in the  LBNESubVolume constructor, or any of the G4 geometry
** construction process, where the G4Placement occured. 
**
** Example: when it is time to place the Horn1 outer conductor (or a volume firmly attached to it) into 
** Horn1 hall (or a smaller box that fully contains it), one does 
**
** G4ThreeVector nominalPlacement (0., -5., -3471.);  // Where the z coordinate is either hardtyped, or obtained 
** from the subclass G4SubVolume. We have been told that a vertical offset of 5 mm is "really there.." 
** These are the nominal coordinates. They could be set by the messenger of the LBNESubvolume class, 
** if need be. 
**
** LBNESurveyor* mySurveyor = LBNESurveyor::GetInstance();
** std::vector<LBNESurveyedPt>::const_iterator itPinUpstream=mySurveyor->GetPoint("Horn1/UpstreamLeftPin");
** G4ThreeVector pinUpsL =  itPinUpstream->GetPosition();
** G4ThreeVector pinUpsR =  mySurveyor->GetPoint("Horn1/UpstreamRightPin")->GetPosition();
**
** ... Assume for now the downstream pin is perfectly aligned 
** deduce an overall placement:
** 
** G4ThreeVector honr1OuterPos = nominalPlacement + 0.5*(pinUpsL + pinUpsR);
** aHorn1OuterPhysical = new G4PVPlacement(0, honr1OuterPos,...) 
**
** Unlike LBNESubVolume class, the LBNESurveyor class has knowledge about the complete survey of the beam 
** line, and therefore can check relative alignment prior to the geometry construction. It could also contain
** surveyed point (monuments) that would have no correspondance to a corner, or a center,  of the G4Volume. 
**
** Paul L. G. Lebrun, June 2013. 
*/
#include <string>
#include <vector>
#include "G4String.hh"
#include "G4UImessenger.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4ThreeVector.hh"

class LBNESurveyedPt;

class LBNESurveyorMessenger: public G4UImessenger {
  
  public:
    LBNESurveyorMessenger();
    LBNESurveyorMessenger(LBNESurveyedPt* aPts, const G4String &aName, const G4String &description);
    ~LBNESurveyorMessenger();
    void SetNewValue(G4UIcommand *cmd, G4String val);

  private:
    std::string fFullName;
    LBNESurveyedPt* fPoint; // back pointer to the point in question... Could be null is this messenger is blank. 
    G4UIcmdWithABool*  fSetPositionFromToleranceCmd;
    G4UIcmdWith3Vector*  fToleranceCmd;
    G4UIcmdWith3Vector*  fPositionCmd;

};

class LBNESurveyedPt {
 public:
   LBNESurveyedPt();
   LBNESurveyedPt(const std::string &aName);
   LBNESurveyedPt(const std::string &aName, const G4ThreeVector &tolerance);
   
private:
   std::string fName;  // e.g., Horn1UpstreamPinLeftSide
   bool fPositionSetByTolerance; // 
   G4ThreeVector fTolerance;  // The expected tolerance on placing the object. 
                            // For instance, it is assumed the left side of the Horn1 alignment pin 
			    // will be known (i.e., surveyed) to within a 500 microns accuracy.
   G4ThreeVector fPosition;  // The actual position with respect to the nominal alignment.
                            //  That is, if the alignment is really really good, then fPosition goes towards the null vector. 

   LBNESurveyorMessenger fMessenger;

public:
   inline const std::string& GetName() const {return fName;}
   inline void SetTolerance(const  G4ThreeVector &tolerance) { fTolerance = tolerance; }
   inline void SetPosition(const  G4ThreeVector &pos) { fPosition = pos;}
   inline const G4ThreeVector& GetTolerance() const { return fTolerance; }
   inline const G4ThreeVector& GetPosition() const { return fPosition; }
   void SetPositionByTolerance(bool t);
   inline bool IsPositionSetByTolerance() const { return fPositionSetByTolerance;}

private:
   void defineMessenger();  
};

//
// The surveyor knows about the basic layout of the LBNE beam: there is one only one target, preceded by a baffle, 
// Two horns and so forth.  This the methods for this class are specific to LBNE beam line. 
//
class LBNESurveyor
{

private:
  LBNESurveyor();
  static LBNESurveyor* fInstance;
  std::vector<LBNESurveyedPt> fData;

public:
  LBNESurveyor* GetInstance();
  LBNESurveyor(LBNESurveyor const &); // not implemented 
  LBNESurveyor& operator=(LBNESurveyor const&); // not implemented 
  inline void AddPoint(const std::string &aName) { fData.push_back(LBNESurveyedPt(aName)); }
  inline void AddPoint(const std::string &aName, const G4ThreeVector tolerance) { fData.push_back(LBNESurveyedPt(aName, tolerance)); }
   
  void SetIt(); // Compute all position by tolerance, when applicable, 
  inline std::vector<LBNESurveyedPt>::const_iterator GetPoint(const std::string &aName) const {
    for(std::vector<LBNESurveyedPt>::const_iterator it=fData.begin(); it!=fData.end(); ++it) {
      if (aName == it->GetName()) return it;
    }
   return fData.end();
  }
  inline void setPointPosition(const std::string &aName, const G4ThreeVector &position) {
    for(std::vector<LBNESurveyedPt>::iterator it=fData.begin(); it!=fData.end(); ++it) {
      if (aName == it->GetName()) it->SetPosition(position);
    }
  }
  inline void setPointTolerance(const std::string &aName, const G4ThreeVector &tolerance) {
    for(std::vector<LBNESurveyedPt>::iterator it=fData.begin(); it!=fData.end(); ++it) {
      if (aName == it->GetName()) it->SetTolerance(tolerance);
    }
  }
  void setPointPositionWithinTolerance(const std::string &aName);
//  void SetTargetSegments(bool upstreamSegment, int model);  // Based on prescribed tolerances, and perhaps a model on how 
                                                              // the target alignment get modified as it heats up, 
							      // Now done in VolumePlacement. 
  void MoveTargetInOutHorn(double deltaZ); // To adjust the longitudinal position of the target. 

private:
  void SetThings(); // Update and implement constraints.    
  // This one should perhaps move to the VolumePlacements class.  			     
  bool TieTargetSegments(double maxRadialMove=50.); // Very specific to the LBNE beam secondary (pions) beam layout, where the upstream and 
                            // downstream target segments, which are implemented in two distinct subvolumes, (targetHall and Horn1, respectively), 
			    // must be, part of one continuous 3D object. The paramter maxRadialMove is the maximum allowed to adjust the
			    // transverse position of the elements. Default value is extremly large (5 cm.,), meaning that volume clashes are 
			    // likely if the tolerance is larger then the inner diameter of the Horn1. To be negotiated. 
			    // This maxRadial move applies to the downstream end of the target... 
};

#endif

