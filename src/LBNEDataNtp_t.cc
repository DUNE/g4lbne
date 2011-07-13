//
// LBNEDataNtp_t.cc
//
//  ADM, July 2005
//  This is a class that defines the LBNEDataNtp_t object that is used to 
//  store the g4numi output in a root tree.
// $Id: LBNEDataNtp_t.cc,v 1.1 2011/07/13 16:20:52 loiacono Exp $
//------------------

#include "globals.hh"
#include "G4ios.hh"

#include "LBNEDataNtp_t.hh"
ClassImp(LBNEDataNtp_t)
//------------------------------------------------------------------------------
LBNEDataNtp_t::LBNEDataNtp_t()
{
   Clear();
}


//----------------------------------------------------------------------
LBNEDataNtp_t::~LBNEDataNtp_t()
{
  // LBNEDataNtp_t destructor
}

//----------------------------------------------------------------------
void LBNEDataNtp_t::Clear()
{
   run         = -999;       
   evtno       = -999;       
   protonN     = -999;
   beamHWidth  = -99999.; 
   beamVWidth  = -99999.;
   beamX       = -99999.;
   beamY       = -99999.;
   protonX     = -99999.;
   protonY     = -99999.;
   protonZ     = -99999.;
   protonPx    = -99999.;
   protonPy    = -99999.;
   protonPz    = -99999.;
   nuTarZ      = -99999.;
   hornCurrent = -99999.;
   Ndxdz       = -99999.;
   Ndydz       = -99999.;
   Npz         = -99999.;
   Nenergy     = -99999.;
   Norig       = -999;
   Ndecay      = -999;
   Ntype       = -999;
   Vx          = -99999.;
   Vy          = -99999.;
   Vz          = -99999.;
   pdPx        = -99999.;
   pdPy        = -99999.;
   pdPz        = -99999.;
   ppdxdz      = -99999.;
   ppdydz      = -99999.;
   pppz        = -99999.;
   ppenergy    = -99999.;
   ppmedium    = -99999.;
   ptype       = -999;
   ptrkid      = -999;
   ppvx        = -99999.;
   ppvy        = -99999.;
   ppvz        = -99999.;
   muparpx     = -99999.;
   muparpy     = -99999.;
   muparpz     = -99999.;
   mupare      = -99999.;
   Necm        = -99999.;
   Nimpwt      = -99999.;
   xpoint      = -99999.;
   ypoint      = -99999.;
   zpoint      = -99999.;
   tvx         = -99999.;
   tvy         = -99999.;
   tvz         = -99999.;
   tpx         = -99999.;
   tpy         = -99999.;
   tpz         = -99999.;
   tptype      = -999;
   tgen        = -999;
   


  
  for( Int_t index=0; index<5; ++index)
  {
    NdxdzNear[index] = -99999.;
    NdydzNear[index] = -99999.;
    NenergyN[index]  = -99999.;
    NWtNear[index]   = -99999.;
  }

  for ( Int_t index=0; index<3; ++index)
  {
    NdxdzFar[index] = -99999.;
    NdydzFar[index] = -99999.;
    NenergyF[index] = -99999.;
    NWtFar[index]   = -99999.;
  }

  fTrkPtMap.clear();

   
}

//----------------------------------------------------------------------
void LBNEDataNtp_t::AddTrkPoint(TrkPoint::TrkPoint_t trkpt, TrackPoint_t &tp)
{
   /*std::cout << "LBNEDataNtp" << std::endl;
   std::cout << "type   = " << tp.type << std::endl;
   std::cout << "  x    = " << tp.x << std::endl;
   std::cout << "  y    = " << tp.y << std::endl;
   std::cout << "  z    = " << tp.z << std::endl;
   std::cout << " px    = " << tp.px << std::endl;
   std::cout << " py    = " << tp.py << std::endl;
   std::cout << " pz    = " << tp.pz << std::endl;
   */

   TrkPtMap::iterator tpit = fTrkPtMap.find(trkpt);
   if(tpit == fTrkPtMap.end())
   {
      TrackPoint_t trackpoint;
      tpit = fTrkPtMap.insert(TrkPtMap::value_type(trkpt, trackpoint)).first;
   }

   TrackPoint_t &trackpoint = (tpit -> second);
   trackpoint = tp;

/*
std::cout << "LBNEDataNtp TrackPoint in G4LBNE data after assignment - " << std::endl;
   std::cout << "             type   = " << trackpoint.type << std::endl;
   std::cout << "               x    = " << trackpoint.x << std::endl;
   std::cout << "               y    = " << trackpoint.y << std::endl;
   std::cout << "               z    = " << trackpoint.z << std::endl;
   std::cout << "               px   = " << trackpoint.px << std::endl;
   std::cout << "               py   = " << trackpoint.py << std::endl;
   std::cout << "               pz   = " << trackpoint.pz << std::endl;

   trackpoint.Print();
*/
}


