#ifndef DIGIT_H
#define DIGIT_H

#include <iostream>

#include "Hu.h"
#include "Raw.h"

class Digit
{
  int x;
  int y;
  int width;
  int height;
  Hu hus;
  Raw * raw;
public:
  Digit(int x, int y, int width, int height, Hu hus, Raw * raw);
  ~Digit();
  int getLeft();
  int getTop();
  int getBot();
  int getMid();
  int resolve();
  bool operator < (Digit & other);
};

#endif
