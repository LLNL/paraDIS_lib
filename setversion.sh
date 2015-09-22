#!/usr/bin/env bash
    
timestampOnly=false
current_version=$(grep '#define RC_PARADIS_VERSION ' paradis_version.h | awk '{print $3}' | sed 's/"//g' )
function usage() {
    echo "Usage:  setversion.sh [OPTIONS]"
    echo "OPTIONS:"
	echo "-p/--print, then just echo version to stdout and make no change" 
	echo "-t/--timestamp: then only changes the timestamp, leaves version alone" 
    echo "-v/--version version: set version to given version" 
    echo "Example: setversion.sh $current_version"
    echo "Note: Current version is $current_version"
}

errexit () {
	echo "$1"
	exit ${2:-1}
}

if [ "$1" == "" ]; then 
    usage
    exit 1
fi
if [ "$1" == "--print" ] || [ "$1" == '-p' ]; then 
	echo $current_version    
elif [ "$1" == "--timestamp" ] || [ "$1" == '-t' ]; then 
	sed -i "s/#define RC_PARADIS_VERSION_DATE.*/#define RC_PARADIS_VERSION_DATE \"$(date)\"/g" paradis_version.h
    echo "Timestamp changed, version unchanged in paradis_version.h"
elif [ "$1" == "--version" ] || [ "$1" == '-v' ]; then 
    shift
	sed -i -e "s/#define RC_PARADIS_VERSION .*/#define RC_PARADIS_VERSION \"$1\"/g" -e "s/#define RC_PARADIS_VERSION_DATE.*/#define RC_PARADIS_VERSION_DATE \"$(date)\"/g" paradis_version.h
    echo "Changed version and timestamp in paradis_version.h"
else
	usage
	errexit "Unknown option \"$1\""
fi
exit 0

