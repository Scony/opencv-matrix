#include "Raw.h"

using namespace std;

Raw::Raw(string fname)
{
  IplImage * in = cvLoadImage(fname.c_str(),0);
  IplImage * gray = cvCreateImage( cvSize( in->width, in->height ), IPL_DEPTH_8U, 1 );
  IplImage * grayd2 = cvCreateImage( cvSize( in->width/2, in->height/2 ), IPL_DEPTH_8U, 1 );

  cvSmooth(in, in, CV_GAUSSIAN, 11, 11, 2, 2);
  cvCanny(in, in, 20, 30, 3);

  storage = cvCreateMemStorage(0);
  CvSeq * contour = 0;
  cvFindContours(in, storage, &contour, sizeof(CvContour),CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
  for(; contour != 0; contour = contour->h_next)
    {
      cvDrawContours(in,contour,CV_RGB(255,255,255),CV_RGB(255,255,255),CV_FILLED,5);
      // cvShowImage("dance",in);
      // cvWaitKey(0);
    }
  cvReleaseMemStorage(&storage);
  storage = cvCreateMemStorage(0);
  cvFindContours(in, storage, &contour, sizeof(CvContour),CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
  // for(; contour != 0; contour = contour->h_next)
  //   {
  // 	CvRect bound = cvBoundingRect(contour, 1);
  // 	cvDrawContours(in,contour,CV_RGB(255,255,255),CV_RGB(255,255,255),CV_FILLED);
  // 	cvShowImage("dance",in);
  // 	cvWaitKey(0);
  //   }
  digits[9] = contour;
  digits[0] = digits[9]->h_next;
  digits[8] = digits[0]->h_next;
  digits[1] = digits[8]->h_next;
  digits[7] = digits[1]->h_next;
  digits[5] = digits[7]->h_next;
  digits[4] = digits[5]->h_next;
  digits[3] = digits[4]->h_next;
  digits[6] = digits[3]->h_next;
  digits[2] = digits[6]->h_next;

  cvReleaseImage(&in);
  cvReleaseImage(&gray);
  cvReleaseImage(&grayd2);
}

Raw::~Raw()
{
  cvReleaseMemStorage(&storage);
}

int Raw::bestMatch(CvSeq * contour)
{
  double matches[10];
  for(int i = 0; i < 10; i++)
    matches[i] = cvMatchShapes(digits[i],contour,CV_CONTOURS_MATCH_I1);
  double min = matches[0];
  int mini = 0;
  for(int i = 1; i < 10; i++)
    if(matches[i] < min)
      {
	min = matches[i];
	mini = i;
      }
  return mini;
}
