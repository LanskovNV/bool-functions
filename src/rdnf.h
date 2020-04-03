#pragma once
#ifndef RDNF_H_
#define RDNF_H_
// leins, 06.06.2018 

#include <vector>

typedef unsigned int uint;

struct pair_t
{
  uint x;    // minterm value

  uint mask; // used to check if var in minterm is important or not
                     // 0 - important, 1 - not important 
};

struct elem_t
{
  pair_t p;            // minterm
  std::vector<uint> m; // index of included variables
  bool completed;      // if all is 1 -> end of gluing minterms
};

// class to generate 
// rdnf table
class rdnf_cl
{
private:
  uint numOfVar;
  std::vector<elem_t> minterms; // indexes of glued minterms &
                                // minterm binary code &
                                // mask ( look at pair_t comment )

  std::vector<uint> allIndex; // all used indexes

  // source functions
  uint **build_quine_matr(); // right --> allIndex
                             // down  --> minterms
  uint is_gluing(pair_t p1, pair_t p2);          // returns bit witch we should change in mask
  void change_mask(uint n, pair_t &p);        // select new not important bit in x
  bool is_skip(uint mask1, uint mask2, uint n);
  elem_t glue(elem_t e1, elem_t e2, uint n);
  void mark_elem(elem_t &e);
  void glue_all();

public:
  rdnf_cl(bool *TVT, uint size);
  // returns RDNF table
  uint **create_table(bool *tvt);
};

#endif
