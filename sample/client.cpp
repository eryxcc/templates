#include <client.h>
#include <login.h>

tcpclient c;

struct savestate {
  char status[200];
  int question;
  } ss;

void tlo() { 
  c.flushout(); 
  if(debuglog) fflush(debuglog);
  }

int LIMIT = 20, cmdsend;

bool checkOK(const char* where) {
  cmdsend++;
  if(c.getline(tlo) != "OK") {
    printf("Nie udalo sie: %s! [%s]\n", where, c.getline_last);
    // exit(1);
    fprintf(debuglog, "Nie udalo sie: %s! [%s]\n", where, c.getline_last);
    if(strstr(c.getline_last, "commands limit")) {
      printf("CLIMIT=%d\n", cmdsend);
      }
    return false;
    }
  return true;
  }

void clearall() {
  }

void saveall() {
  char dbuf[64];
  sprintf(dbuf, "game-%s-%d.sav", probname.c_str(), c.serverid);
  FILE *f = fopen(dbuf, "w");
  fwrite(&ss, sizeof(ss), 1, f);
  fclose(f);
  
  sprintf(dbuf, "die-%s-%d.sav", probname.c_str(), c.serverid);
  f = fopen(dbuf, "r");
  if(f) {
    exit(0);
    }
  }

void loadall() {
  clearall();
  char dbuf[64];
  sprintf(dbuf, "game-%s-%d.sav", probname.c_str(), c.serverid);
  FILE *f = fopen(dbuf, "r");
  if(!f) return;
  int err = fread(&ss, sizeof(ss), 1, f);
  if(err != sizeof(ss)) clearall();
  fclose(f);
  }


int main(int argc, char **argv) {

  /* if(argc < 2) {
    printf("wymagany argument: numer serwera (0 lub 1)\n");
    exit(1);
    }

  serverid = (argv[1][0] - '0';
  */
  
  probname = "sample";
  
  c.server = "localhost";
  c.port = 2424;
  c.serverid = 0;

  c.opendebug();
  
  loadall();
  
  loginMarathon(c);
  
  while(true) {

    cmdsend = 0;

    c.sendf("QUESTION\n");
    if(checkOK("question")) {
      c.lineready(true); scanerr=fscanf(c.srvread, "%d", &ss.question);
      }
    
    c.sendf("ANSWER %d\n", ss.question * ss.question);
    if(checkOK("answer")) {
      c.lineready(true); scanerr=fscanf(c.srvread, "%s", ss.status);
      }

    saveall();
    
#ifdef MARATHON24
    c.sendf("WAIT\n");
    if(c.getline(tlo) != "OK") printf("Brak OK na WAIT!\n");
    if(c.getline(tlo) != "WAITING") printf("Brak WAITING na WAIT!\n");
    if(c.getline(tlo) != "OK") printf("Brak drugiego OK na WAIT!\n");
#endif

#ifdef DEADLINE24    
    if(cmdsend == LIMIT) {
    
      while(true) {
        fprintf(srvwrite, "DESCRIBE_WORLD\n");
        if(!checkOK("d-climit")) { LIMIT = cmdsend-2; if(LIMIT < 20) LIMIT = 20; break; }
        lineready(true); 
        // scanerr=c.recvf("%d%d%d %lf", &N, &A, &T, &K);
        }
      double czas;
      lineready(true); scanerr=c.recvf("WAITING %lf", &czas);
      checkOK("wait-climit");
      }
    
    else {

      fprintf(srvwrite, "WAIT\n");
      checkOK("wait");
      double czas;
      lineready(true); scanerr=c.recvf("WAITING %lf", &czas);
      checkOK("wait2");
      // printf("wait = %lf\n", czas);
      }
#endif    
    }
  
  return 0;
  }
