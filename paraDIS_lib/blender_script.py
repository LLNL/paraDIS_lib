#!/usr/bin/env python
"""
Blender python script for creating a rendering from the JSON output of analyzeParaDIS.	Requires diverging_map.py to exist in its directory.  Also see render_parallel.sh

Method 1: command line, loads data in rs0240-nodes.json and rs0240-segments.json and renders to a file called 'testdir/testing.png'
export BLENDER_TIMESTEP=1  # defaults to 1
export BLENDER_OUTDIR=frames # defaults to 'frames'
export BLENDER_FRAMENAMES=frame # defaults to 'frame'
export BLENDER_DATAFILE='output.json' # defaults to output.json
export BLENDER_RESOLUTION_PERCENTAGE=100 # linear scaling, defaults to 100
blender -b --python blender_script.py  -F PNG -x 1 -f 1

Method 2: GUI, sets up the scene for interactive tweaking.	
Do the following in the blender python window: 

# Load contents of blender_script.py
# You may modify and reload as often as you wish, which redefines the functions without executing them.

import os, sys
projdir = os.path.expanduser('~/current_projects/paraDIS/paraDIS_lib/')
sys.path.append(projdir)
os.chdir(projdir)
filename = projdir + '/blender_script.py'
exec(compile(open(filename).read(), filename, 'exec'))

# To load the data, setup the scene, and draw all the segments:
datafile = 'rs0001.json'
LoadAndSetup(datafile)

# Or choose from a subset of LoadSetupRender(): 
SetupContext()
LoadData(datafile)
MakeSegments(data)
CreateScene(data)

# Useful: :-)
deleteObjectsByName('.001')

Remember:  Anything you do in blender shows up as a python command in the info window at the top of the GUI!  Very helpful to know this! 

"""
from math import *
from mathutils import *
import	sys, json, os, bpy
import numpy
projdir = os.path.expanduser('~/current_projects/paraDIS/paraDIS_lib')
sys.path.append(os.getcwd())
sys.path.append(projdir)
os.chdir(projdir)
from diverging_map import *

#========================================================================
def FloatArrayFromString(s):
	tokens = s.split()
	array = []
	for token in tokens:
		array.append(float(token))
	return array

#========================================================================
def errexit(msg, retval=1):
	sys.stderr.write(msg+'\n')
	sys.exit(retval)
	
#========================================================================
# assume starting from homefile (with cube, camera and light)
def SetupContext():
	if bpy.context.scene.render.engine=='BLENDER_RENDER':
		print ('Render is = BLENDER_RENDER')
		bpy.context.scene.render.engine="CYCLES"
		print ('Converted to BLENDER_CYCLES')
	else:
		print ('Render is = CYCLES')
	bpy.data.scenes["Scene"].cycles.samples = 200
	bpy.data.scenes["Scene"].cycles.preview_samples = 200
	bpy.data.scenes["Scene"].render.resolution_x = 1920
	bpy.data.scenes["Scene"].render.resolution_y = 1080
	try:
		bpy.data.scenes["Scene"].render.resolution_percentage = int(os.getenv("BLENDER_RESOLUTION_PERCENTAGE"))
	except:
		bpy.data.scenes["Scene"].render.resolution_percentage = 100	 
	deleteObjectsByName(['Cube', 'Lamp'])
	for area in bpy.context.screen.areas:
		if area.type == 'VIEW_3D':
			area.spaces[0].viewport_shade = 'RENDERED'
	return

#========================================================================
def deleteObjectsByName(names):
	if select_names(names, extend=False):
		bpy.ops.object.delete()		


#========================================================================
# select the objects whose name contains anything in keynamelist and deselect everything else.
def select_names(keynamelist, extend = False ):
	found = False
	if type(keynamelist) != type([1]) and type(keynamelist) != type((1,)):
		keynamelist = [str(keynamelist)]
	if extend == False:
		bpy.ops.object.select_all(action='DESELECT')
	for o in bpy.data.objects:
		for key in keynamelist:
			if key in o.name:
				o.select = True
				found = True
				bpy.context.scene.objects.active = o
	return found

