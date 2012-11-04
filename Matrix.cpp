#include "Matrix.h"

using namespace std;

Matrix::Matrix(IplImage * in, Raw * raw)
{
  this->raw = raw;
  IplImage * seted = setUp(in);
  IplImage * roied = getROI(seted);
  matrix = filterContours(roied);
  cvReleaseImage(&seted);
  cvReleaseImage(&roied);
}

Matrix::~Matrix()
{
  cvReleaseImage(&matrix);
}

IplImage * Matrix::show()
{
  return matrix;
}

void Matrix::learn()
{
  IplImage * cpy = cvCloneImage(matrix);

  cvNamedWindow("learn",CV_WINDOW_AUTOSIZE);

  CvMemStorage * storage = cvCreateMemStorage(0);
  CvSeq * contour = 0;
  cvFindContours(cpy, storage, &contour, sizeof(CvContour),CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
  list<Hu> hus;
  for(;contour != 0; contour = contour->h_next)
    {
      CvRect bound = cvBoundingRect(contour, 1);
      CvMoments moments;
      CvHuMoments huMoments;
      cvMoments(contour,&moments);
      cvGetHuMoments(&moments,&huMoments);
      hus.push_back(Hu(huMoments.hu1,
		       huMoments.hu2,
		       huMoments.hu3,
		       huMoments.hu4,
		       huMoments.hu5,
		       huMoments.hu6,
		       huMoments.hu7));
      cvDrawContours(cpy,contour,cvScalar(255),cvScalar(255),CV_FILLED,1);
      cvShowImage("learn", cpy);
      cvWaitKey(0);
    }

  cvDestroyWindow("learn");
  // cvReleaseMemStorage(&storage);
  // cvReleaseImage(&cpy);
  cout << "Type " << hus.size() << " integers:\n";
  fstream in("base",fstream::out | fstream::app);
  for(list<Hu>::iterator i = hus.begin(); i != hus.end(); i++)
    {
      int digit;
      cin >> digit;
      in << digit;
      double * phus = i->getHus();
      for(int i = 0; i < 7; i++)
	in << " " << phus[i];
      in << endl;
    }
  in.close();
}

IplImage * Matrix::setUp(IplImage * in)
{
  IplImage * gray = cvCreateImage( cvSize( in->width, in->height ), IPL_DEPTH_8U, 1 );

  cvCvtColor(in, gray, CV_RGB2GRAY);

  cvSmooth(gray, gray, CV_GAUSSIAN, 11, 11, 2, 2);
  cvCanny(gray, gray, 20, 30, 3);

  CvMemStorage * storage = cvCreateMemStorage(0);
  CvSeq * contour = 0;
  cvFindContours(gray, storage, &contour, sizeof(CvContour),CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
  for(;contour != 0; contour = contour->h_next)
    {
      cvDrawContours(gray,contour,CV_RGB(255,255,255),CV_RGB(255,255,255),CV_FILLED,5);
    }

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

void Matrix::resolve()
{
  IplImage * cpy = cvCloneImage(matrix);

  CvMemStorage * storage = cvCreateMemStorage(0);
  CvSeq * contour = 0;
  cvFindContours(cpy, storage, &contour, sizeof(CvContour),CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
  list<Row> rows;
  for(; contour != 0; contour = contour->h_next)
    {
      CvRect bound = cvBoundingRect(contour, 1);
      CvMoments moments;
      CvHuMoments huMoments;
      cvMoments(contour,&moments);
      cvGetHuMoments(&moments,&huMoments);
      Hu moms(huMoments.hu1,
	      huMoments.hu2,
	      huMoments.hu3,
	      huMoments.hu4,
	      huMoments.hu5,
	      huMoments.hu6,
	      huMoments.hu7);
      Digit present(bound.x,bound.y,bound.width,bound.height,moms,raw);
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
    }
  rows.sort();
  for(list<Row>::iterator i = rows.begin(); i != rows.end(); i++)
    {
      i->sort();
      i->print();
    }

  cvReleaseImage(&cpy);
  // cvReleaseMemStorage(&storage);
}
