// $Id: LBNENuWeight.hh,v 1.1 2011/07/13 16:20:52 loiacono Exp $

#ifndef LBNENUWEIGHT_H
#define LBNENUWEIGHT_H 1

#include "Rtypes.h"

#include "LBNEDataNtp_t.hh"

#include <vector>

class LBNENuWeight
{
  public:
    LBNENuWeight();
    ~LBNENuWeight();
 
    double GetWeight(const LBNEDataNtp_t* nudata, 
		     const std::vector<double> xdet,
		     double& nu_wght, double& nu_energy);
 
};
 
#endif
