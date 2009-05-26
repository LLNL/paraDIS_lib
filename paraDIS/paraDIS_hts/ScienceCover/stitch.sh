#!/usr/local/bin/bash
# $Id: stitch.sh,v 1.5 2005/02/14 18:47:30 rcook Exp $
# stitch together images made with povray-batch.sh
echo '$Id: stitch.sh,v 1.5 2005/02/14 18:47:30 rcook Exp $'
. ${HOME}/current_projects/RC_bash_lib/shellfuncs.sh
. image-common.sh
#set -x; set -v; 
# level starts at zero and may be no more than log base 4 of numprocs
if [ x$1 == x ] ; then echo usage: stitch.sh numprocs; exit 1; fi


#set +x; set +v; 

# COMPOSITING TREE FOR BATCH PROCESSING!  :-)  
# procnum is always zero-based, as are filenum, etc. 

export chunksize=4



#=================================
# for now, just stitch and exit

stitch $width $height $1 
exit $?

# usage: stitch width height firstfilenum lastfilenum numfiles
function stitch () {
    pbmmake -black $1 $2 > base.pbm
    cmd='convert ' 
    filenum=$3
    last=$4
    while [ $filenum -le $last ] ; do
	start=`startrow $2 $i $3`
	let start--
	name=`povname $i $basename`
	cmd=${cmd}"-draw 'image Over 0,$start ${1},${2} $name' "
	let filenum++
    done
    cmd=${cmd}"base.pbm final.png"
    echo "$cmd" 
    eval $cmd
}

# get_firstnum tasknum numtasks globalfirst globallast
# tasknum and numtasks refer to current batch job
# global first and last is the file number range
function get_firstnum () {
    filenum=`expr $1 * $chunksize`
    if [ $filenum -ge $numprocs ] ; then 
	$filenum=`expr $numprocs - 1`
    fi
    echo $filenum
}
# get_lastnum tasknum numtasks
function get_lastnum () {
    last=`expr ( $1 + 1 ) * $chunksize - 1`
    if [ $last -ge $numprocs ] ; then 
	last=`expr $numprocs - 1`
    fi
    echo $last
}

# list all input files for current stitch level
# usage list_input_files basename level procnum numprocs 
function list_input_files () {
    basename=${1}
    level=$2
    procnum=$3
    numprocs=$4

    basename=${basename}_level${level}_
    filenum=`expr $procnum * $chunksize`
    last=`expr $filenum + $chunksize - 1`
    if [ $last -ge $numprocs ] ; then 
	last=$numprocs
    fi
    files=''
    while [ $filenum -lt $last ] ; do
	files=$files" $basename$filenum"
	let filenum++
    done

}

# usage name_output_file basename level procnum 
function name_output_file () {
    basename=$1
    level=$2
    procnum=$3
    outlevel=`expr $level + 1`
    echo ${basename}_level${level}_${procnum}
}

# return 0 unless all files are ready to go
function inputs_not_ready() {
    basename=${1}
    level=$2
    procnum=$3
    numprocs=$4
    for file in `list_input_files $basename $levl $procnum $numprocs`; do if [ ! -f $file ] ; then return 0; fi; done
    return 1
}

# for the current level, check for input files, then
# stitch them into a temp file, then move the temp file to the output file
# return 0 if successful, 1 if not
function stitch_level () {
    cmd='stitch $width $height $firstnum $lastnum'
    for file in `list_input_files $basename $levl $procnum $numprocs`; do 
	
    done
    
    return 1
}

# outer loop; create the tree
function stitch-batch () {
    level=0;
    # loop while there is work to do
    numprocs=`getnumprocs`
    while [ x"`list_input_files $level $numprocs" != x ] ; do 
	while inputs_not_ready; do sleep 5; done 
	if stitch_level $level $numprocs; then 
	    echo level $level SUCCESS
	else
	    echo level $level FAILED
	    return 1
	fi
	let level++
    done
    echo "done with composite tree"
    return 0
}
