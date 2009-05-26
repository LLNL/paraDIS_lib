#!/bin/env bash
#. ~/.profile
# $Id: import-restart.sh,v 1.36 2007/03/16 22:26:25 rcook Exp $ 
# $File$

SYS_TYPE=${SYS_TYPE:-$(uname)}

# the purpose of this script is to convert restart files into a registry file
function announce_self() {
    echo
    echo import-restart.sh '($Id: import-restart.sh,v 1.36 2007/03/16 22:26:25 rcook Exp $)' on `date` called on machine `uname -n` as $0 $* 
    echo
}
announce_self $*

if [ -e `dirname $0`/shellfuncs.sh ] ; then 
    . `dirname $0`/shellfuncs.sh
else
    . `dirname $0`/RC_bash_lib/shellfuncs.sh
fi

# note:  cannot set to "unlimited"  must use number <=2000000 (two million)
#ulimit -c unlimited
ulimit -c 2000000

first=1
help=no
last=1001
logfile=
drawonly=1
step=10
registry=`date +%Y-%m-%d`.reg
spheres=no
sphereradius=25
cylradius=10
htsname=import
neighbors=0
tmpdir='.'
verbose=0
oldstyle=no
keepfiles=yes
inputtemplate='restart-files/rs%04d'
converter=`dirname $0`/bin/${SYS_TYPE}/restartConvert
skiphts=no
butterflies=no
monsters=no
minval=1
maxval=8
armstats=
filterfile=
simplearms=
totalview=
xmin=
xmax=
ymin=
ymax=
zmin=
zmax=

function usage () {
    echo '*************************************************'
    echo Help Menu
    echo "usage: import-restart.sh [options]"
    echo "options are processed in order encountered, defaults are given in parenthesis"
    echo " -armfilter filename: only process the arm numbers given in named file"
    echo " -armstats filename: put armstats in filename"
    echo " -butterflies yes/no (no): look for butterflies"
    echo " -cylradius num (10): use num as cylinder radius"
    echo " -drawonly nth (1): If nonzero, then draw only every nth cylinder (to create a sparse scene for speed rendering)"
    echo " -first num (1): first restart file number"
    echo " -help:  print this menu and exit"
    echo " -htsname name (import): use the given name as a base for naming imported hts files"
    echo " -inputtemplate template (rs%04d): look for input files using given C-style format as filename pattern.  Example: /tmp/rs%04d"
    echo " -keep yes/no (yes): keep temporary hts files around in temp space (does not affect registry)"    
    echo " -last num (1): last restart file number"
    echo " -minval num (1): tell TSB to use num as minimum data value"
    echo " -maxval num (8): tell TSB to use num as maximum data value"
    echo " -monsters yes/no (no): output all arms that are terminated at either end by a four-armed or higher monster, or which are terminated at both ends by three armed nodes which are each the ends of a butterfly body"
    echo " -registry regname (`date +%Y-%m-%d`.reg): import to registry \"regname\""
    echo " -converter appname (restartConvert): use appname to do conversion"
    echo " -scratch dirname (.):  use dirname/tmp as scratch directory" 
    echo " -skiphts yes/no (no):  if yes, then do not convert to hts, only register hts files"
    echo " -oldstyle yes/no (no)  if yes, then input files are in old-style format"
    echo " -spheres yes/no (no)  if yes, output spheres as well as cylinders"
    echo " -sphereradius num (25)  use num as sphere radius"
    echo " -step num (10):  restart file step size"
    echo " -v num:  set verbosity to num (0 is quiet, 5 is very very high)"
    echo " -simplearms   if this switch is present, run restartConvert with -simplearms flag, which generates straight, simple arms without any interior segments"
    echo " -tv   if this switch is present, run restartConvert under TotalView"
    echo " -xmin/xmax/ymin/ymax/zmin/zmax limit: set scene bounds (do not draw beyond given limit)"
    echo ' -neighbors num (0):  The -neighbors num option controls output of worms based on the number of neighbors of nodes.  The argument 'num' has the following meaning.  Note that nodes are only drawn if -spheres is given as an argument. \
    We define an 'interior node' as a node having exactly two neighboring nodes, and an 'interior segment' as a segment with at least one interior node as its endpoints.  The vast majority of nodes and segments will be of this type.  'Terminal nodes' are all non-interior nodes, and 'terminal segments' are non-interior segments.  An 'arm' is a series of connected interior segments and nodes and the 0, 1 or 2 terminal nodes attached thereto.\
    num == 0 means output ALL worms and nodes  (if -spheres is given) (no filtering).  This is the default.\
    num == -1:  this is meaningless and will result in an error. \
    num == -2:  output all 'interior' segments and nodes (if -spheres given)\
    num < -2:  output all terminal nodes  (if -spheres given) with exactly abs(num) neighbors, and all arms connected to such nodes. \
    num == 1:  this is meaningless and will result in an error. \
    num > 1:  Act as if 0, -2, ... -n were given in separate invocations (which would be much slower, but have the same effect).\
    For example, a value of 3 yields a worm file with all worms in it, a file with only worms attached to single-neighbored nodes, and a file with only worms attached to nodes with two neighbors.'
    echo '*************************************************'
    echo
}

