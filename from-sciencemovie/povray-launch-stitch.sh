#!/usr/local/bin/bash
#cat $0
echo $Id: povray-launch-stitch.sh,v 1.8 2005/08/18 21:26:29 rcook Exp $
function usage () {
    echo usage: povray-launch-stitch.sh numnodes resultname '(-bf)'
    echo '-b means standby mode'
    echo '-f means force directory overwrite if needed'
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

set -v
numnodes=$1
result=$2
shift 2
if  echo $* | grep -- -b >/dev/null 2>&1 ; then
    standby='-b'
fi
tpn=${tpn:-2}
numtasks=`expr $numnodes '*' $tpn`
timelimit=${timelimit:-'4h'}
dir=/g/g0/rcook/current_projects/paraDIS_hts/ScienceCover
if [ ! -d ${dir}/$result ] ; then 
    mkdir -p ${dir}/$result || (echo could not make directory ${dir}/${result} -- aborting; exit 1) 
else 
    if echo $* | grep -- -f >/dev/null 2>&1  ; then 
	rm -rf $dir/$result/*
    else
	echo result dir ${dir}/$result already exists -- aborting 
	exit 2
    fi
fi

cp *.sh wormparams.inc generic.ini $result 

cd ${dir}/$result
. ./image-common.sh >/dev/null 2>&1
cp  ../${basename}_${step}.pov .

# cat the files together instead of using #include to speed parsing
rm -f combined.pov
cat wormparams.inc > combined.pov
sed 's/#include "wormparams.inc"//' ${basename}_${step}.pov >> combined.pov

${HOME}/bin/pscript.sh $standby -g -k -n $numnodes -t $timelimit -c "srun -W 72000  -n $numtasks ${dir}/${result}/povray-batch.sh $povargs"

sleep 5
samplelognum=`expr $numnodes / 2`
starttime=`date`
while pstat; do date; echo started at $starttime; echo logfile: ; tail -n 3 .povlog_0.txt ; echo ; echo '*****************' ; echo; tail -n 3 .povlog_${samplelognum}.txt; echo; echo; sleep 5; done


./stitch.sh $numtasks || ( echo stitch failed; exit 1 )

set -v; set -x

mv final.png ${result}.png

set +v; set +x
image=${dir}/${result}/${result}.png
if [ -e $image ] ; then 
    echo image is at ${image}
    display $image ; 
fi&
