
// $Id: LBNEMagneticField.hh,v 1.1.1.1 2011/07/13 16:20:52 loiacono Exp $
// --------------------------------------------------------------
// LBNEMagneticField.hh modified by Yuki 2004/7/16
// modified by Yuki 8/2/04

#ifndef LBNEMagneticField_H
#define LBNEMagneticField_H 1

#include "globals.hh"
#include "G4MagneticField.hh"
#include "LBNEDataInput.hh"

class LBNEMagneticField : public G4MagneticField
{
  public:
    LBNEMagneticField();
    ~LBNEMagneticField();

  virtual void GetFieldValue( const double Point[3],
                               double *Bfield ) const;
  
  private:
    LBNEDataInput* LBNEData;
  //G4double current;
};

class LBNEMagneticFieldIC : public G4MagneticField
{
  public:
    LBNEMagneticFieldIC();
    ~LBNEMagneticFieldIC();

  virtual void GetFieldValue( const double Point[3], double *Bfield ) const;

  private:
    LBNEDataInput* LBNEData;
  //G4double current;
};

class LBNEMagneticFieldOC : public G4MagneticField
{
  public:
    LBNEMagneticFieldOC();
    ~LBNEMagneticFieldOC();

  virtual void GetFieldValue( const double Point[3], double *Bfield ) const;

  private:
    LBNEDataInput* LBNEData;
  //G4double current;
};

#endif

