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

  CvCapture * capture = cvCaptureFromCAM(CV_CAP_ANY);

  IplImage * frame = NULL;
  IplImage * src = NULL;
  IplImage * cpy = NULL;
  IplImage * oldSrc = NULL;

  cvNamedWindow("MatSolver",CV_WINDOW_AUTOSIZE);

  while(1)
    {
      frame = cvQueryFrame(capture);

      oldSrc = src;
      src = cvCloneImage(frame);
      cpy = cvCreateImage(cvSize(src->width,src->height),IPL_DEPTH_8U,1);
      cvCvtColor(src,cpy,CV_RGB2GRAY);

      cvSmooth(cpy, cpy, CV_GAUSSIAN, 11, 11, 2, 2);
      cvCanny(cpy, cpy, 10, 60, 3);

      CvMemStorage * storage = cvCreateMemStorage(0);
      CvSeq * first = NULL;
      CvSeq * contour = NULL;
      cvFindContours(cpy, storage, &first, sizeof(CvContour),CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
      CvSeq * max = first;
      int maxArea = 0;
      for(contour = first; contour != 0; contour = contour->h_next)
	{
	  CvRect bound = cvBoundingRect(contour,0);
	  if(maxArea < bound.width * bound.height)
	    {
	      max = contour;
	      maxArea = bound.width * bound.height;
	    }
	}
      for(contour = first; contour != 0; contour = contour->h_next)
	if(contour != max)
	  cvDrawContours(src,contour,CV_RGB(0,255,255),CV_RGB(0,255,255),CV_FILLED);
	else
	  {
	    CvScalar color(CV_RGB(255,0,0));
	    CvRect bound = cvBoundingRect(contour,0);
	    cvDrawContours(src,contour,color,color,CV_FILLED);
	    cvRectangle(src,cvPoint(bound.x,bound.y),cvPoint(bound.x+bound.width,bound.y+bound.height),color,1);
	  }

      cvShowImage("MatSolver",src);

      cvReleaseMemStorage(&storage);
      cvReleaseImage(&cpy);
      if(oldSrc)
	cvReleaseImage(&oldSrc);
      
      if((cvWaitKey(10) & 255) == 10)
	break;

    }

  cvShowImage("MatSolver",frame);

  cvWaitKey(0);

  Matrix mat(frame,&raw);

  cvShowImage("MatSolver",mat.show());

  cvWaitKey(0);

  if(argc > 1)
    mat.learn();
  else
    mat.resolve();

  cvDestroyWindow("MatSolver");
  if(src)
    cvReleaseImage(&src);
  cvReleaseCapture(&capture);

  return 0;
}
