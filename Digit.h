#ifndef DIGIT_H
#define DIGIT_H

#include <opencv/cv.h>
#include <opencv/highgui.h>

class Digit
{
  int x;
  int y;
  int width;
  int height;
  CvSeq * contour;
public:
  Digit(int x, int y, int width, int height, CvSeq * contour);
  ~Digit();
  int getLeft();
  int getTop();
  int getBot();
  int getMid();
  bool operator < (Digit & other);
};

#endif
