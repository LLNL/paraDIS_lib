#!/usr/local/bin/python 

"""
Some commands salvaged from the old autosos.py system
"""

import sys, traceback, string, os, re, commands, popen2, select, time, pexpect, getnodes
from DebugUtils import *


#===========================================================
gPassword = ""
def getpasswd():
    import termios
    fd = sys.stdin.fileno()
    tc = termios.tcgetattr(fd)
    old = tc[3] & termios.ECHO
    tc[3] = tc[3] & ~termios.ECHO
    try:
        termios.tcsetattr(fd, termios.TCSADRAIN, tc)
        passwd = raw_input("Password\n>>> ")
    finally:
        tc[3] = tc[3] | old
        termios.tcsetattr(fd, termios.TCSADRAIN, tc)
    print
    return passwd


#============================================================
# send a file to the client host
def sendfile(filename):
    global remotehost
    command = "scp"
    args = [filename, "%s:%s"%(remotehost, os.path.basename(filename))]
    dbprint (1, "scp %s %s"%(args[0], args[1]))
    dbprint(3, command+' '+`args`)
    try:
        exps = []
        for regexp in ('yes/no', 'assword', '100%'):
            exps.append(re.compile(regexp))
        exps.append(pexpect.EOF)
    
        cmd = pexpect.spawn(command, args=args)
        result = None
        while result != 0:
            result = cmd.expect_list(exps, local_timeout=int(sshto))
            if (result == 0):#"yes/no"
                dbprint (2, 'matched "yes/no"')
                cmd.sendline('yes')
            elif (result == 1):#"assword"
                gPassword = getpasswd()
                cmd.sendline(gPassword)
            elif (result == 2):# '100%' -- success
                dbprint(2, "scp complete")
                break
            elif (result == 3): # probably good
                dbprint(2, cmd.before)
                break
        cmd.close()
    except:                
        print "an exception occured while scp'ing file", filename
        traceback.print_exc()        
        sys.exit(2)

#    cmd.read()
#=========================================================
# send the command file to the client host
sendfile(cmdfilename)    


#============================================================
#  launch client on remotehost

# first make the shell script to set display and launch client
securityCode = makeSecurityCode()

remotelog = "${HOME}/ensight8.log"
remotecommand = "%s/bin/ensight8.client -gold -p %s -security %s -cm -d %s"%(ceihome, os.path.basename(cmdfilename), securityCode, rdisplay)
if bash:
    remotescriptname = 'ensight.bash'
else:
    remotescriptname = 'ensight.csh'
    
remotescript = open(remotescriptname, "w")

shell = '/bin/csh'
if bash:
    shell = '/bin/bash'
    remotescript.write('#!/bin/bash\n')
    remotescript.write('export DISPLAY=%s\n'%rdisplay)
    remotescript.write('%s 2>&1 | tee %s\n'%(remotecommand, remotelog))    
else:
    remotescript = open(remotescriptname, "w")
    remotescript.write('#!/bin/csh\n')
    remotescript.write('setenv DISPLAY %s\n'%rdisplay)
    remotescript.write('%s |& tee %s\n'%(remotecommand, remotelog))

remotescript.close()
commands.getoutput('chmod 770 '+remotescriptname)
sendfile(remotescriptname)

args = [remotehost, shell, remotescriptname]
dbprint(2, "%s %s"%(ssh, string.join(args)))
try:
    exps = []
    expwords = ('<Ready>', 'yes/no', 'assword', 'not found', 'Error')
    for regexp in expwords:
        exps.append(re.compile(regexp))
    exps.append(pexpect.EOF)
    
    sshcmd = pexpect.spawn(ssh, args=args)
    result = None
    while result != 0:
        result = sshcmd.expect_list(exps, local_timeout=int(sshto))
        if (result == 0):# success
            dbprint(1, "remote client launched and ready for connection")
            break
        elif (result == 1):#"yes/no"
            dbprint (2, 'matched "yes/no"')
            sshcmd.sendline('yes')
        elif (result == 2):#"assword"
            if not gPassword:
                gPassword = getpasswd()
            sshcmd.sendline(gPassword)
        elif (result == 3):#'not found'
            errExit( string.strip(sshcmd.before+expwords[result]))
        elif result == 4:# "Error"
            errExit( "ERROR ********\nAn error occured:\n%s\nYou might want to check the log %s on the remote host for details"%(string.strip(sshcmd.before+expwords[result]+sshcmd.read()), remotelog))  
        elif result == 5:
            errExit( "ERROR ********\nunexpected EOF.  You might want to check the log %s on the remote host for details.  Here is the last thing I heard:\n%s"%(remotelog, sshcmd.before))
            
except:                
    print "an exception occured while running ssh ."
    traceback.print_exc()        
    print "sshcmd output:", sshcmd.before
    sys.exit(2)
    
# now the client will be listening... connect to client side

#=========================================================
#now launch EnSight SOS server
if '-d' in sys.argv:
    delay = int(sys.argv[sys.argv.index('-d')+1])
else:
    delay = 5
time.sleep(delay)

if numProcs <=8:
    threads = numProcs
else:
    threads = 8
    
if not '-nothread' in sys.argv:
    os.putenv('ENSIGHT8_MAX_SOSTHREADS', str(threads))
    print "ENSIGHT8_MAX_SOSTHREADS is", threads
else:
    print "ENSIGHT8_MAX_SOSTHREADS not set due to -nothread option in args"
print '\n*********'
print 'Launching SOS server...\n' 

sys.stdout.flush()

args = [sosbin, '-c', remotehost, '-security', securityCode, '-rsh', rsh]
args = args+['-ctries', '1000']
dbprint (2, sosbin + ' ' + string.join(args))
if '-noserver' in sys.argv:
    print "You have elected to run the server by hand.  Please use the following command:"
    print sosbin,
    for arg in args:
        print arg,
    print    
else:
    os.execv(sosbin,  args)

