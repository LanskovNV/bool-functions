// leins, 23.05.2018

#include <iostream>
#include "bool_func.h"

using namespace std;

int main()
{
  bool TVT[8] = {1, 1, 1, 0, 0, 0, 1, 0};
  bool_func_cl BF(3, TVT);
  
  cout << "BF in point 010 is: " << BF.get_val(2);

  return 0;
}
