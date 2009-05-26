#!/usr/local/bin/bash
#cat $0
echo $Id: povray-launch-stitch.sh,v 1.1 2006/01/31 03:32:25 rcook Exp $
. `dirname $0`/image-common.sh 

function usage () {
    echo usage: povray-launch-stitch.sh numnodes resultname '(-bf)'
    echo '-standby means standby mode'
    echo '-force means force directory overwrite if needed'
    echo 'Environment variables that affect stuff include:'
    echo 'povargs: a single quoted string containing extra args to pass to povray'
    echo 'tpn: tasks per node (default "-2")'
    echo 'timelimit:  defaults to "4h"'
}
if [ x$1 == x ] || [ x$2 == x ] ; then
    echo error: missing argument '(requires 2)'
    usage
    exit 1
fi



getargs $*

[ x$standby == x$true ] && standby='-b'

[ -d $datadir ]  || errexit "datadir $datadir does not exist"
[ ! -d $imgdir ] || mkdir -p $imgdir  || errexit "cannot make image dir $imgdir"

numtasks=`expr $numnodes '*' $tpn`

if [ x$newpov != xfalse ]; then
    combinepov wormparams.inc ${basename}_${step}.pov tmpy.pov || errexit "creating povray file"
fi

cd $imgdir 

${HOME}/bin/pscript.sh $standby -g -k -n $numnodes -t $timelimit -c "srun -W 72000  -n $numtasks /g/g0/rcook/current_projects/paraDIS_hts/BigData-2006/povray-batch.sh -datadir $datadir -imgdir $imgdir -initfile $initfile -width $width -height $height -basename $basename -maxendpointval $maxendpointval"

echo "When the job is finished, use ${imgdir}/stitch.sh $numtasks  to stitch the images together"
