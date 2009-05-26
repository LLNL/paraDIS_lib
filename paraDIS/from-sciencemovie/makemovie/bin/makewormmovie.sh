#!/usr/local/bin/bash
# $Id: makewormmovie.sh,v 1.6 2005/11/09 18:03:37 rcook Exp $
# This script generates the frames for a paraDIS movie given a set of povray files.  It can be run in parallel or serial, and will use SLURM as needed to determine its rank.  It assumes the existence of a common include file, by default called "wormparams.inc" which influences the behavior of each povray file, setting the camera, etc.  
#set -e
#set -x
#set -u
. $HOME/RC_bash_lib/shellfuncs.sh || ( echo cannot source shellfuncs.sh;  exit 1)
echo $0 is called at `date` with args $*

# basic information
basedir=`pwd`
bindir=`dirname $0`

# general program behavior and parallelism
logfile=false
parallel=false
renderoffscreen=false
scriptname=''
# frame and file numbers are one-based
firstframe=1
lastframe=

# spline control
prologframes=100
animstart=0.3
animend=0.7
epilogframes=100

# data information
povname='converted_restart'
padlength=0
nopadding=false
datadir='/nfs/tmp0/rcook/paradis/2005-11/pov'
verbose=0
# frame and file numbers are one-based
#povfirst=75
#povlast=5487
povfirst=75
povlast=674
povstep=1

# image settings
incfile="/nfs/tmp0/rcook/paradis/2005-11/movie/wormparams.inc"
lowquality=false
height=480
width=640
wormcolor=2
fog=false
media=false
#parallelism
mproc=`uname -n`
procnum=${procnum:-`getprocnum`}
numprocs=${numprocs:-`getnumprocs`}
box=false
backdrop=false

# debugging
axes=false
debug=1
preview=false
campath=false
spline=

# ========================================================
# parse command line 
. $HOME/RC_bash_lib/shellfuncs.sh || (echo could not source shellfuncs.sh; exit 1)
BOOL_OPTS_ARRAY=( -axes -box -backdrop -campath -fog -lowquality -media -offscreen -nopadding -parallel -preview )
BOOL_VARS_ARRAY=( axes box backdrop campath fog lowquality media renderoffscreen nopadding parallel preview )
OPTS_ARRAY=( -datadir -firstframe -lastframe -incfile -prologframes -epilogframes -animstart -animend -povfirst -povlast -povstep -padlength -spline -height -width -logfile -moviename -moviedir -povname -v -wormcolor )
VARS_ARRAY=( datadir firstframe lastframe incfile prologframes epilogframes animstart animend povfirst povlast povstep padlength spline height width logfile moviename moviedir povname verbose wormcolor )

function usage () {
    echo '*************************************************'
    echo Help Menu
    echo "usage: $0 [options]"
    echo "options are processed in order encountered, defaults are given in parenthesis.  "
    echo "  -help: if -help is given, then show this menu" 
    echo "  -axes (off): if axes is given, an axis will be drawn at the look-at point"
    echo "  -campath (off): if campath is given, the image will contain a rendering of the camera path through space from clock=0 to clock=1.0"
    echo "  -firstframe num (1): after computing total frames, start at this frame number (useful for restarting in the middle of a movie)"
    echo "  -prologframes num (100): generate 100 frames for the prolog sequence"
    echo "  -animstart fraction (0.33): end the prolog at fraction and begin animating through time until epilog beginning"
    echo "  -animend fraction (0.67): end the animation through time at fraction and begin the epilog with the final time"
    echo "  -spline num (not set): if given (use between 0.0 and 1.0), this value will be used for the camera spline value at every frame.  This is primarily for debugging, obviously." 
    echo "  -epilogframes num (100): generate indicated number of epilog frames"
    echo "  -inifile name (wormparams.inc): use name for initfile"
    echo "  -povfirst num (1): use num as first input file number"
    echo "  -povlast num (399): use num as last input file number"
    echo "  -povstep num (1): step file numbers by num"
    echo "  -padlength num (1): every povray file ends in a number num digits long, e.g., if padlength=4, then the first file might be called converted_restart_0001, etc. Otherwise, padding will be fit to the largest number.  e.g., if the last povnum is 400, then padlength will be 3, so the first file will be converted_restart_001, the 99th file is converted_restart_099, etc."
    echo "  -nopadding (off): if nopadding is given, then povray file numbers will assumed to be not padded by leading zeros (see -padlength)"
    echo "  -lowquality (off): render at low quality (no media, no anti-aliasing, etc)"
    echo "  -fog (off): render with fog"
    echo "  -media (off): render enclosing box with media"
    echo "  -height (640): set image height to num pixels" 
    echo "  -width (480): set image width to num pixels" 
    echo "  -logfile name (none): use name for logfile"
    echo "  -datadir name (/nfs/tmp0/rcook/paradis/2005-11/pov): directory where pov files live" 
    echo "  -povname name (converted_restart): prefix for pov files" 
    echo "  -moviename name (worms): use name for movie frames" 
    echo "  -moviedir dir (images): put images in named directory (create if nonexistent)" 
    echo "  -parallel (off): if given, this script is being run in parallel. This means the script will generally do less than the whole problem, assuming others are also working.  This is done in round-robin fashion based on number of processors detected by SLURM or movie_setup.sh scripts. "
    echo "  -offscreen (off): render to files offscreen, don't show images" 
    echo "  -preview (off): if an image is shown, use povray's preview mode to progressively render it for speed" 
    echo "  -v num (0): run at verbosity level num (5 is high, 0 is off)"
    echo "  -wormcolor num (0): rendering style for worms"
   echo '*************************************************'
    echo
}

