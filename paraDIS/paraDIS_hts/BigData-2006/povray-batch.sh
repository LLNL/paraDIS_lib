#!/usr/local/bin/bash 
# $Id: povray-batch.sh,v 1.1 2006/01/31 03:32:25 rcook Exp $
# This script is intended to be run from pscript.sh, but for some reason does not pick up SLURM_PROCID when run that way.  It seems to work from a batch xterm, though. 
#  
set +ue
. `dirname $0`/image-common.sh
echo '$Id: povray-batch.sh,v 1.1 2006/01/31 03:32:25 rcook Exp $'
#echo current shell options are $-

getargs "$@"

if testbool $debug ; then  
    debug="declare=debuglookat=true"
fi

cd $datadir || errexit "cannot cd to $datadir"
[ ! -d $imgdir ] || mkdir -p $imgdir || errexit "cannot make image dir $imgdir"

exec >$imgdir/.povlog_${procnum}.txt 2>&1
set -vx
echo '$Id: povray-batch.sh,v 1.1 2006/01/31 03:32:25 rcook Exp $'
echo running on host `uname -n`

color=8
startrow=`startrow $height $procnum $numprocs`
endrow=`endrow $height $procnum $numprocs`

[ x$newpov == xfalse ] || combinepov wormparams.inc ${basename}_${step}.pov tmpy.pov || errexit "creating povray file"

cd $datadir
#cmd='povray generic.ini declare=wormcolor=${color} +Icombined.pov +o`povname $procnum $basename` +H${height} +W${width} $debug +SR${startrow} +ER${endrow} +Ft -D $*'
set -vx
$povray $initfile declare=wormcolor=${color}  +I$datadir/tmpy.pov +o$imgdir/${basename}_${procnum}.tga  +H${height} +W${width} $debug  +SR${startrow} +ER${endrow} +Ft declare=maxendpointval=$maxendpointval -D 
set +vx
