//
// LBNETrackInformation.hh
//
#ifndef LBNETrackInformation_h
#define LBNETrackInformation_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "G4Allocator.hh"
#include "G4VUserTrackInformation.hh"

class LBNETrackInformation : public G4VUserTrackInformation 
{
  public:
    LBNETrackInformation();
    LBNETrackInformation(const LBNETrackInformation* aTrackInfo);
    virtual ~LBNETrackInformation();
  
    inline void *operator new(size_t);
    inline void operator delete(void *aTrackInfo);
    inline int operator ==(const LBNETrackInformation& right) const
    {return (this==&right);}

    inline G4int GetDecayCode() const 
    {return decay_code;}
    inline void SetDecayCode(G4int decaycode)
    {decay_code=decaycode;} 
  
    inline G4int GetTgen() const 
    {return tgen;}
    inline void SetTgen(G4int tgeneration)
    {tgen=tgeneration;}

    inline G4double GetNImpWt() const 
    {return Nimpwt;}
    inline void SetNImpWt(G4double nimpweight)
    {Nimpwt=nimpweight;}
    
    void Print() const;

  void Print(const G4Track *aTrack) const;

  private:
    G4int              decay_code;
    G4int              tgen;
    G4double           Nimpwt;

};

extern G4Allocator<LBNETrackInformation> aTrackInformationAllocator;

inline void* LBNETrackInformation::operator new(size_t)
{ void* aTrackInfo;
 aTrackInfo = (void*)aTrackInformationAllocator.MallocSingle();
 return aTrackInfo;
}

inline void LBNETrackInformation::operator delete(void *aTrackInfo)
{ aTrackInformationAllocator.FreeSingle((LBNETrackInformation*)aTrackInfo);}

#endif

