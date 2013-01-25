
G4LBNE Documentation

The Beam Simulation Group Wiki is here
https://cdcvs.fnal.gov/redmine/projects/lbne-beamsim/wiki
This contains much information about Fermilab accounts,
g4lbne and flux files.

*************************************************************
Getting g4lbne
*************************************************************
get g4lbne from /cvs/projects/lbne-beamsim repository

in read mode:
cvs -d :pserver:anonymous@cdcvs.fnal.gov:/cvs/projects/lbne-beamsim checkout lbne-beamsim/g4lbne


(see Wiki first) in read/write mode:
cvs -d :ext:p-lbne-beamsim@cdcvs.fnal.gov:/cvs/projects/lbne-beamsim checkout lbne-beamsim/g4lbne

************************************************************


*************************************************************
Setting up products and work directory
*************************************************************
You need to setup Geant4 (and CLHEP) and ROOT products.
You will need to determine what products you want to use.

G4LBNE CAN BE INSTALLED AT ANY INSTITUTION(SEE THE WIKI).
BUT WE MUST ALL USE THE SAME PRODUCT VERSIONS!!!!!!!!!!


AT FERMILAB:
On lbnegpvm01 (At Fermilab) ..... in the g4lbne/setups directory
There is a setup_g4lbne_fnal.sh file. (This file will setup
Geant4, ROOT and CLHEP that are installed and used by Larsoft 
in the nusoft area at fermilab.)

IN THAT FILE DO THE FOLLOWING....
( look for ##!!!!!!!!!!!! in the setup_g4lbne_fnal.sh file)
You need to define a g4lbne work directory, for example
export G4WORKDIR="path to work directory/g4lbne_work"
this is where the binaries will be stored when compiling

Now....
source this file (in g4lbne directory do..)
source setups/setup_g4lbne_fnal.sh


*************************************************************
Compiling
*************************************************************

in the g4lbne directory,
for the first time type
make clean
then 
make

if there is an error message that says something about 
*** No rule to make target `g4lbneCint.o', needed by `libg4lbneDict'.  Stop.
or upon linking
not being able to find a lib.so file just type make again.

(If running with 32bit versions of Geant4, ROOT, CLHEP on a 64bit machine, 
there will be errors related to 64 bit compilation...these dont 
prevent the code from compiling and running.)  

once you get

... Done!
cp G4WORKDIR/bin/Linux-g++/g4lbne .

as your last line...you are good :-)
************************************************************


*************************************************************
Running  (see next section for a step by step example)
*************************************************************

There are 3 example macros in the macros/ directory
nubeam-G4PBeam-stdnubeam.mac
nubeam-Fluka-stdnubeam.mac
nubeam-G4PBeam-Tracking.mac
Read the comments in each macro to understand what is in them.
Visualization commands are also listed in each macro.
The macro 
vis_nubeam-G4PBeam-stdnubeam.mac
is already setup for visualization.

nubeam-G4PBeam-stdnubeam.mac is for running the standard neutrino
beam simulation with a geant4 generated proton beam on the target.

nubeam-Fluka-stdnubeam.mac is for running the standard neutrino
beam simulation with a Fluka target hadron production ntuple as input.

nubeam-G4PBeam-Tracking.mac is a special tracking simulation. This is
not something a standard user would ever use.

The Geometry is configured from input files in the /input directory
to run g4lbne type for example

./g4lbne --input inputs/lbnedocdb2161v6.input macros/nubeam-G4PBeam-stdnubeam.mac


************************************************************


*************************************************************
Running and Plotting (an example)
*************************************************************

in g4lbne directory there is a directory called "example"
once you have compiled and in the g4lbne directory run the following

./g4lbne --input inputs/lbnedocdb2161v6.input example/nubeam-G4PBeam-stdnubeam_lbnedocdb2161v6_010.mac

examime the example/nubeam-G4PBeam-stdnubeam_lbnedocdb2161v6_010.mac macro...you will see that the Run ID is 10 and the output file name will be 
g4lbne_example_lbnedocdb2161v6_010.root
in the example directory

once that is finished run...

./g4lbne --input inputs/lbnedocdb2161v6.input example/nubeam-G4PBeam-stdnubeam_lbnedocdb2161v6_011.mac

Once that is finished...in the g4lbne/example directory you have 2 flux files
called 

g4lbne_example_lbnedocdb2161v6_010.root
g4lbne_example_lbnedocdb2161v6_011.root