if echo $* | grep -- -help >/dev/null; then
    usage
    exit 0
fi

getargs $*
if [ $verbose -ge 4 ] ; then 
    set -xv
fi
#set -e
# ===================================================
# setup needed directories
moviename=${moviename:-images-$width-$height}
moviedir=${moviedir:-/nfs/tmp0/rcook/paradis/2005-11/movie/$moviename}

incfile=`fullpath $incfile`
moviedir=`fullpath $moviedir`
datadir=`fullpath $datadir`
if [ ! -e $moviedir ] ; then 
    mkdir -p $moviedir
    if [ ! -e $moviedir ] ; then 
	echo "cannot make moviedir $moviedir"
	exit 1
    fi
fi

# =====================================================
# padding
echo nopadding is $nopadding
if [ x$nopadding = xtrue ] ; then
    padlength=0
elif [ $padlength = 0 ] ; then
    padlength=`length $povlast`
fi

# ========================================================
# figure out our process number (rank)
if [ x$parallel = xtrue ] ; then
    renderoffscreen=true


    # if we are in parallel, we must spit stuff out to a logfile
    if [ $logfile = false ] ; then
	if [ ! -d  ${moviedir}/logfiles ]; then mkdir -p ${moviedir}/logfiles; fi
	logfile=${moviedir}/logfiles/${moviename}_${procnum}.log
    else
	logfile=${logfile}_${procnum}.log
    fi
    echo logfile is $logfile

    echo sending parallel process output to logfile $logfile
    echo '' > $logfile
    exec >>$logfile 2>&1
    
    

elif [ $logfile != false ] ; then
    echo "Per request, logfile is ${logfile}"
    echo '' > $logfile
    exec >>$logfile 2>&1
fi

echo Begin logging at `date`
echo $0 run with args $*
[ $firstpov -le $lastpov ] || errexit "firstpov must be less than or equal to lastpov"
numfiles=`expr \( $povlast '-' $povfirst + 1 \) / $povstep`
echo Global file range is from $povfirst to $povlast with step $povstep, making $numfiles povray files

if [ $procnum = '0' ] ; then
    echo 'running in serial'
else
    echo I am proc number \"$procnum\" out of \"$numprocs\" total procs
fi

# =====================================================
if [ $animstart = $animend ] ; then
    animframes=0
else
    animframes=$numfiles
fi

numframes=`expr $prologframes + $animframes + $epilogframes`

if [ x$lastframe = x ] ; then lastframe=$numframes; fi

echo "There are $numframes frames overall including $prologframes prolog frames and $epilogframes epilog frames and $animframes animation frames (one per file during animation sequence).  The last frame rendered with be number $lastframe."
firstframe=`expr $firstframe + $procnum`
echo "First frame for me is number $firstframe."


# =====================================================
# set up the spline
# divide into three pieces: 

# [ 0,  $animstart )
prologstep=0.0
if [ $prologframes -gt 0 ] ; then
    # divide [ 0,  $animstart ) into $prologframes sections:
    prologstep=`/usr/local/bin/python -c "print ($animstart)/$prologframes"`
fi
echo "prologstep is $prologstep"

# [ $animstart, $animend ]
animstep=0.0
if [ $animframes -gt 0 ] ; then
    # divide into n-1 sections to get n frames
    if [ $animframes -gt 1 ] ; then 
	animstep=`/usr/local/bin/python -c "print ($animend-$animstart)/($animframes-1)"`
fi
    else 
	animstep=0
    fi
echo "animstep is $animstep"

# ( $animend , 1.0 ]
epilogstep=0.0
if [ $epilogframes -gt 0 ] ; then
    # divide   into $prologframes sections:
    epilogstep=`/usr/local/bin/python -c "print (1.0-$animend)/$epilogframes"`
    epilogstart=`/usr/local/bin/python -c "print $animend+$epilogstep"`
fi
echo "epilogstep is $epilogstep"

currentframe=$firstframe 
currentpov=$povfirst
if [ x$spline != x ] ; then 
    currentspline=$spline
else
    currentspline=0.0
    if [ $firstframe -le $prologframes ] ; then
	currentspline=`/usr/local/bin/python -c "print (${firstframe}-1) * $prologstep"`     
    elif [ $firstframe -le `expr $prologframes + $animframes` ] ; then
	currentpov=`expr $povfirst + \( $currentframe - 1 - $prologframes \) '*' $povstep`
	if [ $currentpov -gt $povlast ] ; then 
	    currentpov=$povlast
	fi
	currentspline=`/usr/local/bin/python -c "print $animstart + ($firstframe - 1 - $prologframes) * $animstep"`    
    else
	currentpov=$povlast
	currentspline=`/usr/local/bin/python -c "print $animend + ($firstframe - $prologframes - $animframes) * $epilogstep"`
    fi
