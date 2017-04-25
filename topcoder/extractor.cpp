#include "topcoder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

  if(argc < 2) {
    fprintf(stderr, "%s: not enough arguments\n", argv[0]);
    exit(1);
    }

  FILE *f = fopen(argv[1], "rt");
  
  char buf[60000];
  fgets(buf, 60000, f);
  if(strcmp(buf, "//EXTRACT\n")) return 0;
  printf("Working on %s\n", argv[1]);

  FILE *h = NULL;
  while(!feof(f)) {
    
    fgets(buf, 60000, f);
    if(strstr(buf, "//FILE ") == buf) {
      if(h) fclose(h);
      FOR(k,0,60000) if(buf[k] == '\r' || buf[k] == '\n') buf[k] = 0;
      printf("Extracting %s\n", buf+7);
      h = fopen(buf + 7, "wt");
      continue;
      }
    if(feof(f)) break;
    
    
    if(h) fprintf(h, "%s", buf);
    }
  
  fclose(f);
  
  // mark the extractfile as used...
  f = fopen(argv[1], "wt");
  fprintf(f, "%s\n", "(Not compiled yet.)\n");
  fclose(f);
  
  if(h) fclose(h);
  }
