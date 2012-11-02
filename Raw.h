#ifndef RAW_H
#define RAW_H

#include <fstream>
#include <list>

#include "Hu.h"

class Raw
{
  std::list<Hu> digits[10];
public:
  Raw();
  ~Raw();
  int bestMatch(Hu digit);
};

#endif
