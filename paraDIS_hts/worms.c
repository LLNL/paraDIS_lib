/* 
Example file showing how HTSFile.h can be used to create many many worms for viewing in TSB
*/


#include "RC_c_lib/args.h"
#include "RC_c_lib/debugutil.h"
#include "HTSFile.h"
#include <stdio.h>

void usage(void){
  fprintf(stderr, "Usage: makeworms [options]\n");
  fprintf(stderr, "\tOptions:\n");
  fprintf(stderr, "\t-outfile: name of hts file to create\n");
  fprintf(stderr, "\t-dataname: name of data to appear in hts file\n");
  fprintf(stderr, "\t-xworms: number of worms to create in the x direction\n");
  fprintf(stderr, "\t-yworms: number of worms to create in the y direction\n");
  fprintf(stderr, "\t-zworms: number of worms to create in the z direction\n");
  fprintf(stderr, "\t-complex: complexity of each worm (must be at least 2)\n");
  return;
}

void MakeWorm(struct HTSFile *filep, float loc1[3], float loc2[3], float complexity, float cylradius, float sphereradius, float value){
  AddSphere(filep, loc1, sphereradius, &value, complexity, 0);
  AddCylinder(filep, loc1, loc2, cylradius, &value, complexity, 0);
  AddSphere(filep, loc2, sphereradius, &value, complexity, 0);
  return ; 
}

int main (int argc, char *argv[]){
  int xworms=44, yworms=44, zworms=44, complexity=-1; 
  float sphereradius=1.0, cylradius=1.0;
  char *filename = "80kworms.hts";
  char *dataname = "wormdata"; 
  long help=0; 

  argt args[9] = {{STRING_TYPE, "-outfile", 1, filename}, 
		  {STRING_TYPE, "-dataname", 1, dataname},
		  {DOUBLE_TYPE, "-sradius", 1, &sphereradius},
		  {DOUBLE_TYPE, "-cradius", 1, &cylradius},
		  {LONG_TYPE, "-xworms", 1, &xworms}, 
		  {LONG_TYPE, "-yworms", 1, &yworms}, 
		  {LONG_TYPE, "-zworms", 1, &zworms}, 
		  {LONG_TYPE, "-complex", 1, &complexity}, 
		  {BOOL_TYPE, "-help", 1, &help}
  };

  arg_expect_args(args, 9);
  arg_parse_args(argc, argv, 1);

  if (help){
    usage();
    exit(0);
  }

  struct HTSFile *filep = OpenHTSFile(filename, &dataname, 1, 0);
  if (!filep) {
    fprintf(stderr, "Cannot open file %s\n", filename);
    exit(1);
  }

  int wormnum = 0;

  //make 85,184 worms
  float loc1[3]={0, 0, 0}, loc2[3]={3.5, 0, 0}; 
  int i,j,k;
  for (k=0; k<zworms; k++){
    fprintf(stdout, "%d...\t", k); fflush(stdout);
    for (j=0; j<yworms; j++){
      for (i=0; i<xworms; i++) {
	loc1[0] += 5.0;
	loc2[0] += 5.0;
	MakeWorm(filep, loc1, loc2, complexity, cylradius, sphereradius, (i+j+k)%3);
	wormnum++;
      }
      loc1[1]+=3.0; 
      loc2[1]+=3.0;
      loc1[0] = 0; 
      loc2[0] = 3.5; 
    }
    loc1[2]+=3.0; 
    loc2[2]+=3.0;
    loc1[1] = 0; 
    loc2[1] = 0; 
  }

  fprintf(stdout, "Writing file...\n");
  CloseHTSFile(filep);
  fprintf(stdout, "\nCompleted %d worms.  File is %s\n", wormnum, filename);
  fflush(stdout);
  return 0;
}
