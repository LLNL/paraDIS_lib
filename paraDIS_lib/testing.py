import numpy, math
from mathutils import Vector

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
def MakeCylinder(name, ep1, ep2, radius, orientation=None):
	# print ("Make cylinder %s from ep1 %s to ep2 %s, in orientation %s"%(name, ep1,ep2,orientation))
	ep1 = numpy.array(ep1)
	ep2 = numpy.array(ep2)
	cylvector = ep2 - ep1
	center = ep1 + 0.5 * cylvector
	depth = numpy.linalg.norm(numpy.array(cylvector))
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
		# cyl.rotation_euler = FindRotation(ep1, ep2)
		cyl.rotation_euler = orientation
		# compare = FindRotation(ep1, ep2)
		# print ("computed rotation %s"%str(cyl.rotation_euler ))
		# print ("compare rotation %s"%str(compare ))
	else:
		cyl.rotation_euler = FindRotation(ep1, ep2)
	return cyl

def MakeSphere(name, loc, radius=0.2):
	bpy.ops.mesh.primitive_uv_sphere_add(location=loc, size = radius)
	bpy.data.objects['Sphere'].name = "%s-Sphere"%name
	return

# =====================================================================

# ==========================================================================
import os, sys
projdir = os.path.expanduser('~/current_projects/paraDIS/paraDIS_lib/')
sys.path.append(projdir)
os.chdir(projdir)
filename = projdir + '/blender_script.py'
exec(compile(open(filename).read(), filename, 'exec'))

datafile = 'rs0001.json'
# datafile = 'rs0240.json'
# SetupContext()
LoadData(datafile)

cyl_h = 0.2 # length of component cylinders forming segments
cyl_segs = 10
segDiameter = 0.1

dbfilename = 'debug-loop.out'
dbfile = open(dbfilename, 'w')
print ("Writing info to %s"%dbfilename)
data['SegmentLists'] = {}
SegmentLists = data['SegmentLists']
numsegs = 0
for segname in data['Segments'].keys():	
	seg = data['Segments'][segname]
	burgers = int(seg['burgers'])
	energy = floor(burgers/10)
	if energy not in SegmentLists:
		dbfile.write(" Initialize SegmentList %d\n"%energy)
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
	for item in ["atom1", "atom2", "energy", "dv", "dvlen", "n", "cross_a", "n_cross_a", "cross_b", "n_cross_b"]:
		exec("dbfile.write(\"%s is %%s\\n\"%%%s)"%(item,item))
		dbfile.flush()	
	# make a stack of faces to locate the cylinders in the segment
	numcyls = int(ceil(dvlen / cyl_h))		
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
		for item in ["numfaces", "numverts", "g", "p1", "p2", "p3", "p4", "faceverts"]:	
			exec("dbfile.write(\"%s: %s is %%s\\n\"%%str(%s))"%(segname, item,item))
			dbfile.flush()
		# print ("After adding, SegmentLists[energy][\"faces\"][%d] is %s"%(numfaces, SegmentLists[energy]["faces"][numfaces]))
		# numfaces = len(SegmentLists[energy]["faces"])
		# numverts = len(SegmentLists[energy]["vertices"]
	msg = "Got segment %d: %s, energy %d:  %d faces, %d vertices"%(numsegs, seg, energy, numfaces, numverts)
	dbfile.write (msg+'\n')
	print(msg)
	numsegs = numsegs + 1

dbfile.close()


# find the evil:
energy = 1
bpy.ops.object.material_slot_add()
SegmentLists[energy]['material'] = bpy.data.materials.new("testing")
# CreateBurgerEnergyMaterial(energy)

print("Energy %s. Build a mesh from the segments.\n"%energy)
mesh = bpy.data.meshes.new("Energy level %d mesh"%energy)
mesh.from_pydata(SegmentLists[energy]["vertices"], [], SegmentLists[energy]["faces"])
mesh.update()
new_mesh = bpy.data.objects.new("Energy level %d object"%energy, mesh)
bpy.context.scene.objects.link(new_mesh)

