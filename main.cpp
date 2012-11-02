#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <string>
#include <list>

#include "Raw.h"
#include "Matrix.h"

using namespace std;

Raw raw("template.jpg");

int main()
{
  // cout.setf(ios::fixed);

  // IplImage * img = cvLoadImage("frame.jpg",1);
  // IplImage * rdy = setUp(img);
  // IplImage * roi = getROI(rdy);
  // IplImage * filtered = filterContours(roi);
  // // resolveMatrix(filtered);

  // cvNamedWindow("src",CV_WINDOW_AUTOSIZE);
  // cvNamedWindow("fix1",CV_WINDOW_AUTOSIZE);
  // cvNamedWindow("fix2",CV_WINDOW_AUTOSIZE);

  // cvShowImage("src", rdy);
  // cvShowImage("fix1", roi);
  // cvShowImage("fix2", filtered);

  // cvWaitKey(0);
  // contourDance(filtered);

  // cvDestroyWindow("src");
  // cvDestroyWindow("fix1");
  // cvDestroyWindow("fix2");

  // cvReleaseImage(&img);
  // cvReleaseImage(&rdy);
  // cvReleaseImage(&roi);
  // cvReleaseImage(&filtered);

  return 0;
}
