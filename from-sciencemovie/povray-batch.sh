#!/usr/local/bin/bash 
# $Id: povray-batch.sh,v 1.7 2005/03/19 02:03:58 rcook Exp $
# This script is intended to be run from pscript.sh, but for some reason does not pick up SLURM_PROCID when run that way.  It seems to work from a batch xterm, though. 
#  
set +u
set +e
set -v; set -x
echo '$Id: povray-batch.sh,v 1.7 2005/03/19 02:03:58 rcook Exp $'
#echo current shell options are $-

wdir=`dirname $0`
cd $wdir
. ./image-common.sh
exec >.povlog_`getprocnum`.txt 2>&1
color=8
echo; echo; echo beginning povray-batch.sh; echo; echo
sleep 5
echo SLURM_PROCID is $SLURM_PROCID
startrow=`startrow $height $procnum $numprocs`
endrow=`endrow $height $procnum $numprocs`
echo povargs are $povargs


cmd='povray generic.ini declare=wormcolor=${color} +Icombined.pov +o`povname $procnum $basename` +H${height} +W${width} $debug +SR${startrow} +ER${endrow} +Ft -D $*'
eval echo $cmd
eval $cmd

set +v; set +x
