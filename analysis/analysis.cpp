#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <string>

using namespace std;

int main()
{ 
  CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);

  IplImage * frame;
  IplImage * dst;
  IplImage * cpy;
  int zm = 153;

  cvNamedWindow("raw",CV_WINDOW_AUTOSIZE);
  cvCreateTrackbar("somee","raw",&zm,256,NULL);
  cvNamedWindow("fixed",CV_WINDOW_AUTOSIZE);
  cvNamedWindow("cpy",CV_WINDOW_AUTOSIZE);

  while(1)
    {

      frame = cvQueryFrame(capture);

      dst = cvCreateImage( cvSize( frame->width, frame->height ), IPL_DEPTH_8U, 1 );
      cpy = cvCreateImage( cvSize( frame->width, frame->height ), frame->depth, frame->nChannels );
      cvCopy(frame,cpy);
      cvCvtColor(frame, dst, CV_RGB2GRAY);

      cvSmooth(dst, dst, CV_GAUSSIAN, 11, 11, 2, 2);
      cvCanny(dst, dst, 10, 60, 3);

      CvMemStorage * storage = cvCreateMemStorage(0);
      CvSeq * contour = 0;
      if((cvWaitKey(10) & 255) == 10)
	{
	  cvSaveImage("output.jpg",frame,0);
	  break;
	}
      cvFindContours(dst, storage, &contour, sizeof(CvContour),CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); // odszukac drzewo
      for(;contour != 0; contour = contour->h_next)
	cvDrawContours(cpy,contour,CV_RGB(255,153,0),CV_RGB(255,153,0),CV_FILLED);

      // cvErode(frame,frame,NULL,10);
      //cvThreshold(dst, dst, 100, 256, CV_THRESH_BINARY); // 70 x 150
      // cvDilate(frame,frame,NULL,10);

      cvShowImage("raw", frame);
      cvShowImage("fixed", dst);
      cvShowImage("cpy", cpy);

      if((cvWaitKey(10) & 255) == 27)
	break;
    }

  cvDestroyWindow("raw");
  cvDestroyWindow("fixed");
  cvDestroyWindow("cpy");
  cvReleaseImage(&frame);
  cvReleaseImage(&dst);
  cvReleaseImage(&cpy);
  cvReleaseCapture(&capture);

  return 0;
}
