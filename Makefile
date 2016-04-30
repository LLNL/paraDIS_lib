# $Id$
# $File$
# paraDIS makefile
#
# Makefile for paraDIS library
#
include mf-include.common
export DEBUG ?= -g
export INSTALL_DIR ?= $(HOME)/$(SYS_TYPE)

ifndef SYS_TYPE
export SYS_TYPE = $(shell uname)
endif
CWD = $(shell pwd)


BOOSTDIR ?= ./boost
TCLAPDIR ?= ./tclap
#CPPLIBDIR ?= ./RC_cpp_lib
#CLIBDIR = $(CPPLIBDIR)/RC_c_lib

INCLUDES =  -I$(CLIBDIR) -I$(CPPLIBDIR) -I$(BOOSTDIR)/include -I$(TCLAPDIR)/include -I.
LDFLAGS+=-lstdc++ -L$(INSTALL_DIR)/lib -L$(BOOSTDIR)/lib -lboost_filesystem -lboost_system
#ELECTRIC_FENCE = libefence.a -lpthread
#CXX = purify g++-3.2.3
#CC = purify gcc-3.2.3

#USER_CFLAGS= -g
USER_CXXFLAGS+=$(INCLUDES) -DNO_SETBUF -Wno-unused-local-typedefs
CXXFLAGS += -I$(INSTALL_DIR)/include -DBOOST_SYSTEM_NO_DEPRECATED
CFLAGS += -I$(INSTALL_DIR)/include

PARADISLIB = $(SYS_TYPE)/libparadis.a
TARGETS = $(SYS_TYPE)/bin/analyzeParaDIS $(SYS_TYPE)/bin/paradisTest $(SYS_TYPE)/bin/paradisStreamingTest
SOURCES = analyzeParaDIS.C  paradis.h paradisStreaming.h paradis.C paradisTest.C paradis_c_interface.h paradisStreaming.C  paradisStreamingTest.C  paradis_c_interface.C  paradis_types.h   BurgersTypes.C RCDebugStream.C RCDebugStream.h debugutil.c debugutil.h timer.C timer.h
SFILES = paradis.C paradis_c_interface.C paradisStreaming.C paradisTest.C analyzeParaDIS.C BurgersTypes.C RCDebugStream.C debugutil.c timer.C
LIBOBJ = $(SYS_TYPE)/paradis_c_interface.o  $(SYS_TYPE)/paradis.o $(SYS_TYPE)/paradisStreaming.o  $(SYS_TYPE)/BurgersTypes.o $(SYS_TYPE)/RCDebugStream.o $(SYS_TYPE)/debugutil.o $(SYS_TYPE)/timer.o
all: timestamp $(TARGETS)

package:
	name=paradis-lib-v$$(./setversion.sh -p); \
		git archive master --prefix=$$name/ | gzip >$${name}.tgz; \
		echo created archive $${name}.tgz

debug:
	DEBUG=-g $(MAKE) all

profile:
	DEBUG="-g -pg" $(MAKE) all

timestamp: $(SOURCES)
	./setversion.sh --timestamp
	date > timestamp

install: all
	mkdir -p $(INSTALL_DIR)/bin && cp $(TARGETS) $(INSTALL_DIR)/bin/
	mkdir -p $(INSTALL_DIR)/lib && cp $(PARADISLIB) $(INSTALL_DIR)/lib/

install-version: install
	cd $(INSTALL_DIR)/bin; for tool in paradisTest paradisStreamingTest analyzeParaDIS; do cp $$tool $${tool}-$$(analyzeParaDIS --version-num); done

#global: uninstall all
#	INSTALL_DIR=/usr/global/tools/IMG_private/paraDIS/$$SYS_TYPE $(MAKE) install-version;
global:
	echo "global installation is no longer supported.  Please use the eris build farm to install into /usr/local/tools"

install-debug:
	DEBUG=-g $(MAKE) install


libparadis: $(PARADISLIB)

$(SYS_TYPE)/%.o: %.C
	mkdir -p $(SYS_TYPE)
	@echo; echo '***************************************'; echo
	echo building $@ due to $?
	$(CXX) -std=c++0x $(CXXFLAGS) -DUSE_ABORT=1 -c -o $@ $<
	@echo DONE WITH $@
	@echo; echo '***************************************'; echo


$(PARADISLIB): $(LIBOBJ)
	mkdir -p $(CWD)/$(SYS_TYPE)
	ar -rc $@ $(LIBOBJ)
	@echo DONE WITH $@

$(SYS_TYPE)/bin/paradisTest: $(SYS_TYPE)/paradisTest.o $(PARADISLIB)
	[ -d $(SYS_TYPE)/bin ] || mkdir -p $(SYS_TYPE)/bin
	$(LD) -o $@ $(LDFLAGS)  $^ $(ELECTRIC_FENCE)

$(SYS_TYPE)/bin/analyzeParaDIS: $(SYS_TYPE)/analyzeParaDIS.o $(PARADISLIB)
	[ -d $(SYS_TYPE)/bin ] || mkdir -p $(SYS_TYPE)/bin
	@echo; echo '***************************************'; echo
	$(LD) -o $@  $^ $(LDFLAGS) $(ELECTRIC_FENCE) $(DEBUG)
	@echo; echo '***************************************'; echo


$(SYS_TYPE)/bin/paradisStreamingTest: $(SYS_TYPE)/paradisStreamingTest.o $(PARADISLIB)
	[ -d $(SYS_TYPE)/bin ] || mkdir -p $(SYS_TYPE)/bin
	@echo; echo '***************************************'; echo
	$(LD) -o $@  $^ $(LDFLAGS) $(ELECTRIC_FENCE)
	@echo; echo '***************************************'; echo

.DUMMY:

uninstall:
	rm -f $(INSTALL_DIR)/bin/{analyzeParaDIS,paradisTest,paradisStreamingTest}

clean: undepend
	rm -rf $(PROGNAME) makeworms *.o $(SYS_TYPE)/* *~ $(SYS_TYPE)/lib/*o $(SYS_TYPE)/lib/*a

undepend:
	makedepend -Y $(SFILES)

depend:
	makedepend -Y -p $(SYS_TYPE)/ $(SFILES)

# DO NOT DELETE

paradis.o: paradis.h BurgersTypes.h paradis_version.h
paradis_c_interface.o: paradis.h BurgersTypes.h paradis_c_interface.h
paradisStreaming.o: paradisStreaming.h paradis.h BurgersTypes.h
paradisTest.o: paradis.h BurgersTypes.h
analyzeParaDIS.o: paradis_c_interface.h paradis.h BurgersTypes.h
BurgersTypes.o: BurgersTypes.h
