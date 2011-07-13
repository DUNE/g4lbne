//
// G4LBNEData_t.hh
//
//  ADM, July 2005
//  This is a class that defines the LBNEDataNtp_t object that is used to store the g4numi output
//   in a root tree.
//------------------
#ifndef LBNEDATANTP_T_HH
#define LBNEDATANTP_T_HH

//C++
#include <vector>
#include <map>
#include <iostream>
#include <string>

#include "Rtypes.h" // include Float_t, Double_t, etc definitions
#include "TROOT.h"          // Top level (or root) structure for all classes
#include "TObject.h"

#include "TrackPoint_t.hh"


namespace TrkPoint
{
   typedef enum ETrkPoint 
   {
      kTarget             = 1  ,
      kTargetExit         = 2  ,
      kTargetEndPlane     = 3  , //this one will only be valid if the target is not inside horn 1 at all
      kHorn1Enter         = 11  ,
      kHorn1NeckPlane     = 12  ,
      kHorn1Exit          = 13  ,
      kHorn1EndPlane      = 14  ,
      kHorn2Enter         = 21  ,
      kHorn2NeckPlane     = 22  ,      
      kHorn2Exit          = 23  ,
      kHorn2EndPlane      = 24  ,
      kDPipeEnter         = 31  ,
      kDPipeExit          = 32  ,
      kUnknown            = -99  
   } TrkPoint_t;
   
   TrkPoint_t  StringToEnum(const std::string &trkpt);
   TrkPoint_t  IntToEnum   (int trkpt);
   std::string IntToString (int trkpt);
   int         StringToInt (const std::string &trkpt);
   std::string AsString    (TrkPoint_t trkpt);
   std::string AsLabel     (TrkPoint_t trkpt);
   int         AsInt       (TrkPoint_t trkpt);
   

   bool Overlap(const TrkPoint_t lhs, const TrkPoint_t rhs);

}



class LBNEDataNtp_t 
{
  
public:
   // a constructor and a destructor
   LBNEDataNtp_t();
   virtual ~LBNEDataNtp_t();
   
   typedef std::map<int, TrackPoint_t> TrkPtMap;
   typedef TrkPtMap::const_iterator TrkPtMapIter;

   void Clear();


   void AddTrkPoint(TrkPoint::TrkPoint_t trkpt, TrackPoint_t &tp);
   void AddTrkPoint(std::string &strkpt, TrackPoint_t &tp);
   void AddTrkPoint(TrkPoint::TrkPoint_t trkpt, int type, int trkid, 
		    int gen, double impwt, float x, float y, float z, 
		    float px, float py, float pz);
   void AddTrkPoint(std::string &strkpt, int type, int trkid,  
		    int gen, double impwt, float x, float y, float z, 
		    float px, float py, float pz);
   /*
   void AddTrkPointTrackID(TrkPoint::TrkPoint_t &trkpt, int trkid);
   void AddTrkPointParticleType(TrkPoint::TrkPoint_t &trkpt, int type);
   void AddTrkPointPosition    (TrkPoint::TrkPoint_t &trkpt, 
				float x, float y, float z);
   void AddTrkPointMomentum    (TrkPoint::TrkPoint_t &trkpt, 
				float px, float py, float pz);
   */

   const TrkPtMap& GetTrackPointMap() const;

   bool  TrkPtKeyExists(int key) const;
   const TrackPoint_t& GetTrkPtAt(int key) const;

   int    GetTrkPtParticleTypeAt(int key) const;
   int    GetTrkPtTrackIDAt(int key)      const;
   int    GetTrkPtGenerationAt(int key)   const;
   double GetTrkPtImpWeightAt(int key)    const;
   float  GetTrkPtPositionAt(int key, const std::string &pos) const;
   float  GetTrkPtMomentumAt(int key, const std::string &mpos) const;


public:
   
  
   // the following variables are placed in the root tree
   Int_t run;       
   Int_t evtno;
   Int_t protonN;
   Float_t beamHWidth;
   Float_t beamVWidth;
   Float_t beamX;
   Float_t beamY;
   Float_t protonX;
   Float_t protonY;
   Float_t protonZ;
   Float_t protonPx;
   Float_t protonPy;
   Float_t protonPz;
   Float_t nuTarZ;
   Float_t hornCurrent;
   Float_t Ndxdz;
   Float_t Ndydz;
   Float_t Npz;
   Float_t Nenergy;
   Float_t NdxdzNear[5];
   Float_t NdydzNear[5];   
   Float_t NenergyN[5];    
   Double_t NWtNear[5];     
   Float_t NdxdzFar[3];    
   Float_t NdydzFar[3];   
   Float_t NenergyF[3];    
   Double_t NWtFar[3];    
   Int_t    Norig;
   Int_t    Ndecay;
   Int_t    Ntype;
   Float_t Vx;
   Float_t Vy;
   Float_t Vz;
   Float_t pdPx;
   Float_t pdPy;
   Float_t pdPz;
   Float_t ppdxdz;
   Float_t ppdydz;
   Float_t pppz;
   Float_t ppenergy;
   Float_t ppmedium;
   Int_t    ptype;
   Int_t    ptrkid;
   Float_t ppvx;
   Float_t ppvy;
   Float_t ppvz;
   Float_t muparpx;
   Float_t muparpy;
   Float_t muparpz;
   Float_t mupare;
   Float_t Necm;
   Double_t Nimpwt;
   Float_t xpoint;
   Float_t ypoint;
   Float_t zpoint;
   Float_t tvx;
   Float_t tvy;
   Float_t tvz;
   Float_t tpx;
   Float_t tpy;
   Float_t tpz;
   Int_t    tptype;
   Int_t    tgen;


private:

   TrkPtMap fTrkPtMap;
   

   
   
private:
   ClassDef(LBNEDataNtp_t ,1) // LBNEDataNtp_t
   
};

inline const LBNEDataNtp_t::TrkPtMap& LBNEDataNtp_t::GetTrackPointMap() const
{
   return fTrkPtMap;
}

inline bool LBNEDataNtp_t::TrkPtKeyExists(int key) const
{
   TrkPtMapIter tpit = fTrkPtMap.find(key);
   if(tpit == fTrkPtMap.end())
   {
      return false;
   }

   return true;
}



#endif 

