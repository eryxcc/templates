#ifndef _TOPCODER_H_
#define _TOPCODER_H_

// Eryx's new template for I/O contests, May 3, 2015
//@@

#include <algorithm>
//@? string vs
#include <string>
//@? vector vi vs vll
#include <vector>
//@? isalpha isalnum iscntrl isdigit isgraph islower isupper tolower toupper isspace
#include <ctype.h>
//@? atan cos sin tan tanh frexp modf ceil fabs floor acos asin atan2 cosh sinh exp ldexp log log10 pow sqrt fmod M_E M_PI M_SQRT2 M_SQRT3 hypot
#include <math.h>
//@? cin cout cerr
#include <iostream>
//@? set
#include <set>
//@? map MKey MRet
#include <map>
//@? cll cld
#include <complex>
//@? istringstream ostringstream
#include <sstream>
//@? printf sprintf sscanf
#include <stdio.h>
//@? rand
#include <stdlib.h>
//@? strcmp
#include <string.h>
//@@
using namespace std;
//@@

//@? ll bitc gcd cll fll parsevll getLL modar modarP
typedef long long ll;
//@? ld cld
typedef long double ld;
//@? vi parsevi getVI
typedef vector<int> vi;
//@? vll
typedef vector<ll> vll;
//@? vs parsevs addguards
typedef vector<string> vs;
//@? cll
typedef complex<ll> cll;
//@? cld
typedef complex<ld> cld;
//@@

//@+ FOR parsevi parsevs matrix addguards
//@+ Size parsevi parsevs matrix addguards
//@+ FIRST FIRSTMIN 
//@+ MIN FIRSTMIN
//@+ 0gccops FIRSTMIN
//@+ EXISTS FORALL
//@+ FIRST EXISTS FORALL
//@+ FOLD0 SUMTO PRODTO MAXTO MINTO SUM PROD QAND QOR QXOR
//@+ FOLD1 MAX MIN
//@+ 0rangecomm FOR FORR FIRST LAST FOLD0 FOLD1
//@+ LS FOR FIRST FOLD0 FOLD1
//@+ CLC FIRST LAST FOLD0 FOLD1 BINFIRST
//@+ LET FIRST LAST FOLD0 FOLD1 BINFIRST

//@? 0workset & LS
#define LS !=
//@? 0ignoreme
#undef LS
//@? 0workset ! LS
#define LS <
//@@
#define Size(x) (int(x.size()))
//@? LET
#define LET(k,val) __typeof(val) k = (val)
//@? CLC
// execute "act", and return "val" as an expression result
#define CLC(act,val) ([&] () {act; return (val); } ())
//@@

//@? 0rangecomm
// All macros with parameters "k,a,b" run the "k" variable in range [a,b)
//@? FOR
#define FOR(k,a,b) for(__typeof(a) k=(a); k LS (b); ++k)
//@? FORREV
// the range is traversed from b-1 to a in FORREV
#define FORREV(k,a,b) for(__typeof(b) k=(b); (a) <= (--k);)
//@@

