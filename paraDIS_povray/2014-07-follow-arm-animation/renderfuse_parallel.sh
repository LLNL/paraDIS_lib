#!/usr/bin/env bash

# example:  
# data=rs1907 
# data=rs0240 
# srun -W 3000000 -n 100 renderfuse_parallel.sh $data |& tee renderfuse_parallel-$(date +%F-%H-%M-%S).log
errexit() {
	echo $1
	exit 1
}
. $HOME/RC_bash_lib/shellfuncs.sh || errexit "Could not source shellfuncs.sh"
set -xv
# get user input
quick=
while [ "${1:0:1}" != "-" ] ; do 
	if [ "$1" == '-q' ] || [ "$1" == "--quick" ]; then 
		quick="--quick"
		shift
	fi
done
	
export basename=${1:-${basename:-rs0240}}
distPerTimestep=${2:-"-1"};
# compute some values
export procnum=`getprocnum` 2>/dev/null
export numprocs=`getnumprocs` 2>/dev/null

outdir=${basename}_$(date +%F-%H-%M-%S)
mkdir -p $outdir
# just redirect srun to a single file instead of doing this: 
#logfile=$outdir/logfiles/renderfuse_parallel_${procnum}.log
#mkdir -p $outdir/logfiles
#echo Sending output to logfile $logfile
#exec >& $logfile 

declfile=${basename}-decl.pov
objfile=${basename}-obj.pov
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

cp $0 $outdir
# pushd $outdir
# tmpdir=$(mktemp -d --tmpdir=$outdir)
# cp ${basename}-{decl,obj}.pov $tmpdir/
# basename=$tmpdir/$(basename $basename)
# start looping\
frame=$procnum
mydist=$(expr \( $frame + 1 \)  \* $distPerTimestep);
while [ "$mydist" -le "$maxdist" ]; do 
	frame=$frame outdir=$outdir display=Off dofuse=1 segdistance=$mydist render.sh $quick
	let "frame += $numprocs"
	mydist=$(expr \( $frame + 1 \) \* $distPerTimestep);
done
cp -r $outdir/* ${basename}-$(date +%F-%H-%M-%S)/
echo $0 task $procnum complete at $(date)
