//----------------------------------------------------------------------
// LBNEAnalysis.cc
//
// $Id: LBNEAnalysis.cc,v 1.1.1.1 2011/07/13 16:20:52 loiacono Exp $
//----------------------------------------------------------------------

#include <vector>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <map.h>

//Root 
#include <TSystem.h>        // ROOT head file for a generic interface to the OS
#include <TStopwatch.h>     // ROOT head file for stopwatch for real and cpu time
#include <TFile.h>          
#include <TTree.h>

//GEANT4 
#include "globals.hh"
#include "G4ios.hh"
#include "G4Track.hh"
#include "G4SteppingManager.hh"
#include "G4ThreeVector.hh"
#include "G4TrajectoryContainer.hh"
#include "G4RunManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"
#include "G4Run.hh"

//g4numi 
#include "LBNEDataNtp_t.hh"
#include "LBNEParticleCode.hh"
#include "LBNEAnalysis.hh"
#include "LBNETrackInformation.hh"
#include "LBNEPrimaryGeneratorAction.hh"
#include "LBNEDataInput.hh"
#include "LBNENuWeight.hh"

using namespace std;

LBNEAnalysis* LBNEAnalysis::instance = 0;

//------------------------------------------------------------------------------------
LBNEAnalysis::LBNEAnalysis()
   :fOutFile(0),
    fOutTree(0)
{
  LBNEData = LBNEDataInput::GetLBNEDataInput();

  if(LBNEData->GetDebugLevel() > 0)
  {
     std::cout << "LBNEAnalysis Constructor Called." << std::endl;
  }

#ifdef G4ANALYSIS_USE
#endif

  
  //g4data = new data_t();
  fLBNEOutNtpData = new LBNEDataNtp_t();
  
  

  fcount = 0;
  fentry = 0;
  
  //
  //need code map defined but initialized
  //or get a seg fault don't know why
  //
  /*code[-13]   = 10;
  code[13]    = 11;
  code[111]   = 23;
  code[211]   = 13;
  code[-211]  = 14;
  code[130]   = 12;
  code[321]   = 15;
  code[-321]  = 16;
  code[2112]  = 8;
  code[2212]  = 1;
  code[-2212] = 2;
  code[310]   = 19;
  code[3122]  = 17;
  code[3222]  = 21;
  code[3212]  = 22;
  code[3112]  = 20;*/
  
}
//------------------------------------------------------------------------------------
LBNEAnalysis::~LBNEAnalysis()
{ 
   if(LBNEData->GetDebugLevel() > 0)
   {
      std::cout << "LBNEAnalysis Destructor Called." << std::endl;
   }

#ifdef G4ANALYSIS_USE
  // delete things
#endif
}
//------------------------------------------------------------------------------------
LBNEAnalysis* LBNEAnalysis::getInstance()
{
  if (instance == 0) instance = new LBNEAnalysis;
  return instance;
}
//------------------------------------------------------------------------------------
G4bool LBNEAnalysis::CreateOutput()
{

   if (LBNEData->GetCreateOutput())
   {
      G4String spaces = "      ";
      std::cout << "    LBNEAnalysis::CreateOutput() - Creating output ntuple..." << std::endl; 

      
      if(LBNEData->GetSimulation() == "Standard Neutrino Beam" ||
	 LBNEData->GetSimulation() == "Target Tracking"      ||
	 LBNEData->GetSimulation() == "Horn 1 Tracking" ||
	 LBNEData->GetSimulation() == "Horn 2 Tracking")
      {
	 //std::cout << spaces << "Creating G4LBNEData Ntuple..." << std::endl;
	 //return LBNEAnalysis::CreateG4LBNEDataNtp();

	 G4RunManager* pRunManager = G4RunManager::GetRunManager();
	 sprintf(nuNtupleFileName,"%s_%03d%s.root",(LBNEData->GetOutputNtpFileName()).c_str(),pRunManager->GetCurrentRun()->GetRunID(), (LBNEData->GetGeometryTag()).c_str());
	 fOutFile = new TFile(nuNtupleFileName,"RECREATE","root ntuple");
	 G4cout << "Creating neutrino ntuple: "<<nuNtupleFileName<<G4endl;
	 //fOutTree = new TTree("nudata","g4numi Neutrino ntuple");
	 //fOutTree->Branch("data","data_t",&g4data,32000,1);
         fOutTree = new TTree("nudata","g4lbne Neutrino ntuple");
	 fOutTree->Branch("data","LBNEDataNtp_t",&fLBNEOutNtpData,32000,1);

	 return true;
	 
      }
      else
      {
	 std::cout << "LBNEAnalysis::CreateOutput() - PROBLEM: Don't know about the \"" 
		   << LBNEData->GetSimulation() << "\" Simulation" << std::endl;
      }
      
      return false;
      
   }
   
   return false;
   
   
}


