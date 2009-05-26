#!/usr/bin/env python
import sys, scipy


def usage():
    print "Usage:  spheres2pov.py [options] infile outfile [logfile]"
    print "default logfile is \"infile-basename.log\""
    return


if len(sys.argv) < 2:
    print "error: insufficient args"
    usage()
    sys.exit(1)

ifname = sys.argv[1]
try:
    dot = ifname.rindex(".")
    basename = ifname[0:dot]
except:
    print "warning: cannot deduce basename of input file because input file has no extension; using entire infile name"
    basename = ifname
    
if len(sys.argv) == 3:
    ofname = sys.argv[2]
else:
    ofname = basename + ".pov"

if len(sys.argv) == 4:
    logfile = sys.argv[3]
else:
    logfile = basename + ".log"

savedout = sys.stdout
savederr = sys.stderr
print "Sending output to", logfile
sys.stdout = sys.stderr = open(logfile, "w")


#========================================================
def PrintThing(thing):
    exec("print \"%s: \","%thing + thing)
    return

#===================================================
def WriteDebugNode(node):
    outfile.write("make_debug_node(<%f,%f,%f>)\n"%(node[0], node[1], node[2]))
    return

  

#===================================================
def ExtendBounds(atom):
    for axis in range(0,3):        
        if atom["location"][axis] > gBounds[axis][1]:
            gBounds[axis][1] = atom["location"][axis]
        if atom["location"][axis] < gBounds[axis][0]:
            gBounds[axis][0] = atom["location"][axis]
        if atom["raw location"][axis] > gRawBounds[axis][1]:
            gRawBounds[axis][1] = atom["raw location"][axis]
        if atom["raw location"][axis] < gRawBounds[axis][0]:
            gRawBounds[axis][0] = atom["raw location"][axis]
    return

#===================================================
# parse the input file
#========================================================
# open the file
print "reading file %s"%ifname
infile = open(ifname);
gMatrix = [[],[],[]] # 3x3 matrix
gAtoms = [] # atom is an ID and a location
gBounds = [[100000,-100000],[100000,-100000],[100000,-100000]]
gRawBounds = [[100000,-100000],[100000,-100000],[100000,-100000]]

lineNo = 1
# first get number of particles (atoms):
try:
    line = infile.readline().split()
    print ("line is %s"%line)
    numAtoms = int(line[4])
    lineNo = lineNo+1
except:
    print "Error reading number of atoms on line 0: \"%s\""%line
    raise

print "Number of atoms:", numAtoms

# now read matrix:
try: 
    for j in range(0,3):
        for i in range(0,3):
            line = infile.readline().split()
            print ("line %d is \"%s\""%(lineNo, line))
            gMatrix[j].append(float(line[2]))
            lineNo = lineNo+1
except:
    print "Error reading gMatrix[%d,%d] on line %d: \"%s\""%(i,j,lineNo, line)
    raise

print "gMatrix is", gMatrix
# and now read the atoms:
atomID = 0
try: 
    for line in infile:  
        print ("parsing atom %d on line %d: \"%s\""%(atomID, lineNo,str(line)))
        line = line.split()
        fileLoc=[float(line[2]), float(line[3]), float(line[4])]
        loc=[0,0,0]
        # use the matrix to convert the coords:
        for i in range(0,3):
            for j in range(0,3):
                loc[i] = loc[i] + gMatrix[i][j]*fileLoc[j]
        atom ={"ID":atomID,"radius":float(line[0]), "raw location": fileLoc, "location":loc, "color":float(line[7])}
        gAtoms.append(atom)
        print ("Appended atom: %s"%str(atom))
        ExtendBounds(atom)
        lineNo = lineNo+1
        atomID = atomID+1
        
except:
    print "Error parsing line", lineNo
    raise

print "Done parsing file"
#========================================================
def average(tuple):
    return (tuple[0]+tuple[1])/2.0;

gCenter = [average(gBounds[0]), average(gBounds[1]), average(gBounds[2])]
#========================================================

for thing in ["gBounds", "gRawBounds", "gCenter"]:
    exec("print \"%s: \","%thing + thing)

#========================================================
# write the output file
#========================================================
print "opening output file %s"%ofname
outfile = open(ofname, "w")

# put some stuff out in the scene

outfile.write("#declare upperbounds=<%f,%f,%f>;\n"%(gBounds[0][1], gBounds[1][1], gBounds[2][1]));
outfile.write("#declare lowerbounds=<%f,%f,%f>;\n"%(gBounds[0][0], gBounds[1][0], gBounds[2][0]));

outfile.write("#declare gCenter = <%f,%f,%f>;\n"%(gCenter[0], gCenter[1], gCenter[2]))

#include the global includes to allow calculations to proceed based on the above:
outfile.write("#include \"jaime.inc\"\n")

for atom in gAtoms:
    outfile.write("// atom %d\n"%atom["ID"])
    outfile.write("make_atom(%f, %f, <%f,%f,%f>)\n"%(atom["radius"], atom["color"], atom["location"][0], atom["location"][1], atom["location"][2]))
    
print "Successfully completed"
savedout.write("Completed successfully.  Log file is %s.  To create the image, run:\n./render_povray.sh %s\nTo preview the image, type:\nrender_povray.sh -p %s\n"%(logfile, ofname, ofname))    
    
