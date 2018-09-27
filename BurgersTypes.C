/*   Written by Richard David Cook
     at Lawrence Livermore National Laboratory
     Contact:  wealthychef@gmail.com
     See license.txt for information about usage.
     Spoiler alert:  it's GNU opensource.
*/


#include "BurgersTypes.h"
#include <boost/numeric/ublas/vector.hpp>

// GRRR.  Visit hooks are lame.  This is bad code but if I don't structure it like this, the SVN hooks complain.
#ifdef USE_ABORT
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




/* ============================================================ */
vector<BurgerTypeInfo> BurgInfos = {
  {BCC_BURGERS_UNKNOWN, {0, 0, 0},                          0, "BCC_BURGERS_UNKNOWN"},
  {00,                {0.0000000,   0.0000000,  0.0000000}, 0, "BCC_BURGERS_NONE"},
  {BCC_BURGERS_PPP,   {0.5773503,   0.5773503, 0.5773503},  1, "BCC_BURGERS_PPP"},
  {BCC_BURGERS_PPM,   {0.5773503,   0.5773503, -0.5773503}, 1, "BCC_BURGERS_PPM"},
  {BCC_BURGERS_PMP,   {0.5773503,  -0.5773503,  0.5773503}, 1, "BCC_BURGERS_PMP"},
  {BCC_BURGERS_PMM,   {0.5773503,  -0.5773503, -0.5773503}, 1, "BCC_BURGERS_PMM"},
  {20,                {1.1547006,   0.0000000,  0.0000000}, 2, "BCC_BURGERS_200"},
  {21,                {0.0000000,   1.1547006,  0.0000000}, 2, "BCC_BURGERS_020"},
  {22,                {0.0000000,   0.0000000,  1.1547006}, 2, "BCC_BURGERS_002"},
  {30,                {1.1547006,   1.1547006,  0.0000000}, 3, "BCC_BURGERS_220"},
  {31,                {1.1547006,   0.0000000,  1.1547006}, 3, "BCC_BURGERS_202"},
  {32,                {0.0000000,   1.1547006,  1.1547006}, 3, "BCC_BURGERS_022"},
  {40,                {1.7320509,   0.5773503,  0.5773503}, 4, "BCC_BURGERS_311"},
  {41,                {0.5773503,   1.7320509,  0.5773503}, 4, "BCC_BURGERS_131"},
  {42,                {0.5773503,   0.5773503,  1.7320509}, 4, "BCC_BURGERS_113"},
  {50,                {1.1547006,   1.1547006,  1.1547006}, 5, "BCC_BURGERS_222"},
  {60,                {0.0000000,   0.0000000,  2.3094012}, 6, "BCC_BURGERS_004"},
  {70,                {1.7320509,   1.7320509,  0.5773503}, 7, "BCC_BURGERS_331"},
  {71,                {1.7320509,   0.5773503,  1.7320509}, 7, "BCC_BURGERS_313"},
  {72,                {0.5773503,   1.7320509,  1.7320509}, 7, "BCC_BURGERS_133"},
  {80,                {2.3094012,   1.1547006,  0.0000000}, 8, "BCC_BURGERS_420"},
  {81,                {1.1547006,   2.3094012,  0.0000000}, 8, "BCC_BURGERS_240"},
  {82,                {0.0000000,   1.1547006,  2.3094012}, 8, "BCC_BURGERS_024"},
  {83,                {0.0000000,   2.3094012,  1.1547006}, 8, "BCC_BURGERS_042"},
  {84,                {1.1547006,   0.0000000,  2.3094012}, 8, "BCC_BURGERS_204"},
  {85,                {2.3094012,   0.0000000,  1.1547006}, 8, "BCC_BURGERS_402"},
  {HCP_BURGERS_UNKNOWN, {0, 0, 0},                0, "HCP_BURGERS_UNKNOWN"},
  {1000, {-0.50000000000,  0.86602540378,  0.00000000000}, 0, "HCP_Burg00"},
  {1001, { 1.00000000000,  0.00000000000,  0.00000000000}, 0, "HCP_Burg01"},
  {1002, {-0.50000000000, -0.86602540378,  0.00000000000}, 0, "HCP_Burg02"},
  {1003, {-0.50000000000,  0.86602540378,  1.56800000000}, 0, "HCP_Burg03"},
  {1004, { 1.00000000000,  0.00000000000,  1.56800000000}, 0, "HCP_Burg04"},
  {1005, {-0.50000000000, -0.86602540378,  1.56800000000}, 0, "HCP_Burg05"},
  {1006, {-0.50000000000,  0.86602540378, -1.56800000000}, 0, "HCP_Burg06"},
  {1007, { 1.00000000000,  0.00000000000, -1.56800000000}, 0, "HCP_Burg07"},
  {1008, {-0.50000000000, -0.86602540378, -1.56800000000}, 0, "HCP_Burg08"},
  {1009, { 0.00000000000,  0.00000000000,  1.56800000000}, 0, "HCP_Burg09"},
  {1010, {-1.50000000000,  0.86602540378,  0.00000000000}, 0, "HCP_Burg10"},
  {1011, { 0.00000000000,  1.73205080760,  0.00000000000}, 0, "HCP_Burg11"},
  {1012, {-1.00000000000,  1.73205080760,  1.56800000000}, 0, "HCP_Burg12"},
  {1013, {-1.50000000000,  0.86602540378, -1.56800000000}, 0, "HCP_Burg13"},
  {1014, { 0.00000000000,  1.73205080760, -1.56800000000}, 0, "HCP_Burg14"},
  {1015, {-1.00000000000,  1.73205080760, -1.56800000000}, 0, "HCP_Burg15"},
  {1016, {-1.50000000000,  0.86602540378,  1.56800000000}, 0, "HCP_Burg16"},
  {1017, { 0.00000000000,  1.73205080760,  1.56800000000}, 0, "HCP_Burg17"},
  {1018, { 1.50000000000,  0.86602540378,  0.00000000000}, 0, "HCP_Burg18"},
  {1019, { 2.00000000000,  0.00000000000,  1.56800000000}, 0, "HCP_Burg19"},
  {1020, { 1.50000000000,  0.86602540378, -1.56800000000}, 0, "HCP_Burg20"},
  {1021, { 2.00000000000,  0.00000000000, -1.56800000000}, 0, "HCP_Burg21"},
  {1022, { 1.50000000000,  0.86602540378,  1.56800000000}, 0, "HCP_Burg22"},
  {1023, {-1.00000000000, -1.73205080760,  1.56800000000}, 0, "HCP_Burg23"},
  {1024, {-1.00000000000, -1.73205080760, -1.56800000000}, 0, "HCP_Burg24"},
  {1025, { 0.50000000000,  0.86602540378,  3.13600000000}, 0, "HCP_Burg25"},
  {1026, {-1.00000000000,  0.00000000000,  3.13600000000}, 0, "HCP_Burg26"},
  {1027, {-1.00000000000,  1.73205080760,  0.00000000000}, 0, "HCP_Burg27"},
  {1028, { 0.00000000000,  0.00000000000,  3.13600000000}, 0, "HCP_Burg28"},
  {1029, {-1.50000000000,  0.86602540378,  3.13600000000}, 0, "HCP_Burg29"},
  {1030, { 0.00000000000,  1.73205080760,  3.13600000000}, 0, "HCP_Burg30"},
  {1031, {-0.50000000000,  0.86602540378,  3.13600000000}, 0, "HCP_Burg31"},
  {1032, { 0.50000000000, -0.86602540378,  3.13600000000}, 0, "HCP_Burg32"},
  {1033, { 1.50000000000, -0.86602540378,  3.13600000000}, 0, "HCP_Burg33"},
  {1034, { 2.00000000000,  0.00000000000,  0.00000000000}, 0, "HCP_Burg34"},
  {1035, { 1.50000000000,  0.86602540378,  3.13600000000}, 0, "HCP_Burg35"},
  {1036, { 1.00000000000,  0.00000000000,  3.13600000000}, 0, "HCP_Burg36"},
  {1037, { 0.00000000000, -1.73205080760,  3.13600000000}, 0, "HCP_Burg37"},
  {1038, {-1.50000000000, -0.86602540378,  3.13600000000}, 0, "HCP_Burg38"},
  {1039, {-1.00000000000, -1.73205080760,  0.00000000000}, 0, "HCP_Burg39"},
  {1040, {-0.50000000000, -0.86602540378,  3.13600000000}, 0, "HCP_Burg40"}
};


