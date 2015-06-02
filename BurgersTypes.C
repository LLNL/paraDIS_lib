#include "BurgersTypes.h"
// GRRR.  Visit hooks are lame.  This is bad code but if I don't structure it like this, the SVN hooks complain. 
#ifdef  USE_ABORT
#define errexit abort()
#define errexit1 abort()
#else
#define errexit return 
#define errexit1 return err
#endif 

#define paradis_assert(test) if (!(test)) {                             \
	dbecho(0, "ERROR: %s %d: failed test: "#test"\n", __FUNCTION__, __LINE__); \
	errexit;                                                            \
  }


/* Per new info from Moono c. late May 2015 */ 
/* vector<vector<float> > hcpBurgersList {
  {-0.500000000, 0.86602540378, 0.0000000000}, 
  {1.0000000000, 0.0000000000, 0.0000000000},
  {-0.500000000, -0.86602540378, 0.0000000000},
  {-0.500000000, 0.86602540378, 1.5680000000},
  {1.0000000000, 0.0000000000, 1.5680000000},
  {-0.500000000, -0.86602540378, 1.5680000000},
  {-0.500000000, 0.86602540378, -1.5680000000},
  {1.0000000000, 0.0000000000, -1.5680000000},
  {-0.500000000, -0.86602540378, -1.5680000000},
  {0.0000000000, 0.0000000000, 1.5680000000},
  {-1.5000000000, 0.86602540378, 0.0000000000},
  {0.0000000000, 1.7320508076, 0.0000000000},
  {-1.0000000000, 1.7320508076, 1.5680000000},
  {-1.5000000000, 0.86602540378, -1.5680000000},
  {0.0000000000, 1.7320508076, -1.5680000000},
  {-1.0000000000, 1.7320508076, -1.5680000000},
  {-1.5000000000, 0.86602540378, 1.5680000000},
  {0.0000000000, 1.7320508076, 1.5680000000},
  {1.5000000000, 0.86602540378, 0.0000000000},
  {2.0000000000, 0.0000000000, 1.5680000000},
  {1.5000000000, 0.86602540378, -1.5680000000},
  {2.0000000000, 0.0000000000, -1.5680000000},
  {1.5000000000, 0.86602540378, 1.5680000000},
  {-1.0000000000, -1.7320508076, 1.5680000000},
  {-1.0000000000, -1.7320508076, -1.5680000000},
  {0.500000000, 0.86602540378, 3.1360000000},
  {-1.0000000000, 0.0000000000, 3.1360000000},
  {-1.0000000000, 1.7320508076, 0.0000000000},
  {0.0000000000, 0.0000000000, 3.1360000000},
  {-1.5000000000, 0.86602540378, 3.1360000000},
  {0.0000000000, 1.7320508076, 3.1360000000},
  {-0.500000000, 0.86602540378, 3.1360000000},
  {0.500000000, -0.86602540378, 3.1360000000},
  {1.5000000000, -0.86602540378, 3.1360000000},
  {2.0000000000, 0.0000000000, 0.0000000000},
  {1.5000000000, 0.86602540378, 3.1360000000},
  {1.0000000000, 0.0000000000, 3.1360000000},
  {0.0000000000, -1.7320508076, 3.1360000000},
  {-1.5000000000, -0.86602540378, 3.1360000000},
  {-1.0000000000, -1.7320508076, 0.0000000000},
  {-0.500000000, -0.86602540378, 3.1360000000}
};
*/ 
/* If needed, we can now initialize a vector of structs using C++-11 syntax
   in a more "listlike" way.  
*/ 
class BurgerTypeInfo {
public:
  BurgerTypeInfo(int num, vector<float> v, int e, string nom): 
    burgnum(num), burg(v), energyLevel(e), name(nom) {}
  int burgnum ; 
  vector<float> burg;
  int energyLevel; 
  string name; 
};

// Test the burgers for equality
bool operator == (const BurgerTypeInfo&b1, const vector<float> &bval) {
  for (int i=0; i<3; i++) {
    if (fabs(bval[i]-b1.burg[i])>BURGERS_EPSILON) return false; 
  }
  return true; 
}

// This will be used for sorting the burgtypes by burgnum for display output
bool compareByBurgnum  (const BurgerTypeInfo&b1, const BurgerTypeInfo &b2) {
  return b1.burgnum < b2.burgnum; 
}

// This will be used for sorting the burgtypes by burgers vector for faster compares
bool compareByVector (const BurgerTypeInfo&b1, const BurgerTypeInfo &b2) {
  for (int i=0; i<3; i++) {
    if (b2.burg[i]-b1.burg[i]>BURGERS_EPSILON) return true; // b1 < b2
    if (b1.burg[i]-b2.burg[i]>BURGERS_EPSILON) return false;// b2 < b1
    // b1.burg[i] and b2.burg[i] are equal within BURGERS_EPSILON; check next
  }
  return false; // the two are equal within BURGERS_EPSILON
}


BurgerTypeInfo test {1000, {-0.500000000, 0.86602540378, 0.0000000000}, 0, "HCP0"};

