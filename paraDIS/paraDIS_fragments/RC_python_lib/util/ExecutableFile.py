""" this is the AscivisExecutable module by Rich Cook for LLNL VIEWS project.
Implements AscivisExecutable, a subclass of TSB_File; see
AscivisExecutable.AscivisExecutable.__doc__ for info about that class."""

import os,sys, string, popen2, select, time,re

gDebug = 0

def info():
    print "this is the AscivisExecutable module by Rich Cook for LLNL VIEWS project."
    return    

#===================================================================
#WaitForCommand and CheckForOutput:
# Intended to wait for long processes and print their outputs in real-time.
# Invented for the case of the build, to see it happen.  If the elaborate
# polling done here is not done, then there's no way to measure build
# progress until it is done completely.

def CheckForOutput(theProcess, fdstdout, fdstderr, bufsize, NoStdErr=None, Verbose='yes', stdoutwrite=sys.stdout.write):
    """Checks to see if a process has any output waiting on stdout or stderr.
    If Verbose mode is on, it prints the output.  If NoStdErr is on, then
    any output to stderr generates an exception."""

    if gDebug:
        print "CheckForOutput"
    selectables = [fdstdout, fdstderr]
    (input, output, exc) = ([],[],[])
    #note that the output of theProcess is the 'input' of select.select, so
    # the return values look funny, but they're right
    (output, input, exc) = select.select(selectables,[], selectables)
    while output:
        for theOutput in output:
            if theOutput == fdstderr:
                if NoStdErr:
                    raise ("WaitForCommand-- Error: "+
                           "read std err while NoStdErr is true")
                eoutput = os.read(fdstderr, 50)
                if Verbose and eoutput:
                    if gDebug:
                        stdoutwrite("testing stdoutwrite")
                    stdoutwrite(eoutput)
                    if gDebug:
                        stdoutwrite("testing stdoutwrite")
            elif theOutput == fdstdout:
                stdoutput = os.read(fdstdout, 50)
                if Verbose and stdoutput:
                   if gDebug:
                        stdoutwrite("testing stdoutwrite")
                   stdoutwrite( stdoutput)
                   if gDebug:
                        stdoutwrite("testing stdoutwrite")
            sys.stdout.flush()
            (output, input, exc) = select.select(selectables,[], selectables)

        if exc: #need this or we never exit the loop for some reason
            if gDebug:
                stdoutwrite("exc is" + str(exc) +'\n')
            break
    if gDebug:
        stdoutwrite( "exiting CheckForOutput" + '\n')        
    return    

def WaitForCommand(iCommand, NoStdErr=None, RaiseExceptionOnErr=None, Verbose=None, bufsize=-1, stdoutwrite=sys.stdout.write):
    """run a lengthy command.  Wait for it to finish.  By default, stderr is
    also captured and written to stdout, but if NoStdErr is set, then any
    stderr will raise an exception, which is useful for commands such as
    mkdir that you don't expect stderr from.  Also, if RaiseExceptionOnErr
    is true, then an exception is raised by a process exiting with a
    non-zero return value."""

    theProcess = popen2.Popen3(iCommand,capturestderr=1, bufsize=bufsize)
    fdstderr = theProcess.childerr.fileno()
    fdstdout = theProcess.fromchild.fileno()
    while 1:
        returnCode = theProcess.poll()
        CheckForOutput(theProcess, fdstderr, fdstdout, bufsize, NoStdErr, Verbose, stdoutwrite)
        if gDebug:
            print "returnCode=", returnCode
        if returnCode != -1:
            if gDebug:
                print "return code is not -1"
            if returnCode and RaiseExceptionOnErr:
                raise 'Error: return code is ' + str(returnCode)
            if Verbose:
                stdoutwrite('return code is '+ str(returnCode) + '\n')
            break
    return


#===========================================================================

