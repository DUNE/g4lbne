//----------------------------------------------------------------------
//
// $Id: LBNEDataInput.hh,v 1.4 2013/01/31 19:26:03 loiacono Exp $
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// The LBNEDataInput class is used to contain all of the input data
// that is used by the rest of G4numi. This class is a singleton,
// meaning it only has one instance. You can obtain this instance by
// calling the function GetLBNEDataInput.
//
// LBNEDataInput currently reads in data from a .input file using the
// LBNEKeyedInput class. A file name should be specified before
// GetLBNEDataInput is called for the first time. To specify a file
// name, call the setInputFile function. If no file name is given,
// "default.input" will be used.
//
// See LBNEDataInput.cc for more details about specific functions.
//----------------------------------------------------------------------

#ifndef LBNEDataInput_h
#define LBNEDataInput_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include <vector>

#include "G4LogicalVolume.hh"

typedef std::vector<G4double> vdouble_t;
typedef std::vector<G4int> vint_t;
typedef std::vector<G4String> vstring_t;
typedef std::vector<G4bool> vbool_t;
//typedef std::vector<NumiHornSpiderSupport> vNumiHornSpiderSupport_t;

class LBNEKeyedInput;

class LBNEDataInput
{
public:
   LBNEDataInput();
   ~LBNEDataInput();
   
   static void setInputFile(const G4String& filename);
   static LBNEDataInput* GetLBNEDataInput();
   void printAll();
   
   void PrintSetup();

   void ApplyStepLimits(G4LogicalVolume *);
   void ApplyTimeLimits(G4LogicalVolume *);

public:
   friend class LBNEDetectorConstruction;
   //friend class NumiDetectorMessenger;
   //friend class NumiMagneticField;
   //friend class NumiMagneticFieldIC;
   //friend class NumiMagneticFieldOC;
   //friend class NumiPrimaryGeneratorAction;
   //friend class NumiStackingAction;
   //friend class NumiAnalysis;
   

   
private:
   static G4String fInputFileName;
   static LBNEDataInput* fLBNEDataInput;

   void readFile();
   void preInit();
   void initHornStandard();
   void initBeam(G4double beamz0 = -999.0);
   void initRock();
   void initTargetArea();
   void initTarget();
   void initNUMITarget();
   void initBaffle();
   void initTunnel();
   void initShield();
   void initDecayPipe();
   void initBlocks();
   void initHadrBox();
   void initHorns();
   void initDetectors();
   
   
   template<class T>
   void get(T& buffer, const G4String& name);
   template<class T>
   void get(std::vector<T>& buffer, const G4String& name, G4int expectedLength);
   template<class T>
   void getFirst(T& buffer, const G4String& name);
   
   void get(G4double& buffer, const G4String& name, G4double scalar);
   void get(vdouble_t& buffer, const G4String& name,
	    G4int expectedLength, G4double scalar);
   void getFirst(G4double& buffer, const G4String& name, G4double scalar);
   
   template<class T>
   void getConstructFlag(T& buffer, const G4String& name);
   
   LBNEKeyedInput* keyedInput;
   
   template<class T>
   void print(const G4String& name, const T& value);
   template<class T>
   void print(const G4String& name, const std::vector<T>& value);
   
public:

   //
   //set functions
   //

   //
   //Set the Simulation. NO simulation with run if this is not set
   //This ensures that you are sure you are running the simulation you want
   //
   //possible simulations that are currently enabled are below
   void SetSimulation         (G4String &sim); 

   void SetInputNtpFileName   (G4String fileName)  { fInputNtupleFileName=fileName; }   
   void SetInputNtpTreeName   (G4String &name)     { fInputNtpTreeName = name;}
   void SetOutputNtpFileName  (G4String fileName)  { fOutputNtpFileName=fileName;}
   void SetGeometryTag        (G4String geoName)   { fGeometryTag = geoName; }  
   void SetRunNumber          (G4String runNum)    { fRunNumber=runNum; }
   void SetASCIIOutputFileName(G4String fileName)  { fASCIIOutputFileName=fileName; }

   void SetKillTracking      (G4bool val)       { fKillTracking = val;}
   void SetConstructTarget   (G4bool val)       { fConstructTarget = val;}
   
