#ifndef _CLIENT_H_
#define _CLIENT_H_

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

#include <topcoder.h>
#include <netinet/tcp.h>

#include <cstdarg>

using namespace std;

struct tcpclient {
  int srvsock;
  
  // czy logowac wejscie/wyjscie
  bool logging;
  
  // czy cos sie zepsulo?
  bool had_error;
  
  // czy logowac wejscie/wyjscie jako binarne
  bool binarylogging;
  
  // maksymalny rozmiar danych, ktore wypisujemy
  int maxlogging;
  
  int port;
  string server;

  int serverid;
  tcpclient() { logging = true; binarylogging = false; maxlogging = 1024; }

  FILE *srvwrite, *srvread;

  // opcjonalne definicje: 
  // PIPE_IN -> uzywanie PIPE do buforowania wejscia (nie wymaga pliku)
  // PIPE_OUT -> uzywanie PIPE do buforowania wyjscia
  // BUF_OUT -> uzywanie pliku do buforowania wyjscia (najwyrazniej nie dziala na OSX)
  // pipe sa mniej bezpieczne (jak jest duzo danych, to sie moze zablokowac)
  // brak zadnej z opcji (PIPE_OUT i BUF_OUT) chyba tez jest niebezpieczny
  // (wtedy fprintf wysyla bezposrednio do socketa, z testow mi wyszlo, ze to 
  // gubi dane, jak jest ich za duzo)

#ifdef USE_PIPES
  #define PIPE_OUT
  #define PIPE_IN
#else
  #define BUF_OUT
#endif
  
  #ifdef PIPE_OUT
  int ofd[2]; // pipe do buforowania wyjscia
  #endif
  
  #ifdef BUF_OUT
  FILE *ofd0, *ofd1;
  #endif
  
  #ifdef PIPE_IN
  int pfd[2]; // pipe do buforowania wejscia  
  #else
  FILE *pfd0, *pfd1;
  #endif
  
  void closeClient();
  void initClient();
  void flushout();

  void opendebug();

  // ASCII & BINARY
  //================
  
  // zwraca liczbê odebranych znaków

  // koniec wejscia blokujacego: zwraca 0
  // koniec wejscia nieblokujacego: zwraca -1
  // -1 oznacza b³¹d odczytu
  int getinput(bool blocking);
  
  // uwaga: wyjscie binarne i ASCII sie wykluczaja!

  // BINARY
  //========

  // ile bajtow zostalo wczytane, ale nie odebrane przez recv*
  int inputbytes;
  
  void sendbytes(void* data, int bytes);
  template<class T> void senddata(T& data) { sendbytes(&data, sizeof(data)); }

  // recvbytes zwraca 'false' jesli byl blad odczytu

  bool recvbytes(void* data, int bytes);
  template<class T> bool recvdata(T& data) { return recvbytes(&data, sizeof(data)); }
  
  // ASCII
  //=======

  // ile linii zostalo odebranych (przez getinput), ale nie wczytanych (przez lineready)
  int inputlines;

  // wczytuje przygotowana linie
  char getline_last[1000000];

  bool lineready(bool blocking);
  
  // blokujaco czyta linie
  string getline_blocking();
  
  // wczytuje linie, ktora zostala wczesniej przygotowana przez lineready
  string getline_ready();

  // wczytuje linie, podczas czekania wykonuje akcje
  template<class T> string getline(T akcja);
  
  // wyslij sformatowane
  void sendf(const char *format, ...);

  // zamiast getline mozna parsowac (dziala jak scanf)
  int recvf(const char *format, ...);

  };

