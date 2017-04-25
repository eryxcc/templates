// produkcja obrazka w formacie svg
// (raczej na wlasny uzytek, niekoniecznie konkursowy)

#include <graph.h>

int main() {

  picturegroup p;
  
  int m = 0;
  
  for(int i=500; i > 0; i -= 50) {
    m++;
    style b(0xFFFF0000, (m&1) ? 0x80FFFFFF : 0x80000000, 1);
    p.add(circle(b, vec(500, 500), i));
    }
  
  drawSvg("svgout.svg", 1000, 1000, &p);
  return 0;
  }
