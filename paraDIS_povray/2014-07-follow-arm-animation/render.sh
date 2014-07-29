#!/usr/bin/env bash 
# $Id: quicklook.sh,v 1.3 2006/01/20 19:09:03 rcook Exp $
# This script is for quick and dirty looks at the current image, using multires and other speedups
#  

echo '$Id: quicklook.sh,v 1.3 2006/01/20 19:09:03 rcook Exp $'
errexit() {
	echo $1
	exit ${2:-1}
}

. ${HOME}/current_projects/RC_bash_lib/shellfuncs.sh || errexit "Could not source shellfuncs.sh"

set -vx
wdir=`dirname $0`

default_height=960
default_width=1280
default_basename=rs0240
default_verbose=5
default_debug='Declare=debug=yes'
default_armweight='-armweight 0.2'


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


#usage:  doconvert (args are taken from environment for now) 
function doconvert () {
    cmd='../restartConvert -pov -template rs%04d.data -first $step -name ${basename} -v $verbose -spheres $armweight'
    eval echo $cmd
    eval $cmd >${basename}.log 2>&1
}

# usage:  povname procnum basename 
# echoes the povray filename (used for consistent naming between scripts)
function povname () {
    echo ${2}_${1}.tga
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

export procnum=`getprocnum` 2>/dev/null
export numprocs=`getnumprocs` 2>/dev/null

height=${height:-$default_height}
width=${width:-$default_width}

cd $wdir
echo; echo; echo beginning $0; echo; echo
#sleep 5
echo SLURM_PROCID is $SLURM_PROCID
# cat the files together instead of using #include to speed parsing
basename=${1:-rs0240}
declfile="${basename}-decl.pov"
objfile="${basename}-obj.pov"

display=${display:-On}
segdistance=${segdistance:-0}
dofuse=${dofuse:-0}
if [ "$segdistance}" -gt 0 ]; then 
	dofuse=1
fi

cat <<EOF >tmpy.ini
Output_File_Type=n
Verbose=On
Pause_when_done=On
Start_Row=0.0
End_Row=1.0
Video_Mode=1
Display=$display
Antialias=On
Antialias_Threshold=0.99
Sampling_Method=2
Declare=Shadows=1
Declare=SphereRadius=50
Declare=CylinderRadius=50
Declare=BoundsRadius=50
Declare=LightTheFuse=${dofuse}
Declare=MaxSegmentDistance=${segdistance}
;; Declare=camPath=1
EOF

cat ${declfile} render.inc ${objfile} > tmpy.pov

cmd="povray +H${height} +W${width} $debug  +o${basename}_quickie.png   +SP256 tmpy.ini +Itmpy.pov"
#cmd='povray3.7 -Itmpy.ini  +o${povfile}_quickie.png +D +H${height} +W${width} $debug  +SP256 +P tmpy.pov'
echo $cmd
$cmd