class AscivisExecutable():
    """AscivisExecutable inherits from TSB_File; it implements an interface for
    interfacing with executable files.  Only gives output of command if you
    explicitly ask for it.  For longer non-interactive commands whose
    output you want to watch in real-time, you should use WaitForCommand()
    above."""
    
    mProcess = None# will be a Popen3 object from popen2 module
    mArgs = None
    mPath = None
    mRegisterPID=None
    
    def __init__(self, env=None, listeners=None, tsbtest=None, path=None, args=None):
        if tsbtest:
            listeners = tsbtest.GetListeners()
            env = tsbtest.GetEnvironment()
        elif not listeners and not env:
            print "tsbtest:", tsbtest
            print "env:", env
            print "listeners:", listeners
            sys.stdout.flush()
            raise "Error:need either tsbtest or listeners and env args to AscivisExecutable"
        
        TSB_File.TSB_File.__init__(self, env, listeners, path=path)
        self.mArgs = args
        return


    def SetArgs(self, iArgs):
        self.mArgs = iArgs
        return

    def GetArgs(self):
        if self.mArgs:
            theArgs=self.mArgs[:]
            return theArgs
        return ""
    
    def IsExecutable(self):
        "double-check to ensure that the command is actually executable."
        #call self.Exists() to be sure that path is fully resolved first:
        if self.Exists() and  os.access(self.mPath, os.X_OK):            
            return 1

        return 0

    def ResolveFromPath(self):
        """Resolves executable's internal path first with the
        'BinInstallDirectory' global var (where the buildTSB module places
        its binaries) and then with `/bin/sh -c whence` Returns 1 if the
        path exists as is, 2 if the file is in the daily_bin directory, 3
        if it is in the user's PATH, and 0 if it cannot be resolved."""

        theDir = self.GetEnvInfoString("BinaryPaths")
        if theDir:
            theFile = theDir + '/' + self.mPath
            if os.path.exists(theFile):
                self.mPath = os.path.abspath(theFile)     
                return 2

        resolved = None
        try:
            resolved = os.popen("whence " + self.mPath + ' 2> /dev/null')
            resolved = resolved.read()
            #if not found, resolved will be null string ""
        except:
            resolved = None
        if not resolved:
            try:
                resolved = os.popen("which  " + self.mPath + ' 2> /dev/null')
                resolved = resolved.read()
                #if not found, resolved will be null string ""
            except:
                resolved = None
        if not resolved:
            try:     
                resolved = os.popen("type " + self.mPath + ' 2> /dev/null')
                resolved = string.split(resolved.read())
                if resolved[1] == 'is':
                    if resolved[3:5] == ['tracked', 'alias']:
                        #e.g., "grep is a tracked alias for /sbin/grep" (jackal)
                        resolved = resolved[6]
                    elif resolved[2] == ['hashed'] and resolved[3][0]=='(':
                        #e.g., "grep is hashed (/bin/grep)" 
                        length = len(resolved[3])
                        resolved = resolved[3][1:length]
                    elif resolved[3:5]== [ 'shell', 'builtin']:
                        #e.g.,  "type is a shell builtin" (all)
                        resolved = self.mPath
                    elif resolved[2][0] == '/':
                        #easiest:  e.g., "grep is /bin/grep" (all)
                        resolved = resolved[2]
                    else:
                        if gDebug:
                            print "got unknown response to 'type' command:", string.join(resolved)
                        return 0
            except:            
                if gDebug:
                    print "Warning:  both whence and type commands failed in Bourne shell.  Something is whacked."
                resolved = None
        if not resolved:
            return 0
        if resolved[-1] == '\n':
            self.mPath = resolved[:-1] #remove trailing '\n'
        else:
            self.mPath = resolved
        return 3
    
    def Exists(self):
        "determines if the executable exists."
        if not self.mPath:
            return 0

        if os.path.exists(self.mPath):
            if self.mPath[0] != '/':
                self.mPath = os.getcwd()+'/'+self.mPath
            return 1
        
        return  self.ResolveFromPath():

    
    def Start(self, args="", registerPID='yes'):
        """Execute the program and attach to it; return immediately.
        Normally the registerPID flag is set to true, so that if the
        process is somehow left running at end of all tests, it will be
        killed."""

        if not self.IsExecutable():
            raise "Error: Start(): not executable: '" + self.mPath +"'"
        elif gDebug:
            print "method used to resolve was", resolved
        if args:
            commandline = "%s %s" % (self.mPath, args)
        elif self.mArgs:
            commandline = "%s %s" % (self.mPath, self.mArgs)
        else:
            commandline = self.mPath        
        if gDebug:
            print "commandline is", commandline
        self.mProcess = popen2.Popen3(commandline, 1, bufsize=1)
        if registerPID:
            self.AppendEnvInfo('Running_PIDs', self.mProcess.pid)
            self.mRegisterPID=registerPID
        # to escape the shell, use [commandline] instead of commandline
        return self.mProcess
    
    def RunUntilFinished(self, timeout=None):
        """Start the process and wait until it has finished or until the
        timeout if given."""

        self.Start()
        self.WaitForFinish(timeout=timeout)
        return
    
    def IsFinished(self):
        "Check to see if the program has completed executation."
        if self.mProcess and self.mProcess.poll() == -1:
            return 0
        return 1
        
    def WaitForFinish(self, timeout):
        "Do not return until the program has exited or until time elapses."
        if timeout:
            startTime=time.time()
            while 1:
                elapsed=time.time()-startTime
                if theProcess.IsFinished() or elapsed>timeout:
                    return
        else:
            try:
                return self.mProcess.wait()
            except OSError: #process has already terminated
                return self.mProcess.poll()
        self.UnregisterPID()
        return

    def GetReturnCode(self, timeout=0):
        """If the program has exited, return its exit status, otherwise,
        return -1."""
        return self.mProcess.poll()

    def GetPID(self):
        if not self.mProcess:
            return None
        return self.mProcess.pid
    
    def SetStdIn(self, iString):
        "Write to the process stdin stream."        
        os.write( self.mProcess.tochild.fileno(), iString)
        return

    def GetStdErr(self, poll=None, numBytes=1000, until=None):
        """Read the process stderr stream.  If not poll, get all remaining
        stdout using read() (block until it's there).  Otherwise, poll
        'poll' times (if negative, poll until process completes) or until
        data is found and return the first numBytes from the successful
        poll result."""
        return self.GetStreamOut(poll=poll, numBytes=numBytes, stream='stderr', until=until)
        
    def GetStdOut(self, poll=None, numBytes=1000, until=None):
        """Read the process stderr stream.  If not poll, get all remaining
        stdout using read() (block until it's there).  Otherwise, poll
        'poll' times (if negative, poll until process complets) or until
        data is found and return the first numBytes from the successful
        poll result."""
        return self.GetStreamOut(poll=poll, numBytes=numBytes, stream='stdout', until=until)
        
    def GetStreamOut(self, poll=None, interval=None, numBytes=1000, stream='stdout', until=None):
        """Read the process stream (stderr or stdout).  If not poll, get all
        remaining stdout using read() (block until it's there).  Otherwise,
        poll 'poll' times (if negative, poll until process complets) or
        until data is found and return the first numBytes from the
        successful poll result.  If an 'until' string is given, read the
        stream with pollig interval until the string is seen."""

        if until:
            theChars=""
            pattern=re.compile(until)
            while 1:                
                c = self.GetStreamOut(poll=1, interval=interval, numBytes=1, stream=stream, until=None)
                theChars = theChars+c
                if pattern.search(theChars):
                    return theChars
                
        if stream=='stdout':
            theChild = self.mProcess.fromchild
        elif stream == 'stderr':
            theChild = self.mProcess.childerr
        else:
            raise "Unknown file descriptor " + stream
        
        if not poll:
            if gDebug:
                print "blocking for read()"
            return theChild.read()
        else:            
            theFD = theChild.fileno()
            out=""
            if gDebug:
                print "polling for read() on FD", theFD
            while poll:
                if self.mProcess.poll() != -1:
                    poll = 0
                result = select.select([theFD],[], [theFD], 0)
                if gDebug and result[0]:
                    print "got result"
                while result[0]:
                    if gDebug:
                        print "result is", result
                    outtemp = os.read(theFD, numBytes)
                    if result[2]:
                        while outtemp:
                            out = out+outtemp
                            outtemp = os.read(theFD, numBytes)
                        break;
                    out = out+outtemp
                    result = select.select([theFD],[], [theFD], 0)
                    
                if gDebug:
                    print "out is", out
                if out:
                    return out
                if poll > 0:#don't let it wrap around
                    poll = poll - 1
                if interval:
                    time.sleep(interval)
        return ""
    
            
    def Kill(self):
        'Kills the program if it is still running.'
        if not self.mProcess:
            return
        thePID = self.mProcess.pid
        if gDebug:
            print "Killing PID", thePID
        while self.mProcess.poll() == -1:
            time.sleep(0.5)
            try:
                os.kill(thePID, 9)
            except:
                pass

        self.UnregisterPID()
        self.mProcess = None
        return

    def UnregisterPID(self):
        """Remove our PID from the list of processes needing to be killed at
        end of all tests"""

        if self.mRegisterPID:
            self.RemoveEnvInfo('Running_PIDs', self.GetPID())
        return
    
    def __del__(self):
        'try to kill the process when my ref count reaches zero'
        self.Kill()
        return

    mPath = None

    
def ModuleTest():
    a = AscivisExecutable(path="ls")
    print a.Start()
    print a.WaitForFinish()
    print a.WaitForFinish()