//@? EXISTS FORALL SUM PROD QXOR QAND QOR MAX MIN MAXTO MINTO SUMTO PRODTO SETOF
// Standard mathematical quantifiers, plus tools to implement them
//@? FIRST
// find the first k in [a,b) that satisfies cond, or b if none
#define FIRST(k,a,b,cond) CLC(LET(k, a); for(; k LS (b); ++k) if(cond) break, k)
//@? LAST
// find the last k in [a,b) that satisfies cond, or a-1 if none
#define LAST(k,a,b,cond) CLC(LET(k, b); while((a) <= (--k)) if(cond) break, k)
//@? EXISTS
#define EXISTS(k,a,b,cond) (FIRST(k,a,b,cond) LS (b))
//@? FORALL
#define FORALL(k,a,b,cond) (!EXISTS(k,a,b,!(cond)))
//@? FOLD0
#define FOLD0(k,a,b,init,act) CLC(LET(k, a); LET(R##k, init); for(; k LS (b); ++k) {act;}, R##k)
//@? SUMTO SUM
#define SUMTO(k,a,b,init,x)  FOLD0(k,a,b,init,R##k += (x))
//@? SUM
#define SUM(k,a,b,x) SUMTO(k,a,b,(__typeof(x)) (0), x)
//@? PRODTO PROD
#define PRODTO(k,a,b,init,x) FOLD0(k,a,b,init,R##k *= (x))
//@? PROD
#define PROD(k,a,b,x) PRODTO(k,a,b,(__typeof(x)) (1), x)
//@? QXOR
#define QXOR(k,a,b,x) FOLD0(k,a,b,(__typeof(x)) (0), R##k ^= x)
//@? QAND
#define QAND(k,a,b,x) FOLD0(k,a,b,(__typeof(x)) (-1), R##k &= x)
//@? QQOR
#define QOR(k,a,b,x) FOLD0(k,a,b,(__typeof(x)) (-1), R##k |= x)
//@? FOLD1
// note: the range has to be non-empty here
#define FOLD1(k,a,b,init,act) CLC(LET(k, a); LET(R##k, init); for(++k; k LS (b); ++k) act, R##k)
//@? MAX
// #define MAX(k,a,b,x) FOLD1(k,a,b,x, R##k >?= (x))
//@? MIN
// #define MIN(k,a,b,x) FOLD1(k,a,b,x, R##k <?= (x))
//@? FIRSTMIN
// return the first k for which x achieves the minimum
// #define FIRSTMIN(k,a,b,x) (MIN(k,a,b,make_pair(x,k)).second)
//@@

//@? INF tcize
#define INF 1000000000
//@? tcize
// produce TC-style answer
int tcize(int n) {return n<INF ? n : -1;}
//@@

//@? parsevi getVI
// parse a space-delimited string into a vi
vi parsevi(string s) {
  s = s + " ";
  int q = 0;
  vi res;
  FOR(l,0, Size(s)) {
    if(s[l] == ' ') { res.push_back(q); q = 0;}
    else { q = q * 10 + s[l] - '0'; }
    }
  return res;
  }
//@@

//@? parsevs
// parse a space-delimited string into a vs
vs parsevs(string s) {
  s = s + " ";
  string q = "";
  vs res;
  FOR(l,0, Size(s)) {
    if(s[l] == ' ') { res.push_back(q); q = "";}
    else { q += s[l]; }
    }
  return res;
  }
//@@

//@? MKey MRet
// calculate key, check if memoized
#define MKey(x) \
  typedef __typeof(memo) tmemo; tmemo::key_type key = (x); \
  if(memo.find(key) != memo.end()) return memo[key]

// return and memoize
#define MRet(y) return (memo[key] = y)
//@@

//@? cll cld & inner
// inner product
template <class T> T operator | (complex<T> x, complex<T> y) {
  return (x*conj(y)).real();
  }
//@@

//@? cll cld & cross
// cross product
template <class T> T operator ^ (complex<T> x, complex<T> y) {
  return (x*conj(y)).imag();
  }
//@@

//@? bitc
// Bit Count
int bitc(ll r) {return r == 0 ? 0 : (bitc(r>>1) + (r&1));}
//@@

//@? gcd reduceGCD fll
// Greatest Common Divisor
ll gcd(ll x, ll y) {return x ? gcd(y%x,x) : y;}
//@@

//@? 0gccops 0gccopsx
// Operators missing from GCC...
//@? 0gccops
// template<class T> T& operator >?= (T& x, T y) {if(y>x) x=y; return x;}
// template<class T> T& operator <?= (T& x, T y) {if(y<x) x=y; return x;}
//@? 0gccopsx
// template<class T> T operator >? (T x, T y) {return x>y?x:y;}
// template<class T> T operator <? (T x, T y) {return x<y?x:y;}
//@@

//@? Pa Ir
#define Pa(xy) ((xy).first)
#define Ir(xy) ((xy).second)
//@@

//@? cts
// char to string
string cts(char c) {string s=""; s+=c; return s;}
//@@

//@? its
// int to string
string its(int i) {char buf[200]; sprintf(buf, "%d", i); return buf;}
//@@

//@? 0stream
template<class T> ostream& operator<<(ostream& os, const vector<T>& v) {
  os << "{";
  for(LET(k,v.begin()); k != v.end(); ++k) {
    os << (*k); os << ",";
    }
  os << "}";
  return os;
  }
  
