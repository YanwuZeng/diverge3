#include <string>
#include <algorithm>

#include "align.h"

//----------------------------------------------------------------------

using namespace std;

#define M_LEFT  0x01
#define M_DIAG  0x02
#define M_UP    0x04
#define I_LEFT  0x08
#define I_DIAG  0x10
#define I_UP    0x20

typedef struct {
  int v;
  int d;
} point_t;

//----------------------------------------------------------------------

#if 0
static void
print_sm(const point_t * const *sm, const string seq1, const string seq2) {
  int seq1_len = seq1.length();
  int seq2_len = seq2.length();

  cout << '\t';
  for(int i=0; i<seq1_len; i++) {
    cout << '\t' << seq1[i];
  }
  cout << '\n';
  for(int j=0; j<=seq2_len; j++) {
    if(j > 0) {
      cout << seq2[j-1];
    }
    for(int i=0; i<=seq1_len; i++) {
      cout << '\t' << sm[i][j].v;
    }
    cout << '\n';
  }

  cout << '\n';

#if 0
  cout << '\t';
  for(int i=0; i<seq1_len; i++) {
    cout << '\t' << seq1[i] << '(' << i+1 << ')';
  }
  cout << '\n';
  for(int j=0; j<=seq2_len; j++) {
    if(j > 0) {
      cout << seq2[j-1] << '(' << j << ')';
    }
    for(int i=0; i<=seq1_len; i++) {
      cout << '\t' << sm[i][j].d;
    }
    cout << '\n';
  }

  cout << '\n';
#endif

  cout << '\t';
  for(int i=0; i<seq1_len; i++) {
    cout << '\t' << seq1[i];
  }
  cout << '\n';
  for(int j=0; j<=seq2_len; j++) {
    if(j > 0) {
      cout << seq2[j-1];
    }
    for(int i=0; i<=seq1_len; i++) {
      cout << '\t';
      if(sm[i][j].d == 0) cout << " 0 ";
      else {
	if(sm[i][j].d & M_LEFT) cout << "M_L";
	if(sm[i][j].d & M_DIAG) cout << "M_D";
	if(sm[i][j].d & M_UP)   cout << "M_U";
	if(sm[i][j].d & I_LEFT) cout << "I_L";
	if(sm[i][j].d & I_DIAG) cout << "I_D";
	if(sm[i][j].d & I_UP)   cout << "I_U";
      }
    }
    cout << '\n';
  }

  cout << '\n';
  for(int i=0; i<seq1_len; i++) {
    cout << '\t' << seq1[i];
  }
  cout << '\n';
  for(int j=0; j<seq2_len; j++) {
    cout << '\t' << seq2[j];
  }
  cout << '\n';
}
#endif

//----------------------------------------------------------------------

static point_t **
create_sm(int seq1_len, int seq2_len) {
  point_t **sm = new point_t*[seq1_len+1];
  for(int i=0; i<seq1_len+1; i++) {
    sm[i] = new point_t[seq2_len+1];
    memset(sm[i], 0, sizeof(point_t)*(seq2_len+1));
  }

  return sm;
}

//----------------------------------------------------------------------

static void
free_sm(point_t **sm, int seq1_len) {
  for(int i=0; i<seq1_len+1; i++) {
    delete[] sm[i];
  }
  delete[] sm;
}
     
//----------------------------------------------------------------------

static inline int
penality(char a, char b) {
  return (a == b) ? 10 : -10;
}

//----------------------------------------------------------------------

