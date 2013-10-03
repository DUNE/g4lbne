
##################################################
##
## submit_flux.py
##
## Laura Fields
## Aug 2012
##
## Submits g4lbne flux jobs using macro, input, number of
## protons, number of jobs and output directory
## specified by user
##
###################################################

from optparse import OptionParser
import sys, os, subprocess, shutil

###################################################
#
# Determine default g4lbne directory
# (the directory this script is in)
#
###################################################
scriptdir = os.path.abspath(sys.argv[0]+"/../..")
print "default g4lbne directory:",scriptdir

###################################################
#
# Setup parser that reads in options
#
###################################################

usage = "usage: %prog [options]"
parser = OptionParser(usage=usage)

parser.add_option("-g", "--g4lbne_dir", dest="g4lbne_dir",
                  help="g4lbne directory", default=scriptdir)
parser.add_option("-p", "--physics_list", dest="physics_list",
                  help="Geant4 Physics List", default="QGSP_BERT");
parser.add_option("-m", "--macro", dest="macro",
                  help="Template Macro", default="Nominal")
parser.add_option("-c", "--horn_current", dest="horn_current",
                  help="Horn Current in kA", default=200)
parser.add_option("-n", "--n_events", dest="n_events",
                  help="Number of events per job", default=500000)
parser.add_option("-f", "--first_job", dest="first_job",
                  help="First job number", default="1")
parser.add_option("-l", "--last_job", dest="last_job",
                  help="Last job number", default="1")
parser.add_option("-o", "--output_dir", dest="output_dir",
                  help="Output directory", default="/lbne/data/users/$USER/fluxfiles/g4lbne")
parser.add_option("-v", "--interactive", dest="interactive",action="store_true",
                  help="Run interactively", default=False)

(options, args) = parser.parse_args()


if options.g4lbne_dir.find("$USER")>=0:
    options.g4lbne_dir = options.g4lbne_dir.replace("$USER",os.getenv("USER"));
if options.output_dir.find("$USER")>=0:
    options.output_dir = options.output_dir.replace("$USER",os.getenv("USER"));

###################################################
#
# Check validity of specified options
#
###################################################
g4lbne_executable=options.g4lbne_dir+"/g4lbne";
if not os.path.exists(g4lbne_executable):
    print "I can't find a g4lbne executable at "+g4lbne_executable
    sys.exit()

g4lbne_setup=options.g4lbne_dir+"/setups/setup_g4lbne_fnal.sh"
if not os.path.exists(g4lbne_setup):
    print "I can't find a g4lbne setup script at "+g4lbne_setup
    sys.exit()
    
macro_template = options.g4lbne_dir+"/macros/"+options.macro+".mac"
if not os.path.exists(macro_template):
    print "I can't find a macro at "+macro_template
    sys.exit()

#input_card = options.g4lbne_dir+"/inputs/"+options.input+".input"
#if not os.path.exists(macro_template):
#    print "I can't find an input card at at "+input_card
#    sys.exit()

physics_list = options.physics_list
print "Using GEANT4 physics list",physics_list

###################################################
#
# Determine G4LBNE Version
#
###################################################
#f = open(options.g4lbne_dir+"/CVS/Tag")
#lines = f.readlines();
#f.close()

#version = lines[0].strip()[1:]
version="v3r0p1"

###################################################
#
# Print options
#
###################################################
if options.interactive: 
    print "Running",str(int(options.last_job)-int(options.first_job)+1),"jobs starting with JobID",options.first_job,"."
    print "Submitting",str(int(options.last_job)-int(options.first_job)+1),"jobs starting with JobID",options.first_job,"."
print "Using the g4lbne installed at",options.g4lbne_dir
print "Each job will have",options.n_events,"protons on target."
print "Using macro template:",macro_template
#print "Using input card:",input_card
print "Output will be written to:",options.output_dir

###################################################
#
# Create Directories
#
###################################################
macro_dir = options.output_dir+"/"+version+"/"+physics_list+"/"+options.macro+"/"+str(options.horn_current)+"kA/macros/"
flux_dir = options.output_dir+"/"+version+"/"+physics_list+"/"+options.macro+"/"+str(options.horn_current)+"kA/flux/"
log_dir = options.output_dir+"/"+version+"/"+physics_list+"/"+options.macro+"/"+str(options.horn_current)+"kA/logfiles/"

