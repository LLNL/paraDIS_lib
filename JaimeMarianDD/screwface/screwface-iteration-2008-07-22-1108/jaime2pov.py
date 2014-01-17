#!/usr/bin/env python
import sys, scipy

def usage():
    print "Usage:  jaime2pov.py infile outfile"
    return


if len(sys.argv) < 2:
    print "error: insufficient args"
    usage()
    sys.exit(1)

ifname = sys.argv[1]
if len(sys.argv) == 3:
    ofname = sys.argv[2]
else:
    try:
        dot = ifname.rindex(".")
    except:
        print "error: cannot deduce output file because input file has no extension"
        sys.exit(1)

    ofname = ifname[0:dot] + ".pov"

#========================================================
def PrintThing(thing):
    exec("print \"%s: \","%thing + thing)

#===================================================
def WriteDebugNode(node):
    outfile.write("make_debug_node(<%f,%f,%f>)\n"%(node[0], node[1], node[2]))
    return

#===================================================
def WriteTriangle(nodes, tricolor):
    print "WriteTriangle(%s, %d)"%(str(nodes), tricolor)
    nodestrings = []
    for node in nodes:
        nodestrings.append("<%f,%f,%f>"%(node[0], node[1], node[2]))
    outfile.write("// triangle color %d nodes: %s\n"%(tricolor, str(nodes)))
    outfile.write("make_triangle(%s, %s, %s, %d)\n"%
                  (nodestrings[0], nodestrings[1], nodestrings[2], tricolor))
    return

#===================================================
def WriteDislocationSegment(node1,node2, discolor, sphereBothEnds):
    if (node1 == node2):
        print "warning:  skipping degenerate dislocation: %s, %s"%(str(node1), str(node2))
        return
    outfile.write("// segment connecting nodes %s and %s with color %d\n"%(node1, node2, discolor))
    nodestrings = []
    for node in [node1,node2]:
        nodestrings.append("<%f,%f,%f>"%(node[0], node[1], node[2]))

    outfile.write("make_dislocation(%s, %s, %d, %d)\n"%
                  (nodestrings[0], nodestrings[1], discolor, sphereBothEnds))
    return


#===================================================
def CreateExtraEndNodes(nodes):
    # create imaginary extra node at beginning and end of list, to allow interpolation along end segments
    # prepend and append an extra point to the list of nodes that is just
    # an extension of the first and last segments, but shorter so it has
    # less impact on the spline
    axis = 3
    
    nodes = [[0,0,0]] + nodes + [[0,0,0]]    
    while axis:
        axis=axis-1
        nodes[0][axis] = nodes[1][axis] + 0.1*(nodes[1][axis] - nodes[2][axis])
        nodes[-1][axis] = nodes[-2][axis] + 0.1*(nodes[-2][axis] - nodes[-3][axis])
    return  nodes

    
#===================================================
# http://www.lighthouse3d.com/opengl/maths/index.php3?catmullrom
# coefficients for Catmull-Rom interpolation
M12 =	 1.0
M21 =	-0.5
M23 =	 0.5
M31 =	 1.0
M32 =	-2.5
M33 =	 2.0
M34 =	-0.5
M41 =	-0.5
M42 =	 1.5
M43 =	-1.5
M44 =	 0.5
 
