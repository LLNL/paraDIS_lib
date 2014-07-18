#!/usr/local/bin/bash
# purpose:  wait until the given job id terminates, then call stitch.sh in the given directory. 

# one of the most useful things I ever fucking wrote: 
. $HOME/RC_bash_lib/shellfuncs.sh

secs=${secs:-60}

function usage() {
    echo "wait-and-stich.sh jobid dirname numimages"
}

[ x$3 != x ] || ( usage; errexit "insufficient args" )

jid=$1
dir=$2
numprocs=$3
# by default, we're in ScienceCover directory
if echo $dir | grep -v '/' >/dev/null 2>&1; then
    dir=/g/g0/rcook/current_projects/paraDIS_hts/ScienceCover/$dir
fi

date
echo $0 $* 
set -vx
cd $dir || errexit "cannot change to dir $dir"
exec >wait-and-stitch.log 2>&1

date
echo $0 $* 
while pstat -n $jid >/dev/null 2>&1; do
    echo still waiting for job $jid at `date`
    echo sleeping another $secs seconds
    sleep $secs 
done
sedfiles -e "s:\..*image-common.sh:. $dir/image-common.sh:" stitch.sh
chmod u+xrw stitch.sh
./stitch.sh $numprocs
mv final.png `basename $dir`.png
echo Finished with $dir on `date` | /bin/mail -s "wait-and-stitch.sh finished with $dir" rcook@llnl.gov
