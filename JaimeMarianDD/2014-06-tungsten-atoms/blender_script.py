#!/usr/bin/env python
"""
Blender python script for creating an atomistic rendering for Jaime

Method 1 does not work:  
/Volumes/MacProHD/Programs/graphics/Blender/blender.app/Contents/MacOS/blender -P /Users/cook47/current_projects/paraDIS/JaimeMarianDD/2014-06-tungsten-atoms/blender_script.py

Method 2: 
Do the following in the blender python window.  The script assumes you have done this.  
import os, sys
sys.path.append('/Users/cook47/current_projects/paraDIS/JaimeMarianDD/2014-06-tungsten-atoms')
os.chdir('/Users/cook47/current_projects/paraDIS/JaimeMarianDD/2014-06-tungsten-atoms')
filename = 'blender_script.py'
exec(compile(open(filename).read(), filename, 'exec'))

# To load the data, setup the scene, and draw all the atoms: 
LoadSetupRender()

Or just
LoadData()
CreateScene(data)

LoadAtoms(data)

You may modify and reload as often as you wish with this syntax.
Remember:  Anything you do in blender shows up as a python command in the info window at the top of the GUI!  Very helpful to know this! 

"""

import math, sys, json
import numpy
sys.path.append('/Users/cook47/current_projects/paraDIS/JaimeMarianDD/2014-06-tungsten-atoms')
#exec(compile(open(filename).read(), 'diverging_map.py', 'exec'))
from diverging_map import *
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


#========================================================================
def pointCamera(camobj, lookat):
      # aim the camera
    direction = lookat - camobj.location
    # point the cameras '-Z' and use its 'Y' as up
    rot_quat = direction.to_track_quat('-Z', 'Y')
    camobj.rotation_euler = rot_quat.to_euler()

# ====================================================================
# Create a cylinder as part of the bounding box "wireframe"
def MakeCylinder(name, ep1, ep2, radius, orientation):
    print ("Make cylinder %s from ep1 %s to ep2 %s, in orientation %s"%(name, ep1,ep2,orientation))
    if orientation == 'X':
        rotation = [0, pi/2.0, 0]
    if orientation == 'Y':
        rotation = [pi/2.0, 0, 0]
    if orientation == 'Z':
        rotation = [0, 0, 0]
    cylvector = numpy.array(ep2) - numpy.array(ep1)
    center = numpy.array(ep1) + 0.5 * cylvector
    depth = numpy.linalg.norm(cylvector)
    print ("cylvector is %s, center is %s, and depth is %s"% (cylvector, center, depth))
    bpy.ops.mesh.primitive_cylinder_add(radius = radius, depth = depth, location = center, rotation = rotation)
    return

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
    camobj.location = [-250, -50, 35]
    cam = bpy.data.cameras['Camera']
    cam.show_limits = True
    cam.clip_end = 20*boundsSize[2]
    camobj.scale = Vector(boundsSize) * 0.1 
    #
    bpy.ops.object.empty_add()
    lookat = bpy.data.objects['Empty'].name= "lookat"
    lookat = bpy.data.objects["lookat"]
    lookat.location = center
    #
    pointCamera(camobj, lookat.location)
    #
    space = getView3Dspace(0)
    space.clip_start=0.001
    space.clip_end = 1000*1000
    bpy.context.scene.world.use_nodes = True
    bpy.context.scene.world.node_tree.nodes['Background'].inputs['Color'].default_value = [0,0,0.05,1]
    bpy.ops.object.lamp_add(type="SUN")
    bpy.data.objects['Sun']
    bpy.data.objects['Sun'].data.node_tree.nodes['Emission'].inputs['Strength'].default_value = 5
    # create_light(name, radius, rotation, location, value=1):
    # create_light_plane("light1", 1.0, (0,0,0), (0,0, 2*data["size"][2]), 10.0)
    # bpy.data.objects['light1'].scale = [boundsSize[0]/4.0, boundsSize[1]/4.0, 1]

