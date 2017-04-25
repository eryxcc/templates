// test rysowania na ekranie

#define USDL
#define UGFX

#include <topcoder.h>
#include <graph.h>

int SX = 640, SY = 480;

int main(int argc, char ** argv) {
  initGraph(SX, SY, "target");

  for(int y=0; y<SY; y++)
  for(int x=0; x<SX; x++) pix(x,y,surface) = 0x10101 * (128 + rand() % 64);

#ifdef MAC
  // on OSX we need to change the colors...
  for(int y=0; y<SY; y++)
  for(int x=0; x<SX; x++) {
    int& i( pix(x,y,surface) );
    i = ((i & 0xFF00FF) << 8) | ((i & 0xFF00FF00) >> 8);
    i = ((i & 0xFFFF) << 16) | ((i & 0xFFFF0000) >> 16);
    }
#endif

  SDL_LockSurface(surface);
  int m = 0;
  for(int i=200; i>0; i-=20) {
    m++;
    style b(0xFFFF0000, (m&1) ? 0x80FFFFFF : 0x80000000, 1);
    circle(b, vec(SX/2, SY/2), i).drawSDL(surface);
    }
  style b(0xFFFF0000, (m&1) ? 0x80FFFFFF : 0x80000000, 1);
  line(b, vec(0,0), vec(SX,SY)).drawSDL(surface);  

  SDL_UnlockSurface(surface);

  displaystr(SX - 16, 16, 12, "PS", 0x40FFFF00, 2);

  SDL_UpdateRect(surface, 0, 0, 0, 0);
  
  // z jakiegos powodu trzeba ustawic alpha
  SDL_LockSurface(surface);
  for(int y=0; y<SY; y++)
  for(int x=0; x<SX; x++) pix(x,y,surface) |= 0xFF000000;
  SDL_UnlockSurface(surface);
  
  writePng("target.png", surface);

  getchar();
  return 0;
  }
