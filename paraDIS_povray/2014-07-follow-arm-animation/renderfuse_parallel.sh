#!/usr/bin/env bash
# example:  
# data=rs1907 
# data=rs0240 
# srun -W 3000000 -n 100 renderfuse_parallel.sh $data |& tee renderfuse_parallel-$(date +%F-%H-%M-%S).log

PROG=$(basename $0)
usage() {
	echo "$0 -- launches render.sh in parallel and sets up logs, images etc. to land in the right subdirectory.  to run in serial, Just use render.sh" 
	echo "$0 [options] -- [render.sh options]"
	echo "----------------------------------------------------"
	echo "OPTIONS:"
	echo "-b/--basename name: e.g. rs0240, if you have rs0240-obj.pov and rs0240-decl.pov.  Default: rs0240"
	echo "-h/--help:          print this menu" 
	echo "-n/--numnodes n:    control parallelism.  Default: 4 * SLURM_JOB_NUM_NODES = $(expr 4 * $SLURM_JOB_NUM_NODES)"
	echo "-o/--outdir name:   Store output in dir 'name'." 
	echo "-t/--timesteps n:   Number of timesteps to generate.  Default is 180." 
	echo "-q/--quiet:         Normally, stdout/stderr are sent to a log file and to stdout. If this switch is used, then output will only go to a log."
	echo "everything after -- is passed to render.sh in subprocesses.  Here is the render help menu:"
	render.sh -h	
}
errexit() {
	echo "ERROR: $1"
	exit 1
}
. $HOME/RC_bash_lib/shellfuncs.sh || errexit "Could not source shellfuncs.sh"
set -xv

#  http://stackoverflow.com/questions/2721946/cross-platform-getopt-for-a-shell-script
# use getopt to do error checking on command line
getopt -T > /dev/null
if [ $? -eq 4 ]; then
  # GNU enhanced getopt is available
  ARGS=$(getopt --name "$PROG" --long basename:,help,numnodes:,timesteps:,quiet,outdir:,verbose --options b:hn:t:qo:v -- "$@")
else
  # Original getopt is available (no long option names, no whitespace, no sorting)
  ARGS=$(getopt b:hn:t:qo: "$@")
fi
if [ $? -ne 0 ]; then
  echo "$PROG: usage error (use -h for help)" >&2
  exit 2
fi
eval set -- $ARGS
 
timesteps=180
logonly=false
numnodes=$(expr 4 * $SLURM_JOB_NUM_NODES)
basename=rs0240
outdir=${basename}_$(date +%F-%H-%M-%S)

while [ $# -gt 0 ]; do
    case "$1" in
        -b | --basename)  basename="$2"; shift;;
        -h | --help)      usage; exit 0;;
        -n | --numnodes)  numnodes="$2"; shift;;
        -t | --timesteps) timesteps="$2"; shift;;
        -o | --outdir)    outdir="$2"; shift;;
        -v | --verbose)   VERBOSE=yes;;
        --)               shift; break;; # end of options
    esac
    shift
done
if $VERBOSE; then
	set -xv
fi
# compute some values
export procnum=`getprocnum` 2>/dev/null
export numprocs=`getnumprocs` 2>/dev/null


mkdir -p $outdir/logistics
# just redirect srun to a single file instead of doing this: 
# logfile=$outdir/logistics/renderfuse_parallel_${procnum}.log
# echo Sending output to logfile $logfile
# exec >& $logfile 

declfile=${basename}-decl.pov
objfile=${basename}-obj.pov
maxdist=$(grep ParaDIS_MaxLightTheFuseDistance $declfile | sed  -e 's/.*= \([0-9]*\);/\1/')
if [ "$maxdist" == "" ] || ! expr $maxdist + 1; then 
	errexit "Cannot get maxdist from declaration file $declfile"
fi
distPerTimestep=$(expr $maxdist / $timesteps)
if [ $(expr $maxdist % $timesteps) != 0 ]; then
	let distPerTimestep++
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
	cmd="frame=$frame outdir=$outdir display=Off dofuse=1 segdistance=$mydist glowradius=${glowradius:3} afterglow=${afterglow:On} render.sh "
	eval $cmd
	let "frame += $numprocs"
	mydist=$(expr \( $frame + 1 \) \* $distPerTimestep);
done
#cp -r $outdir/* ${basename}-$(date +%F-%H-%M-%S)/
echo $0 task $procnum complete at $(date)
