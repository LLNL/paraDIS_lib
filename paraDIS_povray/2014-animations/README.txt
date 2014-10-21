The purpose of this directory is to create an animation of the "fuse" animation concept developed in a recent ParaDIS team meeting. 

Each animation will be stored in a separate git branch.  The README.branch will hopefully be modified for each animation.  

As of the end of this phase of the project, the process was to run analyzeParaDIS on a dumpfile, creating the "decl" and "obj" files.  

analyzeParaDIS(rs0240.data) -> rs0240.{decl,pov}

Next, modify render.inc to get the effects you want.

Finally, run render.sh, which can run either in parallel or in serial.  


