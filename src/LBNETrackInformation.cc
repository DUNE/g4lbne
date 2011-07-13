//
// LBNETrackInformation.cc
//

#include "G4VProcess.hh"
#include "LBNETrackInformation.hh"
#include "G4ios.hh"

G4Allocator<LBNETrackInformation> aTrackInformationAllocator;

LBNETrackInformation::LBNETrackInformation()
  :decay_code(0),
   tgen(0),
   Nimpwt(1.0)
{

}


LBNETrackInformation::LBNETrackInformation(const LBNETrackInformation* aTrackInfo)
{
  decay_code = aTrackInfo->decay_code;
  tgen = aTrackInfo->tgen;
  Nimpwt = aTrackInfo->Nimpwt;
}

LBNETrackInformation::~LBNETrackInformation(){}

void LBNETrackInformation::Print() const
{
    G4cout 
     << "Decay code = " << decay_code << G4endl;
    G4cout 
     << "tgen = " << tgen << G4endl;
    G4cout 
     << "nimpwt = " << Nimpwt << G4endl;
}

void LBNETrackInformation::Print(const G4Track *aTrack) const
{ 
  const G4String spaces = "   ";

  G4cout << spaces << "Track ID       = " << aTrack->GetTrackID()            << G4endl
	 << spaces << "Particle Name  = " << aTrack->GetDefinition()->GetParticleName()         << G4endl
	 << spaces << "Parent ID      = " << aTrack->GetParentID()           << G4endl;
  if(aTrack->GetVolume()) G4cout << spaces << "Current volume = " << aTrack->GetVolume()-> GetName() << G4endl;
  else                    G4cout << spaces << "Current volume = NOT CURRENTLY AVAILABLE" << G4endl;
  if(aTrack->GetCreatorProcess())  G4cout << spaces << "Creator Process = "<< aTrack->GetCreatorProcess()->GetProcessName() << G4endl;
  else                             G4cout << spaces << "Creator Process = NOT CURRENTLY AVAILABLE" << G4endl;


}