fi
echo "Starting spline at $currentspline"


# =====================================================
# now render each frame at the current spline value
tilestart=0.0
tileend=1.0
#set -x
#set -v 
#set -e
while [ $currentframe -le $lastframe ] ; do

    echo "*********** Beginning frame number $currentframe **************"
    echo "Current spline is $currentspline, current pov file is $currentpov"
    cd $basedir
    #echo I am tile $tilenum of $numtiles tiles
    #tilestart=`/usr/local/bin/python -c "print ${tilenum}.0/${numtiles}.0"`
    #tileend=`/usr/local/bin/python -c "print (${tilenum}.0+1.0)/${numtiles}.0"`
    #echo "My tile range is from $tilestart to $tileend"
    povnum=`pad2length $currentpov $padlength`
    framenum=`padzeros $currentframe $numframes`

    # make the combined pov file:
    combined_pov_file="${moviedir}/${povname}_frame${framenum}_combined.pov"
    source_pov_file=${datadir}/${povname}_${povnum}.pov
    cp $incfile $combined_pov_file
    sed 's/#include "wormparams.inc"//' $source_pov_file >> $combined_pov_file

    # make the povray init file that will tell povray what to do
    povinifile=${moviedir}/worms_${framenum}.ini
    echo povray init file is $povinifile
    echo "# povray file for creating image" > $povinifile
    echo "# another comment " >> $povinifile
    echo scriptname is $scriptname
    if [ x$scriptname = x ] ; then 
	echo no script supplied
	    cat >> $povinifile <<EOF
Input_File_Name=$combined_pov_file
Library_Path="/usr/global/tools/povray/3.61/${SYS_TYPE}/share/povray-3.6"
Output_File_Type=n
Output_File_Name=${moviedir}/${moviename}_${framenum}
Verbose=On
Pause_when_done=Off
Height=$height
Start_Row=$tilestart
End_Row=$tileend
Width=$width
Video_Mode=1
Declare=wormcolor=$wormcolor
EOF
	if [ $axes = true ] ; then
	    echo 'Declare=axes=1' >>$povinifile
	fi
	if [ $box = true ] ; then
	    echo 'Declare=box=1' >>$povinifile
	fi
	if [ $backdrop = true ] ; then
	    echo 'Declare=backdrop=1' >>$povinifile
	fi
	if [ $campath = true ] ; then
	    echo 'Declare=camPath=1' >>$povinifile
	fi
	if [ $media = true ] ; then
	    echo 'Declare=media=1' >>$povinifile
	fi
	if [ $fog = true ] ; then
	    echo 'Declare=fog=1' >>$povinifile 
	fi
	if [ $renderoffscreen = true ] ; then
	    echo 'Display=Off' >> $povinifile
	else 
	    if [ $preview = true ] ; then 
    cat >> $povinifile <<EOF
Preview_Start_Size=64
EOF
	    fi		
	fi
	echo lowquality is $lowquality
	if [ x$lowquality != xtrue ] ; then 
	    cat >> $povinifile <<EOF
Antialias=On
Antialias_Threshold=0.99
Sampling_Method=2
Declare=Shadows=1
EOF
	else
	    cat >> $povinifile <<EOF	
Declare=Shadows=0
EOF
	fi
    else
	cat $scriptname >> $povinifile
    fi
    echo "Declare=cameraSpline=$currentspline" >>$povinifile
    echo ''>>$povinifile
    cd $datadir
    cmd="/usr/global/tools/povray/3.6.1/${SYS_TYPE}/bin/povray ${povinifile}"
    date
    echo $cmd
    if ${cmd} ; then
	echo image ${current} finished without error
    else
	echo Error while processing image $current
	break
    fi

    currentframe=`expr $currentframe + $numprocs`
    # assign next according to round-robin scheme
    if [ $currentframe -le $prologframes ] ; then
	if [ x$spline = x ] ; then 
	    currentspline=`/usr/local/bin/python -c "print (${currentframe}-1) * $prologstep"` 
	fi
    else
	if [ $currentpov -lt $povlast ] ; then
	    currentpov=`expr $povfirst + \( $currentframe - $prologframes - 1 \) '*' $povstep `
	    if [ $currentpov -gt $povlast ] ; then
		currentpov=$povlast
	    fi
	fi
	if [ x$spline = x ] && [ $currentframe -le `expr $prologframes + $animframes` ] ; then
	    currentspline=`/usr/local/bin/python -c "print $animstart + ($currentframe - 1 - $prologframes) * $animstep"`
	else
	    currentspline=`/usr/local/bin/python -c "print $animend + ($currentframe - $prologframes - $animframes) * $epilogstep"`
	fi
    fi
    

    if [ $debug != 1 ] ; then 
	rm -f $povinifile
    fi
done

echo End $0 at `date`
