

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

     
    export SETUP_LOCATION=/nusoft/app/externals/

    #
    #Locate and Setup UPS
    #
    # Make sure this file exists.
    if [ -f ${SETUP_LOCATION}/setup ]; then
	source ${SETUP_LOCATION}/setup
    fi
    #
    #Now SETUP_UPS and UPS_DIR env vars should now be set
    #so setup ups now
    #
    setup ${SETUP_UPS}

    #
    #Set the versions of root, and geant4 you want to setup
    #If you want to print the avaliable versions define print_products variable
    print_products=1
    #
    export ROOT_VERSION=v5_30_06
    export ROOT_FLAVOR=Linux64bit+2.6-2.5
    export ROOT_QUALIFIERS=gcc46:prof

    export GEANT4_VERSION=v4_9_4_p03
    export GEANT4_FLAVOR=Linux64bit+2.6-2.5
    export GEANT4_QUALIFIERS=gcc46:prof

    #

    if [ -z "${print_products}" ]; then
	echo "PRODUCTS = ${PRODUCTS}"
    
	ups list -a root
	ups list -a geant4
	ups list -a clhep
    
    fi


    echo "Setting up ROOT"
    export SETUP_ROOT="root ${ROOT_VERSION} -f ${ROOT_FLAVOR} -z ${SETUP_LOCATION} -q ${ROOT_QUALIFIERS}"
    setup ${SETUP_ROOT}
    echo "setup ${SETUP_ROOT}"
    
    echo "Setting up GEANT4"
    export SETUP_GEANT4="geant4 ${GEANT4_VERSION} -f ${GEANT4_FLAVOR} -z ${SETUP_LOCATION} -q ${GEANT4_QUALIFIERS}"
    setup ${SETUP_GEANT4}
    echo "setup ${SETUP_GEANT4}"

    echo "Setting up G4Photon"
    export SETUP_G4PHOTON="g4photon v2_1"
    setup ${SETUP_G4PHOTON}
    echo "setup ${SETUP_G4PHOTON}"

##!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    export G4WORKDIR="SET YOUR WORKING DIRECTORY e.g /lbne/app/users/loiacono/lbne-beamsim/g4lbne_work"
##!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

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
    
    #
    # Set a flag to suppress unnecessary re-executions of this script.
    #    
    #export G4LBNE_IS_SETUP=1

fi