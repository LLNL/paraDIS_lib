#!/usr/bin/env python
"""
Blender python script for creating an atomistic rendering for Jaime
To use, do this in the blender python window.  The script assumes you have done this.  
import os
os.chdir('/Users/cook47/current_projects/paraDIS/JaimeMarianDD/2014-06-MDspheres')
filename = 'blender_script.py'
exec(compile(open(filename).read(), filename, 'exec'))

You may modify and reload as often as you wish with this syntax. 
"""

import math, sys, json
 
#========================================================================
# assume starting from homefile (with cube, camera and light)
if bpy.context.scene.render.engine=='BLENDER_RENDER':
 print ('Render is = BLENDER_RENDER')
 bpy.context.scene.render.engine="CYCLES"
 print ('Converted to BLENDER_CYCLES')
else:
 print ('Render is = CYCLES')

bpy.data.scenes["Scene"].cycles.samples = 200
bpy.data.scenes["Scene"].cycles.preview_samples = 200
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
def deleteObjectsByName(names):
    if select_names(names, extend=False):
        bpy.ops.object.delete()     


#========================================================================
def pointCamera(camobj, lookat):
      # aim the camera
    direction = lookat - camobj.location
    # point the cameras '-Z' and use its 'Y' as up
    rot_quat = direction.to_track_quat('-Z', 'Y')
    camobj.rotation_euler = rot_quat.to_euler()

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
     emitter = lightmat.node_tree.nodes.new("ShaderNodeEmission")
     emitter.inputs['Strength'].default_value = value
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
    camobj.location = [center[0], center[1], center[2] + 2* boundsSize[2]]
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
    # create_light(name, radius, rotation, location, value=1):
    create_light_plane("light1", 1.0, (0,0,0), (0,0, 2*data["size"][2]), 10.0)
    bpy.data.objects['light1'].scale = [boundsSize[0]/4.0, boundsSize[1]/4.0, 1]

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
    return

#========================================================================
def createBoundsPlanes(data):
    bounds = data["bounds"]
    boundsSize = data["size"]
    center =  data["center"]
    diag = data["diagonal"]
    # XY plane:
    createBoundsPlane("XY Plane", (0, 0, 0), (data["center"][0], data["center"][1], data['bounds'][2][0]), [boundsSize[0]/2.0, boundsSize[1]/2.0, 1] )    
    # YZ plane (rotate Y into Z)
    createBoundsPlane("YZ Plane", (0, pi/2.0, 0), (bounds[0][0], center[1], center[2]), [boundsSize[2]/2.0, boundsSize[1]/2.0, 1])    
    # XZ plane (rotate X into Z)
    createBoundsPlane("XZ Plane", (pi/2.0, 0, 0), (center[0], bounds[1][0], center[2]), [boundsSize[0]/2.0, boundsSize[2]/2.0, 1])    
    return

#========================================================================
# Ideally, I would combine all atoms into a single mesh for drawing efficiency, but each atom needs its own color, so...
def make_atom(atom):
    bpy.ops.mesh.primitive_uv_sphere_add(location=atom["location"], size = 0.5 )
    atomobj = bpy.data.objects['Sphere']
    atomobj.name = "Atom %d"%atom['id']
    return

#========================================================================
def LoadAtoms(data):
    for atom in data["atoms"]:
        make_atom(atom)



# create scene
os.chdir("/Users/cook47/current_projects/paraDIS/JaimeMarianDD/2014-06-MDspheres")
infile = open("MD_300K_1100MPa_d40_slice[1].json")
data = json.load(infile)["data"]
createBoundsPlanes(data)
SetupCameraAndFrustrum(data)


