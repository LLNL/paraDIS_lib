#!/usr/bin/env bash
#set -xv
# Render animation.blend file in parallel on the cluster using blender-2.70a
# Assumes SLURM is present.  If not, runs in serial.  Each frame takes about 3 minutes to render.  
# usage:  srun -W 300000 -n 12 render_parallel.sh

numframes=300
clobber=1 # unset this to not clobber existing frames/frame_%04d.png files

# To make smaller frames for fast test cases, set this: 
export BLENDER_RESOLUTION_PERCENTAGE=100

if [ "$SLURM_NPROCS" == "" ]; then 
    SLURM_NPROCS=1
    SLURM_PROCID=0
fi

echo PROCID IS $SLURM_PROCID out of  $SLURM_NPROCS
logfile=logfile_${SLURM_PROCID}-2.log
echo "Rendering in parallel; logfile is $logfile"
exec >& $logfile

echo PROCID IS $SLURM_PROCID out of  $SLURM_NPROCS

framespertask=$(expr $numframes / "$SLURM_NPROCS") 
extraframes=$(expr $numframes % "$SLURM_NPROCS")

# loop and find our first frame
myfirstframe=1
procnum=0
while [ "$procnum" -lt "$SLURM_PROCID" ]; do 
    myfirstframe=$(expr $myfirstframe + $framespertask)
    if [ $extraframes != 0 ]; then 
        let myfirstframe++
        let extraframes--
    fi
    let procnum++
done
mylastframe=$(expr "$myfirstframe" +  $framespertask - 1)
if [ $extraframes != 0 ]; then 
    let mylastframe++
fi

echo "TASK $SLURM_PROCID : $FIRST FRAME: $myfirstframe  LAST FRAME: $mylastframe, NUMFRAMES: $(expr $mylastframe - $myfirstframe + 1)"

framenum=$myfirstframe
while [ "$framenum" -le "$mylastframe" ]; do 
    outfile=frames/frame$(printf '%04d' $framenum).png
    if [ "$clobber" ] || [ ! -f $outfile ]; then 
        # render from a blend file:
        # blender animation.blend -b -o frames/frame  -F PNG -x 1 -f $framenum
        # render from a python file:
        export BLENDER_TIMESTEP=$framenum
        export BLENDER_DATAFILE="$HOME/current_projects/paraDIS/JaimeMarianDD/2014-06-tungsten-atoms/MD_300K_1100MPa_d40_slice[1].json"
        blender animation.blend -b -f $framenum --python blender_script.py 
    else 
        echo "$outfile already exists; skipping"
    fi
    let framenum++
done

