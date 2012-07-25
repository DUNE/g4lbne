# --------------------------------------------------------------
# GNUmakefile for physics list user.  
# JPW. Fri Jul 25 10:39:58 CEST 2003
# --------------------------------------------------------------

name := g4lbne

G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../..
endif

CPPFLAGS += -g -fPIC -Wno-deprecated

include $(G4INSTALL)/config/architecture.gmk

#
# define G4LISTS_BASE, if you have your own physics lists area installed
# point G4LISTS_BASE to the directory, that contains the subdirectory 'lists'.
#
ifndef G4LISTS_BASE
  EXTRALIBS += -L$(G4LIB)/.lists_build/$(G4SYSTEM)
  G4LISTS_BASE = $(G4INSTALL)/hadronic_lists
  EXTRALIBS += -L$(G4LIB)/plists/$(G4SYSTEM)	
else
  EXTRALIBS += -L$(G4LISTS_BASE)/hadronic/plists/lib/$(G4SYSTEM)
  EXTRALIBS += -L$(G4LIB)/plists/$(G4SYSTEM)
endif

# Assume if G4LISTS_BASE exists that we need to explicitly
# include these libraries (Geant4.8).  If it doesn't, then
# we do not (Geant 4.9)
LISTINC:=$(shell if [ -d G4LISTS_BASE ]; then echo "yes"; fi)
ifeq ($(LISTINC),yes)
echo $G4LISTS_BASE
	EXTRALIBS += -lQGSP
	EXTRALIBS += -lPackaging
	CPPFLAGS += -I$(G4LISTS_BASE)/hadronic/QGSP/include
	CPPFLAGS += -I$(G4LISTS_BASE)/hadronic/Packaging/include
endif


#for root
#CPPFLAGS   +=  -I$(ROOTSYS)/include
CPPFLAGS += $(shell $(ROOTSYS)/bin/root-config --cflags)

ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --glibs) -lMinuit -lHtml
ROOTLIBS      := $(filter-out -lNew,$(ROOTLIBS))
#ROOTLIBS      := $(filter-out -lThread,$(ROOTLIBS))
ROOTLIBS      := $(filter-out -lpthread,$(ROOTLIBS))
INTYLIBS      += $(ROOTLIBS)

#for debuging


CPPFLAGS += -I$(G4INCLUDE)


.PHONY: all
all: g4lbneCint lib  libg4lbneDict bin 
	cp $(G4WORKDIR)/bin/Linux-g++/$(G4TARGET) .

include $(G4INSTALL)/config/binmake.gmk

CXXFLAGS_WITHOUT_O := $(filter-out -O% , $(CXXFLAGS))
CXXFLAGS_WITHOUT_O := $(filter-out +O% , $(CXXFLAGS_WITHOUT_O))

g4lbneCint: include/TrackPoint_t.hh include/LBNEDataNtp_t.hh Linkdef.h
	rootcint -f ./src/g4lbneCint.cc -c -I./include ../include/TrackPoint_t.hh ../include/LBNEDataNtp_t.hh ../Linkdef.h

libg4lbneDict:  $(G4WORKDIR)/tmp/$(G4SYSTEM)/g4lbne/TrackPoint_t.o $(G4WORKDIR)/tmp/$(G4SYSTEM)/g4lbne/LBNEDataNtp_t.o $(G4WORKDIR)/tmp/$(G4SYSTEM)/g4lbne/g4lbneCint.o
	gcc -g -fPIC -shared -o libg4lbneDict.so    $(G4WORKDIR)/tmp/$(G4SYSTEM)/g4lbne/TrackPoint_t.o $(G4WORKDIR)/tmp/$(G4SYSTEM)/g4lbne/LBNEDataNtp_t.o $(G4WORKDIR)/tmp/$(G4SYSTEM)/g4lbne/g4lbneCint.o
