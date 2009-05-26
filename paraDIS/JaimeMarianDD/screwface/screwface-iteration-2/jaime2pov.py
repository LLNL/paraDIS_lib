#!/usr/bin/env python
import sys

def usage():
    print "Usage:  jaime2pov.py infile outfile"
    return

if len(sys.argv) < 3:
    print "error: insufficient args"
    usage()
    sys.exit(1)
ifname = sys.argv[1]
ofname = sys.argv[2]


#========================================================
def PrintThing(thing):
    exec("print \"%s: \","%thing + thing)

#===================================================
def WriteTriangle(nodes, tricolor):
    print "WriteTriangle(%s, %d)"%(str(nodes), tricolor)
    nodestrings = []
    for node in nodes:
        nodestrings.append("<%f,%f,%f>"%(gNodes[node][0], gNodes[node][1], gNodes[node][2]))
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
        nodestrings.append("<%f,%f,%f>"%(gNodes[node][0], gNodes[node][1], gNodes[node][2]))

    outfile.write("make_dislocation(%s, %s, %d, %d)\n"%
                  (nodestrings[0], nodestrings[1], discolor, sphereBothEnds))
    return


#===================================================
def WriteFan(fan):
    print "WriteFan(%s)"%str(fan)
    outfile.write("// WriteFan(%s)\n"%str(fan))
    nodenum = 1
    discolor=fan[0]
    tricolor=fan[1]
    nodelist=fan[2]
    print "nodelist is", str(nodelist)
    while nodenum < len(nodelist):
        print "node number", nodenum
        WriteDislocationSegment(nodelist[nodenum], nodelist[nodenum-1], discolor, 0)
        if nodenum+1 < len(nodelist):
            WriteTriangle([nodelist[0],nodelist[nodenum],nodelist[nodenum+1]], tricolor)
        nodenum = nodenum+1
    WriteDislocationSegment(nodelist[0], nodelist[len(fan)-1], discolor, 0)
    return

#===================================================
def WriteDislocation(disloc):
    print "WriteDiscolation(%s)"%str(disloc)
    outfile.write("// WriteDislocation(%s)\n"%str(disloc))
    sys.stdout.flush()
    discolor=disloc[0]
    nodelist=disloc[1]
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
    while trinum < len(striphalves[0]) and trinum < len(striphalves[1]):
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
for realline in infile.xreadlines():
    line=realline.split()
    print "parsing line: \"%s\""%str(line)
    if not line or  line[0] == 'VARIABLES="X"' or line[0][0] == "#":
        continue
    elif line[0] == "NODES:":
        state = "NODES"
        continue
    elif line[0] == "FAN=":
        # fan: discolor tricolor nodelist
        state = "FANS"
        exec("fan = ["+line[1]+","+line[2]+","+line[3]+"]")
        print "adding fan", fan
        gFans.append(fan)
        continue
    elif line[0] == "STRIP=":
        # strip: discolor tricolor nodelist
        state = "STRIPS"
        exec("strip = ["+line[1]+","+line[2]+","+line[3]+","+line[4] + "]")
        print "adding strip", strip
        gStrips.append(strip)
        continue
    elif line[0] == "DISLOC=":
        # dislocation:: discolor nodelist
        state = "DISLOC"
        exec("disloc = ["+line[1] + ", "+line[2] + "]")
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
                      
    
