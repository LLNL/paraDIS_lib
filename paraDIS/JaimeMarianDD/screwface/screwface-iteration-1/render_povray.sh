#!/usr/bin/env bash
datafile=$1
image=$2
height=400
width=1600
#debug="declare=debug=1"
antialias="+A0.1 +AM2 +R3" 
#progressive="+SP256 +P"
povray $debug +I${datafile} +o${image} +H${height} +W${width}  +Ft $progressive $antialias