template<class T, class U> ostream& operator<<(ostream& os, const pair<T,U>& p) {
  return os << "(" << p.first << "," << p.second << ")";
  }

//@? ostream & set
template<class T> ostream& operator<<(ostream& os, const set<T>& v) {
  os << "{";
  for(LET(k,v.begin()); k != v.end(); ++k) {
    os << (*k); os << ",";
    }
  os << "}";
  return os;
  }
//@@

//@? trashtrashtrash
#ifdef floyd
FOR(k,0,100) FOR(i,0,100) FOR(j,0,100) w0[i][j] <?= w0[i][k] + w0[k][j];
#endif

// Notes: - add 0options as default

//@@

//@? addguards
// add guards to a vs
vs addguards(vs data, char guard) {
  vs res;
  string border;
  FOR(k,0,Size(data[0])+2) border += guard;
  res.push_back(border);
  FOR(k,0,Size(data)) res.push_back(guard + data[k] + guard);
  res.push_back(border);
  return res;
  }
//@@

//@? reduceGCD fll
template<class T> void reduceGCD(T& l, T& m) {
  T a = gcd(l,m); if(a<0) a=-a; if(m<0) a=-a; l/=a; m/=a; if(m==0&&l<0) l=-l;
  }

//@? fll
#define O(P,v) operator P (fll x, fll y) {return v;}
#define F(P,d) fll& operator P##= (fll&x, fll y) {d; return x;} fll O(P, x P##= y)
#define D(P) bool O(P, (x-y).l P 0)

struct fll {
  ll l; ll m;
  void red() {reduceGCD(l,m);}
  void unr(ll a) {l*=a; m*=a;}
  fll(ll x = 0, ll y = 1) {l=x; m=y; red();}
  };

F(+, ll a = gcd(x.m, y.m); ll b=x.m; x.unr(y.m/a); y.unr(b/a); x.l += y.l; x.red())
F(-, y.l = -y.l; x+=y)
F(*, swap(x.m, y.m); x.red(); y.red(); x.l *= y.l; x.m *= y.m; x.red())
F(/, swap(x.m, y.l); x.red(); y.red(); x.l *= y.m; x.m *= y.l; x.red())

D(<) D(<=) D(>) D(>=) D(==) D(!=)
#undef F
#undef O
#undef D

//@? ftd
ld ftd(fll x) {return ld(x.l)/x.m;}

//@? fllts
string llts(ll x) {char buf[30]; sprintf(buf, "%lld", x); return buf;}
string fllts(fll x) {return llts(x.l) + "/" + llts(x.m);}

//@? calcBinom

ll binom[100][100];

void calcBinom() {
  binom[0][0] = 1;
  FOR(n,1,100) {
    FOR(k,1,100) {
      binom[n][k] = binom[n-1][k-1] + binom[n-1][k];
      }
    binom[n][0] = 1;
    }
  }

