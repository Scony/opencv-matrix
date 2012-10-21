#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, const char* argv[])
{ 
  CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);

  IplImage* frame;
  IplImage *dst;

  cvNamedWindow("afterEffects", CV_WINDOW_AUTOSIZE);

  while (1)
    {

      frame = cvQueryFrame(capture);

      dst = cvCreateImage( cvSize( frame->width, frame->height ), IPL_DEPTH_8U, 1 );
      cvCvtColor(frame, dst, CV_RGB2GRAY);

      // cvErode(frame,frame,NULL,10);
      cvThreshold(dst, dst, 100, 256, CV_THRESH_BINARY); // 70 x 150
      // cvDilate(frame,frame,NULL,10);

      cvShowImage("afterEffects", dst);

      if ((cvWaitKey(10) & 255) == 27)
	{
	  break;
	}
    }

  cvReleaseCapture(&capture);
  cvDestroyWindow("afrerEffects");
  cvReleaseImage(&frame);

  return 0;
}
