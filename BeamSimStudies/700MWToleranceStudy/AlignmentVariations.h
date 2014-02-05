#include <string>
#include "AlignmentVariation.h"

class AlignmentVariations {

 public:
  
  AlignmentVariation *HornCurrent;
  AlignmentVariation *Horn1XTilt;
  AlignmentVariation *Horn1YTilt;
  AlignmentVariation *Horn2XTilt;
  AlignmentVariation *Horn2YTilt;
  AlignmentVariation *Horn1XOffset;
  AlignmentVariation *Horn1YOffset;
  AlignmentVariation *Horn2XOffset;
  AlignmentVariation *Horn2YOffset;
  AlignmentVariation *TargetXOffset;
  AlignmentVariation *TargetYOffset;
  AlignmentVariation *TargetXTilt;
  AlignmentVariation *TargetYTilt;
  AlignmentVariation *BeamSigmaX;
  AlignmentVariation *BeamSigmaY;
  AlignmentVariation *DecayPipeRadius;
  AlignmentVariation *BeamOffsetX;
  AlignmentVariation *BeamOffsetY;
  AlignmentVariation *FarDetX;
  AlignmentVariation *FarDetY;
  AlignmentVariation *NearDetX;
  AlignmentVariation *NearDetY;
  AlignmentVariation *WaterLayerThickness;
  AlignmentVariation *BaffleScraping;
  AlignmentVariation *NoShielding;
  AlignmentVariation *DecayPipeOffsetX;
  AlignmentVariation *SkinDepthIC;
  AlignmentVariation *BeamTiltX;
  AlignmentVariation *BeamTiltY;
  AlignmentVariation *TargetDensity;

    
  AlignmentVariations() {
    
    HornCurrent = new AlignmentVariation("HornCurrent","current","v3r0p10");
    HornCurrent->SetLabel("Horn Current (2 kA)");
    HornCurrent->SetUnits("kA");
    HornCurrent->SetDisk("/lbne/data2");
    HornCurrent->AddVariation("212");
    HornCurrent->AddVariation("208");
    HornCurrent->AddVariation("204");
    HornCurrent->AddVariation("202");
    HornCurrent->AddVariation("198");
    HornCurrent->AddVariation("196");
    HornCurrent->AddVariation("192");
    HornCurrent->AddVariation("188");
    HornCurrent->SetTolerance("2");
    HornCurrent->SetCV("200");

    Horn1XOffset = new AlignmentVariation("Horn1XOffset","macro","v3r0p10");
    Horn1XOffset->SetUnits("mm");
    Horn1XOffset->SetLabel("Horn 1 X Shift (0.5 mm)");
    Horn1XOffset->SetDisk("/lbne/data2");
    Horn1XOffset->AddVariation("0.5");
    Horn1XOffset->AddVariation("1");
    Horn1XOffset->AddVariation("2");
    Horn1XOffset->AddVariation("2.5");
    Horn1XOffset->SetTolerance("0.5");

    Horn1YOffset = new AlignmentVariation("Horn1YOffset","macro","v3r0p10");
    Horn1YOffset->SetUnits("mm");
    Horn1YOffset->SetLabel("Horn 1 Y Shift (0.5 mm)");
    Horn1YOffset->SetDisk("/lbne/data2");
    Horn1YOffset->AddVariation("0.5");
    Horn1YOffset->AddVariation("1");
    Horn1YOffset->AddVariation("2");
    Horn1YOffset->AddVariation("2.5");
    Horn1YOffset->SetTolerance("0.5");

    Horn2XOffset = new AlignmentVariation("Horn2XOffset","macro","v3r0p10");
    Horn2XOffset->SetUnits("mm");
    Horn2XOffset->SetLabel("Horn 2 X Shift (0.5 mm)");
    Horn2XOffset->SetDisk("/lbne/data2");
    Horn2XOffset->AddVariation("0.5");
    Horn2XOffset->AddVariation("1");
    Horn2XOffset->AddVariation("2");
    Horn2XOffset->AddVariation("2.5");
    Horn2XOffset->SetTolerance("0.5");

    Horn2YOffset = new AlignmentVariation("Horn2YOffset","macro","v3r0p10");
    Horn2YOffset->SetUnits("mm");
    Horn2YOffset->SetLabel("Horn 2 Y Shift (0.5 mm)");
    Horn2YOffset->SetDisk("/lbne/data2");
    Horn2YOffset->AddVariation("0.5");
    Horn2YOffset->AddVariation("1");
    Horn2YOffset->AddVariation("2");
    Horn2YOffset->AddVariation("2.5");
    Horn2YOffset->SetTolerance("0.5");

    TargetYOffset = new AlignmentVariation("TargetYOffset","macro","v3r0p10");
    TargetYOffset->SetUnits("mm");
    TargetYOffset->SetLabel("Target Y Shift (0.5 mm)");
    TargetYOffset->SetDisk("/lbne/data2");
    TargetYOffset->AddVariation("0.5");
    TargetYOffset->AddVariation("1");
    TargetYOffset->AddVariation("2");
    TargetYOffset->AddVariation("3");
    TargetYOffset->SetTolerance("0.5");

    TargetXOffset = new AlignmentVariation("TargetXOffset","macro","v3r0p10");
    TargetXOffset->SetUnits("mm");
    TargetXOffset->SetLabel("Target X Shift (0.5 mm)");
    TargetXOffset->SetDisk("/lbne/data2");
    TargetXOffset->AddVariation("0.5");
    TargetXOffset->AddVariation("1");
    TargetXOffset->AddVariation("2");
    TargetXOffset->AddVariation("3");
    TargetXOffset->SetTolerance("0.5");

    TargetYTilt = new AlignmentVariation("TargetYTilt","macro","v3r0p10");
    TargetYTilt->SetUnits("mm");
    TargetYTilt->SetLabel("Target Y Tilt (0.5 mm)");
    TargetYTilt->SetDisk("/lbne/data2");
    TargetYTilt->AddVariation("0.5");
    TargetYTilt->AddVariation("1");
    TargetYTilt->AddVariation("2");
    TargetYTilt->AddVariation("3");
    TargetYTilt->SetTolerance("0.5");

    TargetXTilt = new AlignmentVariation("TargetXTilt","macro","v3r0p10");
    TargetXTilt->SetUnits("mm");
    TargetXTilt->SetLabel("Target X Tilt (0.5 mm)");
    TargetXTilt->SetDisk("/lbne/data2");
    TargetXTilt->AddVariation("0.5");
    TargetXTilt->AddVariation("1");
    TargetXTilt->AddVariation("2");
    TargetXTilt->AddVariation("3");
    TargetXTilt->SetTolerance("0.5");

    Horn1XTilt = new AlignmentVariation("Horn1XTilt","macro","v3r0p10");
    Horn1XTilt->SetDisk("/lbne/data2");
    Horn1XTilt->SetUnits("mm");
    Horn1XTilt->SetLabel("Horn 1 X Tilt (0.5 mm)");
    Horn1XTilt->AddVariation("0.25");
    Horn1XTilt->AddVariation("0.5");
    Horn1XTilt->AddVariation("1");
    Horn1XTilt->AddVariation("1.5");
    Horn1XTilt->AddVariation("2");
    Horn1XTilt->AddVariation("2.5");
    Horn1XTilt->SetTolerance("0.5");

    Horn1YTilt = new AlignmentVariation("Horn1YTilt","macro","v3r0p10");
    Horn1YTilt->SetUnits("mm");
    Horn1YTilt->SetLabel("Horn 1 Y Tilt (0.5 mm)");
    Horn1YTilt->SetDisk("/lbne/data2");
    Horn1YTilt->AddVariation("0.25");
    Horn1YTilt->AddVariation("0.5");
    Horn1YTilt->AddVariation("1");
    //Horn1YTilt->AddVariation("1.5");
    Horn1YTilt->AddVariation("2");
    //Horn1YTilt->AddVariation("2.5");
    Horn1YTilt->SetTolerance("0.5");

    Horn2XTilt = new AlignmentVariation("Horn2XTilt","macro","v3r0p10");
    Horn2XTilt->SetUnits("mm");
    Horn2XTilt->SetLabel("Horn 2 X Tilt (0.5 mm)");
    Horn2XTilt->SetDisk("/lbne/data2");
    Horn2XTilt->AddVariation("0.5");
    Horn2XTilt->AddVariation("1");
    Horn2XTilt->AddVariation("2");
    Horn2XTilt->AddVariation("2.5");
    Horn2XTilt->SetTolerance("0.5");

    Horn2YTilt = new AlignmentVariation("Horn2YTilt","macro","v3r0p10");
    Horn2YTilt->SetUnits("mm");
    Horn2YTilt->SetLabel("Horn 2 Y Tilt (0.5 mm)");
    Horn2YTilt->SetDisk("/lbne/data2");
    Horn2YTilt->AddVariation("0.5");
    Horn2YTilt->AddVariation("1");
    Horn2YTilt->AddVariation("2");
    //Horn2YTilt->AddVariation("2.5");
    Horn2YTilt->SetTolerance("0.5");

    BeamSigmaX = new AlignmentVariation("BeamSigmaX","macro","v3r0p10");
    BeamSigmaX->SetUnits("mm");
    BeamSigmaX->SetLabel("Beam Width X (0.1 mm)");
    BeamSigmaX->SetCV("1.3");
    BeamSigmaX->SetDisk("/lbne/data2");
    BeamSigmaX->AddVariation("0.7");
    BeamSigmaX->AddVariation("0.9");
    BeamSigmaX->AddVariation("1.1");
    BeamSigmaX->AddVariation("1.2");
    BeamSigmaX->AddVariation("1.4");
    BeamSigmaX->AddVariation("1.5");
    BeamSigmaX->AddVariation("1.7");
    BeamSigmaX->AddVariation("1.9");
    BeamSigmaX->SetTolerance("0.1");

    BeamSigmaY = new AlignmentVariation("BeamSigmaY","macro","v3r0p10");
    BeamSigmaY->SetUnits("mm");
    BeamSigmaY->SetLabel("Beam Width Y (0.1 mm)");
    BeamSigmaY->SetDisk("/lbne/data2");
    BeamSigmaY->SetCV("1.3");
    BeamSigmaY->AddVariation("0.7");
    BeamSigmaY->AddVariation("0.9");
    BeamSigmaY->AddVariation("1.1");
    BeamSigmaY->AddVariation("1.2");
    BeamSigmaY->AddVariation("1.4");
    BeamSigmaY->AddVariation("1.5");
    BeamSigmaY->AddVariation("1.7");
    BeamSigmaY->AddVariation("1.9");
    BeamSigmaY->SetTolerance("0.1");

    DecayPipeRadius = new AlignmentVariation("DecayPipeRadius","macro","v3r0p10");
    DecayPipeRadius->SetUnits("m");
    DecayPipeRadius->SetLabel("Decay Pipe Radius (0.1 m)");
    DecayPipeRadius->SetCV("2.0");
    DecayPipeRadius->SetDisk("/lbne/data2");
    DecayPipeRadius->AddVariation("1.4");
    DecayPipeRadius->AddVariation("1.6");
    DecayPipeRadius->AddVariation("1.8");
    DecayPipeRadius->AddVariation("1.9");
    DecayPipeRadius->AddVariation("2.1");
    DecayPipeRadius->AddVariation("2.2");
    DecayPipeRadius->AddVariation("2.4");
    DecayPipeRadius->AddVariation("2.6");
    DecayPipeRadius->SetTolerance("0.1");

    BeamOffsetX = new AlignmentVariation("NominalX","macro","v3r0p10");
    BeamOffsetX->SetCV("0");
    BeamOffsetX->SetLabel("Beam Shift X (0.45 mm)");
    BeamOffsetX->SetTolerance(".45");
    BeamOffsetX->SetUnits("mm");
    BeamOffsetX->SetUser("bashyal8");
    BeamOffsetX->AddVariation("1");
    BeamOffsetX->AddVariation("2");
    BeamOffsetX->AddVariation("3");

    BeamOffsetY = new AlignmentVariation("NominalY","macro","v3r0p10");
    BeamOffsetY->SetCV("0");
    BeamOffsetY->SetLabel("Beam Shift Y (0.45 mm)");
    BeamOffsetY->SetTolerance(".45");
    BeamOffsetY->SetUnits("mm");
    BeamOffsetY->SetUser("bashyal8");
    BeamOffsetY->AddVariation("1");
    BeamOffsetY->AddVariation("2");
    BeamOffsetY->AddVariation("3");

    NearDetX = new AlignmentVariation("LBNENDX","location","v3r0p10");
    NearDetX->SetCV("0");
    NearDetX->SetLabel("Near Det Shift X (25 mm)");
    NearDetX->SetTolerance("25");
    NearDetX->SetUnits("mm");
    NearDetX->AddVariation("255");
    NearDetX->AddVariation("510");

    NearDetY = new AlignmentVariation("LBNENDY","location","v3r0p10");
    NearDetY->SetCV("0");
    NearDetY->SetLabel("Near Det Shift Y (25 mm)");
    NearDetY->SetTolerance("25");
    NearDetY->SetUnits("mm");
    NearDetY->AddVariation("255");
    NearDetY->AddVariation("510");

    FarDetX = new AlignmentVariation("LBNEFDX","location","v3r0p10");
    FarDetX->SetCV("0");
    FarDetX->SetLabel("Far Det Shift X (21 m)");
    FarDetX->SetDisk("/lbne/data2");
    FarDetX->SetTolerance("21");
    FarDetX->SetUnits("m");
    FarDetX->AddVariation("21");
    FarDetX->AddVariation("42");
    FarDetX->AddVariation("84");
    FarDetX->AddVariation("500");
    FarDetX->AddVariation("2000");

    FarDetY = new AlignmentVariation("LBNEFDY","location","v3r0p10");
    FarDetY->SetDisk("/lbne/data2");
    FarDetY->SetCV("0");
    FarDetY->SetLabel("Far Det Shift Y (21 m)");
    FarDetY->SetTolerance("21");
    FarDetY->SetUnits("m");
    FarDetY->AddVariation("21");
    FarDetY->AddVariation("42");
    FarDetY->AddVariation("84");
    FarDetY->AddVariation("500");
    FarDetY->AddVariation("2000");
    
    WaterLayerThickness = new AlignmentVariation("NominalWaterLayer_","macro","v3r0p10");
    WaterLayerThickness->SetLabel("Water Thickness (0.5 mm)");
    WaterLayerThickness->SetCV("0");
    WaterLayerThickness->SetTolerance("0.5");
    WaterLayerThickness->SetUnits("mm");
    WaterLayerThickness->AddVariation("0.5");
    WaterLayerThickness->AddVariation("1");
    WaterLayerThickness->AddVariation("1.5");

    BaffleScraping = new AlignmentVariation("BeamOffsetX7Sigma0pnt001","shift","v3r0p10");
    BaffleScraping->SetLabel("Baffle Scraping (0.25%)");
    BaffleScraping->SetScaleFactor(0.0025);
    BaffleScraping->SetUser("seongtae");

    NoShielding = new AlignmentVariation("NoShielding","shift","v3r0p10");

    DecayPipeOffsetX = new AlignmentVariation("DecayPipeOffsetX","macro","v3r0p10");
    DecayPipeOffsetX->SetLabel("Decay Pipe Shift X (20 mm)");
    DecayPipeOffsetX->SetCV("0");
    DecayPipeOffsetX->SetTolerance("20");
    DecayPipeOffsetX->SetUnits("");
    DecayPipeOffsetX->AddVariation("20");
    DecayPipeOffsetX->AddVariation("40");
    DecayPipeOffsetX->AddVariation("60");
    DecayPipeOffsetX->AddVariation("80");
    DecayPipeOffsetX->AddVariation("100");
    DecayPipeOffsetX->AddVariation("300");
    DecayPipeOffsetX->AddVariation("500");
    DecayPipeOffsetX->AddVariation("700");
    DecayPipeOffsetX->AddVariation("900");
    DecayPipeOffsetX->SetUser("seongtae");

    SkinDepthIC = new AlignmentVariation("SkinDepthIC_","macro","v3r0p10");
    SkinDepthIC->SetCV("10");
    SkinDepthIC->SetLabel("IC Skin Depth (#infty#rightarrow6.6mm)");
    SkinDepthIC->SetTolerance("-3.4");
    SkinDepthIC->SetUnits("mm");
    SkinDepthIC->AddVariation("2.5");
    SkinDepthIC->AddVariation("3.5");
    SkinDepthIC->AddVariation("4.5");
    SkinDepthIC->AddVariation("5.5");
    SkinDepthIC->AddVariation("6.6");
    
    BeamTiltX = new AlignmentVariation("Tilt_t","macro","v3r0p10");
    BeamTiltX->SetCV("0");
    BeamTiltX->SetLabel("BeamTilt X (70 #murad)");
    BeamTiltX->SetTolerance("70");
    BeamTiltX->SetUnits("");
    BeamTiltX->AddVariation("700");
    BeamTiltX->AddVariation("933");
    BeamTiltX->AddVariation("1166");
    BeamTiltX->AddVariation("1400");
    //BeamTiltX->AddVariation("1633");
    //BeamTiltX->AddVariation("1866");
    //BeamTiltX->AddVariation("2100");
    BeamTiltX->SetMacroSuffix("_p0");
    BeamTiltX->SetUser("tbw6971");

    BeamTiltY = new AlignmentVariation("Tilt_t","macro","v3r0p10");
    BeamTiltY->SetCV("0");
    BeamTiltY->SetLabel("BeamTilt Y (70 #murad)");
    BeamTiltY->SetTolerance("70");
    BeamTiltY->SetUnits("");
    BeamTiltY->AddVariation("700");
    BeamTiltY->AddVariation("933");
    BeamTiltY->AddVariation("1166");
    BeamTiltY->AddVariation("1400");
    //BeamTiltY->AddVariation("1633");
    //BeamTiltY->AddVariation("1866");
    //BeamTiltY->AddVariation("2100");
    BeamTiltY->SetMacroSuffix("_p90d");
    BeamTiltY->SetUser("tbw6971");

    TargetDensity = new AlignmentVariation("Density_","macro","v3r0p10");
    TargetDensity->SetCV("1.78");
    TargetDensity->SetLabel("Target Density (2%)");
    TargetDensity->SetTolerance("0.0356");
    TargetDensity->SetUnits("");
    TargetDensity->AddVariation("1.6732");
    TargetDensity->AddVariation("1.7088");
    TargetDensity->AddVariation("1.7444");
    TargetDensity->AddVariation("1.7622");
    TargetDensity->AddVariation("1.7978");
    TargetDensity->AddVariation("1.8156");
    TargetDensity->AddVariation("1.8512");
    TargetDensity->AddVariation("1.8868");
  }

};