#========================================================================
def createBoundsPlane(name, rotation, location, scale):
    bpy.ops.mesh.primitive_plane_add(radius=1, rotation=rotation, location=location) 
    plane = bpy.data.objects['Plane']
    plane.name = name
    bpy.data.objects[name].scale = scale
    bpy.data.materials.new('%s_material'%name)
    planemat = bpy.data.materials['%s_material'%name]
    plane.data.materials.append(planemat)
    planemat.use_nodes = True
    glossnode = planemat.node_tree.nodes.new("ShaderNodeBsdfGlossy")
    glossnode.name = 'glossnode_%s'%name
    glossnode.inputs['Roughness'].default_value = 0.35
    glossnode.inputs['Color'].default_value = [0.2,0.2,0.2,1]
    # planemat.node_tree.nodes.remove(planemat.node_tree.nodes["Diffuse BSDF"])
    outnode = planemat.node_tree.nodes['Material Output']
    planemat.node_tree.links.new(glossnode.outputs['BSDF'], outnode.inputs['Surface'])
    return

#========================================================================
def createBoundsPlanes(data):
    bounds = data["bounds"]
    boundsSize = data["size"]
    center =  data["center"]
    diag = data["diagonal"]
    # XY plane:
    createBoundsPlane("XY_Plane", (0, 0, 0), (data["center"][0], data["center"][1], data['bounds'][2][0]), [boundsSize[0]*10.0, boundsSize[1]*10.0, 1] )    
    # YZ plane (rotate Y into Z)
    # createBoundsPlane("YZ_Plane", (0, pi/2.0, 0), (bounds[0][0], center[1], center[2]), [boundsSize[2]/2.0, boundsSize[1]/2.0, 1])    
    # XZ plane (rotate X into Z)
    # createBoundsPlane("XZ_Plane", (pi/2.0, 0, 0), (center[0], bounds[1][0], center[2]), [boundsSize[0]/2.0, boundsSize[2]/2.0, 1])    
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

#========================================================================
# Ideally, I would combine all atoms into a single mesh for drawing efficiency, but each atom needs its own color, so...

RGB1 = numpy.array([59, 76, 192])
RGB2 = numpy.array([180, 4, 38])
colormap = ColorMapCreator(RGB1, RGB2) 

def MakeAtom(atom):
    bpy.ops.mesh.primitive_uv_sphere_add(location=atom["location"], size = 1.2 )
    obj = bpy.data.objects['Sphere']
    name = "Atom_%d"%atom['id']
    obj.name = name
    bpy.ops.object.shade_smooth()
    bpy.data.materials.new('%s_material'%name)
    mat = bpy.data.materials['%s_material'%name]
    obj.data.materials.append(mat)
    mat.use_nodes = True
    glossnode = mat.node_tree.nodes.new("ShaderNodeBsdfGlossy")
    glossnode.name = "gloss_%d"%atom['id']
    glossnode.inputs['Roughness'].default_value = 0.35
    color = colormap.interpolateColor(RGB1, RGB2, atom['c_centro']/10.0)
    glossnode.inputs['Color'].default_value = [color[0]/255.0,color[1]/255.0,color[2]/255.0,1]
    # mat.node_tree.nodes.remove(mat.node_tree.nodes["Diffuse BSDF"])
    outnode = mat.node_tree.nodes['Material Output']
    mat.node_tree.links.new(glossnode.outputs['BSDF'], outnode.inputs['Surface'])
    
    #colors = [ [1,0,0,1], [1,1,0,1], [1,0,1,1], [0,1,0,1], [0,1,1,1] ]
    #ramp = MakeColorRamp(mat, glossnode, colors)
    #ramp.name = 'ColorRame_%s'%name
    #ramp.inputs['Fac'].default_value = atom['c_centro']/10.0
    return

#========================================================================

def MakeAtoms(data):
    for atom in data["atoms"]:
        MakeAtom(atom)

#========================================================================
def CreateScene(data):
    SetupContext()
    createBoundsPlanes(data)
    SetupCameraAndFrustrum(data)

#========================================================================
data = {"nothing":None}
def LoadData():
    global data
    os.chdir("/Users/cook47/current_projects/paraDIS/JaimeMarianDD/2014-06-tungsten-atoms")
    infile = open("MD_300K_1100MPa_d40_slice[1].json")
    data = json.load(infile)["data"]
    return 


#========================================================================
def LoadSetupRender():
    LoadData()
    MakeAtoms(data)
    CreateScene(data)

