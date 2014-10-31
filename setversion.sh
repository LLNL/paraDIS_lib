#!/usr/bin/env bash
    
timestampOnly=false
function usage() {
    echo "Usage:  setversion.sh [--timestamp or $version]"
	echo "If --timestamp, then only changes the timestamp, leaves version alone" 
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
