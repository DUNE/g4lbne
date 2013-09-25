##################################################
##
## copy_fastmc_flux.py
##
## Laura Fields
## Aug 2013
##
## Copies FastMC flux histograms from user area
## to FMC area
##
###################################################

from optparse import OptionParser
import sys, os, subprocess, shutil


###################################################
#
# Setup parser that reads in options
#
###################################################

usage = "usage: %prog [options]"
parser = OptionParser(usage=usage)

parser.add_option("-d", "--fmc_dir", dest="fmc_dir",
                  help="Name of directory you want to create in $FMC_FLUX_FILES", default="")
parser.add_option("-p", "--physics_list", dest="physics_list",
                  help="Geant4 Physics List", default="QGSP_BERT");
parser.add_option("-v", "--g4lbne_version", dest="g4lbne_version",
                  help="G4LBNE version", default="v2r4p1");
parser.add_option("-i", "--input", dest="input",
                  help="Input Card", default="Nominal")
parser.add_option("-m", "--macro", dest="macro",
                  help="G4LBNE Macro", default="nubeam-G4PBeam-stdnubeam")
parser.add_option("-t", "--test", dest="test",action="store_true",
                  help="Test mode -- just print file locations but don't move anything", default=False)
parser.add_option("-f", "--fdloc", dest="fdloc",
                  help="Far Detector Location", default="LBNEFD");
parser.add_option("-n", "--ndloc", dest="ndloc",
                  help="Near Detector Location", default="LBNEND");

(options, args) = parser.parse_args()

if options.fmc_dir == "":
    print "\nPlease specify a $FMC_FLUX_FILES subdirectory name using --fmc_dir flag"
    print "Execute \"python copy_fastmc_flux.py --help\" for more info\n"
    sys.exit()

###################################################
#
# Find source files
#
###################################################
username = os.getenv("USER")

sources = []

sources.append("/lbne/data/users/"+username+"/fluxfiles/g4lbne/"+options.g4lbne_version+"/"+options.input+"_FHC/"+options.physics_list+"/"+options.macro+"/flux/histos_g4lbne_"+options.input+"_FHC_"+options.physics_list+"_"+options.fdloc+"_fastmc.root")
sources.append("/lbne/data/users/"+username+"/fluxfiles/g4lbne/"+options.g4lbne_version+"/"+options.input+"_RHC/"+options.physics_list+"/"+options.macro+"/flux/histos_g4lbne_"+options.input+"_RHC_"+options.physics_list+"_"+options.fdloc+"_fastmc.root")
sources.append("/lbne/data/users/"+username+"/fluxfiles/g4lbne/"+options.g4lbne_version+"/"+options.input+"_FHC/"+options.physics_list+"/"+options.macro+"/flux/histos_g4lbne_"+options.input+"_FHC_"+options.physics_list+"_"+options.ndloc+"_fastmc.root")
sources.append("/lbne/data/users/"+username+"/fluxfiles/g4lbne/"+options.g4lbne_version+"/"+options.input+"_RHC/"+options.physics_list+"/"+options.macro+"/flux/histos_g4lbne_"+options.input+"_RHC_"+options.physics_list+"_"+options.ndloc+"_fastmc.root")

###################################################
#
# Define destination
#
###################################################

dests = []

dests.append(os.getenv("FMC_FLUX_FILES")+"/"+options.fmc_dir+"/g4lbne_"+options.g4lbne_version+"_FHC_FD.root")
dests.append(os.getenv("FMC_FLUX_FILES")+"/"+options.fmc_dir+"/g4lbne_"+options.g4lbne_version+"_RHC_FD.root")
dests.append(os.getenv("FMC_FLUX_FILES")+"/"+options.fmc_dir+"/g4lbne_"+options.g4lbne_version+"_FHC_ND.root")
dests.append(os.getenv("FMC_FLUX_FILES")+"/"+options.fmc_dir+"/g4lbne_"+options.g4lbne_version+"_RHC_ND.root")

for i in range(0,4):
    if os.path.exists(sources[i]):
        print "Copying "+sources[i]+" to "+dests[i]
    else:
        print "ERROR: Could not find "+sources[i]

if options.test:
    print "You requested test mode, so I'm not actually going to copy anything."
    sys.exit()

###################################################
#
# Create Directories
#
###################################################
if not os.path.exists(os.path.dirname(dests[0])):
    os.makedirs(os.path.dirname(dests[0]))
    os.chmod(os.path.dirname(dests[0]),0777)

###################################################
#
# Copy files
#
###################################################
for i in range(0,4):
    if os.path.exists(sources[i]):
        shutil.copyfile(sources[i],dests[i])
        os.chmod(dests[i],0777)

###################################################
#
# Create RIK files
#
###################################################
os.system("root -b -q $FMC_GENIE/addfineflux.C\\(\\\"$FMC_FLUX_FILES/"+options.fmc_dir+"\\\"\\)")