   void SetOKToRun           (G4bool val)       { fOKToRun = val;}
   void SetCreateOutput      (G4bool output)    { fCreateOutput=output;}
   void SetNImpWeightOn      (G4bool val)       { fNImpWeightOn = val;}
   void SetUseFlukaInput     (G4bool val)       { fUseFlukaInput = val; }
   void SetUseMarsInput      (G4bool val)       { fUseMarsInput = val; }
   void SetCreateASCIIOutput (G4bool output)    { fCreateASCIIOutput=output; } 


   
   void SetDebugLevel(G4int val)                { fDebugLevel = val;}   
   //   
   //if the macro sets the NEvents don't 
   //let the input ntuple reset it
   //unless the macro set NEvents to -1
   //which means run over all events in the 
   //input ntuple
   void SetNEvents   (G4int events)             { if(fNEvents<0)fNEvents=events; }
   
   void SetExtraFlukaNumiTargetZShift(G4double shift)  { fExtraFlukaNumiTargetZShift = shift; }
   void SetKillTrackingThreshold     (G4double th )    { fKillTrackingThreshold=th;}
   void SetStepLimit                 (G4double l)      { fStepLimit = l; }
   
   void SetDefaultMaterial           (G4Material* mat) { fDefaultMaterial = mat;}
   
   
   
   //
   //get functions
   //
   G4String GetSimulation()            { return fSimulation;}
   G4String GetInputNtpFileName()      { return fInputNtupleFileName;}   
   G4String GetInputNtpTreeName()      { return fInputNtpTreeName;}
   G4String GetOutputNtpFileName()     { return fOutputNtpFileName;}
   G4String GetGeometryTag()           { return fGeometryTag; }
   G4String GetRunNumber()             { return fRunNumber; }
   G4String GetASCIIOutputFileName()   { return fASCIIOutputFileName; }
   G4String GetHornNeckPartName(G4int ihorn) const;

   G4int    GetDebugLevel()            { return fDebugLevel;}
   G4int    GetNEvents()               {return fNEvents;}   
   
   G4bool   GetKillTracking()          { return fKillTracking;}
   G4bool   GetCreateOutput()          { return fCreateOutput;}
   G4bool   GetOKToRun()               { return fOKToRun;}
   G4bool   GetImpWeightOn()           { return fNImpWeightOn;}
   G4bool   GetUseFlukaInput()         { return fUseFlukaInput; }
   G4bool   GetUseMarsInput()          { return fUseMarsInput; }
   G4bool   GetCreateASCIIOutput()     { return fCreateASCIIOutput;} 
   G4bool   GetConstructTarget()       { return fConstructTarget;}
   
   G4double GetHornCurrent()                { return fHornCurrent;}
   G4double GetExtraFlukaNumiTargetZShift() { return fExtraFlukaNumiTargetZShift;}
   G4double GetKillTrackingThreshold()      { return fKillTrackingThreshold;}
   G4double GetMaterialSigma()              { return fMaterialSigma;}
   G4double GetProtonMomentum()             { return fProtonMomentum;}
   G4double GetBeamSigmaX()                 { return fBeamSigmaX;}
   G4double GetBeamSigmaY()                 { return fBeamSigmaY;}
   G4double GetBeamSigDx()                  { return fBeamSigDx;}
   G4double GetBeamSigDy()                  { return fBeamSigDy;}
   G4double GetBeamMaxDx()                  { return fBeamMaxDx;} 
   G4double GetBeamMaxDy()                  { return fBeamMaxDy;} 
   G4double GetBeamXDirection()             { return fBeamDirection.x();}
   G4double GetBeamYDirection()             { return fBeamDirection.y();}
   G4double GetBeamZDirection()             { return fBeamDirection.z();}
   G4double GetBeamXPosition()              { return fBeamPosition.x();}
   G4double GetBeamYPosition()              { return fBeamPosition.y();}
   G4double GetBeamZPosition()              { return fBeamPosition.z();}
   G4double GetProtonKineticEnergy()        { return fProtonKineticEnergy;}
   G4double GetRockRadius()                 { return RockRadius;}
   G4double GetRockHalfLen()                { return RockHalfLen;}

   G4ThreeVector GetBeamDirection()         { return fBeamDirection;}
   G4ThreeVector GetBeamPosition()          { return fBeamPosition;}