def CatmullRomCubicInterpolateNodes(fournodes, numInteriorNodes):
    # given four nodes, interpolate between the second and third nodes,
    # creating numInteriorNodes new nodes.  Returns the new nodes to insert. 
    # Uses Catmull-Rom (cubic) interpolation
    print "CatmullRomCubicInterploateNodes: ", fournodes
    axis = 0
    c1=[]; c2=[]; c3=[]; c4=[]
    while axis < 3:
        c1.append(M12*fournodes[1][axis])
        c2.append(M21*fournodes[0][axis] + M23*fournodes[2][axis])
        c3.append(M31*fournodes[0][axis] + M32*fournodes[1][axis] + M33*fournodes[2][axis] + M34*fournodes[3][axis])
        c4.append(M41*fournodes[0][axis] + M42*fournodes[1][axis] + M43*fournodes[2][axis] + M44*fournodes[3][axis])
        axis = axis+1

    interiorNodes = []
    intNode = 1
    while intNode <= numInteriorNodes:
        t = float(intNode)/(numInteriorNodes+1.0)
        axis = 0
        newnode  = []
        while axis < 3:
            newnode.append(((c4[axis]*t+c3[axis])*t+c2[axis])*t+c1[axis])
            axis = axis+1
        #newnode = newnode+append2nodes
        print "for t=", t, "adding node", newnode
        interiorNodes.append(newnode)

        intNode = intNode + 1

    print "interiorNodes is", interiorNodes
    return interiorNodes
    

def HermiteCubicInterpolateNodes(fournodes, numInteriorNodes):
    # given four nodes, interpolate between the second and third nodes,
    # creating numInteriorNodes new nodes.  Returns the new nodes to insert. 
    # Uses Hermite (cubic) interpolation
    # create the coefficients of the cubic interpolation function
    print "HermiteCubicInterploateNodes: ", fournodes
    axis=0
    coefficients = []
    while axis<3:
        # for each axis, create the four coefficients needed
        a0 = fournodes[3][axis] - fournodes[2][axis] - fournodes[0][axis] + fournodes[1][axis]
        a1 = fournodes[0][axis] - fournodes[1][axis] - a0
        a2 = fournodes[2][axis] - fournodes[0][axis]
        a3 = fournodes[1][axis]
        print "coefficients[%d] = "%axis, [a0,a1,a2,a3]
        coefficients.append([a0,a1,a2,a3])
        axis = axis+1

    # create the interior node set using the coefficients created above
    interiorNodes = []
    intNode = 1
    #append2nodes = fournodes[0][3:5]
    while intNode <= numInteriorNodes:
        t = float(intNode)/(numInteriorNodes+1.0)
        t2 = t*t
        t3 = t2*t        
        axis = 0
        newnode  = []
        while axis < 3:
            newnode.append(coefficients[axis][0] * t3 +
                           coefficients[axis][1] * t2 +
                           coefficients[axis][2] * t +
                           coefficients[axis][3])
            axis = axis+1
        #newnode = newnode+append2nodes
        print "for t=", t, "adding node", newnode
        interiorNodes.append(newnode)

        intNode = intNode + 1

    print "interiorNodes is", interiorNodes
    return interiorNodes
                  
#===================================================
def InterpolateNodes(nodelist, numInteriorNodes=8):
    print "InterpolateNodes (%d nodes): "%len(nodelist), nodelist
    # interpolate numInteriorNodes nodes between each node using cubic spline
    if (len(nodelist) < 3): return nodelist

    for node in nodelist:
        WriteDebugNode(node)

    nodelist = CreateExtraEndNodes(nodelist)
    print "with extra nodes (%d nodes): "%len(nodelist), nodelist
    nodelists = []
    nodenum = 1
    lastnode = len(nodelist)-3
    newnodes = []
    while nodenum <= lastnode:
        newnodes.append(nodelist[nodenum])
        newnodes = newnodes + CatmullRomCubicInterpolateNodes(nodelist[nodenum-1:nodenum+3], numInteriorNodes)
        nodenum = nodenum+1

    newnodes.append(nodelist[nodenum])
    print "newnodes (%d nodes) is"%len(newnodes), newnodes
    return newnodes

        
#===================================================
def CreateActualNodeList(nodelist):
    # return a list of actual nodes from a list of node indices
    actualNodes = []
    for nodeindex in nodelist:
        actualNodes.append(gNodes[nodeindex])
    return actualNodes

