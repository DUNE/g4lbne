

if [ -z "${G4LBNE_IS_SETUP}" ]; then
    
    case ${HOSTNAME} in
        argoneut*)
            export GROUP=argoneut
            export EXPERIMENT=argoneut
            ;;
        uboone*)
            export GROUP=microboone
            export EXPERIMENT=uboone
            ;;
        lbne*)
            export GROUP=lbne
            export EXPERIMENT=lbne
            ;;
    esac
    echo experiment is $GROUP


        
    SETUP_LOCATION=/grid/fermiapp/lbne/lar/code/larsoft/setup/

    # Make sure this is an executable script.
    if [ -x ${SETUP_LOCATION}/setup_larsoft_fnal_setup.sh ]; then 

        # Execute the script and save the result.  Note that the "-s"
        # option causes the result file's commands to be
        # sh-compatible.  Pass along any arguments to this script.
	result=`${SETUP_LOCATION}/setup_larsoft_fnal_setup.sh -s $@`

        # Make sure the result is a readable file.
        if [ -r ${result} ]; then
            # Execute the contents of this file in the current environment.
            source ${result}
        fi
    fi

    # Set a flag to suppress unnecessary re-executions of this script.
    export G4LBNE_IS_SETUP=1
fi


    echo "PRODUCTS = ${PRODUCTS}"
    
    ups list -a root
    ups list -a geant4
    ups list -a clhep
    
    
    echo "Setting up ROOT"
    setup ${SETUP_ROOT}
    echo "setup ${SETUP_ROOT}"
    
    echo "Setting up GEANT4"
    setup ${SETUP_GEANT4}
    echo "setup ${SETUP_GEANT4}"

##!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    export G4WORKDIR="SET YOUR WORKING DIRECTORY e.g /lbne/app/users/loiacono/lbne-beamsim/g4lbne_work"
##!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    echo "Setting up G4Photon"
    setup ${SETUP_G4PHOTON}
    echo "setup ${SETUP_G4PHOTON}"

    export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:$G4LIB/Linux-g++"




    echo "*********************"
    echo Locating ROOT...
    env | grep root
    echo "*********************"
    echo Locating Geant4...
    env | grep geant4
    echo "*********************"
    echo Locating CLHEP...
    env | grep clhep
    echo "*********************"
    
    echo G4WORKDIR is ${G4WORKDIR}
    echo LD_LIBRARY_PATH is ${LD_LIBRARY_PATH} 
    


