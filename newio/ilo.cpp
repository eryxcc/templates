/*

A runner for GCJ-style interactive problems.

Usage:

ilo [judge with parameters] -- [solution with parameters]

It runs two processes (P0 and P1) and sends the output of P0 to P1, and the output of P1 to P0.
It also displays all messages received from P0 or P1 (and status messages) on stdout, to help
with debugging.

*/

#include <vector>
#include <string> 
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

using namespace std;

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

vector<string> proc[2];

int pipes[6][2], fullout;

string buff[6];

void ansicol16(int col, FILE *f = stdout) {
  int coltab[16] = {
    30, 34, 32, 36, 31, 35, 33, 37,
    30, 34, 32, 36, 31, 35, 33, 37
    };
  fprintf(f, "\033[%d;%dm", col >= 8 ? 1 : 2, coltab[col]);
  }  

void ansiclear(FILE *f = stdout) {
  fprintf(f, "\033[0m");
  }  

int main(int argc, char **argv) {
  int id = 0;
  for(int i=1; i<argc; i++) {
    string s = argv[i];   
    if(s == "--") { id++; if(id == 2) { printf("bad\n"); exit(1); } }
    else proc[id].push_back(s);
    }
  for(int a=0; a<6; a++) pipe(pipes[a]);
  fullout = dup(1);
  int pids[2];
  for(int a=0; a<2; a++) {
    int f = fork();
    if(!f) {
      close(0);
      dup(pipes[a][0]);
      close(1);
      dup(pipes[a^2][1]);
      close(2);
      dup(pipes[a^4][1]);
      vector<char*> args;
      for(string& s: proc[a]) args.push_back(strdup(s.c_str()));
      
      for(int a=0; a<6; a++) close(pipes[a][1]);

      execv(args[0], &(args[0]));
      return 0;
      }
    else {
      pids[a] = f;
      ansicol16(15, stdout);
      printf("%s [%dS] started (pid %d of %d)\n", timestamp(), a, f, getpid());
      fflush(stdout);
      }
    }
  for(int a=2; a<6; a++) close(pipes[a][1]);
  int children_left = 2;
  for(int a=2; a<6; a++)
    fcntl(pipes[a][0], F_SETFL, O_NONBLOCK);
  while(children_left) {
    char buf[1000];
    int wstatus;
    int w = waitpid(-1, &wstatus, WNOHANG);
    if(w != 0) {
      children_left--;
      ansicol16(15, stdout);
      for(int a=0; a<2; a++) if(pids[a] == w) {
        printf("%s [%dS] terminated\n", timestamp(), a);
        fflush(stdout);
        close(pipes[a^1][1]);
        }
      }
    for(int a=2; a<6; a++) {
      int r = read(pipes[a][0], buf, 1000);
      if(r > 0) {
        for(int b=0; b<r; b++) {
          if(buf[b] == 10) {
            ansicol16(9 + ((a&1) ? 1 : 0) + ((a&4) ? 4 : 0), stdout);
            printf("%s [%d%c] %s\n", timestamp(), a&1, a&4 ? 'E' : ' ', buff[a].c_str()), buff[a] = "";
            fflush(stdout);
            }
          else buff[a] += buf[b];
          }
        if(a < 4)
          write(pipes[a^3][1], buf, r);
        }
      }
    usleep(10);
    }
  ansiclear();
  return 0;
  }