vector<BurgerTypeInfo> HCPBurgInfos {
  {1000, {-0.500000000, 0.86602540378, 0.0000000000}, 0, "HCP_Burg00"},
  {1001, {1.0000000000, 0.0000000000, 0.0000000000}, 0, "HCP_Burg01"},
  {1002, {-0.500000000, 0.86602540378, 0.0000000000}, 0, "HCP_Burg02"}, 
  {1003, {1.0000000000, 0.0000000000, 0.0000000000}, 0, "HCP_Burg03"},
  {1004, {-0.500000000, -0.86602540378, 0.0000000000}, 0, "HCP_Burg04"},
  {1005, {-0.500000000, 0.86602540378, 1.5680000000}, 0, "HCP_Burg05"},
  {1006, {1.0000000000, 0.0000000000, 1.5680000000}, 0, "HCP_Burg06"},
  {1007, {-0.500000000, -0.86602540378, 1.5680000000}, 0, "HCP_Burg07"},
  {1008, {-0.500000000, 0.86602540378, -1.5680000000}, 0, "HCP_Burg08"},
  {1009, {1.0000000000, 0.0000000000, -1.5680000000}, 0, "HCP_Burg09"},
  {1010, {-0.500000000, -0.86602540378, -1.5680000000}, 0, "HCP_Burg10"},
  {1011, {0.0000000000, 0.0000000000, 1.5680000000}, 0, "HCP_Burg11"},
  {1012, {-1.5000000000, 0.86602540378, 0.0000000000}, 0, "HCP_Burg12"},
  {1013, {0.0000000000, 1.7320508076, 0.0000000000}, 0, "HCP_Burg13"},
  {1014, {-1.0000000000, 1.7320508076, 1.5680000000}, 0, "HCP_Burg14"},
  {1015, {-1.5000000000, 0.86602540378, -1.5680000000}, 0, "HCP_Burg15"},
  {1016, {0.0000000000, 1.7320508076, -1.5680000000}, 0, "HCP_Burg16"},
  {1017, {-1.0000000000, 1.7320508076, -1.5680000000}, 0, "HCP_Burg17"},
  {1018, {-1.5000000000, 0.86602540378, 1.5680000000}, 0, "HCP_Burg18"},
  {1019, {0.0000000000, 1.7320508076, 1.5680000000}, 0, "HCP_Burg19"},
  {1020, {1.5000000000, 0.86602540378, 0.0000000000}, 0, "HCP_Burg20"},
  {1021, {2.0000000000, 0.0000000000, 1.5680000000}, 0, "HCP_Burg21"},
  {1022, {1.5000000000, 0.86602540378, -1.5680000000}, 0, "HCP_Burg22"},
  {1023, {2.0000000000, 0.0000000000, -1.5680000000}, 0, "HCP_Burg23"},
  {1024, {1.5000000000, 0.86602540378, 1.5680000000}, 0, "HCP_Burg24"},
  {1025, {-1.0000000000, -1.7320508076, 1.5680000000}, 0, "HCP_Burg25"},
  {1025, {-1.0000000000, -1.7320508076, -1.5680000000}, 0, "HCP_Burg25"},
  {1027, {0.500000000, 0.86602540378, 3.1360000000}, 0, "HCP_Burg27"},
  {1028, {-1.0000000000, 0.0000000000, 3.1360000000}, 0, "HCP_Burg28"},
  {1029, {-1.0000000000, 1.7320508076, 0.0000000000}, 0, "HCP_Burg29"},
  {1030, {0.0000000000, 0.0000000000, 3.1360000000}, 0, "HCP_Burg30"},
  {1031, {-1.5000000000, 0.86602540378, 3.1360000000}, 0, "HCP_Burg31"},
  {1032, {0.0000000000, 1.7320508076, 3.1360000000}, 0, "HCP_Burg32"},
  {1033, {-0.500000000, 0.86602540378, 3.1360000000}, 0, "HCP_Burg33"},
  {1034, {0.500000000, -0.86602540378, 3.1360000000}, 0, "HCP_Burg34"},
  {1035, {1.5000000000, -0.86602540378, 3.1360000000}, 0, "HCP_Burg35"},
  {1036, {2.0000000000, 0.0000000000, 0.0000000000}, 0, "HCP_Burg36"},
  {1037, {1.5000000000, 0.86602540378, 3.1360000000}, 0, "HCP_Burg37"},
  {1038, {1.0000000000, 0.0000000000, 3.1360000000}, 0, "HCP_Burg38"},
  {1039, {0.0000000000, -1.7320508076, 3.1360000000}, 0, "HCP_Burg39"},
  {1040, {-1.5000000000, -0.86602540378, 3.1360000000}, 0, "HCP_Burg40"},
  {1041, {-1.0000000000, -1.7320508076, 0.0000000000}, 0, "HCP_Burg41"},
  {1042, {-0.500000000, -0.86602540378, 3.1360000000}, 0, "HCP_Burg42"}
}; 

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
    // Sort the HCP Burgers vector for quick finds later:
    std::sort(HCPBurgInfos.begin(), HCPBurgInfos.end(), compareByVector); 
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
  vector<BurgerTypeInfo>::iterator pos = 
    find(HCPBurgInfos.begin(), HCPBurgInfos.end(), burg); 
  if (pos == HCPBurgInfos.end()) {
    for (int i=0; i<3; i++) burg[i] *= -1.0; 
    pos = find(HCPBurgInfos.begin(), HCPBurgInfos.end(), burg); 
  }
  if (pos == HCPBurgInfos.end()) {
    dbprintf(1, "\n\n********************************\n");
    dbprintf(1, "WARNING: Weird burgers vector <%g,%g,%g> encountered\n", burg[0], burg[1], burg[2]);
    dbprintf(1, "\n********************************\n\n");
    return -42; 
  }
  
  return pos->burgnum;  
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