//@? BINFIRST
// find the first k in [a,b) that satisfies cond, or b if none
// using binary search
#define BINFIRST(k,a,b,cond)          \
  CLC(                                \
    LET(Type, a+b);                   \
    LET(k##mIn, (__typeof(Type))(a)); LET(k##mAx, (__typeof(Type))(b));   \
    while(k##mIn != k##mAx) {         \
      LET(k, (k##mIn>>1)+(k##mAx>>1)+ \
      (((k##mIn&1)+(k##mAx&1))>>1)    \
      ); \
      if(cond) k##mAx = k;            \
      else k##mIn = k+1;              \
      },                              \
    k##mIn                            \
    )                                 
//@@

//@? DEB
#ifdef DEBUG
#define DEB(x) x
#else
#define DEB(x)
#endif
//@@

//@? getLine getNum getDouble getStr getLL
string getLine() {
  string s;
  while(!feof(stdin)) {
    char c = fgetc(stdin);
    if(c == 13) continue;
    if(c == 10) return s;
    s += c;
    }
  return s;
  }

//@? scanf scanerr getNum getStr getDouble getLL getVI
int scanerr;

//@? getNum
int getNum() {
#ifdef LINEBYLINE
  string s = getLine();
  return atoi(s.c_str());
#else
  int i;
  scanerr = scanf("%d", &i);
  return i;
#endif
  }

#ifndef BUFSIZE
#define BUFSIZE 1000000
#endif

char buf[BUFSIZE];

//@? getStr
string getStr() {
#ifdef LINEBYLINE
  return getStr();
#else
  scanerr = scanf("%s", buf);
  return buf;
#endif
  }

//@? getDouble
double getDouble() {
#ifdef LINEBYLINE
  string s = getLine();
  return atof(s.c_str());
#else
  scanerr = scanf("%s", buf);
  return atof(buf);
#endif
  }

//@? getLL
ll getLL() {
#ifdef LINEBYLINE
  string s = getLine();
  return atoll(s.c_str());
#else
#ifdef USEWIN
  string s = getStr();
  return atoll(s.c_str());
#else
  ll v;
  scanerr = scanf("%Ld", &v);
  return v;
#endif
#endif
  }

//@? getVI
vi getVI() {
#ifdef LINEBYLINE
  return parsevi(getLine());
#else
  int N = getNum();
  vi result;
  FOR(k,0,N) result.push_back(getNum());
  return result;
#endif
  }

//@? modar modarP
#define TDEF template<int P>
#define modarP modar<P>

TDEF struct modar {
  int content;
  
  modar(int _c) { content = _c % P; if(content<0) content += P; }
  modar(ll _c) { content = _c % P; if(content<0) content += P; }
  modar() {}
  int asInt() { return content; }
  };

TDEF modarP operator == (modarP x, modarP y) { return x.content == y.content; }
TDEF modarP operator != (modarP x, modarP y) { return x.content != y.content; }

#define F(P, d) \
  TDEF modarP& operator P##= (modarP& x, modarP y) { d; return x; } \
  TDEF modarP& operator P##= (modarP& x, int y) { return x P##= modarP(y); } \
  TDEF modarP& operator P##= (modarP& x, ll  y) { return x P##= modarP(y); } \
  TDEF modarP operator P (modarP x, modarP y) { return x P##= y; } \
  F0(P)

#define F0(P) \
  TDEF modarP operator P (modarP x, int y) { return x P modarP(y); } \
  TDEF modarP operator P (modarP x, ll y) { return x P modarP(y); } 

F(+, ll tmp = x.content + ll(y.content); x.content = tmp>P ? tmp-P : tmp;) 
F(-, ll tmp = x.content - ll(y.content); x.content = tmp<0 ? tmp+P : tmp;)
F(*, ll tmp = x.content * ll(y.content); x.content = tmp % P;) 
F(/, x *= power(y, P-2);)

F0(==) F0(!=)
#undef F
#undef F0

TDEF modarP power(modarP x, int q) {
  modarP one(1);
  while(true) {
    if(q&1) one *= x;
    q >>= 1;
    if(!q) return one;
    x *= x;
    }
  }

//@? matrix unitMatrix zeroMatrix

template<class T> struct matrix {
  int rows, cols;
  vector<T> data;
  matrix(int _r, int _c) : rows(_r), cols(_c), data(_r*_c) {}
  T* operator [] (int rowid) { return &data[cols * rowid]; }
  };

template<class T> matrix<T> operator += (matrix<T>& A, matrix<T> B) {
  FOR(i,0,Size(A.data)) A[i] += B[i]; return A;
  }

template<class T> matrix<T> operator -= (matrix<T>& A, matrix<T> B) {
  FOR(i,0,Size(A.data)) A[i] -= B[i]; return A;
  }

template<class T> matrix<T> operator + (matrix<T> A, matrix<T> B) { return A+=B; }
template<class T> matrix<T> operator - (matrix<T> A, matrix<T> B) { return A-=B; }

template<class T> matrix<T> operator * (matrix<T> A, matrix<T> B) { 
  matrix<T> C(A.rows, B.cols);
  for(int i=0; i<A.rows; i++) for(int k=0; k<B.cols; k++) {
    T Cval(0);
    for(int j=0; j<A.cols; j++) Cval += A[i][j] * B[j][k];
    C[i][k] = Cval;
    }
  return C;
  }

//@? unitMatrix
template<class T> matrix<T> unitMatrix(int size) { 
  matrix<T> result(size, size);
  for(int i=0; i<size; i++) for(int j=0; j<size; j++)
    result[i][j] = (i==j) ? 1 : 0;
  return result;
  }

//@? zeroMatrix
template<class T> matrix<T> zeroMatrix(int size) { 
  matrix<T> result(size, size);
  for(int i=0; i<size; i++) for(int j=0; j<size; j++)
    result[i][j] = 0;
  return result;
  }

//@? removeStackLimit

#include <sys/resource.h>

void removeStackLimit() {
    const rlim_t kStackSize = 1600 * 1048576LL;   // min stack size = 16 MB
    struct rlimit rl;
    int result;

    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0)
    {
        if (rl.rlim_cur < kStackSize)
        {
            rl.rlim_cur = kStackSize;
            result = setrlimit(RLIMIT_STACK, &rl);
            if (result != 0)
            {
                fprintf(stderr, "setrlimit returned result = %d\n", result);
            }
        }
    }
  }


//@? powerMatrix
template<class T> matrix<T> powerMatrix(matrix<T> x, int q) {
  matrix<T> one = unitMatrix<T>(x.rows);
  while(true) {
    if(q&1) one = one * x;
    q >>= 1;
    if(!q) return one;
    x = x * x;
    }
  }

//@? rope

/*
#include <ext/rope> //header with rope
using namespace __gnu_cxx; //namespace with rope and some additional stuff
*/

//@? getVa
#include <sys/time.h>
ll getVa() {
  struct timeval tval;
  gettimeofday(&tval, NULL);
  return tval.tv_sec * 1000000 + tval.tv_usec;
  }

//@? debuglog
FILE *debuglog = stderr;

//@@

//@? scanerr
char *scanerrp;

//@? timestamp
// "%F %T"
string stampformat = "%T";

#define MAX_TIMESTAMP 1000

char *timestamp() {
  static char tms[MAX_TIMESTAMP];
  time_t timep;
  tm tmv;
  time(&timep);
  localtime_r(&timep, &tmv);
  strftime(tms, MAX_TIMESTAMP, stampformat.c_str(), &tmv);
  return tms;
  }

string timestamp_str() { return timestamp(); }

//@? linesplit

void powerlogger(FILE *f, const string& header, string data, bool binarylog = false, int maxlog = 1024) {
  if(!f || Size(data) == 0) return;
  if(Size(data) > maxlog) {
    fprintf(f, "%s (%d bytes)\n", header.c_str(), Size(data));
    fflush(f);
    return;
    }
  
  if(binarylog) {
    string plain;
    int inl = 0;
    for(int i=0; i<Size(data); i++) {
      // fprintf(f, "[i%dl%d]", i, inl);
      if(inl == 0) 
        fprintf(f, "%s ", header.c_str());
      fprintf(f, "%02X ", (unsigned char) data[i]);
      if(data[i] >= 32) plain += data[i];
      else plain += ".";
      inl++;
      if(inl == 16) {
        fprintf(f, "%s\n", plain.c_str());
        inl = 0; plain = "";
        }
      }
    if(inl) {
      if(Size(data) > 16) while(inl<16) fprintf(f, "   "), inl++;
      fprintf(f, "%s\n", plain.c_str());
      }
    fflush(f);
    return;
    }
  
  int lasteol = 0;
  for(int i=1; i<Size(data); i++) if(data[i] == '\n') {
    data[i] = 0;
    fprintf(f, "%s %s\n", header.c_str(), data.c_str() + lasteol);
    data[i] = '\n';
    lasteol = i+1;
    }
  if(lasteol != Size(data)) {
    fprintf(f, "%s %s (+)\n", header.c_str(), data.c_str() + lasteol);
    }
  fflush(f);
  }

//@? probname
string probname = "default";

//@? datatostr
template<class T> string datatostr(const T& d) {
  return string((char*) &d, sizeof(d));
  }
  
template<class T> T strtodata(const string& s) {
  return *((T*) s.c_str());
  }

#endif
