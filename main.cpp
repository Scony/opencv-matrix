#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <string>
#include <list>

#include "Raw.h"
#include "Matrix.h"

using namespace std;

int main(int argc, char * argv[])
{
  Raw raw;

  // cout.setf(ios::fixed);

  IplImage * img = cvLoadImage("frame.jpg",1);

  cvNamedWindow("src",CV_WINDOW_AUTOSIZE);

  Matrix mat(img);
  cvShowImage("src", mat.show());
  if(argc>1)
    mat.learn();
  else
    ;

  cvWaitKey(0);

  cvDestroyWindow("src");
  cvReleaseImage(&img);

  return 0;
}