Now plot the flux using the nudata.C macro
Look in nudata.h at the constructor nudata::nudata()
( look for #!!!!!!!!!!!! in that function )
 The file names are already set to the files you just created
but you can add more, if you make more, or change the names etc.
Also make sure the potperfile variable is the number of pot you
used to make 1 file (look in nubeam-G4PBeam-stdnubeam_lbnedocdb2161v6_010.mac 
you will see the line
/LBNE/run/NEvents 10000
hence since you are using just the Geant4 generated Proton beam you have used a total of 10000 protons


to make the plots open a root session
type the following...(amongst the output)

root [0] .x nudata.C
(const class nudata)30558496
root [1] nudata t
root [2] t.Loop()
Warning in <TClass::TClass>: no dictionary for class TrackPoint_t is available
Warning in <TClass::TClass>: no dictionary for class LBNEDataNtp_t is available
Total number of Entries = 6710
Info in <TCanvas::Print>: eps file g4lbne_fluxexample_le010z185i_NumiDPHelium_totalfluxatMINOSND.eps has been created
Info in <TCanvas::Print>: file g4lbne_fluxexample_le010z185i_NumiDPHelium_totalfluxatMINOSND.png has been created
Info in <TCanvas::Print>: eps file g4lbne_fluxexample_le010z185i_NumiDPHelium_numufluxatMINOSND.eps has been created
Info in <TCanvas::Print>: file g4lbne_fluxexample_le010z185i_NumiDPHelium_numufluxatMINOSND.png has been created
Info in <TCanvas::Print>: eps file g4lbne_fluxexample_le010z185i_NumiDPHelium_numubarfluxatMINOSND.eps has been created
Info in <TCanvas::Print>: file g4lbne_fluxexample_le010z185i_NumiDPHelium_numubarfluxatMINOSND.png has been created
Info in <TCanvas::Print>: eps file g4lbne_fluxexample_le010z185i_NumiDPHelium_nuefluxatMINOSND.eps has been created
Info in <TCanvas::Print>: file g4lbne_fluxexample_le010z185i_NumiDPHelium_nuefluxatMINOSND.png has been created
Info in <TCanvas::Print>: eps file g4lbne_fluxexample_le010z185i_NumiDPHelium_nuebarfluxatMINOSND.eps has been created
Info in <TCanvas::Print>: file g4lbne_fluxexample_le010z185i_NumiDPHelium_nuebarfluxatMINOSND.png has been created
Info in <TCanvas::Print>: eps file g4lbne_fluxexample_le010z185i_NumiDPHelium_totalfluxatSOMEDETECTOR.eps has been created
Info in <TCanvas::Print>: file g4lbne_fluxexample_le010z185i_NumiDPHelium_totalfluxatSOMEDETECTOR.png has been created
Info in <TCanvas::Print>: eps file g4lbne_fluxexample_le010z185i_NumiDPHelium_numufluxatSOMEDETECTOR.eps has been created
Info in <TCanvas::Print>: file g4lbne_fluxexample_le010z185i_NumiDPHelium_numufluxatSOMEDETECTOR.png has been created
Info in <TCanvas::Print>: eps file g4lbne_fluxexample_le010z185i_NumiDPHelium_numubarfluxatSOMEDETECTOR.eps has been created
Info in <TCanvas::Print>: file g4lbne_fluxexample_le010z185i_NumiDPHelium_numubarfluxatSOMEDETECTOR.png has been created
Info in <TCanvas::Print>: eps file g4lbne_fluxexample_le010z185i_NumiDPHelium_nuefluxatSOMEDETECTOR.eps has been created
Info in <TCanvas::Print>: file g4lbne_fluxexample_le010z185i_NumiDPHelium_nuefluxatSOMEDETECTOR.png has been created
Info in <TCanvas::Print>: eps file g4lbne_fluxexample_le010z185i_NumiDPHelium_nuebarfluxatSOMEDETECTOR.eps has been created
Info in <TCanvas::Print>: file g4lbne_fluxexample_le010z185i_NumiDPHelium_nuebarfluxatSOMEDETECTOR.png has been created
root [3] .q

After each print statment you will have plots of the Total Flux, NuMu Flux only, NuMuBar Flux only, NuE Flux only and NuEBar flux only at the MINOS Near detector and at "some detector".  All of the histograms will also be written to the file flux_histograms.root.  

The MINOS near detector plots are produced using the weights from the g4lbne ntuple.  The "some detector" plots use weights from the NuWeight() function in nudata.C, which reweights the MINOSNear detector flux to flux at some other (x,y,z) coordinate specified by the user.  The position of "some detector" is currently set to the position of the MINOS near detector, so the MINOSND and SOMEDETECTOR plots will be identical.  However, you may change the position of "some detector" to be anything you desire.  Each plot will be saved as an eps and a png in the g4lbne/example directory.  

More information on how the flux is calculated can be obtained by examining nudata.C.

************************************************************