static void
calc_sim(string seq1, string seq2, point_t **sm_m, point_t **sm_i) {
  int seq1_len = seq1.length();
  int seq2_len = seq2.length();
  int i, j;
  int ga = 10;  // Gap opening penality
  int gb = 1;   // Gap extension penality
  
  sm_m[0][0].v = sm_i[0][0].v = 0;
  sm_m[0][0].d = sm_i[0][0].d = 0;

  for(i=1; i<=seq1_len; i++) {
#ifdef BEGIN_END_GAP_PENALIZE
    sm_m[i][0].v = -(ga + (i-1) * gb);
    sm_m[i][0].d = M_LEFT;
    
    sm_i[i][0].v = -(ga + (i-1) * gb);
    sm_i[i][0].d = I_LEFT;
#else
    sm_m[i][0].v = 0;
    sm_m[i][0].d = M_LEFT;
    
    sm_i[i][0].v = 0;
    sm_i[i][0].d = I_LEFT;
#endif
  }

  for(j=1; j<=seq2_len; j++) {
#ifdef BEGIN_END_GAP_PENALIZE
    sm_m[0][j].v = -(ga + (j-1) * gb);
    sm_m[0][j].d = M_UP;
    
    sm_i[0][j].v = -(ga + (j-1) * gb);
    sm_i[0][j].d = I_UP;
#else
    sm_m[0][j].v = 0;
    sm_m[0][j].d = M_UP;
    
    sm_i[0][j].v = 0;
    sm_i[0][j].d = I_UP;
#endif
  }
  
  for(i=1; i<=seq1_len; i++) {
    for(j=1; j<=seq2_len; j++) {
      {
	int a = sm_m[i-1][j-1].v + penality(seq1[i-1], seq2[j-1]); // Diag from M
	int b = sm_i[i-1][j-1].v + penality(seq1[i-1], seq2[j-1]); // Diag from I
	
	sm_m[i][j].v = 0;
	sm_m[i][j].d = 0;
	if(a >= b) {
	  sm_m[i][j].v = a;
	  sm_m[i][j].d |= M_DIAG;
	}
	if(b >= a) {
	  sm_m[i][j].v = b;
	  sm_m[i][j].d |= I_DIAG;
	}
      }

      {
	int a = sm_m[i][j-1].v - ga;  // Up from M
	int b = sm_i[i][j-1].v - gb;  // Up from I
	int c = sm_m[i-1][j].v - ga;  // Left from M
	int d = sm_i[i-1][j].v - gb;  // Left from I
	
	sm_i[i][j].v = 0;
	sm_i[i][j].d = 0;
	if(a >= b && a >= c && a >= d) {
	  sm_i[i][j].v = a;
	  sm_i[i][j].d |= M_UP;
	}
	if(b >= a && b >= c && b >= d) {
	  sm_i[i][j].v = b;
	  sm_i[i][j].d |= I_UP;
	}
	if(c >= a && c >= b && c >= d) {
	  sm_i[i][j].v = c;
	  sm_i[i][j].d |= M_LEFT;
	}
	if(d >= a && d >= b && d >= c) {
	  sm_i[i][j].v = d;
	  sm_i[i][j].d |= I_LEFT;
	}
      }
    }
  }
}

//----------------------------------------------------------------------

static void
build_align(const point_t * const *sm, const point_t * const *sm_m, const point_t * const *sm_i,
	    const string seq1, const string seq2,
	    int i, int j, string &as1, string &as2) {
  if(i == 0 && j == 0) {
    reverse(as1.begin(), as1.end());
    reverse(as2.begin(), as2.end());
  } else {
    if(sm[i][j].d & M_DIAG) {
      as1 += seq1[i-1];
      as2 += seq2[j-1];
      build_align(sm_m, sm_m, sm_i, seq1, seq2, i-1, j-1, as1, as2);
    } else if(sm[i][j].d & M_LEFT) {
      as1 += seq1[i-1];
      as2 += '-';
      build_align(sm_m, sm_m, sm_i, seq1, seq2, i-1, j, as1, as2);
    } else if(sm[i][j].d & M_UP) {
      as1 += '-';
      as2 += seq2[j-1];
      build_align(sm_m, sm_m, sm_i, seq1, seq2, i, j-1, as1, as2);
    } else if(sm[i][j].d & I_DIAG) {
      as1 += seq1[i-1];
      as2 += seq2[j-1];
      build_align(sm_i, sm_m, sm_i, seq1, seq2, i-1, j-1, as1, as2);
    } else if(sm[i][j].d & I_LEFT) {
      as1 += seq1[i-1];
      as2 += '-';
      build_align(sm_i, sm_m, sm_i, seq1, seq2, i-1, j, as1, as2);
    } else if(sm[i][j].d & I_UP) {
      as1 += '-';
      as2 += seq2[j-1];
      build_align(sm_i, sm_m, sm_i, seq1, seq2, i, j-1, as1, as2);
    }
  }
}
  
