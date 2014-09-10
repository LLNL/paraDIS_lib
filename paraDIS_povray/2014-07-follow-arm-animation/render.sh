#!/usr/bin/env bash 
# This script is for fullscale rendering, as well as quick and dirty looks at the current image, using multires and other speedups

# srun -n 40 -W 500000 render.sh
# 
# set -e
PROG=$(basename $0)
usage() {
	echo "$0 -- render the render.inc file with povray" 
	echo "$0 [options]"
	echo "----------------------------------------------------"
	echo "OPTIONS:"
	echo "-b/--basename name:   Set to \"name\" to render name-obj.pov and name-decl.pov.  Default: rs0240"
	echo "-d/--display:         Render to display instead of file"
	echo "-f/--fast:            Same as --quality 7 --no-antialias"
	echo "-h/--help:            print this menu" 
	echo "-A/--no-antialias:    Set Antialias to Off."
	echo "-n/--numtasks n:      control parallelism.  Default: 4 * SLURM_JOB_NUM_NODES = $(expr 4 * $SLURM_JOB_NUM_NODES)"
	echo "-o/--outdir name:     Store output in dir 'name'." 
	echo "-q/--quiet:           Normally, stdout/stderr are sent to a log file and to stdout. If this switch is used, then output will only go to a log."
	echo "-s/--serial:          Same as --numtasks 1"
	echo "-Q/--quality n:       Set Quality to 'n' (9 is full)."
	echo "-t/--timesteps n:     Number of timesteps to generate.  Default is 180." 
}

errexit() {
	echo "ERROR: $1" 
	exit ${2:-1}
}
. ${HOME}/current_projects/RC_bash_lib/shellfuncs.sh || errexit "Could not source shellfuncs.sh"

#  http://stackoverflow.com/questions/2721946/cross-platform-getopt-for-a-shell-script
# use getopt to do error checking on command line
getopt -T > /dev/null
if [ $? -eq 4 ]; then
  # GNU enhanced getopt is available
	ARGS=$(getopt --name "$PROG" --long no-antialias,basename:,fast,help,quality:,verbose --options Ab:fhq: -- "$@")
else
  # Original getopt is available (no long option names, no whitespace, no sorting)
	ARGS=$(getopt b:fhq:v "$@")
fi
if [ $? -ne 0 ]; then
	echo "$PROG: usage error (use -h for help)" >&2
	exit 2
fi
eval set -- $ARGS
# Parameters are now sorted: options appear first, followed by --, then arguments
# e.g. entering: "foo bar" -o abc baz -v
#      produces: -o 'abc' -v -- 'foo bar' 'baz'

# compute some values
export procnum=`getprocnum` 2>/dev/null
export numprocs=`getnumprocs` 2>/dev/null

display=Off
timesteps=180
logonly=false
numtasks=$(expr 4 * $numprocs)
basename=rs0240
outdir=${basename}_$(date +%F-%H-%M-%S)
ischild=false
VERBOSEFLAG=

while [ $# -gt 0 ]; do
    case "$1" in
        -A | --no-antialias)  antialias=Off;;
-b | --basename)      basename="$2"; shift;;
-c | --childproc)     ischild=true;;
-d | --display)       display=On;;
-f | --fast)          fast=true;;
-h | --help)          usage; exit 0;;
-n | --numtasks)      numtasks="$2"; shift;;
-o | --outdir)        outdir="$2"; shift;;
-q | --quality)       quality="$2"; shift;;
-s | --serial)        numtasks=1; shift;;
-t | --timesteps)     timesteps="$2"; shift;;
-v | --verbose)       VERBOSE=true;;
--)                   shift; break;; # end of options
esac
shift
done

mkdir -p $outdir/{logistics,images}

if ! $ischild && [ "$numtasks" -gt 1 ]; then 
	echo "Executing $numtasks tasks in parallel.  logfile is $logfile"
	srun -W 3000000 -n $numtasks $0 --childproc -b $basename -t $timesteps -o $outdir $VERBOSEFLAG |& tee $logfile	
	exit $?
fi

if [ $numtasks -gt 1 ]; then 
	logfile=$outdir/parallel_render-$(date +%F-%H-%M-%S)-$procnum.log
	echo "output for processor $procnum on host $(uname -n) is going to $logfile"
	exec >& $logfile
fi

echo; echo; echo "beginning $(date): $0 $@"; echo; echo

if [ ! -z "$VERBOSEFLAG" ]; then
	set -xv
