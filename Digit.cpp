#include "Digit.h"

using namespace std;

Digit::Digit(int x, int y, int width, int height, Hu hus, Raw * raw)
{
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
  this->hus = hus;
  this->raw = raw;
}

Digit::~Digit()
{

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

int Digit::resolve()
{
  return raw->bestMatch(hus);
}

bool Digit::operator < (Digit & other)
{
  return x < other.x;
}
