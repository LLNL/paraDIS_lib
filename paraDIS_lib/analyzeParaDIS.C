#include "Prefs.h"
#include "RC_c_lib/args.h"
#include "paradis_c_interface.h" // for testing
#include "paradis.h"
#include <string.h>
//#include <RC_c_lib/tclap_utils.h>
#include <tclap/CmdLine.h>
#include "tclap/Arg.h"
#include <boost/algorithm/string/erase.hpp> 

paraDIS::DataSet *gDataSet = NULL; 

// =======================================================================
void errexit(TCLAP::CmdLine &cmd, string msg) {
  cerr << endl << "*** ERROR *** : " << msg  << endl<< endl;
  cmd.getOutput()->usage(cmd); 
  cerr << endl << "*** ERROR *** : " << msg << endl << endl;
  exit(1); 
}

// =======================================================================
int main(int argc, char *argv[]) {     
 
  TCLAP::CmdLine  cmd("analyzes paraDIS dumpfiles to find M nodes and metaarms.", ' ', GetLibraryVersionString(argv[0])); 

  TCLAP::ValueArg<string> basenameFlag("b", "basename", "set output basename (default: if datafile = rs0001.data, then basename = rs0001)", false, "", "basename", cmd); 

  TCLAP::SwitchArg debugfilesFlag("d", "debugfiles", "dump out detailed debug files in output directory", cmd); 

  TCLAP::SwitchArg noDebugfilesFlag("", "no-debugfiles", "do NOT dump out detailed debug files in output directory", cmd); 

  TCLAP::SwitchArg doc("D", "doc", "Print out the documentation for paraDIS data types.", cmd); 

  //  TCLAP::SwitchArg noRenumber("", "no-renumber", "Do not renumber nodes.  Use this flag when debugging.", cmd); 

  TCLAP::ValueArg<string> fileoutFlag("o", "outfile", "Print output to outdir/filename.  If filename is stderr, then print to stderr. Default: outdir/basename.out", false, "", "filename", cmd); 

  TCLAP::SwitchArg fullout("F", "full", "Same as running --stats --debugfiles --tagfile --vtkfile.  Note that using --no-renumber can also help.", cmd); 

  TCLAP::ValueArg<int> numbins("n", "numbins", "during report, divide total arms into num bins by length and report on the total length in each bin", false, 0, "integer", cmd); 

  TCLAP::ValueArg<double> screwToleranceAngle("t", "screw-tolerance", "The allowed angular deviation in screw determination.  Default: 0.05", false, (0.05), "float", cmd); 

  TCLAP::ValueArg<string> outdirFlag("O", "outdir", "set output directory (default: basename + \"-debug\")", false, "", "dirname", cmd); 

  TCLAP::SwitchArg statsFlag("s", "stats", "print arm, metaarm files in output directory", cmd); 

  TCLAP::SwitchArg tagfileFlag("", "tagfile", "Output a new data file basename-tagged.data, identical to the input except that all node entries have the correct \"tag\" values.", cmd); 

  TCLAP::ValueArg<float> threshold("", "threshold", "report on number of arms of various types which have length less than the given threshold", false, -1.0, "FLOAT", cmd); 

  TCLAP::MultiArg<int> tracearms("", "trace-arm", "Write tracefiles for the arm with the given ID, including its neighbors (see -tracedepth)", false, "arm index", cmd); 

  TCLAP::MultiArg<int> tracenodes("", "trace-node", "Write tracefiles for the node with the given ID, including its neighbors (see -tracedepth)", false, "node index", cmd);  

  TCLAP::ValueArg<int> tracedepth("", "trace-depth", "Set the depth to which arm and node traces are done.  Default: 2", false, 2, "positive integer", cmd); 

  TCLAP::ValueArg<int> verbosityFlag("v", "verbosity", "set verbosity level to num (scale of 0-5, 5 is very verbose, 0 is strangely quiet).", false, -1, "integer", cmd); 

  TCLAP::SwitchArg versionNumber("", "version-num", "report version number and exit", cmd); 

  TCLAP::SwitchArg vtkfileFlag("V", "vtk-file", "export results to VTK fileset for VisIt parallel visualization", cmd); 

  TCLAP::UnlabeledMultiArg<string> dumpfiles("dumpfiles", "dump files name(s)", true, "dump file name(s)", cmd); 
 
  try {
	cmd.parse(argc, argv);
  } catch(...) {
    cmd.getOutput()->usage(cmd); 
	//std::cout << e.what() << std::endl;
	return 1;
  }
  
  try {
    if (versionNumber.getValue()) {
      cout << GetLibraryVersionNumberString() << endl; 
      exit(0);
    } else {
      cout << GetLibraryVersionString(argv[0]) << endl; 
    }

    if (doc.getValue()) {
      cerr << paraDIS::doctext << endl; 
      exit(0);
    }

    bool debugfiles = debugfilesFlag.getValue(); 
    bool stats = statsFlag.getValue(); 
    bool tagfile = tagfileFlag.getValue();
    int verbosity = verbosityFlag.getValue(); 
    bool vtkfile = vtkfileFlag.getValue(); 

    if (fullout.getValue()) {
      debugfiles = true; 
      stats = true; 
      tagfile = true;
      vtkfile = true; 
      if (verbosity == -1) {
        verbosity = 5; 
      }
    }

    if (noDebugfilesFlag.getValue()) {
      debugfiles = false; 
    }

    if (verbosity == -1) {
      verbosity = 0; 
    }
    if (verbosity) {
      cerr << "verbosity is " << verbosity << endl; 
    }
    for (vector<string>::const_iterator dfile = dumpfiles.getValue().begin(); dfile != dumpfiles.getValue().end(); dfile++) {
      string basename = basenameFlag.getValue(); 
      if (basename == "") {
        basename = *dfile; 
        boost::erase_last(basename, ".data"); 
        boost::erase_last(basename, ".dat"); 
        if (basename == *dfile) {          
          errexit(cmd, str(boost::format("Data file name %s does not end with '.dat' or '.data'.")%*dfile)); 
        }
      } 

      string fileout = fileoutFlag.getValue(); 
      if (fileout == "") fileout = basename + ".out"; 
      
      string outdir = outdirFlag.getValue(); 
      if (outdir == "") outdir = basename + "-output"; 
 
      gDataSet = new paraDIS::DataSet; 
      
      for (vector<int>::const_iterator num = tracearms.getValue().begin(); num != tracearms.getValue().end(); num++) {
        gDataSet->TraceArm(*num); 
      }
      
      gDataSet->SetTraceDepth(tracedepth.getValue()); 

      for (vector<int>::const_iterator num = tracenodes.getValue().begin(); num != tracenodes.getValue().end(); num++) {
        gDataSet->TraceNode(*num); 
      }
      
      gDataSet->SetOutputDir(outdir); 
      gDataSet->SetOutputBaseName(Basename(basename)); 
      gDataSet->SetDataFile(*dfile);     
      
      gDataSet->EnableDebugOutput(debugfiles); 
      gDataSet->EnableStatsOutput(stats); 
      gDataSet->EnableTagFileOutput(tagfile); 
      gDataSet->EnableVTKFileOutput(vtkfile); 
      
      if (verbosity) {
        gDataSet->SetVerbosity(verbosity, fileout); 
        fprintf(stderr, "Writing verbose output at level %d to file %s/%s\n", verbosity, outdir.c_str(), fileout.c_str()); 
      }
      gDataSet->SetThreshold(threshold.getValue());
      
      gDataSet->SetScrewToleranceAngle(screwToleranceAngle.getValue()); 

      gDataSet->SetProcNum(0, 1); 
      
      gDataSet->SetNumBins(numbins.getValue());
      
      gDataSet->ReadData("", false); 
      
      
      //======== BEGIN TEST CODE FROM DUMPFILE.C ========================
      /*  
          paraDIS_init(gDataSet); 
         #define vtkIdType uint32_t
          uint32_t nodenum = 0;
          uint32_t mNumMetaArmSegments = 0; // 0 based node IDs
          vtkIdType nodeIndices[2]; 
          uint32_t manum = 0, numMetaArms = paraDIS_GetNumMetaArms(); 
          dbprintf(5, "BEGIN TEST CODE: Checking %d metaarms\n", numMetaArms); 
          while (manum < numMetaArms) {
          if (manum == 44) {
          dbprintf(5, "manum == 44\n"); 
          }
          float *locations = paraDIS_GetMetaArmPoints(manum); 
          uint32_t segnum = 0, segsInArm = paraDIS_GetMetaArmNumSegments(manum); // sanity check
          while (!paraDIS_EndBuffCheck(locations)) {
          float *next = (locations+3); 
          if (*locations != WRAPPED_NODE && 
          *next != WRAPPED_NODE && 
          !paraDIS_EndBuffCheck(next) ) {
          nodeIndices[0] = nodenum;  
          nodeIndices[1] = nodenum+1; 
          dbprintf(5, "Dumpfile::GetMesh: MetaArm %d segment %d  node indices: (%d, %d)\n", manum, mNumMetaArmSegments,nodeIndices[0], nodeIndices[1]);  
          //linemesh->InsertNextCell(VTK_LINE, 2, nodeIndices);
          mNumMetaArmSegments++; 
          segnum++; 
          }
          if (*locations == WRAPPED_NODE) {
          dbprintf(5, "Dumpfile::GetMesh: MetaArm %d: Skip location: WRAPPED_NODE\n", manum); 
          locations++; 
          } else { // not wrapped 
          dbprintf(5, "Dumpfile::GetMesh: MetaArm %d points->InsertNextPoint %d at (%f, %f, %f);\n", manum, nodenum, locations[0], locations[1], locations[2]);  
          locations += 3; 
          nodenum++; 
          }
          if (segnum > segsInArm) {
          dbprintf(0, "ERROR: metaarm %d: numsegs (%d) has exceeded number of segements from paraDIS_GetMetaArmPoints (%d)\n", manum, segnum, segsInArm); 
          abort(); 
          }
          } 
          if (segnum != segsInArm) {
          dbprintf(0, "ERROR: metaarm %d: number of segments from paraDIS_GetMetaArmPoints (%d) does not match number of segments from paraDIS_GetMetaArmNumSegments() (%d, segsInArm = %d)\n", manum, segnum, paraDIS_GetMetaArmNumSegments(manum), segsInArm); 
          }
      
          dbprintf(5, "Done with metaarm %d\n\n", manum); 
          ++manum;
          }
      */
      //======== END TEST CODE FROM DUMPFILE.C ========================

      if (verbosity) {
        cerr << str(boost::format("Wrote verbose output to file %1%/%2%")%outdir%fileout) << endl; 
      }
    }
    
  } catch (string err) {
    cerr << "Error: " << err << endl; 
  }
  return 0; 
}
