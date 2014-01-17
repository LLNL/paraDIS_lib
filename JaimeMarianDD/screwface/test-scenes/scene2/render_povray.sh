#!/usr/bin/env bash
datafile=$1
height=400
width=1600
#axes="declare=debug=1"
progressive="+SP256 +P"
povray $axes +I${datafile}.pov +o${datafile}_quickie.png +H${height} +W${width}  +Ft $progressive +A
