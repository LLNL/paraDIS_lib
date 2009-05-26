#!/usr/local/bin/bash
# $Id: makepics.sh,v 1.1 2006/01/31 03:32:25 rcook Exp $ 
# first simple image generator, parameterized by color scheme
povray  worms.ini declare=wormcolor=10 +olight.png 
povray  worms.ini declare=wormcolor=0 +odark.png 
povray  worms.ini declare=wormcolor=1 +ogreen.png 