/* ============================================================ */
string DocumentAllBurgersTypes(void) {
  string output  =   "  //  Segment BURGERS TYPES: (P = plus(+) and M = minus(-))\n"
    "// These are valued in order of increasing energy levels, corresponding to the sum of the square of the components of the burgers vector.  \n";

  output += str(boost::format("%=15s%=30s%=8s%=23s\n")%"Burgers Value"%"Vector"%"Energy"%"Name");
  for (vector<BurgerTypeInfo>::iterator bt = BurgInfos.begin();
       bt != BurgInfos.end(); bt++) {
    output += str(boost::format("%=15d%10.6f%10.6f%10.6f%=8d%=23s\n")
                  %(bt->burgnum)%(bt->burgvec[0])%(bt->burgvec[1])%(bt->burgvec[2])
                  %(bt->energy)%(bt->name));
  }
  return output;
}


/* ============================================================ */
/* If needed, we can now initialize a vector of structs using C++-11 syntax
   in a more "listlike" way.
*/
// Test the burgers for equality within BURGERS_EPSILON tolerance
bool operator == (const BurgerTypeInfo&b1, const vector<float> &bval) {
  int posmatches = 0, negmatches = 0;
  for (int i=0; i<3; i++) {
    if (fabs(bval[i]-b1.burgvec[i]) < BURGERS_EPSILON) {
      posmatches++;
    } else if (fabs(-bval[i]-b1.burgvec[i]) < BURGERS_EPSILON) {
      negmatches++;
    }
  }
  if (b1.burgnum >= 1000 || b1.energy == 1) {
    // For HCP or type 111 BCC arms, signs matter,
    // so only accept three of the same type of match, positive or negative
    return posmatches == 3 || negmatches == 3;
  }
  // sign does not matter, so any three value matches are a match
  return (posmatches + negmatches == 3);
}

