#define BUFSIZE 1000000
int Tests, cnum;
// #define USEWIN
#define MANYTESTS 1
// #define LINEBYLINE

#include <topcoder.h>
#line 10 "work.cpp"

/// ----


//Eryx

// !FDI

void solveCase() {
  int res = 0;

  int N = getNum();
  
  printf("Case #%d: %d\n", cnum, res);
  }

#define P 1000000007

int main() {

  if(!MANYTESTS) Tests = 1;
  else Tests = getNum();
  
  for(cnum=1; cnum<=Tests; cnum++)
    solveCase();
    
  // finish
  return 0;
  }

// This solution includes hidden routines to solve test cases in separate
// processes in order to make it faster. I will update them to run on a
// cluster if I get one ;)
// See https://github.com/eryxcc/templates
