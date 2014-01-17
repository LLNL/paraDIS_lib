#!/usr/bin/env python

import shelve, sys, os, traceback

toappend = os.path.abspath(os.path.dirname(sys.argv[0]))+'/RC_python_lib/util'
#print "appending to path:", toappend
sys.path.append(toappend)
import simpleargs
from DebugUtils import *

def usage():
    print "Usage: armsFromFragments.py [opts] filename"
    print "(filename can be in any place; need not be at end)"
    print "Options:"
    print "-o filename: output file for arms (default is input filename plus .arms)"
    print "-shelf: use out-of-core processing.  This is slower, so use only for huge files that crash with memory errors."
    print "-v or -verbose n: set verbosity to n"

useShelf = []
verbose = [0]
outfilename = []
opts = {
    '-o': outfilename,
    '-v': verbose,
    '--verbose': verbose
    }
boolopts = {
    '-shelf': useShelf
    }

remainingArgs = simpleargs.get_args(opts, boolopts)

SetVerbosity(verbose[0])

if len(remainingArgs) > 1:
    print "Error: too many args. Stopping as a precaution -- I can only do one file at a time."
    usage()
    sys.exit(1)

if not len(remainingArgs):
    print "Error: require input file name"
    usage()
    sys.exit(2)

infilename = remainingArgs[0]
if not outfilename:
    outfilename = [infilename + '.arms']

infile = open(infilename);
if not infile:
    print "Error: cannot open file:", infilename
    sys.exit(32)

if useShelf:
    arm_dict = shelve.open('.'+infilename+'.armdb')
    endpoint_dict = shelve.open('.'+infilename+'.epdb')
    fragment_dict = shelve.open('.'+infilename+'.fragmentdb')
else:
    arm_dict = {}
    endpoint_dict = {}
    fragment_dict = {}

# =============================================================================
lineno = 0
line = "nothing"
words = []
fragnum = 0
state = "doing nothing"
def GetNextInterestingLine():
    global lineno, line, words, infile
    start = 1
    while start or (line and (not words or words[0] == '#')):
        start = 0
        line = infile.readline()
        words = line.split()
        lineno = lineno+1
        
# =============================================================================
def ComputeBurgersType(burgsVector):
    newvec = []
    for num in burgsVector:
        if abs(num) > 1.15 and abs(num) < 1.16:
            newvec.append('1')
        elif num == 0.000:
            newvec.append('0')
        elif num > 0.57 and num < 0.58:
            newvec.append('+')
        elif num < -0.57 and num > -0.58:
            newvec.append('-')
        else:
            newvec.append('?')

    if newvec == ["1","0","0"]: return 1
    elif newvec == ["0","1","0"]: return 2
    elif newvec == ["0","0","1"]: return 3
    elif newvec == ["+","+","+"] or newvec == ["-","-","-"]: return 4
    elif newvec == ["+","+","-"] or newvec == ["-","-","+"]: return 5
    elif newvec == ["+","-","+"] or newvec == ["-","+","-"]: return 6
    elif newvec == ["-","+","+"] or newvec == ["+","-","-"]: return 7
    return 8
    
