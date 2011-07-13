//
// LBNETrajectory.cc
//

#include "LBNETrajectory.hh"

#include "G4ParticleTable.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"
#include "G4Polyline.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4VisAttributes.hh"
#include "G4VVisManager.hh"
#include "G4UnitsTable.hh"


#include "LBNETrackInformation.hh"
#include "LBNEDataInput.hh"

G4Allocator<LBNETrajectory> myTrajectoryAllocator;

//-------------------------------------------------------------------------
LBNETrajectory::LBNETrajectory()
{
   fParticleDefinition = 0;
   fParticleName       = "";
   fPDGCharge          = 0;
   fPDGEncoding        = 0;
   fTrackID            = 0;
   fParentID           = 0;
   fPositionRecord     = 0;
   fMomentum           = G4ThreeVector(0.,0.,0.);
   fMomentumRecord     = 0;
   fVertexPosition     = G4ThreeVector(0.,0.,0.);
   fParticleMass       = 0.;
   fDecayCode          = 0;
   fTgen               = 0;
   fNImpWt             = 1.;
   fPreStepVolume      = 0;
   fStepLength         = 0;

   fLBNEData = LBNEDataInput::GetLBNEDataInput();

}
//-------------------------------------------------------------------------
LBNETrajectory::LBNETrajectory(const G4Track* aTrack)
{
   fParticleDefinition = aTrack->GetDefinition();
   fParticleName       = fParticleDefinition->GetParticleName();
   fPDGCharge          = fParticleDefinition->GetPDGCharge();
   fPDGEncoding        = fParticleDefinition->GetPDGEncoding();
   fTrackID            = aTrack->GetTrackID();
   fParentID           = aTrack->GetParentID();
   fPositionRecord     = new LBNETrajectoryPointContainer();
   fPositionRecord -> push_back(new G4TrajectoryPoint(aTrack->GetPosition()));
   fMomentumRecord     = new LBNETrajectoryMomentumContainer();
   fMomentumRecord -> push_back(aTrack->GetMomentum());
   fPreStepVolume      = new LBNETrajectoryVolumeName();  
   fPreStepVolume  -> push_back(aTrack->GetVolume()->GetName());
   fStepLength         = new DVec();  
   fStepLength     -> push_back(aTrack->GetStepLength());
   fMomentum           = aTrack->GetMomentum();
   fVertexPosition     = aTrack->GetPosition();
   fParticleMass       = aTrack->GetDefinition()->GetPDGMass();

   LBNETrackInformation* info = (LBNETrackInformation*)(aTrack->GetUserInformation());
   if (info!=0) 
   {
     fDecayCode = info->GetDecayCode();
     fTgen      = info->GetTgen();
     fNImpWt    = info->GetNImpWt();
   }
   else 
   { 
     fDecayCode = 0;
     fTgen      = 0; 
     fNImpWt    = 1.;
   }

   fLBNEData=LBNEDataInput::GetLBNEDataInput();
}

//-------------------------------------------------------------------------
LBNETrajectory::LBNETrajectory(LBNETrajectory & right)
  : G4VTrajectory()
{
  fParticleName       = right.fParticleName;
  fParticleDefinition = right.fParticleDefinition;
  fPDGCharge          = right.fPDGCharge;
  fPDGEncoding        = right.fPDGEncoding;
  fTrackID            = right.fTrackID;
  fParentID           = right.fParentID;
  fMomentum           = right.fMomentum;
  fVertexPosition     = right.fVertexPosition;
  fParticleMass       = right.fParticleMass;
  fDecayCode          = right.fDecayCode;
  fTgen               = right.fTgen;
  fNImpWt             = right.fNImpWt;
  fPositionRecord     = new LBNETrajectoryPointContainer();
  fMomentumRecord     = new LBNETrajectoryMomentumContainer();
  fPreStepVolume      = new LBNETrajectoryVolumeName();  
  fStepLength         = new DVec();  

  for(size_t i=0;i<right.fPositionRecord->size();i++)
  {
     G4TrajectoryPoint* rightPoint = (G4TrajectoryPoint*)((*(right.fPositionRecord))[i]);
     fPositionRecord->push_back(new G4TrajectoryPoint(*rightPoint));
  }
  for(size_t i=0;i<right.fMomentumRecord->size();i++)
  {
     G4ThreeVector rightMomentum = (G4ThreeVector)((*(right.fMomentumRecord))[i]);
     fMomentumRecord->push_back(rightMomentum);
  }
  for(size_t i=0;i<right.fPreStepVolume->size();i++)
  {
     G4String rightPreStepVolume=(G4String)((*(right.fPreStepVolume))[i]);
     fPreStepVolume->push_back(rightPreStepVolume);
  }
  for(size_t i=0;i<right.fStepLength->size();i++)
  {
     G4double rightsteplength =(G4double)((*(right.fStepLength))[i]);
     fStepLength->push_back(rightsteplength);
  }
  

  fLBNEData=LBNEDataInput::GetLBNEDataInput();
}

