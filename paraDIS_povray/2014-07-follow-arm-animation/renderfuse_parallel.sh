#!/usr/bin/env bash

. $HOME/RC_bash_lib/shellfuncs.sh

errexit() {
	echo $1
	exit 1
}
# get user input
basename=${1:-rs0240}
distPerTimestep=${2:-"-1"};

# compute some values
declfile=${basename}-decl.pov
maxdist=$(grep ParaDIS_MaxLightTheFuseDistance $declfile | sed  -e 's/.*= \([0-9]*\);/\1/')
if [ "$maxdist" == "" ] || ! expr $maxdist + 1; then 
	errexit "Cannot get maxdist from declaration file $declfile"
fi
if [ "$distPerTimestep" == "-1" ]; then 
	distPerTimestep=$(expr $maxdist / 180)
	echo "Computed movement rate is $distPerTimestep segments per timestep"
fi

# start looping
mydist=$(expr $procnum \* $distPerTimestep);

while [ "$armdist" -le "$maxdist" ]; do 
	