//----------------------------------------------------------------------
void LBNEDataNtp_t::AddTrkPoint(std::string &strkpt, TrackPoint_t &tp)
{

   TrkPoint::TrkPoint_t trkpt = TrkPoint::StringToEnum(strkpt);
   TrkPtMap::iterator tpit = fTrkPtMap.find(trkpt);
   if(tpit == fTrkPtMap.end())
   {
      TrackPoint_t trackpoint;
      tpit = fTrkPtMap.insert(TrkPtMap::value_type(trkpt, trackpoint)).first;
   }

   TrackPoint_t &trackpoint = (tpit -> second);
   trackpoint = tp;
}

//----------------------------------------------------------------------
void LBNEDataNtp_t::AddTrkPoint(TrkPoint::TrkPoint_t trkpt, int type, 
			       int trkid, int gen, double impwt, 
			       float x, float y, float z, 
			       float px, float py, float pz)
{
   /*std::cout << "LBNEDataNtp" << std::endl;
   std::cout << "type   = " << type << std::endl;
   std::cout << "  x    = " << x << std::endl;
   std::cout << "  y    = " << y << std::endl;
   std::cout << "  z    = " << z << std::endl;
   std::cout << " px    = " << px << std::endl;
   std::cout << " py    = " << py << std::endl;
   std::cout << " pz    = " << pz << std::endl;
   */

   TrkPtMap::iterator tpit = fTrkPtMap.find(trkpt);
   if(tpit == fTrkPtMap.end())
   {
      TrackPoint_t trackpoint;
      tpit = fTrkPtMap.insert(TrkPtMap::value_type(trkpt, trackpoint)).first;
   }

   TrackPoint_t &trackpoint = (tpit -> second);
   trackpoint.type  = type;
   trackpoint.trkid = trkid;
   trackpoint.gen   = gen;  
   trackpoint.impwt = impwt;  
   trackpoint.x = x;
   trackpoint.y = y;
   trackpoint.z = z;
   trackpoint.px = px;
   trackpoint.py = py;
   trackpoint.pz = pz;
   
/*
   std::cout << "LBNEDataNtp TrackPoint in G4LBNE data after assignment - " << std::endl;
   std::cout << "             type   = " << trackpoint.type << std::endl;
   std::cout << "               x    = " << trackpoint.x << std::endl;
   std::cout << "               y    = " << trackpoint.y << std::endl;
   std::cout << "               z    = " << trackpoint.z << std::endl;
   std::cout << "               px   = " << trackpoint.px << std::endl;
   std::cout << "               py   = " << trackpoint.py << std::endl;
   std::cout << "               pz   = " << trackpoint.pz << std::endl;


   trackpoint.Print();
*/
}

//----------------------------------------------------------------------
void LBNEDataNtp_t::AddTrkPoint(std::string &strkpt, int type, 
			       int trkid, int gen, double impwt, 
			       float x, float y, float z, 
			       float px, float py, float pz)
{
   TrkPoint::TrkPoint_t trkpt = TrkPoint::StringToEnum(strkpt);
   TrkPtMap::iterator tpit = fTrkPtMap.find(trkpt);
   if(tpit == fTrkPtMap.end())
   {
      TrackPoint_t trackpoint;
      tpit = fTrkPtMap.insert(TrkPtMap::value_type(trkpt, trackpoint)).first;
   }
   
   TrackPoint_t &trackpoint = (tpit -> second);
   trackpoint.type  = type;
   trackpoint.trkid = trkid;
   trackpoint.gen   = gen;  
   trackpoint.impwt = impwt;  
   trackpoint.x = x;
   trackpoint.y = y;
   trackpoint.z = z;
   trackpoint.px = px;
   trackpoint.py = py;
   trackpoint.pz = pz;
}

/*
//----------------------------------------------------------------------
void LBNEDataNtp_t::AddTrkPointParticleType(TrkPoint::TrkPoint_t &trkpt, int type)
{
}

//----------------------------------------------------------------------
void LBNEDataNtp_t::AddTrkPointTrackID(TrkPoint::TrkPoint_t &trkpt, int trkid)
{
}

//----------------------------------------------------------------------
void LBNEDataNtp_t::AddTrkPointPosition(TrkPoint::TrkPoint_t &trkpt, 
				       float x, float y, float z)
{
}
//----------------------------------------------------------------------
void LBNEDataNtp_t::AddTrkPointMomentum(TrkPoint::TrkPoint_t &trkpt, 
				       float px, float py, float pz)
{
}
*/


