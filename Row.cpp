#include "Row.h"

using namespace std;

Row::Row(Digit digit)
{
  digits.push_back(digit);
  top = digit.getTop();
  bot = digit.getBot();
}

Row::~Row()
{
  digits.clear();
}

bool Row::match(Digit digit)
{
  return top <= digit.getMid() && digit.getMid() <= bot ? true : false;
}

void Row::insert(Digit digit)
{
  digits.push_back(digit);
}

void Row::sort()
{
  digits.sort();
}

void Row::print()
{
  for(list<Digit>::iterator i = digits.begin(); i != digits.end(); i++)
    if(i==digits.begin())
      cout << i->resolve();
    else
      cout << " " << i->resolve();
  cout << endl;
}

bool Row::operator < (Row & other)
{
  return top < other.top;
}
