#!/usr/bin/env bash
    
function usage() {
    echo "Usage:  setversion.sh version"
}

if [ "x$1" == x ]; then 
    usage
    exit 1
fi

sed -i -e "s/#define RC_PARADIS_VERSION .*/#define RC_PARADIS_VERSION \"$1\"/g" -e "s/#define RC_PARADIS_VERSION_DATE.*/#define RC_PARADIS_VERSION_DATE \"$(date)\"/g" paradis_version.h