# =============================================================================
# ParseFragment: read a fragment and add info to the fragment and endpoint databases
def ParseFragment():
    global state
    fragmentID = infile.tell()
    GetNextInterestingLine()
    state="reading fragment header"
    dbprint(4, "parsing fragment %d\n"%fragmentID)
    if words[0] != "fragment":
        raise "Cannot find fragment tag at beginning of fragment"
    burgsVector = (float(words[1]),float(words[2]),float(words[3]))
    burgersType=ComputeBurgersType(burgsVector)
    numEndpoints = int(words[4])
    numInterior = int(words[5])
    fragment = {'addedToArm':0, 'filePosition':fragmentID, 'burgersType': burgersType, 'endpoints':[]}

    nodenum = 0
    while nodenum < numEndpoints:
        state="reading endpoint %d"%nodenum
        GetNextInterestingLine()
        if words[0] != 'endpoint':
            raise "garbled line"
        nodeID = words[1].split(',')
        nodeID = (int(nodeID[0]), int(nodeID[1]))
        connectivity = int(words[5])
        try:
            dbprint(4, "See if the endpoint %s already exists?"%str(nodeID))
            endpoint = endpoint_dict[nodeID]
            dbprint(4, "append new fragment (arm) info")
        except:
            dbprint(4, "create a new endpoint structure")
            endpoint = {'epID':nodeID, 'connectivity':connectivity,'fragments':[],
                        'monsterType': abs(connectivity), 'addedToArm':0}
                
            endpoint_dict[nodeID] = endpoint
        fragment['endpoints'].append(endpoint)
        nodenum = nodenum +1
        
    nodenum = 0
    while nodenum < numInterior:
        state="reading interior node %d"%nodenum
        GetNextInterestingLine()
        if words[0] != "interior":
            raise "Interior node line does not start with 'interior' keyword."
        nodenum = nodenum + 1

    dbprint(4, "Made fragment %d: %s\n"%(fragmentID,str(fragment)))
    # check for a special case:  if the fragment consists of a real endpoint and one other node, one of which is a ghost node, then this counts the endpoint as part of two fragments which are really the same fragment looked at from two directions, i.e., this fragment might be redundant.  In this case, only create the fragment if both endpoints are real endpoints AND if endpointID[0] < endpointID[1].
    if not numInterior and numEndpoints == 2:
        dbprint(5, "suspicious fragment")
        connectivities = []
        IDs = []
        havereal = 0
        haveghost = 0
        endpoints = fragment['endpoints']
        for endpoint in endpoints:
            connectivities.append(endpoint['connectivity'])
            IDs.append(endpoint['epID'])            

        for epnum in [0,1]:
            if abs(connectivities[epnum]) != 2: 
                dbprint(5, "possible problem -- we are a real endpoint in a two-node fragment\n")
                havereal = havereal + 1
            if connectivities[epnum] < 0:
                dbprint(5, "possible problem -- we have a ghost endpoint in a two-node fragment\n")
                haveghost = haveghost + 1
             
        if havereal and haveghost:
            if endpoints[0]['epID'] > endpoints[1]['epID']:
                dbprint(5, "Bail -- two real endpoints in a two node fragment, with endpointID[0] > endpointID[1]")
                return
            if havereal != 2:
                dbprint(5, "Bail -- one real endpoint and one interior ghost node in a two-node fragment")
                return
                 
            
    dbprint(5, "Valid fragment: we're ok, so cement the fragment-endpoint relationships:\n")
    for endpoint in fragment['endpoints']:
        endpoint['fragments'].append(fragmentID)

    dbprint(5, "After new relationships, we have fragment = %s\n\n"%str(fragment))
    fragment_dict[fragmentID] = fragment

# =============================================================================
def CheckFourArmedMonster(endpoint, neighbors):
    burgersTypes = [0,0,0,0,0,0,0,0,0]
    type111 = 0
    type100 = 0
    for neighborID in neighbors:
        neighbor = fragment_dict[neighborID]
        burgersType = neighbor['burgersType']
        if burgersTypes[burgersType]: # no duplicates allowed!
            return        
        burgersTypes[burgersType] = burgersTypes[burgersType] + 1
        if burgersType < 4:
            type100 = type100 + 1
        elif burgersType < 8:
            type111 = type111 + 1
        else:
            return
    if type111 == 4:
        endpoint['monsterType'] = -4
    if type100 == 2 and type111 == 2:
        endpoint['monsterType'] = -44
    return
        
# =============================================================================
def CheckButterflyArm(arm):
    if arm['burgersType'] > 3: 
        return
    burgersTypes = [0,0,0,0,0,0,0,0,0]
    numchecked = 0
    for endpoint in arm['endpoints']:
        for neighborid in endpoint['fragments']:
            neighbor = fragment_dict[neighborid]
            if neighbor in arm['fragments']:
                continue
            burgersType = neighbor['burgersType']
            if burgersType < 4 or burgersType > 7: # only allow type 111
                return
            if burgersTypes[burgersType]: # no duplicates allowed!
                return            
            burgersTypes[burgersType] = burgersTypes[burgersType] + 1
            numchecked = numchecked+1
    if numchecked != 4:
        raise "CheckButterflyArm: expected 4 exterior arms, found %d"%numchecked

    # found a butterfly arm!
    for endpoint in arm['endpoints']:
        endpoint['monsterType'] = -3
    return
        
# =============================================================================
def CheckArmEndpoints(arm):
    endpoints = arm['endpoints']
    if len(endpoints) == 0: return

    # first approximation: 
    for endpoint in endpoints:
        endpoint['monsterType'] = len(endpoint['fragments'])

    # now check for butterflies, type -3 arms:
    if len(endpoints) == 2 and len(endpoints[0]['fragments']) == 3 and len(endpoints[1]['fragments']) == 3:
        CheckButterflyArm(arm)
    else:
        # and check for type -44  and -4 arms:
        for endpoint in endpoints:
            if len(endpoint['fragments']) == 4:
                CheckFourArmedMonster(endpoint, endpoint['fragments'])

    # we check for type -33 arms in the final pass while we assign arm types
    return