//------------------------------------------------------------------------------------
void LBNEAnalysis::CloseOutput()
{  
  if(LBNEData->GetCreateOutput())
  {
     //if(fOutFile && fOutFile->IsOpen())
     //{
	fOutFile->cd();
	fOutTree->Write();
	fOutFile->Close();
/*
	if(fOutFile->IsOpen())
	{
	   std::cout << "   PROBLEM: Failed to close Output Ntuple " << fOutFile -> GetName() << std::endl;
	}
	else 
	{*/
	   std::cout << "   Sucessfully closed Output Ntuple " << fOutFile -> GetName() << std::endl;
/*	}
*/
	delete fOutFile;
	//}
  }

}



//------------------------------------------------------------------------------------
void LBNEAnalysis::SetCount(G4int count)
{
  fcount = count;
}
//------------------------------------------------------------------------------------
G4int LBNEAnalysis::GetCount()
{
  return fcount;
}
//------------------------------------------------------------------------------------
void LBNEAnalysis::SetEntry(G4int entry)
{
  fentry = entry;
}
//------------------------------------------------------------------------------------
G4int LBNEAnalysis::GetEntry()
{
  return fentry;
}


//------------------------------------------------------------------------------------
void LBNEAnalysis::FillNeutrinoNtuple(const G4Track& track)
{

   if(LBNEData->GetDebugLevel() > 3) 
   { G4cout << "LBNEAnalysis::FillNeutrinoNtuple() called." << G4endl;}
   
   if (!LBNEData->GetCreateOutput()) return;

   fLBNEOutNtpData -> Clear();

   G4RunManager* pRunManager        = G4RunManager::GetRunManager();
   LBNEPrimaryGeneratorAction *NPGA = (LBNEPrimaryGeneratorAction*)(pRunManager)->GetUserPrimaryGeneratorAction();
   G4int evtno                      = pRunManager->GetCurrentEvent()->GetEventID();
    
  
   //Neutrino vertex position and momentum
   G4ThreeVector pos = track.GetPosition()/mm; 
   x = pos.x();
   y = pos.y();
   z = pos.z();
   G4ThreeVector NuMomentum = track.GetMomentum();
   G4int parentID           = track.GetParentID();

   fLBNEOutNtpData->ptrkid = parentID;
   
   if (parentID == 0)
   { std::cout << "Event no: " << evtno << " LBNEAnalysis::FillNeutrinoNtuple - "
	       << "PROBLEM: Direct Nu Parent has track ID = 0." << std::endl;
      return; //I have to make some changes so that neutrinos in fluka/mars ntuples don't crash
   }
   
   LBNETrajectory* NuParentTrack     = GetParentTrajectory(parentID);
   G4int point_no                    = NuParentTrack->GetPointEntries();
   G4ThreeVector ParentMomentumFinal = NuParentTrack->GetMomentum(point_no-1);
   G4ThreeVector vertex_r            = (NuParentTrack->GetPoint(point_no-1)->GetPosition()/m)*m; //Should be the same as Neutrino vertex
   G4String parent_name              = NuParentTrack->GetParticleName();
   G4double Parent_mass              = NuParentTrack->GetMass();
   G4double gamma                    = sqrt(ParentMomentumFinal*ParentMomentumFinal+Parent_mass*Parent_mass)/Parent_mass; 
   G4double Parent_energy            = gamma*Parent_mass;
   G4ThreeVector beta_vec            = ParentMomentumFinal/Parent_energy;
   G4double partial                  = gamma*(beta_vec*NuMomentum);
   G4double enuzr                    = gamma*(track.GetTotalEnergy())-partial; //neutrino energy in parent rest frame
   
   //fill histograms, ntuples,...

   fLBNEOutNtpData->protonN    = NPGA->GetProtonNumber();
   
   fLBNEOutNtpData->run        = pRunManager->GetCurrentRun()->GetRunID();
   fLBNEOutNtpData->evtno      = pRunManager->GetCurrentEvent()->GetEventID();
   fLBNEOutNtpData->beamHWidth = LBNEData->GetBeamSigmaX()/cm;
   fLBNEOutNtpData->beamVWidth = LBNEData->GetBeamSigmaY()/cm;
   fLBNEOutNtpData->beamX      = LBNEData->GetBeamXPosition()/cm;
   fLBNEOutNtpData->beamY      = LBNEData->GetBeamYPosition()/cm;
   
   //G4int particleID = track.GetParentID();
   G4ThreeVector protonOrigin = NPGA->GetProtonOrigin();
   fLBNEOutNtpData->protonX            = protonOrigin[0];
   fLBNEOutNtpData->protonY            = protonOrigin[1];
   fLBNEOutNtpData->protonZ            = protonOrigin[2];
   
   G4ThreeVector protonMomentum = NPGA->GetProtonMomentum();
   fLBNEOutNtpData->protonPx             = protonMomentum[0];
   fLBNEOutNtpData->protonPy             = protonMomentum[1];
   fLBNEOutNtpData->protonPz             = protonMomentum[2];
   
   fLBNEOutNtpData->nuTarZ      = LBNEData->GetTargetZ0(0);
   fLBNEOutNtpData->hornCurrent = LBNEData->GetHornCurrent()/ampere/1000.;
   
   // Random decay - these neutrinos rarely hit any of the detectors
   fLBNEOutNtpData->Ndxdz   = NuMomentum[0]/NuMomentum[2];
   fLBNEOutNtpData->Ndydz   = NuMomentum[1]/NuMomentum[2];
   fLBNEOutNtpData->Npz     = NuMomentum[2]/GeV;
   fLBNEOutNtpData->Nenergy = track.GetTotalEnergy()/GeV;
   
   //other info
   // Neutrino origin:
   // 3 From muon decay
   // 1 From particle from target
   // 2 From scraping
   //check if nu is from muon decay or from a particle from target, otherwise Norig = 2
   G4int Norig = 2;
   if ((parent_name=="mu+") || (parent_name=="mu-")) Norig = 3;
   G4String firstvolname = NuParentTrack->GetPreStepVolumeName(0);
   if (firstvolname.contains("Baffle") || firstvolname.contains("TGT")) Norig = 1;
   
   fLBNEOutNtpData->Norig  = Norig;
   fLBNEOutNtpData->Ndecay = NuParentTrack->GetDecayCode();
   
   G4ParticleDefinition * particleType = track.GetDefinition();
   G4int ntype   = LBNEParticleCode::AsInt(LBNEParticleCode::StringToEnum(particleType->GetParticleName()));
   fLBNEOutNtpData->Ntype = ntype;
   fLBNEOutNtpData->Vx    = x/cm;
   fLBNEOutNtpData->Vy    = y/cm;
   fLBNEOutNtpData->Vz    = z/cm;
   fLBNEOutNtpData->pdPx  = ParentMomentumFinal[0]/GeV;
   fLBNEOutNtpData->pdPy  = ParentMomentumFinal[1]/GeV;
   fLBNEOutNtpData->pdPz  = ParentMomentumFinal[2]/GeV;
   
   G4ThreeVector ParentMomentumProduction = NuParentTrack->GetMomentum(0);
   fLBNEOutNtpData->ppdxdz = ParentMomentumProduction[0]/ParentMomentumProduction[2];
   fLBNEOutNtpData->ppdydz = ParentMomentumProduction[1]/ParentMomentumProduction[2];
   fLBNEOutNtpData->pppz   = ParentMomentumProduction[2]/GeV; 
   
   G4double parentp = sqrt(ParentMomentumProduction*ParentMomentumProduction);
   
   fLBNEOutNtpData->ppenergy = sqrt((parentp*parentp-Parent_mass*Parent_mass))/GeV;
   
   fLBNEOutNtpData->ppmedium = 0.; //this is still empty
   
   fLBNEOutNtpData->ptype = LBNEParticleCode::AsInt(LBNEParticleCode::StringToEnum(parent_name));
   
   G4ThreeVector production_vertex = (NuParentTrack->GetPoint(0)->GetPosition()/m)*m; 
   fLBNEOutNtpData->ppvx = production_vertex[0]/cm;
   fLBNEOutNtpData->ppvy = production_vertex[1]/cm;
   fLBNEOutNtpData->ppvz = production_vertex[2]/cm;
   
   //if nu parent is a muon then find muon parent info
   if ((parent_name=="mu+" || parent_name=="mu-") && NuParentTrack->GetParentID()!=0)
   {
      G4int mupar                   = NuParentTrack->GetParentID();
      LBNETrajectory* MuParentTrack = GetParentTrajectory(mupar);
      G4int nopoint_mupar           = MuParentTrack->GetPointEntries();
      G4ThreeVector muparp          = MuParentTrack->GetMomentum(nopoint_mupar-1);
      G4double muparm               = MuParentTrack->GetMass();
      fLBNEOutNtpData->muparpx               = muparp[0]/GeV; // vector of hadron parent of muon
      fLBNEOutNtpData->muparpy               = muparp[1]/GeV; // 
      fLBNEOutNtpData->muparpz               = muparp[2]/GeV;
      fLBNEOutNtpData->mupare                = (sqrt(muparp*muparp+muparm*muparm))/GeV;
   }
   else
   {
      fLBNEOutNtpData->muparpx = -999999.;  
      fLBNEOutNtpData->muparpy = -999999.;
      fLBNEOutNtpData->muparpz = -999999.;
      fLBNEOutNtpData->mupare = -999999.;
   }
   
   fLBNEOutNtpData->Necm               = enuzr/GeV; // Neutrino energy in parent rest frame
   LBNETrackInformation* info = (LBNETrackInformation*)(track.GetUserInformation());
   fLBNEOutNtpData->Nimpwt             = info->GetNImpWt();  // Importance weight
   fLBNEOutNtpData->tgen               = info->GetTgen()-1;
   
   fLBNEOutNtpData->xpoint = 0.;  // x, y, z of parent at user selected vol
   fLBNEOutNtpData->xpoint = 0.;
   fLBNEOutNtpData->xpoint = 0.;
   
   //    
   //	 tgen is is the "generation" number
   //	 of the particle that makes it out of the target. Beam protons have
   //	 tgen=1, any particle produced by a p-C interaction would have tgen=2,
   //	 particles produced from interactions of those products have tgen=3 etc.
   //	 etc. until the cascade exiting the target core.
   // 
   

   //
   //if not using external ntuple then need to find the particle that exited the target
   //
   G4int tptrkid = -99;
   if(!(LBNEData->GetUseFlukaInput()) && !(LBNEData->GetUseMarsInput())) 
   {


      G4int eventID     =  G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
      G4bool findTarget                = false;
      G4bool trajectoryCompletelyInTgt = true;
      G4int trackID                    = track.GetParentID();
      
      std::vector<G4int> trackIDvec;
	
      while (trackID > 0)
      {
	 trackIDvec.push_back(trackID);
	 LBNETrajectory* TrackTrajectory = GetTrajectory(trackID);
	 trackID = TrackTrajectory->GetParentID();
      }
      
      for(unsigned int itrk = trackIDvec.size(); itrk > 0; --itrk)
      {
	 trackID = trackIDvec[itrk-1];
	 
	 LBNETrajectory* TrackTrajectory = GetTrajectory(trackID);
	 
	 G4int numberOfPoints = TrackTrajectory->GetPointEntries();

	 for (G4int ii=0; ii < numberOfPoints; ++ii)
	 {
	    G4String prevolname = TrackTrajectory->GetPreStepVolumeName(ii);
	    G4String postvolname = "";
	    if(ii < numberOfPoints-1) postvolname = TrackTrajectory->GetPreStepVolumeName(ii+1);
	    

            /*for debugging
            //
	    if(evtno == 73 )
	    {
	       G4String partname_temp = TrackTrajectory->GetParticleName();
	       std::cout << "eventid: " << evtno
			 << " particle: " << partname_temp  
			 << " trackid: " << trackID 
			 << " ii = " << ii 
			 << " prestepvolname = " << prevolname 
			 << " postvolname = " << postvolname << std::endl;
	    }
            //
	    */

	    if(!postvolname.contains("TGT") &&
	       !postvolname.empty())
	    {
	       if (trackID == 1)
	       { 
		  if(!postvolname.contains("TGAR") &&
		     !postvolname.contains("TargetInnerExit") &&
		     !postvolname.contains("TargetOuterExit")) trajectoryCompletelyInTgt = false;
	       }
	       else trajectoryCompletelyInTgt = false;
	    }
	    if(!prevolname.contains("TGT")) 
	    {
	       if (trackID == 1)
	       { 
		  if(!prevolname.contains("TGAR") &&
		     !prevolname.contains("TargetOuterExit") &&
		     !prevolname.contains("TargetInnerExit") ) trajectoryCompletelyInTgt = false;
	       }
	       else trajectoryCompletelyInTgt = false;
	    }
	    
	    
	    //
	    //particle as exits target
	    //
	    if ((prevolname.contains("TargetInnerExit")) && postvolname.contains("TargetOuterExit"))
	    {
	       G4ThreeVector ParticleMomentum = TrackTrajectory->GetMomentum(ii);              
	       G4ThreeVector ParticlePosition = TrackTrajectory->GetPoint(ii)->GetPosition();
	       G4int tptype = LBNEParticleCode::AsInt(LBNEParticleCode::StringToEnum(TrackTrajectory->GetParticleName()));
	       fLBNEOutNtpData->tvx = ParticlePosition[0]/cm;
	       fLBNEOutNtpData->tvy = ParticlePosition[1]/cm;
	       fLBNEOutNtpData->tvz = ParticlePosition[2]/cm;
	       fLBNEOutNtpData->tpx = ParticleMomentum[0]/GeV;
	       fLBNEOutNtpData->tpy = ParticleMomentum[1]/GeV;
	       fLBNEOutNtpData->tpz = ParticleMomentum[2]/GeV;
	       fLBNEOutNtpData->tptype = tptype;
	       tptrkid = trackID;
	       findTarget = true;
	       
	       break;
	    }

	 }// end for loop over all points in single trajectory

	 if(findTarget) break;
	    
      }// end for loop over all trajectories

      if(!findTarget && !trajectoryCompletelyInTgt)
      {
	 //
	 //if the particle was produced in the baffle or budal fin BLK14
         //then this will happen. Have to modify the above code to account for this case
	 //
	 
	 //std::cout << "EVENTID: " << eventID << " LBNEAnalysis::FillNeutrinoNtuple - PROBLEM: " 
	 //	   << "Did NOT FIND the particle exiting the target but the Trajectory IS NOT " 
	 //	   << "completely contained in the target. This shouldn't happen." << std::endl;
	 
      }
      else if(!findTarget && trajectoryCompletelyInTgt)
      {
	 //It is possible that no particles escape the target and neutrinos
	 //   are created in the target. 
      }
      else if(findTarget && trajectoryCompletelyInTgt)
      {
	 if(trackID != 1)
	 {

///////////////////////////////////////
///////////////////////////////////////
///////////////////////////////////////
///////////////////////////////////////
///////////////////////////////////////
	    std::cout << "EVENTID: " << eventID << " LBNEAnalysis::FillNeutrinoNtuple - PROBLEM: " 
		      << "FOUND the particle exiting the target but the Trajectory IS " 
		      << "completely contained in the target. This shouldn't happen." << std::endl;
///////////////////////////////////////
///////////////////////////////////////
///////////////////////////////////////
///////////////////////////////////////
///////////////////////////////////////
///////////////////////////////////////
	 }
      }
   }
   else          // using external ntuple, so set these to whatever comes from that ntuple
   {
      G4ThreeVector ParticlePosition = NPGA->GetParticlePosition();
      fLBNEOutNtpData->tvx = ParticlePosition[0]/cm;
      fLBNEOutNtpData->tvy = ParticlePosition[1]/cm;
      fLBNEOutNtpData->tvz = ParticlePosition[2]/cm;
      G4ThreeVector ParticleMomentum = NPGA->GetParticleMomentum();
      fLBNEOutNtpData->tpx = ParticleMomentum[0]/GeV;
      fLBNEOutNtpData->tpy = ParticleMomentum[1]/GeV;
      fLBNEOutNtpData->tpz = ParticleMomentum[2]/GeV;
      fLBNEOutNtpData->tptype = NPGA->GetParticleType();
   }   
   //end find particle exiting target




   //
   //Near Detector
   // Neutrino data at different points
   // need neutrino parent info to be filled in fLBNEOutNtpData by this point
   const std::vector<G4double> xdet_near = LBNEData->GetXDetNear();
   const std::vector<G4double> ydet_near = LBNEData->GetYDetNear();
   const std::vector<G4double> zdet_near = LBNEData->GetZDetNear();
   
   if(xdet_near.size() != ydet_near.size() || 
      xdet_near.size() != zdet_near.size() ||
      ydet_near.size() != zdet_near.size())
   {
      G4cout << "LBNEAnalysis::FillNeutrinoNtuple - "
	     << "Near Detector vectors are not the same size." << G4endl; 
   }
   
   unsigned int ndets = xdet_near.size();
   for(unsigned int idet = 0; idet < ndets; ++idet)
   {
      fLBNEOutNtpData->NdxdzNear[idet] = (x-xdet_near[idet])/(z-zdet_near[idet]);
      fLBNEOutNtpData->NdydzNear[idet] = (y-ydet_near[idet])/(z-zdet_near[idet]);
      
      LBNENuWeight nuwgh;
      G4double nu_wght;
      G4double nu_energy;
      std::vector<double> r_det;
      r_det.push_back(xdet_near[idet]/cm);
      r_det.push_back(ydet_near[idet]/cm);
      r_det.push_back(zdet_near[idet]/cm);
      nuwgh.GetWeight(fLBNEOutNtpData, r_det,nu_wght,nu_energy);
      fLBNEOutNtpData->NenergyN[idet] = nu_energy; //in GeV
      fLBNEOutNtpData->NWtNear[idet]  = nu_wght;
   }
   //end near det

   //
   //Far Detector
   // Neutrino data at different points
   // need neutrino parent info to be filled in fLBNEOutNtpData by this point
   const std::vector<G4double> xdet_far = LBNEData->GetXDetFar();
   const std::vector<G4double> ydet_far = LBNEData->GetYDetFar();
   const std::vector<G4double> zdet_far = LBNEData->GetZDetFar();
   
   if(xdet_far.size() != ydet_far.size() || 
      xdet_far.size() != zdet_far.size() ||
      ydet_far.size() != zdet_far.size())
   {
      G4cout << "LBNEAnalysis::FillNeutrinoNtuple - "
	     << "Far Detector vectors are not the same size." << G4endl; 
   }
   
   ndets = xdet_far.size();
   for(unsigned int idet = 0; idet < ndets; ++idet)
   {
      fLBNEOutNtpData->NdxdzFar[idet] = (x-xdet_far[idet])/(z-zdet_far[idet]);
      fLBNEOutNtpData->NdydzFar[idet] = (y-ydet_far[idet])/(z-zdet_far[idet]);
      
      LBNENuWeight nuwgh;
      G4double nu_wght;
      G4double nu_energy;
      std::vector<double> r_det;
      r_det.push_back(xdet_far[idet]/cm);
      r_det.push_back(ydet_far[idet]/cm);
      r_det.push_back(zdet_far[idet]/cm);
      nuwgh.GetWeight(fLBNEOutNtpData, r_det,nu_wght,nu_energy);
      fLBNEOutNtpData->NenergyF[idet] = nu_energy; //in GeV
      fLBNEOutNtpData->NWtFar[idet]   = nu_wght;
   }
   //end far det



   //
   //loop over the entire list of neutirno parents
   //to see where each parent was along the beamline
   //
   /* for printing
   G4int trackID                   = track.GetParentID();

   std::vector<G4int> trackIDvec;

   while (trackID > 0)
   {
      trackIDvec.push_back(trackID);
      LBNETrajectory* TrackTrajectory = GetTrajectory(trackID);
      trackID = TrackTrajectory->GetParentID();
   }
   if(fLBNEOutNtpData->evtno == 1450 || fLBNEOutNtpData->evtno == 1679)
   {
      std::cout << "Event ID: " << fLBNEOutNtpData->evtno << " Trajectories: "; 
      for(unsigned int itrk = trackIDvec.size(); itrk > 0; --itrk)
      {
	 std::cout << trackIDvec[itrk-1] << " ";
      }
      std::cout << std::endl;
   }
*/

   G4int trackID                   = track.GetParentID();
   LBNETrajectory* TrackTrajectory = GetTrajectory(trackID);         

   while (trackID > 0)
   {
      LBNEAnalysis::TrackThroughGeometry(TrackTrajectory);
      
      trackID = TrackTrajectory->GetParentID();
      if(trackID > 0) TrackTrajectory = GetTrajectory(trackID);      


   }//end while trackID > 0
   //end tracking through geometry


  //
  //Done. Fill Tree.
  //
  fOutTree->Fill();  

  //
  // Write to ascii file
  //
  if (LBNEData->GetCreateASCIIOutput())
  {
     std::string asciiFileName = LBNEData->GetASCIIOutputFileName();
     std::ofstream asciiFile(asciiFileName.c_str(), std::ios::app);
     if(asciiFile.is_open())
     {
	asciiFile << fLBNEOutNtpData->Ntype<< " " << fLBNEOutNtpData->Nenergy << " " 
		  << fLBNEOutNtpData->NenergyN[0] << " " << fLBNEOutNtpData->NWtNear[0];
	asciiFile << " " << fLBNEOutNtpData->NenergyF[0] << " " << fLBNEOutNtpData->NWtFar[0] 
		  <<" "<<fLBNEOutNtpData->Nimpwt<< G4endl; 
	asciiFile.close();
     }
  }


}

