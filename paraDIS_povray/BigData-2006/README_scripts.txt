good files:
povray-launch-stitch.sh
povray-batch.sh
quicklook.sh
image-common.sh
bigdata.ini
stitch.sh (untested)



To make a 128x960 image from /nfs/tmp0/rcook/paradis/2006-01-bigdata/16_slices/slice_3/converted_restart_242.pov :

/g/g0/rcook/current_projects/paraDIS_hts/BigData-2006/povray-launch-stitch.sh -datadir `pwd` -imgdir `pwd` -initfile /nfs/tmp0/rcook/paradis/2006-01-bigdata/16_slices/slice_3/bigdata.ini -width 1280 -height 960 -numnodes 24