# =============================================================================
gNextArm = 0
def MakeNextArm(key):
    global state, gNextArm
    state = "parsing fragment %d"%key
    dbprint(3, state)
    fragment = fragment_dict[key]
    if fragment['addedToArm']:
        dbprint(3,"Fragment %d is already part of an arm"%key)
        return
    state = 'making arm out of fragment %s' %str(fragment)
    dbprint(3, state)
    arm = {'armID':gNextArm, 'burgersType':fragment['burgersType'],
           'fragments':[fragment], 'endpoints':[], 'armType':0}
    gNextArm = gNextArm + 1
    fragment['addedToArm'] = 1
    
    for endpoint in fragment['endpoints']:
        dbprint(4, "looking at endpoint %s\n"%str(endpoint))
        # do a little QA while we're at it...
        neighbors = endpoint['fragments']
        numneighbors = len(neighbors)
        if numneighbors != abs(endpoint['connectivity']) and not (endpoint['connectivity'] == -2 and numneighbors == 1):
            raise "endpoint %s has connectivity of %d but has %d neighbors"%(str(endpoint), endpoint['connectivity'],numneighbors)
        if numneighbors != 2:
            if endpoint not in arm['endpoints']:
                arm['endpoints'].append(endpoint)
        else: #
           for newFragmentID in endpoint['fragments']:
               newFragment = fragment_dict[newFragmentID]
               if newFragment['addedToArm']: continue
               while newFragment:
                   arm['fragments'].append(newFragment)
                   newFragment['addedToArm'] = 1
                   for newEndpoint in newFragment['endpoints']:
                       neighbors = newEndpoint['fragments']
                       numneighbors = len(neighbors)
                       connectivity = newEndpoint['connectivity']
                       if numneighbors != abs(connectivity) and connectivity != -2: 
                           raise "endpoint %s has connectivity of %d but has %d neighbors"%(str(newEndpoint), newEndpoint['connectivity'],numneighbors)
                       if numneighbors != 2:
                           if newEndpoint not in arm['endpoints']:
                               arm['endpoints'].append(newEndpoint)
                       else:
                           newFragment = None;
                           for checkFragmentID in newEndpoint['fragments']:
                               checkFragment = fragment_dict[checkFragmentID]
                               if not checkFragment['addedToArm']: newFragment = checkFragment

    print "finished arm:", arm
    CheckArmEndpoints(arm)
    return

# =============================================================================
def CategorizeArm(arm):
    endpoints = arm['endpoints']
    if len(endpoints) < 2:
        arm['armType'] = 2 # loop
        return
    if len(endpoints) > 2:
        raise "Arm has more than 2 endpoints!"
    
    if arm['burgersType'] < 4: # type 100 arm
        for epnum in [0,1]:
            if len(endpoints[epnum]['fragments']) == 3 and  endpoints[1-epnum]['monsterType'] == -44 and endpoints[epnum]['monsterType'] != -3:
                endpoints[epnum]['monsterType'] = -33
    
    numMonsters = 0
    for endpoint in arm['endpoints']:
        if endpoint['monsterType']  in [-3,-33, -4, -44]:
            numMonsters = numMonsters + 1
    if numMonsters == 2:  arm['armType'] = 5
    elif numMonsters == 1:  arm['armType'] = 4
    else: arm['armType'] == 3
    if arm['burgersType'] < 4: # type 100 arm
        arm['armType'] = arm['armType'] + 3
    return

def WriteArms():
    state = "Writing arms to vis file %s"%outfilename
    dbprint(1, state)
    raise "WriteArms needs to be written in 2008"

# =============================================================================
try:
    # parse file and make endpoints, fragments, etc.
    GetNextInterestingLine()
    if len(words) != 4 or words[0] != 'fragments' or words[1] != 'version':
        raise "weird line before version string"
    version = int(words[2])
    numfragments = int(words[3])
    if version != 0:
        raise "incompatible file version %d in version string:"%words[2]

    # make a first pass through the file
    state="Making first pass through data to prepare arm fragments"
    dbprint(1, state)
    firstFragment = infile.tell()
    
    while fragnum < numfragments:
        ParseFragment()
        fragnum = fragnum + 1
    
    if GetVerbosity() > 4:
        for key in fragment_dict.keys():
            frag = fragment_dict[key]
            for fragkey in frag.keys():
                print "fragment %d: %s:"%(key,fragkey), frag[fragkey]
            
    # make a second pass throught the file
    state = "Making second pass through data to create arms"
    dbprint(1, state)
    for key in fragment_dict.keys():
        MakeNextArm(key)

    # pass throught the arms and finalize them for vis
    state = "Making final pass through data to analyze arms"
    dbprint(1, state)
    for key in arm_dict.keys():
        CategorizeArm(key)        

    # write the finalized arms out in cross-platform, binary format
    WriteArms()
    
except:
    print "Error while reading line %d in state \"%s\": \"%s\""%(lineno, state, line.strip())
    traceback.print_exc()
    sys.exit(1)

print "version is", version
print "line %d is \"%s\""%( lineno, line.strip())
