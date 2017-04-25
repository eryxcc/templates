#define BUFSIZE 1000000

#include "../inc/topcoder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

set<string> tokens;

vector<string> src;

bool find(string token) {
  return tokens.count(token);
  }

vector<string> tokenize(string s) {
  s = s + " ";
  string buf = "";
  vs res;
  for(int k=5; k<Size(s); k++)
    if(s[k] == ' ') {res.push_back(buf); buf = "";}
    else buf += s[k];
  return res;
  }

string tokres;

bool truecond(string s) {
  bool cur = false;
  vector<string> v = tokenize(s);
  tokres = v[0];
  FOR(k,0,Size(v)) {
    if(v[k] == "&") {
      if(!cur) return false;
      cur = false;
      }
    else if(v[k] == "!") {
      if(cur) return false;
      }
    else cur = cur || find(v[k]);
    }
  return cur;
  }

int main(int argc, char **argv) {

  if(argc < 3) {
    fprintf(stderr, "%s: not enough arguments\n", argv[0]);
    exit(1);
    }

  // find all tokens in the input file
  FILE *f = fopen(argv[1], "rt");
       
  while(!feof(f)) {
    char buf[60000];
    fgets(buf, 60000, f);
    
    if(feof(f)) break;
    
    FOR(k,0,60000) if(buf[k] == '\r' || buf[k] == '\n') buf[k] = 0;
      else if(buf[k] == 0) break;
    
    string zbuf = buf; zbuf += " ";
    string tmp = "";
    FOR(k,0,Size(zbuf)) {
      if(isalnum(zbuf[k])) tmp += zbuf[k];
      else {if(tmp != "") tokens.insert(tmp); tmp = "";}
      }

    src.push_back(buf);
    }
  fclose(f);
  
  // write the output file...
  FILE *g = fopen(argv[2], "wt");
  
  // include the input...
  FOR(k,0,Size(src)) {
    if(src[k] != "#include <topcoder.h>") {
      fprintf(g, "%s\n", src[k].c_str());
      fflush(g);
      }
    else {
      // include the header...
      FILE *h = fopen("topcoder.h", "rt");
      bool empty = true;
      bool inc = true;
      while(!feof(h)) {
        char buf[60000];
        fgets(buf, 60000, h);
        
        FOR(k,0,60000) if(buf[k] == '\r' || buf[k] == '\n') buf[k] = 0;
          else if(buf[k] == 0) break;
        
        string hl = buf;
        
        if(hl[0] == '/' && hl[1] == '/' && hl[2] == '@') {
          if(hl[3] == '@') inc = true;
          else if(hl[3] == '?') {
            inc = truecond(hl);
            }
          else if(hl[3] == '+') {
            if(truecond(hl))
              tokens.insert(tokres);
            }
          }
    
        else if(inc) {
          if(empty && hl[0] == 0) continue; // eliminate double empty lines      
    //    fprintf(g, "[%d]", empty);
          empty = hl[0] == 0;
    //    fprintf(g, "[%d]", empty);
          fprintf(g, "%s\n", hl.c_str());
          fflush(g);
          }
        }
      fclose(h);
      
      }
    }
      
  // done...
  fclose(g);
  }
