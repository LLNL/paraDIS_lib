/* $Id: restartConvert.C,v 1.122 2007/06/30 00:12:00 rcook Exp $ */
#include "restartConvert.h"
#include "args.h"
#include "debugutil.h"
#include "stringutil.h"
#include "timer.h"

/*
  #include <HTSFile.h>
  #include <HTSMesh.h>
*/ 
#include <vector>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sys/types.h>
#include <dirent.h>
#include "libparadis.h"

//Stripable *sjunk; 
using namespace std;
using namespace ParaDIS; 
  
//============================================================================
void filter::ParseFilterFile(char *filename, char verbose=0) {
  rangelist = NULL; 
  if (!filename || !*filename)
    return; 

  try {
    rangelist = new RangeList(filename); 
    if (!rangelist) throw string("allocation of rangelist failed in ParseFilterFile"); 
    if (!rangelist->ranges.size()) {
      delete rangelist; 
      rangelist = NULL;
    }
    else {
      rangelist->Sort();
      rangelist->Unique();
    }
  }
  catch (string err) {
    cerr << "Could not parse filter file due to error: " << endl; 
    cerr << err << endl; 
    rangelist = NULL;
  }

  
  return ;
 
}

//===========================================================
void print_version(void) {
  printf("This is restartConvert $Revision: 1.122 $ with header %s)\n", HEADER_ID);
  return ;
}
//======================================================================================

void usage(char *progname){
  printf("Usage: %s [options] dumpfile(s)\n", progname);
  printf("\tOptions (defaults given in parentheses):\n");
  printf("\t-armfilter filename (none): Only draw arms that have arm number listed in given filter file\n");
  printf("\t-armstats (off):  Print a formatted file with arm statistics for evaluation.\n");
  printf("\t-statsfile filename (armstats.txt): Use given name for armstats filename (see -armstats option above). \n");
  printf("\t-nodrawnstats  (off): Normally, we go through every arm segment after drawing and report some overall statistics about what was drawn and what types of arms were encountered.  This is four times slower than dumping HTS vertices for a given dataset,  but also very informative, so we enable it by default.  Use this flag to DISABLE this slow step.\n");
  printf("\t-c complexity (-1): the higher the complexity, the prettier, and the slower to draw.  -1 is ugly but fast.  10 would be pretty nice but very slow.\n");
  printf("\t-first num (0): start input and output number from num\n");
  printf("\t-h or -help (off): display this help menu\n");
  printf("\t-last num (0): end input and output number at num\n");
   printf("\t-monsters (off): output all arms that are terminated at either end by a four-armed or higher monster, or which are terminated at both ends by three armed nodes which are each the ends of a butterfly body\n");
  printf("\t-armweight (-1): ONLY used when outputting to povray format.  Arms are drawn only when their \"weight\" exceeds this value. -1 means to draw all arms, 0 means draw any MM or MN arms, and a 0.5 means draw only MM arms.\n");
  printf("\t-oldstyle (off): input files are old-style format\n");
  printf("\t-pov (off): output to povray files\n");
  printf("\t-sphrad radius (25.0): set radius of spheres to radius\n");
  printf("\t-cylrad radius (10.0): set radius of cylinders to radius\n");
  printf("\t-drawonly nth (1): If nonzero, then draw only every nth cylinder to the povray file (to create a sparse scene for speed rendering)\n");
  printf("\t-simplearms (off): Draw each arm as a straight line without drawing all the segments in the arm.\n");
  printf("\t-spheres (off): Draw spheres (nodes) as well as cylinders (connectivities) (if equal to 1, then draw every cylinder)\n");
  printf("\t-step num (10): step by num when loking for input file numbering.\n");
  printf("\t-template pattern (rs%%04d): Use pattern as the template for the input file names if no filenames are given on standard input.\n");
  printf("\t-v n: set verbosity level to n (0 is quiet, 5 is very verbose)\n");
  printf("\t-version (off):  Print program version info and exit.\n");
  printf("\t-xmin/xmax/ymin/ymax/zmin/zmax limit: set scene bounds (do not draw beyond given limit)\n");
  printf("\t-neighbors num (0):  The -neighbors num option controls output of worms based on the number of neighbors of nodes.  The argument 'num' has the following meaning.  Note that nodes are only drawn if -spheres is given as an argument. \n\
    We define an 'interior node' as a node having exactly two neighboring nodes, and an 'interior segment' as a segment with at least one interior node as its endpoints.  The vast majority of nodes and segments will be of this type.  'Terminal nodes' are all non-interior nodes, and 'terminal segments' are non-interior segments.  An 'arm' is a series of connected interior segments and nodes and the 0, 1 or 2 terminal nodes attached thereto.\n\
    num == 0 means output ALL worms and nodes  (if -spheres is given) (no filtering).  This is the default.\n\
    num == -1:  this is meaningless and will result in an error. \n\
    num == -2:  output all 'interior' segments and nodes (if -spheres given)\n\
    num < -2:  output all terminal nodes  (if -spheres given) with exactly abs(num) neighbors, and all arms connected to such nodes, minus interior nodes. \n\
    num == 1:  this is meaningless and will result in an error. \n\
    num > 1:  Act as if 0, -2, ... -n were given in separate invocations (which would be much slower, but have the same effect).\n\
    For example, a value of 3 yields a worm file with all worms in it, a file with only worms attached to single-neighbored nodes, and a file with only worms attached to nodes with two neighbors.\n" );
   return;
}


