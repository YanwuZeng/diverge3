#ifndef _COMMON_H_
#define _COMMON_H_

#include <string>
#include <vector>


#define appname "Qt Diverge 3.1 Beta 1" 

#ifdef BOOTSTRAP
#define version "3.1 Beta 1/ Bootstrap" 
#else
#define version "3.1 Beta 1" 
#endif

class summary_t {
public:
  std::string name;
  std::vector<double> values;
};


#endif
