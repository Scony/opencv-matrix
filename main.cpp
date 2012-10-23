#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <string>

using namespace std;

IplImage * setUp(IplImage * in)
{
  IplImage * gray = cvCreateImage( cvSize( in->width, in->height ), IPL_DEPTH_8U, 1 );
  IplImage * grayd2 = cvCreateImage( cvSize( in->width/2, in->height/2 ), IPL_DEPTH_8U, 1 );
  IplImage * grayd4 = cvCreateImage( cvSize( in->width/4, in->height/4 ), IPL_DEPTH_8U, 1 );

  cvCvtColor(in, gray, CV_RGB2GRAY);

  cvSmooth(grayd4, grayd4, CV_GAUSSIAN, 11, 11, 2, 2);
  cvPyrDown(gray,grayd2);
  cvPyrDown(grayd2,grayd4);
  cvCanny(grayd4, grayd4, 10, 60, 3);

  cvReleaseImage(&gray);
  cvReleaseImage(&grayd2);

  return grayd4;
}

IplImage * getROI(IplImage * in)
{
  IplImage * inCpy = cvCloneImage(in);
  IplImage * roi;

  CvMemStorage * storage = cvCreateMemStorage(0);
  CvSeq * contour = 0;
  CvRect maxBound = cvRect(0,0,1,1);
  cvFindContours(inCpy, storage, &contour, sizeof(CvContour),CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
  for(;contour != 0; contour = contour->h_next)
    {
      CvRect bound = cvBoundingRect(contour, 1);
      if((bound.width-bound.x)*(bound.height-bound.y) > (maxBound.width-maxBound.x)*(maxBound.height-maxBound.y))
	maxBound = bound;
    }

  cvSetImageROI(in,maxBound);
  roi = cvCloneImage(in);
  cvResetImageROI(in);
  cvReleaseImage(&inCpy);

  return roi;
}

int main()
{
  IplImage * img = cvLoadImage("frame.jpg",1);
  IplImage * rdy = setUp(img);
  IplImage * roi = getROI(rdy);
  
  cvNamedWindow("src",CV_WINDOW_AUTOSIZE);
  cvNamedWindow("fix1",CV_WINDOW_AUTOSIZE);

  cvShowImage("src", rdy);
  cvShowImage("fix1", roi);

  cvWaitKey(0);

  cvDestroyWindow("src");

  cvReleaseImage(&img);
  cvReleaseImage(&rdy);
  cvReleaseImage(&roi);

  return 0;
}
