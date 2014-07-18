#!/usr/bin/env bash 
# $Id: quicklook.sh,v 1.3 2006/01/20 19:09:03 rcook Exp $
# This script is for quick and dirty looks at the current image, using multires and other speedups
#  

echo '$Id: quicklook.sh,v 1.3 2006/01/20 19:09:03 rcook Exp $'
#echo current shell options are $-
set -v
set -x
wdir=`dirname $0`
. $wdir/image-common.sh
cd $wdir
color=8
echo; echo; echo beginning $0; echo; echo
#sleep 5
echo SLURM_PROCID is $SLURM_PROCID
# cat the files together instead of using #include to speed parsing
rm -f tmpy.pov
cat wormparams.inc > tmpy.pov
sed 's/#include "wormparams.inc"//' ${basename}_${step}.pov >> tmpy.pov
cmd='povray generic.ini declare=wormcolor=${color}  +Itmpy.pov +o${basename}_quickie.png +H${height} +W${width} $debug  +Ft +SP256 +P'
#cmd='povray generic.ini declare=wormcolor=${color}  +I${basename}_${step}.pov +o${basename}_quickie.png +H${height} +W${width} $debug  +Ft +SP256 '
eval echo $cmd
eval $cmd
set +v
set +x
