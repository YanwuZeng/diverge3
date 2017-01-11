#ifndef _PDB_H_
#define _PDB_H_

#include <string>
#include <vector>

class Residue {
public:
  Residue() : alt_pos(' '), type('-'), x(0.0), y(0.0), z(0.0),
	      cg_n(0), cg_x(0.0), cg_y(0.0), cg_z(0.0) {}
  ~Residue() {}

  bool parse(const char *line);
  bool add_to_cg(const Residue &res);
  static char aa_3_to_1(std::string name);
  
  std::string atom_type;
  char alt_pos;
  std::string res_type;
  char type;
  std::string res_num;
  double x, y, z;

  int cg_n;
  double cg_x, cg_y, cg_z;
};

class Chain {
public:
  Chain() : name(' ') {}
  ~Chain() {}

  //void print() const;
  void clip(size_t s, size_t n);
  std::string sequence() const;
  std::vector<std::string> res_nums() const;
  const Residue* find_res(std::string res_num) const;
  int find_res_index(std::string res_num) const;
  
  char name;
  std::vector<Residue> residues;
};

class PDB {
public:
  PDB() {}
  ~PDB() {}

  void clear();
  bool load(std::string filename);
  //void print() const;
  const Chain* find_chain(char chain_ID) const;
  
  std::string name;
  std::string title;
  std::string source;
  std::string filename;
  std::vector<Chain> chains;
};

#endif