//-----------------------------------------------------------------------------------------
//
void LBNEAnalysis::FillTrackingNtuple(const G4Track& track, LBNETrajectory* currTrajectory)
{
   
   if (!LBNEData->GetCreateOutput()) return;

   fLBNEOutNtpData -> Clear();

   G4RunManager* pRunManager = G4RunManager::GetRunManager();
   LBNEPrimaryGeneratorAction *NPGA = 
      (LBNEPrimaryGeneratorAction*)(pRunManager)->GetUserPrimaryGeneratorAction();




   fLBNEOutNtpData->protonN    = NPGA->GetProtonNumber();
   
   fLBNEOutNtpData->run        = pRunManager->GetCurrentRun()->GetRunID();
   fLBNEOutNtpData->evtno      = pRunManager->GetCurrentEvent()->GetEventID();
   fLBNEOutNtpData->beamHWidth = LBNEData->GetBeamSigmaX()/cm;
   fLBNEOutNtpData->beamVWidth = LBNEData->GetBeamSigmaY()/cm;
   fLBNEOutNtpData->beamX      = LBNEData->GetBeamXPosition()/cm;
   fLBNEOutNtpData->beamY      = LBNEData->GetBeamYPosition()/cm;
   
   //G4int particleID = track.GetParentID();
   G4ThreeVector protonOrigin = NPGA->GetProtonOrigin();
   fLBNEOutNtpData->protonX            = protonOrigin[0];
   fLBNEOutNtpData->protonY            = protonOrigin[1];
   fLBNEOutNtpData->protonZ            = protonOrigin[2];
   
   G4ThreeVector protonMomentum = NPGA->GetProtonMomentum();
   fLBNEOutNtpData->protonPx             = protonMomentum[0];
   fLBNEOutNtpData->protonPy             = protonMomentum[1];
   fLBNEOutNtpData->protonPz             = protonMomentum[2];
   
   fLBNEOutNtpData->nuTarZ      = LBNEData->GetTargetZ0(0);
   fLBNEOutNtpData->hornCurrent = LBNEData->GetHornCurrent()/ampere/1000.;






   G4int trackID                   = track.GetTrackID();
   G4String currentTrackVolName    = track.GetVolume() -> GetName();
   LBNETrajectory* TrackTrajectory = currTrajectory;

   //G4int eventID                   =  G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
   //std::cout << "Event: " << eventID << " TrackID: " << trackID << " curr vol = " << currentTrackVolName << std::endl;


   while (trackID > 0)
   {
      
      LBNEAnalysis::TrackThroughGeometry(TrackTrajectory);

      trackID = TrackTrajectory->GetParentID();
      if(trackID > 0) TrackTrajectory = GetTrajectory(trackID);

   }
  
   fOutTree->Fill();  
   
}