#========================================================================
# Select a list of objects
def select_objs(objlist, extend = False):
	 if type(objlist) != type([1]) and type(objlist) != type((1,)):
		  objlist = [objlist]
	 if extend == False:
		  bpy.ops.object.select_all(action='DESELECT')
	 for o in objlist:
		  o.select = True
	 bpy.context.scene.objects.active = o

#========================================================================
def getView3Dspace(n=0):
	 """ Get the nth space from the given area of the screen which has a
	 'VIEW_3D' type.  Useful for setting view properties of 3D views
	 """
	 # We only want views that have a space type of "VIEW_3D"
	 # for area in bpy.context.window.screen.areas: print(area.type)
	 viewlist = []
	 for a in bpy.context.window.screen.areas:
		  if a.type == 'VIEW_3D':
			   viewlist.append(a)
	 v = viewlist[n]
	 found = None
	 for spc in v.spaces:
		  if spc.type == "VIEW_3D":
			   found = spc
			   break
	 if not found:
		  print("cannot find requested VIEW_3D space in screen areas")
	 return found

# ====================================================================
# ColorFromBurgers(): burgers type as integer --> color
# Got good 8-color map from http://colorbrewer2.org, move it into blender color space by normalizing each one:
colors = []
for color in  [numpy.array((228,26,28)), numpy.array((55,126,184)), numpy.array((77,175,74)), numpy.array((152,78,163)), numpy.array((255,127,0)), numpy.array((255,255,51)), numpy.array((166,86,40)), numpy.array((247,129,191))]:
	colors.append(color/numpy.linalg.norm(color))

def ColorFromBurgers(burgers):
	color = None
	if burgers < 0:
		color = colors[0]
	elif burgers == 0:
		color = colors[1]
	elif burgers < 20: # 10 - 13
		color = ( 0.25 + ( burgers - 10.0 ) / 4.0 ) * colors[2]
	elif burgers < 30: # 20 - 22
		color = ( 0.33 + ( burgers - 20.0 ) / 3.0 ) * colors[3]
	elif burgers < 40: # 30 - 32
		color = ( 0.33 + ( burgers - 30.0 ) / 3.0 ) * colors[4]
	elif burgers < 50: # 40 - 42
		color = ( 0.33 + ( burgers - 40.0 ) / 4.0 ) * colors[5]
	elif burgers == 50:
		color = colors[6]
	elif burgers == 60:
		color = colors[7]
	else:
		print("Bad burgers value: %d"%burgers)
		return None
	color = numpy.append(color, 1.0)
	return color # require opacity value

# ====================================================================
def FindRotation(ep1, ep2):
	# Thanks to http://mcngraphics.com/thelab/blender/connect/
	ep1 = numpy.array(ep1)
	ep2 = numpy.array(ep2)
	center = ep1 + 0.5 * (ep2 - ep1)
	# Position of ep2 if an imaginary sphere encompassing
	# both objects was transformed to 0, 0, 0
	ep2new = ep2 - center
	# --------------------------------------------
	# Spherical coordinates (radius r, inclination theta, azimuth phi) from 
	# the Cartesian coordinates (x, y, z) of o1 on our imaginary sphere
	r = numpy.linalg.norm(ep2new) # radius of sphere
	theta = math.acos(ep2new[2]/r)
	phi = math.atan2( ep2new[1], ep2new[0])
	return (0,theta, phi)

# ====================================================================
# Create a pyramind mesh (for future reference?)
def CreatePyramidMesh(name, ep1, ep2, radius):
	# Define the coordinates of the vertices. Each vertex is defined by a tuple of 3 floats.
	coords=[(-1.0, -1.0, -1.0), (1.0, -1.0, -1.0), (1.0, 1.0 ,-1.0), \
			(-1.0, 1.0,-1.0), (0.0, 0.0, 1.0)]	
	# Define the faces by index numbers of its vertices. Each face is defined by a tuple of 3 or more integers.
	# N-gons would require a tuple of size N.
	faces=[ (2,1,0,3), (0,1,4), (1,2,4), (2,3,4), (3,0,4)]	
	me = bpy.data.meshes.new("PyramidMesh")   # create a new mesh  	
	ob = bpy.data.objects.new("Pyramid", me)          # create an object with that mesh
	ob.location = (0,0,0)   # position object at 3d-cursor
	bpy.context.scene.objects.link(ob)                # Link object to scene 
	# Fill the mesh with verts, edges, faces 
	me.from_pydata(coords,[],faces)   # edges or faces should be [], or you ask for problems
	me.update(calc_edges=True)    # Update mesh with new data	
	return

