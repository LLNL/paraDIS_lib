$Id: README_IMPORT_RESTART.SH,v 1.3 2007/06/28 21:46:07 rcook Exp $

==========================================================================================
IMPORTANT:  2007-06-28
import-restart.sh is now OBSOLETE.  import-restart.sh is for converting paraDIS dump files into a format that can be read by the TeraScale Browser.  A VisIt reader has been created to replace it. 



==========================================================================================
ORIGINAL NOTES FOR REFERENCE: 
This documents the typical usage of import-restart.sh, which is intended to simplify Meijie Tang's life by automating the process of converting a ParaDIS restart file into HTS files, then importing the results into a registry for use with the TeraScale Browser (TSB or just tsb). 
import-restart.sh -help should give you what you need to know.

Here is a typical invocation by Meijie Tang
import-restart.sh  -step 1 -first 1320 -last 1320 -inputtemplate ORIGI/restart-files/rs%04d.data -registry ORIGI/rs1320.reg -monsters yes -neighbors 4 -cylradius 100 -spheres yes -sphereradius 300  -armstats ORIGI/arms1320 -v 3 -keep no > ORIGI/logfile1320.txt 2>&1

# --------------------------
# filter files
Filter file format: 
If you only want to draw arms 1, 5, 7 through 9, and 45-78, you just put the following in the file:

1, 5, [7-9], [45-78]

Commas are optional and are just there for readability. Leaving them out is fine.  Multiple lines is fine. Numbers out of order are fine.  The only thing that is a problem is maybe making a range like [43-22], where the min is greater than the max.
