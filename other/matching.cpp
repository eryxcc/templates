#include <algorithm>
#include <string>
#include <vector>
#include <ctype.h>
#include <math.h>
using namespace std;

#define LS <
#define Size(x) (int(x.size()))
#define LET(k,val) typeof(val) k = (val)
#define CLC(act,val) (*({act; static typeof(val) CLCR; CLCR = (val); &CLCR;}))

#define FOR(k,a,b) for(typeof(a) k=(a); k LS (b); ++k)

// for each boy find a girl he likes...

#define MAXBOYS 200
#define MAXGIRLS 200

int boys, girls;

bool likes[MAXBOYS][MAXGIRLS];

int girlFor[MAXBOYS];
int boyFor[MAXGIRLS];

int queue[MAXBOYS], qb, qe;

bool findMatching() {
  int comefrom[MAXGIRLS];
  
  FOR(b,0,boys) girlFor[b] = -1;
  FOR(g,0,girls) boyFor[g] = -1;
  
  FOR(b,0,boys) {
    qb = 0; qe = 1; queue[0] = b;
    
    FOR(g,0,girls) comefrom[g] = -1;

    while(qb < qe) {

      int cb = queue[qb++];

      FOR(g,0,girls) if(likes[cb][g]) if(comefrom[g] == -1) {
        comefrom[g] = cb;
//      printf("Try G%d for B%d\n", g, cb);
        if(boyFor[g] == -1) {
          int cg = g;
          while(cg != -1) {
            cb = comefrom[cg];
            swap(girlFor[cb], cg);
            boyFor[girlFor[cb]] = cb;
//          printf("%d -> %d\n", cb, girlFor[cb]);
            }
          goto nextBoy;
          }
        else {
//        printf("- go to B%d\n", boyFor[g]);
          queue[qe++] = boyFor[g];
          }
        }
      }
    
    return false;
    nextBoy: ;
    }
  
  return true;
  }

int main() {
  boys = 5;
  girls = 5;
  likes[0][0] = true;
  likes[1][1] = true;
  likes[2][2] = true;
  likes[3][3] = true;
  likes[3][4] = true;
  likes[2][3] = true;
  likes[1][2] = true;
  likes[0][1] = true;
  likes[4][0] = true;
  
  if(findMatching()) {
    printf("OK!\n");
    FOR(b,0,boys) printf("%d -> %d\n", b, girlFor[b]);
    }
  }
