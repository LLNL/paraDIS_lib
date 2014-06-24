#!/usr/bin/env bash
# parse and render MD{1,3,4,5,6}.cfg.  Intended for batch use.  Example: 
# mxterm 3 6 240  # reserve 3 nodes, 6 CPU's on prism for 4 hours
# srun -n 5 parse_render.sh  # run 5 instances of this script.  Each will snarf its SLURM_PROCID and do something wonderful with it. 
set -x 
file=$SLURM_PROCID
if [ $file == 0 ]; then file=6; fi
if [ $file == 2 ]; then file=5; fi
touch render_cookie_$file
spheres2pov.py MD${file}.cfg && render_povray.sh MD${file}.pov
rm render_cookie_$file
while ls render_cookie*; do
    echo task $file done and waiting for cookies to disappear
    sleep 60
done

    

