#include <vector>
#include <map>
#include <string>
#include <stdio.h>
#include <algorithm>

using namespace std;

typedef vector<string> row;
typedef vector<row> db;
typedef map<string, db> idb;

char sep = ',';

db readcsv(string fname = "", char xsep = sep) {
  db res;
  row cur;
  string s;
  FILE *f = fname == "" ? stdin : fopen(fname.c_str(), "rt");
  bool state = false;
  while(!feof(f)) {
    char c = fgetc(f);
    if(c == -1) break;
    if(c == '"') state = !state; // , s += c;
    else if(c == xsep && !state) cur.push_back(s), s = "";
    else if(c == 10) cur.push_back(move(s)), s="", res.push_back(move(cur)), cur.clear();
    else s += c;
    }
  if(fname != "") fclose(f);
  if(cur.size()) cur.push_back(move(s)), res.push_back(move(cur));
  return res;
  }

template<class...T> void out1(const row& r) {
  bool first = true;
  for(auto s:r) {
    if(first) first = false; else printf("%c", sep);
    printf("%s", s.c_str());
    }
  }

template<class...T> void out1(const string& s) {
  printf("%s", s.c_str());
  }

void outmore() {}

template<class T, class... S> void outmore(T t, S...s) {
 putc(sep, stdout);
 out1(t); outmore(s...);
 }

template<class T, class... S> void out(T t, S...s) {
 // putc(',');
 out1(t); outmore(s...); putc('\n', stdout);
 }

void out(const db& z) { for(row r:z) out(r); }

template<class...T> void c1(row& out, const row& r) {
  for(auto s:r) {
    out.push_back(s);
    }
  }

template<class...T> void c1(row& out, const string& s) {
  out.push_back(s);
  }

void cmore(row& out) {}

template<class T, class... S> void cmore(row& out, T t, S...s) {
  c1(out,t); cmore(out,s...);
  }

template<class T, class... S> row c(T t, S...s) {
 // putc(',');
 row r; c1(r,t); cmore(r,s...); return r;
 }

void store(db& z, const row& r) {
  z.push_back(r);
  }

template<class T, class MI, class... U> void store(T& t, const row& r, MI id, U... at) {
  store(t[id], r, at...);
  }

string applyindexer(const row& r, int i) {
  return r[i];
  }

template<class I> auto applyindexer(const row& r, I i) -> decltype(i(r)) 
  { return i(r); }

template<unsigned...s> struct seq { typedef seq<s...> type; };
template<unsigned max, unsigned... s> struct make_seq:make_seq<max-1, max-1, s...> {};
template<unsigned...s> struct make_seq<0, s...>:seq<s...> {};

template<class T, unsigned... s> auto applyindexer_seq(const row& r, T t, seq<s...>) -> 
  decltype(make_tuple(applyindexer(r, get<s>(t))...)) 
  { return make_tuple(applyindexer(r, get<s>(t))...); }

template<class... T> auto applyindexer(const row& r, tuple<T...> t) -> 
  decltype(applyindexer_seq(r, t, make_seq<sizeof...(T)>()))
  { return applyindexer_seq(r, t, make_seq<sizeof...(T)>()); }

template<class T, class I, class...U> void storeall(T& t, const row& r, I i, U...u) {
  store(t, r, applyindexer(r,i), u...);
  }

template<class T, class I, class...U> void storeall(T& t, const db& z, I i, U...u) {
  for(auto r:z) storeall(t,r,i,u...);
  }

template<class T, class I, class MI, class X, class... U> 
void storeall(T& t, const map<MI,X>& x, I i, U... u) {
  for(auto xx:x) storeall(t, xx.second, i, u..., xx.first);
  }

extern row r0;

template<class T, class I> 
auto index(T z, I i) -> map<decltype(applyindexer(r0,i)),T> {
  map<decltype(applyindexer(r0,i)),T> res;
  storeall(res, z, i);
  return res;
  }

template<class I> 
auto singleindex(const db& z, I i) -> map<decltype(applyindexer(r0,i)),row> {
  map<decltype(applyindexer(r0,i)),row> res;
  for(row r:z) res[applyindexer(r,i)] = r;
  return res;
  }

void flattento(db& out, row r) { out.push_back(r); }

template<class T> void flattento(db& out, const vector<T>& t) {
  for(auto tt:t) flattento(out, tt);
  }

template<class I, class T> void flattento(db& out, const map<I, T>& t) {
  for(auto tt:t) flattento(out, tt.second);
  }
  
template<class T> db flatten(const T& t) {
  db z; flattento(z,t); return z;
  }

template<class I> db sorti(const db& z, I i) {
  return flatten(index(z,i));
  }

int atoi(string s) { return atoi(s.c_str()); }
string itoa(int i) { char buf[20]; sprintf(buf, "%d", i); return buf; }

template<class I1, class I2> db join(const db& z1, I1 i1, const db& z2, I2 i2) {
  auto zi2 = index(z2, i2);
  db result;
  for(auto r1: z1) for(auto r2: zi2[applyindexer(r1, i1)])
    result.push_back(c(r1, r2));
  return result;
  }

template<class I1, class I2> db jselect(const db& z1, I1 i1, const db& z2, I2 i2) {
  auto zi2 = index(z2, i2);
  db result;
  for(auto r1: z1) if(zi2[applyindexer(r1, i1)].size() > 0)
    result.push_back(r1);
  return result;
  }

void xformrow(row& r2, const row& r1) { }

template<class I, class... J> void xformrow(row& r2, const row& r1, I i, J... j) {
  r2.push_back(applyindexer(r1, i));
  xformrow(r2, r1, j...);
  }

template<class... I> db xform(const db& z1, I... i) {
  db res;
  for(auto r1: z1) {
    row r2;
    xformrow(r2, r1, i...);
    res.push_back(r2);
    }
  return res;
  }

db uniq(const db& z) {
  db res;
  for(int i=0; i<int(z.size()); i++)
    if(i == 0 || z[i] != z[i-1])
      res.push_back(z[i]);
  return res;
  }

#define INDEXER(r,x,c) [c](const row& r) { return x; }

int ival;

#define INT(x) (ival=(x), [ival](const row& r) { return atoi(r[ival]); })

#define BUILD(b,x) [&]() { db b; x; return b; }

#define F(x) for(auto x)
#define P(d,x) d.push_back(x);
#define A auto

#define BUILD0(x) BUILD(b0,x)
#define P0(x) P(b0,x)

#define MT make_tuple

#define R readcsv