/* ============================================================ */
// This will be used for sorting the burgtypes by burgnum for display output
bool compareByBurgnum  (const BurgerTypeInfo&b1, const BurgerTypeInfo &b2) {
  return b1.burgnum < b2.burgnum;
}

/* ============================================================ */
// This will be used for sorting the burgtypes by burgers vector for faster compares
// Therefore, we do not worry about equality of negative vectors here.
bool compareByVector (const BurgerTypeInfo&b1, const BurgerTypeInfo &b2) {
  for (int i=0; i<3; i++) {
    if (b2.burgvec[i]-b1.burgvec[i]>BURGERS_EPSILON) return true; // b1 < b2
    if (b1.burgvec[i]-b2.burgvec[i]>BURGERS_EPSILON) return false;// b2 < b1
    // b1.burg[i] and b2.burg[i] are equal within BURGERS_EPSILON; check next
  }
  return false; // the two are equal within BURGERS_EPSILON
}

/* ============================================================ */
map<int, BurgerTypeInfo> BurgTypeToBurgInfoMap;

BurgerTypeInfo BurgTypeToBurgInfo(int burgnum) {
  if (!BurgTypeToBurgInfoMap.size()) {
    for (vector<BurgerTypeInfo>::iterator bt = BurgInfos.begin();
         bt != BurgInfos.end(); bt++) {
      BurgTypeToBurgInfoMap[bt->burgnum] = *bt;
    }
  }
  return BurgTypeToBurgInfoMap[burgnum];
}

/* ============================================================ */
map<vector<float>, BurgerTypeInfo> BurgVecToBurgInfoMap;

BurgerTypeInfo BurgVecToBurgInfo(const vector<float> &burgvec) {
  if (!BurgVecToBurgInfoMap.size()) {
    for (vector<BurgerTypeInfo>::iterator bt = BurgInfos.begin();
         bt != BurgInfos.end(); bt++) {
      BurgVecToBurgInfoMap[bt->burgvec] = *bt;
      vector<float> negvec {-bt->burgvec[0], -bt->burgvec[1], -bt->burgvec[2]};
      BurgVecToBurgInfoMap[negvec] = *bt;
    }
  }
  if (BurgVecToBurgInfoMap[burgvec].burgnum == BCC_BURGERS_UNKNOWN) {
    vector<float> negvec = {-burgvec[0], -burgvec[1], -burgvec[2]};
    // Not in the current list.  See if we can find a close match, and cache it:
    for (vector<BurgerTypeInfo>::iterator bt = BurgInfos.begin();
         bt != BurgInfos.end(); bt++) {
      if (*bt == burgvec || *bt == negvec) {
        BurgVecToBurgInfoMap[burgvec] = *bt;
        break;
      }
    }
  }
  BurgerTypeInfo burginfo = BurgVecToBurgInfoMap[burgvec];
  return burginfo;
}

