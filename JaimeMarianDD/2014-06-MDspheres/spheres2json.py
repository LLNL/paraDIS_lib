#!/usr/bin/env python
import sys, scipy, json, math


def usage():
    print "Usage:  spheres2json.py infile [outfile] [logfile]"
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
    ofname = basename + ".json"

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
# parse the input file
#========================================================

lineNo = 0

def expectline(infile, expected):
    global lineNo
    line = "bogus"    
    while line and expected not in line:
        lineNo = lineNo + 1
        line = infile.readline()
    if not line:
        print "Cannot find string \"%s\" in any line in file"% expected
        sys.exit(1)
    return line

# open the file
print "reading file %s"%ifname
infile = open(ifname);
gAtoms = [] # atom has type, location, c_centro, R,G,B
gBounds = [[100000,-100000],[100000,-100000],[100000,-100000]]

#  get number of particles (atoms):
expectline(infile, "ITEM: NUMBER OF ATOMS")
   
try:
    lineNo = lineNo + 1
    numAtoms = int(infile.readline().strip())
except:
    print "Error reading number of atoms on line %d: \"%s\""%(lineNo, line)
    raise

print "Number of atoms:", numAtoms

# now read bounds:
expectline(infile, "ITEM: BOX BOUNDS")

for i in range(0,3):
    try: 
        line = infile.readline().split()
        gBounds[i] = (float(line[0]), float(line[1]))
        lineNo = lineNo+1
    except:
        print "Error reading %s bounds on line %d.  Bad line \"%s\""%(["X","Y","Z"][i], lineNo, line)
        raise

print "gBounds is", gBounds
# and now read the atoms:
expectline(infile, "ITEM: ATOMS type x y z c_centro c_cna c_ackland Color.R Color.G Color.B selection")

atomID = 0
for line in infile:
    try:
        lineNo = lineNo + 1
        tokens = line.split()
        atom = {"id": atomID, "type": int(tokens[0]), "location": (float(tokens[1]), float(tokens[2]), float(tokens[3])), "c_centro": float(tokens[4]), "color":  (float(tokens[7]), float(tokens[8]), float(tokens[9]))}
        print "Got atom line %d: %s"%(lineNo, str(atom))
        gAtoms.append(atom)
        atomID = atomID + 1        
    except:
        print "Error parsing line %d: \"%s\""%( lineNo, line)
        raise

print "Done parsing file"
#========================================================
def average(tuple):
    return (tuple[0]+tuple[1])/2.0;

gData = {"bounds": gBounds}
gData["center"] = [average(gBounds[0]), average(gBounds[1]), average(gBounds[2])]
gData["size"] = []
gData["center"] = []
gData["diagonal"] = 0.0
for i in range(3):
    gData["size"].append(gBounds[i][1] - gBounds[i][0])
    gData["center"].append ( gData["bounds"][i][0] + 0.5 *  gData["size"][i])
    gData["diagonal"] = gData["diagonal"] + gData["size"][i] * gData["size"][i] 
gData["diagonal"] = math.sqrt(gData["diagonal"])
gData["atoms"] = gAtoms
#========================================================
print "gData is", str(gData)

#========================================================
# write the output file
#========================================================
print "opening output file %s"%ofname
outfile = open(ofname, "w")

outfile.write( json.dumps({"data": gData}) + "\n")
# put some stuff out in the scene

msg = "Completed successfully.  Log file is %s.  To create the image, run blender, open a python script, and import with:\ninfile=open(\"%s\")\ndata = json.load(infile)[\"data\"]\n"%(logfile, ofname)
print msg
savedout.write(msg)    

