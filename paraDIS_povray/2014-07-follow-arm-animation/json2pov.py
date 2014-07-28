#!/usr/bin/env python
""" Converts JSON output from analyzeParaDIS to povray-friendly macros that can be interpreted by a separate module.
"""
import json, sys

def errexit(msg, errno=1):
	print msg
	sys.exit(errno)
	
if len(sys.argv) < 2:
	print "usage: json2pov.py infile [segfile]"
	sys.exit(1)

# ===================================================================
# open JSON file
jsonfilename = sys.argv[1]
if jsonfilename[-5:] != '.json':
	errexit("Input file must have .json extension.")
	
data = json.load(open(jsonfilename, 'r'))

# ===================================================================
if len(sys.argv) == 3:
	segfilename = sys.argv[2]
else:
	segfilename = jsonfilename[-5:0] + "-segments.pov" 
	
segfile = open(segfilename, 'w')
numMetaArms = len(data['MetaArms'])
manum = 1
segfile.write("setbounds(%s)\n"%data['Bounds'].replace(' ',', '));
segfile.write("union {\n\tunion {\n");

for metaarmname in data['MetaArms']:
	sys.stdout.write("Segfile: processing metaarm %d of %d\r"%(manum, numMetaArms))
	metaarm = data['MetaArms'][metaarmname]
	MAID = metaarm['ID']
	nodes = []
	for armname in metaarm['Arms']:
		armid = metaarm['Arms'][armname]['ID']
		arm = data['Arms']["Arm %s"%armid]
		burger = arm['burgers']
		for segname in arm['Segments']:
			segid = arm['Segments'][segname]['ID']
			seg = data['Segments']['Segment %s'%segid] 
			ep0loc = "<%s>"%seg['EP 0'].replace(' ',', ')
			ep1loc = "<%s>"%seg['EP 1'].replace(' ',', ')
			segfile.write("segment(%s %s, %s, %s, %s, %s)\n"%(segid, armid, MAID, ep0loc, ep1loc, burger))
		for nodename in arm['Nodes']:
			nodeid = arm['Nodes']['nodename']['ID']
			node = data['Nodes']['Node '+nodeid]
			nodes.append("<" + node['location'].replace(' ',', ') + ">")
		
sys.stdout.write("\n")

segfile.write("\t}\n}\n");
segfile.close()