//-----------------------------------------------------------------------------------------
void LBNEAnalysis::TrackThroughGeometry(const LBNETrajectory* TrackTrajectory)
{
   G4int trackID = TrackTrajectory-> GetTrackID();

   G4int numberOfPoints            = TrackTrajectory->GetPointEntries();

   for (G4int ii=numberOfPoints-1; ii > -1; --ii)
   {
      
      G4String prevolname = TrackTrajectory->GetPreStepVolumeName(ii);
      G4String postvolname = "";
      if(ii < numberOfPoints-1) postvolname = TrackTrajectory->GetPreStepVolumeName(ii+1);
      
      G4ThreeVector ParticleMomentum = TrackTrajectory->GetMomentum(ii);              
      G4ThreeVector ParticlePosition = TrackTrajectory->GetPoint(ii)->GetPosition();
      
      
      
      /*
	G4double steplength = TrackTrajectory->GetStepLength(ii);              
      // if(fLBNEOutNtpData->evtno == 22)
      if(prevolname.contains("_in"))
      //if(prevolname.contains("PH02-01_in"))
      {
      //++count;
      //if(count > 1)
      std::cout << "eventid: " << fLBNEOutNtpData->evtno << " trackid: " << trackID << " ii = " << ii 
      << " particle = " << TrackTrajectory->GetParticleName()
      << " prestepvolname = " << prevolname << " postvolname = " << postvolname 
      << " steplength = " << steplength/mm << " mm" << std::endl;
      }
      */
    
      TrackPoint_t TrkPt;
      TrkPt.type = LBNEParticleCode::AsInt(LBNEParticleCode::StringToEnum(TrackTrajectory->GetParticleName()));
      TrkPt.x     = ParticlePosition[0]/cm;
      TrkPt.y     = ParticlePosition[1]/cm;
      TrkPt.z     = ParticlePosition[2]/cm;
      TrkPt.px    = ParticleMomentum[0]/GeV;
      TrkPt.py    = ParticleMomentum[1]/GeV;
      TrkPt.pz    = ParticleMomentum[2]/GeV;
      TrkPt.trkid = trackID;
      TrkPt.impwt = TrackTrajectory->GetNImpWt();
      TrkPt.gen   = (TrackTrajectory->GetTgen())-1;
      
      //
      //particle created in the target
      //
      if (prevolname.contains("TGT") && ii==0)
      {
	 fLBNEOutNtpData -> AddTrkPoint(TrkPoint::StringToEnum("Target"), TrkPt);
      }
      //
      //particle as exits target
      //
      if ((prevolname.contains("TargetInnerExit")) && postvolname.contains("TargetOuterExit"))
      {
	 /*if(fLBNEOutNtpData->evtno == 1450 || fLBNEOutNtpData->evtno == 1679)
	   {
	   std::cout << "EVENT ID: " << fLBNEOutNtpData->evtno << std::endl;
	   std::cout << "   LBNEAnalysis particle exiting target..." << std::endl;
	   std::cout << "      track id = " <<  TrkPt.trkid << std::endl;
	   std::cout << "      type     = " <<  TrkPt.type << std::endl;
	   std::cout << "      x pos    = " <<  TrkPt.x << std::endl;
	   std::cout << "      y pos    = " <<  TrkPt.y << std::endl;
	   std::cout << "      z pos    = " <<  TrkPt.z << std::endl;
	   std::cout << "      px mom   = " <<  TrkPt.px << std::endl;
	   std::cout << "      py mom   = " <<  TrkPt.py << std::endl;
	   std::cout << "      pz mom   = " <<  TrkPt.pz << std::endl;
	   std::cout << "   LBNEAnalysis Nu Parent found exiting target..." << std::endl;
	   std::cout << "      track id = " <<  tptrkid << std::endl;
	   std::cout << "      type     = " <<  fLBNEOutNtpData->tptype << std::endl;
	   std::cout << "      x pos    = " <<  fLBNEOutNtpData->tvx << std::endl;
	   std::cout << "      y pos    = " <<  fLBNEOutNtpData->tvy << std::endl;
	   std::cout << "      z pos    = " <<  fLBNEOutNtpData->tvz << std::endl;
	   std::cout << "      px mom   = " <<  fLBNEOutNtpData->tpx << std::endl;
	   std::cout << "      py mom   = " <<  fLBNEOutNtpData->tpy << std::endl;
	   std::cout << "      pz mom   = " <<  fLBNEOutNtpData->tpz << std::endl;
	   }*/
	 
	 fLBNEOutNtpData -> AddTrkPoint(TrkPoint::StringToEnum("TargetExit"), TrkPt);
      }
      //
      //particle at plane of end of tgt. This only makes sense 
      //if only running with the target and target hall constructed.
      //otherwise this will
      //
      if (prevolname.contains("TgtEndPlane"))
      {
	 fLBNEOutNtpData -> AddTrkPoint(TrkPoint::StringToEnum("TargetEndPlane"), TrkPt);
      }


      if(LBNEData->GetSimulation() == "Target Tracking") continue;



      //particle enters horn 1
      if ((prevolname.contains("TGAR")) && postvolname.contains("PH01"))
      {
	 fLBNEOutNtpData -> AddTrkPoint(TrkPoint::StringToEnum("Horn1Enter"), TrkPt);
      }
      //particle at neck of horn 1 the last entry will be just before it leaves the neck
      //if (prevolname.contains("PH01-02"))
      if(prevolname.contains(LBNEData->GetHornNeckPartName(1)) ||
	 prevolname.contains("PH01NeckPlane"))
      {
	 fLBNEOutNtpData -> AddTrkPoint(TrkPoint::StringToEnum("Horn1NeckPlane"), TrkPt);
      }
      //particle exists horn 1
      if ((prevolname.contains("PH01")) && postvolname.contains("TGAR"))
      {
	 fLBNEOutNtpData -> AddTrkPoint(TrkPoint::StringToEnum("Horn1Exit"), TrkPt);
      }
      //particle at end plane of horn 1
      if (prevolname.contains("PH01EndPlane"))
      {
	 fLBNEOutNtpData -> AddTrkPoint(TrkPoint::StringToEnum("Horn1EndPlane"), TrkPt);
      }
      //particle enters horn 2
      if ((prevolname.contains("TGAR")) && postvolname.contains("PH02"))
      {
	 fLBNEOutNtpData -> AddTrkPoint(TrkPoint::StringToEnum("Horn2Enter"), TrkPt);
      }
      //particle at neck of horn 2 the last entry will be just before it leaves the neck
      //if (prevolname.contains("PH02-02"))
      if(prevolname.contains(LBNEData->GetHornNeckPartName(2)) ||
	 prevolname.contains("PH02NeckPlane"))
      {
	 fLBNEOutNtpData -> AddTrkPoint(TrkPoint::StringToEnum("Horn2NeckPlane"), TrkPt);
      }
      //particle exits horn 2
      if ((prevolname.contains("PH02")) && postvolname.contains("TGAR"))
      {
	 fLBNEOutNtpData -> AddTrkPoint(TrkPoint::StringToEnum("Horn2Exit"), TrkPt);
      }
      //particle at end plane of horn 2
      if (prevolname.contains("PH02EndPlane"))
      {
	 fLBNEOutNtpData -> AddTrkPoint(TrkPoint::StringToEnum("Horn2EndPlane"), TrkPt);
      }
      //Particle enters DP
      if ( (prevolname.contains("UPWN") || 
	    prevolname.contains("DPipeWall") ||
	    prevolname.contains("DNWN") ) && 
	   postvolname.contains("DPipeVol") )
      {
	 if(prevolname.contains("DNWN")) 
	    std::cout << "LBNEAnalysis::FillNeutrinoNtuple - " 
		      << "Event ID = " << fLBNEOutNtpData->evtno 
			 << ". Particle is entering Decay Pipe from the Downstream Window" 
		      << "The particle is a " << TrackTrajectory->GetParticleName() 
		      << ", type = " << TrkPt.type << std::endl;
	 
	 fLBNEOutNtpData -> AddTrkPoint(TrkPoint::StringToEnum("DPipeEnter"), TrkPt);
      }
      //Particle exits DP
      if ( prevolname.contains("DPipeVol")  &&
	   (postvolname.contains("UPWN")      || 
	    postvolname.contains("DPipeWall") ||
	    postvolname.contains("DNWN") ) )
	 
      {
	 if(prevolname.contains("UPWN")) 
	 {
	    std::cout << "LBNEAnalysis::FillNeutrinoNtuple - " 
		      << "Event ID = " << fLBNEOutNtpData->evtno 
		      << ". Particle is exiting Decay Pipe from the Upstream Window. " 
		      << "The particle is a " << TrackTrajectory->GetParticleName() 
		      << ", type = " << TrkPt.type << std::endl;
	 }
	 
       fLBNEOutNtpData -> AddTrkPoint(TrkPoint::StringToEnum("DPipeExit"), TrkPt);
      }
      
   }//end loop over npts in Track
   
}



















