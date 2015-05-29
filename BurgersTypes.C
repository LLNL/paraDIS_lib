#include "BurgersTypes.h"
// GRRR.  Visit hooks are lame.  This is bad code but if I don't structure it like this, the SVN hooks complain. 
#ifdef  USE_ABORT
#define errexit abort()
#define errexit1 abort()
#else
#define errexit return 
#define errexit1 return err
#endif 

#define paradis_assert(test) if (!(test)) { \
	dbecho(0, "ERROR: %s %d: failed test: "#test"\n", __FUNCTION__, __LINE__); \
	errexit; \
  }


vector<int> GetAllBurgersTypes(void) {
  vector<int> alltypes; 
  alltypes.push_back(BCC_BURGERS_UNKNOWN); 
  alltypes.push_back(BCC_BURGERS_NONE); 
  alltypes.push_back(BCC_BURGERS_PPP); 
  alltypes.push_back(BCC_BURGERS_PPM); 
  alltypes.push_back(BCC_BURGERS_PMP); 
  alltypes.push_back(BCC_BURGERS_PMM);
  alltypes.push_back(BCC_BURGERS_200);
  alltypes.push_back(BCC_BURGERS_020);
  alltypes.push_back(BCC_BURGERS_002); 
  alltypes.push_back(BCC_BURGERS_220); 
  alltypes.push_back(BCC_BURGERS_202); 
  alltypes.push_back(BCC_BURGERS_022); 
  alltypes.push_back(BCC_BURGERS_311); 
  alltypes.push_back(BCC_BURGERS_131); 
  alltypes.push_back(BCC_BURGERS_113); 
  alltypes.push_back(BCC_BURGERS_222); 
  alltypes.push_back(BCC_BURGERS_004); 
  alltypes.push_back(BCC_BURGERS_331); 
  alltypes.push_back(BCC_BURGERS_313); 
  alltypes.push_back(BCC_BURGERS_133); 
  alltypes.push_back(BCC_BURGERS_420); 
  alltypes.push_back(BCC_BURGERS_240); 
  alltypes.push_back(BCC_BURGERS_024); 
  alltypes.push_back(BCC_BURGERS_042); 
  alltypes.push_back(BCC_BURGERS_204); 
  alltypes.push_back(BCC_BURGERS_402); 
  return alltypes;
}

string BurgersTypeNames(int btype) {
  switch (btype) {
  case BCC_BURGERS_UNKNOWN  : return "UNKNOWN";
  case BCC_BURGERS_NONE     : return "NONE";
  case BCC_BURGERS_PPP      : return "PPP";
  case BCC_BURGERS_PPM      : return "PPM";
  case BCC_BURGERS_PMP      : return "PMP";
  case BCC_BURGERS_PMM      : return "PMM";
  case BCC_BURGERS_200      : return "200";
  case BCC_BURGERS_020      : return "020";
  case BCC_BURGERS_002      : return "002";
  case BCC_BURGERS_220      : return "220";
  case BCC_BURGERS_202      : return "202";
  case BCC_BURGERS_022      : return "022";
  case BCC_BURGERS_311      : return "311";
  case BCC_BURGERS_131      : return "131";
  case BCC_BURGERS_113      : return "113";
  case BCC_BURGERS_222      : return "222";
  case BCC_BURGERS_004      : return "004";
  case BCC_BURGERS_331      : return "331";
  case BCC_BURGERS_313      : return "313";
  case BCC_BURGERS_133      : return "133"; 
  case BCC_BURGERS_420      : return "420";  
  case BCC_BURGERS_240      : return "240";  
  case BCC_BURGERS_024      : return "024";  
  case BCC_BURGERS_042      : return "042";  
  case BCC_BURGERS_204      : return "204";  
  case BCC_BURGERS_402      : return "402";  
  default: return str(boost::format("UNKNOWN CODE %1%")%btype);
  }
}

string ArmTypeNames(int atype) {
  switch (atype) {
  case ARM_EMPTY         : return "EMPTY";
  case ARM_UNKNOWN        : return "UNKNOWN";
  case ARM_UNINTERESTING  : return "UNINTERESTING";
  case ARM_LOOP           : return "LOOP";
  case ARM_MM_111          : return "MM_111";
  case ARM_MN_111         : return "MN_111";
  case ARM_NN_111          : return "NN_111";
  case ARM_SHORT_NN_111   : return "SHORT_NN_111";
  default: return "UNKNOWN ARMTYPE";
  }
}

