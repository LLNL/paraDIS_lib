#!/usr/local/bin/bash
# $Id: stitch.sh,v 1.2 2006/01/31 04:35:19 rcook Exp $
# stitch together images made with povray-batch.sh
echo '$Id: stitch.sh,v 1.2 2006/01/31 04:35:19 rcook Exp $'
. ${HOME}/current_projects/RC_bash_lib/shellfuncs.sh
. `dirname $0`/image-common.sh

getargs $*

#set -x; set -v; 
# level starts at zero and may be no more than log base 4 of numprocs


#set +x; set +v; 

# COMPOSITING TREE FOR BATCH PROCESSING!  :-)  
# procnum is always zero-based, as are filenum, etc. 

chunksize=4
set -vx
cd $imgdir

#=================================
# for now, just stitch and exit
stitch $width $height $numfiles
exit $?

set +vx
