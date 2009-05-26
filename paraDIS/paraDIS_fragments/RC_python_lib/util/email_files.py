#!/usr/bin/env python
# requires python 2.2 or later or email package to be installed over 2.1

# usage:  email_files.py [-msgfile message.txt] [-msg "message text"] file1 file2 ...


#========================================================================
# stolen from http://www.bigbold.com/snippets/posts/show/2038
import smtplib
from email.MIMEMultipart import MIMEMultipart
from email.MIMEBase import MIMEBase
from email.MIMEText import MIMEText
from email.Utils import COMMASPACE, formatdate
from email import Encoders

def send_mail(send_from, send_to, subject, text, files=[], server="localhost"):
  assert type(send_to)==list
  assert type(files)==list

  msg = MIMEMultipart()
  msg['From'] = send_from
  msg['To'] = COMMASPACE.join(send_to)
  msg['Date'] = formatdate(localtime=True)
  msg['Subject'] = subject

  msg.attach( MIMEText(text) )

  for file in files:
    part = MIMEBase('application', "octet-stream")
    part.set_payload( open(file,"rb").read() )
    Encoders.encode_base64(part)
    part.add_header('Content-Disposition', 'attachment; filename="%s"' % os.path.basename(file))
    msg.attach(part)

  smtp = smtplib.SMTP(server)
  smtp.sendmail(send_from, send_to, msg.as_string())
  smtp.close()

#========================================================================
def usage():
    print "usage: email_files.py [-r recipient] [-mf/msgfile message.txt] [-m/--msgtext \"message body\"] [-s subject] [-server host] file1 file2 ..."
    print "to send to multiple recips, use -r flag multiple times"
    return


#========================================================================
# args parsing
if __name__ == '__main__' :
    import sys, os

    help=[]
    mtext=[]
    mtextfile=[]
    sender=[]
    subject=[]
    recips=[]
    from simpleargs import *
    boolopts={
        '-h': help,
        '-help': help
        }
    opts = {
        '-r': recips,
        '-m': mtext,
        '--msgtext': mtext,
        '-mf' : mtextfile,
        '--msgfile': mtextfile,
        '-s': subject,
        '-from': sender
        }
    try:
        firstfile=get_args(opts, boolopts)
    except:
        print "Error getting args:",
        usage()
        raise

    
    files=[]
    if firstfile:
        files=sys.argv[firstfile:]
        
    if help:
        usage()
        sys.exit(0)
        
    if not mtext and not mtextfile and len(sys.argv) <= 2:
        print "you must have at least one file or a message to send!"
        usage()
        sys.exit(1)
        
    if mtext and mtextfile:
        print "you can only use one of -mt and -mtf, not both"
        usage()
        sys.ext(2)

    if not recips:
        print "you must specify at least one recipient using -r flag"
        usage()
        sys.exit(3)

    text=""
    if mtext:
        text=mtext[0]

    if mtextfile:
        mtextfile=open(mtextfile[0],'r')
        text=mtextfile.read()

    # assume USER is set
    host=os.environ["HOST"]
    if sender:
        send_from=sender
    else:
        send_from=os.environ["USER"]+"@"+host+".llnl.gov"

    if not subject:
        subject="mail from "+send_from
    else:
        subject=subject[0]

    # legal mail servers on LC clusters are the cluster name plus "i" as far as I can tell
    if os.path.exists("/usr/global"):
      host=os.environ["CLUSTER"]+'i'
      
    send_mail(send_from, recips, subject, text, files, host)
    