# executables have to go on /app
macro_dir = macro_dir.replace("lbne/data","lbne/app")

print "Creating",macro_dir
if not os.path.exists(macro_dir):
    os.makedirs(macro_dir)
print "Creating",flux_dir
if not os.path.exists(flux_dir):
    os.makedirs(flux_dir)
print "Creating",log_dir
if not os.path.exists(log_dir):
    os.makedirs(log_dir)

os.chmod(macro_dir,0777)
os.chmod(flux_dir,0777)
os.chmod(log_dir,0777)
    
temp_dir = " "
if not options.interactive:
    temp_dir = "$_CONDOR_SCRATCH_DIR/"

###################################################
#
# Loop over requested jobs
#
###################################################
file_prefix = "g4lbne_"+version+"_"+physics_list+"_"+options.macro+"_"+str(options.horn_current)+"kA"
for i in range(int(options.first_job),int(options.last_job)+1):

    # Write macro file
    oldmac = open(macro_template)    
    new_macro_filename = macro_dir+os.path.basename(macro_template).replace(".mac","_"+str(i).zfill(3)+".mac")
    newmac = open(new_macro_filename, 'w')
    horn_current_set = False
    for s in oldmac.xreadlines():
        if s.find("/LBNE/output/OutputNtpFileName")>=0:
            newmac.write("/LBNE/output/OutputNtpFileName "+temp_dir+file_prefix+"\n")
        elif s.find("/LBNE/rndm/setRndmSeed")>=0:
            newmac.write("/LBNE/rndm/setRndmSeed "+str(i)+"\n")
        elif s.find("/LBNE/run/setRunID")>=0:
            newmac.write("/LBNE/run/setRunID "+str(i)+"\n")
        elif s.find("/LBNE/run/NEvents")>=0:
            newmac.write("/LBNE/run/NEvents "+str(options.n_events)+"\n")
        elif s.find("/LBNE/det/seHornCurrent")>=0:
            newmac.write("/LBNE/det/seHornCurrent "+str(options.horn_current)+" kA \n")
            horn_current_set = True
        else:
            newmac.write(s);

    if not horn_current_set:
        print "ERROR: The macro "+macro_template+" does set the horn current; please add a line such as /LBNE/det/seHornCurrent 200 kA to your macro template"
        sys.exit()

    # Write the script that will run on the grid
    script_filename = macro_dir+os.path.basename(macro_template).replace(".mac","_"+str(i).zfill(3)+".sh")
    wrapper = open(script_filename, 'w')
    #wrapper.write("#!/bin/sh \n\n")
    wrapper.write("source "+g4lbne_setup+" \n\n")
    #wrapper.write(g4lbne_executable+" --physicslist "+physics_list+" --input "+input_card+" "+new_macro_filename+'\n\n')
    wrapper.write(g4lbne_executable+" --physicslist "+physics_list+" "+new_macro_filename+'\n\n')
        # copy the output
    temp_output = temp_dir+file_prefix+"_"+str(i).zfill(3)+"v3xxx.root"
    final_output = flux_dir+file_prefix+"_"+str(i).zfill(3)+".root"
    if options.interactive:
        wrapper.write("cp "+temp_output+" "+final_output+"\n");
    else:
        wrapper.write("/grid/fermiapp/minos/scripts/cpn "+temp_output+" "+final_output+"\n");

    wrapper.close();
    
    os.chmod(script_filename,0777);



    # remove old files to prevent permissions conflicts
    logfile = log_dir + os.path.basename(final_output).replace(".root",".log");
    if os.path.exists(logfile):
        os.remove(logfile)
    if os.path.exists(final_output):
        os.remove(final_output);

    # Submit the job
    
    if options.interactive:
        print "type "+script_filename+" to execute"
    else:
        os.system("jobsub -g -L "+logfile+" "+script_filename);

    print "DONE"
    
