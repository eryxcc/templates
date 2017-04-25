// test czytania obrazu

#define USDL
#include <topcoder.h>
#include <graph.h>

int main() {
  SDL_Surface *srf = readPng("image16_example.png");
  initGraph(srf->w, srf->h, "image16");

  SDL_LockSurface(surface);
  for(int y=0; y<srf->h; y++)
  for(int x=0; x<srf->h; x++)
    pix(x,y,surface) = pix(x,y,srf);
  SDL_UnlockSurface(surface);
  SDL_UpdateRect(surface, 0, 0, 0, 0);  

  getchar();
  return 0;
  }
