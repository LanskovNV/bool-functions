// leins, 06.06.2018

// the idea of this algorithm 
// has taken from wikipedia, 
// https://ru.wikipedia.org/wiki/%D0%9C%D0%B5%D1%82%D0%BE%D0%B4_%D0%9A%D1%83%D0%B0%D0%B9%D0%BD%D0%B0_%E2%80%94_%D0%9C%D0%B0%D0%BA-%D0%9A%D0%BB%D0%B0%D1%81%D0%BA%D0%B8

#include <algorithm>
#include "rdnf.h"

using namespace std;

rdnf_cl::rdnf_cl(bool *TVT, uint size)
{
  for (uint i = 0; i < size; i++)
	if (TVT[i] == 1)
	{
	  elem_t newEl;
	  pair_t newP;

	  allIndex.push_back(i);

	  newP.mask = 0x0;
	  newP.x = i;
	  newEl.p = newP;
      newEl.m.push_back(i);
	  newEl.completed = 0;
	  minterms.push_back(newEl);
	}
} // end of init func

// returns bit mask witch we should change in mask ( or 0 )
uint rdnf_cl::is_gluing(pair_t p1, pair_t p2)
{
  uint cnt = 0;
  uint ans = 0;

  for (uint i = 1; i < numOfVar; i << 1)
  {
	if (!is_skip(p1.mask, p2.mask, i))
	{
	  if (p1.x & i != p2.x & i) // bit is not the same
	  {
		cnt++;
		ans = i;
	  }
	}
  }
  if (cnt == 1)
	return ans;
  else
	return 0;
} // end of is_gluing

// select new not important bit in x
void rdnf_cl::change_mask(uint n, pair_t &p)
{
  p.mask ^= 1 << (n - 1);
} // end of change mask func

bool rdnf_cl::is_skip(uint mask1, uint mask2, uint n)
{
  if (mask1 & n && mask2 & n)
	return true;
  else
	return false;
} // end of skip func

elem_t rdnf_cl::glue(elem_t e1, elem_t e2, uint n) // n is a mask to change mask
{
  vector<uint>::iterator it;

  for (it = e2.m.begin(); it != e2.m.end(); it++)
	e1.m.push_back(*it);
  change_mask(n, e1.p);

  return e1;
} // end of glue func 

void rdnf_cl::mark_elem(elem_t &e)
{
  e.completed = true;
}

void rdnf_cl::glue_all()
{
  vector<elem_t> newElVect;
  vector<elem_t>::iterator it;

  for (uint i = 0; i < minterms.size() - 1; i++)
  {
	uint is_completed = 0;

	if (!minterms[i].completed)
	{
	  for (uint j = i + 1; j < minterms.size(); j++)
	  {
		if (!minterms[j].completed)
		{
		  if (uint n = is_gluing(minterms[i].p, minterms[j].p))
		  {
			newElVect.push_back(glue(minterms[i], minterms[j], n));
			is_completed++;
		  }
		}
	  }
	}

	if (is_completed == 0 || minterms[i].completed)
	{
	  mark_elem(minterms[i]);
	  newElVect.push_back(minterms[i]);
	}
  }

  for (it = minterms.begin(); it != minterms.end(); it++)
	minterms.pop_back();
  for (uint i = 0; i < newElVect.size(); i++)
  {
	minterms.push_back(newElVect[i]);
	newElVect.erase(newElVect.begin());
  }
} // end of glue_all func

bool **rdnf_cl::build_quine_matr()
{
  bool run = false;
  uint nStr = minterms.size();
  uint nVect = allIndex.size();
  bool **qMatr = new bool*[nStr];

  for (uint i = 0; i < nStr; i++)
	qMatr[i] = new bool[nVect];

  do
  {
	run = false;
	glue_all();
	for (auto it : minterms)
	{
	  if (!it.completed)
	  {
		run = true;
		break;
	  }
	}  
  } while (run);

  for (uint i = 0; i < nStr; i++)
	for (uint j = 0; j < nVect; j++)
	{
	  bool val = false;

	  for (uint k = 0; k < minterms[i].m.size(); i++)
	  {
		if (minterms[i].m[k] > allIndex[j])
		  break;
		if (minterms[i].m[k] == allIndex[j])
		{
		  val = true;
		  break;
		}
	  }
	  qMatr[i][j] = val;
	}
  return qMatr;
} // end of build_quine func

uint **rdnf_cl::create_table(bool *TVT)
{
  bool **qMatr = build_quine_matr();
  uint **RDNF;
  uint nStr = minterms.size();
  uint nVect = allIndex.size();
  vector<elem_t> ans_vect;
  vector<uint> non_ker;
  vector<uint>::iterator it;
  uint size = 1 << numOfVar;

  // 1) find ker minterms
  for (uint i = 0; i < nVect; i++)
  {
	bool is_ker = false;
	uint cnt = 0;
	uint nOfMinterm = 0;

	for (uint j = 0; j < nStr; j++)
	{
	  if (qMatr[j][i] == true)
	  {
		cnt++;
		nOfMinterm = j;
	  }
	}
	if (cnt == 1 && minterms[nOfMinterm].completed)
	{
	  ans_vect.push_back(minterms[nOfMinterm]);
	  minterms[nOfMinterm].completed = false;  // already in ans_vect
	}
  }

  // 2) find not in ker vectors
  for (uint i = 0; i < nVect; i++)
  {
	bool found = false;

	for (uint j = 0; j < ans_vect.size(); j++)
	{
	  it = find(ans_vect[j].m.begin(), ans_vect[j].m.end(), allIndex[i]);
	  
	  if (it != ans_vect[j].m.end())
	  {
		found = true;
		break;
	  }
	}
	if (!found)
	{
	  non_ker.push_back(allIndex[i]);
	}
  }

  // 3) choose minterms from non ker vectors
  // foreach vector from non ker
  for (it = non_ker.begin(); it != non_ker.end(); it++)
  {
	// for by elements from vector from non ker
	for (uint j = 0; j < nStr; j++)
	{
	  if (qMatr[j][*it]) // == 1
	  {
		vector<elem_t>::iterator elIt;

		// if not already in ans_vect, put minterm there 
		// and go to next non from ker vector
		elIt = find(ans_vect.begin(), ans_vect.end(), minterms[j]);
		if (elIt != ans_vect.end()) // found
		  ;
		else 
		{
		  ans_vect.push_back(minterms[j]);
		  break;
		}
	  }
	}
  }

  // 4) generate RDNF matr:
  // first string - including mask (p.mask)
  // second string - invertion bits mask
  RDNF = new uint*[2];
  for (uint i = 0; i < 2; i++)
	RDNF[i] = new uint[size];

  for (uint i = 0; i < size; i++)
  {
	vector<elem_t>::iterator it1;
	uint inv_mask = 0;

	RDNF[0][i] = 0x0;
	RDNF[1][i] = 0x0;

	for (it1 = minterms.begin(); it1 != minterms.end(); it++)
	{
	  RDNF[0][it1->p.x] = it1->p.mask ^ 0xFFFFFFFF >> (1 - numOfVar);
	  
	  // generate mask
	  for (uint mask = 1 << numOfVar; mask > 0; mask >>= 1)
		if ((mask & it1->p.x) != 0)
		  inv_mask ^= mask;
	
	  RDNF[1][it1->p.x] = inv_mask;
	}
  }

  // free memory
  for (uint i = 0; i < minterms.size(); i++)
	delete[] qMatr[i];
  delete[] qMatr;

  return RDNF;
} // end of create_table func