//------------------------------------------------------------------------------------
LBNETrajectory* LBNEAnalysis::GetParentTrajectory(G4int parentID)
{

   if(LBNEData->GetDebugLevel() == 10) { G4cout << "LBNEAnalysis::GetParentTrajectory() called." << G4endl;}
      
  G4TrajectoryContainer* container = 
    G4RunManager::GetRunManager()->GetCurrentEvent()->GetTrajectoryContainer();
  if(container==0) return 0;

  TrajectoryVector* vect = container->GetVector();
  G4VTrajectory* tr;
  G4int ii = 0; 
  while (ii<G4int(vect->size())){  
    tr = (*vect)[ii]; 
    LBNETrajectory* tr1 = (LBNETrajectory*)(tr);  
    if(tr1->GetTrackID() == parentID) return tr1; 
    ++ii; 
  }

  return 0;
}

//----------------------------------------------------------------------------------------------------------
LBNETrajectory* LBNEAnalysis::GetTrajectory(G4int trackID)
{
   G4TrajectoryContainer* container = G4RunManager::GetRunManager()->GetCurrentEvent()->GetTrajectoryContainer();
   
   if(container==0) 
   {
      G4cout << "LBNEAnalysis::GetTrajectory - PROBLEM: No Trajectory Container for track ID = " << trackID << endl;
      return 0;
   }   

   TrajectoryVector* vect = container->GetVector();
   G4VTrajectory* tr;
   G4int ii = 0; 
   while (ii < G4int(vect->size()))
   {  
      tr = (*vect)[ii]; 
      LBNETrajectory* tr1 = (LBNETrajectory*)(tr);  
      if(tr1->GetTrackID() == trackID) return tr1; 
      ++ii; 
   }

   G4cout << "LBNEAnalysis::GetTrajectory - PROBLEM: Failed to find track with ID = " << trackID << endl;
   
   return 0;
}