#===================================================
def WriteFan(fan):
    print "WriteFan(%s)"%str(fan)
    outfile.write("// WriteFan(%s)\n"%str(fan))
    nodenum = 1
    discolor=fan[0]
    tricolor=fan[1]
    nodelist=CreateActualNodeList(fan[2])
    if (fan[3]):
        nodelist = [nodelist[0]] + InterpolateNodes(nodelist[1:])
        
    print "nodelist is", str(nodelist)
    while nodenum < len(nodelist):
        print "node number", nodenum
        WriteDislocationSegment(nodelist[nodenum], nodelist[nodenum-1], discolor, 0)
        if nodenum+1 < len(nodelist):
            WriteTriangle([nodelist[0],nodelist[nodenum],nodelist[nodenum+1]], tricolor)
        nodenum = nodenum+1
    WriteDislocationSegment(nodelist[0], nodelist[len(nodelist)-1], discolor, 0)
    return

#===================================================
def WriteDislocation(disloc):
    print "WriteDiscolation(%s)"%str(disloc)
    outfile.write("// WriteDislocation(%s)\n"%str(disloc))
    sys.stdout.flush()
    discolor=disloc[0]
    nodelist=CreateActualNodeList(disloc[1])
    if (disloc[2]):
        nodelist = InterpolateNodes(nodelist)
        
    segnum=1
    while segnum < len(nodelist):
        if segnum == 1:
            WriteDislocationSegment(nodelist[segnum], nodelist[segnum-1], discolor, 1)
        else:
            WriteDislocationSegment(nodelist[segnum], nodelist[segnum-1], discolor, 0)
        segnum=segnum+1
    return

#===================================================
def WriteStrip(strip):
    print "WriteStrip(%s)"%str(strip)
    outfile.write("// WriteStrip(%s)\n"%str(strip))

    discolor=strip[0]
    tricolor=strip[1]
    if abs(len(strip[2]) - len(strip[3])) > 1:
        raise "Error: one triangle strip has more than one node more than the other.  len(strip[2])=%d,  len(strip[3]) = %d\nStrips:\n%s\n%s\n"%(len(strip[2]), len(strip[3]), str(strip[2]), str(strip[3]))
    strip[2] = CreateActualNodeList(strip[2])
    strip[3] = CreateActualNodeList(strip[3])
    if (strip[4]):
        strip[2] = InterpolateNodes(strip[2])
        strip[3] = InterpolateNodes(strip[3])

    if len(strip[2]) > len(strip[3]):
        striphalves = [strip[2],strip[3]]
    else:
        striphalves = [strip[3],strip[2]]
        
       
    for striphalf in striphalves:
        nodenum = 1
        print "striphalf is", striphalf
        while (nodenum < len(striphalf)):
            if nodenum == 1:
                WriteDislocationSegment(striphalf[nodenum-1], striphalf[nodenum], discolor, 1)
            else:
                WriteDislocationSegment(striphalf[nodenum-1], striphalf[nodenum], discolor, 0)
            nodenum = nodenum + 1

    # interleaving triangles
    
    trinum=0
    # try to make a pair of triangles:
    while trinum < len(striphalves[1]): # the second half is always shorter
        if len(striphalves[0]) > trinum+1:
            WriteTriangle([striphalves[0][trinum], striphalves[1][trinum], striphalves[0][trinum+1]], tricolor)
        if len(striphalves[1]) > trinum+1:
            WriteTriangle([striphalves[0][trinum+1], striphalves[1][trinum], striphalves[1][trinum+1]], tricolor)
        trinum = trinum +1
            
        
    

#===================================================
def ExtendBounds(node):
    axis=3
    while axis:
        axis = axis-1
        if node[axis] > gBounds[axis][1]:
            gBounds[axis][1] = node[axis]
        if node[axis] < gBounds[axis][0]:
            gBounds[axis][0] = node[axis]
    return

#===================================================
# parse the input file
#========================================================
# open the file
print "reading file %s"%ifname
infile = open(ifname);

gNodes =[0] # Jaime uses 1-based arrays, so put a filler in there
gFans = []
gStrips = []
gDislocations = []
gBounds = [[0,0],[0,0],[0,0]]

