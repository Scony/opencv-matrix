#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <string>

using namespace std;

int main()
{ 
  IplImage * src = cvLoadImage("output.jpg",0);
  IplImage * dst = cvCreateImage( cvSize( src->width/2, src->height/2 ), src->depth, src->nChannels );
  IplImage * dst2 = cvCreateImage( cvSize( dst->width/2, dst->height/2 ), dst->depth, dst->nChannels );
  IplImage * dst3;

  int tr1 = 10;
  int tr2 = 60;
  int alt = 3;


  cvNamedWindow("raw",CV_WINDOW_AUTOSIZE);
  cvNamedWindow("fixed",CV_WINDOW_AUTOSIZE);
  cvNamedWindow("fixed2",CV_WINDOW_AUTOSIZE);
  cvNamedWindow("fixed3",CV_WINDOW_AUTOSIZE);
  cvCreateTrackbar("tr1","fixed",&tr1,256,NULL);
  cvCreateTrackbar("tr2","fixed",&tr2,256,NULL);
  cvCreateTrackbar("alt","fixed",&alt,256,NULL);

  // cvCopy(frame,cpy);
  // cvCvtColor(frame, dst, CV_RGB2GRAY);

  cvWaitKey(0);

  cvSmooth(src, src, CV_GAUSSIAN, 11, 11, 2, 2);
  cvPyrDown(src,dst);
  cvPyrDown(dst,dst2);
  dst3 = cvCloneImage(dst2);
  cvCanny(dst2, dst2, tr1, tr2, alt);

  cvShowImage("raw", src);
  cvShowImage("fixed", dst);
  cvShowImage("fixed2", dst2);
  cvShowImage("fixed3", dst3);
  // cvShowImage("fixed", dst);
  // cvShowImage("cpy", cpy);

  cvWaitKey(0);

  CvMemStorage * storage = cvCreateMemStorage(0);
  CvSeq * contour = 0;
  cvFindContours(dst2, storage, &contour, sizeof(CvContour),CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); // odszukac drzewo
  for(;contour != 0; contour = contour->h_next)
    {
      cvDrawContours(dst3,contour,CV_RGB(255,153,0),CV_RGB(255,153,0),CV_FILLED);
      CvRect rect = cvBoundingRect(contour, 1);
      cvRectangle(dst3, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height), CV_RGB(255, 0, 0), 1);
      cvShowImage("fixed3", dst3);
      cvWaitKey(0);
    }

  cvWaitKey(0);

  // cvErode(frame,frame,NULL,10);
  //cvThreshold(dst, dst, 100, 256, CV_THRESH_BINARY); // 70 x 150
  // cvDilate(frame,frame,NULL,10);

  cvDestroyWindow("raw");
  cvDestroyWindow("fixed");
  cvDestroyWindow("fixed2");
  cvDestroyWindow("fixed3");

  cvReleaseImage(&src);
  cvReleaseImage(&dst);
  cvReleaseImage(&dst2);
  cvReleaseImage(&dst3);
  // cvReleaseImage(&dst);

  return 0;
}

