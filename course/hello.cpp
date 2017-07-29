#include "easy.cpp"
using namespace easy;

int main() {
  string s;
  write("What's your name? ");
  read(s);
  writeln("Hi ", s, "!");
  write("How old are you? ");
  int i;
  read(i);
  writeln("You are ", i, " years old");
  return 0;
  }
