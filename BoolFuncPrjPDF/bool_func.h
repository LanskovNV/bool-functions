#pragma once
#ifndef BOOL_FUNC_H
#define BOOL_FUNC_H
// leins, 02.06.2018

#include <iostream>

// new type names
typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned long int ulint;

enum RepT
{
  tTVT,
  tPDNF,
  tPCNF,
  tZheP,
  tKarn,
  tRDNF,
  tDF,
  tCF
};

// source functions
uint Grey(uint Ind);
uint Grey2Int(uint G);

// exception messages
#define  IllegalRepTinBF "Illegal RepT t in BF(..., RepT t)"
#define NoutOfRange "32 <= n"
#define XoutOfRange "1U << n <= X"
#define UnKnownRepT "UnKnown RepT t"
#define BFnewArrayFail "BF(byte N, uint *p, RepT t = tTVT, size_t M = 0): new ArrayFail"
#define BFnewMatrixFail "BF(byte N, uint **p, RepT t = tTVT, size_t M = 0): newMatrix Fail"

/*********************
 * bool function class
*********************/
class BF
{
private:
  byte n; // number of boolean variables < 32
  size_t mTVT, mPDNF, mPCNF, mZheP, mKarn, LKarn, mRDNF, mDF, mCF;
  uint *TVT, *PDNF, *PCNF, *ZheP, **Karn, **RDNF, **DF, **CF;

  void writeTVT(uint X, bool TVX); // TVT(X) <- TVX

public:
  BF(); // const BF == false
  BF(bool TV); // const BF == TrueValue

  // PDNF, PCNF, ZheP -> TVT -> PDNF, PCNF, ZheP, Karn, RDNF
  BF(byte N, uint *p, RepT t = tTVT, size_t M = 0);

  // Karn, DF, CF -> TVT -> PDNF, PCNF, ZheP, Karn, RDNF
  BF(byte N, uint **p, RepT t = tKarn, size_t M = 0);

  ~BF();

  // get values
  bool Val(uint X, RepT t = tTVT);

  // input/output operators


  // check properties (x6)

  // converters
};

#endif // BOOL_FUNC_H
