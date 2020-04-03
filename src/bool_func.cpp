// leins, 02.06.2018

#include <iostream>
#include <cstdlib>
#include "bool_func.h"
#include "rdnf.h"

using namespace std;

// allocate memory, used in constructors only
void bool_func_cl::init(uint n)
{
  uint m = (1 << n);
  numOfVar = n;
  TVT = new bool[m];
  for (uint i = 0 ; i < m; i++)
    TVT[i] = 0x0;
  ZheP = new bool[m];
  mKarn = 1 << (n >> 1); // num of strings
  LKarn = 1 << (n / 2  + n % 2); // num of vectors
  Karn = new bool*[mKarn];
  for (uint i = 0; i < mKarn; i++)
    Karn[i] = new bool[LKarn];
  RDNF = new bool*[mKarn];
  for (uint i = 0; i < mKarn; i++)
    RDNF[i] = new bool[LKarn];
} // end of init func

uint bool_func_cl::grey(uint ind)
{
  return ind ^ (ind >> 1);
} // grey from index

// Index of Grey Code
uint bool_func_cl::grey_to_ind(uint n, uint x)
{
  uint d = 0;
  uint p = 0;
  uint mask = 1 << numOfVar;

  for (uint i = 0; i < n; i++)
  {
	uint b = (x & mask) ^ p;

	p = b;
	d <<= 1;
	if (b == 1)
	  d++;
  }
  return d;
} // end of grey_to_ind func

// set one variable in tvt
/*
void bool_func_cl::put_point_val(uint x, bool val)
{
  TVT[x] = val;
} // end of put_point_val function
*/
// destructor
bool_func_cl::~bool_func_cl()
{
  size_t i;
  for (i = 0; i < mKarn; i++)
    delete[] RDNF[i];
  delete[] RDNF;
  for (i = 0; i <mKarn; i++)
    delete[] Karn[i];
  delete[] Karn;
  delete[] ZheP;
  delete[] TVT;
}

// source function to get PDNF, PCNF value
// invert "corretct" bits in x 
uint bool_func_cl::invertion_by_type(uint x, func_type_t type)
{
  uint result = 0;

  if (type == t_PDNF) // 0 --> 1
  {
	for (uint mask = 1 << numOfVar; mask > 0; mask >>= 1)
	  if ((mask & x) == 1)
		x ^= mask;
	result = x;
  }
  else if (type == t_PCNF) // 1 --> 0
  {
	for (uint mask = 1 << numOfVar; mask > 0; mask >>= 1)
	  if ((mask & x) == 0)
		x ^= mask;
	result = x;
  }
  else
  {
	cout << "(invertion_by_type): incorrect func type(PDNF, PCNF only)" << endl;
  }

  return result;
} // end of invertion by type function

bool bool_func_cl::consolidate(uint x, func_type_t type)
{
  bool result = false;

  if (type == t_PDNF) // &
  {
	for (uint mask = 1 << numOfVar; mask > 0; mask >>= 1)  
	  result &= (x & mask);
  }
  else if (type == t_PCNF) // |
  {
	for (uint mask = 1 << numOfVar; mask > 0; mask >>= 1)
	  result |= x & mask;
  }
  else
  {
	cout << "(consolidate): incorrect func type(PDNF, PCNF only)" << endl;
  }

  return result;
} // end of consolidate func

// this function generate 
// RDNF table values
// first string - is minterm in RDNF
// second string - invertion mask
bool ** bool_func_cl::quine_mcCluskey()
{
  bool **RDNF = new bool*[2]; // two vectors in RDNF matr

  for (uint i = 0; i < 2; i++)
	RDNF[i] = new bool[mRDNF];



  return RDNF;
} // end of quine_mcCluskey func

// get values
bool bool_func_cl::get_val(uint x, func_type_t type = t_TVT)
{
  uint Xmask = MASK >> (MAX_2_POWER_DEG - numOfVar);
  bool retVal = false;

  switch (type)
  {
  case t_TVT:
	retVal = TVT[x]; // x is a vector of bool variables, in uint x is an index
	break;
  case t_PDNF:
	bool PDNFfX = true;
	uint val = 0;
	uint tmp;

	for (uint i = 0; i < m; i++, val++)
	{
	  if (TVT[i] == 1)
	  {
		tmp = consolidate(invertion_by_type(val, type), type);
		PDNFfX |= tmp;
	  }
	}
	retVal = PDNFfX;
	break;
  case t_PCNF:
	bool PCNFfX = true;
	uint val = 0;
	uint tmp;

	for (uint i = 0; i < m; i++, val++)
	{
	  if (TVT[i] == 1)
	  {
		tmp = consolidate(invertion_by_type(val, type), type);
		PCNFfX &= tmp;
	  }
	}
	retVal = PCNFfX;
	break;
  case t_ZheP:
	bool ZhePfX = false;
	uint val = 0;

	for (uint i = 0; i < m; i++, val++)
	{
	  ZhePfX ^= (i == 0) ? ZheP[i] : (ZheP[i] & (x & val));
	}
	retVal = ZhePfX;
	break;
  case t_Karn:
	uint i = grey_to_ind(mKarn, x);
	uint j = grey_to_ind(LKarn, x);

	retVal = Karn[i][j];
  case t_RDNF:

	retVal = (x & RDNF[0][x]) ^ RDNF[1][x];
	
	break;
  default:
	cout << "Val(X, t): UnKnown RepT t = " << type << endl;
  } // end switch
} // end of get_val

