#!/usr/local/bin/python
# a useful module on batch nodes!  
from DebugUtils import *
import string, re, sys, os, commands, traceback

def usage():
    print "Usage: getnodes.py [options] "
    print " Options:"
    print "  -nofm  print as out put \"node m of n\" where n is total number of reserved nodes and m is our personal 1-based index into the list of reserved nodes, eg, \"node 2 of 8\""
    print "  -num  print our personal 1-based index into the list of reserved nodes (see -nofm option)"
    print "  -tot  print total number of reserved nodes (see -nofm option)"

def getnodes():
    processors = []
    try: # AIX batch
        processors =   os.environ['LOADL_PROCESSOR_LIST']
        dbprint (1, "found LOADL_PROCESSOR_LIST "    )
        dbprint (2, "LOADL_PROCESSOR_LIST is '"+processors+"'")
        processors = re.sub('[ ]+',',',processors)
        processors = string.split(processors, ',')
        for proc in processors:
            if not proc or proc[0] == ',':
                processors.remove(proc)
        return processors
    
    except: # SLURM batch job
        num = 0
        processors = []
        dbprint (1, "LOADL_PROCESSOR_LIST unavailable, using rmsquery and assuming pengra"    )
        try:
            id = string.strip(os.environ['SLURM_JOBID'])
        except:
            print "Error.  SLURM is not working as expected on this host.  Contact Rich Cook at 3-9605 and demand to know what is going on around here."
            print os.environ
            sys.stdout.flush()
            traceback.print_exc()
            sys.exit(1)
        
        table = string.split(commands.getoutput('squeue'),'\n')
        dbprint(3, 'table is '+`table`)
        dbprint (2,"table is "+ `table`)
        dbprint (2, "looking for id" + `id`)
        processors = []
        for line in table:
            words = string.split(line)
            if not words:
                continue
            dbprint (3, "words is"+ `words`)

            if  string.find(words[0], id) != -1 and string.find(words[0], id+'.') == -1:
                dbprint (3, "found cpus!"+ `words[7]`)
                if "[" not in words[7]:
                    processors = [words[7]]
                else:
                    cpuinfo = re.match('(.*)\[(.*)\]', words[7])
                    nodename = cpuinfo.group(1)
                    nodelist = cpuinfo.group(2)
                    dbprint (3, "nodename:"+ `nodename`)
                    dbprint (3, "nodelist:" + `nodelist`)
                break
        if not processors:
            for cpurange in string.split(nodelist, ','):
                cpurange = string.split(cpurange, '-')
                dbprint (3, 'current cpurange is'+ `cpurange`)
                if len(cpurange)==2:
                    dbprint(3, "cpurange is a range, not a number:"+`cpurange`)
                    cnum = int(cpurange[0])
                    cmax = int(cpurange[1])
                elif len(cpurange) == 1:
                    dbprint(3, "cpurange is a number, not a range:"+`cpurange`)
                    cmax = cnum = int(cpurange[0])
                else:
                    raise "cpurange (%s) is garbled"%`cpurange`
                while cnum <= cmax:
                    processors.append(nodename+str(cnum))
                    cnum = cnum+1
        dbprint (2, "processors:"+ `processors`)
        return processors

if __name__ == "__main__":
    # print a space-delimited list for unix tools to enjoy
    if "-v" in sys.argv:
        SetVerbosity(sys.argv[sys.index("-v")+1])

    nodes = getnodes()
        
    if "-nofm" in sys.argv:
        cmd = os.popen("uname -n");
        me = string.strip(cmd.read())
        m = len(nodes)
        n = nodes.index(me)+1
        print "node", n, "of", m
    elif "-num" in sys.argv:
        cmd = os.popen("uname -n");
        me = string.strip(cmd.read())
        n = nodes.index(me)+1
        print n
    elif "-tot" in sys.argv:
        cmd = os.popen("uname -n");
        me = string.strip(cmd.read())
        m = len(nodes)
        print m
    else:
        for node in nodes:
            print node,
        print
    