string MetaArmTypeNames(int mtype) {
  switch (mtype) {
  case METAARM_UNKNOWN             : return "METAARM_UNKNOWN";
  case METAARM_111                 : return "METAARM_111";
  case METAARM_LOOP_111            : return "METAARM_LOOP_111";
  case METAARM_LOOP_HIGH_ENERGY    : return "METAARM_LOOP_HIGH_ENERGY";
  default                          : return "METAARM ERROR";
  }
}; 

// =====================================================================
int InterpretBurgersType(vector<float> burg) {
  static int doBCC = -1; 
  if (doBCC < 0) {
    for (int i = 0; i<3; i++) {
      if (fabs(burg[i])>BURGERS_EPSILON) {
        if (fabs(burg[i]/0.577350 - (int)(burg[i]/0.577350))>BURGERS_EPSILON) {
          doBCC = 0; 
        }
        else {
          doBCC = 1; 
        }
        break; 
      }
    }
    if (doBCC < 0) {
      cerr << "Error:  Cannot determine if burgers vector is BCC or HCP" << endl; 
      errexit;
    }
  }
  if (doBCC) return InterpretBCCBurgersType(burg); 
  return InterpretHCPBurgersType(burg); 
}


// =====================================================================
int InterpretHCPBurgersType(vector<float> burg) {
  vector<int> catarray(3,0); 
  for (int i = 0; i<3; i++) {
    catarray[i] = burg[i]/0.577350;
    if (catarray[i] < -1 ) catarray[i] *= -1;
    if (abs(catarray[i]) > 4) {
      dbprintf(1, "\n\n********************************\n");
      dbprintf(1, "WARNING: Weird value %g encountered in Category\n", catarray[i]);
      dbprintf(1, "\n********************************\n\n");
    }
  }
  return 42;  
}