//----------------------------------------------------------------------
LBNETrajectory::LBNETrajectory(const LBNETrajectory* right)
  : G4VTrajectory()
{
  fParticleName       = right -> fParticleName;
  fParticleDefinition = right -> fParticleDefinition;
  fPDGCharge          = right -> fPDGCharge;
  fPDGEncoding        = right -> fPDGEncoding;
  fTrackID            = right -> fTrackID;
  fParentID           = right -> fParentID;
  fMomentum           = right -> fMomentum;
  fVertexPosition     = right -> fVertexPosition;
  fParticleMass       = right -> fParticleMass;
  fDecayCode          = right -> fDecayCode;
  fTgen               = right -> fTgen;
  fNImpWt             = right -> fNImpWt;
  

  fPositionRecord     = new LBNETrajectoryPointContainer();
  for(size_t i=0; i < (right->fPositionRecord)->size(); ++i)
  {
     G4TrajectoryPoint* rightPoint = (G4TrajectoryPoint*)((*(right->fPositionRecord))[i]);
     fPositionRecord->push_back(new G4TrajectoryPoint(*rightPoint));
  }

  fMomentumRecord = new LBNETrajectoryMomentumContainer();
  for(size_t i=0; i < (right->fMomentumRecord)->size(); ++i)
  {
     G4ThreeVector rightMomentum = (G4ThreeVector)((*(right->fMomentumRecord))[i]);
     fMomentumRecord->push_back(rightMomentum);
  }

  fPreStepVolume = new LBNETrajectoryVolumeName();  
  for(size_t i=0; i < (right->fPreStepVolume)->size(); ++i)
  {
     G4String rightPreStepVolume =(G4String)((*(right->fPreStepVolume))[i]);
     fPreStepVolume->push_back(rightPreStepVolume);
  }

  fStepLength = new DVec();  
  for(size_t i=0; i < (right->fStepLength)->size(); ++i)
  {
     G4double rightsteplength =(G4double)((*(right->fStepLength))[i]);
     fStepLength->push_back(rightsteplength);
  }
  
  fLBNEData=LBNEDataInput::GetLBNEDataInput();
}

//-------------------------------------------------------------------------
LBNETrajectory::~LBNETrajectory()
{
  size_t i;
  for(i=0;i<fPositionRecord->size();i++)
  {
    delete  (*fPositionRecord)[i];
  }
  fPositionRecord->clear();
  delete fPositionRecord;


  fMomentumRecord->clear();  
  delete fMomentumRecord;

  fPreStepVolume->clear();
  delete fPreStepVolume;

  fStepLength->clear();
  delete fStepLength;
 
}

