#!/usr/bin/env bash
# $Id: image-common.sh,v 1.7 2006/02/27 16:40:50 rcook Exp $
# functions for decomposing an image according to the processor being run in batch

height=960
width=1280
#height=2100
#width=2800
#height=1920
#width=2560
#height=960
#width=1280
#height=480
#width=640
#height=240
#width=320
#height=120
#width=160
export step=5300
export basename=${basename:-worms-5300-2004-04-04}
#export basename=worms-2712-2004-04-04
#export basename=boxed-5300-2004-04-04
#export basename=boxed
#export basename=${basename:-tester}
export verbose=5
#debug=Declare=debug=yes
export armweight='-armweight 0.2'
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

#usage:  doconvert (args are taken from environment for now) 
function doconvert () {
    cmd='../restartConvert -pov -template rs%04d.data -first $step -name ${basename} -v $verbose -spheres $armweight'
    eval echo $cmd
    eval $cmd >${basename}.log 2>&1
}

# usage:  povname procnum basename 
# echoes the povray filename (used for consistent naming between scripts)
function povname () {
    echo ${2}_${1}.tga
}


# usage: stitch width height numprocs firstproc
function stitch () {
    pbmmake -black $1 $2 > base.pbm
    cmd='convert ' 
    if [ x$4 != x ]; then i=$4; else  i=0; fi
    while [ $i -lt $3 ] ; do
	start=`startrow $2 $i $3`
	let start--
	name=`povname $i $basename`
	cmd=${cmd}"-draw 'image Over 0,$start ${1},${2} $name' "
	let i++
    done
    cmd=${cmd}"base.pbm final.png"
    echo "$cmd" 
    eval $cmd
}

export procnum=`getprocnum` 2>/dev/null
export numprocs=`getnumprocs` 2>/dev/null
