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
      if(bound.width*bound.height > maxBound.width*maxBound.height)
	maxBound = bound;
    }

  cvSetImageROI(in,maxBound);
  roi = cvCreateImage(cvGetSize(in),in->depth,in->nChannels);
  cvCopy(in,roi,NULL);
  cvResetImageROI(in);
  cvReleaseImage(&inCpy);

  return roi;
}

IplImage * filterContours(IplImage * in)
{
  IplImage * inCpy = cvCloneImage(in);
  IplImage * out = cvCreateImage( cvSize( in->width, in->height ), IPL_DEPTH_8U, 1 );

  CvMemStorage * storage = cvCreateMemStorage(0);
  CvSeq * contour = 0;
  cvFindContours(inCpy, storage, &contour, sizeof(CvContour),CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
  for(; contour != 0; contour = contour->h_next)
    {
      CvRect bound = cvBoundingRect(contour, 1);
      if(bound.width > out->width*0.8 || bound.height > out->height*0.8)
	continue;
      if(bound.width < out->width*0.8 && bound.height < out->height*0.8 &&
	 bound.width > 10 && bound.height > 10)
	cvDrawContours(out,contour,CV_RGB(255,255,255),CV_RGB(255,255,255),CV_FILLED);
    }

  cvReleaseImage(&inCpy);

  return out;
}

int ** resolveMatrix(IplImage * in) //wskazniki na rozmiary
{
  IplImage * inCpy = cvCloneImage(in);

  CvMemStorage * storage = cvCreateMemStorage(0);
  CvSeq * first = 0;
  CvSeq * contour = 0;
  int maxWidth = 0;
  int maxHeight = 0;
  cvFindContours(inCpy, storage, &first, sizeof(CvContour),CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
  for(contour = first;  contour != 0; contour = contour->h_next)
    {
      CvRect bound = cvBoundingRect(contour, 1);
      if(bound.width > maxWidth)
	maxWidth = bound.width;
      if(bound.height > maxHeight)
	maxHeight = bound.height;
      cvRectangle(in,cvPoint(bound.x,bound.y),cvPoint(bound.x+bound.width,bound.y+bound.height),CV_RGB(255,255,255),1);
    }

  //znajdz pasy (pas = klasa/struktura) i zrob ich liste
  //do pasow wrzucaj cyfry (cyfry = klasa/struktura [->contour + rect])
  //dla kazdego pasa przesortuj cyfry po x-ie
  //dla kazdego pasa przerob cyfry w liczby na podstawie maxW... i maxH... (liczby = klasa/struktura)
  //dla kazdej liczby rozwiaz jej wartosc (jak nie mozesz to -1 cala)

  cvReleaseImage(&inCpy);

  return NULL;
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
	  //show image & huMoment (store huMoment)
	  CvMoments moments;
	  CvHuMoments huMoments;
	  cvMoments(contour,&moments);
	  cvGetHuMoments(&moments,&huMoments);
	  cout << huMoments.hu1 << " " << huMoments.hu2 << " " << huMoments.hu3 << endl;
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
  IplImage * filtered = filterContours(roi);
  // resolveMatrix(filtered);

  cvNamedWindow("src",CV_WINDOW_AUTOSIZE);
  cvNamedWindow("fix1",CV_WINDOW_AUTOSIZE);
  cvNamedWindow("fix2",CV_WINDOW_AUTOSIZE);

  cvShowImage("src", rdy);
  cvShowImage("fix1", roi);
  cvShowImage("fix2", filtered);

  cvWaitKey(0);
  // contourDance(filtered);

  cvDestroyWindow("src");
  cvDestroyWindow("fix1");
  cvDestroyWindow("fix2");

  cvReleaseImage(&img);
  cvReleaseImage(&rdy);
  cvReleaseImage(&roi);
  cvReleaseImage(&filtered);

  return 0;
}
