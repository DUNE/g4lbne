The Beam Simulation Group Wiki is here
https://cdcvs.fnal.gov/redmine/projects/lbne-beamsim/wiki
This contains much information about Fermilab accounts,
g4lbne and flux files.  

*************************************************************
Getting g4lbne
*************************************************************
See the wiki for instructions on checking out g4lbne


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
Geant4, ROOT and CLHEP ).  Do:

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

There are several example macros in the macros/ directory, such as

- nubeam-G4PBeam-stdnubeam.mac: for running the standard neutrino
beam simulation with a geant4 generated proton beam on the target.

- nubeam-Fluka-stdnubeam.mac: for running the standard neutrino
beam simulation with a Fluka target hadron production ntuple as input.

- nubeam-G4PBeam-Tracking.mac: is a special tracking simulation. This is
not something a standard user would ever use.

- vis_nubeam-G4PBeam-stdnubeam.mac: runs the standard neutrino beam 
simulation and writes out files that can be used for visualization of t
the geometry.

Read the comments in the macros for more details.

The beamline geometry is configured from input files in the /inputs directory.
Nominal_FHC.input is the current default geometry for neutrino mode running.
Nominal_RHC.input is the current default for antineutrino running.  Both
of these correspond to the CD1 CDR reference design and differ only in the
magnitude of the horn current.  le010z185i is the NuMI geometry.

*************************************************************
Running and Plotting (a simple example)
*************************************************************

in g4lbne directory there is a directory called "example"
once you have compiled and in the g4lbne directory run the following

./g4lbne --input inputs/Nominal_FHC.input example/nubeam-G4PBeam-stdnubeam_lbnedocdb2161v6_010.mac

If you examime the example/nubeam-G4PBeam-stdnubeam_010.mac macro...you 
will see that the Run ID is 10 and the output file name will be 
g4lbne_example_010.root
in the example directory

once that is finished running, try another one...

./g4lbne --input inputs/Nominal_FHC.input example/nubeam-G4PBeam-stdnubeam_011.mac

Once that is finished...in the g4lbne/example directory you have 2 flux files
called 

g4lbne_example_010.root
g4lbne_example_011.root

You can now plot the flux.  An example of how to do this is available in 
nudata.C, a ROOT macro.  The input filenames are set in the nudata::nudata() 
constructor in the file nudata.h.  These should already be set to the files
you just created, but you can add more, change the names, etc.  If you change
the number of POT generated, set in the g4lbne macro in a line like this:

/LBNE/run/NEvents 10000

you should also change the "potperfile" variable in nudata.h to reflect your 
POT choice.

To make plots with nudata, open a root session and type the following:
type the following...(amongst the output)

root [0] .L nudata.C++
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

The MINOS near detector plots are produced using the weights from the g4lbne ntuple.  The "some detector" plots use weights from the NuWeight() function in nudata.C, which reweights the default flux at some other (x,y,z) coordinate specified by the user.  The position of "some detector" is currently set to the position of the MINOS near detector, so the MINOSND and SOMEDETECTOR plots will be identical.  However, you may change the position of "some detector" to be anything you desire.  Each plot will be saved as an eps and a png in the g4lbne/example directory.  

More information on how the flux is calculated can be obtained by examining nudata.C.  Note than in addition to applying the detector position weights discussed above, the plots also use "importance weights" which are used to ensure that there are sufficient statistics in the high energy tails without exceedingly large file sizes.  If you modify nudata.C or create your own flux plotting macros, it is imperative that you use both the position weights and the importance weights when creating your plots.


*************************************************************
Running and Plotting (a more complex example involving grid 
                      running and oscillated event rates)
*************************************************************
Below is a description of the procedure for creating a set of plots
that compare flux and oscillated and unoscillated event rates for two
different geometry configurations using large statistics samples. You 
will need to have Fermilab grid credentials to do this (see the wiki).

1. Create a modified geometry

For this example, you'll want to make some sort of change to the nominal geometry.  For example, you could change the decay pipe fill material to Helium by opening inputs/Nominal_FHC.input, and changing the line

DecayPipeFillGEANTmat 15

to

DecayPipeFillGEANTmat 27

Save the fill as DPHelium_FHC.input.  Similarly, create a file called DPHelium_RHC.input that is the same as Nominal_RHC.input with the DecayPipeFillGEANTmat again changed to 27.

2. Generate nominal and varied flux ntuples

After obtaining credentials for running on the FermiGrid, you can use the script example/submit_flux.py to generate a large sample of flux MC in both the nominal and varied configurations.  The various inputs and options of submit_flux.py are available by executing 

python submit_flux.py --help

The following commands will submit 250 jobs each in neutrino and anti-neutrino mode using the nominal and varied input cards.  Each job will simulate 100,000 protons on target.  You can simulate more or less POT if you want, but it's important that you remember what POT/file you requested since it will be necessary later for normalizing plots.

