#include <string>
#include <vector>

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "pdb.h"

using namespace std;

//This file contains the defintions of PDB related methods. The description 
//of PDB format can be found in the following link http://pdb.rutgers.edu/format-faq-v1.html.

static char *
str_clean_rn(char *str) {
  char *p;

  if((p=strrchr(str, '\r'))) *p='\0';
  if((p=strrchr(str, '\n'))) *p='\0';

  return str;
}

static string
basename(string str) {
#ifndef WIN32
  size_t pos = str.rfind("/");
#else
  size_t pos = str.rfind("\\");
#endif
  if(pos == string::npos) {
    return str;
  } else {
    return str.substr(pos+1);
  }
}

char
Residue::aa_3_to_1(string name) {
  char t='X';

  if     (name == "ALA") t='A'; 
  else if(name == "ARG") t='R'; 
  else if(name == "ASN") t='N'; 
  else if(name == "ASP") t='D'; 
  else if(name == "CYS") t='C'; 
  else if(name == "GLN") t='Q'; 
  else if(name == "GLU") t='E'; 
  else if(name == "GLY") t='G'; 
  else if(name == "HIS") t='H'; 
  else if(name == "ILE") t='I'; 
  else if(name == "LEU") t='L'; 
  else if(name == "LYS") t='K'; 
  else if(name == "MET") t='M'; 
  else if(name == "PHE") t='F'; 
  else if(name == "PRO") t='P'; 
  else if(name == "SER") t='S'; 
  else if(name == "THR") t='T'; 
  else if(name == "TRP") t='W'; 
  else if(name == "TYR") t='Y'; 
  else if(name == "VAL") t='V'; 
  else if(name == "UNK") t='X'; 
  else t='?';

  return t;
}

bool
Residue::parse(const char *line) {  
  type = '-';
    
  atom_type = "";
  int i;
  for(i=0; i<3; i++) {
    char c = line[i + 13];
    // Assume field is left justified and without spaces.
    if(c == ' ') break;
    atom_type += c;
  }

  alt_pos = line[16];

  res_type = "";
  for(i=0; i<3; i++) {
    char c = line[i + 17];
    // Field is not left justified in case of nucleotides.
    // Ex. "CA " and "  T"
    if(c != ' ') {
      res_type += c;
    }
  }

  res_num = "";
  // Include "code for insertion - iCode" in res_num (the last character)
  for(i=0; i<5; i++) {
    char c = line[i + 22];
    // Assume field is right justified (or at least has spaces on the left)
    // and does contain inner spaces.
    if(c != ' ') {
      res_num += c;
    }
  }

  char t[16];

  t[8]='\0';
  memcpy(t, line+30, 8);
  x=atof(t);
  
  memcpy(t, line+38, 8);
  y=atof(t);
  
  memcpy(t, line+46, 8);
  z=atof(t);
  
  type = aa_3_to_1(res_type);

  return true;
}

bool
Residue::add_to_cg(const Residue &res) {
  if(res_type != res.res_type) return false;
  if(type != res.type)         return false;
  if(res_num != res.res_num)   return false;

  if(res.atom_type == "N" || res.atom_type == "CA" ||
     res.atom_type == "C" || res.atom_type == "O") {
    return false;
  }
  
  cg_x += res.x;
  cg_y += res.y;
  cg_z += res.z;
  cg_n++;
  
  return true;
}


#if 0
void
Chain::print() const {
  cout << "chain " << name << '\n';
  vector<Residue>::const_iterator i;
  for(i = residues.begin(); i != residues.end(); i++) {
    cout << '\t'
	 << i->res_num << '\t'
	 << i->atom_type << '\t'
	 << i->res_type << '\t'
	 << i->type << "\t("
	 << i->x << ", " << i->y << ", " << i->z << ")\t("
	 << i->cg_x/i->cg_n << ", "
	 << i->cg_y/i->cg_n << ", "
	 << i->cg_z/i->cg_n << ")\n";
  }
}
#endif

void
Chain::clip(size_t s, size_t n) {
  assert(n + s <= residues.size());
  assert(s <= residues.size());
  residues.erase(residues.begin() + n + s, residues.end());
  residues.erase(residues.begin(), residues.begin() + s);
}

string
Chain::sequence() const {
  string seq;

  vector<Residue>::const_iterator i;
  for(i = residues.begin(); i != residues.end(); i++) {
    seq += i->type;
  }
  
  return seq;
}

vector<string>
Chain::res_nums() const {
  vector<string> res_nums;
  res_nums.reserve(residues.size());

  vector<Residue>::const_iterator i;
  for(i = residues.begin(); i != residues.end(); i++) {
    res_nums.push_back(i->res_num);
  }
  
  return res_nums;
}

const Residue *
Chain::find_res(string res_num) const {
  vector<Residue>::const_iterator i;
  for(i = residues.begin(); i != residues.end(); i++) {
    if(res_num == i->res_num) {
      return &(*i);
    }
  }
  return NULL;
}

