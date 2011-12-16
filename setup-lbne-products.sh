#!/bash/sh

setup_g4lbne()
{
    echo "Setting up UPS"
    source /grid/fermiapp/products/lbne/etc/setups.sh

    echo "PRODUCTS = ${PRODUCTS}"


    ups list -a root
    ups list -a geant4
    ups list -a clhep

    echo "Setting up ROOT"
    #setup root v5_10_00a -q GCC_3_4_3 -f Linux+2.4-2.3.2
    setup root v5_26_00 -q GCC_3_4_6 -f Linux+2.6
    echo "Setting up GEANT4"
    setup -q g77-OpenGL -f Linux+2.6 geant4 v4_9_2_p03
    echo "Setting up CLHEP"
    setup clhep v2_0_4_6
    export G4WORKDIR="/lbne/app/users/loiacono/lbne-beamsim/g4lbne/g4lbne_work"
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
}