python submit_flux.py -f 1 -l 250 -n 100000 --macro=nubeam-G4PBeam-stdnubeam  --input=Nominal_FHC
python submit_flux.py -f 1 -l 250 -n 100000 --macro=nubeam-G4PBeam-stdnubeam  --input=Nominal_RHC
python submit_flux.py -f 1 -l 250 -n 100000 --macro=nubeam-G4PBeam-stdnubeam  --input=DPHelium_FHC
python submit_flux.py -f 1 -l 250 -n 100000 --macro=nubeam-G4PBeam-stdnubeam  --input=DPHelium_RHC

Each job will take approximately two hours once it starts running on a grid machine.  The whole set of four samples will typically complete within a few hours.  You can see the status of your jobs by executing condor_q | grep <your_userid>.  When the jobs have completed, output files and logfiles should appear in your /lbne/data/users/<your_userid>/fluxfiles/ directory.  If they don't, check the logfiles in your $CONDOR_TMP area, which will hopefully hold clues about what went wrong.

2. Make flux and event rate histograms

To make flux and event rate histograms, you can use the Root macro example/makeFluxHistograms.C.  It compiles and runs code in eventRates.C that loops over the flux ntuples and plots the flux and event rates at a specified detector location.  It assumes that files are in the directory structure created by submit_flux.py and has input options to specify the g4lbne version, input card, detector location and geant4 physics list.  You can also specify the number of files you generated and the number of POT per file.  For example, to process the ntuples created above you would do:

root -q -b makeFluxHistograms.C\(\"G4PBeam\",\"v2r4p1\",\"Nominal_FHC\",\"LBNEFD\", \"QGSP_BERT\",\"250\",\"100000\"\);
root -q -b makeFluxHistograms.C\(\"G4PBeam\",\"v2r4p1\",\"Nominal_RHC\",\"LBNEFD\", \"QGSP_BERT\",\"250\",\"100000\"\);
root -q -b makeFluxHistograms.C\(\"G4PBeam\",\"v2r4p1\",\"DPHelium_FHC\",\"LBNEFD\", \"QGSP_BERT\",\"250\",\"100000\"\);
root -q -b makeFluxHistograms.C\(\"G4PBeam\",\"v2r4p1\",\"DPHelium_RHC\",\"LBNEFD\", \"QGSP_BERT\",\"250\",\"100000\"\);

Each 25,000,000 POT job takes around 10 minutes.  You may find it useful to put the commands above in a bash script and to execute it with 'nohup' so that the jobs will continue running even if you close your terminal window.

Each instance of makeFluxHistograms.C will create two histos_*.root files in the /lbne/data/users/<your_userid>/fluxfiles/ directory where your flux ntuples are stored -- one with flux/event rates histograms covering 0-20 GeV, intended for presentation plots, and one (with "_fastmc" in the file name) with fine binning meant serve as FastMC input.

2. Make comparison plots and tables

The root macro eventRateComparison.C can be used to make some simple plots comparing your two flux simulations.  You must specify whether you want to compare oscillated or unoscillated event rates, and the simulation, version, inputcard, location, physics list of the two simulations you want to compare.  For example:

root -q -b eventRateComparison.C\(\"unoscillated\",\"G4PBeam\",\"G4PBeam\",\"v2r4p1\",\"v2r4p1\",\"Nominal_FHC\",\"DecayPipeHelium_FHC\",\"LBNEFD\",\"LBNEFD\",\"QGSP_BERT\",\"QGSP_BERT\",\"Air\",\"Helium\"\)
root -q -b eventRateComparison.C\(\"unoscillated\",\"G4PBeam\",\"G4PBeam\",\"v2r4p1\",\"v2r4p1\",\"Nominal_RHC\",\"DecayPipeHelium_RHC\",\"LBNEFD\",\"LBNEFD\",\"QGSP_BERT\",\"QGSP_BERT\",\"Air\",\"Helium\"\)

will make plots comparing unoscillated neutrino and anti-neutrino mode flux and event rates while:

root -q -b eventRateComparison.C\(\"oscillated\",\"G4PBeam\",\"G4PBeam\",\"v2r4p1\",\"v2r4p1\",\"Nominal_FHC\",\"DecayPipeHelium_FHC\",\"LBNEFD\",\"LBNEFD\",\"QGSP_BERT\",\"QGSP_BERT\",\"Air\",\"Helium\"\)
root -q -b eventRateComparison.C\(\"oscillated\",\"G4PBeam\",\"G4PBeam\",\"v2r4p1\",\"v2r4p1\",\"Nominal_RHC\",\"DecayPipeHelium_FHC\",\"LBNEFD\",\"LBNEFD\",\"QGSP_BERT\",\"QGSP_BERT\",\"Air\",\"Helium\"\)

will similarly compare oscillated fluxes and event rates.  The last two input strings are meant to describe the two samples and will be used in the plot legends and file names to identify what is being compared.
  
Each time you run eventRateComparison.C, a set of image and text files will be written out to a folder called eventRateComparisons in your working directory.
