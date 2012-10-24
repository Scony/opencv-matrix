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

  // cvErode(gray,gray,NULL,4);
  // cvDilate(gray,gray,NULL,4);
  cvSmooth(gray, gray, CV_GAUSSIAN, 11, 11, 2, 2); //?
  cvPyrDown(gray,grayd2);
  // cvPyrDown(grayd2,grayd4);
  cvCanny(grayd2, grayd2, 10, 60, 3);

  cvReleaseImage(&gray);
  cvReleaseImage(&grayd4);

  return grayd2;
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
  roi = cvCreateImage(cvGetSize(in),in->depth,in->nChannels);
  cvCopy(in,roi,NULL);
  cvResetImageROI(in);
  cvReleaseImage(&inCpy);

  return roi;
}

void contourDance(IplImage * in)
{
  IplImage * inCpy = cvCloneImage(in);
  IplImage * colored = cvCreateImage(cvSize(inCpy->width,inCpy->height),8,3);

  CvMemStorage * storage = cvCreateMemStorage(0);
  CvSeq * firstContour = 0;
  cvFindContours(inCpy, storage, &firstContour, sizeof(CvContour),CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
  bool exit = false;
  while(!exit)
    {
      cvSet(colored,cvScalar(0,0,0));
      for(CvSeq * contour = firstContour; contour != 0; contour = contour->h_next)
	{
	  cvDrawContours(colored,contour,CV_RGB(255,153,0),CV_RGB(255,153,0),CV_FILLED);
	  cvShowImage("dance",colored);
	  if((cvWaitKey(1000) & 255) == 27)
	    {
	      exit = true;
	      break;
	    }
	}
    }

  cvDestroyWindow("dance");
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
  contourDance(roi);

  cvDestroyWindow("src");

  cvReleaseImage(&img);
  cvReleaseImage(&rdy);
  cvReleaseImage(&roi);

  return 0;
}
