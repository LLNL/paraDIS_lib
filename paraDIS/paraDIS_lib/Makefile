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
MF-INCLUDE ?= $(CWD)/mf-include
include $(MF-INCLUDE)/mf-include.common

SFILES = paradis.C paradis_c_interface.C paradisStreaming.C paradisTest.C analyzeParaDIS.C


INCLUDES =  -I$(CWD)/RC_c_lib -I$(CWD)/RC_cpp_lib
LDFLAGS+=-lstdc++
#ELECTRIC_FENCE = libefence.a -lpthread
#CXX = purify g++-3.2.3
#CC = purify gcc-3.2.3

#USER_CFLAGS= -g
USER_CXXFLAGS+=$(INCLUDES) -DNO_SETBUF 

PARADISLIB = $(CWD)/$(SYS_TYPE)/lib/libparadis.a
RCCLIB=$(INSTALL_DIR)/lib/librcc.a 
RCCPPLIB=$(INSTALL_DIR)/lib/librccpp.a 
TARGETS = $(CWD)/$(SYS_TYPE)/bin/paradisTest $(CWD)/$(SYS_TYPE)/bin/paradisStreamingTest $(CWD)/$(SYS_TYPE)/bin/analyzeParaDIS

all: $(TARGETS)

debug:
	DEBUG=-g $(MAKE) all

install: all
	mkdir -p $(INSTALL_DIR)/bin && cp $(TARGETS) $(INSTALL_DIR)/bin

install-debug: 
	DEBUG=-g $(MAKE) install

global: all
	INSTALL_DIR=/usr/global/tools/IMG_private/paraDIS/$$SYS_TYPE $(MAKE) install

libparadis: $(PARADISLIB) 

$(SYS_TYPE)/paradis.o: paradis.C
	$(CXX)  $(CXXFLAGS) -c -o $@  $<
	@echo DONE WITH $@

$(SYS_TYPE)/paradisStreaming.o: paradisStreaming.C
	$(CXX)  $(CXXFLAGS) -c -o $@  $<
	@echo DONE WITH $@

$(SYS_TYPE)/paradis_c_interface.o: paradis_c_interface.C
	$(CXX)  $(CXXFLAGS) -c -o $@  $<
	@echo DONE WITH $@


$(PARADISLIB): $(RCCLIB) $(RCCPPLIB) $(SYS_TYPE)/paradis_c_interface.o  $(SYS_TYPE)/paradis.o $(SYS_TYPE)/paradisStreaming.o 
	[ -d $(SYS_TYPE)/lib/rclibobjs ] ||  mkdir -p $(SYS_TYPE)/lib/rclibobjs
	cd $(SYS_TYPE)/lib/rclibobjs && ar -x $(RCCPPLIB) &&  ar -x $(RCCLIB)
