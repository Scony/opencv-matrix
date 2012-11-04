#ifndef MATRIX_H
#define MATRIX_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <list>
#include <fstream>

#include "Row.h"
#include "Hu.h"
#include "Raw.h"

class Matrix
{
  Raw * raw;
  IplImage * matrix;
  IplImage * setUp(IplImage * in);
  IplImage * getROI(IplImage * in);
  IplImage * filterContours(IplImage * in);
 public:
  Matrix(IplImage * in, Raw * raw);
  ~Matrix();
  IplImage * show();
  void learn();
  void test();
  void resolve();
};

#endif