//-------------------------------------------------------------------------
void LBNETrajectory::ShowTrajectory() const
{
   G4cout << G4endl << "TrackID =" << fTrackID 
        << " : ParentID=" << fParentID << G4endl;
   G4cout << "Particle name : " << fParticleName 
        << "  Charge : " << fPDGCharge << G4endl;
   G4cout << "Original momentum : " <<
G4BestUnit(fMomentum,"Energy") << G4endl;
   G4cout << "Vertex : " << G4BestUnit(fVertexPosition,"Length") << G4endl;
   G4cout << "  Current trajectory has " << fPositionRecord->size() 
        << " points." << G4endl;

   for( size_t i=0 ; i < fPositionRecord->size() ; i++){
       G4TrajectoryPoint* aTrajectoryPoint = (G4TrajectoryPoint*)((*fPositionRecord)[i]);
       G4cout << "Point[" << i << "]" 
            << " Position= " << aTrajectoryPoint->GetPosition() << G4endl;
   }
}
//-------------------------------------------------------------------------
void LBNETrajectory::ShowTrajectory(std::ostream& o) const
{
    G4VTrajectory::ShowTrajectory(o);
}
//-------------------------------------------------------------------------
void LBNETrajectory::DrawTrajectory(G4int /*i_mode*/) const
{
   G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
   G4ThreeVector pos;

   G4Polyline pPolyline;
   for (size_t i = 0; i < fPositionRecord->size() ; i++) {
     G4TrajectoryPoint* aTrajectoryPoint = (G4TrajectoryPoint*)((*fPositionRecord)[i]);
     pos = aTrajectoryPoint->GetPosition();
     pPolyline.push_back( pos );
   }

   G4Colour color(1.0,1.0,1.0); //white

   if(fPDGCharge ==  0.)   color = G4Colour::White();
   if(fPDGCharge ==  1.0)  color = G4Colour(1.0, 0.5, 0.0); //orange
   if(fPDGCharge == -1.0)  color = G4Colour(0.5, 0.0, 0.5); //purple


   if (fParticleDefinition==G4Proton::ProtonDefinition()) color = G4Color::Green();
   if (fParticleDefinition==G4Neutron::NeutronDefinition()) color = G4Color::Grey();
   if (fParticleDefinition==G4PionMinus::PionMinusDefinition()) color = G4Color::Cyan();
   if (fParticleDefinition==G4PionPlus::PionPlusDefinition()) color = G4Color::Blue();
   if (fParticleDefinition==G4KaonMinus::KaonMinusDefinition()) color = G4Color::Yellow();
   if (fParticleDefinition==G4KaonPlus::KaonPlusDefinition()) color = G4Color::Yellow();
   if (fParticleDefinition==G4MuonMinus::MuonMinusDefinition()) color = G4Color::Red();
   if (fParticleDefinition==G4MuonPlus::MuonPlusDefinition()) color = G4Color::Red();

   if ((fParticleDefinition == G4NeutrinoE::NeutrinoEDefinition())  ||
       (fParticleDefinition == G4NeutrinoMu::NeutrinoMuDefinition()) ||
       (fParticleDefinition == G4NeutrinoTau::NeutrinoTauDefinition()) ||
       (fParticleDefinition == G4AntiNeutrinoE::AntiNeutrinoEDefinition()) ||
       (fParticleDefinition == G4AntiNeutrinoMu::AntiNeutrinoMuDefinition()) ||
       (fParticleDefinition == G4AntiNeutrinoTau::AntiNeutrinoTauDefinition()))
      color = G4Color::Magenta();







G4VisAttributes attribs(color);
   pPolyline.SetVisAttributes(attribs);
   if(pVVisManager) pVVisManager->Draw(pPolyline);
      
/*
   G4Colour colour(0.2,0.2,0.2);
   if(fParticleDefinition==G4Gamma::GammaDefinition())
      colour = G4Colour(0.,0.,1.);
   else if(fParticleDefinition==G4Electron::ElectronDefinition()
         ||fParticleDefinition==G4Positron::PositronDefinition())
      colour = G4Colour(1.,1.,0.);
   else if(fParticleDefinition==G4MuonMinus::MuonMinusDefinition()
         ||fParticleDefinition==G4MuonPlus::MuonPlusDefinition())
      colour = G4Colour(0.,1.,0.);
   else if(fParticleDefinition->GetParticleType()=="meson")
   {
      if(fPDGCharge!=0.)
         colour = G4Colour(1.,0.,0.);
      else
         colour = G4Colour(0.5,0.,0.);
   }
   else if(fParticleDefinition->GetParticleType()=="baryon")
   {
      if(fPDGCharge!=0.)
         colour = G4Colour(0.,1.,1.);
      else
         colour = G4Colour(0.,0.5,0.5);
   }

   //

   //draw only protons,pi+ and pi-
   G4VisAttributes attribs;
   if (fParticleDefinition==G4Proton::ProtonDefinition()) {
     colour=G4Colour(0.,0.,1.);
     attribs=G4VisAttributes(colour);
     pPolyline.SetVisAttributes(attribs);
     if(pVVisManager) pVVisManager->Draw(pPolyline);
   }
   if (fParticleDefinition==G4PionMinus::PionMinusDefinition()) {
     colour=G4Colour(1.,0.,0.);
     attribs=G4VisAttributes(colour);
     pPolyline.SetVisAttributes(attribs);
     if(pVVisManager) pVVisManager->Draw(pPolyline);
   }
   if (fParticleDefinition==G4PionPlus::PionPlusDefinition()) {
     colour=G4Colour(0.,1.,0.);
     attribs=G4VisAttributes(colour);
     pPolyline.SetVisAttributes(attribs);
     if(pVVisManager) pVVisManager->Draw(pPolyline);
   }

*/

}

