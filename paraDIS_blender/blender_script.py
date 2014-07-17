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
# datafile = 'rs0001.json'
# datafile = 'rs0240.json'
# datafile = 'rs0032.json'
# LoadAndSetup(datafile)

# Or choose from a subset of LoadSetupRender(): 
# datafile = 'rs0001.json'
# datafile = 'rs0032.json'
# datafile = 'rs0240.json'
# datafile = 'rs1320.json'
datafile = 'rs1907.json'
SetupContext()
LoadData(datafile)
CreateScene(data)
ExtrudeArms(data, timeout = 60)

LinkObjects(data)

# MakeNodes(data)
# MakeSegmentsCylinderMeshes(data)
# MakeSegmentsDupliverts(data)
# MakeSegments(data, limits=MakeLimits(0.10, 0.50)) # 10 percent of data
# MakeSegments(data)
# SetupAnimation()  # careful with this one; it will mess with the camera

# Useful: :-)
deleteObjectsByName('.001')

Remember:  Anything you do in blender shows up as a python command in the info window at the top of the GUI!  Very helpful to know this! 

"""
from math import *
from mathutils import *
import	sys, json, os, bpy, time
import numpy
projdir = os.path.expanduser('~/current_projects/paraDIS/paraDIS_lib')
sys.path.append(os.getcwd())
sys.path.append(projdir)
os.chdir(projdir)
from diverging_map import *

#========================================================================
def FloatArrayFromString(s, append = None):
	tokens = s.split()
	arrlen = len(tokens)
	if append != None:
		arrlen = len(tokens)+1
		tokens.append(append)
	array = numpy.zeros(arrlen)
	n = 0
	for token in tokens:
		array[n] = float(token)
		n=n+1
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
# Got good 11-color map from http://colorbrewer2.org, move it into blender color space by normalizing each one:
colors = []
for color in  [numpy.array((141,211,199)), numpy.array((255,255,179)), numpy.array((190,186,218)), numpy.array((251,128,114)), numpy.array((128,177,211)), numpy.array((253,180,98)), numpy.array((179,222,105)), numpy.array((252,205,229)), numpy.array((217,217,217)), numpy.array((188,128,189)), numpy.array((204,235,197))]:
	colors.append(color/numpy.linalg.norm(color))

def ColorFromBurgers(burgers):
	color = None
	if burgers < 0:
		color = colors[0]
	elif burgers == 0:
		color = colors[1]
	elif burgers == 10: # 10 - 13 -- the most common case
		color = colors[2]
	elif burgers == 11: # 10 - 13 -- the most common case
		color = colors[3]
	elif burgers == 12: # 10 - 13 -- the most common case
		color = colors[4]
	elif burgers == 13: # 10 - 13 -- the most common case
		color = colors[5]
	elif burgers < 30: # 20 - 22
		color = ( 0.33 + ( burgers - 20.0 ) / 3.0 ) * colors[6]
	elif burgers < 40: # 30 - 32
		color = ( 0.33 + ( burgers - 30.0 ) / 3.0 ) * colors[7]
	elif burgers < 50: # 40 - 42
		color = ( 0.33 + ( burgers - 40.0 ) / 4.0 ) * colors[8]
	elif burgers == 50:
		color = colors[9]
	elif burgers == 60:
		color = colors[10]
	else:
		print("Bad burgers value: %d"%burgers)
		return None
	color = numpy.append(color, 1.0)
	return color # require opacity value

# ====================================================================
# FindRotation
# Computes a Y and Z rotation that take the segment from a Z orientation
#	 (the blender cylinder uv mesh default) to its proper orientation.
# This it the same formula for rotating cylinders used in the paradis.C code.
# Reproduced here for reference.  
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
	theta = acos(ep2new[2]/r)
	phi = atan2( ep2new[1], ep2new[0])
	return (0,theta, phi)

	
# ====================================================================
# Used for drawing bounding box frame cylinders
def MakeCylinder(name, ep1, ep2, radius, orientation=None):
	# print ("Make cylinder %s from ep1 %s to ep2 %s, in orientation %s"%(name, ep1,ep2,orientation))
	ep1 = numpy.array(ep1)
	ep2 = numpy.array(ep2)
	cylvector = ep2 - ep1
	center = ep1 + 0.5 * cylvector
	depth = numpy.linalg.norm(cylvector)
	# print ("cylvector is %s, center is %s, and depth is %s"% (cylvector, center, depth))
	bpy.ops.mesh.primitive_cylinder_add(radius = radius, depth = depth, location = center)
	cyl = bpy.data.objects['Cylinder']
	cyl.name = name
	if orientation == 'X':
		cyl.rotation_euler = [0, pi/2.0, 0]
	elif orientation == 'Y':
		cyl.rotation_euler = [pi/2.0, 0, 0]
	elif orientation == 'Z':
		cyl.rotation_euler = [0, 0, 0]
	elif orientation != None:
		# rotation = FindRotation(ep1, ep2)
		# cyl.rotation_euler = rotation 
		cyl.rotation_euler = orientation		
		# print ("computed rotation %s"%str(cyl.rotation_euler ))
		# print ("compare rotation %s"%str(rotation ))
	else:
		cyl.rotation_euler = FindRotation(ep1, ep2)
	return cyl

# ===========================================================================
# Color segment object
def CreateBurgersMaterial(burgers):
	color = ColorFromBurgers(burgers)
	name = "burgers %d"%burgers
	bpy.data.materials.new('%s material'%name)
	mat = bpy.data.materials['%s material'%name]
	mat.use_nodes = True
	# node = mat.node_tree.nodes.new("ShaderNodeBsdfDiffuse")
	# node.name = "diffuse_%s"%name
	colornode = mat.node_tree.nodes.new("ShaderNodeBsdfGlossy")
	colornode.name = "%s color"%name
	colornode.inputs['Roughness'].default_value = 0.2
	color = ColorFromBurgers(burgers)
	colornode.inputs['Color'].default_value = color
	emission = mat.node_tree.nodes.new("ShaderNodeEmission")
	emission.name = "%s emission"%name
	emission.inputs['Color'].default_value = color		
	mixnode = mat.node_tree.nodes.new("ShaderNodeMixShader")
	mixnode.name = "%s mixnode"%name
	mixnode.inputs['Fac'].default_value = 0.2
	mat.node_tree.links.new(colornode.outputs['BSDF'], mixnode.inputs[1])
	mat.node_tree.links.new(emission.outputs['Emission'], mixnode.inputs[2]) 
	outnode = mat.node_tree.nodes['Material Output']
	mat.node_tree.links.new(mixnode.outputs['Shader'], outnode.inputs['Surface'])
	return mat

# ===========================================================================
# Used for drawing segments. 
def ExtrudePolyLine(name, locations, bevelobj, material):  
	curvedata = bpy.data.curves.new(name="%s line curve"%name, type='CURVE')  
	curvedata.dimensions = '3D'	 
	#
	obj = bpy.data.objects.new("%s line obj"%name, curvedata)  
	obj.location = (0,0,0) #object origin	
	# obj.data.materials.append(material)
	# bpy.ops.object.shade_smooth()
	# obj.active_material = material
	#	
	polyline = curvedata.splines.new('NURBS')  
	polyline.points.add(len(locations)-1)  
	for num in range(len(locations)):
		toadd = (locations[num])
		# print ("Adding point %s"%toadd)
		polyline.points[num].co = toadd 
	#
	polyline.order_u = len(polyline.points)-1
	polyline.use_endpoint_u = True
	obj.data.bevel_object = bevelobj
	return {"object": obj, "material": material}

# ===========================================================================
def AddTimingData(data, elapsed, arms, segments, nodes, curves):
	if 'timing' not in data or not data['timing']:
		data['timing'] = {'elapsed':[0], 'arms':[0], 'segments':[0], 'nodes':[0], 'curves':[0]}
	for thing in ['elapsed', 'arms', 'segments', 'nodes', 'curves']:
		exec("data['timing']['%s'].append(%s)"%(thing,thing))

# ===========================================================================
def PrintTimingData(data, thing):
	print ("Timing data:")
	numentries = len(data['timing']['elapsed'])
	print ("%10s%15s%15s%15s"%("n", "elapsed", thing, "%s/sec"%thing))
	for n in range(numentries):
		elapsed = data['timing']['elapsed'][n]
		numthings = data['timing'][thing][n]
		if elapsed == 0:
			thingspersec = 0
		else:
			thingspersec = float(numthings)/elapsed
		exec("print('%%10f%%15.2f%%15.2f%%15.2f' %% (%f, %f, %f, %f))"%(n, elapsed, numthings, thingspersec))
	return

# ===========================================================================
def ExtrudeArms(data, timeout = 300, interval = 5):
	data['curve materials'] = {}
	materials = data['curve materials']
	bpy.ops.curve.primitive_bezier_circle_add(radius = 50)
	circleobj = bpy.data.objects["BezierCircle"]
	circleobj.name = "Segment base circle"
	data['circleobj'] = circleobj
	starttime = time.time()
	interrupted = False
	numnodes = 0
	numarms = 0
	numcurves = 0
	data['timing']  = None
	if 'objects' not in data:
		data['objects'] = []
	nextTiming = 1
	objects = data['objects']
	for armname in data['Arms']:
		arm = data['Arms'][armname]
		# burgers = int(data['Segments']['Segment %s'%arm['Segments']['Segment 0']['ID']]['burgers'])
		burgers = int(arm['burgers'])
		if burgers not in materials:
			materials[burgers] = CreateBurgersMaterial(burgers)
			# bpy.data.materials.new("burgers %d texture"%burgers)
		material = materials[burgers]
		curvenum = 0
		locations = []
		for nodenum in range(len(data['Arms'][armname]['Nodes'])):
			nodeID = data['Arms'][armname]['Nodes']['Node %s'%nodenum]['ID']
			if nodeID == "WRAP":
				objects.append(ExtrudePolyLine("%s curve %d"%(armname, curvenum), locations, circleobj, material))
				numcurves = numcurves + 1
				numnodes = numnodes + len(locations)
				locations = []
				curvename = "%s curve %d"%(armname, curvenum)
				curvenum = curvenum + 1
			else:
				node = data['Nodes']['Node %s'%nodeID]
				locations.append(FloatArrayFromString(node['location'], append = 1.0))
		if locations:
			objects.append(ExtrudePolyLine("%s curve %d"%(armname, curvenum), locations, circleobj, material))
			numcurves = numcurves + 1
			numnodes = numnodes + len(locations)
		elapsed = time.time() - starttime
		numarms = numarms+1
		numsegs = numnodes - numcurves
		if elapsed > nextTiming:
			AddTimingData(data, elapsed, numarms, numsegs, numnodes, numcurves)
			nextTiming = elapsed + interval
		if elapsed > timeout:
			print("Interrupted rendering at %f seconds due to %f timeout"%(elapsed,timeout))
			break	
	AddTimingData(data, elapsed, numarms, numsegs, numnodes, numcurves)
	for name in ['numarms', 'numcurves', 'numnodes', 'numsegs']:
		exec('print("%s: %%d  in %f seconds, %s/sec = %%f " %% (%s, %s/%f))' %
			 (name, elapsed, name, name, name, elapsed))
	PrintTimingData(data, 'curves')
	return

# ===========================================================================
def LinkObjects(data, interval=5):
	objects = data['objects']
	linkstart = time.time()
	nextTiming = 0
	data['linktimes'] = []
	numobjs = 0
	for obj in objects:
		try:
			bpy.context.scene.objects.link(obj['object'])
			obj['object'].data.materials.append(obj['material'])
		except:
			print( "oops")
		elapsed = time.time() - linkstart
		numobjs = numobjs + 1
		if elapsed > nextTiming:
			data['linktimes'].append((elapsed, numobjs, float(numobjs)/elapsed))
			nextTiming = elapsed + 5
	data['linktimes'].append((elapsed, numobjs, float(numobjs)/elapsed))
	# for thing in ['arms', 'segments', 'nodes', 'curves']:
	# 	PrintTimingData(data, thing)
	return

# ===========================================================================
# Function, which produces a cylinder. All is somewhat easy to understand.
# Stolen from build_stick() in import_pdb.py in Blender install directory
def MakeSegmentCylinder(name, radius, top, bottom, sectors):
	dphi = 2.0 * pi/(float(sectors)-1)
	length = numpy.linalg.norm(top-bottom)
	vertices_top = [top]
	vertices_bottom = [bottom]
	vertices = []
	for i in range(sectors-1):
		x = radius * cos( dphi * i )
		y = radius * sin( dphi * i )
		z =	 length / 2.0
		vertex = Vector((x,y,z))
		vertices_top.append(vertex)
		z = -length / 2.0
		vertex = Vector((x,y,z))
		vertices_bottom.append(vertex)
	vertices = vertices_top + vertices_bottom
		
	# Side facets (Cylinder)
	faces1 = []	   
	for i in range(sectors-1):
		if i == sectors-2:
			faces1.append(	[i+1, 1, 1+sectors, i+1+sectors] )
		else:
			faces1.append(	[i+1, i+2, i+2+sectors, i+1+sectors] )
	
	# Top facets
	faces2 = []	   
	for i in range(sectors-1):
		if i == sectors-2:
			face_top = [0,sectors-1,1]
			face_bottom = [sectors,2*sectors-1,sectors+1]
		else:
			face_top	= [0]
			face_bottom = [sectors]
			for j in range(2):
				face_top.append(i+j+1)
				face_bottom.append(i+j+1+sectors)
		faces2.append(face_top)
		faces2.append(face_bottom)
	
	# Build the mesh, Cylinder
	cylinder = bpy.data.meshes.new("%s Cylinder mesh"%name)
	cylinder.from_pydata(vertices, [], faces1)
	cylinder.update()
	new_cylinder = bpy.data.objects.new("%s Cylinder object"%name, cylinder)
	new_cylinder.rotation_euler = FindRotation(top,bottom)
	new_cylinder.location = bottom + 0.5 * (top-bottom)
	bpy.context.scene.objects.link(new_cylinder)
	
	# Build the mesh, Cups
	cups = bpy.data.meshes.new("%s Cups mesh"%name)
	cups.from_pydata(vertices, [], faces2)
	cups.update()
	new_cups = bpy.data.objects.new("%s Cups object"%name, cylinder)
	new_cups.location = bottom + 0.5 * (top-bottom)
	new_cups.rotation_euler = FindRotation(top,bottom)
	bpy.context.scene.objects.link(new_cups)
	
	return (new_cylinder, new_cups)

# ===========================================================================
# NOT USED.  This was an attempt to create the meshes for segments "by hand"
# It works, but is slow.  Does not scale above a few arms (hundreds of segments)
def MakeSegmentsCylinderMeshes(data):
	print("burgerMaterials called\n")
	timeout = 60
	cyl_h = 0.2 # length of component cylinders forming segments
	cyl_segs = 10
	segDiameter = 2.0
	starttime = time.time()
	data['SegmentLists'] = {}# organized by burgers, each with its own material
	SegmentLists = data['SegmentLists']
	for segname in data['Segments'].keys():
		seg = data['Segments'][segname]
		sys.stdout.flush()
		burgers = int(seg['burgers'])
		energy = floor(burgers/10)
		if energy not in SegmentLists:			
			# dbfile.write("Initialize the SegmentList for energy level %d\n"%energy)
			SegmentLists[energy] = {"vertices": [], "faces": []}
			bpy.ops.object.material_slot_add()
			SegmentLists[energy]['material'] = bpy.data.materials.new("material energy %d"%energy)
			
		# Now create and add the vertices and faces for the current segment
		# We need an two orthogonal vectors to our segment.
		atom1 = FloatArrayFromString(seg['EP 0'])
		atom2 = FloatArrayFromString(seg['EP 1'])
		
		# CreateBurgerEnergyMaterial(energy)
		
		print("%s. Build the object. Get the cylinder from the 'build_segment' function."%segname)
		segmentObject = MakeSegmentCylinder(segname, segDiameter, atom2, atom1, cyl_segs)
		segment_cylinder = segmentObject[0]
		segment_cylinder.active_material = SegmentLists[energy]['material']
		segment_cups = segmentObject[1]
		segment_cups.active_material = SegmentLists[energy]['material']          
		
		print("%s. Smooth the segment."%segname)
		bpy.ops.object.select_all(action='DESELECT')
		segment_cylinder.select = True
		segment_cups.select = True
		bpy.ops.object.shade_smooth()
		if timeout > 0 and time.time() - starttime > timeout:
			break
	elapsed = time.time() - starttime
	numsegs = len(data['Segments'])
	print("Creating %d segments and updating scene took %f seconds, %f segments/second"%(numsegs, elapsed, numsegs/elapsed))		
	return

# ===========================================================================
# NOT USED.  Too slow.  Does not scale above a few hundred segments, much less arms. 
# Function, which draws the segments with help of the dupliverts technique.
# Return: list of dupliverts structures.
# Code based on PDB reader in blender
def MakeSegmentsDupliverts(data):
	if not 'SegmentLists' in data:
		data['SegmentLists'] = {} # organized by burgers, each with its own material
	SegmentLists = data['SegmentLists']
	print("MakeSegmentsDupliverts called\n")
	SegmentLists = {} # organized by burgers, each with its own material
	cyl_h = 0.2 # length of component cylinders forming segments
	cyl_segs = 10
	segDiameter = 0.1
	debug = False
	starttime = time.time()
	if 'debug' in data and data['debug']:
		debug = True
		dbfilename = 'debug-loop.out'
		dbfile = open(dbfilename, 'w')
		print ("Writing info to %s"%dbfilename)
	for segname in data['Segments'].keys():
		seg = data['Segments'][segname]
		sys.stdout.flush()
		burgers = int(seg['burgers'])
		energy = floor(burgers/10)
		if energy not in SegmentLists:			
			# dbfile.write("Initialize the SegmentList for energy level %d\n"%energy)
			SegmentLists[energy] = {"vertices": [], "faces": []}
			
		# Now create and add the vertices and faces for the current segment
		# We need an two orthogonal vectors to our segment.
		atom1 = FloatArrayFromString(seg['EP 0'])
		atom2 = FloatArrayFromString(seg['EP 1'])
		dv = atom2 - atom1
		dvlen = numpy.linalg.norm(atom2-atom1)
		n  = dv / dvlen  # unit normal vector in direction of segment
		# Simpler method:  Find cross product of an arbitrary vector with n	
		cross_a = numpy.cross(n,(0,0,1))  # b is orthogonal to n
		if numpy.linalg.norm(cross_a) == 0: # oops, n == (0,0,1), try again
			cross_a = numpy.cross(n,(0,1,0))
		n_cross_a = cross_a / numpy.linalg.norm(cross_a)   # unit normal orthogonal to n
		cross_b = numpy.cross(n_cross_a, n) # orthogonal to both n and n_cross_a
		n_cross_b = cross_b / numpy.linalg.norm(cross_b)
		# make a stack of faces to locate the cylinders in the segment
		numcyls = int(ceil(dvlen / cyl_h))		
		if debug:
			for item in ["atom1", "atom2", "energy", "dv", "dvlen", "n", "cross_a", "n_cross_a", "cross_b", "n_cross_b", "numcyls"]:
				exec("dbfile.write(\"%s is %%s\\n\"%%%s)"%(item,item))
				dbfile.flush()	
		for j in range(numcyls):
			# make a square face at each midpoint of the cylinder
			g  = atom1 + n * cyl_h * (j - 0.5)
			p1 = g + n_cross_a * 0.5* segDiameter
			p2 = g - n_cross_a * 0.5* segDiameter
			p3 = g - n_cross_b * 0.5* segDiameter
			p4 = g + n_cross_b * 0.5* segDiameter
			# facenum = len(SegmentLists[energy]["facenum"])
			SegmentLists[energy]["vertices"].append(p1)
			SegmentLists[energy]["vertices"].append(p2)
			SegmentLists[energy]["vertices"].append(p3)
			SegmentLists[energy]["vertices"].append(p4)
			numfaces = len(SegmentLists[energy]["faces"])
			faceverts = (numfaces*4+0,numfaces*4+2,numfaces*4+1,numfaces*4+3)		
			SegmentLists[energy]["faces"].append(faceverts)
			numverts = len(	SegmentLists[energy]["vertices"])
			numfaces = len(SegmentLists[energy]["faces"])
			if debug:
				for item in ["numfaces", "numverts", "g", "p1", "p2", "p3", "p4", "faceverts"]:	
					exec("dbfile.write(\"%s: %s is %%s\\n\"%%str(%s))"%(segname, item,item))
					dbfile.flush()
	if debug:
		dbfile.close()
	endtime = time.time()
	numsecs = endtime - starttime
	numsegs = len(data['Segments'])
	print ("Creating %d segments took %d seconds, for %f segments/sec"%(numsegs, numsecs, float(numsegs)/(endtime - starttime)))
	print ("Got segment %s, now with %d faces, %d vertices\n"%(seg, numfaces, numverts))
	print("MakeSegmentsDupliverts done with segment list creation\n")
	if debug:
		dbfilename = 'debug-SegmentList.out'
		dbfile = open(dbfilename, 'w')
		starttime = time.time()
		print ("Writing info to %s"%dbfilename)
		for energy in SegmentLists:
			for etype in ['faces', 'vertices']:
				dbfile.write ("BEGIN SegmentLists[%d][%s] (%d items) ===================== \n"%(energy, etype, len(SegmentLists[energy][etype])))
				for itemnum in range(len(SegmentLists[energy][etype])):
					dbfile.write ("%s[%d] is %s\n"%(etype, itemnum, SegmentLists[energy][etype][itemnum]))
					dbfile.flush()			
				
		dbfile.close()
		endtime = time.time()
		print("Writing segment list file took %f seconds"%(endtime-starttime))

	starttime = time.time()		
	for energy in SegmentLists.keys():
		# for now, just put a bland texture in
		bpy.ops.object.material_slot_add()
		SegmentLists[energy]['material'] = bpy.data.materials.new("testing")
		# CreateBurgerEnergyMaterial(energy)

		print("Energy %s. Build a mesh from the segments."%energy)
		mesh = bpy.data.meshes.new("Energy level %d mesh"%energy)
		mesh.from_pydata(SegmentLists[energy]["vertices"], [], SegmentLists[energy]["faces"])
		mesh.update()
		new_mesh = bpy.data.objects.new("Energy level %d object"%energy, mesh)
		bpy.context.scene.objects.link(new_mesh)
		
		print("Energy %s. Build the object. Get the cylinder from the 'build_segment' function."%energy)
		top    = Vector((0,0,cyl_h  / 2.0))
		bottom = Vector((0,0,-cyl_h / 2.0))
		segmentObject = MakeSegmentCylinder("Energy level %d"%energy, segDiameter, top, bottom, cyl_segs)
		segment_cylinder = segmentObject[0]
		segment_cylinder.active_material = SegmentLists[energy]['material']
		segment_cups = segmentObject[1]
		segment_cups.active_material = SegmentLists[energy]['material']          
		
		print("Energy %s. Smooth the cylinders."%energy)
		bpy.ops.object.select_all(action='DESELECT')
		segment_cylinder.select = True
		segment_cups.select = True
		bpy.ops.object.shade_smooth()
		
		print("Energy %s. Parenting the mesh to the cylinder."%energy)
		segment_cylinder.parent = new_mesh
		segment_cups.parent = new_mesh
		new_mesh.dupli_type = 'FACES'
		new_mesh.location = data['center']
		SegmentLists[energy]['mesh'] = new_mesh
	endtime = time.time()
	print("Creating meshes and updating scened took %f seconds"%(endtime-starttime))		

# ===========================================================================
# I broke this out so I could test textures easier. 
def MakeSegmentOld(segname, radius, endcaps=True):
	seg = data["Segments"][segname]
	cyl = MakeCylinder(segname, FloatArrayFromString(seg['EP 0']),  FloatArrayFromString(seg['EP 1']), radius, FloatArrayFromString(seg['rotation']))
	burgers = int(seg['burgers'])
	if burgers:
		objs = [cyl]
		if endcaps:
			bpy.ops.mesh.primitive_uv_sphere_add(location=ep1, size = radius )
			ep1s = bpy.data.objects['Sphere']
			ep1s.name = "%s ep1"%name
			bpy.ops.mesh.primitive_uv_sphere_add(location=ep2, size = radius )
			ep2s = bpy.data.objects['Sphere']
			ep2s.name = "%s ep2"%name
			objs = objs + [ep1s, ep2s]
		for obj in objs:
			name = obj.name
			color = ColorFromBurgers(burgers)
			bpy.ops.object.shade_smooth()
			bpy.data.materials.new('%s_material'%name)
			mat = bpy.data.materials['%s_material'%name]
			obj.data.materials.append(mat)
			mat.use_nodes = True
			# node = mat.node_tree.nodes.new("ShaderNodeBsdfDiffuse")
			# node.name = "diffuse_%s"%name
			colornode = mat.node_tree.nodes.new("ShaderNodeBsdfGlossy")
			colornode.name = "%s color"%name
			colornode.inputs['Roughness'].default_value = 0.6
			color = ColorFromBurgers(burgers)
			colornode.inputs['Color'].default_value = color
			emission = mat.node_tree.nodes.new("ShaderNodeEmission")
			emission.name = "%s emission"%name
			emission.inputs['Color'].default_value = color		
			mixnode = mat.node_tree.nodes.new("ShaderNodeMixShader")
			mixnode.name = "%s mixnode"%name
			mat.node_tree.links.new(colornode.outputs['BSDF'], mixnode.inputs[1])
			mat.node_tree.links.new(emission.outputs['Emission'], mixnode.inputs[2]) 
			outnode = mat.node_tree.nodes['Material Output']
			mat.node_tree.links.new(mixnode.outputs['Shader'], outnode.inputs['Surface'])
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
def CreateAreaLight(location, strength, name, size=1200):
	bpy.ops.object.lamp_add(type="AREA")
	light = bpy.data.objects['Area']
	light.name = name	  
	light.data.node_tree.nodes['Emission'].inputs['Strength'].default_value = strength
	light.location = location
	light.data.shadow_soft_size = 50
	light.data.size = size
	#location = (0, 0, bounds[4] + boundsSize[2]*0.9)
	# create_light(name, radius, rotation, location, value=1):
	#create_light_plane("light1", 500, (0,0,0), location, 10.0)
	# bpy.data.objects['light1'].scale = [boundsSize[0]/4.0, boundsSize[1]/4.0, 1]
	return

# ============================================================
def CreateSunLight(name, strength, rotation=(0,0,0)):
	bpy.ops.object.lamp_add(type="SUN")
	light = bpy.data.objects['Sun']
	light.name = name	  
	light.data.node_tree.nodes['Emission'].inputs['Strength'].default_value = strength

# ============================================================
def CreateLights(data, doSun = True, brightness = 2*1000*1000, size=1200):
	bounds = numpy.copy(data["bounds"])
	boundsSize = numpy.copy(data["size"])
	if not brightness:
		brightness = 8 * boundsSize[2]
	i = 0
	if doSun:
		CreateSunLight("Sun light", 1)
	if brightness > 0:
		for xfac in [ -0.1, 0.5, 1.1 ]:
			for yfac in [ -0.1, 0.5, 1.1 ]: 
				location = (bounds[0] + boundsSize[0] * xfac, bounds[2] + boundsSize[1] * yfac, bounds[4] + boundsSize[2]*1.2)
				CreateAreaLight(location, brightness, "Area light %d"%i, size)
				i = i+1
	return

# ============================================================
#	exec(compile(open(filename).read(), filename, 'exec'))
def TestLights():
	deleteObjectsByName('light')
	CreateLights(data, 10*1000*1000)

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
	bounds = numpy.copy(data["bounds"])
	boundsSize = numpy.copy(data["size"])
	center =  numpy.copy(data["center"])
	#
	# Set up camera, lights, 
	camobj = bpy.data.objects['Camera']
	camobj.location = numpy.add(center, [0, 2.6*boundsSize[1], 0.1*boundsSize[2]])
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
		lookat.location = center
	#
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


# ======================================================================
def CreateBoundingBox(data):
	bounds = numpy.copy(data["bounds"])
	boundsSize = numpy.copy(data["size"])
	print ("bounds = %s"%bounds)
	verts = []
	for i in range(2):
		for j in range(2):
			for k in range(2):
				verts.append(numpy.array((bounds[i], bounds[2+j], bounds[4+k])))
	cylnum = 0
	for z in range(0,8,2): 
		MakeCylinder("Cylinder-Z %d"%cylnum, verts[z], verts[z+1], 0.005 * boundsSize[0], 'Z')
		cylnum = cylnum + 1
	for y in [0,1,4,5]:
		MakeCylinder("Cylinder-Y %d"%cylnum, verts[y], verts[y+2], 0.005 * boundsSize[0], 'Y')
		cylnum = cylnum + 1
	for x in range(4):
		MakeCylinder("Cylinder-X %d"%cylnum, verts[x], verts[x+4], 0.005 * boundsSize[0], 'X')
		cylnum = cylnum + 1
	return	  


#========================================================================
def createBoundsPlane(name, rotation, location, scale):
	# Making radius = 0.5 means scaling works as expected
	bpy.ops.mesh.primitive_plane_add(radius=0.5, rotation=rotation, location=location)
	plane = bpy.data.objects['Plane']
	plane.name = name
	plane.scale = scale
	bpy.data.materials.new('%s_material'%name)
	mat = bpy.data.materials['%s_material'%name]
	plane.data.materials.append(mat)
	mat.use_nodes = True
	node = mat.node_tree.nodes.new("ShaderNodeBsdfVelvet")
	# node = mat.node_tree.nodes["Diffuse BSDF"]
	node.inputs['Color'].default_value = [0.5, 0.5, 0.5, 1]	   
	# node.inputs['Roughness'].default_value = 1.0
	# mat.node_tree.nodes.remove(mat.node_tree.nodes["Diffuse BSDF"])
	# node = mat.node_tree.nodes.new("ShaderNodeBsdfGlossy")
	# node.name = 'glossnode_%s'%name
	# node.inputs['Roughness'].default_value = 0.35
	# node.inputs['Color'].default_value = [0.2,0.2,0.2,1]
	outnode = mat.node_tree.nodes['Material Output']
	mat.node_tree.links.new(node.outputs['BSDF'], outnode.inputs['Surface'])
	return

# ========================================================================
def createBoundsPlanes(data):
	bounds = numpy.copy(data["bounds"])
	boundsSize = numpy.copy(data["size"])
	center =  numpy.copy(data["center"])
	# YZ planes (rotate Y into Z)
	# createBoundsPlane("YZ_Plane 0", (0, pi/2.0, 0), (bounds[0] - 0.2*boundsSize[0], center[1], center[2]), [10*boundsSize[2], 10*boundsSize[1], 10])	
	# createBoundsPlane("YZ_Plane 1", (0, pi/2.0, 0), (bounds[1] + 0.2*boundsSize[0], center[1], center[2]), [boundsSize[2], boundsSize[1], 1])	
	# XZ plane (rotate X into Z)
	createBoundsPlane("XZ_Plane 0", (pi/2.0, 0, 0), (center[0], bounds[2] - 0.2*boundsSize[1], center[2]), [10*boundsSize[0], 10*boundsSize[2], 10])	
	# createBoundsPlane("XZ_Plane 1", (pi/2.0, 0, 0), (center[0], bounds[3] + 0.2*boundsSize[1], center[2]), [boundsSize[0], boundsSize[2], 1])	
	# XY planes (no rotation, default orientation):
	createBoundsPlane("XY_Plane 0", (0, 0, 0), (data["center"][0], data["center"][1], bounds[4] - 0.2*boundsSize[2]), [10*boundsSize[0], 10*boundsSize[1], 1] )	 
	# createBoundsPlane("XY_Plane 1", (0, 0, 0), (data["center"][0], data["center"][1], bounds[5] + 0.2*boundsSize[2]), [boundsSize[0], boundsSize[1], 1] )	 
	return

# deleteObjectsByName('Plane')
#	exec(compile(open(filename).read(), filename, 'exec'))

# ========================================================================
def CreateTexturedCube(data):
	bounds = numpy.copy(data["bounds"])
	boundsSize = numpy.copy(data["size"])
	center =  numpy.copy(data["center"])
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
def InBounds(seg, limits):
	if limits == None:
		return True
	# If one endpoint is in, the segment is in
	for epname in ['EP 0', 'EP 1']:
		point = FloatArrayFromString(seg[epname])
		inbounds = True
		for i in range(3):
			if point[i] < limits[2*i] or point[i] > limits[2*i+1]:
				inbounds = False
		if inbounds:
			return True
	return False

# ======================================================================
def MakeLimits(fraction, offset):
	global data
	bounds = numpy.copy(data['bounds'])
	limits = numpy.copy(data['bounds'])
	center = numpy.copy(data['center'])
	boundsSize = numpy.copy(data['size'])
	limits[0] = bounds[0] + (offset - fraction/2.0) * boundsSize[0]
	limits[1] = bounds[0] + (offset + fraction/2.0) * boundsSize[0]
	return limits

# deleteObjectsByName('Segment')
# exec(compile(open(filename).read(), filename, 'exec'))
# onepercentCenterLimits = MakeLimits(0.01, 0.5)
# tenpercentCenterLimits = MakeLimits(0.1, 0.5)
# threepercentCenterLimits = MakeLimits(0.03, 0.5)
# MakeSegments(data, limits=tenpercentCenterLimits)
# MakeSegments(data, limits=MakeLimits(0.30, 0.50)) # 20 percent

	
# ===========================================================================
def MakeSegments(data, radius=None, limits=None):
	starttime = time.time()
	if radius == None:
		radius = 0.001 * data['size'][0]
	print ("radius is %s"%str(radius))
	numsegs = 0
	for segname in data["Segments"].keys():
		seg = data["Segments"][segname]
		if not InBounds(seg, limits):
			continue
		MakeSegment(segname, radius)
		numsegs = numsegs + 1
	endtime = time.time()
	secs = endtime - starttime
	
	print ("MakeSegments took %d seconds to make %d segments, or %f segments/sec\n"%(secs, numsegs, float(numsegs)/secs))

# ===========================================================================
def MakeSphere(name, loc, radius=0.2):
	bpy.ops.mesh.primitive_uv_sphere_add(location=loc, size = radius)
	bpy.data.objects['Sphere'].name = "%s-Sphere"%name
	return

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
	createBoundsPlanes(data)
	# CreateTexturedCube(data)
	SetupCameraAndFrustrum(data)
	CreateLights(data)
	CreateBoundingBox(data)

#========================================================================
try:
	test = len(data['Segments'])
except:
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
	data['size'] = numpy.zeros(3)
	data['center'] = numpy.zeros(3)
	for i in range(3):
		data['size'][i] = data['bounds'][2*i+1] - data['bounds'][2*i]
		data['center'][i] = data['bounds'][2*i] + 0.5 * data['size'][i]
	return 


#========================================================================
def LoadAndSetup(datafile=""):
	SetupContext()
	LoadData(datafile)
	# MakeNodes(data)
	CreateScene(data)
	MakeSegments(data)
	# SetupAnimation()

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