void tcpclient::closeClient() {
  if(srvread) fclose(srvread), srvread = NULL;
  if(srvwrite) fclose(srvwrite), srvwrite = NULL;
  if(srvsock) close(srvsock), srvsock = 0, inputlines = 0, inputbytes = 0;
#ifdef PIPE_IN
  if(pfd[1]) close(pfd[1]), pfd[1] = 0;
#else
  if(pfd1) fclose(pfd1), fclose(pfd0), pfd1 = pfd0 = 0;
#endif
#ifdef PIPE_OUT
  if(ofd[0]) close(ofd[0]), ofd[0] = 0;
  #endif
#ifdef BUF_OUT
  if(ofd1) fclose(ofd1), fclose(ofd0), ofd1 = ofd0 = 0;
  #endif
  }

void tcpclient::initClient() {
  closeClient();

  struct sockaddr_in connTo;

  srvsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (srvsock < 0) {
    had_error = true;
    perror("error opening stream socket");
    exit(1);
    }

  int flag = 1;
  if(setsockopt(srvsock,IPPROTO_TCP,TCP_NODELAY,(char *)&flag,sizeof(flag)) == -1) {
    perror("setsockopt nodelay");
    exit(1);
    }
  
  inputbytes = 0;
  inputlines = 0;

  connTo.sin_family = AF_INET;

  struct hostent *hp;
  hp = gethostbyname(server.c_str());
  if (hp) {
    memcpy(&connTo.sin_addr.s_addr, hp->h_addr, hp->h_length);
  } else {
    had_error = true;
    perror("DNS error");
    exit(1);
    }

  // printf("adres = %d\n", connTo.sin_addr.s_addr);

  connTo.sin_port = htons(port);
  memset(connTo.sin_zero, 0, 8);

  int con = connect(srvsock, (struct sockaddr *) &connTo, sizeof connTo);
  if (con < 0) {
    had_error = true;
    perror("connecting");
    close(srvsock); srvsock = 0;
    return;
    }
  
#ifdef PIPE_IN
  if(pipe(pfd) < 0) {
    had_error = true;
    perror("pipe");
    }
  int ipipesize = 800000;
#ifndef MAC
  fcntl(pfd[1], F_SETPIPE_SZ, &ipipesize);
  fcntl(pfd[0], F_SETPIPE_SZ, &ipipesize);
#endif
  srvread = fdopen(pfd[0], "r");
#else
  char xbuf[1024];
  sprintf(xbuf, "pipe-%s-in-%d.log", probname.c_str(), serverid);
  pfd1 = fopen(xbuf, "wb");
  pfd0 = fopen(xbuf, "rb");
  srvread = pfd0;
#endif

  srvwrite = NULL;
  
#ifdef PIPE_OUT
  if(pipe(ofd) < 0) {
    had_error = true;
    perror("pipe");
    }
  
  int opipesize = 800000;
  fcntl(ofd[1], F_SETPIPE_SZ, &opipesize);
  fcntl(ofd[0], F_SETPIPE_SZ, &opipesize);

  srvwrite = fdopen(ofd[1], "w");
  fcntl(ofd[0], F_SETFL, O_NONBLOCK);
#endif

#ifdef BUF_OUT
  sprintf(xbuf, "pipe-%s-out-%d.log", probname.c_str(), serverid);
  ofd1 = fopen(xbuf, "wb");
  ofd0 = fopen(xbuf, "rb");
  srvwrite = ofd1;
#endif

  if(!srvwrite) srvwrite = fdopen(srvsock, "w");
  }

void tcpclient::flushout() {
  fflush(srvwrite);
  int b = 0;
  
#ifdef PIPE_OUT
  char buf[1048576];
  b = read(ofd[0], buf, 1048576);
  if(b < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
    // no data
    return;
    }
  else if(b <= 0) {
    had_error = true;
    perror("pipe error");
    return;
    }
#endif
#ifdef BUF_OUT
  char buf[1048576];

// nie rozumiem dlaczego bez tego na OSX nie dziala

  fseek(ofd0, 0, SEEK_CUR);

  b = fread(buf, 1, 1048576, ofd0);
#endif

  buf[b] = 0; if(logging) powerlogger(debuglog, timestamp_str() + " ---", string(buf, b), binarylogging, maxlogging);
  int bstart = 0;
  while(b) {
    int ret = write(srvsock, buf+bstart, b);
    if(ret <= 0) {
      had_error = true;
      perror("write error (output)");
      exit(1);
      }
    bstart += ret; b -= ret;
    }
  }

