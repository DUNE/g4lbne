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
  AlignmentVariation *NearDetX;
  AlignmentVariation *FarDetX;
  AlignmentVariation *FarDetY;
  AlignmentVariation *WaterLayerThickness;
  AlignmentVariation *BaffleScraping;
  AlignmentVariation *NoShielding;
  AlignmentVariation *DecayPipeOffsetX;
  AlignmentVariation *SkinDepthIC;
  AlignmentVariation *BeamTiltX;

    
  AlignmentVariations() {
    
    HornCurrent = new AlignmentVariation("HornCurrent","current","v3r0p10");
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
    Horn1XOffset->SetDisk("/lbne/data2");
    Horn1XOffset->AddVariation("0.5");
    Horn1XOffset->AddVariation("1");
    Horn1XOffset->AddVariation("2");
    Horn1XOffset->AddVariation("2.5");
    Horn1XOffset->SetTolerance("0.5");

    Horn1YOffset = new AlignmentVariation("Horn1YOffset","macro","v3r0p10");
    Horn1YOffset->SetUnits("mm");
    Horn1YOffset->SetDisk("/lbne/data2");
    Horn1YOffset->AddVariation("0.5");
    Horn1YOffset->AddVariation("1");
    Horn1YOffset->AddVariation("2");
    Horn1YOffset->AddVariation("2.5");
    Horn1YOffset->SetTolerance("0.5");

    Horn2XOffset = new AlignmentVariation("Horn2XOffset","macro","v3r0p10");
    Horn2XOffset->SetUnits("mm");
    Horn2XOffset->SetDisk("/lbne/data2");
    Horn2XOffset->AddVariation("0.5");
    Horn2XOffset->AddVariation("1");
    Horn2XOffset->AddVariation("2");
    Horn2XOffset->AddVariation("2.5");
    Horn2XOffset->SetTolerance("0.5");

    Horn2YOffset = new AlignmentVariation("Horn2YOffset","macro","v3r0p10");
    Horn2YOffset->SetUnits("mm");
    Horn2YOffset->SetDisk("/lbne/data2");
    Horn2YOffset->AddVariation("0.5");
    Horn2YOffset->AddVariation("1");
    Horn2YOffset->AddVariation("2");
    Horn2YOffset->AddVariation("2.5");
    Horn2YOffset->SetTolerance("0.5");

    TargetYOffset = new AlignmentVariation("TargetYOffset","macro","v3r0p10");
    TargetYOffset->SetUnits("mm");
    TargetYOffset->SetDisk("/lbne/data2");
    TargetYOffset->AddVariation("0.5");
    TargetYOffset->AddVariation("1");
    TargetYOffset->AddVariation("2");
    TargetYOffset->AddVariation("3");
    TargetYOffset->SetTolerance("0.5");

    TargetXOffset = new AlignmentVariation("TargetXOffset","macro","v3r0p10");
    TargetXOffset->SetUnits("mm");
    TargetXOffset->SetDisk("/lbne/data2");
    TargetXOffset->AddVariation("0.5");
    TargetXOffset->AddVariation("1");
    TargetXOffset->AddVariation("2");
    TargetXOffset->AddVariation("3");
    TargetXOffset->SetTolerance("0.5");

    TargetYTilt = new AlignmentVariation("TargetYTilt","macro","v3r0p10");
    TargetYTilt->SetUnits("mm");
    TargetYTilt->SetDisk("/lbne/data2");
    TargetYTilt->AddVariation("0.5");
    TargetYTilt->AddVariation("1");
    TargetYTilt->AddVariation("2");
    TargetYTilt->AddVariation("3");
    TargetYTilt->SetTolerance("0.5");

    TargetXTilt = new AlignmentVariation("TargetXTilt","macro","v3r0p10");
    TargetXTilt->SetUnits("mm");
    TargetXTilt->SetDisk("/lbne/data2");
    TargetXTilt->AddVariation("0.5");
    TargetXTilt->AddVariation("1");
    TargetXTilt->AddVariation("2");
    TargetXTilt->AddVariation("3");
    TargetXTilt->SetTolerance("0.5");

    Horn1XTilt = new AlignmentVariation("Horn1XTilt","macro","v3r0p10");
    Horn1XTilt->SetDisk("/lbne/data2");
    Horn1XTilt->SetUnits("mm");
    Horn1XTilt->AddVariation("0.25");
    Horn1XTilt->AddVariation("0.5");
    Horn1XTilt->AddVariation("1");
    Horn1XTilt->AddVariation("1.5");
    Horn1XTilt->AddVariation("2");
    Horn1XTilt->AddVariation("2.5");
    Horn1XTilt->SetTolerance("0.5");

    Horn1YTilt = new AlignmentVariation("Horn1YTilt","macro","v3r0p10");
    Horn1YTilt->SetUnits("mm");
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
    Horn2XTilt->SetDisk("/lbne/data2");
    Horn2XTilt->AddVariation("0.5");
    Horn2XTilt->AddVariation("1");
    Horn2XTilt->AddVariation("2");
    Horn2XTilt->AddVariation("2.5");
    Horn2XTilt->SetTolerance("0.5");

    Horn2YTilt = new AlignmentVariation("Horn2YTilt","macro","v3r0p10");
    Horn2YTilt->SetUnits("mm");
    Horn2YTilt->SetDisk("/lbne/data2");
    Horn2YTilt->AddVariation("0.5");
    Horn2YTilt->AddVariation("1");
    Horn2YTilt->AddVariation("2");
    //Horn2YTilt->AddVariation("2.5");
    Horn2YTilt->SetTolerance("0.5");

    BeamSigmaX = new AlignmentVariation("BeamSigmaX","macro","v3r0p10");
    BeamSigmaX->SetUnits("mm");
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

    NearDetX = new AlignmentVariation("NominalX","macro","v3r0p10");
    NearDetX->SetCV("0");
    NearDetX->SetTolerance(".025");
    NearDetX->SetUnits("mm");
    NearDetX->SetUser("bashyal8");
    NearDetX->AddVariation("1");
    NearDetX->AddVariation("2");
    NearDetX->AddVariation("3");
    //NearDetX->AddVariation("4");
    //NearDetX->AddVariation("5");
    //NearDetX->AddVariation("6");
    //NearDetX->AddVariation("7");
    //NearDetX->AddVariation("8");
    //NearDetX->AddVariation("9");

    FarDetX = new AlignmentVariation("LBNEFDX","location","v3r0p10");
    FarDetX->SetCV("0");
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
    FarDetY->SetTolerance("21");
    FarDetY->SetUnits("m");
    FarDetY->AddVariation("21");
    FarDetY->AddVariation("42");
    FarDetY->AddVariation("84");
    FarDetY->AddVariation("500");
    FarDetY->AddVariation("2000");
    
    WaterLayerThickness = new AlignmentVariation("NominalWaterLayer_","macro","v3r0p10");
    WaterLayerThickness->SetCV("0");
    WaterLayerThickness->SetTolerance("0.5");
    WaterLayerThickness->SetUnits("mm");
    WaterLayerThickness->AddVariation("0.5");
    WaterLayerThickness->AddVariation("1");
    WaterLayerThickness->AddVariation("1.5");

    BaffleScraping = new AlignmentVariation("BeamOffsetX7Sigma0pnt001","shift","v3r0p10");
    BaffleScraping->SetScaleFactor(0.0025);
    BaffleScraping->SetUser("seongtae");

    NoShielding = new AlignmentVariation("NoShielding","shift","v3r0p10");

    DecayPipeOffsetX = new AlignmentVariation("DecayPipeOffsetX","macro","v3r0p10");
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
    SkinDepthIC->SetCV("1000");
    SkinDepthIC->SetTolerance("993.4");
    SkinDepthIC->SetUnits("mm");
    SkinDepthIC->AddVariation("2.5");
    SkinDepthIC->AddVariation("6.6");
    SkinDepthIC->AddVariation("7.7");
    
    BeamTiltX = new AlignmentVariation("Tilt_t","macro","v3r0p10");
    BeamTiltX->SetCV("0");
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

  }

};

