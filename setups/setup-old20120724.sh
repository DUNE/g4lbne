#!/bash/sh

sm()
{
   export MINOS_SETUP_DIR="/grid/fermiapp/minos/minossoft/setup"
   #source /afs/fnal.gov/ups/etc/setups.sh
   echo "Setting up UPS"
   source /usr/local/etc/setups.sh
   echo "Setting up MINOSSOFT"
   source ${MINOS_SETUP_DIR}/setup_minossoft_FNALU.sh $*
   export srt=$SRT_PUBLIC_CONTEXT

   echo "PRODUCTS = ${PRODUCTS}"

      
}


setup_g4lbne()
{
    sm -r R1.31 -O
    
    echo "PRODUCTS = ${PRODUCTS}"
    
    ups list -a root
    ups list -a geant4
    ups list -a clhep
    
    
    echo "Setting up GEANT4"
    setup -q GCC_3_4_3-OpenGL -f Linux+2.4 geant4 v4_9_2_p03
    #need v4_9_2_p03 for patch
    echo "Setting up CLHEP"
    setup clhep v2_0_4_6
##!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    export G4WORKDIR="SET YOUR WORKING DIRECTORY e.g /lbne/app/users/loiacono/lbne-beamsim/g4lbne_work"
##!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    echo "Setting up G4Photon"
    setup g4photon
    export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:$G4LIB/plists/Linux-g++"


    sm -r R1.31 -O

    


    echo "Setting up GEANT4"
    setup -q GCC_3_4_3-OpenGL -f Linux+2.4 geant4 v4_9_2_p03
    #need v4_9_2_p03 for patch
    echo "Setting up CLHEP"
    setup clhep v2_0_4_6
##!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    export G4WORKDIR="SET YOUR WORKING DIRECTORY e.g /lbne/app/users/loiacono/lbne-beamsim/g4lbne_work"
##!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    echo "Setting up G4Photon"
    setup g4photon
    export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:$G4LIB/plists/Linux-g++"


    export G4INSTALL="/afs/fnal.gov/ups/geant4source/v4_9_2_p03/NULL-g77/geant4.9.2.p03"

    
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
    
}

