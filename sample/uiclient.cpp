#include <client.h>
#include <login.h>
#define USDL
#include <graph.h>

tcpclient c;

bool ivis = true;

int SX = 640, SY = 480;
int mx, my;

void tlo();

struct savestate {
  char status[200];
  int question;
  } ss;

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

void clicked(int x, int y, int b) {
  mx = x; my = y;
  if(b >= 0) {
    // ...
    }
  }

void wizualizacja() {
  SDL_LockSurface(surface);
  
  for(int uy=SY-1; uy>=0; uy--)
  for(int ux=SX-1; ux>=0; ux--) {
    pix(ux, uy, surface) = 0x10101 * (rand() % (1 + 16 * ss.question));
    }
  
#ifdef MAC
  // on OSX we need to change the colors...
  for(int y=0; y<SY; y++)
  for(int x=0; x<SX; x++) {
    int& i( pix(x,y,surface) );
    i = ((i & 0xFF00FF) << 8) | ((i & 0xFF00FF00) >> 8);
    i = ((i & 0xFFFF) << 16) | ((i & 0xFFFF0000) >> 16);
    }
#endif

  SDL_UnlockSurface(surface);
  SDL_UpdateRect(surface, 0, 0, 0, 0);  
  }

// reagowanie na klawisze
void klawisze() {
  SDL_Event event;
  SDL_Delay(1);
  bool ev = SDL_PollEvent(&event);
  if(ev) switch (event.type) {
    case SDL_QUIT:
      exit(1);
      return;

    case SDL_MOUSEBUTTONDOWN: {
      clicked(event.button.x, event.button.y, event.button.button);
      break;
      }
    
    case SDL_MOUSEMOTION: {
      clicked(event.motion.x, event.motion.y, -1);
      break;
      }
    
    case SDL_KEYDOWN: {
      int key = event.key.keysym.sym;
      int uni = event.key.keysym.unicode;
      
      printf("uni = %c key = %d\n", uni, key);
      
      break;
      }
    
    }
  }

void tlo() { 
  c.flushout(); 
  if(debuglog) fflush(debuglog);
  if(ivis) {
    wizualizacja();
    klawisze();
    }
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
  initGraph(640, 480, probname + " server " + its(c.serverid));
  
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