int
Chain::find_res_index(string res_num) const {
  vector<Residue>::const_iterator i;
  int j;
  for(i = residues.begin(), j=0; i != residues.end(); i++, j++) {
    if(res_num == i->res_num) {
      return j;
    }
  }
  return -1;
}

void
PDB::clear() {
  name = "";
  title = "";
  source = "";
  filename = "";
  chains.clear();
}

bool
PDB::load(string filename) {
  FILE *file = fopen(filename.c_str(), "r");
  if(!file) {
    return false;
  }

  name = basename(filename);
  title = "";
  source = "";
  PDB::filename = filename;
  chains.clear();
  
  size_t line_num = 0;
  
  char line[1024];
  while(fgets(line, 1024, file)) {
    line_num++;

    if(strncmp(line, "END", 3)==0 ||
       strncmp(line, "ENDMDL", 6)==0) break;

    // No tag that we look at has characters past 70 and occassionaly there
    // is information that can mess up the parsing so hack it off.
    line[70] = '\0';

    for(int i=strlen(line) - 1; i>=0; i--) {
      if(isspace(line[i])) {
	line[i] = '\0';
      } else {
	break;
      }
    }
    
    if(strncmp(line, "HEADER ", 7) == 0) {
      string tmp = string(line).substr(62, 4);
      if(!tmp.empty()) name = tmp;
    } else if(strncmp(line, "TITLE ", 6) == 0) {
      title += line + 10;
    } else if(strncmp(line, "SOURCE ", 7) == 0) {
      if(source.empty()) {
	char *p = strstr(line, "ORGANISM_SCIENTIFIC: ");
	if(p) {
	  source = p + strlen("ORGANISM_SCIENTIFIC: ");
	}
      }
      char *p = strstr(line, "ORGANISM_COMMON: ");
      if(p) {
	source = p + strlen("ORGANISM_COMMON: ");
      }
    } else if(strncmp(line, "ATOM  ", 5) == 0) {
      // Without the following mutiple corresponding atoms are thrown in.
      // Eg. 1bz6
      if(line[16] != ' ') {
#if 0
	cerr << "PDB::load(): Warning: " << filename
	     << " contains alternate sites for residue on line "
	     << line_num << " choosing A positions\n";
#endif
	if(line[16] != 'A') continue;
      }
      char chain_id = line[21];
      if(chains.empty() || chains.back().name != chain_id) {
	Chain tmp;
	tmp.name = chain_id;
	chains.push_back(tmp);
      }
      Residue res;
      if(res.parse(line)) {
	if(res.type == '?' || res.type == 'X' || res.type == '-') {
#if 0
	  cerr << "PDB::load(): Warning: Ignoring unknown residue type "
	       << res.res_type << " (maps to " << res.type
	       << ") in " << filename << " on line " << line_num << '\n';
#endif
	  continue;
	}

	Chain &chain = chains.back();

	Residue r; 
	if (! chain.residues.empty())
	{
		r = chain.residues.back(); 
	}

	if(chain.residues.empty() || r.res_num != res.res_num) {
	  if(res.atom_type != "CA") {
	    res.add_to_cg(res);
	    res.x = 0.0;
	    res.y = 0.0;
	    res.z = 0.0;
	  }
	  chain.residues.push_back(res);
	} else {
	  if(res.atom_type == "CA") {
	    if(r.atom_type == "CA") {
#if 0
	      cerr << "PDB::load(): Warning: Residue in "
		   << filename << " on line " << line_num
		   << " CA data was already set\n";
#endif
	      fclose(file);
	      return false;
	    }
	    r.atom_type = res.atom_type;
	    r.x = res.x;
	    r.y = res.y;
	    r.z = res.z;
		
		chain.residues.push_back(r); 

	  } else {
	    r.add_to_cg(res);

	  }
	}
      }
    }
  }

  fclose(file);

  vector<Chain>::iterator i;
  for(i=chains.begin(); i!=chains.end(); i++) {
    for(int j=i->residues.size()-1; j>=0; j--) {
      if(i->residues[j].atom_type != "CA") {
#if 0
	cerr << "PDB::load(): Warning: removing residue without CA in "
	     << filename << '\n';
#endif
	i->residues.erase(i->residues.begin()+j);
      }
    }
  }

  return true;
}

#if 0
void
PDB::print() const {
  cout << name << '\n'
       << title << '\n'
       << source << '\n';
  vector<Chain>::const_iterator i;
  for(i = chains.begin(); i != chains.end(); i++) {
    i->print();
  }
}
#endif

const Chain *
PDB::find_chain(char chain_ID) const {
  vector<Chain>::const_iterator i;
  for(i=chains.begin(); i!=chains.end(); i++) {
    if(i->name == chain_ID) {
      return &(*i);
    }
  }
  return NULL;
}
