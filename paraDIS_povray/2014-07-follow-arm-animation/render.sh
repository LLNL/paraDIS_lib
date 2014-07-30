#!/usr/bin/env bash 
# This script is for fullscale rendering, as well as quick and dirty looks at the current image, using multires and other speedups
#  

errexit() {
	echo $1
	exit ${2:-1}
}

. ${HOME}/current_projects/RC_bash_lib/shellfuncs.sh || errexit "Could not source shellfuncs.sh"

set -vx
wdir=`dirname $0`
cd $wdir
echo; echo; echo beginning $0; echo; echo

if [ "$1" == "-quick" ] || [ "$1" == "-q" ]; then 
	quick=true
	shift
else
	quick=false
fi
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
declfile="${basename}-decl.pov"
objfile="${basename}-obj.pov"
outdir=${outdir:-.}
logfile=$outdir/logistics/${basename}_${frame}.log
inifile=$outdir/logistics/${basename}_${frame}.ini 
outfile=$outdir/images/${basename}_${frame}.png  
height=${height:-960}
width=${width:-1280}

display=${display:-On}
segdistance=${segdistance:-0}
animatefuse=${animatefuse:-0}
glowradius=${glowradius:-0}
if [ "$animatefuse" != 0 ]; then 
	animategrow=0
else 
	animategrow=${animategrow:-0}
fi

if $quick; then 
	antialias=Off
	quality=7
else
	antialias=${antialias:-On}
	quality=${quality:-9} # http://www.povray.org/documentation/view/3.7.0/223/
fi

mkdir -p $outdir/{logistics,images}
numprocs=$(getnumprocs)
if [ $numprocs -gt 1 ]; then 
	echo "output is going to $logfile"
	exec >& $logfile
fi

date
for thing in basename frame declfile objfile outdir logfile inifile outfile; do
	eval echo $thing is '\"$'$thing'\"'
done

# if [ "${segdistance}" -gt 0 ]; then 
# 	animatefuse=1
# fi

	
povfile=$outdir/logistics/${basename}-${frame}-combined.pov
cat ${declfile} render.inc ${objfile} > $povfile

cat <<EOF >$inifile
Antialias=$antialias
;; Antialias_Threshold=0.2
Declare=debug=$debug
Declare=BoundsRadius=50
Declare=CylinderRadius=50
Declare=AnimateFuse=${animatefuse}
Declare=AnimateGrow=${animategrow}
Declare=GlowRadius=${glowradius}
Declare=MaxSegmentDistance=${segdistance}
Declare=Shadows=1
Declare=SphereRadius=50
Display=$display
End_Row=1.0
Input_File_Name=$povfile
Output_File_Type=n
Pause_when_done=On
Quality=$quality
Sampling_Method=2
Start_Row=0.0
Video_Mode=1
Verbose=On
;; Declare=camPath=1
EOF

echo 'inifile contents:'
cat $inifile

cmd="povray +H${height} +W${width} $debug  +o${outfile}  +SP256 $inifile"
#cmd='povray3.7 -Itmpy.ini  +o${povfile}_quickie.png +D +H${height} +W${width} $debug  +SP256 +P tmpy.pov'
echo $cmd
$cmd

echo render complete