static int
build_align(const point_t * const *sm_m, const point_t * const *sm_i,
	    const string seq1, const string seq2,
	    string &seq1_align, string &seq2_align) {
  seq1_align = "";
  seq2_align = "";
#ifdef BEGIN_END_GAP_PENALIZE
  if(sm_m[seq1.length()][seq2.length()].v >= sm_i[seq1.length()][seq2.length()].v) {
    build_align(sm_m, sm_m, sm_i, seq1, seq2, seq1.length(), seq2.length(), seq1_align, seq2_align);
    return sm_m[seq1.length()][seq2.length()].v;
  } else {
    build_align(sm_i, sm_m, sm_i, seq1, seq2, seq1.length(), seq2.length(), seq1_align, seq2_align);
    return sm_i[seq1.length()][seq2.length()].v;
  }
#else
  int i, j;
  int max_mi = seq1.length(), max_mj = seq2.length();
  int max_mv = sm_m[max_mi][max_mj].v;
  int max_ii = seq1.length(), max_ij = seq2.length();
  int max_iv = sm_i[max_ii][max_ij].v;
  for(i=0; i<=seq1.length(); i++) {
    if(sm_m[i][seq2.length()].v > max_mv) {
      max_mi = i; max_mj = seq2.length();
      max_mv = sm_m[max_mi][max_mj].v;
    }
    if(sm_i[i][seq2.length()].v > max_iv) {
      max_ii = i; max_ij = seq2.length();
      max_iv = sm_i[max_ii][max_ij].v;
    }
  }
  for(j=0; j<=seq2.length(); j++) {
    if(sm_m[seq1.length()][j].v > max_mv) {
      max_mi = seq1.length(); max_mj = j;
      max_mv = sm_m[max_mi][max_mj].v;
    }
    if(sm_i[seq1.length()][j].v > max_iv) {
      max_ii = seq1.length(); max_ij = j;
      max_iv = sm_m[max_ii][max_ij].v;
    }
  }
  //cout << "max_mv:" << max_mv << '\t' << "max_iv:" << max_iv << '\n';
  if(max_mv >= max_iv) {
    if(max_mi == seq1.length()) {
      for(j=seq2.length(); j>max_mj; j--) {
	seq1_align += '-';
	seq2_align += seq2[j-1];
      }
    } else {
      for(i=seq1.length(); i>max_mi; i--) {
	seq1_align += seq1[i-1];
	seq2_align += '-';
      }
    }
    build_align(sm_m, sm_m, sm_i, seq1, seq2, max_mi, max_mj, seq1_align, seq2_align);
    return max_mv;
  } else {
    if(max_ii == seq1.length()) {
      for(j=seq2.length(); j>max_ij; j--) {
	seq1_align += '-';
	seq2_align += seq2[j-1];
      }
    } else {
      for(i=seq1.length(); i>max_ii; i--) {
	seq1_align += seq1[i-1];
	seq2_align += '-';
      }
    }
    build_align(sm_i, sm_m, sm_i, seq1, seq2, max_ii, max_ij, seq1_align, seq2_align);
    return max_iv;
  }
#endif
}

//----------------------------------------------------------------------

int
align(string seq1, string seq2, string &seq1_align, string &seq2_align) {
  int seq1_len = seq1.length();
  int seq2_len = seq2.length();

  //cout << "seq1 len: " << seq1_len << '\n';
  //cout << "seq2 len: " << seq2_len << '\n';

  point_t **sm_m = create_sm(seq1_len, seq2_len);
  point_t **sm_i = create_sm(seq1_len, seq2_len);
  
  calc_sim(seq1, seq2, sm_m, sm_i);

  //print_sm(sm_m, seq1, seq2);
  //print_sm(sm_i, seq1, seq2);

  int score = build_align(sm_m, sm_i, seq1, seq2, seq1_align, seq2_align);
  
  free_sm(sm_m, seq1_len);
  free_sm(sm_i, seq1_len);

  return score;
}

//----------------------------------------------------------------------

#if 0
int
main() {
#if 0
  string seq1 = "TVLG";
  string seq2 = "TLG";
#endif
#if 0
  string seq1 = "FHKK";
  string seq2 = "FHK";
#endif
#if 1
  string seq1 = "KKHF";
  string seq2 = "KHF";
#endif
  string as1, as2;
  int score = align(seq1, seq2, as1, as2);
  cout << seq1 << '\n'
       << seq2 << '\n'
       << score << '\n'
       << as1 << '\n'
       << as2 << '\n';
  return 0;
}
#endif

//----------------------------------------------------------------------
