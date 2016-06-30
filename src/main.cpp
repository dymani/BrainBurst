//#include "BB/Game.h"
//#include "BB/GameState/GameStateInit.h"
//
//int main() {
//  bb::Game game;
//  game.pushState(new bb::GameStateInit{game});
//  int returnVal = game.run();
//#if _DEBUG
//  system("PAUSE");
//#endif
//  return returnVal;
//}

// main.cpp
#include <LuaBridge/LuaBridge.h>
#include <iostream>
extern "C" {
# include "lua/lua.h"
# include "lua/lauxlib.h"
# include "lua/lualib.h"
}

using namespace luabridge;
int main() {
  lua_State* L = luaL_newstate();
  luaL_dofile(L, "assets/data/gameStates/init.lua");
  luaL_openlibs(L);
  lua_pcall(L, 0, 0, 0);
  LuaRef s = getGlobal(L, "texture");
  std::string luaString = s.cast<std::string>();
  std::cout << luaString << std::endl;
  system("PAUSE");
}