print("Energy %s. Build the object. Get the cylinder from the 'build_segment' function.\n"%energy)
segmentObject = MakeSegmentCylinder("Energy level %d"%energy, segDiameter, cyl_h, cyl_segs)
segment_cylinder = segmentObject[0]
segment_cylinder.active_material =  SegmentLists[energy]['material']
segment_cups = segmentObject[1]
segment_cups.active_material =    SegmentLists[energy]['material'] 

print("Energy %s. Smooth the cylinders.\n"%energy)
bpy.ops.object.select_all(action='DESELECT')
segment_cylinder.select = True
segment_cups.select = True
bpy.ops.object.shade_smooth()

print("Energy %s. Parenting the mesh to the cylinder.\n"%energy)
segment_cylinder.parent = new_mesh
segment_cups.parent = new_mesh
new_mesh.dupli_type = 'FACES'
new_mesh.location = data['center']
SegmentLists[energy]['mesh'] = new_mesh


dbfilename = 'debug-SegmentList.out'
dbfile = open(dbfilename, 'w')
print ("Writing info to %s"%dbfilename)
for energy in SegmentLists:
	for etype in ['faces', 'vertices']:
		dbfile.write ("BEGIN SegmentLists[%d][%s] (%d items) ===================== \n"%(energy, etype, len(SegmentLists[energy][etype])))
		for itemnum in range(len(SegmentLists[energy][etype])):
			dbfile.write ("%s[%d] is %s\n"%(etype, itemnum, SegmentLists[energy][etype][itemnum]))
			dbfile.flush()			

dbfile.close()



# ====================================================================================





cyl_h = 0.05 # length of component cylinders forming segments
cyl_segs = 10
segDiameter = 0.1
Origin = numpy.array((0,0,0))
atom1 = numpy.array((2,2,0))
atom2 = numpy.array((5,3,0))
dv = atom2 - atom1
dvlen = numpy.linalg.norm(atom2-atom1)
n  = dv / dvlen  # unit normal vector in direction of segment
length_A = numpy.dot(-n, atom1) 
b     = atom1 + length_A * n  # location of R, the right angle
n_b   = b / numpy.linalg.norm(b) #  orthogonal to segment
cross_b = numpy.cross(n_b, n) # orthogonal to both n and n_b
# make a stack of faces to locate the cylinders in the segment
numcyls = int(ceil(dvlen / (cyl_h)))
for item in ["atom1", "atom2", "Origin", "b"]:
	exec("MakeSphere(\"%s\", %s, 0.2)"%(item,item))

MakeCylinder("b", Origin, b, 0.05)
MakeCylinder("-n", Origin, -n, 0.05)
MakeCylinder("gamma*n", Origin, gamma*n, 0.05)
MakeCylinder("atom1 to b", atom1, b, 0.05)
MakeCylinder("atom1-atom2", atom1, atom2, 0.05)
MakeCylinder("atom1", Origin, atom1, 0.05)
MakeCylinder("dv", Origin, dv, 0.05)
MakeCylinder("n_b", Origin, n_b, 0.05)
for j in range(numcyls):
	# make a square face at each midpoint of the cylinder
	g  = atom1 + n * cyl_h * (j-0.5)
	p1 = g + n_b * segDiameter
	p2 = g - n_b * segDiameter
	p3 = g - cross_b * segDiameter
	p4 = g + cross_b * segDiameter
	for item in ["p1", "p2", "p3", "p4", "g"]:
		exec("MakeSphere(\"%s\", %s, 0.02)"%(item,item))

	

"""
stick: ['_Carbon', Vector((0.33213165402412415, 7.638453960418701, -1.1028887033462524)), Vector((-0.5030002593994141, -1.0039997100830078, 0.6770000457763672)), bpy.data.materials['Carbon']],
dv is <Vector (-0.5030, -1.0040, 0.6770)>, 
v1 is <Vector (0.3321, 7.6385, -1.1029)>,
n is <Vector (-0.3836, -0.7657, 0.5163)>,
gamma is 6.545495361089706,
b is <Vector (-2.1788, 2.6267, 2.2766)>,
n_b is <Vector (-0.5311, 0.6403, 0.5549)>,
dl is 0.05000000074505806,
loops is 14
"""
# p1 is <Vector (0.1293, 7.9137, -0.8938)>
# p2 is <Vector (0.5542, 7.4015, -1.3378)>
# p3 is <Vector (-0.1116, 7.6208, -1.5072)>
# p4 is <Vector (0.7950, 7.6944, -0.7244)>
# g is <Vector (0.3417, 7.6576, -1.1158)>

