// logowanie sie do serwera
// teamname i password zmieniamy w zaleznosci od potrzeb

#ifndef _LOGIN_H_
#define _LOGIN_H_

const char* teamname = "precision";
const char* password = "screwdrivers";

void loginMarathon(tcpclient& c) {
  printf("Logowanie...\n");
  while(c.srvsock == 0) {
    c.initClient();
    usleep(10000);
    }
  if(c.getline_blocking() != "LOGIN") printf("Brak komendy login!\n");
  fprintf(c.srvwrite, "%s\n", teamname);
  // flushout(); <- niepotrzebne bo getLine wczytuje
  if(c.getline_blocking() != "PASSWORD") printf("Brak komendy password!\n");
  fprintf(c.srvwrite, "%s\n", password);
  if(c.getline_blocking() != "OK") printf("Logowanie niepoprawne!\n");
  else printf("Logowanie OK!\n");
  }  

#define MARATHON24

#endif
