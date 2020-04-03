#pragma once
#ifndef BOOL_FUNC_H
#define BOOL_FUNC_H
// leins, 02.06.2018

#include <fstream> // for i/o operators definitions

// new type names
typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned long int ulint;

// macroses
#define MAX_2_POWER_DEG 32 // максимальная поддерживаемая степень двойки 
#define MASK 0xFFFFFFFF

enum func_type_t
{
  t_TVT,
  t_PDNF,
  t_PCNF,
  t_ZheP,
  t_Karn,
  t_RDNF
};

// source functions

class bool_func_cl
{
private:
  byte numOfVar; // num of variables in tvt
  size_t m; // size of TVT, PDNF, PCNF, RDNF
  size_t mKarn, LKarn; // sizes of KARN table

  // all representation types:
  uint *TVT;    // 0 or 1
  uint *ZheP;   // 0 or 1
  uint **Karn;  // 0 or 1
  uint **RDNF;  // uint values

  // source funcs
  void init(uint n); // allocate memory, used in constructors only
  uint grey(uint ind); // grey from index
  uint grey_to_ind(uint n, uint x); // Index of Grey Code
  // void put_point_val(uint x, bool val); // set one variable in tvt
  uint invertion_by_type(uint x, func_type_t type);
  uint consolidate(uint x, func_type_t type);
  uint **quine_mcCluskey();

  // convertors
  void from_TVT(func_type_t type);
  void to_TVT(func_type_t type);

public:
  // constructors
  bool_func_cl (byte n); // without init value
  bool_func_cl (byte n, uint *vect, func_type_t type = t_TVT);   // vector
  bool_func_cl (byte n, uint **vect, func_type_t type = t_Karn); // table

  // destructor
  ~bool_func_cl();

  // i/o operators
  void operator<<(std::fstream &stream);
  void operator>>(std::fstream &stream);

  // get values
  bool get_val(uint x, func_type_t type = t_TVT);

  // check
  bool is_correct(); // compare all types values
  bool check_save_zero();
  bool check_save_one();
  bool check_self_duality();
  bool check_mono();
  bool check_line();
  bool check_symmetric();
};

#endif // BOOL_FUNC_H
