//
// LBNETrackingAction.hh
//
#ifndef LBNETrackingAction_h
#define LBNETrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "LBNETrajectory.hh"

class LBNEDataInput;
class LBNERunManager;
class LBNEPrimaryGeneratorAction;
class G4EventManager;
class LBNEEventAction;

class LBNETrackingAction : public G4UserTrackingAction
{
  public:
    LBNETrackingAction();
    virtual ~LBNETrackingAction();

    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

   void SetPreLBNETrackInformation(const G4Track* aTrack);
   void SetPostLBNETrackInformation(const G4Track* aTrack);
   void AnalyzeIfNeutrino(const G4Track* aTrack);
   void AnalyzeIfMuon(const G4Track* aTrack);

  private:
   LBNEDataInput *fLBNEData;
   LBNERunManager *pRunManager;
   LBNEPrimaryGeneratorAction *NPGA;
   G4EventManager *EvtManager;
   LBNEEventAction *LBNEEvtAct;
};

#endif
