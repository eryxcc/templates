#include "../libs/graph2.h"

int main() {
  style b(0xFF000000, 0x80000000, 1);
  pic p;

  vec pt[16];
  for(int i=0; i<16; i++) pt[i] = xspin(M_PI/8*i) * vec(100, 0);

  if(0)
  for(int i=0; i<16; i++)
  for(int j=0; j<i; j++)
    p += line(b, pt[i], pt[j]);
    
  p += line(b, vec(-100, 0), vec(100,0));
  p += line(b, vec(0,-100), vec(0,100));
  
  line l1(b, vec(10, 0), vec(60, 80));
  line l2(b, vec(0, 30), vec(90, 0));
  
  auto styles = {
    style(0xFF000000, 0x80A00000, 0.5),
    style(0xFF000000, 0x8000A000, 0.5),
    style(0xFF000000, 0x800000A0, 0.5),
    style(0xFF000000, 0x80A0A000, 0.5),
    style(0xFF000000, 0x80A000A0, 0.5),
    style(0xFF000000, 0x8000A0A0, 0.5)
    };
  
  auto st = styles.begin();
  
  p += l1;
  p += l2;
  for(vec v: crosses(l1, l2))
    p += circle(*(st++), v, 4);
    
  circle c(b, vec(0, 0), 40);
  p += c;
  
  for(vec v: crosses(l1, c))
    p += circle(*(st++), v, 3);

  circle c2(b, vec(-10, -30), 20);
  p += c2;

  for(vec v: crosses(c, c2))
    p += circle(*(st++), v, 2);

  
  svg_normalizer s("gtest.svg");
  s += p;
  
  return 0;
  }