/* ============================================================ */
int BurgVecToBurgType(const vector<float> &burgvec) {
  BurgerTypeInfo binfo = BurgVecToBurgInfo(burgvec);
  return binfo.burgnum;
}

/* ============================================================ */
string BurgTypeToName(int btype) {
  BurgerTypeInfo binfo = BurgTypeToBurgInfo(btype);
  return binfo.name;
}


/* ============================================================ */
map<int, string> ArmTypeToNameMap;

string ArmTypeToName(int atype) {
  if (!ArmTypeToNameMap.size()) {
    ArmTypeToNameMap.clear();
    ArmTypeToNameMap[ARM_EMPTY] =            "ARM_EMPTY"; // for deletion after decomposition
    ArmTypeToNameMap[ARM_UNKNOWN] =          "ARM_UNKNOWN";
    ArmTypeToNameMap[ARM_UNINTERESTING] =    "ARM_UNINTERESTING";
    ArmTypeToNameMap[ARM_LOOP] =             "ARM_LOOP";
    ArmTypeToNameMap[ARM_BCC_MM_111] =       "ARM_BCC_MM_111";
    ArmTypeToNameMap[ARM_BCC_MN_111] =       "ARM_BCC_MN_111";
    ArmTypeToNameMap[ARM_BCC_NN_111] =       "ARM_BCC_NN_111";
    ArmTypeToNameMap[ARM_BCC_SHORT_NN_111] = "ARM_BCC_SHORT_NN_111";
    ArmTypeToNameMap[ARM_BOUNDARY] =         "ARM_BOUNDARY";  // Has terminal node with one neighbor segment; happens in non-periodic data
    for (vector<BurgerTypeInfo>::iterator bt = BurgInfos.begin();
         bt != BurgInfos.end(); bt++) {
      if (bt->burgnum >= 1000) {
        ArmTypeToNameMap[bt->burgnum] = str(boost::format("ARM_HCP_BURGERS_%04d")%(bt->burgnum));
        ArmTypeToNameMap[bt->burgnum+1000] = str(boost::format("ARM_HCP_UNZIPPED_%04d")%(bt->burgnum+1000));
      }
    }
  }
  string result = ArmTypeToNameMap[atype];
  if (result == "") {
    return "ERROR_UNKNOWN_ARM_TYPE";
  }
  return result;
}

/* ============================================================ */
vector<int> GetAllArmTypes(void) {
  string initializeArms = ArmTypeToName(0);
  vector<int> result;
  for (map<int,string>::iterator pos = ArmTypeToNameMap.begin();
       pos != ArmTypeToNameMap.end(); pos++) {
    result.push_back(pos->first);
  }
  return result;
}

/* ============================================================ */
string DocumentAllArmTypes(void) {
  string output;
  output += "Each Arm has an Arm Type which usually corresponds closely to the burgers value of the arm segments it contains.  However, it also expresses emergent properties of the arm such as whether it is a LOOP or has been otherwise defined to have interesting topology or features.\n";
  output += str(boost::format("%=15s%=23s\n")%"Arm Type"%"Name");
  vector<int> armtypes = GetAllArmTypes();
  for (vector<int>::iterator pos = armtypes.begin();
       pos != armtypes.end(); pos++) {
    output += str(boost::format("%=15d%=23s\n")
                  %(*pos)%ArmTypeToName(*pos));
  }
  return output;
}

/* ============================================================ */
map<int, string> MetaArmTypeToNameMap;
string MetaArmTypeToName(int mtype) {
  if (!MetaArmTypeToNameMap.size()) {
    MetaArmTypeToNameMap[METAARM_UNKNOWN] = "METAARM_UNKNOWN";
    MetaArmTypeToNameMap[METAARM_111] = "METAARM_111";
    MetaArmTypeToNameMap[METAARM_LOOP_111] = "METAARM_LOOP_111";
    MetaArmTypeToNameMap[METAARM_LOOP_HIGH_ENERGY] = "METAARM_LOOP_HIGH_ENERGY";
  }
  string result = MetaArmTypeToNameMap[mtype];
  if (result == "") {
    return "ERROR_UNKNOWN_METAARM_TYPE";
  }
  return result;
};



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
