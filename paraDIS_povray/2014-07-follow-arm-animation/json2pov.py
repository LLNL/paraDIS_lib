#!/usr/bin/env python
""" Converts JSON output from analyzeParaDIS to povray-friendly macros that can be interpreted by a separate module.
"""
import json, sys

def errexit(msg, errno=1):
	print msg
	sys.exit(errno)
	
if len(sys.argv) < 2:
	print "usage: json2pov.py infile [outfile]"
	sys.exit(1)

infilename = sys.argv[1]
if infilename[-5:] != '.json':
	errexit("Input file must have .json extension.")
	
if len(sys.argv) == 3:
	outfilename = sys.argv[2]
else:
	outfilename = infilename.replace('.json', '.pov')
	
# open JSON file
data = json.load(open(infilename, 'r'))

povfile = open(outfilename, 'w')
numMetaArms = len(data['MetaArms'])
manum = 1
povfile.write("setbounds(%s)\n"%data['Bounds'].replace(' ',', '));
povfile.write("union {\n\tunion {\n");
for metaarmname in data['MetaArms']:
	sys.stdout.write("Processing metaarm %d of %d\r"%(manum, numMetaArms))
	metaarm = data['MetaArms'][metaarmname]
	MAID = metaarm['ID']
	for armname in metaarm['Arms']:
		armid = metaarm['Arms'][armname]['ID']
		arm = data['Arms']["Arm %s"%armid]
		burger = arm['burgers']
		for segname in arm['Segments']:
			segid = arm['Segments'][segname]['ID']
			seg = data['Segments']['Segment %s'%segid] 
			ep0loc = "<%s>"%seg['EP 0'].replace(' ',', ')
			ep1loc = "<%s>"%seg['EP 1'].replace(' ',', ')
			povfile.write("segment(%s %s, %s, %s, %s, %s)\n"%(segid, armid, MAID, ep0loc, ep1loc, burger))
			
povfile.write("\t}\n}\n");
povfile.close()
