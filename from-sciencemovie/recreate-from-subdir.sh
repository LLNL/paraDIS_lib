#!/usr/local/bin/bash
# recreate an image from a subdirectory of ScienceCover
#$Id: recreate-from-subdir.sh,v 1.4 2006/02/12 17:38:49 rcook Exp $
. $HOME/RC_bash_lib/shellfuncs.sh || exit 1
set -vx

if [ -e /usr/local/bin/python ] ; then 
    python=/usr/local/bin/python
elif [ -e /usr/bin/python ] ; then 
    python=/usr/bin/python
else
    python=python
fi

# full list:
# vasilyrequest=(newfinish greenmonster-nobox-bg testing vasily-gradient-media gradient-green-yellow-white gradient-green-white-texture gradient-green-white-sparkle-media grayfog white2  movieframe892)
# done: greenmonster-nobox-bg   testing grayfog white2  gradient-green-yellow-white gradient-green-white-texture movieframe892 vasily-gradient-media newfinish
# remaining:
# went very fast on pvc with 50 nodes:
vasilyrequest=( greenmonster-nobox-bg   testing grayfog white2  gradient-green-yellow-white gradient-green-white-texture )
# need all of gauss:
vasilyrequest=( newfinish gradient-green-white-sparkle-media vasily-gradient-media )
# after 19 hours has 1/4 to go on 70 gauss processors! 
vasilyrequest=(  gradient-green-white-sparkle-media)
vasilyrequest=(media-light)
height=${height:-4800}
width=${width:-6000}
numprocs=${numprocs:-70}
if [ $numprocs -gt 70 ] ; then
    standby=--standby
fi
# usage:  recreate-from-subdir.sh subdir
# do it
topdir=/g/g0/rcook/current_projects/paraDIS_hts/ScienceCover
for request in ${vasilyrequest[*]}; do
    cd $topdir/$request || errexit "Cannot cd to $topdir/$request"
    # make sure we are using the right version of our files: 
    sedfiles -e "s:/usr/local/bin/python:${python}:" -e "s/height=.*/height=${height}/" -e "s/width=.*/width=${width}/" image-common.sh || errexit "sedfiles failed"
    sedfiles -e "s:wdir=.*:wdir=${topdir}/${request}:" povray-batch.sh || errexit "sedfiles failed"
    chmod u+x  povray-batch.sh image-common.sh || errexit "chmod failed"
    jidfile=.jidfile-$request
    pscript.sh -n ${numprocs} -t 48h -jn "$request" -c "`pwd`/povray-batch.sh" --jidfile $jidfile $standby
    nohup /g/g0/rcook/current_projects/paraDIS_hts/ScienceCover/wait-and-stitch.sh `cat $jidfile` $request `expr $numprocs '*' 2`&
done

