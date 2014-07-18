#!/usr/local/bin/bash
# $Id: image-common.sh,v 1.2 2006/01/31 04:35:02 rcook Exp $
# functions for decomposing an image according to the processor being run in batch
. $HOME/RC_bash_lib/shellfuncs.sh

BOOL_OPTS_ARRAY=( -debug -force -newpov -standby )
BOOL_VARS_ARRAY=( debug force newpov standby )
OPTS_ARRAY=( -basename -datadir -height -imgdir -initfile -maxendpointval -numfiles -numnodes -povray -time -tpn -v -width )
VARS_ARRAY=( basename datadir height imgdir initfile maxendpointval numfiles numnodes povray time tpn verbose width )

function errexit() {
    echo $1
    exit ${2:-1}
}

force=false
timelimit=16h
tpn=2
numnodes=4

procnum=`getprocnum`
numprocs=`getnumprocs`
# reasonable defaults
#height=2100
#width=2800
#height=1920
#width=2560
height=960
width=1280
height=480
width=640
#height=240
#width=320
#height=120
#width=160
datadir=.
debug=
imgdir=.
maxendpointval=50
newpov=false
povray=povray
step=242
basename=converted_restart
verbose=5
initfile=bigdata.ini
numfiles=1

if [ -e /usr/global/tools/povray/3.6.1/$SYS_TYPE/bin/povray ] ; then 
    povray=/usr/global/tools/povray/3.6.1/$SYS_TYPE/bin/povray
fi


# to adjust the length of arm that is colored white: 
maxendpointval=${maxendpointval:-50}

#debug=Declare=debug=yes
armweight='-armweight 0.2'
#procnum=2
#numprocs=6

. ${HOME}/current_projects/RC_bash_lib/shellfuncs.sh

# usage: startrow height procnum numprocs
# procnum is 0-based, just SLURM_PROCID
function startrow () {
    /usr/local/bin/python -c "print (int) (${1}.0 * (${2}.0)/${3}.0) + 1" 
}
# usage: endrow height procnum numprocs
function endrow () {
    /usr/local/bin/python -c "print (int) (${1}.0 * (${2}.0+1)/${3}.0)"     
}
#startrow 100 1 5  (answer:  1)
#startrow 100 3 5  (answer:  41)
#endrow 100 3 5 ( answer:  60)
#endrow 100 5 5 ( answer:  100)

# combinepov:  combine an include file and a povray file to an output file
# usage: makepov  incfile povfile outfile
function combinepov  () {
    incfile=$1
    povfile=$2
    outfile=$3
    rm -f $outfile
    cat $incfile > $outfile
    sed 's/#include "wormparams.inc"//' $povfile >> $outfile
}

#usage:  doconvert (args are taken from environment for now) 
function doconvert () {
    cmd='../restartConvert -pov -template rs%04d.data -first $step -name ${basename} -v $verbose -spheres $armweight'
    eval echo $cmd
    eval $cmd >${basename}.log 2>&1
}

# usage:  povname 
# echoes the povray filename (used for consistent naming between scripts)
function povname () {
    echo $datadir${basename}_${procnum}.pov
}


# usage: stitch width height numprocs firstproc
function stitch () {
    cd $imgdir
    width=$1
    height=$2
    numfiles=$3
    startnum=${4:-0}
    pbmmake -black $width $height > base.pbm
    cmd='convert ' 
    i=$startnum
    while [ $i -lt $numfiles ] ; do
	start=`startrow $height $i $numfiles`
	let start--
	name=${basename}_$i
	# repair the broken tga file:
	convert ${name}.tga ${name}.png 
	#rm ${name}.tga
	cmd=${cmd}" -draw 'image Over 0,$start ${width},${height} ${name}.png' "
	let i++
    done
    cmd=${cmd}" base.pbm final.png"
    echo "$cmd" 
    eval $cmd
    return $?
}

procnum=`getprocnum` 2>/dev/null
numprocs=`getnumprocs` 2>/dev/null
