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
    #sm -r R2.3 -O
    
    
    echo "PRODUCTS = ${PRODUCTS}"
    
    ups list -a root
    ups list -a geant4
    ups list -a clhep
    
    
    echo "Setting up GEANT4"
    setup -q GCC_3_4_3-OpenGL -f Linux+2.4 geant4 v4_9_2_p03
    #need v4_9_2_p03 for patch
    echo "Setting up CLHEP"
    setup clhep v2_0_4_6
    export G4WORKDIR="/minerva/app/users/loiacono/LBNE/temp/g4lbne_work"
    echo "Setting up G4Photon"
    setup g4photon
    export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:$G4LIB/plists/Linux-g++"

    
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

setup_grid()
{
    source /grid/fermiapp/minerva/condor-scripts/setup_minerva_condor.sh
}


setup_mnv()
{

    export MINERVA_SETUP_DIR="/grid/fermiapp/minerva/software_releases/v8r2p1_DEV"
    #source /afs/fnal.gov/ups/etc/setups.sh
        #source $MINERVA_SETUP_DIR/setup/setupSw.sh
    source $MINERVA_SETUP_DIR/setup/setup.minerva.condor.sh
    source $MINERVA_SETUP_DIR/setup.sh
    #source /grid/fermiapp/minerva/condor-scripts/setup_minerva_condor.sh
    export CVSROOT="minervacvs@cdcvs.fnal.gov:/cvs/mnvsoft"

    ups list -a root
    ups list -a geant4
    ups list -a clhep

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

}