Origin = numpy.array((0,0,0))
dv = Vector((-0.5030, -1.0040, 0.6770))
v1 = Vector((0.3321, 7.6385, -1.1029))
v2 = v1+dv


n  = dv / dv.length
gamma = -n * v1
b     = v1 + gamma * n  # b is orthogonal to n,  
n_b   = b / b.length
dl = 0.05
Stick_diameter = 0.02
loops = int(ceil(dv.length / (2.0 * dl)))
print ("stick: %s\n dv is %s\n\nv1 is %s\n\nn is %s\n\ngamma is %s\n\nb is %s\n\nn_b is %s\n\ndl is %s\n\nloops is %s\n"%("no stick", dv, v1, n, gamma, b, n_b, dl, loops))

for item in ["dv", "v1", "v2"]:
	exec("MakeSphere(\"%s\", %s, 0.1)"%(item,item))

MakeCylinder("b", Origin, b, 0.05)
MakeCylinder("-n", Origin, -n, 0.05)
MakeCylinder("gamma*n", Origin, gamma*n, 0.05)
MakeCylinder("v1 to b", v1, b, 0.05)
MakeCylinder("v1-v2", v1, v2, 0.05)
MakeCylinder("v1", Origin, v1, 0.05)
MakeCylinder("dv", Origin, dv, 0.05)
MakeCylinder("n_b", Origin, n_b, 0.05)

for j in range(loops):
	g  = v1 - n * dl / 2.0 - n * dl * j
	p1 = g + n_b * Stick_diameter * 2
	p2 = g - n_b * Stick_diameter * 2
	p3 = g - n_b.cross(n) * Stick_diameter * 3
	p4 = g + n_b.cross(n) * Stick_diameter * 3	
	for item in ["p1", "p2", "p3", "p4", "g"]:
		if j == 0:
			for item in ["p1", "p2", "p3", "p4", "g"]:
				exec("print(\"%s is %%s\"%%%s)"%(item,item))
		exec("MakeSphere(\"%s\", %s, 0.01)"%(item,item))


# ===============================================================
# See if I can extrude a cylinder along a polycurve
# weight  
w = 1 

# we don't have to use the Vector() notation.  
listOfVectors = [(0,0,0),(1,0,0),(2,0,0),(2,3,0),(0,2,1)]

def ExtrudePolyLine(name, pointlist):
	bpy.ops.curve.primitive_bezier_circle_add(radius = 1)
	circleobj = bpy.data.objects["BezierCircle"]
	circleobj.name = "%s circle"%name
	MakePolyLine("%s line obj"%name, "%s line curve"%name, pointlist)
	lineobj = bpy.data.objects["%s line obj"%name]
	lineobj.data.bevel_object = circleobj
	return

def MakePolyLine(objname, curvename, cList):  
	curvedata = bpy.data.curves.new(name=curvename, type='CURVE')  
	curvedata.dimensions = '3D'	 
	#
	objectdata = bpy.data.objects.new(objname, curvedata)  
	objectdata.location = (0,0,0) #object origin  
	bpy.context.scene.objects.link(objectdata)	
	#	
	polyline = curvedata.splines.new('NURBS')  
	polyline.points.add(len(cList)-1)  
	for num in range(len(cList)):  
		polyline.points[num].co = (cList[num])+(w,)	 
	#		
	polyline.order_u = len(polyline.points)-1
	polyline.use_endpoint_u = True
	return

ExtrudePolyLine("Testing", listOfVectors)

# MakePolyLine("NameOfMyCurveObject", "NameOfMyCurve", listOfVectors)
bpy.ops.curve.primitive_bezier_circle_add(radius = 1)
circleobj = bpy.data.curves["BezierCircle"]
circleobj.name = "Segment base circle"
arm = data['Arms'][armname]
locations = []
for nodenum in range(len(data['Arms'][armname]['Nodes'])):
	node = data['Nodes']['Node %s'%data['Arms'][armname]['Nodes']['Node %s'%nodenum]['ID']]
	locations.append(FloatArrayFromString(node['location'], append1=True))
ExtrudePolyLine(armname, locations, circleobj)
	
