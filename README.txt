
G4LBNE Documentation

*************************************************************
Getting g4lbne
*************************************************************
get g4lbne from /cvs/projects/lbne-beamsim repository

in read mode:
cvs -d :pserver:anonymous@cdcvs.fnal.gov:/cvs/projects/lbne-beamsim checkout g4lbne

in read/write mode:
cvs -d :ext:p-lbne-beamsim@cdcvs.fnal.gov:/cvs/projects/lbne-beamsim checkout g4lbne

************************************************************


*************************************************************
Setting up products and work directory
*************************************************************
You need to setup Geant4 (and CLHEP) and ROOT products.
You will need to determine what products you want to use.

on the Fermilab Linux computer flxi05.fnal.gov something like this might work:
setup geant4 v4_8_1_p02 -q GCC_3_4_3 -f Linux+2.4-2.3.2
setup root v5_10_00a -q GCC_3_4_3 -f Linux+2.4-2.3.2
setup g4photon
but these are not the most recent versions of Geant4 and ROOT.

You need to define a g4lbne work directory, for example
export G4WORKDIR="path to work directory/g4lbne_work"
this is where the binaries will be stored when compiling
and set the LD_LIBRARY_PATH as follows
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:$G4LIB/plists/Linux-g++"

************************************************************

*************************************************************
Compiling
*************************************************************

in the g4lbne directory,
for the first time type
make clean
then 
make

if there is an error message upon linking that says something about 
not being able to find a lib.so file just type make again.

************************************************************


*************************************************************
Running
*************************************************************

There are 3 example macros 
nubeam-G4PBeam-stdnubeam.mac
nubeam-Fluka-stdnubeam.mac
nubeam-G4PBeam-Tracking.mac
Read the comments in each macro to understand what is in them.
Visualization commands are also listed in each macro.

nubeam-G4PBeam-stdnubeam.mac is for running the standard neutrino
beam simulation with a geant4 generated proton beam on the target.

nubeam-Fluka-stdnubeam.mac is for running the standard neutrino
beam simulation with a Fluka target hadron production ntuple as input.

nubeam-G4PBeam-Tracking.mac is a special tracking simulation. This is
not something a standard user would ever use.

The Geometry is configured from input files in the /input directory
to run g4lbne type for example

./g4lbne --input inputs/lbnedocdb2161v6.input nubeam-G4PBeam-stdnubeam.mac


************************************************************
