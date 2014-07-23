#!/usr/bin/env bash 
# $Id: quicklook.sh,v 1.3 2006/01/20 19:09:03 rcook Exp $
# This script is for quick and dirty looks at the current image, using multires and other speedups
#  

echo '$Id: quicklook.sh,v 1.3 2006/01/20 19:09:03 rcook Exp $'
set -vx
wdir=`dirname $0`
. $wdir/image-common.sh
cd $wdir
echo; echo; echo beginning $0; echo; echo
#sleep 5
echo SLURM_PROCID is $SLURM_PROCID
# cat the files together instead of using #include to speed parsing
rm -f tmpy.pov

cat <<EOF >tmpy.ini
;; Library_Path="/usr/local/tools/povray/share/povray-3.6"
Output_File_Type=n
Verbose=On
Pause_when_done=Off
Start_Row=0.0
End_Row=1.0
Video_Mode=1

;; Display=Off
Antialias=On
Antialias_Threshold=0.99
Sampling_Method=2
Declare=Shadows=1
EOF

cat renderparams.inc > tmpy.pov
cat ${basename}_${step}.pov >> tmpy.pov
cmd='povray -Itmpy.ini   +o${basename}_quickie.png +H${height} +W${width} $debug  +SP256 +P tmpy.pov'
eval echo $cmd
eval $cmd