// zwraca liczbê odebranych znakow
// blocking: czy blokowac
int tcpclient::getinput(bool blocking) {
  flushout();
  int b;
  char buf[1048576];
  fcntl(srvsock, F_SETFL, blocking ? 0 : O_NONBLOCK);
  b = read(srvsock, buf, 1048576);
  if(b < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
    // blocking, no data!
    return 0;
    }
  if(b <= 0) {
    printf("b = %d\n", b);
    had_error = true;
    perror("read");
    return b;
    }
  buf[b] = 0; if(logging) powerlogger(debuglog, timestamp_str() + "  - ", string(buf, b), binarylogging, maxlogging);
#ifdef PIPE_IN
  if(write(pfd[1], buf, b) != b) {
    had_error = true;
    perror("write error (input)");
    }
#else
  if(int(fwrite(buf, 1, b, pfd1)) != b) {
    had_error = true;
    perror("write error (input)");
    }
  fflush(pfd1);
  #endif
  for(int i=0; i<b; i++) if(buf[i] == 10) inputlines++;
  inputbytes += b;
  return b;
  }

void loginMarathon(tcpclient&);

// jesli blocking=false i serwer nie wyslal jeszcze nastepnej linijki, to zwracamy false
// wpp (czekamy na linijke i) zwracamy true
bool tcpclient::lineready(bool blocking) {
  if(srvsock == 0) return false;
  if(inputlines > 0) { inputlines--; return true; }
  again:
  getinput(blocking);
  if(inputlines > 0) { inputlines--; return true; }
  if(blocking) goto again;
  return false;
  }

string tcpclient::getline_ready() {
  scanerrp = fgets(getline_last, 1000000, srvread);
  // wywalamy LF
  for(int i=0; getline_last[i]; i++) 
    if(getline_last[i] == 10) 
      getline_last[i] = 0;
  // zakladam, ze linia pusta wystepuje tylko w przypadku, gdy poprzedni scanf nie wczytal konca linii
  if(getline_last[0] == 0)
    return getline_ready();
  // zwracamy
  return getline_last;
  }

// przygotowuje i wczytuje linie
string tcpclient::getline_blocking() {
  lineready(true);
  return getline_ready();
  }

// przygotowuje i wczytuje linie, wykonujac 'akcja' podczas oczekiwania
template<class T> string tcpclient::getline(T akcja) {
  while(!lineready(false)) akcja();
  return getline_ready();
  }

void tcpclient::sendf(const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  vfprintf(srvwrite, format, ap);
  va_end(ap);
  };
  
int tcpclient::recvf(const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  int i = vfscanf(srvread, format, ap);
  va_end(ap);
  return i;
  };
  
void tcpclient::opendebug() {
  char xbuf[1024];
  sprintf(xbuf, "debug-%s-%d.log", probname.c_str(), serverid);
  debuglog = fopen(xbuf, "w");
  }

void tcpclient::sendbytes(void* data, int bytes) {
  if(bytes < 0) {
    perror("bytes < 0");
    exit(1);
    }
  if(fwrite(data, 1, bytes, srvwrite) != (unsigned) bytes) {
    perror("not enough bytes sent");
    exit(1);
    }
  flushout();
  }

bool tcpclient::recvbytes(void* data, int bytes) {
  if(bytes < 0) {
    perror("bytes < 0");
    exit(1);
    }

  while(inputbytes < bytes) {
    if(had_error) return false;
    getinput(true);
    }
  int iread = fread(data, 1, bytes, srvread);
  if(iread != (unsigned) bytes) {
    perror("not enough bytes received");
    exit(1);
    }
  inputbytes -= bytes;
  return true;
  }

#endif
