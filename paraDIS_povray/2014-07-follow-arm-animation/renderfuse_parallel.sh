#!/usr/bin/env bash

# example:  srun -W 3000000 -n 48 renderfuse_parallel.sh |& tee renderfuse_parallel.log
errexit() {
	echo $1
	exit 1
}
. $HOME/RC_bash_lib/shellfuncs.sh || errexit "Could not source shellfuncs.sh"
set -xv
# get user input
basename=${1:-rs0240}
distPerTimestep=${2:-"-1"};
outdir=${basename}-outdir
# compute some values
export procnum=`getprocnum` 2>/dev/null
export numprocs=`getnumprocs` 2>/dev/null
# just redirect srun to a single file instead of doing this: 
#logfile=$outdir/logfiles/renderfuse_parallel_${procnum}.log
#mkdir -p $outdir/logfiles
#echo Sending output to logfile $logfile
#exec >& $logfile 

declfile=${basename}-decl.pov
maxdist=$(grep ParaDIS_MaxLightTheFuseDistance $declfile | sed  -e 's/.*= \([0-9]*\);/\1/')
if [ "$maxdist" == "" ] || ! expr $maxdist + 1; then 
	errexit "Cannot get maxdist from declaration file $declfile"
fi
if [ "$distPerTimestep" == "-1" ]; then 
	distPerTimestep=$(expr $maxdist / 180)
fi
for thing in procnum numprocs basename distPerTimestep outdir; do 
	eval echo $thing is '\"$'$thing'\"'
done

# start looping\
frame=$procnum
mydist=$(expr \( $frame + 1 \)  \* $distPerTimestep);
while [ "$mydist" -le "$maxdist" ]; do 
	frame=$frame outdir=$outdir antialias=Off quality=7 display=Off dofuse=1 segdistance=$mydist render.sh 
	let "frame += $numprocs"
	mydist=$(expr \( $frame + 1 \) \* $distPerTimestep);
done
echo $0 task $procnum complete at $(date)