   const vstring_t& GetListOfSimulations()  { return fSimulationVec;}

   const std::vector<G4double>& GetXDetNear() { return xdet_near;}
   const std::vector<G4double>& GetYDetNear() { return ydet_near;}
   const std::vector<G4double>& GetZDetNear() { return zdet_near;}
   const std::vector<G4double>& GetXDetFar()  { return xdet_far;}
   const std::vector<G4double>& GetYDetFar()  { return ydet_far;}
   const std::vector<G4double>& GetZDetFar()  { return zdet_far;}

   G4Material* GetDefaultMaterial()         {return fDefaultMaterial;}



   G4double GetTargetZ0(G4int i=0) const;

   G4int GetNumberOfHorns() const 
      {return NPhorns;}       //number of horns
   const vint_t GetVectorOfNHornParts() const 
      {return PhornNParts;}  //vector of number of parts per horn
   G4int GetNPartsOfHorn(unsigned int nhorn) const;
   G4int GetTotalNumberOfHornParts() const 
      { return PhornNphorn;}
   G4double GetHornCurrentForHornPart(G4int npart) const;

   

////////////////////////////////////
///////////////////////////////////////
/////////////////////////////////////
private:

   G4String fSimulation;
   G4String fInputNtupleFileName;
   G4String fInputNtpTreeName;
   G4String fOutputNtpFileName;
   //G4String fBeamConfig;
   //G4String fTargetConfig;
   //G4String fIHornConfig;
   //G4String fHornConfig;
   G4String fGeometryTag;
   G4String fRunNumber;
   G4String fASCIIOutputFileName;
   
   G4bool   fKillTracking;
   //G4bool   fUseCorrHornCurrent;
   G4bool   fOKToRun;
   G4bool   fCreateOutput;
   G4bool   fNImpWeightOn;
   G4bool   fUseFlukaInput;
   G4bool   fUseMarsInput;
   G4bool   fCreateASCIIOutput;
   G4bool   fBeamOnTarget;
   G4bool   fConstructTarget;

   G4int    fDebugLevel;
   G4int    fRunPeriod;
   G4int    fNEvents;
   
   G4double fHornCurrent;
   G4double fExtraFlukaNumiTargetZShift;
   G4double fKillTrackingThreshold;
   G4double fMaterialSigma;
   G4double fStepLimit;   
   G4double fTimeLimit;   
   G4double fProtonMomentum; 
   G4double fBeamSigmaX;
   G4double fBeamSigmaY;
   G4double fBeamSigDx;
   G4double fBeamSigDy; 
   G4double fBeamMaxDx; 
   G4double fBeamMaxDy;
   G4double fProtonKineticEnergy;

   G4ThreeVector fBeamPosition; 
   G4ThreeVector fBeamDirection;
   
   vstring_t fSimulationVec;

   G4Material* fDefaultMaterial;
   

   
   

private:
   
   // World Volume
   G4double RockRadius, RockHalfLen, RockDensity; //, RockRadLen;
   
   // Target Area
   G4double TargetAreaZ0, TargetAreaLength, TargetAreaHeight,TargetAreaWidth;
   G4int TargetAreaGEANTmat;
   
   // Target
   

   G4int TargetNtarget;
   G4String TargetShape;
   vdouble_t TargetX0, TargetY0, TargetZ0, TargetDxdz, TargetDydz;
   vdouble_t TargetLength, TargetRadius, TargetWidth, TargetHeight;
   vdouble_t TargetA, TargetDensity, TargetZ;
   vint_t TargetGEANTmat;
   
  //HPBaffle
  G4bool HPBaffle;
  G4int HPBaffleGEANTMat;
  G4double HPBaffleX0,HPBaffleY0,HPBaffleZ0,HPBaffleDXDZ,HPBaffleDYDZ;
  G4double HPBaffleLength,HPBaffleRin,HPBaffleRout;

  // Tunnel
  G4double TunnelZ0, TunnelRadius, TunnelLength, TunnelA, TunnelZ;
  G4int TunnelGEANTmat;
  G4double BeamAngle;//nominally .058 radians