//********************************************************************
//********************************************************************
//Access to Track Point Map (see inlined functions too)

//----------------------------------------------------------------------
const TrackPoint_t& LBNEDataNtp_t::GetTrkPtAt(int key) const
{
   //
   // Return data at key, if key exits 
   //

   if(!(LBNEDataNtp_t::TrkPtKeyExists(key)))
   {
      std::cout << "LBNEDataNtp_t::TrkPtDataAt() - PROBLEM: Key " 
	   << key << " doesn't exist in the TrackPoint Map." 
	   << " MUST call LBNEDataNtp_t::TrkPtKeyExists() before "
	   << "calling this function. GOING TO CRASH NOW." << std::endl;

   }

   TrkPtMapIter it = fTrkPtMap.find(key);

   //const TrkPtMapIter it = std::find(fTrkPtMap.begin(), fTrkPtMap.end(), key);

   return it -> second;
}


//----------------------------------------------------------------------
int LBNEDataNtp_t::GetTrkPtParticleTypeAt(int key) const
{
   //
   // Return data at key, if key exits 
   //

   if(!(LBNEDataNtp_t::TrkPtKeyExists(key)))
   {
      std::cout << "LBNEDataNtp_t::GetTrkPtParticleTypeAt() - PROBLEM: Key " 
	   << key << " doesn't exist in the TrackPoint Map." 
	   << " MUST call LBNEDataNtp_t::TrkPtKeyExists() before "
	   << "calling this function. GOING TO CRASH NOW." << std::endl;

   }

   TrkPtMapIter it = fTrkPtMap.find(key);

   const TrackPoint_t& trackpoint = (it -> second);

   return trackpoint.GetParticleType();
}

//----------------------------------------------------------------------
int LBNEDataNtp_t::GetTrkPtTrackIDAt(int key) const
{
   //
   // Return data at key, if key exits 
   //

   if(!(LBNEDataNtp_t::TrkPtKeyExists(key)))
   {
      std::cout << "LBNEDataNtp_t::GetTrkPtTrackIDAt() - PROBLEM: Key " 
	   << key << " doesn't exist in the TrackPoint Map." 
	   << " MUST call LBNEDataNtp_t::TrkPtKeyExists() before "
	   << "calling this function. GOING TO CRASH NOW." << std::endl;

   }

   TrkPtMapIter it = fTrkPtMap.find(key);

   const TrackPoint_t& trackpoint = (it -> second);

   return trackpoint.GetTrackID();
}
//----------------------------------------------------------------------
int LBNEDataNtp_t::GetTrkPtGenerationAt(int key) const
{
   //
   // Return data at key, if key exits 
   //

   if(!(LBNEDataNtp_t::TrkPtKeyExists(key)))
   {
      std::cout << "LBNEDataNtp_t::GetTrkPtGenerationAt() - PROBLEM: Key " 
	   << key << " doesn't exist in the TrackPoint Map." 
	   << " MUST call LBNEDataNtp_t::TrkPtKeyExists() before "
	   << "calling this function. GOING TO CRASH NOW." << std::endl;

   }

   TrkPtMapIter it = fTrkPtMap.find(key);

   const TrackPoint_t& trackpoint = (it -> second);

   return trackpoint.GetGeneration();
}
//----------------------------------------------------------------------
double LBNEDataNtp_t::GetTrkPtImpWeightAt(int key) const
{
   //
   // Return data at key, if key exits 
   //

   if(!(LBNEDataNtp_t::TrkPtKeyExists(key)))
   {
      std::cout << "LBNEDataNtp_t::GetTrkPtImpWeightAt() - PROBLEM: Key " 
	   << key << " doesn't exist in the TrackPoint Map." 
	   << " MUST call LBNEDataNtp_t::TrkPtKeyExists() before "
	   << "calling this function. GOING TO CRASH NOW." << std::endl;

   }

   TrkPtMapIter it = fTrkPtMap.find(key);

   const TrackPoint_t& trackpoint = (it -> second);

   return trackpoint.GetImpWeight();
}

