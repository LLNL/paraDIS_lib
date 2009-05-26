#!bash
#$Id: shellfuncs.sh,v 1.14 2005/08/19 23:43:58 rcook Exp $
# TO DO : 
# boolean support should be added for getargs
# it is not an error to have undefined variables:
set +u

dbg=${dbg:-0}
function dbecho () {
    if [ $dbg -ge $1  ] ; then
	shift
	echo $*
    fi
}

function set_debug () {
    dbg=$1
}

# makerelative: 
# if child begins with '/', then echo $child
# if child does not begin with '/', then echo $parent'/'$child
# (or `pwd` if no parent given)
function makerelative () {
    if [ x$1 = x ] ; then
	echo 'usage: makerelative child-path parent-path'
    fi
    child=$1
    parent=$2
    if [ x$parent = x ] ; then parent=`pwd`; fi

    if echo $child | grep  '^/'; then 
	echo>/dev/null; # child was just echoed!
    else
	echo $parent'/'$child
    fi
}
   
# --------------------------------
# getargs
# usage: 
# to use non-boolean options, set up two arrays:  OPTS_ARRAY and VARS_ARRAY
# to use boolean options, set up two arrays: BOOL_ARRAY and BOOL_VARS_ARRAY
# not setting the arrays means you aren't using them.  Setting both means you use both. 
# Variable names obviously do not have to match the option names.
# then just call getargs  $*
# example:
# from calling script, if you do the following:
# 
# BOOL_OPTS_ARRAY=( -help -exit )
# BOOL_VARS_ARRAY=( help exit )
# OPTS_ARRAY=( -dontchange -first -last -verbose )
# VARS_ARRAY=( dontchange first last verbose )
# getargs "$@"
#
# now if the script is called by 
# script.sh -first 1 -last 2 -ver 3 -help -exit
#
# then when it is done, you will have
# first=1, last=2, and verbose=3
# the boolean variable "help" will be set to "true"
# all other variables will be unchanged
# 
# if REJECT_BAD_ARGS is set, then an unrecognized arg returns 1 from the function.  
# if not, the function silently skips over unknown arguments.  
# Ifbad args are detected, then function returns 1.  Else, return 0. 
# 
function getargs () {
    argarray=("$@")
    numargs=${#argarray[*]}
    numopts=${#OPTS_ARRAY[*]}
    numbools=${#BOOL_OPTS_ARRAY[*]}
    if [ x$numopts = x ] ; then numopts=0; fi
    if [ x$numbools = x ] ; then numbools=0; fi
    argnum=0
    while [ $argnum -lt ${numargs} ] ; do 
	if [ x$REJECT_BAD_ARGS != x ] && 
	    echo ${OPTS_ARRAY[*]} | grep -v -- ${argarray[$argnum]} >/dev/null 2>&1 && 
	    echo ${BOOL_OPTS_ARRAY[*]} | grep -v -- ${argarray[$argnum]} >/dev/null 2>&1 ; 
	then 
	    dbecho 1 Bad argument number `expr $argnum + 1`: \"${argarray[$argnum]}\"
	    return 1
	fi
	# check for bool match.  	
	booloptnum=0
	optnum=0
	while [ $booloptnum -lt ${numbools} ]; do 
	    if [ x${argarray[$argnum]} == x${BOOL_OPTS_ARRAY[$booloptnum]} ] ; then
		cmd="${BOOL_VARS_ARRAY[$booloptnum]}"'=true'
		dbecho 2 `eval $cmd`
		eval $cmd
		# skip the next loop
		optnum=$numopts
		break
	    fi
	    let booloptnum++
	    #If no match, reset optnum to 0.  Else, set to ${numopts}
	done
	while [ $optnum -lt ${numopts} ]; do 
	    if [ x${argarray[$argnum]} == x${OPTS_ARRAY[$optnum]} ] ; then
		let argnum++
		cmd="${VARS_ARRAY[$optnum]}"'=${argarray[$argnum]}'
		dbecho 2 `eval $cmd`
		eval $cmd
		break
	    fi
	    let optnum++
	done
	let argnum++
    done
    return 0
}

function usegetargs () {
    BOOL_OPTS_ARRAY=( -bool1 -bool2 -bool3 )
    BOOL_VARS_ARRAY=( bool1 bool2 bool3 ) 
    OPTS_ARRAY=( -first -last -verbose )
    VARS_ARRAY=( first last verbose )
    getargs "$@"
    for var in first last verbose bool1 bool2 bool3; do 
	cmd="echo $var is "'$'$var
	eval $cmd
    done
}

function testgetargs() {
    for var in first last verbose bool1 bool2 bool3; do 
	cmd="$var="
	eval $cmd
    done
    usegetargs -first 1 -verbose 2 -badarg -bool2
}

# again, we assume an array called VARS_ARRAY is set, as with getargs.  We just echo the values of the variables named therein
function echoargs () {
    for var in ${VARS_ARRAY[*]}; do 
	eval echo $var is '$'$var
    done
}

#========================================
# bool2opt()
# pass the NAME of a bool variable 
# if $varname != 'no', varname="-varname" 
# else varname=''
function bool2opt() {
    if eval [ x'$'$1 == xno ] ; then 
	eval $1=
    else
	eval ${1}='-$1'
    fi
}

# --------------------------------
# length
# usage: length word
# echo the length of $1
function length () {
    len=`echo $1 | wc -m`
    expr $len - 1
}

# --------------------------------
# pad2length
# usage: padzeros num length
# pad shortnum with initial zeros until its length is as long as longnum
function pad2length () {
    output=$1
    shortlen=`length $1`
    longlen=$2
    while [ $shortlen -lt $longlen ] ; do
	output=0$output
	shortlen=`expr $shortlen + 1`
    done
    echo $output
}

# --------------------------------
# padzeros
# usage: padzeros shortnum longnum 
# pad shortnum with initial zeros until its length is as long as longnum
function padzeros () {
    longlen=`length $2`
    pad2length $1 $longlen
}

# --------------------------------
# getprocnum
# echo our SLURM_PROCID (which is 0 based)
# I wrote this mainly to document where to get this info
# only works on SLURM machines.  
function getprocnum () {    
    eval 'procnum=${SLURM_PROCID}'
    if [ x$procnum == x ] ; then 
	# we are on a serial machine
	procnum=0
	# we have to assume that the user ran movie_setup.sh earlier
	#echo "Error:  unable to get SLURM_PROCID. Please be sure you are running this on a batch node under SLURM." > /dev/stderr
	#return 1

	# someday, write a code that gets procnum for non-slurm (i.e., AIX) hosts.  Since I usually just do viz work, the hell with that for now. 
    fi
    echo $procnum
    return 0
}


# --------------------------------
# getnumprocs
# return the total number of processors in the current batch job -- useful to determine chunk sizes in conjunction with getprocnum above
# only works on SLURM machines.  
function getnumprocs () {
    eval 'numprocs=${SLURM_NPROCS}'
    if [ x$numprocs == x ] ; then 
	# we are not on a batch node:
	numprocs=1

	# we have to assume that the user ran movie_setup.sh earlier
	#echo "Error:  unable to get SLURM_PROCID. Please be sure you are running this on a batch node under SLURM." > /dev/stderr
	#return 1

	# someday, write a code that gets procnum for non-slurm (i.e., AIX) hosts.  Since I usually just do viz work, the hell with that for now. 
    fi
    echo $numprocs 
    return 0
}
