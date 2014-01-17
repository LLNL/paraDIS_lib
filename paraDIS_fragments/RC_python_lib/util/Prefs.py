#!/usr/bin/env python
"""
$Id: Prefs.py,v 1.13 2007/07/16 23:51:07 rcook Exp $
This module is intended to supply a system for any program to get prefs from an init file, an environment variable, or its command line args.  Command line arguments take precedence over environment variables, which take precedence over init file values.
When get_prefs() is called, the module first parses the given init file.  The init file may give an argsmap file or include another init file, which will be interpreted at the point they are introduced in the file. There is no limit on such recursion.
Typical usage:

import sys, os
sys.path.append(os.environ['projdir']+'/RC_python_lib/util')
import Prefs
Prefs.AddVarsToArgsMap ( [
    ('verbosity', 'VERBOSE', ['-v', '--verbose'], 'overwrite', 0),
    ('help', '', ['-h', '--help', '-help'], 'bool')
    ] )

userprefs = Prefs.ReadAllSources()
SetVerbosity(userprefs['verbosity'])
args = Prefs.GetRemainingArgs()
dostuffwithargs(args)
"""

import sys, os, string, types

#=============================
# DEBUGGING stuff
from  DebugUtils import *

#=============================
# GLOBAL stuff

gPrefs = {} # prefs consisting of keyword/value pairs translated from all environment variables and command line args,  plus all keyword/value pairs from the init file 

# variables declared in the argsmap file end up in the following three dicts:
gArgMap = {} # dict of allowable command line arguments (from argsmap file) {argname:(varname, mode)}
gEnvMap = {} # dict of allowable environment variables (from argsmap file)  {envvarname:(varname, mode)}
gVarMap = {} # dict of special init file keywords (associated with gArgMap and gEnvMap, and possibly appendable/prependable) {varname:mode}

#====================================================================
# recites the options which are parsed by Initialize() function if it is used
def PrefsUsage():
    print "-init initfile ---  specify second init file location"
    print "-v verbosity ---  verbosity (high numbers are more verbose)"
    return


#=====================================
# used in AddVarsToArgsMap:
#=====================================
def TryMakeList(thing):    
    if not isinstance(thing, types.ListType):        
        return [thing]
    return thing

# =====================================================================
# the SHIZZNIT baby
# A variable description is a tuple of strings of the following form:
#  (varname, environment-variable-name, argument, mode, optionalDefault)
#
# as implied, optionalDefault is optional;  if it's not there, it will be assume to have a value of None
#
# To add a variable to the list of mapped options, do this:
# description=("varname", "envname", "args", "mode")
# 
# AddVarsToArgsMap(description)
# 
# To add multiple variables, just give it a list of tuples:
# desc1 = (varname2, envname1, arg1, mode1)
# desc2 = (varname2, envname2, arg2, mode2)
# AddVarsToArgsMap( [desc1, desc2] )
#
# As the name implies, you can call AddVarToArgsMap as many times as you want.  Note that if you add a description to the map that has the same varname as an already added description, it REPLACES the old description with the new one.
#
# You can have multiple envnames and varnames aliased to a variable by using lists instead of strings:
# desc3 = ("something", ["SOMETHING", "SOMETHINGELSE"], ["-sm", "-something", "--something"], "bool")
#
# Leaving an empty list or blank string means there is no corresponding choice, not a problem.  
#
# Variable descriptions can also be supplied in an "argsmap" file, see argsmap_sample for how to do that.
#
# Allowable modes: "overwrite", "append", "prepend", "and", "or", "overwrite", "bool"
#
#=====================================
def AddVarsToArgsMap(vardescriptions):
    dbprint(5, 'AddVarsToArgsMap called')
    if isinstance(vardescriptions, types.ListType):
        dbprint(5, "vardescriptions is a list: %s"%str(vardescriptions))
        for vardesc in vardescriptions:
            AddVarsToArgsMap(vardesc)
        return
    dbprint(5, "vardescriptions is "+str(vardescriptions))
    # add the description to the global mappings:
    varnames=TryMakeList(vardescriptions[0])
    envnames=TryMakeList(vardescriptions[1])
    argnames=TryMakeList(vardescriptions[2])
    mode=vardescriptions[3]    
    for varname in varnames:
        if not varname: raise "Error:  variable name missing in variable description "+str(vardescriptions)
        dbprint (5, "gVarMap["+varname+"]=["+mode+", "+str(envnames)+", "+str(argnames)+"]")
        gVarMap[varname]=[mode, envnames, argnames]
        for envname in envnames:
            if not envname: continue
            for argname in argnames:
                if not argname: continue
                dbprint (5, "gEnvMap["+envname+"]=["+varname+", "+mode+"]")
                gEnvMap[envname]=[varname, mode]
        for argname in argnames:
            if not argname: continue
            dbprint (5, "gArgMap["+argname+"]=["+varname+", "+"mode]")
            gArgMap[argname]=[varname, mode]
        if len(vardescriptions) == 5:
            gPrefs[varname]=vardescriptions[4]
        else:
            gPrefs[varname]=None
            
    
