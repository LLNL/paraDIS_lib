# $Id: Makefile,v 1.31 2007/06/30 00:12:13 rcook Exp $ 
# $File$
# paraDIS makefile
#
# Makefile for binary that uses libhts.a to create ball and stick images
#
ifndef SYS_TYPE
export SYS_TYPE = $(shell uname)
endif
MF-INCLUDE = $(shell pwd)/mf-include
include mf-include/mf-include.$(SYS_TYPE)

SFILES = restartConvert.C 

INCLUDES = -I../RC_c_lib -I../RC_cpp_lib
LDFLAGS+=-lstdc++


DEBUG ?= -O3

#USER_CFLAGS= -g
USER_CXXFLAGS+=$(INCLUDES) -DNO_SETBUF 

RCCLIB=../RC_c_lib/$(SYS_TYPE)/librcc.a 
RCCPPLIB=../RC_cpp_lib/$(SYS_TYPE)/librccpp.a 
TARGETS =   restartConvert
all: $(TARGETS)

debug:
	DEBUG=-g $(MAKE) all

install: all
	cp $(TARGETS) $(HOME)/$(SYS_TYPE)/bin


libparadis:  lib/$(SYS_TYPE)/libparadis.a
	@echo DONE WITH $@

$(SYS_TYPE)/libparadis.o: libparadis.C
	$(CXX)  $(CXXFLAGS) -c -o $@  $^
	@echo DONE WITH $@

lib/$(SYS_TYPE)/libparadis.a: $(RCCLIB) $(RCCPPLIB) $(SYS_TYPE)/libparadis.o
	[ -d lib/$(SYS_TYPE) ] || mkdir -p lib/$(SYS_TYPE)
	ar -rc $@ $+
	@echo DONE WITH $@


.DUMMY: 


$(RCCLIB): .DUMMY
	[ -d ../RC_c_lib ] || ( cd ../ && tar -xvzf RClibs.tgz)
	cd ../RC_c_lib &&  $(MAKE) depend && $(MAKE) install

$(RCCPPLIB): .DUMMY
	[ -d ../RC_cpp_lib ] || ( cd ../ && tar -xvzf RClibs.tgz)
	cd ../RC_cpp_lib &&  $(MAKE) depend && $(MAKE)


restartConvert: bin/$(SYS_TYPE)/restartConvert
	@echo $@ built as $^

bin/$(SYS_TYPE)/restartConvert: $(RCCLIB) $(RCCPPLIB) lib/$(SYS_TYPE)/libparadis.a $(SYS_TYPE)/restartConvert.o 
	[ -d bin/$(SYS_TYPE) ] || mkdir -p bin/$(SYS_TYPE)
	$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $(SYS_TYPE)/restartConvert.o lib/$(SYS_TYPE)/libparadis.a $(RCCLIB) $(RCCPPLIB) 

clean: undepend
	rm -rf $(PROGNAME)  *.o $(SYS_TYPE)/* *~ lib/$(SYS_TYPE)/*o lib/$(SYS_TYPE)/*a 

realclean: clean
	cd ../RC_cpp_lib;  $(MAKE) undepend; $(MAKE) clean;
	cd ../RC_c_lib;  $(MAKE) undepend; $(MAKE) clean;

# DO NOT DELETE
