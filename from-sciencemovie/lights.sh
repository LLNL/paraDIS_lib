#!/usr/local/bin/bash
# $Id: lights.sh,v 1.1 2005/02/25 17:33:30 rcook Exp $
# generate 27 different lighting angles
nodes=${nodes:-8}

echo $Id: lights.sh,v 1.1 2005/02/25 17:33:30 rcook Exp $
lightZ=-1
while [ $lightZ -lt 2 ]; do 
    lightY=-1
    while [ $lightY -lt 2 ]; do 
	lightX=-1
	while [ $lightX -lt 2 ] ; do 
	    rm -rf light_${lightX}_${lightY}_${lightZ}
	    export povargs="declare=lightX=$lightX declare=lightY=$lightY declare=lightZ=$lightZ"
	    cmd="./povray-launch-stitch.sh $nodes light_${lightX}_${lightY}_${lightZ} -f"
	    echo $cmd
	    $cmd
	    let lightX++
	done
	let lightY++
    done
    let lightZ++
done
