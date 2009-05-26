"""
This module contains routines that make it easier to print errors, exceptions, etc.
"""
import traceback, sys, string

gVerbose = 0;
gOutput = sys.stdout

#============================
def PrependRaise(msg):
    raise Exception(str(msg) + "\n" + str(sys.exc_value))    
    
#============================
def AppendRaise(msg):
    raise Exception(str(sys.exc_value) + "\n" + str(msg) )

#============================
def db_to_stderr():
    global gOutput
    gOutput = sys.stderr
    return
#============================
def db_to_stdout():
    global gOutput
    gOutput = sys.stdout
    return
#============================
def dbprint(level, msg):
    global gOutput
    if gVerbose >= level:
        gOutput.write(sys.argv[0]+": "+ msg +"\n")
        gOutput.flush()
        return

#========================================================
def varprint(level, varname):
    dbprint(level, "%s is %s"%(varname, eval(varname)))
    return

#============================
def SetVerbosity(level):
    global gVerbose
    gVerbose=level
    dbprint(5, "setting verbosity to level %d"%int(gVerbose))
    return

#============================
def GetVerbosity():
    return gVerbose

#============================
def db_get_printable_exception():
    info = sys.exc_info()
    return "type: %s\nvalue: %s\ntraceback:\n%s\n"%(str(info[0]), str(info[1]), string.join(traceback.format_tb(info[2])))

#============================
def dbprint_exception(level=2):
    if gVerbose>=level:
        print db_get_printable_exception()
        #traceback.print_stack()
    return

#=========================
# dbcatch:  run a function, catching any exceptions and return 1, or return function value or 0 if function has no return value
# args is a sequence
def dbcatch(fun, args=None, kwargs=None, funreturns=0):
    dbprint(5, "args=%s"%str(args))
    dbprint(5, "kwargs=%s"%str(kwargs))
    dbprint(5, "funreturns=%s"%str(funreturns))
    try:
        if funreturns:
            if args:
                if kwargs:
                    return fun(*args, **kwargs)
                else:
                    return fun(*args)
            else:
                if kwargs:
                    return fun(**kwargs)
                else:
                    return fun()
        else:
            if args:
                if kwargs: 
                    fun(*args, **kwargs)
                else:
                    fun(*args)
            else:
                if kwargs:
                    fun(**kwargs)
                else:
                    fun()
    except:
        dbprint_exception()
        return 1
    return 0

#============================