# ====================================================================
def MakeCylinder(name, ep1, ep2, radius, orientation, burgers=None):
	print ("Make cylinder %s from ep1 %s to ep2 %s, in orientation %s"%(name, ep1,ep2,orientation))
	cylvector = numpy.array(ep2) - numpy.array(ep1)
	center = numpy.array(ep1) + 0.5 * cylvector
	depth = numpy.linalg.norm(cylvector)
	# print ("cylvector is %s, center is %s, and depth is %s"% (cylvector, center, depth))
	bpy.ops.mesh.primitive_cylinder_add(radius = radius, depth = depth, location = center)
	obj = bpy.data.objects['Cylinder']
	obj.name = name
	if orientation == 'X':
		obj.rotation_euler = [0, pi/2.0, 0]
	if orientation == 'Y':
		obj.rotation_euler = [pi/2.0, 0, 0]
	if orientation == 'Z':
		obj.rotation_euler = [0, 0, 0]
	else:
		# obj.rotation_euler = FindRotation(ep1, ep2)
		obj.rotation_euler = orientation
		# compare = FindRotation(ep1, ep2)
		# print ("computed rotation %s"%str(obj.rotation_euler ))
		# print ("compare rotation %s"%str(compare ))
	if burgers:
		color = ColorFromBurgers(burgers)
		bpy.ops.object.shade_smooth()
		bpy.data.materials.new('%s_material'%name)
		mat = bpy.data.materials['%s_material'%name]
		obj.data.materials.append(mat)
		mat.use_nodes = True
		# node = mat.node_tree.nodes.new("ShaderNodeBsdfDiffuse")
		# node.name = "diffuse_%s"%name
		node = mat.node_tree.nodes.new("ShaderNodeBsdfGlossy")
		node.name = "gloss_%s"%name
		node.inputs['Roughness'].default_value = 0.9
		node.inputs['Color'].default_value = ColorFromBurgers(burgers)
		outnode = mat.node_tree.nodes['Material Output']
		mat.node_tree.links.new(node.outputs['BSDF'], outnode.inputs['Surface'])
	return

# MakeCylinder('Testing', (0,0,0), (0,100,0), 2, None, 10)

# ======================================================================
def CreateBoundingBox(data):
	bounds = data["bounds"]
	print ("bounds = %s"%bounds)
	radius = 1
	verts = []
	for i in range(2):
		for j in range(2):
			for k in range(2):
				verts.append((bounds[0][i], bounds[1][j], bounds[2][k]))
	cylnum = 0
	for z in range(0,8,2):
		MakeCylinder("Cylinder %d"%cylnum, verts[z], verts[z+1], radius, 'Z')
		cylnum = cylnum + 1
	for y in [0,1,4,5]:
		MakeCylinder("Cylinder %d"%cylnum, verts[y], verts[y+2], radius, 'Y')
		cylnum = cylnum + 1
	for x in range(4):
		MakeCylinder("Cylinder %d"%cylnum, verts[x], verts[x+4], radius, 'X')
		cylnum = cylnum + 1
	return	  


#========================================================================
# create light plane: 
def create_light_plane(name, radius, rotation, location, value=1): 
	 bpy.ops.mesh.primitive_plane_add(radius=radius, rotation=rotation, location=location)
	 bpy.data.objects['Plane'].name = name
	 light = bpy.data.objects[name]
	 bpy.data.materials.new('%s_material'%name)
	 lightmat = bpy.data.materials['%s_material'%name]
	 light.data.materials.append(lightmat)
	 lightmat.use_nodes = True
	 # lightmat.node_tree.nodes.remove(lightmat.node_tree.nodes["Diffuse BSDF"])
	 # http://www.blender.org/documentation/blender_python_api_2_70_5/bpy.types.html
	 emitter = lightmat.node_tree.nodes.new("ShaderNodeEmission")
	 emitter.inputs['Strength'].default_value = value
	 emitter.name = '%s_emission'%name
	 lightmat.node_tree.links.new(emitter.outputs['Emission'], lightmat.node_tree.nodes['Material Output'].inputs['Surface'])
	 return light

