#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

vector<int> pipes;

int proctype = 0;

int cstatus;

int children;

int maxchildren = 4;

FILE *saveout;

bool lastpipe;

int threaderr;

int checkcase() {

  while(children && waitpid(-1, &cstatus, WNOHANG) > 0)
    children--;
  
  if(proctype == 0) {
    proctype = 2;
    saveout = stdout;
    }
  if(proctype == 2) {
    fprintf(stderr, "[case %3d/%3d children %4d]\n", cnum, Tests, children);
    int pip[2];
    threaderr = pipe(pip);
    if(threaderr < 0) {
      fprintf(stderr, "[pipe error %s]\n", strerror(errno));
      if(lastpipe) fclose(stdout); lastpipe = false; stdout = saveout;
      return true;
      }
        
    int fval = children >= maxchildren ? -1 : fork();
    
    if(fval == -1) {
      // handle it by self
      if(lastpipe) fclose(stdout);
      lastpipe = true;
      stdout = fdopen(pip[1], "w");
      pipes.push_back(pip[0]);
      return true;
      }
    
    else if(fval == 0) {
      // child
      fclose(stdout);
      close(pip[0]); 
      stdout = fdopen(pip[1], "w");
      Tests = -1; proctype = 0;
      return true;
      }
    
    else {
      // master
      pipes.push_back(pip[0]); close(pip[1]); children++;
      return false;
      }
    }
  return false;
  }

void finish() {
  if(proctype == 2) {
    if(lastpipe) fclose(stdout);
    stdout = saveout;

    while(children) {
      fprintf(stderr, "[wait x%d]\n", children);
      waitpid(-1, &cstatus, 0);
      children--;
      }
  
    for(int i=0; i<Size(pipes); i++) {
      // fprintf(stderr, "[pipe #%d: %d]\n", i, pipes[i]);

      FILE *f = fdopen(pipes[i], "r");
      if(!f) fprintf(stderr, "[cannot fdopen]\n");
      else {
        char buf[50000];
        char* res = fgets(buf, 50000, f);
        if(!res) fprintf(stderr, "[error in gets]\n");
        fclose(f);
        fputs(buf, stdout);
        }
      }
    }
  }
