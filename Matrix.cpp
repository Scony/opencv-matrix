#include "Matrix.h"

using namespace std;

IplImage * Matrix::setUp(IplImage * in)
{
  IplImage * gray = cvCreateImage( cvSize( in->width, in->height ), IPL_DEPTH_8U, 1 );
  // IplImage * grayd2 = cvCreateImage( cvSize( in->width/2, in->height/2 ), IPL_DEPTH_8U, 1 );
  // IplImage * grayd4 = cvCreateImage( cvSize( in->width/4, in->height/4 ), IPL_DEPTH_8U, 1 );

  cvCvtColor(in, gray, CV_RGB2GRAY);

  // cvErode(gray,gray,NULL,4);
  // cvDilate(gray,gray,NULL,4);
  cvSmooth(gray, gray, CV_GAUSSIAN, 11, 11, 2, 2); //?
  // cvPyrDown(gray,gray);
  // cvPyrDown(grayd2,grayd4);
  cvCanny(gray, gray, 20, 30, 3);

  CvMemStorage * storage = cvCreateMemStorage(0);
  CvSeq * contour = 0;
  cvFindContours(gray, storage, &contour, sizeof(CvContour),CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
  for(;contour != 0; contour = contour->h_next)
    {
      cvDrawContours(gray,contour,CV_RGB(255,255,255),CV_RGB(255,255,255),CV_FILLED,5);
    }

  // cvReleaseImage(&gray);
  // cvReleaseImage(&grayd4);
  cvReleaseMemStorage(&storage);

  return gray;
}

IplImage * Matrix::getROI(IplImage * in)
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
  // cvReleaseMemStorage(&storage);

  return roi;
}

IplImage * Matrix::filterContours(IplImage * in)
{
  IplImage * inCpy = cvCloneImage(in);
  IplImage * out = cvCloneImage(in);

  CvMemStorage * storage = cvCreateMemStorage(0);
  CvSeq * contour = 0;
  cvFindContours(inCpy, storage, &contour, sizeof(CvContour),CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
  for(; contour != 0; contour = contour->h_next)
    {
      CvRect bound = cvBoundingRect(contour, 1);
      if(bound.width > out->width*0.8 || bound.height > out->height*0.8)
	cvDrawContours(out,contour,cvScalar(0),cvScalar(0),CV_FILLED,15);
    }

  cvReleaseImage(&inCpy);
  // cvReleaseMemStorage(&storage);

  return out;
}

int ** Matrix::resolveMatrix(IplImage * in) //wskazniki na rozmiary
{
  IplImage * inCpy = cvCloneImage(in);

  CvMemStorage * storage = cvCreateMemStorage(0);
  CvSeq * first = 0;
  CvSeq * contour = 0;
  // int maxWidth = 0;
  // int maxHeight = 0;
  cvFindContours(inCpy, storage, &first, sizeof(CvContour),CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
  list<Row> rows;
  for(contour = first;  contour != 0; contour = contour->h_next)
    {
      CvRect bound = cvBoundingRect(contour, 1);
      Digit present(bound.x,bound.y,bound.width,bound.height,contour);
      bool neu = true;
      for(list<Row>::iterator i = rows.begin(); i != rows.end(); i++)
	if(i->match(present))
	  {
	    neu = false;
	    i->insert(present);
	    break;
	  }
      if(neu)
	rows.push_back(Row(present));
      // if(bound.width > maxWidth)
      // 	maxWidth = bound.width;
      // if(bound.height > maxHeight)
      // 	maxHeight = bound.height;
      cvRectangle(in,cvPoint(bound.x,bound.y),cvPoint(bound.x+bound.width,bound.y+bound.height),CV_RGB(255,255,255),1);
    }
  for(list<Row>::iterator i = rows.begin(); i != rows.end(); i++)
    {
      i->sort();
      i->print();
      cout << i->size() << endl;
    }

  //dla kazdego pasa przerob cyfry w liczby na podstawie maxW... i maxH... (liczby = klasa/struktura)
  //dla kazdej liczby rozwiaz jej wartosc (jak nie mozesz to -1 cala)

  cvReleaseImage(&inCpy);
  // cvReleaseMemStorage(&storage);

  return NULL;
}

void Matrix::contourDance(IplImage * in)
{
  IplImage * inCpy = cvCloneImage(in);
  IplImage * colored = cvCreateImage(cvSize(inCpy->width,inCpy->height),8,3);

  cvNamedWindow("dance",CV_WINDOW_AUTOSIZE);

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
	  cout << huMoments.hu1 << " " << huMoments.hu2 << " " << huMoments.hu3 <<
	    " " << huMoments.hu4 << " " << huMoments.hu5 << " " << huMoments.hu6 <<
	    " " << huMoments.hu7 << "//" << cvMatchShapes(firstContour,contour,CV_CONTOURS_MATCH_I1) << "::" << // raw.bestMatch(contour) << 
	    endl;
	  if((cvWaitKey(1000) & 255) == 27)
	    {
	      exit = true;
	      break;
	    }
	}
    }

  cvDestroyWindow("dance");
}
