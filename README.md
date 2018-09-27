# paraDIS_lib
paraDIS_lib is a library that provides the ability to create useful
visualizations and detailed analysis of dumpfiles created from a
ParaDIS simulation code.

paraDIS_lib's oputput consists of both a C++ library,
**libparadis.a**, which can enable visualization and analysis of data
in external tools such as VisIt, and an executable,
**analyzeParaDIS**, which can be used to analyze and visualize a
ParaDIS dumpfile.

See <https://ipo.llnl.gov/technologies/paradis> for more information
about ParaDIS.  The image displayed on the ParaDIS website was created
with the analyzeParaDIS tool, which is part of paraDIS_lib.

## Compiling and Installation

paraDIS\_lib requires g++ 4.9.2 to install.  You can set the CC and
CXX variables to help find the compilers as needed or simply set your
PATH so that gcc and g++ are the right versions.  Also required are
the Boost 1.55 and tclap 1.20 libraries.  By default, the Makefile
assumes they are installed in the current directory as ./boost and
./tclap.  However, you can also set BOOSTDIR and TCLAPDIR when
compiling, something like this:

```bash
CC=/usr/apps/gnu/4.9.2/bin/gcc \
CXX=/usr/apps/gnu/4.9.2/bin/g++ \
INSTALL_DIR=./install_dir/ \
BOOSTDIR=/usr/local/tools/boost-nompi-1.55.0 \
TCLAPDIR=/g/g0/rcook/current_projects/RC_cpp_lib/tclap-1.2.0 \
make install
```

The boost libraries must include compiled libboost\_filesystem and
libboost\_system libraries.

Files are installed into `$INSTALL_DIR` if defined, else a directory
called `./$SYS_TYPE` if `SYS_TYPE` is defined, else a directory called
`./uname -n`.

## Usage

### libparadis.a
paraDIS\_lib builds a library which you should be able to link in as
part of a plugin architecture.  This has been used to build plugins
for LLNL's VisIt tool in the past.  Since the code is written in C++,
a C-style interface is also provided and can be accessed by including
paradis\_c\_interface.h.

### analyzeParaDIS
This is what most users will use.  Run analyzeParaDIS --help to get a
list of options and usage.  Most commonly you are going to run it like
this: `analyzeParaDIS --full <dumpfile.data>` This will create VTK
files which can be viewed independently or all at once by using LLNL's
VisIt tool by opening the .visit file.  Much can be learned also by
viewing the OUTPUT_DIR/summary.txt file that gets created.

## Credits
I wrote all the code in this library in collaboration with Meijie
Tang, Moono Rhee, Sylvie Aubry, Tom Arsenlis, Gregg Hommes and the
rest of the ParaDIS team at LLNL.  What a fun project!