# ============================================================
def CreateLight(location, brightness, name):
	bpy.ops.object.lamp_add(type="POINT")
	point = bpy.data.objects['Point']
	point.name =  name	  
	point.data.node_tree.nodes['Emission'].inputs['Strength'].default_value = brightness
	point.location = location
	point.data.shadow_soft_size = 50
	#location = (0, 0, bounds[2][0] + boundsSize[2]*0.9)
	# create_light(name, radius, rotation, location, value=1):
	#create_light_plane("light1", 500, (0,0,0), location, 10.0)
	# bpy.data.objects['light1'].scale = [boundsSize[0]/4.0, boundsSize[1]/4.0, 1]
	return

# ============================================================
def CreateLights(data, brightness):
	bounds = data["bounds"]
	boundsSize = data["size"]
	i = 0
	for xfac in [ 0.25, 0.75 ]:
		for yfac in [ 0.5, 0.75 ]: 
			location = (bounds[0][0] + boundsSize[0] * xfac, bounds[1][0] + boundsSize[1] * yfac, bounds[2][0] + boundsSize[2]*0.8)
			CreateLight(location, brightness, "light %d"%i)
			i = i+1
	return

#========================================================================
# No longer used; just keep for later reference
def pointCamera(camobj, lookat):
	  # aim the camera
	direction = lookat - camobj.location
	# point the cameras '-Z' and use its 'Y' as up
	rot_quat = direction.to_track_quat('-Z', 'Y')
	camobj.rotation_euler = rot_quat.to_euler()

#========================================================================
# Set up camera and frustrum
def SetupCameraAndFrustrum(data):
	bounds = data["bounds"]
	boundsSize = data["size"]
	center =  data["center"]
	diag = data["diagonal"]
	#
	# Set up camera, lights, 
	camobj = bpy.data.objects['Camera']
	camobj.location = [50, -50, 25]
	cam = bpy.data.cameras['Camera']
	cam.show_limits = True
	cam.clip_end = 20*boundsSize[2]
	camobj.scale = Vector(boundsSize) * 0.1 
	#
	if 'lookat' in bpy.data.objects:
		lookat = bpy.data.objects["lookat"]
	else:
		bpy.ops.object.empty_add()
		bpy.data.objects['Empty'].name= "lookat"
		lookat = bpy.data.objects["lookat"]
		lookat.location = (-39, 37, 0)

		
	camobj.constraints.new(type='TRACK_TO')
	camobj.constraints['Track To'].target = lookat
	camobj.constraints['Track To'].track_axis = 'TRACK_NEGATIVE_Z'
	camobj.constraints['Track To'].up_axis = 'UP_Y'
	#
	# pointCamera(camobj, lookat.location)
	#
	space = getView3Dspace(0)
	space.clip_start=0.001
	space.clip_end = 1000*1000
	bpy.context.scene.world.use_nodes = True
	bpy.context.scene.world.node_tree.nodes['Background'].inputs['Color'].default_value = [0,0,0.05,1]
	return

#========================================================================
def SetupAnimation():
	bpy.ops.object.select_all(action='INVERT')
	bpy.context.active_object.animation_data_clear()
	bpy.context.scene.frame_end = 300
	lookat = bpy.data.objects["lookat"]
	lookat.location = (0, 61, 0)
	lookat.keyframe_insert(data_path="location", frame=1)
	lookat.location = (-136, 61, 0)
	lookat.keyframe_insert(data_path="location", frame=100)
	lookat.location = (100, 61, 0)
	lookat.keyframe_insert(data_path="location", frame=300.0)
	camobj = bpy.data.objects['Camera']
	camobj.location = [-50, -200, 90]
	camobj.keyframe_insert(data_path="location", frame=1)
	camobj.location = [-50, -20, 90]
	camobj.keyframe_insert(data_path="location", frame=100)
	camobj.location = [-20, -20, -20]
	camobj.keyframe_insert(data_path="location", frame=300)


