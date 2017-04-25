// test skryptu Lua

// skrypty w Lua, po to, zeby mozna bylo zmieniac parametry przez
// edycje pliku (i je testowac bez ponownego uruchamiania ani
// ponownej kompilacji), a takze po to, zeby w ogole mozna bylo 
// programowo wplywac na dzialanie algorytmu (szybko, tzn. bez
// ponownego uruchamiania ani ponownej kompilacji), ja chcialbym
// miec cos takiego, ale to pewnie nie jest bardzo potrzebne

#include <luascript.h>

int px, py;

int lc_setarg(lua_State *L) {
  px = (int) lua_getglobalnum("param_x");
  py = (int) lua_getglobalnum("param_y");
  return 0;
  }

int main() {
  initScript();
  lua_setfunction("setarg", lc_setarg);
  lua_reload("testscript.lua");
  printf("px = %d py = %d\n", px, py);
  lua_callfunction("testcall");
  return 0;
  }
  
