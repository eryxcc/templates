#define UTTF
#define USDL
#define UGFX
#define UGD

#include "../libs/graph2.h"

int main() {
  style b(0xFF000000, 0x80000000, 1);
  pic p;
  
  p += line(b, vec(0,0), vec(200,0));
  p += line(b, vec(200,0), vec(200,200));
  p += line(b, vec(200,200), vec(0,200));
  p += line(b, vec(0,200), vec(0,0));
  p += line(b, vec(100, 200), vec(100, 0));
  p += line(b, vec(200, 100), vec(0, 100));
  p += line(b, vec(200, 120), vec(0, 120));
  
  font ff = makefont("DejaVuSans-Bold.ttf", "");
  
  p += text(b, ff, vec(100, 100), topleft, 20, "hello");
  p += text(b, ff, vec(100, 100), topright, 20, "hey");
  
  svg_normalizer s("gtest.svg");
  s += p;
  
  bmp_normalizer bn;
  bn += p;
  
  writePng("gtest.png", bn.getBitmap());
  bn.drawonscreen();
  SDL_Delay(1000);
  
  return 0;
  }