#	ar -rc $@ $(SYS_TYPE)/paradis_c_interface.o  $(SYS_TYPE)/paradis.o $(RCCLIB) $(RCCPPLIB)
	ar -rc $@ $(SYS_TYPE)/paradisStreaming.o $(SYS_TYPE)/paradis_c_interface.o  $(SYS_TYPE)/paradis.o $(SYS_TYPE)/lib/rclibobjs/*.o
	@echo DONE WITH $@

$(CWD)/$(SYS_TYPE)/bin/paradisTest: $(SYS_TYPE)/paradisTest.o $(PARADISLIB)  $(RCCLIB)
	[ -d $(SYS_TYPE)/bin ] || mkdir -p $(SYS_TYPE)/bin
	$(LD) -o $@ $(LDFLAGS)  $^ $(ELECTRIC_FENCE)

$(CWD)/$(SYS_TYPE)/bin/analyzeParaDIS: $(SYS_TYPE)/analyzeParaDIS.o $(PARADISLIB)  $(RCCLIB)
	[ -d $(SYS_TYPE)/bin ] || mkdir -p $(SYS_TYPE)/bin
	$(LD) -o $@ $(LDFLAGS)  $^ $(ELECTRIC_FENCE)


$(CWD)/$(SYS_TYPE)/bin/paradisStreamingTest: $(SYS_TYPE)/paradisStreamingTest.o $(PARADISLIB)  $(RCCLIB)
	[ -d $(SYS_TYPE)/bin ] || mkdir -p $(SYS_TYPE)/bin
	$(LD) -o $@ $(LDFLAGS)  $^ $(ELECTRIC_FENCE)

.DUMMY: 

$(RCCLIB): .DUMMY
	cd RC_c_lib;  $(MAKE) depend; $(MAKE) install

$(RCCPPLIB): .DUMMY
	cd RC_cpp_lib;  $(MAKE) depend; $(MAKE) install


clean: undepend
	rm -rf $(PROGNAME) makeworms *.o $(SYS_TYPE)/* *~ $(SYS_TYPE)/lib/*o $(SYS_TYPE)/lib/*a 

realclean: clean
	cd RC_cpp_lib;  $(MAKE) undepend; $(MAKE) clean;
	cd RC_c_lib;  $(MAKE) undepend; $(MAKE) clean;

undepend: 
	makedepend -Y $(SFILES)
	for dir in `pwd`/RC_c_lib `pwd`/RC_cpp_lib ; do cd $$dir; $(MAKE) undepend; done

# DO NOT DELETE

chaos_4_x86_64_ib/paradis.o: paradis.h /usr/include/boost/cstdint.hpp
chaos_4_x86_64_ib/paradis.o: /usr/include/boost/config.hpp
chaos_4_x86_64_ib/paradis.o: /usr/include/boost/config/user.hpp
chaos_4_x86_64_ib/paradis.o: /usr/include/boost/config/select_compiler_config.hpp
chaos_4_x86_64_ib/paradis.o: /usr/include/boost/config/compiler/gcc.hpp
chaos_4_x86_64_ib/paradis.o: /usr/include/boost/config/select_stdlib_config.hpp
chaos_4_x86_64_ib/paradis.o: /usr/include/boost/config/select_platform_config.hpp
chaos_4_x86_64_ib/paradis.o: /usr/include/boost/config/suffix.hpp
chaos_4_x86_64_ib/paradis.o: /usr/include/limits.h /usr/include/features.h
chaos_4_x86_64_ib/paradis.o: /usr/include/sys/cdefs.h
chaos_4_x86_64_ib/paradis.o: /usr/include/bits/wordsize.h
chaos_4_x86_64_ib/paradis.o: /usr/include/gnu/stubs.h
chaos_4_x86_64_ib/paradis.o: /usr/include/gnu/stubs-64.h
chaos_4_x86_64_ib/paradis.o: /usr/include/bits/posix1_lim.h
chaos_4_x86_64_ib/paradis.o: /usr/include/bits/local_lim.h
chaos_4_x86_64_ib/paradis.o: /usr/include/linux/limits.h
chaos_4_x86_64_ib/paradis.o: /usr/include/bits/posix2_lim.h
chaos_4_x86_64_ib/paradis.o: /usr/include/boost/limits.hpp
chaos_4_x86_64_ib/paradis.o: /usr/include/boost/config.hpp
chaos_4_x86_64_ib/paradis.o: /usr/include/stdio.h /usr/include/bits/types.h
chaos_4_x86_64_ib/paradis.o: /usr/include/bits/typesizes.h
chaos_4_x86_64_ib/paradis.o: /usr/include/libio.h /usr/include/_G_config.h
chaos_4_x86_64_ib/paradis.o: /usr/include/wchar.h /usr/include/bits/wchar.h
chaos_4_x86_64_ib/paradis.o: /usr/include/gconv.h
chaos_4_x86_64_ib/paradis.o: /usr/include/bits/stdio_lim.h
chaos_4_x86_64_ib/paradis.o: /usr/include/bits/sys_errlist.h
chaos_4_x86_64_ib/paradis.o: /usr/include/math.h /usr/include/bits/huge_val.h
chaos_4_x86_64_ib/paradis.o: /usr/include/bits/mathdef.h
chaos_4_x86_64_ib/paradis.o: /usr/include/bits/mathcalls.h
chaos_4_x86_64_ib/paradis.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_cpp_lib/Point.h
chaos_4_x86_64_ib/paradis.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_cpp_lib/EffectiveCPP.h
chaos_4_x86_64_ib/paradis.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_cpp_lib/RCDebugStream.h
chaos_4_x86_64_ib/paradis.o: /usr/include/signal.h /usr/include/bits/sigset.h
chaos_4_x86_64_ib/paradis.o: /usr/include/bits/signum.h /usr/include/time.h
chaos_4_x86_64_ib/paradis.o: /usr/include/bits/siginfo.h
chaos_4_x86_64_ib/paradis.o: /usr/include/bits/sigaction.h
chaos_4_x86_64_ib/paradis.o: /usr/include/bits/sigcontext.h
chaos_4_x86_64_ib/paradis.o: /usr/include/bits/sigstack.h
chaos_4_x86_64_ib/paradis.o: /usr/include/bits/pthreadtypes.h
chaos_4_x86_64_ib/paradis.o: /usr/include/bits/sigthread.h
chaos_4_x86_64_ib/paradis.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_cpp_lib/stringutil.h
chaos_4_x86_64_ib/paradis.o: /usr/include/inttypes.h /usr/include/stdint.h
chaos_4_x86_64_ib/paradis.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_c_lib/debugutil.h
chaos_4_x86_64_ib/paradis.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_cpp_lib/timer.h
chaos_4_x86_64_ib/paradis.o: /usr/include/sys/time.h /usr/include/bits/time.h
chaos_4_x86_64_ib/paradis.o: /usr/include/sys/select.h
chaos_4_x86_64_ib/paradis.o: /usr/include/bits/select.h /usr/include/dirent.h
chaos_4_x86_64_ib/paradis.o: /usr/include/bits/dirent.h
chaos_4_x86_64_ib/paradis.o: /usr/include/sys/stat.h /usr/include/bits/stat.h
chaos_4_x86_64_ib/paradis.o: /usr/include/errno.h /usr/include/bits/errno.h
chaos_4_x86_64_ib/paradis.o: /usr/include/linux/errno.h
chaos_4_x86_64_ib/paradis.o: /usr/include/asm/errno.h
chaos_4_x86_64_ib/paradis.o: /usr/include/asm-x86_64/errno.h
chaos_4_x86_64_ib/paradis.o: /usr/include/asm-generic/errno.h
chaos_4_x86_64_ib/paradis.o: /usr/include/asm-generic/errno-base.h
chaos_4_x86_64_ib/paradis_c_interface.o: paradis.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/boost/cstdint.hpp
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/boost/config.hpp
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/boost/config/user.hpp
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/boost/config/select_compiler_config.hpp
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/boost/config/compiler/gcc.hpp
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/boost/config/select_stdlib_config.hpp
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/boost/config/select_platform_config.hpp
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/boost/config/suffix.hpp
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/limits.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/features.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/sys/cdefs.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/bits/wordsize.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/gnu/stubs.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/gnu/stubs-64.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/bits/posix1_lim.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/bits/local_lim.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/linux/limits.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/bits/posix2_lim.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/boost/limits.hpp
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/boost/config.hpp
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/stdio.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/bits/types.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/bits/typesizes.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/libio.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/_G_config.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/wchar.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/bits/wchar.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/gconv.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/bits/stdio_lim.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/bits/sys_errlist.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/math.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/bits/huge_val.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/bits/mathdef.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/bits/mathcalls.h
chaos_4_x86_64_ib/paradis_c_interface.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_cpp_lib/Point.h
chaos_4_x86_64_ib/paradis_c_interface.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_cpp_lib/EffectiveCPP.h
chaos_4_x86_64_ib/paradis_c_interface.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_cpp_lib/RCDebugStream.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/signal.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/bits/sigset.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/bits/signum.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/time.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/bits/siginfo.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/bits/sigaction.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/bits/sigcontext.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/bits/sigstack.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/bits/pthreadtypes.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/bits/sigthread.h
chaos_4_x86_64_ib/paradis_c_interface.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_cpp_lib/stringutil.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/inttypes.h
chaos_4_x86_64_ib/paradis_c_interface.o: /usr/include/stdint.h
chaos_4_x86_64_ib/paradis_c_interface.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_c_lib/debugutil.h
chaos_4_x86_64_ib/paradis_c_interface.o: paradis_c_interface.h
chaos_4_x86_64_ib/paradisStreaming.o: paradisStreaming.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/unistd.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/features.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/sys/cdefs.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/bits/wordsize.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/gnu/stubs.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/gnu/stubs-64.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/bits/posix_opt.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/bits/types.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/bits/typesizes.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/bits/confname.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/getopt.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/stdint.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/bits/wchar.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/stdio.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/libio.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/_G_config.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/wchar.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/gconv.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/bits/stdio_lim.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/bits/sys_errlist.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/math.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/bits/huge_val.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/bits/mathdef.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/bits/mathcalls.h
chaos_4_x86_64_ib/paradisStreaming.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_cpp_lib/Point.h
chaos_4_x86_64_ib/paradisStreaming.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_cpp_lib/EffectiveCPP.h
chaos_4_x86_64_ib/paradisStreaming.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_cpp_lib/RCDebugStream.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/signal.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/bits/sigset.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/bits/signum.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/time.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/bits/siginfo.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/bits/sigaction.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/bits/sigcontext.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/bits/sigstack.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/bits/pthreadtypes.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/bits/sigthread.h
chaos_4_x86_64_ib/paradisStreaming.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_cpp_lib/stringutil.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/inttypes.h
chaos_4_x86_64_ib/paradisStreaming.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_c_lib/debugutil.h
chaos_4_x86_64_ib/paradisStreaming.o: paradis.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/boost/cstdint.hpp
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/boost/config.hpp
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/boost/config/user.hpp
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/boost/config/select_compiler_config.hpp
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/boost/config/compiler/gcc.hpp
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/boost/config/select_stdlib_config.hpp
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/boost/config/select_platform_config.hpp
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/boost/config/suffix.hpp
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/limits.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/bits/posix1_lim.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/bits/local_lim.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/linux/limits.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/bits/posix2_lim.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/boost/limits.hpp
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/boost/config.hpp
chaos_4_x86_64_ib/paradisStreaming.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_cpp_lib/timer.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/sys/time.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/bits/time.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/sys/select.h
chaos_4_x86_64_ib/paradisStreaming.o: /usr/include/bits/select.h
chaos_4_x86_64_ib/paradisTest.o: paradis.h /usr/include/boost/cstdint.hpp
chaos_4_x86_64_ib/paradisTest.o: /usr/include/boost/config.hpp
chaos_4_x86_64_ib/paradisTest.o: /usr/include/boost/config/user.hpp
chaos_4_x86_64_ib/paradisTest.o: /usr/include/boost/config/select_compiler_config.hpp
chaos_4_x86_64_ib/paradisTest.o: /usr/include/boost/config/compiler/gcc.hpp
chaos_4_x86_64_ib/paradisTest.o: /usr/include/boost/config/select_stdlib_config.hpp
chaos_4_x86_64_ib/paradisTest.o: /usr/include/boost/config/select_platform_config.hpp
chaos_4_x86_64_ib/paradisTest.o: /usr/include/boost/config/suffix.hpp
chaos_4_x86_64_ib/paradisTest.o: /usr/include/limits.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/features.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/sys/cdefs.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/bits/wordsize.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/gnu/stubs.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/gnu/stubs-64.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/bits/posix1_lim.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/bits/local_lim.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/linux/limits.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/bits/posix2_lim.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/boost/limits.hpp
chaos_4_x86_64_ib/paradisTest.o: /usr/include/boost/config.hpp
chaos_4_x86_64_ib/paradisTest.o: /usr/include/stdio.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/bits/types.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/bits/typesizes.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/libio.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/_G_config.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/wchar.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/bits/wchar.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/gconv.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/bits/stdio_lim.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/bits/sys_errlist.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/math.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/bits/huge_val.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/bits/mathdef.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/bits/mathcalls.h
chaos_4_x86_64_ib/paradisTest.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_cpp_lib/Point.h
chaos_4_x86_64_ib/paradisTest.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_cpp_lib/EffectiveCPP.h
chaos_4_x86_64_ib/paradisTest.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_cpp_lib/RCDebugStream.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/signal.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/bits/sigset.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/bits/signum.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/time.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/bits/siginfo.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/bits/sigaction.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/bits/sigcontext.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/bits/sigstack.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/bits/pthreadtypes.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/bits/sigthread.h
chaos_4_x86_64_ib/paradisTest.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_cpp_lib/stringutil.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/inttypes.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/stdint.h
chaos_4_x86_64_ib/paradisTest.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_c_lib/debugutil.h
chaos_4_x86_64_ib/paradisTest.o: RC_c_lib/debugutil.h RC_c_lib/args.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/stdlib.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/sys/types.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/endian.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/bits/endian.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/sys/select.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/bits/select.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/bits/time.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/sys/sysmacros.h
chaos_4_x86_64_ib/paradisTest.o: /usr/include/alloca.h
chaos_4_x86_64_ib/analyzeParaDIS.o: paradis.h /usr/include/boost/cstdint.hpp
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/boost/config.hpp
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/boost/config/user.hpp
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/boost/config/select_compiler_config.hpp
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/boost/config/compiler/gcc.hpp
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/boost/config/select_stdlib_config.hpp
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/boost/config/select_platform_config.hpp
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/boost/config/suffix.hpp
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/limits.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/features.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/sys/cdefs.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/bits/wordsize.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/gnu/stubs.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/gnu/stubs-64.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/bits/posix1_lim.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/bits/local_lim.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/linux/limits.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/bits/posix2_lim.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/boost/limits.hpp
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/boost/config.hpp
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/stdio.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/bits/types.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/bits/typesizes.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/libio.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/_G_config.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/wchar.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/bits/wchar.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/gconv.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/bits/stdio_lim.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/bits/sys_errlist.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/math.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/bits/huge_val.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/bits/mathdef.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/bits/mathcalls.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_cpp_lib/Point.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_cpp_lib/EffectiveCPP.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_cpp_lib/RCDebugStream.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/signal.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/bits/sigset.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/bits/signum.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/time.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/bits/siginfo.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/bits/sigaction.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/bits/sigcontext.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/bits/sigstack.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/bits/pthreadtypes.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/bits/sigthread.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_cpp_lib/stringutil.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/inttypes.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/stdint.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_c_lib/debugutil.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /g/g0/rcook/current_projects/paraDIS/paraDIS_lib/RC_cpp_lib/Prefs.h
chaos_4_x86_64_ib/analyzeParaDIS.o: RC_c_lib/args.h /usr/include/stdlib.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/sys/types.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/endian.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/bits/endian.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/sys/select.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/bits/select.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/bits/time.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/sys/sysmacros.h
chaos_4_x86_64_ib/analyzeParaDIS.o: /usr/include/alloca.h paradis_version.h
