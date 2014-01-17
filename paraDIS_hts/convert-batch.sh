#!/usr/local/bin/bash
# A script to convert many many rs%04d.data files (paraDIS vis dumps) to povray format on batch nodes.  To use, just submit with pscript.sh like this: 
# pscript.sh -n 60  -c /nfs/tmp0/rcook/paradis/2005-11/convert-batch.sh -d -k 

. ~/bin/shellfuncs.sh
set -vx
procnum=`getprocnum`
numprocs=`getnumprocs`
logfile=/nfs/tmp0/rcook/paradis/2005-11/data/conversion_$procnum.log
echo logfile is $logfile
rm $logfile
exec >$logfile 2>&1
set -vx
max=${max:-5487}
min=${min:-75}
num=`expr $max - $min`
first=`expr $min + $procnum`

#round robin
datadir=/nfs/tmp0/rcook/paradis/2005-11/pov
if [ ! -d $datadir ] ; then mkdir -p $datadir; fi
cd $datadir
i=$first
while [ $i -le $max ] ; do 
time restartConvert -pov -first $i -last $i -spheres -template /nfs/tmp0/rcook/paradis/2005-11/2005-11-Thunder/rs%04d.data 
let i=$i+$numprocs
done
