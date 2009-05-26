#!/usr/local/bin/bash

echo $0 beginning at `date`
cd ${HOME}/current_projects/meijie_tang/paraDIS_hts/movie
#./movie_setup.sh

#if [ $HOST_GRP == white ] ; then 
#    poe ./makewormmovie.sh -p -f 1 -l 100 -d /p/gw1/rcook/rs1/ -x 1280 -y 960 -t -r -n 1280x960_rs1_movie_`uname -n`
#elif uname -n | grep emperor >/dev/null; then
#    srun ./makewormmovie.sh -p -f 1 -l 100 -d /p/ba2/rcook/rs1/ -x 1280 -y 960 -t -r -n 1280x960_rs1_movie_`uname -n`
#elif uname -n | grep gviz >/dev/null; then
#    srun ./makewormmovie.sh -p -f 1 -l 11991 -stepsize 10 -d /nfs/tmp3/rcook/pov/ -x 1280 -y 960 -t -r -n 1280x960_allsteps_movie_`uname -n`
#elif uname  | grep Linux >/dev/null; then
    #srun -W 72000 -n 8 ./makewormmovie.sh  -datadir /nfs/tmp3/rcook/pov -povname family_day -moviedir /nfs/tmp3/rcook/images -parallel yes -v 5 -width 2560 -height 1920 -firstframe 450
    srun -W 72000 -n 8 $HOME/bin/makewormmovie.sh  -datadir /nfs/tmp0/rcook/paradis/2005-11/pov -povname converted_restart -moviedir /nfs/tmp0/rcook/2005-11/images -parallel yes -v 5 
#fi
echo frames done at `date`
