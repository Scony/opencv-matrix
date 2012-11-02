#include "Hu.h"

using namespace std;

Hu::Hu(double h1, double h2, double h3, double h4, double h5, double h6, double h7)
{
  hu[0] = h1;
  hu[1] = h2;
  hu[2] = h3;
  hu[3] = h4;
  hu[4] = h5;
  hu[5] = h6;
  hu[6] = h7;
}

Hu::Hu(double * in)
{
  for(int i = 0; i < 7; i++)
    hu[i] = in[i];
}

void Hu::setDigit(int digit)
{
  this->digit = digit;
}

void Hu::print()
{
  for(int i = 0; i < 7; i++)
    cout << hu[i] << " ";
  cout << "= " << digit << endl;
}

int Hu::getDigit()
{
  return digit;
}

double * Hu::getHus()
{
  return hu;
}

double Hu::match(Hu in)
{
  double result = 0.0;
  for(int i = 0; i < 7; i++)
    result += abs(hu[i]-in.hu[i]);
  return result;
}
