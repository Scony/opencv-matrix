#ifndef MATRIX_H
#define MATRIX_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <list>

#include "Row.h"
//#include "

class Matrix
{
  IplImage * setUp(IplImage * in);
  IplImage * getROI(IplImage * in);
  IplImage * filterContours(IplImage * in);
  int ** resolveMatrix(IplImage * in);
  void contourDance(IplImage * in);
};

#endif
