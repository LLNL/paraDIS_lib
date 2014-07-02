2014-07-01 notes

spheres2json.py takes a dumpfile from Jaime and makes it into a json file full of atoms and other info that blender_script.py uses. 

blender_script.py can take an argument, which is a json file containing the atoms.  It reads the json file and sets up the objects in the scene. 

atomtile.png is used by blender_script.py to create the background for the textured cube that defines the simulation boundary and creates the appearance of many tiny tungsten atoms. 

diverging_map.py provides a diverging colormap for blender_script.py

animation.blend contains the keyframes for the camera moves

*.dump are the data files from Jaime

render_parallel.sh uses the animation.blend and blender_script.py files to render a number of frames in parallel on a cluster using SLURM

frames directory contains the output frames 