//----------------------------------------------------------------------
float LBNEDataNtp_t::GetTrkPtPositionAt(int key, const std::string &pos) const
{
   //
   // Return data at key, if key exits 
   //

   if(!(LBNEDataNtp_t::TrkPtKeyExists(key)))
   {
      std::cout << "LBNEDataNtp_t::GetTrkPtPositionAt() - PROBLEM: Key " 
	   << key << " doesn't exist in the TrackPoint Map." 
	   << " MUST call LBNEDataNtp_t::TrkPtKeyExists() before "
	   << "calling this function. GOING TO CRASH NOW." << std::endl;

   }

   TrkPtMapIter it = fTrkPtMap.find(key);

   const TrackPoint_t& trackpoint = (it -> second);

   return trackpoint.GetPosition(pos);
}

//----------------------------------------------------------------------
float LBNEDataNtp_t::GetTrkPtMomentumAt(int key, const std::string &mpos) const
{
   //
   // Return data at key, if key exits 
   //

   if(!(LBNEDataNtp_t::TrkPtKeyExists(key)))
   {
      std::cout << "LBNEDataNtp_t::GetTrkPtMomentumAt() - PROBLEM: Key " 
	   << key << " doesn't exist in the TrackPoint Map." 
	   << " MUST call LBNEDataNtp_t::TrkPtKeyExists() before "
	   << "calling this function. GOING TO CRASH NOW." << std::endl;

   }

   TrkPtMapIter it = fTrkPtMap.find(key);

   const TrackPoint_t& trackpoint = (it -> second);

   return trackpoint.GetMomentum(mpos);
}

//end Acess to TrackPoint Map
//********************************************************************
//********************************************************************

















//-------------------------------------------------------------------------------------
TrkPoint::TrkPoint_t TrkPoint::StringToEnum(const std::string &trkpt)
{
   if      (trkpt == TrkPoint::AsString(TrkPoint::kTarget)          || trkpt == "kTarget")         return kTarget;
   else if (trkpt == TrkPoint::AsString(TrkPoint::kTargetExit)      || trkpt == "kTargetExit")     return kTargetExit;
   else if (trkpt == TrkPoint::AsString(TrkPoint::kTargetEndPlane)  || trkpt == "kTargetEndPlane")  return  kTargetEndPlane;
   else if (trkpt == TrkPoint::AsString(TrkPoint::kHorn1Enter)      || trkpt == "kHorn1Enter")      return kHorn1Enter;
   else if (trkpt == TrkPoint::AsString(TrkPoint::kHorn1NeckPlane)  || trkpt == "kHorn1NeckPlane")  return kHorn1NeckPlane;
   else if (trkpt == TrkPoint::AsString(TrkPoint::kHorn1Exit)       || trkpt == "kHorn1Exit")       return kHorn1Exit;
   else if (trkpt == TrkPoint::AsString(TrkPoint::kHorn1EndPlane)   || trkpt == "kHorn1EndPlane")   return kHorn1EndPlane;
   else if (trkpt == TrkPoint::AsString(TrkPoint::kHorn2Enter)      || trkpt == "kHorn2Enter")      return kHorn2Enter;
   else if (trkpt == TrkPoint::AsString(TrkPoint::kHorn2NeckPlane)  || trkpt == "kHorn2NeckPlane")  return kHorn2NeckPlane;
   else if (trkpt == TrkPoint::AsString(TrkPoint::kHorn2Exit)       || trkpt == "kHorn2Exit")       return kHorn2Exit;
   else if (trkpt == TrkPoint::AsString(TrkPoint::kHorn2EndPlane)   || trkpt == "kHorn2EndPlane")   return kHorn2EndPlane;
   else if (trkpt == TrkPoint::AsString(TrkPoint::kDPipeEnter)      || trkpt == "kDPipeEnter")         return kDPipeEnter;
   else if (trkpt == TrkPoint::AsString(TrkPoint::kDPipeExit)       || trkpt == "kDPipeExit")          return kDPipeExit;

   else if (trkpt == TrkPoint::AsString(TrkPoint::kUnknown)          || trkpt == "kUnknown")        return kUnknown;
   else return kUnknown;
}

//-------------------------------------------------------------------------------------
TrkPoint::TrkPoint_t TrkPoint::IntToEnum(int trkpt)
{
//returns enum
  switch (trkpt)
    {
    case 1:                return kTarget;                  break;
    case 2:                return kTargetExit;              break;
    case 3:                return kTargetEndPlane;          break;
    case 11:               return kHorn1Enter;              break;
    case 12:               return kHorn1NeckPlane;          break;
    case 13:               return kHorn1Exit;               break;
    case 14:               return kHorn1EndPlane;           break;
    case 21:               return kHorn2Enter;              break;
    case 22:               return kHorn2NeckPlane;          break;
    case 23:               return kHorn2Exit;               break;
    case 24:               return kHorn2EndPlane;           break;
    case 31:               return kDPipeEnter;              break;
    case 32:               return kDPipeExit;               break; 
    case -99:              return kUnknown;                 break;
    default:               return kUnknown;                 break;
    } 
}

