#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <string>

using namespace std;

int main()
{ 
  IplImage * frame;
  IplImage * clone = NULL;
  int a = 10, b = 60;

  cvNamedWindow("raw",CV_WINDOW_AUTOSIZE);
  cvCreateTrackbar("a","raw",&a,256,NULL);
  cvCreateTrackbar("b","raw",&b,256,NULL);

  while(1)
    {
      frame = cvLoadImage("frame.jpg",0);
      if(!clone)
	clone = cvCloneImage(frame);
      cvSmooth(clone, clone, CV_GAUSSIAN, 11, 11, 2, 2);
      cvCanny(clone, clone, a, b, 3);
      cvWaitKey(1);
      cvShowImage("raw", clone);
      cvReleaseImage(&clone);
      cvReleaseImage(&frame);
    }

  cvWaitKey(0);
  cvDestroyWindow("raw");
  return 0;
}
