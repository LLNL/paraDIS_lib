#!/bin/env python

"""
$Id: psubber.py,v 1.3 2005/08/15 22:52:33 rcook Exp $
Class to handle submitting of arbitrary batch commands via python on SLURM
The command given is not automatically run under srun.  If you wish srun to get involved,
you must specify this yourself. 
Written by Rich Cook
This script relies on the pscript.sh tool, also by Rich Cook, being in the user's PATH.
It is assumed to be running on a machine which shares a home directory with the machine on which the job will run, in order to place the shell script there.  
"""

import string, sys, time, tempfile, os
from popen2 import *
from exceptions import *
from DebugUtils import *

debug = 0

class PSub:
    mPrefsDirectory = ''
    mProgName = ''
    mNumNodes = 0
    mScriptName = ''
    def __init__ (self, progname=None, numnodes=0):
        self.SetProgramToRun(progname)
        self.SetNumNodes(numnodes)
        return

    def SetProgramToRun(self, progname):
        self.mProgName = progname
        return

    def SetNumNodes(self, numnodes):
        self.mNumNodes = int(numnodes)
        return
    
    def MakePrefsDirectory(self):
        # create a directory for all files and preferences
        try:
            self.mPrefsDirectory = os.environ['HOME'] + '/.psubber'
            if not os.path.exists(self.mPrefsDirectory):
                os.mkdir(self.mPrefsDirectory)
                if not os.path.exists(self.mPrefsDirectory):
                    raise Exception("Cannot create prefs directory "+self.mPrefsDirectory)
        except:
            PrependRaise("MakePrefsDirectory failed")
            
        return
    
    def PrepareShellScript(self):
        # create a shell script which will be the command given to psub
        try:
            if not self.mProgName:
                raise Exception("No program name given in PrepareShellScript")
            
            self.MakePrefsDirectory()
            # make up temporary script name
            self.mScriptName =  self.mPrefsDirectory + '/' + os.path.basename(tempfile.mktemp('.psub-script'))
            script = open(self.mScriptName, 'w')
            script.write("#!/bin/env bash\n")
            script.write(self.mProgName + '\n')
            script.close()
            os.system('chmod 770 '+self.mScriptName)
        except:
            PrependRaise("PrepareShellScript failed")
        return
   
        # make the directory 
    def submit(self): 
        # run psub, which echoes our job id to stdout
        self.PrepareShellScript()
        job_id = None
        command = 'pscript.sh -c '+self.mScriptName + ' -n '+str(self.mNumNodes)
        if debug:
            command = command + ' -g'
        psub = Popen3(command, 1)
        code = psub.wait()
        err = psub.childerr.read()
        if code:
            if code:
                print "Error:  psub exited with nonzero status:", code
            if err:
                print err

        else:    
            result = psub.fromchild.read().split()
            i = 0
            for word in result:
                if word == 'Job':
                    try:
                        job_id = int(result[i+1])
                    except:
                        PrependRaise("Error:  job job id %s does not appear to be a number."%result[i+1])                    
                    break
                i = i+1
            if not id:
                print "Error:  cannot get job job_id from result of psub"

            # while our job has not exited,  run pstat and check the status of our job id
            while 1:
                time.sleep(2)
                pstat = Popen3("pstat", 1)
                code = pstat.wait()
                err = pstat.childerr.read()
                if err:
                    raise "Error:  pstat exited with nonzero status: " + str(err)
                if code:
                    print "Job is complete (exit status %d)."%code 
                    return 0
        
                pstat_out = pstat.fromchild.read()
                result = pstat_out.split()
                try:
                    status_idx = result.index(str(job_id))
                except:
                    PrependRaise("Error:  cannot find job ID %d in pstat results: \n"%job_id + str(result))                   
    
                try:
                    status = result[status_idx+4]
                except:
                    raise "Error:  there is no fifth field in pstat output: \n" + pstat_out
    
                if status == "RUN" or status == "STAGING" or status == "WCPU" or status == "*ELIG":
                    print "Job status:", status
                    continue
                else:
                    raise "Error:  job has unknown status: " + status
                
        
if __name__ == "__main__":
    tester = PSub("srun /g/g0/rcook/bin/tester.sh", 4)
    tester.submit()
    
