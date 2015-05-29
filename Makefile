# $Id: Makefile,v 1.12 2009/04/30 00:55:15 rcook Exp $ 
# $File$
# paraDIS makefile
#
# Makefile for paraDIS library
#
export DEBUG ?= -g
export INSTALL_DIR ?= $(HOME)/$(SYS_TYPE)

ifndef SYS_TYPE
export SYS_TYPE = $(shell uname)
endif
CWD = $(shell pwd)
MF-INCLUDE = $(shell cd ..; pwd)/mf-include
include $(MF-INCLUDE)/mf-include.common

SFILES = paradis.C paradis_c_interface.C paradisStreaming.C paradisTest.C analyzeParaDIS.C BurgersTypes.C

CPPLIBDIR=$(projdir)/RC_cpp_lib
CLIBDIR=$(CPPLIBDIR)/RC_c_lib
TCLAP=$(CPPLIBDIR)/tclap-1.2.0/include
INCLUDES =  -I$(CLIBDIR) -I$(CPPLIBDIR) -I$(TCLAP) -I.
LDFLAGS+=-lstdc++ -lboost_filesystem
#ELECTRIC_FENCE = libefence.a -lpthread
#CXX = purify g++-3.2.3
#CC = purify gcc-3.2.3

#USER_CFLAGS= -g
USER_CXXFLAGS+=$(INCLUDES) -DNO_SETBUF 

PARADISLIB = $(CWD)/$(SYS_TYPE)/lib/libparadis.a
RCCLIB=$(INSTALL_DIR)/lib/librcc.a 
RCCPPLIB=$(INSTALL_DIR)/lib/librccpp.a 
TARGETS = $(CWD)/$(SYS_TYPE)/bin/analyzeParaDIS $(CWD)/$(SYS_TYPE)/bin/paradisTest $(CWD)/$(SYS_TYPE)/bin/paradisStreamingTest 
SOURCES = analyzeParaDIS.C  paradis.h paradisStreaming.h paradis.C paradisTest.C paradis_c_interface.h paradisStreaming.C  paradisStreamingTest.C  paradis_c_interface.C  paradis_types.h   BurgersTypes.C

all: timestamp $(TARGETS)

debug:
	DEBUG=-g $(MAKE) all

profile:  
	DEBUG="-g -pg" $(MAKE) all

timestamp: $(SOURCES)
	setversion.sh --timestamp
	date > timestamp

install: all
	mkdir -p $(INSTALL_DIR)/bin && cp $(TARGETS) $(INSTALL_DIR)/bin

install-version: install
	cd $(INSTALL_DIR)/bin; for tool in paradisTest paradisStreamingTest analyzeParaDIS; do cp $$tool $${tool}-$$(analyzeParaDIS --version-num); done

global: uninstall all
	INSTALL_DIR=/usr/global/tools/IMG_private/paraDIS/$$SYS_TYPE $(MAKE) install-version;

install-debug: 
	DEBUG=-g $(MAKE) install


libparadis: $(PARADISLIB) 

$(SYS_TYPE)/%.o: %.C
	@echo; echo '***************************************'; echo 
	echo building $@ due to $?
	$(CXX)  $(CXXFLAGS) -c -o $@ -DUSE_ABORT=1 $<
	@echo DONE WITH $@
	@echo; echo '***************************************'; echo 

$(SYS_TYPE)/xxparadis.o: paradis.C
	@echo; echo '**************XXXXXXXXXX*************************'; echo 
	$(CXX)  $(CXXFLAGS) -c -o $@ -DUSE_ABORT=1 $<
	@echo DONE WITH $@
	@echo; echo '***************************************'; echo 

$(SYS_TYPE)/xxparadisStreaming.o: paradisStreaming.C
	@echo; echo '**************XXXXXXXXXX**************************'; echo 
	$(CXX)  $(CXXFLAGS) -c -o $@  $<
	@echo DONE WITH $@
	@echo; echo '***************************************'; echo 

$(SYS_TYPE)/xxparadis_c_interface.o: paradis_c_interface.C
	@echo; echo '**************XXXXXXXXXX**************************'; echo 
	$(CXX)  $(CXXFLAGS) -c -o $@  $<
	@echo DONE WITH $@
	@echo; echo '***************************************'; echo 


