#!/usr/local/bin/bash
# $Id: makepics.sh,v 1.1 2005/02/07 18:35:08 rcook Exp $ 
# first simple image generator, parameterized by color scheme
povray  worms.ini declare=wormcolor=10 +olight.png 
povray  worms.ini declare=wormcolor=0 +odark.png 
povray  worms.ini declare=wormcolor=1 +ogreen.png 
