/*
** $RCSfile: pov2hts.c,v $
** $Name:  $
**
** ASCI Visualization Project 
**
** Lawrence Livermore National Laboratory
** Information Management and Graphics Group
** P.O. Box 808, Mail Stop L-561
** Livermore, CA 94551-0808
**
** For information about this project see:
** 	http://www.llnl.gov/sccd/lc/img/
**
**      or contact: asciviz@llnl.gov
**
** For copyright and disclaimer information see:
**      $(ASCIVIS_ROOT)/copyright_notice_1.txt
**
** 	or man llnl_copyright
**
** $Id: pov2hts.c,v 1.5 2006/02/27 16:40:50 rcook Exp $ 
**
*/
/*
**
**  Abstract:  For converting Meijie Tang's Worms of ParaDIS to hts format.
**
**  Author:    Rich Cook
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "HTSFile.h"



char gVerbose = 0;

//=========================================================================
void usage(void){
  fprintf (stderr, "pov2hts [-h(tscomp) mode ] [-m(axojbs) n] [-n(ewreg) regdirname] [-ra(dii) r1 r2] [-re(gfile) name] [-s(tripify) numstrips] [-v(erbose)] filename(s)\n");
  return;
}

//=========================================================================
void ex_error(char *nit){
  fprintf(stderr, "Arg %s needs a parameter...\n", nit);
  usage();
  exit(1);
}



//=========================================================================
int main (int argc, char *argv[]){
  // parse the arguments
  int argnum = 1;
  int complexity = -1;
  int nStrips = 0;
  long maxobjs = 0;
  int err = 0;
  float r1=1.0, r2=1.0;
  int	  htscomp = -1;
  char regdir[2048]="", oldregfile[2048] ="";
  
  if (argc > 100){
    fprintf(stderr, "Error: cannot process more than 100 files");
    exit(1);
  }
  
  while((argnum<argc) && (argv[argnum][0] == '-') && (strlen(argv[argnum])>1) ) {
    if (strstr(argv[argnum],"-c") != NULL) { // -c(omplexity) n
      if (++argnum >= argc) ex_error(argv[0]);
      complexity = atoi(argv[argnum]);
    }
    else if (strstr(argv[argnum],"-h") != NULL) { // -h(tscomp) mode 
      if (++argnum >= argc) ex_error(argv[0]);
      htscomp = atoi(argv[argnum]);
    }
    else if (strstr(argv[argnum],"-m") != NULL) { // -m(axobjs) n
      if (++argnum >= argc) ex_error(argv[0]);
      maxobjs = atoi(argv[argnum]);
    }
    else if (strstr(argv[argnum],"-n") != NULL) { // -n(ewregdir) name
      if (++argnum >= argc) ex_error(argv[0]);
      strncpy(regdir,argv[argnum], 2048);
      regdir[2047] = 0;
    }
    else if (strstr(argv[argnum],"-ra") != NULL) { // -ra(dii) r1 r2
      char **err = NULL;
      if (++argnum >= argc) ex_error(argv[0]);
      r1 = strtod(argv[argnum], err);
      if (err == &argv[argnum]) {
	fprintf(stderr, "Err: bad floating point number: %s", argv[argnum]);
	ex_error(argv[0]);
      }
      if (++argnum >= argc) ex_error(argv[0]);

      r2 = strtod(argv[argnum], err);
      if (err == &argv[argnum]) {
	fprintf(stderr, "Err: bad floating point number: %s", argv[argnum]);
	ex_error(argv[0]);
      }
    }
    else if (strstr(argv[argnum],"-re") != NULL) { // -re(gfile) name
      if (++argnum >= argc) ex_error(argv[0]);
      strncpy(oldregfile,argv[argnum], 2048);
      oldregfile[2047] = 0;
    }
    else if (strstr(argv[argnum],"-s") != NULL) { // -s(tripify) numstrips
      if (++argnum >= argc) ex_error(argv[0]);
      nStrips = atoi(argv[argnum]);
    }
    else if (strstr(argv[argnum],"-v") != NULL) { // -v(erbose)
      gVerbose = 1;     
    }
    else {
      break;
    }
    argnum++;
  }
  
  // loop through the filenames 
  char outfilename[2048]; 
  FILE *infile = NULL;
  if (argnum == argc) {
    fprintf(stderr, "are you forgetting something?  need a filename\n");
    exit(1);
  }
  while (argnum < argc) {
    
    // clean up after previous iterations
    if (infile){
      fclose(infile);
      infile = NULL;
    }
    
    char *infilename = argv[argnum++]; 
    printf("Importing file %s...\n", infilename);
    if (strcmp(&(infilename[strlen(infilename)-4]), ".pov")){
      fprintf(stderr, "Error:  input files must have .pov extension.");
      continue;
    }
    // open the povray file
    infile = fopen(infilename, "r");
    if (0 && !infile) {
      fprintf(stderr, "Error: Cannot open file %s\n", infilename);
      continue;
    }
    strcpy(outfilename, infilename);
    strcpy(&(outfilename[strlen(outfilename)-4]), ".hts");
    
    char *filename = "povray-data";
    struct HTSFile *filep = OpenHTSFile(outfilename, &filename, 1, htscomp);

    //parse the input file, and along the way cram stuff into the chunk
    char line[2048] = "";
    float location1[3], location2[3];
    int iradius, icolor;
    float values[2] = {1, 2};
    float radii[2] = {r1, r2};
    long linenum = 0, numspheres=0, numcyls=0;
    while (fgets(line, 2048, infile) != NULL && (!maxobjs || numspheres+numcyls<maxobjs)) {  
      linenum++;
      if (strstr(line, "sphere")){
	int numread = sscanf(line, 
			     "sphere{ <%f,%f,%f>, sphere_radius%d pigment { color color%d ", 
			     &location1[0], &location1[1], &location1[2], &iradius, &icolor);
	iradius-=1;
	if (numread != 5) {
	  fprintf(stderr, "Error: line %ld: bad line format: \"%s\"", linenum, line);
	  exit(1);
	}
	if (gVerbose){
	  fprintf(stdout, "sphere: location (%f, %f, %f), radius = %f, color = %f\n", 
		  location1[0], location1[1], location1[2], radii[iradius], values[icolor]);
	  fflush(stdout);
	}
	AddSphere(filep, location1, radii[iradius], &values[icolor], complexity, 0);
	numspheres++;
      }
      else if (strstr(line, "cylinder")){
	int numread = sscanf(line, 
			     "cylinder{ <%f,%f,%f>,<%f,%f,%f>,radius%d pigment { color color%d ", 
			     &location1[0], &location1[1], &location1[2], &location2[0], &location2[1], &location2[2], &iradius, &icolor);
	iradius-=1;
	if (numread != 8) {
	  fprintf(stderr, "Error: line %ld: bad line format: \"%s\"", linenum, line);
	  exit(1);
	}
	if (gVerbose){
	  fprintf(stdout, "cylinder: endpoints {(%f, %f, %f), (%f, %f, %f)}, radius = %f, color = %f\n",
		  location1[0], location1[1], location1[2], location2[0], location2[1], location2[2], radii[iradius], values[icolor]);
	  fflush(stdout);
	}
	AddCylinder(filep, location1, location2, radii[iradius], &values[icolor], complexity, 0);
	numcyls++;
      }
    }

    if ((err=CloseHTSFile(filep)) != 0)
      fprintf(stderr, "Error %d while trying to write output file %s\n", err, outfilename);
    
    fprintf(stdout, "done with %ld lines.  %ld spheres and %ld cylinders\n", linenum, numspheres, numcyls);

  }

  // done creating HTS file

  return 0;
}
