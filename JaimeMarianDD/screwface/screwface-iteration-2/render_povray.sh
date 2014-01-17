#!/usr/bin/env bash
datafile=$1
image=$(echo $datafile | sed 's/pov/png')
height=800
width=1000
#debug="declare=debug=1"
antialias="+A0.1 +AM2 +R3" 
#progressive="+SP256 +P"
povray $debug +I${datafile} +o${image} +H${height} +W${width}  +Ft $progressive $antialias
