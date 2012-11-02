#include "Digit.h"

Digit::Digit(int x, int y, int width, int height, CvSeq * contour)
{
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
  this->contour = contour;
}

Digit::~Digit()
{
  //
}

int Digit::getLeft()
{
  return x;
}

int Digit::getTop()
{
  return y;
}

int Digit::getBot()
{
  return y + height;
}

int Digit::getMid()
{
  return (2*y + height) / 2;
}

bool Digit::operator < (Digit & other)
{
  return x < other.getLeft();
}
