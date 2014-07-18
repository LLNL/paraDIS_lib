#!/usr/local/bin/bash 
# $Id: quicklook.sh,v 1.1 2006/01/31 03:32:25 rcook Exp $
# This script is for quick and dirty looks at the current image, using multires and other speedups
#  

echo '$Id: quicklook.sh,v 1.1 2006/01/31 03:32:25 rcook Exp $'
#echo current shell options are $-
. `dirname $0`/image-common.sh

getargs "$@"

if testbool $debug ; then  
    debug="declare=debuglookat=true"
fi

color=8
echo; echo; echo beginning $0; echo; echo
#sleep 5
# cat the files together instead of using #include to speed parsing
cd $datadir || errexit "cannot cd to $datadir"
[ ! -d $imgdir ] || mkdir -p $imgdir || errexit "cannot make image dir $imgdir"
[ x$newpov == xfalse ] || combinepov wormparams.inc ${basename}_${step}.pov tmpy.pov || errexit "creating povray file"

echo
echo
echo '************************'
date
echo
echo
set -vx
$povray $initfile declare=wormcolor=${color}  +I$datadir/tmpy.pov +o$imgdir/${basename}_quickie.tga +H${height} +W${width} $debug  +Ft +SP256 +P declare=maxendpointval=$maxendpointval 
#cmd='povray generic.ini declare=wormcolor=${color}  +I${basename}_${step}.pov +o${basename}_quickie.png +H${height} +W${width} $debug  +Ft +SP256 '
set +vx