#========================================================================
def createBoundsPlane(name, rotation, location, scale):
	bpy.ops.mesh.primitive_plane_add(radius=1, rotation=rotation, location=location) 
	plane = bpy.data.objects['Plane']
	plane.name = name
	plane.scale = scale
	bpy.data.materials.new('%s_material'%name)
	planemat = bpy.data.materials['%s_material'%name]
	plane.data.materials.append(planemat)
	planemat.use_nodes = True
	diffuseNode = planemat.node_tree.nodes["Diffuse BSDF"]
	diffuseNode.inputs['Color'].default_value = [0.2,0.2,0.2,1]	   
	#planemat.node_tree.nodes.remove(planemat.node_tree.nodes["Diffuse BSDF"])
	#glossnode = planemat.node_tree.nodes.new("ShaderNodeBsdfGlossy")
	#glossnode.name = 'glossnode_%s'%name
	#glossnode.inputs['Roughness'].default_value = 0.35
	#glossnode.inputs['Color'].default_value = [0.2,0.2,0.2,1]
	#outnode = planemat.node_tree.nodes['Material Output']
	#planemat.node_tree.links.new(glossnode.outputs['BSDF'], outnode.inputs['Surface'])
	return

#========================================================================
def createBoundsPlanes(data):
	bounds = data["bounds"]
	boundsSize = data["size"]
	center =  data["center"]
	diag = data["diagonal"]
	# XY plane:
	createBoundsPlane("XY_Plane", (0, 0, 0), (data["center"][0], data["center"][1], -25.0), [boundsSize[0]*10.0, boundsSize[1]*10.0, 1] )	 
	# YZ plane (rotate Y into Z)
	# createBoundsPlane("YZ_Plane", (0, pi/2.0, 0), (bounds[0][0], center[1], center[2]), [boundsSize[2]/2.0, boundsSize[1]/2.0, 1])	
	# XZ plane (rotate X into Z)
	# createBoundsPlane("XZ_Plane", (pi/2.0, 0, 0), (center[0], bounds[1][0], center[2]), [boundsSize[0]/2.0, boundsSize[2]/2.0, 1])	
	return

#========================================================================
def CreateTexturedCube(data):
	bounds = data["bounds"]
	boundsSize = data["size"]
	center =  data["center"]
	bpy.ops.mesh.primitive_cube_add()
	cube = bpy.data.objects['Cube']
	name = "Textured Cube"
	cube.name = name
	cube.scale = Vector(boundsSize)/2.0
	cube.location = center
	bpy.data.materials.new('%s_material'%name)
	mat = bpy.data.materials['%s_material'%name]
	cube.data.materials.append(mat)
	mat.use_nodes = True
	bsdfnode = mat.node_tree.nodes["Diffuse BSDF"]
	texturenode = mat.node_tree.nodes.new("ShaderNodeTexImage")
	coordnode = mat.node_tree.nodes.new("ShaderNodeTexCoord")
	mat.node_tree.links.new(texturenode.outputs['Color'], bsdfnode.inputs['Color'])
	mat.node_tree.links.new(coordnode.outputs['Object'], texturenode.inputs['Vector'])
	realpath = projdir + '/atomtile.png'
	img = bpy.data.images.load(realpath)
	texturenode.image = img
	texturenode.projection = 'BOX'
	texturenode.texture_mapping.scale = Vector(boundsSize) * 0.1
	return 

#========================================================================
def MakeColorRamp(mat, linknode, colors):
	ramp = mat.node_tree.nodes.new("ShaderNodeValToRGB")
	if len(colors) > 2:
		frac_inc = 1.0 / (len(colors) - 1)
		frac = frac_inc
		for i in range(len(colors)-2):
			ramp.color_ramp.elements.new(frac)
			frac = frac + frac_inc
	for i in range(len(colors)):
		ramp.color_ramp.elements[i].color = colors[i]		 
	mat.node_tree.links.new(ramp.outputs['Color'], linknode.inputs['Color'])
	return ramp


	
# ===========================================================================
def MakeSegments(data):
	radius = 2
	for segname in data["Segments"].keys():
		seg = data["Segments"][segname]
		# MakeCylinder(segname, FloatArrayFromString(seg['EP 0']), FloatArrayFromString(seg['EP 1']), radius, None, int(seg['burgers']))
		MakeCylinder(segname, FloatArrayFromString(seg['EP 0']), FloatArrayFromString(seg['EP 1']), radius, FloatArrayFromString(seg['rotation']), int(seg['burgers']))

