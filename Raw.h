#ifndef RAW_H
#define RAW_H

#include <opencv/cv.h>
#include <opencv/highgui.h>

class Raw
{
  CvMemStorage * storage;
  CvSeq * digits[10];
public:
  Raw(std::string fname);
  ~Raw();
  int bestMatch(CvSeq * contour);
};

#endif
