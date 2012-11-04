#include "Hu.h"

using namespace std;

Hu::Hu()
{
  for(int i = 0; i < 7; i++)
    hu[i] = 0;
  digit = 0;
}

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

double sgn(double num)
{
  return num == 0 ? 0 : num > 0 ? 1 : -1;
}

double abs(double x)
{
  return x < 0 ? -1*x : x;
}

double Hu::match(Hu in)
{
  double result = 0.0;
  for(int i = 0; i < 7; i++)
    // result += abs(log(hu[i])*sgn(hu[i])-log(in.hu[i])*sgn(in.hu[i]));
    result += abs(hu[i]-in.hu[i]);
    // cout << result << "=" << hu[i] << "vs" << in.hu[i] << endl;
  return result;
}