fi
wdir=`dirname $0`
cd $wdir

# usage: startrow height procnum numprocs
# procnum is 0-based, just SLURM_PROCID
#startrow 100 1 5  (answer:  1)
#startrow 100 3 5  (answer:  41)
#endrow 100 3 5 ( answer:  60)
#endrow 100 5 5 ( answer:  100)

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

function startrow () {
    /usr/local/bin/python -c "print (int) (${1}.0 * (${2}.0)/${3}.0) + 1" 
}
# usage: endrow height procnum numprocs
function endrow () {
    /usr/local/bin/python -c "print (int) (${1}.0 * (${2}.0+1)/${3}.0)"     
}


# usage: stitch width height numprocs firstproc
function stitch () {
    pbmmake -black $1 $2 > base.pbm
    cmd='convert ' 
    if [ x$4 != x ]; then i=$4; else  i=0; fi
    while [ $i -lt $3 ] ; do
		start=`startrow $2 $i $3`
		let start--
		name=`povname $i $basename`
		cmd=${cmd}"-draw 'image Over 0,$start ${1},${2} $name' "
		let i++
    done
    cmd=${cmd}"base.pbm final.png"
    echo "$cmd" 
    eval $cmd
}

default_armweight='-armweight 0.2'
#sleep 5
echo SLURM_PROCID is $SLURM_PROCID

declfile="${declfile:-${basename}-decl.pov}"
objfile="${objfile:-${basename}-obj.pov}"
# copy the pov files into a uniquely named input file for posterity
# unique to each processor to avoid corruption
for name in objfile declfile; do
	if [ ! -f $outdir/logistics/\$${name}-proc$procnum ]; then 
		eval cp \$$name $outdir/logistics/\$${name}-proc$procnum
		eval ${name}=$outdir/logistics/\$${name}-proc$procnum
	fi
done

segdistance=${segdistance:-0}
dofuse=${dofuse:-0}
glowradius=${glowradius:-1}
afterglow=${afterglow:-On}

if [ "$dofuse" != 0 ]; then 
	doglow=0
else 
	doglow=${doglow:-0}
fi

if $fast; then 
	antialias=Off
	quality=7
else
	antialias=${antialias:-On}
	quality=${quality:-9} # http://www.povray.org/documentation/view/3.7.0/223/
fi

date
for thing in basename frame declfile objfile outdir logfile inifile outfile; do
	eval echo $thing is '\"$'$thing'\"'
done

povfile=$outdir/logistics/${basename}-task_${tasknum}-combined.pov
cat ${declfile} render.inc ${objfile} > $povfile

# start looping\
frame=$procnum
mydist=$(expr \( $frame + 1 \)  \* $distPerTimestep);
while [ "$mydist" -le "$maxdist" ]; do 

	debug="${debug:-Declare=debug=1}"


	inifile=$outdir/logistics/${basename}_${frame}.ini 
	outfile=$outdir/images/${basename}_${frame}.png  
	

# tmpfile=$(mktemp -u)
	cat <<EOF >$inifile
	Antialias=$antialias
	Antialias_Threshold=0.2
	Declare=debug=$debug
	Declare=BoundsRadius=${boundsradius:-50}
	Declare=SegmentRadius=${segradius:-50}
	Declare=AnimateFuse=${dofuse}
	Declare=AnimateGrow=${doglow}
	Declare=Backdrop=${backdrop:-0}
	Declare=DoAxes=${doaxes:-0}
	Declare=DoBounds=${dobounds:-1}
	Declare=DoAfterglow=${afterglow}
	Declare=GlowRadius=${glowradius}
	Declare=SegmentDistance=${segdistance}
	Declare=Shadows=1
	Display=$display
	End_Row=1.0
	Height=${height:-960}
	Input_File_Name=$povfile
	Width=${width:-1280}
	Output_File_Name=${outfile}
	Output_File_Type=n
	Pause_when_done=On
	Preview_Start_Size=64
	Quality=${quality:-9}
	Sampling_Method=2
	Start_Row=0.0
	Video_Mode=1
	Verbose=On
	;; Declare=camPath=1
	EOF

	echo 'inifile contents:'
	cat $inifile

	povray $inifile

# mv $tmpfile $outfile
	echo frame $frame complete
	let "frame += $numprocs"
	mydist=$(expr \( $frame + 1 \) \* $distPerTimestep);
done

echo $0 task $procnum complete at $(date)
