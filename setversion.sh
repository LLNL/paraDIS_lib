#!/usr/bin/env bash
    
timestampOnly=false
function usage() {
    current_version=$(grep '#define RC_PARADIS_VERSION ' paradis_version.h | awk '{print $3}' | sed 's/"//g' )
    echo "Usage:  setversion.sh [--timestamp or version]"
	echo "If --timestamp, then only changes the timestamp, leaves version alone" 
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
if [ "$1" == "--timestamp" ]; then 
	sed -i "s/#define RC_PARADIS_VERSION_DATE.*/#define RC_PARADIS_VERSION_DATE \"$(date)\"/g" paradis_version.h
elif [ ${1:0:1} == '-' ]; then 
	usage
	errexit "Unknown option \"$1\""
else
	sed -i -e "s/#define RC_PARADIS_VERSION .*/#define RC_PARADIS_VERSION \"$1\"/g" -e "s/#define RC_PARADIS_VERSION_DATE.*/#define RC_PARADIS_VERSION_DATE \"$(date)\"/g" paradis_version.h
fi
