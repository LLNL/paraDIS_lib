#!/usr/bin/env bash 
# This script is for fullscale rendering, as well as quick and dirty looks at the current image, using multires and other speedups
#  Example: 
# fast=true glowradius=5 dofuse=1 afterglow=Off srun -n 40 -W 500000 renderfuse_parallel.sh |& tee render.out
# 
# set -e
PROG=$(basename $0)
usage() {
	echo "$0 -- render the render.inc file with povray" 
	echo "$0 [options]"
	echo "----------------------------------------------------"
	echo "OPTIONS:"
	echo "-b/--basename name:   Set to \"name\" to render name-obj.pov and name-decl.pov.  Default: rs0240"
	echo "-f/--fast:            Same as --quality 7 --no-antialias"
	echo "-h/--help:            print this menu" 
	echo "-A/--no-antialias:    Set Antialias to Off."
	echo "-q/--quality n:       Set Quality to 'n' (9 is full)."
}

errexit() {
	echo $1
	exit ${2:-1}
}
. ${HOME}/current_projects/RC_bash_lib/shellfuncs.sh || errexit "Could not source shellfuncs.sh"

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
while [ $# -gt 0 ]; do
    case "$1" in
        -A | --no-antialias)  antialias=Off;;
        -b | --basename)      basename="$2"; shift;;
        -f | --fast)          fast=true;;
        -h | --help)          usage; exit 0;;
        -q | --quality)       quality="$2"; shift;;
		-v | --verbose)       VERBOSE=true;;
        --)                   shift; break;; # end of options
    esac
    shift
done
if $VERBOSE; then 
	set -xv
fi
wdir=`dirname $0`
cd $wdir
echo; echo; echo beginning $0; echo; echo
basename=${1:-${basename:-rs0240}}

# usage: startrow height procnum numprocs
# procnum is 0-based, just SLURM_PROCID
#startrow 100 1 5  (answer:  1)
#startrow 100 3 5  (answer:  41)
#endrow 100 3 5 ( answer:  60)
#endrow 100 5 5 ( answer:  100)

function startrow () {
    /usr/local/bin/python -c "print (int) (${1}.0 * (${2}.0)/${3}.0) + 1" 
}
# usage: endrow height procnum numprocs
function endrow () {
    /usr/local/bin/python -c "print (int) (${1}.0 * (${2}.0+1)/${3}.0)"     
}

numprocs=$(getnumprocs)
procnum=$(getprocnum)

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
# cat the files together instead of using #include to speed parsing
frame=${frame:-0}


debug="${debug:-Declare=debug=1}"
# basename=$(basename $basename)
outdir=${outdir:-.}

logfile=$outdir/logistics/${basename}_${frame}.log
mkdir -p $outdir/{logistics,images}
if [ $numprocs -gt 1 ]; then 
	echo "output for processor $procnum on host $(uname -n) is going to $logfile"
	exec >& $logfile
fi
echo "$(date): $0 $@"

inifile=$outdir/logistics/${basename}_${frame}.ini 
outfile=$outdir/images/${basename}_${frame}.png  
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


display=${display:-On}
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

	
povfile=$outdir/logistics/${basename}-${frame}-combined.pov
cat ${declfile} render.inc ${objfile} > $povfile

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
echo render complete
