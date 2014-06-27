#!/usr/bin/env bash
#set -xv
# Render animation.blend file in parallel on the cluster using blender-2.70a


numframes=300

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
    if [ ! -f $outfile ]; then 
        blender animation.blend -b -o frames/frame  -F PNG -x 1 -f $framenum
    else 
        echo "$outfile already exists; skipping"
    fi
    let framenum++
done

