#ifndef HU_H
#define HU_H

#include <iostream>
/* #include <math.h> */
/* #include <stdlib.h> */

class Hu
{
  double hu[7];
  int digit;
 public:
  Hu(double h1, double h2, double h3, double h4, double h5, double h6, double h7);
  Hu(double * in);
  void setDigit(int digit);
  int getDigit();
  double * getHus();
  void print();
  double match(Hu in);
};

#endif
