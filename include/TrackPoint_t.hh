//----------------------------------------------------------------------
//
// ADM, July 2005
// the g4numi output in a root tree.
//
// $Id: TrackPoint_t.hh,v 1.1.1.1 2011/07/13 16:20:52 loiacono Exp $
//----------------------------------------------------------------------

#ifndef TRACKPOINT_T_HH
#define TRACKPOINT_T_HH

//C++
#include <vector>
#include <map>
#include <iostream>
#include <string>

#include "Rtypes.h" // include Float_t, Double_t, etc definitions
#include "TROOT.h"          // Top level (or root) structure for all classes
#include "TObject.h"



class  TrackPoint_t 
{
  
public:
   // a constructor and a destructor
   TrackPoint_t();
   virtual ~TrackPoint_t();

   void Print();

public: 

   Int_t    GetParticleType() const;
   Int_t    GetTrackID() const;
   Int_t    GetGeneration() const;
   Double_t GetImpWeight() const;
   
   Float_t GetPosition(int i) const;
   Float_t GetPosition(const std::string &pos) const;

   Float_t GetMomentum(int i) const;
   Float_t GetMomentum(const std::string &pos) const;
   
public:

   Int_t type; //particle type
   Int_t trkid; //track ID (unique ID per Event)
   Int_t gen;
   Double_t impwt;
   Float_t x;
   Float_t y;
   Float_t z;
   Float_t px;
   Float_t py;
   Float_t pz;
   

public:
   
   TrackPoint_t & operator= (const TrackPoint_t & rhs)
      {
	 if (this != &rhs) // protect against invalid self-assignment
	 { 
	    type  = rhs.type;
	    trkid = rhs.trkid;
	    gen   = rhs.gen;
            impwt = rhs.impwt;
	    x     = rhs.x;
	    y     = rhs.y;
	    z     = rhs.z;
	    px    = rhs.px;
	    py    = rhs.py;
	    pz    = rhs.pz;
	    
	 }
	 // by convention, always return *this
	 return *this;
      }

   
   
private:
   ClassDef(TrackPoint_t , 1)
   
      
      };

inline Int_t    TrackPoint_t::GetParticleType() const { return type; }
inline Int_t    TrackPoint_t::GetTrackID()      const { return trkid; }
inline Int_t    TrackPoint_t::GetGeneration()   const { return gen; }
inline Double_t TrackPoint_t::GetImpWeight()    const { return impwt; }

#endif 

