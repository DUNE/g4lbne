cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(g4lbne)

# Hook into Geant4
find_package(Geant4 REQUIRED)
include(${Geant4_USE_FILE})

# Hook into ROOT
find_package(ROOT REQUIRED)
#include(${ROOT_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

ROOT_GENERATE_DICTIONARY(g4lbnedict ${PROJECT_SOURCE_DIR}/include/TrackPoint_t.hh ${PROJECT_SOURCE_DIR}/include/LBNEDataNtp_t.hh  LINKDEF Linkdef.h OPTIONS -p)

add_executable(g4lbne g4lbne.cc ${sources} ${headers} g4lbnedict.cxx)
target_link_libraries(g4lbne ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} Tree)
