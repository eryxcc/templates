// przykladowy server do testowania klienta

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <netdb.h>

#include <vector>
#include <string>

using namespace std;

int sock;
struct sockaddr_in server;

void initServer() {
  sock = socket(AF_INET, SOCK_STREAM, 0);

  if (sock < 0) {
    perror("socket");
    exit(1);
    }

  // zeby od razu socket byl dostepny z powrotem
  int on = 1;
  
  setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(2424);

  if(bind(sock, (struct sockaddr *) &server, sizeof server) < 0) {
    perror("bind");
    exit(1);
    }

  listen(sock, 20);
  signal(SIGPIPE, SIG_IGN); 
  }

struct clientstruct {
  sockaddr client;
  int sock;
  int length;
  int getcount;
  FILE *fp;
  char servbuf[4096];
  int state; // 0 = bez zagadki, 1 = jest zagadka, 2 = odpowiedziano
  int score, scoreof;
  bool check();
  bool nextturn();
  };

vector<clientstruct*> cli;

template<class T> int Size(T& z) { return z.size(); }

char cmd[64];
int cval;


#define STATE_INIT 0
#define STATE_LOGIN 1
#define STATE_PASSWORD 2
#define STATE_WAITING 3
#define STATE_NORMAL 4
#define STATE_ASKING 5
#define STATE_ANSWERED 6

void checkServer() {
  fcntl(sock, F_SETFL, O_NONBLOCK);
  socklen_t       length;
  sockaddr client;
  length = sizeof(client);
  int msgsock = accept(sock, &client, &length);
  
  if(msgsock > 0) {
    printf("New client!\n");
    clientstruct *cl = new clientstruct;
    cl->sock = msgsock;
    cl->client = client;
    cl->length = length;
    cl->fp = fdopen(cl->sock, "w");
    cl->getcount = 0;
    cl->scoreof = 0;
    cl->score = 0;
    cl->state = STATE_INIT;
    cli.push_back(cl);
    }
  
  for(int i=0; i<Size(cli); i++) if(cli[i]->check()) {
    delete cli[i];
    cli[i] = cli[Size(cli)-1];
    i--;
    cli.resize(Size(cli)-1);    
    }
  }

int puzzle, answer, turncount;

bool clientstruct::check() {

  // uwaga: serwer jest slaby, zle dziala, jak niecala linia zostanie przyslana

  if(state == STATE_INIT) {
    printf("Asking a client to log in\n");
    fprintf(fp, "LOGIN\n");
    fflush(fp);
    state = STATE_LOGIN;
    }
    
  fcntl(sock, F_SETFL, O_NONBLOCK);
  int bcount = read(sock, servbuf+getcount, 4096-getcount);
  
  if(bcount < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
    printf("closed (error)\n");
    close(sock);
    return true;
    }
  
  if(bcount == 0) {
    printf("closed (normal)\n");
    if(fp) fclose(fp);
    else close(sock);
    return true;
    }
  
  if(bcount < 0) {
    // waiting
    return false;
    }
  
  getcount += bcount; servbuf[getcount-1] = 0; // kasujemy LF
  
  std::string sstr = servbuf;
  
  if(state == STATE_LOGIN) {
    printf("Team logging in: %s\n", servbuf);
    fprintf(fp, "PASSWORD\n");
    state = STATE_PASSWORD;
    }
  
  else if(state == STATE_PASSWORD) {
    printf("Team password used: %s\n", servbuf);
    fprintf(fp, "OK\n");
    state = STATE_NORMAL;
    }
  
  else if((state == STATE_NORMAL || state == STATE_ANSWERED) && sstr == "WAIT") {
    fprintf(fp, "OK\nWAITING\n");
    state = STATE_WAITING;
    }
  
  else if(state == STATE_NORMAL && sstr == "QUESTION") {
    fprintf(fp, "OK\n%d\n", puzzle);
    scoreof++;
    state = STATE_ASKING;
    }
    
  else if(sstr == "LONGTEST") {  
//  string z;
//  for(int i=0; i<500000; i++) z += char(48 + rand() % 10);
//  fprintf(fp, "%s\n", z.c_str());
//  ^^ nie dziala
    char id;
    int ile = 500000;
    while(ile > 0) {
      id = '0' + rand() % 10;
      if(write(sock, &id, 1) > 0) ile--;
      }
    id = 10;
    while(write(sock, &id, 1) <= 0) ;
    }

  else if(state == STATE_ASKING && servbuf[0] == 'A') {
    sscanf(servbuf, "%s%d", cmd, &cval);
    if(cval == answer)
      score++, fprintf(fp, "OK\nCORRECT\n");
    else
      fprintf(fp, "OK\nWRONG\n");
    state = STATE_ANSWERED;
    }
  
  else fprintf(fp, "ERROR zla komenda\n");
  
  printf("S%d [%3d/%3d] %s\n", state, score, scoreof, servbuf);
  fflush(fp);
  getcount = 0;

  return false;
  }

bool clientstruct::nextturn() {
  if(state == STATE_ASKING || state == STATE_ANSWERED)
    state = STATE_NORMAL;
  
  if(state == STATE_WAITING) {
    state = STATE_NORMAL;
    fprintf(fp, "OK\n");
    fflush(fp);
    printf("(klient czekajacy zakonczyl czekanie)\n");
    }
  }

int main() {
  initServer();
  while(true) {
    usleep(10000);
    checkServer();
    turncount++;
    if(turncount == 500) {
      turncount = 0;
      puzzle = rand() % 10;
      answer = puzzle * puzzle;
      for(int i=0; i<Size(cli); i++) cli[i]->nextturn();
      }
    }
  return 0;
  }