state = "NO STATE"
lineNo = 0
try: 
    for realline in infile.xreadlines():
        # fix annoying habit of Jaime of leaving spaces out
        realline = realline.replace(")(", ") (")
        lineNo = lineNo + 1
        line=realline.split()
        print "parsing line: \"%s\""%str(line)
        if not line or  line[0] == 'VARIABLES="X"' or line[0][0] == "#":
            continue
        elif line[0] == "NODES:":
            state = "NODES"
            continue
        elif line[0] == "FAN=" or line[0] == "INTERPOLATE_FAN=":
            # fan: discolor tricolor nodelist
            state = "FANS"
            cmd = "fan = ["+line[1]+","+line[2]+","+line[3]+", %d]"%(line[0] == "INTERPOLATE_FAN=")
            print cmd
            exec(cmd)
            gFans.append(fan)
            continue
        elif line[0] == "STRIP=" or line[0]  == "INTERPOLATE_STRIP=":
            # strip: discolor tricolor nodelist nodelist
            state = "STRIPS"
            cmd = "strip = ["+line[1]+","+line[2]+","+line[3]+","+line[4] + ", %d]"%(line[0] == "INTERPOLATE_STRIP=")
            print cmd
            exec(cmd)
            gStrips.append(strip)
            continue
        elif line[0] == "DISLOC=" or line[0] == "INTERPOLATE_DISLOC=":
            # dislocation:: discolor nodelist
            state = "DISLOC"
            cmd = "disloc = ["+line[1] + ", "+line[2] + ", %d]"%(line[0] == "INTERPOLATE_DISLOC=")
            print cmd
            exec(cmd)
            print "adding dislocation", disloc
            gDislocations.append(disloc)
            continue
        else:
            if state != "NODES":
                raise "Bad line"
            exec("node = [%f,%f,%f,%d, %d]"%(float(line[0]),float(line[1]),float(line[2]),int(line[3]), len(gNodes)))
            ExtendBounds(node)
            gNodes.append(node)
            continue
except:
    print "Error parsing line", lineNo
    raise

#========================================================
def average(tuple):
    return (tuple[0]+tuple[1])/2.0;

gCenter = [average(gBounds[0]), average(gBounds[1]), average(gBounds[2])]
#========================================================

for thing in ["gBounds", "gCenter", "gNodes", "gFans", "gStrips", "gDislocations"]:
    exec("print \"%s: \","%thing + thing)

#========================================================
# write the output file
#========================================================
print "opening output file %s"%ofname
outfile = open(ofname, "w")

outfile.write("#declare lookatpos = <%f,%f,%f>;\n"%(gCenter[0], gCenter[1], gCenter[2]))
viewdir=0
viewdist=13
farview=[0,0,0]
while viewdir<3:
    farview[viewdir] = gBounds[viewdir][0] + viewdist*(gBounds[viewdir][1]-gBounds[viewdir][0])
    viewdir=viewdir+1
    
viewfrom=2
if viewfrom == 0:
    outfile.write("#declare camerapos = <%f,%f,%f>;\n"%(farview[0], gCenter[1], gCenter[2]))
elif viewfrom==1:
    outfile.write("#declare camerapos = <%f,%f,%f>;\n"%(gCenter[0], farview[1], gCenter[2]))
elif viewfrom==2:
    outfile.write("#declare camerapos = <%f,%f,%f>;\n"%(gCenter[0], gCenter[1], farview[2]))
    
outfile.write("#declare viewx = x * %f;\n"%(gBounds[0][1]-gBounds[0][0]))
outfile.write("#declare viewy = y * %f;\n"%(gBounds[1][1]-gBounds[1][0]))

outfile.write("#include \"jaime.inc\"\n")
for fan in gFans:
    WriteFan(fan)

for strip in gStrips:
    WriteStrip(strip)

for disloc in gDislocations:
    WriteDislocation(disloc)
    
print "done"
                      
    
