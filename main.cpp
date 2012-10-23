#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <string>

using namespace std;

IplImage * setUp(IplImage * in)
{
  //konwersja
  //rozmycie
  //zmniejszenie
  //canny
  IplImage * neu = cvCloneImage(in);
  return neu;
}

int main()
{
  IplImage * img = cvLoadImage("frame.jpg",1);
  IplImage * rdy = setUp(img);
  
  cvNamedWindow("src",CV_WINDOW_AUTOSIZE);

  cvShowImage("src", rdy);

  cvWaitKey(0);

  cvDestroyWindow("src");

  cvReleaseImage(&img);
  cvReleaseImage(&rdy);

  return 0;
}
