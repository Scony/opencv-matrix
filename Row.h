#ifndef ROW_H
#define ROW_H

#include <list>

#include "Digit.h"

class Row
{
  std::list<Digit> digits;
  int top;
  int bot;
public:
  Row(Digit digit);
  ~Row();
  bool match(Digit digit);
  void insert(Digit digit);
  void sort();
  void print();
  bool operator < (Row & other);
};

#endif