#=====================================
# given a string, try to make it a python object, else leave it as a string
def CoerceToPython(arg):
    value = []
    try: # see if it's an int
        value = eval(arg)
    except:
        value = arg # assume it's a string
    return value

#=========================
def TryMakeListOfStrings(arg):
    # arg is a string.  It either is a simple string, in which case return [arg]:
    # or a list of unquoted strings, like
    # "[thing1, thing2, 4]" , in which case return a list of quoted strings, like
    # ["thing1", "thing2", "4"]
    if arg[0]=='[' and arg[-1] == ']':
        # it's a list of unquoted strings, parse it:
        newlist=[]
        elems = arg.split(',')
        elems[0]=elems[0][1:]
        elems[len(elems)-1] = elems[len(elems)-1][:-1]
        for elem in elems:
            # if quoted, the quotes will remain here!
            newlist.append(elem.strip()) 
        return newlist
    return [arg]
    
def ParseArgsmap(argsmap):
    dbprint(2, "******************************\nParsing .argsmap file: "+ argsmap)
    
    if not os.path.exists(argsmap):
        print "Warning!  .argsmap file %s not found.."%argsmap
        return

    argsmap = open(argsmap, 'r')
    descriptions = []
    for line in argsmap.readlines():#the file is small; read all at once        
	if not len(line) or line[0] == "#" or line[0] == "\n": continue
        words = string.split(line)
        if not words: continue
        if len(words)<3:
            dbprint (1, "*** Warning: malformed .argsmap line:\n\t%s"%line)
            continue
        descriptions.append((TryMakeListOfStrings(words[0]), TryMakeListOfStrings(words[1]), TryMakeListOfStrings(words[2]), words[3]))
    dbprint(5, "descriptions are %s\n"%str(descriptions))
    
    AddVarsToArgsMap(descriptions)
    dbprint (5, "After parsing args map, we have:\n")
    dbprint (5, "gEnvMap=%s"% gEnvMap)
    dbprint (5, "gArgMap=%s"% gArgMap)
    dbprint (5, "gVarMap=%s"% gVarMap)
    
    return


#=========================
def AddToPrefs(varname, arg=None):
    dbprint(5, "AddToPrefs: setting arg for var %s to %s"%(varname, arg))
    if gVarMap.has_key(varname):
        mode = gVarMap[varname][0]
    else:
        if arg==None: # assume bool mode; set to true
            mode = 'bool'
        else:
            mode = 'overwrite'

    dbprint(5, "mode is %s"%mode)

    if mode == 'bool':
        gPrefs[varname] = (arg != 'false')
        return
        
    if mode == "overwrite":
        gPrefs[varname] = arg
        return

    if (mode == 'prepend' or mode == 'append' ):
        if type(arg) != types.ListType:
            if arg==None:
                arg=[]
            else:
                arg=[arg]
        if not gPrefs.has_key(varname):
            gPrefs[varname]=arg
        elif mode == 'prepend':
            gPrefs[varname]=arg+gPrefs[varname]
        else:
            gPrefs[varname]=gPrefs[varname]+arg
        return

    if (mode == 'and' or mode == 'or'):
        argtruth=(arg and arg!='false')
        preftruth=(gPrefs.has_key(varname) and gPrefs[varname] and gPrefs[varname]!='false')
        if mode == 'and':
            gPrefs[varname] = argtruth and preftruth
        else:
            gPrefs[varname] = argtruth or preftruth
        return
    raise ("Unknown mode: %s"%mode)
    

#=========================
def ReadInitFile(initfilename):
    dbprint(2, "ReadInitFile: initfilename is %s"%str(initfilename))
    #ReadInitFile is called before args are parsed
    if not os.path.exists(initfilename):
        dbprint (0, "***************WARNING!  Init file %s not found.  Attempting to continue without parsing an init file." % initfilename)
        return 1
    
    theInitFile = open(initfilename, 'r')
    for line in theInitFile.xreadlines():
	if not len(line) or line[0] == "#" or line[0] == "\n": continue
        dbprint(5, "found valid line:\"%s\""%line)
	words = string.split(line)
        dbprint(5, 'words: %s'%words)
	if not len(words):  continue        

	varname = words[0]
        arg = None
	if len(words) > 1:
            if len(words) > 2:
                words[1]=string.join(words[1:])
            
            arg = CoerceToPython(words[1])

        if (varname == "include" or varname == "argsmap"):
            # if relative path is given, arg is relative to init file location.
            if arg and arg[0] != '/' and string.rfind(initfilename, '/'):
                arg=initfilename[0:string.rfind(initfilename, '/')+1] + arg
            if varname == "include":
                ReadInitFile(arg)
            else:
                ParseArgsmap(arg)
        else: 
            AddToPrefs(varname, arg)
            
    dbprint (2, "resulting dictionary:\n%s"%gPrefs)
    return 1