  // Decay Pipe
  G4double DecayPipeZ0,DecayPipeRadius,DecayPipeLength,DecayPipeFWinThick,DecayPipeEWinThick,DecayPipeWallThick;
  G4double DecayPipeA,DecayPipeZ;
  G4int DecayPipeGEANTmat,DecayPipeFWinmat, DecayPipeEWinmat;
  G4int DecayPipeFillGeantMat;
  
  // Decay Pipe Shield
  G4double ShieldX0,ShieldY0,ShieldZ0,ShieldDxdz, ShieldDydz,ShieldLength,ShieldRout,ShieldRin;
  G4int ShieldGEANTmat;

  //Target Hall Blocks
  G4int BlockNblock;
  vdouble_t BlockX0, BlockY0, BlockZ0, BlockDxdz, BlockDydz;
  vdouble_t BlockLength, BlockHdx, BlockHdy;
  vint_t BlockGeantMaterial;

  //Hadron Absorber Box
  G4double HadrBox_width, HadrBox_height, HadrBox_length;

   //
   // Horn 1 & 2
   //
   G4int hrnmat;
   G4int hrnmatcr;

   G4int NPhorns;       //number of horns
   vint_t PhornNParts;  //vector of number of parts per horn
   G4int PhornNphorn;

   vdouble_t PhornZ1, PhornZ2;  
   vdouble_t PhornNeckZ1, PhornNeckZ2;  
   vint_t  PhornNpoint;  
   vdouble_t PhornAin, PhornBin, PhornCin, PhornAout, PhornBout, PhornCout;
   vdouble_t PhornROCin, PhornROCout;
   vdouble_t PhornThickFront, PhornThickEnd;  
   vdouble_t PhornX0, PhornY0, PhornZ0, PhornDXDZ, PhornDYDZ, PhornCurrent;
   vint_t PhornGEANTmat; 
   //vstring_t PhornName;
   //
   //This gets filled in LBNEHorns
   //used to tell what Part of the horn is the neck in LBNEAnalysis
   //
   vstring_t fHornNeckPartName;
   //end horns
   
  // Flux Area
  G4int nNear,nFar;
  vdouble_t xdet_near,ydet_near,zdet_near;
  vdouble_t xdet_far,ydet_far,zdet_far;

  
};


//-----------------------------------------------------------------
inline G4double LBNEDataInput::GetHornCurrentForHornPart(G4int npart) const
{
   if(npart < 1 || npart > PhornNphorn) 
   {
      G4cout << " LBNEDataInput::GetHornCurrentForHornPart() - "
	     << " invalid horn part " << npart 
	     << ". npart can only be in [1," << PhornNphorn << "]" << G4endl; 

      return -999;
      
   }
   return PhornCurrent[npart-1];
}

//-----------------------------------------------------------------
inline G4int LBNEDataInput::GetNPartsOfHorn(unsigned int nhorn) const
{ 
   if(nhorn != (unsigned int)NPhorns || PhornNParts.size() != nhorn)
   {
      G4cout << " LBNEDataInput::GetNPartsOfHorn() - "
	     << " invalid horn number nhorn " << nhorn 
	     << ". There are " << NPhorns << "(" 
	     << PhornNParts.size() << ")" << G4endl;
      return -999;
   }

   return PhornNParts[nhorn-1];
}

//-----------------------------------------------------------------
inline G4double LBNEDataInput::GetTargetZ0(G4int i) const
{
   if(i >= 0 && i < TargetNtarget )
   {
      return TargetZ0[i];
   }
   else
   {
      G4cout << " LBNEDataInput::GetTargetZ0 - invalid target segment i = " << i 
	     << ". Returning -999 for the target Z0. " << G4endl;
      return -999; 
   }
      
}

//-----------------------------------------------------------------
inline G4String LBNEDataInput::GetHornNeckPartName(G4int ihorn) const
{
   G4int nhorns = fHornNeckPartName.size();

   if(ihorn > 0 && ihorn <= nhorns)
   {
      return fHornNeckPartName[ihorn-1];
   }
   else
   {
      G4cout << " LBNEDataInput::GetHornNeckPartName - invalid horn = " << ihorn 
           << ". Know about " << nhorns << " horns. "
           << ". Returning \"Unknown\" for the horn neck part name. " << G4endl;
      return "Unknown"; 
   }
      
}

#endif
