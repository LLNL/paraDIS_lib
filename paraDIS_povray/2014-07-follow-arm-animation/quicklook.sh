#!/usr/bin/env bash 
# $Id: quicklook.sh,v 1.3 2006/01/20 19:09:03 rcook Exp $
# This script is for quick and dirty looks at the current image, using multires and other speedups
#  

echo '$Id: quicklook.sh,v 1.3 2006/01/20 19:09:03 rcook Exp $'
set -vx
wdir=`dirname $0`
errexit() {
	echo $1
	exit ${2:-1}
}
. $wdir/image-common.sh || errexit "Cannot find image-common.sh"
cd $wdir
echo; echo; echo beginning $0; echo; echo
#sleep 5
echo SLURM_PROCID is $SLURM_PROCID
# cat the files together instead of using #include to speed parsing
basename=${1:-rs0240}
declfile="${basename}-decl.pov"
objfile="${basename}-obj.pov"

display=${display:-On}

cat <<EOF >tmpy.ini
Output_File_Type=n
Verbose=On
Pause_when_done=Off
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
;; Declare=camPath=1
EOF

cat ${declfile} render.inc ${objfile} > tmpy.pov

cmd="povray +H${height} +W${width} $debug  +o${povfile}_quickie.png   +SP256 +P tmpy.ini +Itmpy.pov"
#cmd='povray3.7 -Itmpy.ini  +o${povfile}_quickie.png +D +H${height} +W${width} $debug  +SP256 +P tmpy.pov'
echo $cmd
$cmd

