#include "paradis.h"
#include "Prefs.h"
#include "RC_c_lib/args.h"
#include "paradis_version.h"
paraDIS::DataSet *gDataSet = NULL; 

void print_version(const char *progname) {
  fprintf(stderr, "%s: version %0.1f (%s)\n", progname, RC_PARADIS_VERSION, RC_PARADIS_VERSION_DATE); 
  return; 
}
void usage(void) {
  fprintf(stderr, "=========================================================\n"); 
  fprintf(stderr, "usage:  analyzeParaDIS [options] inputfile\n"); 
  fprintf(stderr, "---------------------------------------\n"); 
  fprintf(stderr, "options:  \n"); 
  fprintf(stderr, " -debugfiles:  dump out detailed analysis in files\n"); 
  fprintf(stderr, " -v (or -verbose) num:  set verbosity level to num (scale of 0-5, 5 is very verbose, 0 is strangely quiet)\n"); 
  fprintf(stderr, " -numbins num: during report, divide total arms into num bins by length and report on the total length in each bin\n"); 
fprintf(stderr, " -threshold num:  report on number of arms of various types which have length less than the given threshold\n"); 
  fprintf(stderr, " -version: report version and exit\n");
  return; 
}

int main(int argc, char *argv[]) {

  try {
    long debugfiles = 0, verbosity=0, numbins=0, help=false, version=false; 
    double threshold=-1.0; 
    argt args[7] = {
      {BOOL_TYPE, "-debugfiles", 1, &debugfiles}, 
      {BOOL_TYPE, "-help", 1, &help}, 
      {LONG_TYPE, "-numbins", 1, &numbins}, 
      {LONG_TYPE, "-v", 1, &verbosity}, 
      {LONG_TYPE, "-verbose", 1, &verbosity}, 
      {DOUBLE_TYPE, "-threshold", 1, &threshold},
      {BOOL_TYPE, "-version", 1, &version}
    }; 
    arg_expect_args(args, 7);
    arg_ignore_bad_args(1); 
    if (!arg_parse_args(&argc, argv)) {
      fprintf(stderr, "****************************************\n"); 
      fprintf(stderr, "Error in args_parse_args\n"); 
      usage(); 
      exit(1); 
    }
    if (help) {
      print_version(argv[0]); 
      usage(); 
      exit(0); 
    }
    if (version) {
      print_version(argv[0]); 
      exit(0);
    }
    if (argc < 2) {
      fprintf(stderr, "Error:  need a filename\n"); 
      usage(); 
      exit(1); 
    }
    
    gDataSet = new paraDIS::DataSet; 
    
    if (verbosity) {
      gDataSet->SetVerbosity(verbosity, "analyzeParaDIS-debug.txt"); 
    }
    gDataSet->SetThreshold(threshold);

    gDataSet->SetNumBins(numbins);

    gDataSet->SetDataFile(argv[1]); 
    
    gDataSet->EnableDebugOutput(debugfiles); 

    gDataSet->ReadData(); 
    
    gDataSet->PrintArmStats();

  } catch (string err) {
    cerr << "Error: " << err << endl; 
  }
  return 0; 
}
