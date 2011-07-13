//----------------------------------------------------------------------
// Sets the relevant memebers of the data class for storing the
// MC data for the Hadron and Muon Monitors.
//
// $Id: TrackPoint_t.cc,v 1.1.1.1 2011/07/13 16:20:52 loiacono Exp $
//----------------------------------------------------------------------


#include "globals.hh"
#include "G4ios.hh"

#include "TrackPoint_t.hh"

ClassImp(TrackPoint_t)

//-----------------------------------------------------------------------------------
TrackPoint_t::TrackPoint_t()
  :type(-999),
   trkid(-999),
   gen(-99),
   impwt(1.0),
   x(-99999.0),
   y(-99999.0),
   z(-99999.0),
   px(-99999.0),
   py(-99999.0),
   pz(-99999.0)
   
{
   
}

//-----------------------------------------------------------------------------------
TrackPoint_t::~TrackPoint_t()
{
  // TrackPoint_t destructor
}


//------------------------------------------------------------------------------------------
Float_t TrackPoint_t::GetPosition(int i) const 
{
   if     (i == 0) return x;
   else if(i == 1) return y;
   else if(i == 2) return z;
   else 
   {
      std::cout << "TrackPoint_t::GetPosition - invalid position index " << i
	     << ". Valid indices are 0, 1, and 2. " << std::endl;
      return -999999.0;
   }
}

//------------------------------------------------------------------------------------------
Float_t TrackPoint_t::GetPosition(const std::string &pos) const
{
   if     (pos == "x" || pos == "X") return x;
   else if(pos == "y" || pos == "Y") return y;
   else if(pos == "z" || pos == "Z") return z;
   else 
   {
      std::cout << "TrackPoint_t::GetPosition - invalid position \"" << pos 
	     << "\". Valid positions are x, y, z, X, Y and Z. " << std::endl;
      return -999999.0;
   }
}

//------------------------------------------------------------------------------------------
Float_t TrackPoint_t::GetMomentum(int i) const
{
   if     (i == 0) return px;
   else if(i == 1) return py;
   else if(i == 2) return pz;
   else 
   {
      std::cout << "TrackPoint_t::GetMomentum - invalid momentum index " << i 
	     << ". Valid indices are 0, 1, and 2. " << std::endl;
      return -999999.0;
   }
}

//------------------------------------------------------------------------------------------
Float_t TrackPoint_t::GetMomentum(const std::string &pos) const
{
   if     (pos == "x" || pos == "X") return px;
   else if(pos == "y" || pos == "Y") return py;
   else if(pos == "z" || pos == "Z") return pz;
   else 
   {
      std::cout << "TrackPoint_t::GetMomentum - invalid momentum direction \"" << pos 
	     << "\". Valid directions are x, y, z, X, Y and Z. " << std::endl;
      return -999999.0;
   }
}

//------------------------------------------------------------------------------------------
void TrackPoint_t::Print()
{

   std::cout << "TrackPoint_t::Print - " << std::endl;
   std::cout << "             type   = " << type << std::endl;
   std::cout << "         track ID   = " << trkid << std::endl;
   std::cout << "       generation   = " << gen << std::endl;
   std::cout << "            impwt   = " << impwt << std::endl;
   std::cout << "               x    = " << x << std::endl;
   std::cout << "               y    = " << y << std::endl;
   std::cout << "               z    = " << z << std::endl;
   std::cout << "               px   = " << px << std::endl;
   std::cout << "               py   = " << py << std::endl;
   std::cout << "               pz   = " << pz << std::endl;
}