OPTS_ARRAY=( -armfilter -armstats -butterflies -converter -cylradius -first -h --help -help -htsname -inputtemplate -keep -last -logfile -minval -maxval -monsters -neighbors -oldstyle -registry -scratch -skiphts -sphereradius -spheres -step -v -xmin -xmax -ymin -ymax -zmin -zmax )
VARS_ARRAY=( filterfile armstats butterflies converter cylradius first help help help htsname inputtemplate keepfiles last logfile minval maxval monsters neighbors oldstyle registry tmpdir skiphts sphereradius spheres step verbose xmin xmax ymin ymax zmin zmax )
BOOL_OPTS_ARRAY=( -simplearms -tv )
BOOL_VARS_ARRAY=( simplearms totalview )
getargs "$@"

if  [ x$help != xno ] ; then 
    usage; exit 0
fi
if [ x$logfile != x ] ; then
    exec >$logfile 2>&1 
    announce_self $*
fi
set_debug $verbose
varval='$var'
for var in ${VARS_ARRAY[*]}; do     
    cmd="dbecho 1 $var is \$$var"
    eval $cmd
done

bool2opt butterflies
bool2opt oldstyle
bool2opt monsters
bool2opt spheres
bool2opt simplearms

for var in xmin xmax ymin ymax zmin zmax ; do
    eval value='$'$var
    # if xmin != "", then set xmin="-xmin 15"
    if [ x$value != x ] ; then
	eval "$var=\"-$var "'$'"$var\""
    fi
done


if testbool $totalview; then 
    totalview=totalview
    totalviewarg="-a"
else
    totalview=
    totalviewarg=
fi

if [ x$drawonly == x1 ] ; then 
    drawonly=
else
    drawonly="-drawonly $drawonly"
fi

if [ ! -d $tmpdir ] ; then mkdir -p $tmpdir || ( echo "cannot make temp dir $tmpdir"; exit 1); fi
if [ x$armstats != x ] ; then armstats="-armstats -statsfile $armstats" ; fi
if [ x$filterfile != x ] ; then filterfile="-armfilter $filterfile" ; fi

#increaseradius='-increaseradius'

#================================================================
# convert to HTS
set -x
set -v
if [ x$skiphts != xyes ] ; then     
    echo conversion begun at `date`
    if ! $totalview $converter $totalviewarg -hts -maxtriangles 100000 -first $first -last $last -step $step -v $verbose -neighbors $neighbors $spheres -template $inputtemplate -name $tmpdir/converted -sphrad $sphereradius -cylrad $cylradius $increaseradius $monsters $butterflies  $oldstyle $armstats $filterfile $drawonly $xmin $xmax $ymin $ymax $zmin $zmax $simplearms; then 
	echo error in conversion step
	exit 1
    fi
fi

#================================================================
#import to registry

function register () {
    echo function register $* called at `date`
    echo registering cylinders at `date`
    if ! registerHTSFile -v $verbose -attmin $minval -attmax $maxval -label $htsname-cyls$1 $registry $tmpdir/converted_*_cyls$1.hts ; then 
	echo error registering hts cylinder files
	return 1
    fi
    if [ x$spheres != x ] ; then 
	echo registering spheres at `date`
	if ! registerHTSFile -v $verbose -attmin $minval -attmax $maxval -label $htsname-spheres$1 $registry $tmpdir/converted_*_spheres$1.hts ; then 
	    echo error registering hts sphere files
	    return 1
	fi
    fi
    if [ x$keepfiles == xno ] ; then 
	rm -f $tmpdir/converted_*_cyls$1.hts $tmpdir/converted_*_spheres$1.hts
    fi
}

#for each neighbor num, import cyls and spheres
if [ x$butterflies != x ] ; then
    echo registering butterflies at `date`
    if ! register _butterflies; then
	echo could not register butterflies
	exit 1 
    fi
fi
if [ monsters != x ] ; then
    echo registering monsters at `date`
    if ! register _monsters; then 
	echo could not register monsters
	exit 2
    fi
fi

num=$neighbors
echo registering neighbors at `date`
if [ $num -lt 0 ] ; then
    num=`expr -1 '*' $num`
    if ! register _$num; then 
	echo failed to register num $num
	exit 3
    fi
else
    # zeroth case:
    #register
    current=0
    while [ $current -le $num ]; do
	if [ $current != 1 ] ; then 
	    if  ! register _$current; then 
		echo failed to register number $current
		exit 4
	    fi
	fi
	let current++
    done
fi

echo adding registerbounds at `date`
if ! addregbounds $registry `$converter -getbounds -first $first -last $last -step $step -template $inputtemplate $oldstyle`; then 
    echo addregbounds failed
    exit 1
fi
echo done with import-restart.sh
exit 0
set +v 
set +x

