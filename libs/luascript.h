#ifndef _LUASCRIPT_H_
#define _LUASCRIPT_H_

// skrypty w Lua, po to, zeby mozna bylo zmieniac parametry przez
// edycje pliku (i je testowac bez ponownego uruchamiania ani
// ponownej kompilacji), a takze po to, zeby w ogole mozna bylo 
// programowo wplywac na dzialanie algorytmu (szybko, tzn. bez
// ponownego uruchamiania ani ponownej kompilacji), ja chcialbym
// miec cos takiego, ale to pewnie nie jest bardzo potrzebne

#include <topcoder.h>

extern "C" {
  #include <lua.h>
  #include <lualib.h>
  #include <lauxlib.h>
  }

lua_State *L;

void lua_setglobalnum(const char *name, lua_Number value) {
  lua_pushnumber(L, value); lua_setglobal(L, name);
  }

lua_Number lua_getglobalnum(const char *name) {
  lua_getglobal(L, name);
  auto l = luaL_checknumber(L, -1);
  lua_pop(L, 1);
  return l;
  }

void lua_setglobalstr(const char *name, const char *value) {
  lua_pushstring(L, value); lua_setglobal(L, name);
  }

string lua_getglobalstr(const char *name) {
  lua_getglobal(L, name);
  string s = luaL_checkstring(L, -1);
  lua_pop(L, 1);
  return s;
  }

void lua_setfunction(const char *name, int f(lua_State *L)) {
  lua_register(L, name, f);
  }

const char *lua_getargstr(lua_State *L, int x) {
  return luaL_checkstring(L, x);
  }

lua_Number lua_getargnum(lua_State *L, int x) {
  return luaL_checknumber(L, x);
  }

int lua_getargint(lua_State *L, int x) {
  return luaL_checkint(L, x);
  }

int lc_square(lua_State *L) {
  lua_Number x = lua_getargnum(L, 1);
  lua_pushnumber(L, x*x);
  return 1;
  }

void lua_callfunction(const char *funname) {
  /* push functions and arguments */
  lua_getglobal(L, funname);  /* function to be called */
  // lua_pushnumber(L, x);   /* push 1st argument */
  // lua_pushnumber(L, y);   /* push 2nd argument */
    
  /* do the call (0 arguments, 0 result) */
  if (lua_pcall(L, 0, 0, 0) != 0)
    printf("error running function '%s': %s", funname, lua_tostring(L, -1));
    
  // if (!lua_isnumber(L, -1))
  //   error(L, "function `f' must return a number");
  // z = lua_tonumber(L, -1);
  // lua_pop(L, 1);  /* pop returned value */
  }

void lua_reload(const char *fname = "config.lua") {
  if(luaL_dofile(L, fname))
    printf("error running %s: %s", fname, lua_tostring(L, -1));
  }

int lc_setarg(lua_State *L);

void initScript() {
  L = lua_open();

  if (!L) {
    printf("lua_open error\n");
    exit(-1);
    }

  luaL_openlibs(L);
#ifdef _CLIENT_H_
  lua_setglobalnum("serverid", serverid);
#endif
  lua_setfunction("square", lc_square);
  }

#endif _LUASCRIPT_H_
