#define USDL
#include <topcoder.h>
#include <graph.h>

// przykladowy program interaktywny

bool ivis = true;

int SX = 640, SY = 480;

int mx, my;

void clicked(int x, int y, int b) {
  mx = x; my = y;
  }

void wizualizacja() {
  SDL_LockSurface(surface);
  
  int rad = 100 + 90 * sin(getVa() * 2 * M_PI / 1000000.0);

  for(int uy=SY-1; uy>=0; uy--)
  for(int ux=SX-1; ux>=0; ux--) {
    int d = (uy - my) * (uy - my) + (ux - mx) * (ux-mx);
    pix(ux, uy, surface) = d < rad ? 0xFFFFFF : 0;
    }
  
  SDL_UnlockSurface(surface);
  SDL_UpdateRect(surface, 0, 0, 0, 0);  
  }

// reagowanie na klawisze
void klawisze() {
  SDL_Event event;
  SDL_Delay(1);
  while(ev = SDL_PollEvent(&event)) switch (event.type) {
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
  // flushout();
  if(debuglog) fflush(debuglog);
  if(ivis) {
    wizualizacja();
    klawisze();
    }
  }

int main() {
  initGraph(SX, SY, "test UI");
  while(true) tlo();
  return 0;
  }
