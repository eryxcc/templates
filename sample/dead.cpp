#include <client.h>
#include <login.h>
#define USDL
#define UGD
#define UTTF
#define UGFX
#include <graph.h>

#include <sys/time.h>

#include "rotations.cpp"

tcpclient c;

int coltab[17] = {
  0, 0x80, 0x8000, 0x8080,
  0x800000, 0x800080, 0x808000, 0xC0C0C0,
  0x606060, 0x0000FF, 0x00FF00, 0x00FFFF,
  0xFF0000, 0xFF00FF, 0xFFFF00, 0xFFFFFF,
  0xFFD500
  };

bool ivis = true;

int SX = 1024, SY = 768;
int flags = SDL_RESIZABLE;
int mx, my;

void tlo();

#include <automap.h>

struct savestate {
  char status[200];
  int question;
  };

#ifdef FIXED_STATE
#define DATABUFSIZE 32000000  
char databuf[32000000];
savestate& ss = *(reinterpret_cast<savestate*> (0x700000));
// savestate& ss = (savestate*)(0x0);
#else
savestate ss;
#endif

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
  ss.mypiece = 0;
  ss.inran = 0;
  ss.pieceknown = false;
  printf("cleared\n");
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
  printf("loaded\n");
  char dbuf[64];
  sprintf(dbuf, "game-%s-%d.sav", probname.c_str(), c.serverid);
  FILE *f = fopen(dbuf, "r");
  if(!f) return;
  int err = fread(&ss, sizeof(ss), 1, f);
  if(err < 1) {
    printf("size error: %d vs %d\n", (int)err, (int)sizeof(ss));
    clearall();
    }
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
  
  SDL_UnlockSurface(surface);
/*
  SDL_LockSurface(surface);
  int m = 0;
  for(int i=200; i>0; i-=20) {
    m++;
    style b(0xFFFF0000, (m&1) ? 0x20FFFFFF : 0x20000000, 1);
    circle(b, vec(SX/2, SY/2), i).drawSDL(surface);
    }
  style b(0xFFFF0000, (m&1) ? 0x80FFFFFF : 0x80000000, 1);
  line(b, vec(0,0), vec(SX,SY)).drawSDL(surface);  

  SDL_UnlockSurface(surface);

  displaystr(SX - 16, 16, 12, "PS", 0x40FFFF00, 2); */

  SDL_UpdateRect(surface, 0, 0, 0, 0);  
  }

int dy[4] = {1,0,-1,0};
int dx[4] = {0,1,0,-1};

// reagowanie na klawisze
void klawisze() {
  SDL_Event event;
  SDL_Delay(1);
  bool ev = SDL_PollEvent(&event);
  if(ev) switch (event.type) {
    case SDL_QUIT:
      exit(1);
      return;
    
    case SDL_VIDEORESIZE:
      SX = event.resize.w;
      SY = event.resize.h;
      printf("SX=%d SY=%d\n", SX, SY);
      surface = SDL_SetVideoMode(SX, SY, 32, flags);
      break;

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

bool ended;

int main(int argc, char **argv) {

  build_realrots();
  
  if(argc < 2) {
    printf("wymagany argument: numer serwera (0 lub 1)\n");
    exit(1);
    }

  c.serverid = argv[1][0] - '0';
  
  #ifdef FIXED_STATE
  if(((char*)&ss) >= databuf && (char*)((&ss)+1) <= (databuf+DATABUFSIZE)) {
    printf("savestate OK\n");
    }
  else {
    printf("savestate location error\n");
    printf("ss = %p..%p\n", &ss, &ss+1);
    printf("databuf = %p..%p\n", databuf, databuf+DATABUFSIZE);
    exit(1);
    }
  #endif
  // iprintf("databuf = %p\n", databuf);
  
  probname = "Sample";
  
  c.server = "server";
  c.port = 20000 + c.serverid;
  c.maxlogging = 1048576;

  c.opendebug();
  
  loadall();
  
  loginMarathon(c);
  initGraph(SX, SY, probname + " server " + its(c.serverid), flags);
  
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
        c.sendf("DESCRIBE_WORLD\n");
        if(!checkOK("d-climit")) { LIMIT = cmdsend-2; if(LIMIT < 20) LIMIT = 20; break; }
        c.lineready(true); 
        // scanerr=c.recvf("%d%d%d %lf", &N, &A, &T, &K);
        }
      double czas;
      c.lineready(true); scanerr=c.recvf("WAITING %lf", &czas);
      checkOK("wait-climit");
      }
    
    else {

      c.sendf("WAIT\n");
      checkOK("wait");
      double czas;
      c.lineready(true); scanerr=c.recvf("WAITING %lf", &czas);
      checkOK("wait2");
      // printf("wait = %lf\n", czas);
      }
#endif    
    }
  
  return 0;
  }
