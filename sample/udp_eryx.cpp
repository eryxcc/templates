#include <iostream>
#include "../tomek_network/network.h"
#include "../tomek_network/util.h"

#include "../tomek_network/network.cc"
#include "../tomek_network/util.cc"

#include <topcoder.h>

using namespace std;

int main() {
  SetLog("example_udp_client.log");
  UdpClient udp(Peer("server", 50000));
  REP(i,5) {
    udp.Write("hello");
    cout << udp.Read() << endl;
    }
  
  powerlogger(stdout, timestamp_str() + " (test) ", datatostr(1000), true, 999);
  
  printf("i = %d\n", strtodata<int>("RIFF"));
  }