//-------------------------------------------------------------------------
const std::map<G4String,G4AttDef>* LBNETrajectory::GetAttDefs() const
{
    G4bool isNew;
    std::map<G4String,G4AttDef>* store
       = G4AttDefStore::GetInstance("LBNETrajectory",isNew);
    if (isNew) 
    {
       
       G4String ID("TrkID");
       (*store)[ID] = G4AttDef(ID,"Track ID","Physics","","G4int");
       
       G4String PID("ParTrkID");
       (*store)[PID] = G4AttDef(PID,"Parent ID","Physics","","G4int");
 
       G4String PN("Name");
       (*store)[PN] = G4AttDef(PN,"Particle Name","Physics","","G4String");
       
       G4String Ch("Q");
       (*store)[Ch] = G4AttDef(Ch,"Charge","Physics","e+","G4double");
       
       G4String PDG("PDG");
       (*store)[PDG] = G4AttDef(PDG,"PDG Encoding","Physics","","G4int");
       
       G4String IMom("IMom");
       (*store)[IMom] = G4AttDef(IMom, "Momentum at start of trajectory",
				 "Physics","G4BestUnit","G4ThreeVector");
       
       G4String IMag("IMomMag");
       (*store)[IMag] = 
	  G4AttDef(IMag, "Magnitude of momentum at start of trajectory",
		   "Physics","G4BestUnit","G4double");
       
       G4String NTP("NPts");
       (*store)[NTP] = G4AttDef(NTP,"No. of points","Physics","","G4int");
       
    }
    return store;
}
//-------------------------------------------------------------------------
std::vector<G4AttValue>* LBNETrajectory::CreateAttValues() const
{
   std::vector<G4AttValue>* values = new std::vector<G4AttValue>;
   
   values->push_back
      (G4AttValue("TrkID",G4UIcommand::ConvertToString(fTrackID),""));
   
   values->push_back
      (G4AttValue("ParTrkID",G4UIcommand::ConvertToString(fParentID),""));
   
   values->push_back(G4AttValue("Name",fParticleName,""));
   
   values->push_back
      (G4AttValue("Q",G4UIcommand::ConvertToString(fPDGCharge),""));
   
   values->push_back
      (G4AttValue("PDG",G4UIcommand::ConvertToString(fPDGEncoding),""));
   
   values->push_back
      (G4AttValue("IMom",G4BestUnit(GetInitialMomentum(),"Energy"),""));
   
   values->push_back
      (G4AttValue("IMomMag",G4BestUnit((GetInitialMomentum()).mag(),"Energy"),""));
   
   values->push_back
      (G4AttValue("NPts",G4UIcommand::ConvertToString(GetPointEntries()),""));
   
   return values;
}


//-------------------------------------------------------------------------
void LBNETrajectory::AppendStep(const G4Step* aStep)
{
  fPositionRecord
    ->push_back(new G4TrajectoryPoint(aStep->GetPostStepPoint()
				      ->GetPosition() ));
   fMomentumRecord->push_back(aStep->GetPostStepPoint()->GetMomentum());

   G4Track* aTrack=aStep->GetTrack();
   LBNETrackInformation* info=(LBNETrackInformation*)(aTrack->GetUserInformation());
   if (info!=0) {
     fDecayCode=info->GetDecayCode();
     fTgen=info->GetTgen();
   }
   else fDecayCode=-1;
   
   G4StepPoint * steppoint=aStep->GetPreStepPoint(); 
   G4String PreVolumeName=steppoint->GetPhysicalVolume()->GetName(); 
   fPreStepVolume->push_back(PreVolumeName); 
   fStepLength->push_back(aStep->GetStepLength());
}
//-------------------------------------------------------------------------  
G4ParticleDefinition* LBNETrajectory::GetParticleDefinition()
{
   return (G4ParticleTable::GetParticleTable()->FindParticle(fParticleName));
}
//-------------------------------------------------------------------------  
void LBNETrajectory::MergeTrajectory(G4VTrajectory* secondTrajectory)
{
  if(!secondTrajectory) return;

  LBNETrajectory* seco = (LBNETrajectory*)secondTrajectory;
  G4int ent = seco->GetPointEntries();
  for(int i=1;i<ent;i++) // initial point of the second trajectory should not be merged
  {
    fPositionRecord->push_back((*(seco->fPositionRecord))[i]);
  }
  delete (*seco->fPositionRecord)[0];
  seco->fPositionRecord->clear();

}

