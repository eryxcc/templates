#include <client.h>
#include <login.h>
#define USDL
#include <graph.h>

tcpclient c;

bool ivis = true;

int SX, SY;
int mx, my;

string seekfor = "--myboundary\r\nContent-Type: image/jpeg\r\n";
int seekpos = 0;

char jpgdata[2000000];
int jpgsize;

int framecount = 0;

SDL_Surface* readJpgData() {
  // for(int k=0; k<20; k++) printf("%02x ", (unsigned char) jpgdata[k]); 
  // printf(" (%d B)\n", jpgsize);
  // for(int k=0; k<20; k++) printf("%c", jpgdata[k] >= 32 && jpgdata[k] < 128 ? jpgdata[k] : '.'); printf("\n");
  for(int k=0; k<4; k++) jpgdata[jpgsize+k] = 0;
  gdImagePtr im;
  int at = 0;
  // w strumieniu czasem jest \0 na poczatku (a czasem go nie ma)
  if(jpgdata[0] == 0) at++;
  // w kadzej klatce poza pierwsza jest 00dcXXXX gdzie X to rozmiar klatki (chyba)
  if(jpgdata[at] == '0') {
    unsigned int& framesize(*((unsigned int*) (jpgdata+at+4)));
    int junkdata = framesize - jpgsize;
    if(junkdata != 16 && junkdata != 17)
      printf("framesize = %d jpgsize = %d d = %d\n", framesize, jpgsize, junkdata);
    at += 8;
    }
  im = gdImageCreateFromJpegPtr(jpgsize-at, jpgdata+at);
  SDL_Surface *s = GDtoSDL(im);
  gdImageDestroy(im);
  return s;
  }

int main(int argc, char **argv) {

  /* if(argc < 2) {
    printf("wymagany argument: numer serwera (0 lub 1)\n");
    exit(1);
    }

  serverid = (argv[1][0] - '0';
  */
  
  probname = "mjpegtest";
  
  // http://repo.hu/mjpg/video.mjpg
  c.server = "repo.hu";
  c.port = 80;
  c.serverid = 0;
  
  c.maxlogging = 2048;
  c.binarylogging = true;

  c.opendebug();
  
  c.initClient();
  
  c.sendf(
    "GET http://repo.hu/mjpg/video.mjpg HTTP/1.1\n"
    "Host: repo.hu\n"
    "\n"
    );
  
  int breaksize = 0;
  jpgsize = 0;
  
  bool firstframe = true;
  
  while(true) {
    char ch;
    if(!c.recvdata(ch)) {
      printf("koniec strumienia\n");
      break;
      }
    jpgdata[jpgsize++] = ch;
    breaksize++;
    // printf("inputbytes = %d [%c]\n", c.inputbytes, ch);
    if(ch == seekfor[seekpos]) {
      seekpos++;
      if(seekfor[seekpos] == 0) {
      
        if(firstframe) {
          printf("read %d bytes of headers\n", jpgsize);
          firstframe = false;
          }
        else {
          framecount++;
          SDL_Surface *frame = readJpgData();
          if(SX == 0) {
            SX = frame->w;
            SY = frame->h;
            initGraph(SX, SY, probname + " MJPEG viewer");
            }
          
          // 'blit' zeby bylo szybciej
          SDL_BlitSurface(frame, NULL, surface, NULL);
          SDL_UpdateRect(surface, 0, 0, 0, 0);  
          
          SDL_FreeSurface(frame);
          }

        string s;
        char ch;
        do { 
          c.recvdata(ch);
          s += ch;
          }
        while(ch != '\n');
        c.recvdata(ch); // CR
        c.recvdata(ch); // LF

        // scanerr =  sscanf(s.c_str(), "Content-Length: %d", &jpgsize);
        // printf("jpg size: %d (%d break, %d err) :: %s\n", jpgsize, breaksize, scanerr, s.c_str());
        // breaksize = 0;
        jpgsize = 0;
        seekpos = 0;
        }
      }
    else if(ch == seekfor[0]) {
      seekpos = 1;
      }
    else {
      seekpos = 0;
      }
    }

  printf("framecount = %d\n", framecount);
  return 0;
  }