// convertors
void bool_func_cl::from_TVT(func_type_t type)
{
  uint **tmp = new uint*[m];
  uint max = m;

  for (uint i = 0; i < m; i++)
	tmp[i] = new uint[m];

  switch (type)
  {
  case t_ZheP:
	for (uint i = 0; i < m; i++)
	  tmp[0][i] = TVT[i];
	for (uint i = 1; i < m; i++)
	  for (uint j = 0; j < max - 1; j++)
	  {
		tmp[i][0] = tmp[i - 1][j] + tmp[i - 1][j + 1];
		max--;
	  }
	for (uint i = 0; i < m; i++)
	  ZheP[i] = tmp[0][1];
	break;
  case t_Karn:
	for (uint i = 0; i < mKarn; i++)
	  for (uint j = 0; j < LKarn; j++)
	  {
		uint x = grey(i) << (numOfVar % 2 ? numOfVar / 2 : numOfVar / 2 + 1) + grey(j);

		Karn[i][j] = TVT[x];
	  }
	break;
  case t_RDNF:
	rdnf_cl rdnf(TVT, m);
	
	RDNF = rdnf.create_table(TVT);
	break;
  }

  for (uint i = 0; i < m; i++)
	delete[] tmp[i];
  delete[] tmp;
} // end of from_TVT converter

// ???
void bool_func_cl::to_TVT(func_type_t type)
{
  switch (type)
  {
  case t_ZheP:
  case t_Karn:
  case t_PDNF:
  case t_PCNF:
  case t_RDNF:
	for (uint i = 0; i < m; i++)
	  TVT[i] = get_val(i, type);
	break;
  default:
	;
	 // ...
  }
} // end of to_TVT converter

// constructors
// without init value
bool_func_cl::bool_func_cl(byte n)
{
  this->init(n);
} // end of bool_func_cl()

// vector
bool_func_cl::bool_func_cl(byte n, uint *vect, func_type_t type = t_TVT)
{
  this->init(n);

  switch (type)
  {
  case t_TVT:
  case t_PCNF:
  case t_PDNF:
	for (uint i = 0; i < m; i++)
	  TVT[i] = vect[i];
	from_TVT(t_ZheP);
	from_TVT(t_Karn);
	// from_TVT(t_RDNF);
	break;
  default:
	// ...
  } // switch(type)

} // end of bool_func_cl(uint*)

// table
bool_func_cl::bool_func_cl(byte n, uint **vect, func_type_t type = t_Karn)
{
  this->init(n);

  switch (type)
  {
  case t_Karn:
	for (uint i = 0; i < mKarn; i++)
	  for (uint j = 0; j < LKarn; j++)
		Karn[i][j] = vect[i][j];
	to_TVT(t_Karn);
	from_TVT(t_ZheP);
	from_TVT(t_RDNF);
  case t_RDNF:
	for (uint i = 0; i < 2; i++)
	  for (uint j = 0; j < mRDNF; j++)
		Karn[i][j] = vect[i][j];
	to_TVT(t_RDNF);
	from_TVT(t_ZheP);
	from_TVT(t_Karn);
	break;
  default:
	// ...
  } // switch(type)
} // end of bool_func_cl(uint **)

// i/o operators
void bool_func_cl::operator<<(std::fstream &stream)
{

} // end of << operator

void bool_func_cl::operator>>(std::fstream &stream)
{

} // end of >> operator

// check
/*
bool bool_func_cl::is_correct(); // compare all types values
bool bool_func_cl::check_save_zero();
bool bool_func_cl::check_save_one();
bool bool_func_cl::check_self_duality();
bool bool_func_cl::check_mono();
bool bool_func_cl::check_line();
bool bool_func_cl::check_symmetric();
*/