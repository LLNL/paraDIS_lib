#!/bin/bash
# This script slices out 1/nth of the data, where n is the number of procs in the current batch environment.  It could make a povray file and a registry, although povray is turned off right now.  
. ~/RC_bash_lib/shellfuncs.sh

slice=${slice:-`getprocnum`}
globalxmin=-20000
numslices=${numslices:-`getnumprocs`}
slicesize=`expr 40000 / $numslices`
verbosity=2
mkdir -p $outdir
cd $outdir
#while [ $slice -lt $maxslice ] ; do
    outdir=${1:-/nfs/tmp0/rcook/paradis/2006-01-bigdata/${numslices}_slices}
    slicedir=$outdir/slice_$slice
    rm -rf $slicedir
    mkdir -p $slicedir || errexit "creating directory $slicedir"
    cd $slicedir
    exec >logfile.txt 2>&1
    echo $0 called as $* on `date`
    set -vx
    xmin=`expr $globalxmin + $slicesize \* $slice`
    xmax=`expr $xmin + $slicesize`
    #restartConvert -pov -first 242 -last 242 -spheres -template /nfs/tmp0/rcook/paradis/2006-01-bigdata/rs%04d.data -v $verbosity -xmin $xmin -xmax $xmax >povlog.txt 2>&1
    import-restart.sh  -first 242 -last 242 -step 1 -inputtemplate /nfs/tmp0/rcook/paradis/2006-01-bigdata/rs%04d.data -registry slice_${slice}_of_${numslices}.reg -monsters yes -neighbors 4 -spheres yes -keep no -v $verbosity -logfile import-restart.log  -xmin $xmin -xmax $xmax -nodrawnstats yes -maxtriangles 100000 -hts  || errexit "running import-restart.sh"
# xmin=$xmax
#    let slice++
#done