//======================================================================================
int main (int argc, char *argv[]){
  args_bool_t
    armstats = 0, 
    doPovray=0, // output povray files
	getbounds = 0,
    monsters = 0, 
    nodrawnstats=0, 
    oldstyle = 0, 
    showhelp = 0, 
	statsOnly = 0, 
	usetemplate = 0,
	version =0; 
  args_long_t verbose =0,	first=0, last=0, step=10;
  char userbasename[256]="converted_restart";
  char inputtemplate[512]="rs%04d";
  char armstatsname[512]="armstats.txt";
  char outfilebase[1024] = ""; 
  DataSet dataset;
  char filterfile[512] = ""; 
  char **unparsed_args = NULL; 
  
  argt args[27] = {
    {BOOL_TYPE, "-armstats", 1, &armstats},
    {BOOL_TYPE, "-nodrawnstats", 1, &nodrawnstats},
    {BOOL_TYPE, "-getbounds", 1, &getbounds},
    {BOOL_TYPE, "-h", 1, &showhelp},
    {BOOL_TYPE, "-help", 1, &showhelp},
    {BOOL_TYPE, "-monsters", 1, &monsters},
    {BOOL_TYPE, "-oldstyle", 1, &oldstyle},
    {BOOL_TYPE, "-pov", 1, &doPovray},
    {BOOL_TYPE, "-spheres", 1, &dataset._filter.drawspheres},
    {BOOL_TYPE, "-version", 1, &version},
    {DOUBLE_TYPE, "-armweight", 1, &dataset._filter.armweightfilter},
    {DOUBLE_TYPE, "-xmin", 1, &dataset._filter.min[0]},
    {DOUBLE_TYPE, "-ymin", 1, &dataset._filter.min[1]},
    {DOUBLE_TYPE, "-zmin", 1, &dataset._filter.min[2]},
    {DOUBLE_TYPE, "-xmax", 1, &dataset._filter.max[0]},
    {DOUBLE_TYPE, "-ymax", 1, &dataset._filter.max[1]},
    {DOUBLE_TYPE, "-zmax", 1, &dataset._filter.max[2]},
    {LONG_TYPE, "-drawonly", 1, &dataset._filter.drawonly},
    {LONG_TYPE, "-first", 1, &first},
    {LONG_TYPE, "-last", 1, &last},
    {LONG_TYPE, "-neighbors", 1, &dataset._filter.neighborfilter},
    {LONG_TYPE, "-step", 1, &step},
    {LONG_TYPE, "-v", 1, &verbose},
    {STRING_TYPE, "-name", 1, &userbasename},
    {STRING_TYPE, "-armfilter", 1, &filterfile},
    {STRING_TYPE, "-statsfile", 1, &armstatsname},
    {STRING_TYPE, "-template", 1, &inputtemplate}
  };
  
  arg_expect_args(args, sizeof(args)/sizeof(argt));
  int numunparsed = arg_parse_args(argc, argv, 1, &unparsed_args);
  if (numunparsed == -1) {
    print_version(); 
    usage(argv[0]);
	exit (1); 
  }
  if (version) {
    print_version(); 
    exit(0); 
  }
  if (showhelp) {
    print_version(); 
    usage(argv[0]);
    exit(0);
  }
  if (getbounds)
    verbose = 0; 
  else {
    print_version(); 
    print_args(argc, argv); 
    fflush(stdout);
  }
  if (dataset._filter.neighborfilter == 1 || dataset._filter.neighborfilter == -1 ) {
    fprintf(stderr, "Error:  neighbors cannot be -1 or 1.\n"); 
    usage(argv[0]); 
    exit(0); 
  }
int i=0; while (i<3) {
    if (dataset._filter.min[i] != DOUBLE_UNDEFINED || dataset._filter.max[i] != DOUBLE_UNDEFINED) {
      dataset._filter.filter_locations = true; 
      break; 
    }
    ++i; 
  }
  dbg_setverbose(verbose);
  
  if (!doPovray && !getbounds) {
	dbprintf(1, "No Povray or bounds query was placed, so I'm assuming you want arm stats...\n"); 
    statsOnly = true;
  }
  
  //parse filters to see if there are restrictions on which nodes to parse
  dataset._filter.ParseFilterFile(filterfile); 
  if (verbose > 4 && dataset._filter.rangelist) {
    cout << "The following ranges of arms will be processed:\n" << endl; 
    dataset._filter.rangelist->Print();
  }
  int err=0, numfiles = numunparsed;
  if (numfiles) {
	first = 0; /* just use all the unparsed args as filenames */ 
	last = numfiles - 1; 
	step = 1; 
  } else {
	/* use filename template -- no file names were given on command line */ 
    usetemplate = 1; 
    if (last < first){
      fprintf(stderr, "Warning: last file number is less than first.  Using first filenum and only doing one file.\n");
      last = first;
    }
    numfiles = 1+last-first; 
    
    char dirname[512];
    strcpy(dirname, inputtemplate);
    char *slash = strrchr(dirname, '/');
    if (slash) {
      *slash=0;
      if (strlen(slash)) {
    DIR *dir=opendir(dirname);
    if (!dir) {
      dbprintf(1, "Warning: attempting to create given input directory %s\n", dirname);
      char cmd[768] = "mkdir -p ";
      strcat(cmd, dirname);
      err=system(cmd); 
      if (err) {
        fprintf(stderr, "Error: directory %s does not exist and attempt to create it failed with error %d\n", dirname, err);
        exit(1);
      }
    }
    else
      closedir(dir);
      }
    }
  }
  if (nodrawnstats) dataset._reportStats = 0; 
  int filenum = first;
  if (strlen(userbasename) > 4 && 
      !strcmp(&userbasename[strlen(userbasename)-4], ".pov")){
    userbasename[strlen(userbasename)-4]=0;
  }
  while (filenum-first < numfiles) {
    if (numfiles != 1 && !usetemplate) 
      sprintf(outfilebase, "%s", userbasename);      
    else {
      int padding = (int)log10((float)(last?last:1))+1;
      char fmtstring[64]="";
      sprintf(fmtstring, "%%s_%%0%dd", padding);/* template */
      sprintf(outfilebase, fmtstring, userbasename, filenum);
    }
    char infilename[512];
    if (usetemplate)
      sprintf(infilename, inputtemplate, filenum);
    else
      strcpy(infilename, unparsed_args[filenum]);

    timer localTimer, globalTimer; 

    try {
      dbprintf(1, "processing file %s\n", infilename);
	  dataset.ResetData(); 
      dataset.GetBounds(infilename, oldstyle);
      if (getbounds) {
        int i=0; 
        while (i<3) 
          printf("%g ", dataset.min[i++]);
        i=0; 
        while (i<3) 
          printf("%g ", dataset.max[i++]);
        printf("\n");
        exit(0);
      }      
      globalTimer.start();
      dataset.ParseDumpFile(infilename, oldstyle);    
      dataset.CreateArms();

	  if (statsOnly) {
        dataset.FilterArms(); 
		dataset.WriteArmStats(armstatsname);
	  }	
      if (doPovray) {
        char outfile[512]; 
        sprintf(outfile, "%s.pov", outfilebase);
        dataset.FilterArms(); 
        if (armstats) {
          dataset.WriteArmStats(armstatsname);
        }
        dataset.DumpToPovray(outfile);
      }
    }
    catch (string err){
      cerr << err << endl;
      exit(2);
    }
    filenum+=step;
  }
  fprintf(stdout, "restartConvert completed at %s\n", datestring()); 
  return 0;
}