#=====================
# sets gPrefs["Args"] to all args starting from the first argument greater than 0 not beginning with a dash, or [] if no such arg exists.  Most programs have some args that are required but are not flags, this points to the first non-flag argument.
# e.g. for:
# program -v 5 filename
# gPrefs['Args'] would be set to ['filename'], because "filename" is first non-dashed argument ("5" belongs to "-v")
# for :
# program --help
# gPrefs['Args'] would be set to []
def ReadArgs():
    dbprint(2,"Beginning ReadArgs()...")
    dbprint(2, "Args are %s"%sys.argv)
    argnum=0
    numargs = len(sys.argv)    
    if numargs>1:
        argnum = 1
        while argnum < numargs:
            theToken=sys.argv[argnum]
            dbprint(5, "theToken is "+theToken+" and argnum is "+str(argnum))
            if theToken[0] != '-':
                argnum = argnum-1
                break
            argnum = argnum+1
             # it's a flag of some sort, so create a new variable with the name of the flag
            if not gArgMap.has_key(theToken):
                dbprint(5, "theToken is not found in the gArgMap")
                flagname=theToken
                varname=flagname[1:]
                envname=varname.upper()
                if argnum < numargs and sys.argv[argnum][0] != '-':
                    # the next arg is not a flag, assume it's meant to be the value of the new flag:
                    value=sys.argv[argnum]
                    argnum = argnum+1
                    mode = "append" # args are parsed last, usually, so this isn't a big deal
                else:
                    value=1
                    mode = "bool"
                AddVarsToArgsMap((varname, envname, flagname, mode))

            varname=gArgMap[theToken][0]
            mode = gArgMap[theToken][1]
            dbprint(5, "mode is %s"%mode)
            if mode == 'bool':
                dbprint(5, "mode is %s and varname is %s"%(mode, varname))
                AddToPrefs(varname)               
            elif argnum < numargs:
                arg = CoerceToPython(sys.argv[argnum])
                argnum = argnum+1
                dbprint(5, "mode is %s, varname is %s, and arg is %s"%(mode, varname, arg))
                AddToPrefs(varname, arg)
            else:                
                raise "Error: argument %s is missing an argument"%theToken
    AddToPrefs("Args", sys.argv[argnum+1:])
    return

#===========================================================
def ReadEnvVars():
    dbprint(2, "Beginning ReadEnvVars...");
    for word in gEnvMap.keys():
        if os.environ.has_key(word):
            dbprint(3, "Found environment var: %s"%word)
            varname=gEnvMap[word][0]
            arg = None
            if os.environ[word]:
                arg=CoerceToPython(os.environ[word])
            AddToPrefs(varname, arg)
    
    return


#===========================================================
def ReadAllSources(initfile=None):
    if initfile:
        ReadInitFile(initfile)
    ReadEnvVars()
    ReadArgs()
    return gPrefs
            
#===========================================================
# keep this around, although it's deprecated, because TestInfrastructure uses it:
def Initialize(mandatoryinitfile = None):
    if '-v' in sys.argv:
        SetVerbosity(int(sys.argv[sys.argv.index('-v')+1]))

    if mandatoryinitfile:
        ReadInitFile(mandatoryinitfile)
        
    if "-init" in sys.argv:
        initfile=sys.argv[sys.argv.index("-init")+1]
        ReadInitFile(initfile)

    return
        
#===========================================================
def GetPrefs():    
    return gPrefs

#===========================================================
def GetRemainingArgs():
    try:
        return gPrefs['Args']
    except:
        return sys.argv[1:]
    
#===========================================================
def ModuleTest():
    print  "********************************"
    print "This is Prefs.ModuleTest()\n"
    print "If called as Prefs.py -switch \"blah blah\", the expected env, and args, gPrefs should be:"
    print "\t{'AndableDummy': False, 'ClobberDummy': 123456.789, 'StringDummy': 'Notice how there are no quotes around this string!', 'AliasedVariable': ['Value from init file', 'blah blah'], 'PrependableDummy': ['prepend', 'to', 'me'], 'include_var': 'I am an include variable', 'OrableDummy': False, 'AppendableDummy': ['append', 'to', 'me']}"
    expectedPrefs={'AndableDummy': False, 'ClobberDummy': 123456.789, 'StringDummy': 'Notice how there are no quotes around this string!', 'AliasedVariable': ['Value from init file', 'blah blah'], 'PrependableDummy': ['prepend', 'to', 'me'], 'include_var': 'I am an include variable', 'OrableDummy': False, 'AppendableDummy': ['append', 'to', 'me']}
    print "who knows?"
    print
    print "Getting preferences from initfile_sample, environment and program flags."
    dbcatch(Initialize, args=('initfile_sample',))
    dbcatch(ReadEnvVars)
    dbcatch(ReadArgs)
    prefs=GetPrefs()
    print 'After initfile_sample, env, and args, gPrefs is:\n\t%s\n'% gPrefs
    print '\nget_prefs() done.'
    print
    if gPrefs == expectedPrefs:
        print '********************************' 
        print "SUCCESS"
        print '********************************' 
        return 0
    else:
        print '********************************' 
        print 'FAILURE (did you use the right command line options?)'
        print '********************************' 
        return 1

if __name__ == '__main__' :
    ModuleTest()
