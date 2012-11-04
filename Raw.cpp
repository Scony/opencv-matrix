#include "Raw.h"

using namespace std;

Raw::Raw()
{
  fstream in("base",fstream::in);
  int x;
  double y[7];
  while(in >> x >> y[0] >> y[1] >> y[2] >> y[3] >> y[4] >> y[5] >> y[6])
    {
      if(x < 0 || x > 9)
	 continue;
      Hu hu(y);
      digits[x].push_back(hu);
    }
}

Raw::~Raw()
{

}

int Raw::bestMatch(Hu digit)
{
  int sln = 0;
  double result = 99999;
  for(int i = 0; i < 10; i++)
    for(list<Hu>::iterator j = digits[i].begin(); j != digits[i].end(); j++)
      if(j->match(digit) < result)
	{
	  result = j->match(digit);
	  sln = i;
	}
  return sln;
}
