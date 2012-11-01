#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <string>
#include <list>

using namespace std;

class Digit
{
  int x;
  int y;
  int width;
  int height;
  CvSeq * contour;
public:
  Digit(int x, int y, int width, int height, CvSeq * contour)
  {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->contour = contour;
  }
  ~Digit() {}
  int getLeft()
  {
    return x;
  }
  int getTop()
  {
    return y;
  }
  int getBot()
  {
    return y + height;
  }
  int getMid()
  {
    return (2*y + height) / 2;
  }
  bool operator < (Digit & other)
  {
    return x < other.getLeft();
  }
};

class Row
{
  list<Digit> digits;
  int top;
  int bot;
public:
  Row(Digit digit)
  {
    digits.push_back(digit);
    top = digit.getTop();
    bot = digit.getBot();
  }
  ~Row()
  {
    digits.clear();
  }
  bool match(Digit digit)
  {
    return top <= digit.getMid() && digit.getMid() <= bot ? true : false;
  }
  void insert(Digit digit)
  {
    digits.push_back(digit);
  }
  int size()
  {
    return digits.size();
  }
  void sort()
  {
    digits.sort();
  }
  void print()
  {
    for(list<Digit>::iterator i = digits.begin(); i != digits.end(); i++)
      cout << i->getLeft() << " ";
    cout << endl;
  }
};

class Raw
{
  CvMemStorage * storage;
  CvSeq * digits[10];
public:
  Raw(string fname)
  {
    IplImage * in = cvLoadImage(fname.c_str(),1);
    IplImage * gray = cvCreateImage( cvSize( in->width, in->height ), IPL_DEPTH_8U, 1 );
    IplImage * grayd2 = cvCreateImage( cvSize( in->width/2, in->height/2 ), IPL_DEPTH_8U, 1 );

    cvCvtColor(in, gray, CV_RGB2GRAY);

    cvSmooth(gray, gray, CV_GAUSSIAN, 11, 11, 2, 2);
    cvPyrDown(gray,grayd2);
    cvCanny(grayd2, grayd2, 10, 60, 3);

    storage = cvCreateMemStorage(0);
    CvSeq * contour = 0;
    cvFindContours(grayd2, storage, &contour, sizeof(CvContour),CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    // for(; contour != 0; contour = contour->h_next)
    //   {
    // 	CvRect bound = cvBoundingRect(contour, 1);
    // 	cvDrawContours(in,contour,CV_RGB(255,93,0),CV_RGB(255,93,0),CV_FILLED);
    // 	cvShowImage("dance",in);
    // 	cvWaitKey(0);
    //   }
    digits[9] = contour;
    digits[0] = digits[9]->h_next;
    digits[8] = digits[0]->h_next;
    digits[7] = digits[8]->h_next;
    digits[5] = digits[7]->h_next;
    digits[1] = digits[5]->h_next;
    digits[6] = digits[1]->h_next;
    digits[4] = digits[6]->h_next;
    digits[3] = digits[4]->h_next;
    digits[2] = digits[3]->h_next;

    cvReleaseImage(&in);
    cvReleaseImage(&gray);
    cvReleaseImage(&grayd2);
  }
  ~Raw()
  {
    cvReleaseMemStorage(&storage);
  }
  int bestMatch(CvSeq * contour)
  {
    double matches[10];
    for(int i = 0; i < 10; i++)
      matches[i] = cvMatchShapes(digits[i],contour,CV_CONTOURS_MATCH_I3);
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
};

Raw raw("template.jpg");

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
  // cvReleaseMemStorage(&storage);

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
  // cvReleaseMemStorage(&storage);

  return out;
}

int ** resolveMatrix(IplImage * in) //wskazniki na rozmiary
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

void contourDance(IplImage * in)
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
	    " " << huMoments.hu7 << "//" << cvMatchShapes(firstContour,contour,CV_CONTOURS_MATCH_I1) << "::" << raw.bestMatch(contour) << endl;
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
  cout.setf(ios::fixed);

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
  contourDance(filtered);

  cvDestroyWindow("src");
  cvDestroyWindow("fix1");
  cvDestroyWindow("fix2");

  cvReleaseImage(&img);
  cvReleaseImage(&rdy);
  cvReleaseImage(&roi);
  cvReleaseImage(&filtered);

  return 0;
}