// =====================================================================
int InterpretBCCBurgersType(vector<float> burg) {
  int burgersType = BCC_BURGERS_UNKNOWN;
  vector<int> catarray(3,0); 
  for (int i = 0; i<3; i++) {
    catarray[i] = burg[i]/0.577350;
    if (catarray[i] < -1 ) catarray[i] *= -1;
    if (abs(catarray[i]) > 4) {
      dbprintf(1, "\n\n********************************\n");
      dbprintf(1, "WARNING: Weird value %g encountered in Category\n", catarray[i]);
      dbprintf(1, "\n********************************\n\n");
    }
  }
    
   if (abs(catarray[0]) == 2 && catarray[1] == 0 && catarray[2] == 0)
    burgersType = BCC_BURGERS_200;
  else if (catarray[0] == 0 && abs(catarray[1]) == 2 && catarray[2] == 0)
    burgersType = BCC_BURGERS_020;
  else if (catarray[0] == 0 && catarray[1] == 0 && abs(catarray[2]) == 2)
    burgersType = BCC_BURGERS_002;
  else if ((catarray[0] == 1 && catarray[1] == 1 && catarray[2] == 1) ||
           (catarray[0] == -1 && catarray[1] == -1 && catarray[2] == -1))
    burgersType = BCC_BURGERS_PPP;
  else if ((catarray[0] == 1 && catarray[1] == 1 && catarray[2] == -1) ||
           (catarray[0] == -1 && catarray[1] == -1 && catarray[2] == 1))
    burgersType = BCC_BURGERS_PPM;
  else if ((catarray[0] == 1 && catarray[1] == -1 && catarray[2] == 1) ||
           (catarray[0] == -1 && catarray[1] == 1 && catarray[2] == -1))
    burgersType = BCC_BURGERS_PMP;
  else if ((catarray[0] == 1 && catarray[1] == -1 && catarray[2] == -1) ||
           (catarray[0] == -1 && catarray[1] == 1 && catarray[2] == 1))
    burgersType = BCC_BURGERS_PMM;
  else if (abs(catarray[0]) == 2 && abs(catarray[1]) == 2 && catarray[2] == 0)
    burgersType = BCC_BURGERS_220;
  else if (abs(catarray[0]) == 2 && catarray[1] == 0 && abs(catarray[2]) == 2)
    burgersType = BCC_BURGERS_202;
  else if (catarray[0] == 0 && abs(catarray[1]) == 2 && abs(catarray[2]) == 2)
    burgersType = BCC_BURGERS_022;
  else if (abs(catarray[0]) == 2 && abs(catarray[1]) == 2 && abs(catarray[2]) == 2)
    burgersType = BCC_BURGERS_222;
  else if (catarray[0] == 3 && abs(catarray[1]) == 1 && abs(catarray[2]) == 1)
    burgersType = BCC_BURGERS_311;
  else if (abs(catarray[0]) == 1 && catarray[1] == 3 && abs(catarray[2]) == 1)
    burgersType = BCC_BURGERS_131;
  else if (abs(catarray[0]) == 1 && abs(catarray[1]) == 1 && catarray[2] == 3)
    burgersType = BCC_BURGERS_113;
  else if ((abs(catarray[0]) == 0 && abs(catarray[1]) == 0 && catarray[2] == 4) ||
           (abs(catarray[0]) == 0 && catarray[1] == 4 && abs(catarray[2]) == 0) ||
           (catarray[0] == 4 && abs(catarray[1]) == 0 && abs(catarray[2]) == 0) )
    burgersType = BCC_BURGERS_004;
   else if (abs(catarray[0]) == 1 && abs(catarray[1]) == 3 && catarray[2] == 3)
     burgersType = BCC_BURGERS_133;
   else if (abs(catarray[0]) == 3 && abs(catarray[1]) == 1 && catarray[2] == 3)
     burgersType = BCC_BURGERS_313;
   else if (abs(catarray[0]) == 3 && abs(catarray[1]) == 3 && catarray[2] == 1)
     burgersType = BCC_BURGERS_331;
   else if (abs(catarray[0]) == 0 && abs(catarray[1]) == 2 && catarray[2] == 4)
	 return BCC_BURGERS_024; 
   else if (abs(catarray[0]) == 0 && abs(catarray[1]) == 4 && catarray[2] == 2)
	 return BCC_BURGERS_042; 
   else if (abs(catarray[0]) == 2 && abs(catarray[1]) == 0 && catarray[2] == 4)
	 return BCC_BURGERS_204; 
   else if (abs(catarray[0]) == 2 && abs(catarray[1]) == 4 && catarray[2] == 0)
	 return BCC_BURGERS_240; 
   else if (abs(catarray[0]) == 4 && abs(catarray[1]) == 0 && catarray[2] == 2)
	 return BCC_BURGERS_402; 
   else if (abs(catarray[0]) == 4 && abs(catarray[1]) == 2 && catarray[2] == 0)
	 return BCC_BURGERS_420; 
  else {
    burgersType = BCC_BURGERS_UNKNOWN;
    dbprintf(3, "\n\n********************************\n");
    dbprintf(3, "ERROR: segment has unknown type: burgers = (%f, %f, %f), categories=(%d, %d, %d).  This will cause problems later in analysis.\n", burg[0], burg[1], burg[2], catarray[0], catarray[1], catarray[2]);
    dbprintf(3, "\n********************************\n\n");
	abort(); 
  }
  return burgersType;
}


// =====================================================================
// AngularDifference used in ScrewType calculations and blender rotations 
// Since the other length is often known in those contexts, we save computation by taking it as a parameter

double AngularDifference(vector<float>v1, vector<float>v2, double v1Length, double v2Length) {
  dbprintf(5, str(boost::format("AngularDifference(<%f %f %f>, <%f, %f, %f>, %f, %f)\n") 
				  % v1[0] % v1[1] % v1[2] % v2[0] % v2[1] % v2[2] 
				  % v1Length % v2Length).c_str()); 
				  
  double dotprod = 0.0; //dot product of segment vec and other vec
  for (uint i=0; i<3; i++) {
	dotprod += v1[i]*v2[i]; 
  }
  if (v1Length < 0) {
	// compute length of v1:
	double sum = 0; 
	for (int i=0; i<3; i++) {
	  sum += v1[i]*v1[i];
	}
	v1Length = sqrt(sum); 
  }
  if (v2Length < 0) {
	// compute length of v2:
	double sum = 0; 
	for (int i=0; i<3; i++) {
	  sum += v2[i]*v2[i];
	}
	v2Length = sqrt(sum); 
  }
  double ratio = dotprod / (v1Length * v2Length); 
  double theta = acos(ratio); // always positive
  dbprintf(5, str(boost::format("AngularDifference: dotprod = %f, v1Length = %f, v2Length = %f, ratio = %f, theta = %f\n") % dotprod % v1Length % v2Length % ratio % theta).c_str()); 
  return theta; 
}
