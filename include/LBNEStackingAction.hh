//
// LBNEStackingAction.hh
//

#ifndef LBNEStackingAction_H
#define LBNEStackingAction_H 1

#include "globals.hh"
#include "G4UserStackingAction.hh"
//#include "G4ThreeVector.hh"

class G4Track;
class LBNEDataInput;
class LBNERunManager;

class LBNEStackingAction : public G4UserStackingAction
{
  public:
    LBNEStackingAction();
    virtual ~LBNEStackingAction();

    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
    virtual void NewStage();
    virtual void PrepareNewEvent();


   void KillEMParticles(G4ClassificationOfNewTrack& classification, 
			const G4Track* aTrack);
   void KillltZeroPzParticles(G4ClassificationOfNewTrack& classification, 
			      const G4Track * aTrack);
   void KillThresholdParticles(G4ClassificationOfNewTrack& classification, 
			       const G4Track * aTrack);
   void KillOutOfWorldParticles(G4ClassificationOfNewTrack& classification, 
				const G4Track * aTrack);
   void KillUnimportantParticles(G4ClassificationOfNewTrack& classification, 
				 const G4Track * aTrack);

  private:
  LBNEDataInput * LBNEData;
   LBNERunManager * pRunManager; 
};

#endif

