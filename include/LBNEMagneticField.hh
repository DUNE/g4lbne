
// $Id: LBNEMagneticField.hh,v 1.2 2012/07/23 18:13:53 loiacono Exp $
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

   void SetHornCurrent(G4double ihorn);
   G4double GetHornCurrent() const { return fhorncurrent;}
   
private:
   LBNEDataInput* LBNEData;
   G4double fhorncurrent;
   G4bool fIsHornCurrentSet;
   
};

inline void LBNEMagneticField::SetHornCurrent(G4double ihorn)
{
   fhorncurrent = ihorn;
   fIsHornCurrentSet = true;
}


class LBNEMagneticFieldIC : public G4MagneticField
{
  public:
    LBNEMagneticFieldIC();
    ~LBNEMagneticFieldIC();

  virtual void GetFieldValue( const double Point[3], double *Bfield ) const;

   void SetHornCurrent(G4double ihorn);
   G4double GetHornCurrent() const { return fhorncurrent;}

  private:
   LBNEDataInput* LBNEData;
   G4double fhorncurrent;
   G4bool fIsHornCurrentSet;

};

inline void LBNEMagneticFieldIC::SetHornCurrent(G4double ihorn)
{
   fhorncurrent = ihorn;
   fIsHornCurrentSet = true;
}

class LBNEMagneticFieldOC : public G4MagneticField
{
  public:
    LBNEMagneticFieldOC();
    ~LBNEMagneticFieldOC();

  virtual void GetFieldValue( const double Point[3], double *Bfield ) const;

   void SetHornCurrent(G4double ihorn);
   G4double GetHornCurrent() const { return fhorncurrent;}

  private:
   LBNEDataInput* LBNEData;
   G4double fhorncurrent;
   G4bool fIsHornCurrentSet;

};

inline void LBNEMagneticFieldOC::SetHornCurrent(G4double ihorn)
{
   fhorncurrent = ihorn;
   fIsHornCurrentSet = true;
}


#endif