#========================================================================
# Ideally, I would combine all nodes into a single mesh for drawing efficiency, but each node needs its own color, so...

RGB1 = numpy.array([59, 76, 192])
RGB2 = numpy.array([180, 4, 38])
colormap = ColorMapCreator(RGB1, RGB2) 

def MakeNode(node):	   
	bpy.ops.mesh.primitive_uv_sphere_add(location=FloatArrayFromString(node["location"]), size = 1.2 )
	obj = bpy.data.objects['Sphere']
	name = "Node_%d"%node['id']
	obj.name = name
	bpy.ops.object.shade_smooth()
	bpy.data.materials.new('%s_material'%name)
	mat = bpy.data.materials['%s_material'%name]
	obj.data.materials.append(mat)
	mat.use_nodes = True
	glossnode = mat.node_tree.nodes.new("ShaderNodeBsdfGlossy")
	glossnode.name = "gloss_%d"%node['id']
	glossnode.inputs['Roughness'].default_value = 0.1
	color = colormap.interpolateColor(RGB1, RGB2, node['c_centro']/10.0)
	glossnode.inputs['Color'].default_value = [color[0]/255.0,color[1]/255.0,color[2]/255.0,1]
	# mat.node_tree.nodes.remove(mat.node_tree.nodes["Diffuse BSDF"])
	outnode = mat.node_tree.nodes['Material Output']
	mat.node_tree.links.new(glossnode.outputs['BSDF'], outnode.inputs['Surface'])
	#colors = [ [1,0,0,1], [1,1,0,1], [1,0,1,1], [0,1,0,1], [0,1,1,1] ]
	#ramp = MakeColorRamp(mat, glossnode, colors)
	#ramp.name = 'ColorRame_%s'%name
	#ramp.inputs['Fac'].default_value = node['c_centro']/10.0
	return

#========================================================================
def MakeNodes(data):
	for node in data["nodes"]:
		MakeNode(node)

#========================================================================
def CreateScene(data):
	# createBoundsPlanes(data)
	CreateTexturedCube(data)
	SetupCameraAndFrustrum(data)
	CreateLights(data, 100000)
	# CreateBoundingBox(data)

#========================================================================
data = {"nothing":None}
def LoadData(datafile=""):
	global data
	if datafile == "":
		datafile='output.json'
	if not os.path.exists(datafile):
		errexit ("Datafile %s not found\n"%datafile)
	print ("Loading data from file %s"%datafile)
	infile = open(datafile)		   
	data = json.load(infile)
	data['bounds'] = FloatArrayFromString(data['Bounds'])
	data['size'] = []
	data['center'] = []
	for i in range(3):
		data['size'].append (data['bounds'][i+1] - data['bounds'][i])
		data['center'].append(data['bounds'][i] + 0.5 * data['size'][i])
	return 


#========================================================================
def LoadAndSetup(datafile=""):
	SetupContext()
	LoadData(datafile)
	MakeNodes(data)
	CreateScene(data)
	SetupAnimation()

#========================================================================
if __name__ == "__main__":
	if len(sys.argv) > 1:	 
		print ("sys.argv is %s"%str(sys.argv))
		try:
			timestep = int(os.getenv("BLENDER_TIMESTEP"))
		except:
			timestep = 1			
		print ("Timestep is %d"%(timestep))
		outdir = os.getenv("BLENDER_OUTDIR")
		if outdir == "":
			outdir=="frames"
		basename = os.getenv("BLENDER_FRAMENAMES")
		if basename == "":
			basename = "frame"
		filename = "%s/%s_%04d"%(outdir,basename,timestep)
		if not os.path.exists(outdir):
			os.mkdir(outdir)
		print ("Rendering to file %s"%filename)
		bpy.data.scenes["Scene"].render.filepath = filename
		bpy.data.scenes["Scene"].frame_set(timestep)
		datafile = os.getenv("BLENDER_DATAFILE")
		LoadAndSetup(datafile)
		bpy.ops.render.render(write_still = True)
	# Don't do this, because it will reload the data when you call the exec() command from above in the GUI: 
	# else:
	#	  LoadSetupRender()