$(PARADISLIB): $(RCCLIB) $(RCCPPLIB) $(SYS_TYPE)/paradis_c_interface.o  $(SYS_TYPE)/paradis.o $(SYS_TYPE)/paradisStreaming.o  $(SYS_TYPE)/BurgersTypes.o 
	[ -d $(SYS_TYPE)/lib/rclibobjs ] ||  mkdir -p $(SYS_TYPE)/lib/rclibobjs
	cd $(SYS_TYPE)/lib/rclibobjs && ar -x $(RCCPPLIB) &&  ar -x $(RCCLIB)
#	ar -rc $@ $(SYS_TYPE)/paradis_c_interface.o  $(SYS_TYPE)/paradis.o $(RCCLIB) $(RCCPPLIB)
	ar -rc $@ $(SYS_TYPE)/paradisStreaming.o $(SYS_TYPE)/paradis_c_interface.o  $(SYS_TYPE)/paradis.o  $(SYS_TYPE)/BurgersTypes.o $(SYS_TYPE)/lib/rclibobjs/*.o
	@echo DONE WITH $@

$(CWD)/$(SYS_TYPE)/bin/paradisTest: $(SYS_TYPE)/paradisTest.o $(PARADISLIB) 
	[ -d $(SYS_TYPE)/bin ] || mkdir -p $(SYS_TYPE)/bin
	$(LD) -o $@ $(LDFLAGS)  $^ $(ELECTRIC_FENCE)

$(CWD)/$(SYS_TYPE)/bin/analyzeParaDIS: $(SYS_TYPE)/analyzeParaDIS.o $(PARADISLIB)  $(RCCLIB)
	[ -d $(SYS_TYPE)/bin ] || mkdir -p $(SYS_TYPE)/bin
	@echo; echo '***************************************'; echo 
	$(LD) -o $@ $(LDFLAGS)  $^ $(ELECTRIC_FENCE) $(DEBUG)
	@echo; echo '***************************************'; echo 


$(CWD)/$(SYS_TYPE)/bin/paradisStreamingTest: $(SYS_TYPE)/paradisStreamingTest.o $(PARADISLIB)  $(RCCLIB)
	[ -d $(SYS_TYPE)/bin ] || mkdir -p $(SYS_TYPE)/bin
	@echo; echo '***************************************'; echo 
	$(LD) -o $@ $(LDFLAGS)  $^ $(ELECTRIC_FENCE)
	@echo; echo '***************************************'; echo 

.DUMMY: 

$(RCCLIB): .DUMMY
	cd $(CLIBDIR);  $(MAKE) install

$(RCCPPLIB): .DUMMY
	cd $(CPPLIBDIR);  $(MAKE) install

uninstall: 
	rm -f $(INSTALL_DIR)/bin/{analyzeParaDIS,paradisTest,paradisStreamingTest}

clean: undepend
	rm -rf $(PROGNAME) makeworms *.o $(SYS_TYPE)/* *~ $(SYS_TYPE)/lib/*o $(SYS_TYPE)/lib/*a 

realclean: clean
	cd $(CPPLIBDIR);  $(MAKE) undepend; $(MAKE) clean;
	cd $(CLIBDIR);  $(MAKE) undepend; $(MAKE) clean;

undepend: 
	makedepend -Y $(SFILES)

depend: 	
	makedepend -Y -p $(SYS_TYPE)/ $(SFILES)
	cd $(CLIBDIR);  $(MAKE) depend
	cd $(CPPLIBDIR);  $(MAKE) depend	

# DO NOT DELETE

chaos_5_x86_64_ib/paradis.o: paradis.h BurgersTypes.h paradis_version.h
chaos_5_x86_64_ib/paradis_c_interface.o: paradis.h BurgersTypes.h
chaos_5_x86_64_ib/paradis_c_interface.o: paradis_c_interface.h
chaos_5_x86_64_ib/paradisStreaming.o: paradisStreaming.h paradis.h
chaos_5_x86_64_ib/paradisStreaming.o: BurgersTypes.h
chaos_5_x86_64_ib/paradisTest.o: paradis.h BurgersTypes.h
chaos_5_x86_64_ib/analyzeParaDIS.o: paradis_c_interface.h paradis.h
chaos_5_x86_64_ib/analyzeParaDIS.o: BurgersTypes.h
chaos_5_x86_64_ib/BurgersTypes.o: BurgersTypes.h