//-------------------------------------------------------------------------------------
std::string TrkPoint::IntToString(int trkpt)
{
   return TrkPoint::AsString(TrkPoint::IntToEnum(trkpt));
}

//-------------------------------------------------------------------------------------
int TrkPoint::StringToInt(const std::string &trkpt)
{
   return TrkPoint::AsInt(TrkPoint::StringToEnum(trkpt));
}

//-------------------------------------------------------------------------------------
std::string TrkPoint::AsString(TrkPoint_t trkpt)
{
   switch (trkpt)
   {
   case kTarget             : return  "Target";           break;
   case kTargetExit         : return  "TargetExit";       break; 
   case kTargetEndPlane     : return  "TargetEndPlane";   break;
   case kHorn1Enter         : return  "Horn1Enter";       break;
   case kHorn1NeckPlane     : return  "Horn1NeckPlane";   break;
   case kHorn1Exit          : return  "Horn1Exit";        break;
   case kHorn1EndPlane      : return  "Horn1EndPlane";    break;
   case kHorn2Enter         : return  "Horn2Enter";       break;
   case kHorn2NeckPlane     : return  "Horn2NeckPlane";   break;
   case kHorn2Exit          : return  "Horn2Exit";        break;
   case kHorn2EndPlane      : return  "Horn2EndPlane";    break;
   case kDPipeEnter         : return  "DPipeEnter";       break;
   case kDPipeExit          : return  "DPipeExit";        break; 
   case kUnknown            : return  "Unknown";          break;       
   default                  : return  "Unknown";          break;
   }
}

//-------------------------------------------------------------------------------------
std::string TrkPoint::AsLabel(TrkPoint_t trkpt)
{
   switch (trkpt)
   {
   case kTarget             : return  "Target";           break;
   case kTargetExit         : return  "TargetExit";       break; 
   case kTargetEndPlane     : return  "TargetEndPlane";   break;
   case kHorn1Enter         : return  "Horn1Enter";       break;
   case kHorn1NeckPlane     : return  "Horn1NeckPlane";   break;
   case kHorn1Exit          : return  "Horn1Exit";        break;
   case kHorn1EndPlane      : return  "Horn1EndPlane";    break;
   case kHorn2Enter         : return  "Horn2Enter";       break;
   case kHorn2NeckPlane     : return  "Horn2NeckPlane";   break;
   case kHorn2Exit          : return  "Horn2Exit";        break;
   case kHorn2EndPlane      : return  "Horn2EndPlane";    break;
   case kDPipeEnter         : return  "DPipeEnter";       break;
   case kDPipeExit          : return  "DPipeExit";        break; 
   case kUnknown            : return  "Unknown";          break;       
   default                  : return  "Unknown";          break;
   }

}

//-------------------------------------------------------------------------------------
int TrkPoint::AsInt(TrkPoint_t trkpt)
{
   switch (trkpt)
   {
   case kTarget             : return  1;    break;
   case kTargetExit         : return  2;    break; 
   case kTargetEndPlane     : return  3;    break;
   case kHorn1Enter         : return  11;   break;
   case kHorn1NeckPlane     : return  12;   break;
   case kHorn1Exit          : return  13;   break;
   case kHorn1EndPlane      : return  14;   break;
   case kHorn2Enter         : return  21;   break;
   case kHorn2NeckPlane     : return  22;   break;
   case kHorn2Exit          : return  23;   break;
   case kHorn2EndPlane      : return  24;   break;
   case kDPipeEnter         : return  31;   break;
   case kDPipeExit          : return  32;   break; 
   case kUnknown            : return  -99;  break;       
   default                  : return  -99;  break;
   }
}

//-------------------------------------------------------------------------------------
bool TrkPoint::Overlap(const TrkPoint_t lhs, const TrkPoint_t rhs)
{
   if(lhs == kUnknown || rhs == kUnknown)
   {
      std::cout << "LBNEDataNtp_t.cxx: TrkPoint::Overlap - TrkPoint is kUnknown." << std::endl;
      return false;
   }

   if(lhs == rhs)
   {
      return true;
   }
   else
      return false;

}
