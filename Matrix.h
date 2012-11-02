#ifndef MATRIX_H
#define MATRIX_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <list>
#include <fstream>

#include "Row.h"
#include "Hu.h"

class Matrix
{
  IplImage * matrix;
  IplImage * setUp(IplImage * in);
  IplImage * getROI(IplImage * in);
  IplImage * filterContours(IplImage * in);
  int ** resolveMatrix(IplImage * in);
  void contourDance(IplImage * in);
 public:
  Matrix(IplImage * in);
  ~Matrix();
  IplImage * show();
  void learn();
};

#endif
