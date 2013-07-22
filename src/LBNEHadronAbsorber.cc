//----------------------------------------------------------------------
// LBNEHadronAbsorber.cc covers the elements of the Hadron Absorber in
// the G4NuMI framework. The blocks and elements within the parent
// volumes are all oriented in a refernce frame that has gravity going
// straight down. To properly align the NuMI Decay tunnel and beam,
// which have a downward slope, with the elements of the MC, the parent
// volumes are rotated. In the G4NuMI world, the beam is horizontal,
// while all the MC elements are rotated. 
//
// $Id: LBNEHadronAbsorber.cc,v 1.2.4.1 2013/07/22 15:06:59 robj137 Exp $
//----------------------------------------------------------------------

#include "LBNEDetectorConstruction.hh"
#include "LBNEDataInput.hh"
#include "G4VisAttributes.hh"
#include "G4PVPlacement.hh"

#include <G4GDMLParser.hh>

#include <TGeoManager.h>
#include <TGeoNode.h>
#include <TGeoVolume.h>

#include <fstream>

#include "globals.hh"


void LBNEDetectorConstruction::ConstructLBNEHadronAbsorber()
{

   LBNEDataInput *LBNEData = LBNEDataInput::GetLBNEDataInput();

   G4cout << "Importing hadron absorber gdml file... " << G4endl;
    
    std::string relpathgdml("gdml/lbne_absorber_only.gdml");
    std::ifstream gdmlfile( relpathgdml.c_str() ); 
    
    if ( gdmlfile ) {
      
         
        G4double DP_end = LBNEData->DecayPipeZ0 + LBNEData->DecayPipeLength;
        G4RotationMatrix *absorberRot=new G4RotationMatrix();
        //zrot->rotateX(-101*mrad);
        absorberRot->rotateZ(-90*deg);
        G4double beam_angle = LBNEData->BeamAngle;
        beam_angle = 0*mrad;
         G4double in = .0254*m;
         G4double CShld_length = 72*in;
         G4double xo = DP_end + 10.25*12*in;
         G4double xp = (-6.0625*12*in - CShld_length/2.0);
         G4double yp = 0;
         G4double yo = -1*in; 
         G4double z_shld = xp*cos(beam_angle) - yp*sin(beam_angle) + xo;
         G4double y_shld = xp*sin(beam_angle) + yp*cos(beam_angle) + yo;
          
         G4ThreeVector tunnelPos = G4ThreeVector(0,0,LBNEData->TunnelLength/2.+LBNEData->TunnelZ0);
         G4ThreeVector shldpos = G4ThreeVector(0, y_shld, z_shld)-tunnelPos;
         
         const G4String GDMLfile = static_cast<const G4String>( relpathgdml );  
         G4GDMLParser parser;
         parser.Read( GDMLfile );
         fConcShld = parser.GetVolume( "Conc_SH" ); 
         //fAHTop = parser.GetVolume( "AH_top" ); 
         //fAHBack = parser.GetVolume( "AH_back" ); 
         //fMuonAlk = parser.GetVolume( "AH_Muon_alk" ); 
         
         G4ThreeVector conc = parser.GetPosition( "Conc_SH_1inTOPpos" )+shldpos;         
         //G4ThreeVector top = parser.GetPosition( "AH_top_1inTOPpos" ) +shldpos;         
         //G4ThreeVector back = parser.GetPosition( "AH_back_1inTOPpos" )+shldpos;         
         //G4ThreeVector muon = parser.GetPosition( "AH_Muon_alk_1inTOPpos" )+shldpos;         

         // rotate about beamline z-axis, zrot rotates about z-axis relative to local object 
         G4ThreeVector concRotZ( -conc.y(), conc.x(), conc.z() );
         //G4ThreeVector topRotZ( -top.y(), top.x(), top.z() );
         //G4ThreeVector backRotZ( -back.y(), back.x(), back.z() );
         //G4ThreeVector muonRotZ( -muon.y(), muon.x(), muon.z() );

         new G4PVPlacement(absorberRot, concRotZ, "Conc_SH", fConcShld, pvTUNE, false, 0);
         //new G4PVPlacement(zrot, topRotZ, "AH_top", fAHTop, pvTUNE, false, 0);
         //new G4PVPlacement(zrot, backRotZ, "AH_back", fAHBack, pvTUNE, false, 0);
         //new G4PVPlacement(zrot, muonRotZ, "AH_Muon_alk", fMuonAlk, pvTUNE, false, 0);


    } else
       std::cout << "Cannot find the gdml file for the hadron absorber" << std::endl;


}



