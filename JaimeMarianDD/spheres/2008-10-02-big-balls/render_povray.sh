#!/usr/bin/env bash

# This script is based on render_povray.sh from the screwface project of August 2008 with Jaime Marian.  In fact, as of 2008-09-12, it is identical.  
pause=-P
quality=
background=
verbose=
antialias="+A0.1 +AM2 +R3" 
height=800
width=1000
while echo $1 | grep -- - >/dev/null; do
   if echo $1 | grep -- -b >/dev/null; then 
       background=1
       shift
   elif echo $1 | grep -- -d >/dev/null; then 
       debug="declare=debug=1"
       descrip=${descrip}"-debug"
       shift
   elif echo $1 | grep -- -h >/dev/null; then 
       echo "usage:  render_povray.sh [options] infile [outfile]"
       echo "infile ends with .pov, and default outfile ends with .png"
       echo "options:"
       echo "-b: background povray (useful for small batch loops)"
       echo "-d: debug"
       echo "-f: far away view"
       echo "-g: show grid for debugging"
       echo "-h: help"
       echo "-l: low-quality rendering"
       echo "-p: progressive rendering"
       echo "-P: pause when finished"
       echo "-r: reverse view"
       echo "-v: be verbose"
       echo "-z: zoomed-in view"
       exit 0       
   elif echo $1 | grep -- -f >/dev/null; then 
       zoom="declare=far=1"
       descrip=${descrip}"-far"
       shift
   elif echo $1 | grep -- -g >/dev/null; then 
       grid="declare=grid=1"
       descrip=${descrip}"-grid"
       shift
   elif echo $1 | grep -- -l >/dev/null; then 
       #antialias="+Q5" # set low quality instead of antialiasing
       lowquality="declare=lowquality=1"
       shift
   elif echo $1 | grep -- -p >/dev/null; then 
       progressive="+SP8"
       shift
   elif echo $1 | grep -- -P >/dev/null; then 
       pause="+P"
       shift
   elif echo $1 | grep -- -r >/dev/null; then 
       reverse="declare=reverse=1"
       descrip=${descrip}"-reverse"
       shift
   elif echo $1 | grep -- -v >/dev/null; then 
       verbose="declare=verbose=1"
       shift
   elif echo $1 | grep -- -z >/dev/null; then 
       zoom="declare=zoom=1"
       descrip=${descrip}"-zoom"
       shift
   fi
done

datafile=$1
image=${2:-$(echo $datafile | sed 's/.pov//')}"${descrip}.png"
set -x

cmd="povray $debug $zoom $reverse $grid +I${datafile} +o${image} +H${height} +W${width} $quality +Ft $progressive $pause $antialias $lowquality $verbose"
if [ x$background = x1 ]; then 
    $cmd &
else
    if $cmd; then 
        echo "output file saved as $image"
    fi
fi

