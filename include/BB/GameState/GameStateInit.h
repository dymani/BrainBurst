#ifndef GAME_STATE_INIT_H
#define GAME_STATE_INIT_H

#include <LuaBridge/LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include <SFML/Graphics.hpp>
#include <memory>
#include "BB/GameState/IGameState.h"
#include "BB/Handler/WindowHandler.h"
#include "BB/Handler/ResourceHandler.h"
#include "BB/Handler/LogHandler.h"

namespace bb {
  class GameStateInit : public IGameState {
  public:
    GameStateInit(Game& game);
    ~GameStateInit();
    void handleInput();
    bool update();
    void draw(const double dt);
  private:
    std::unique_ptr<WindowHandler> m_windowHandler;
    std::unique_ptr<ResourceHandler> m_resourceHandler;
    luabridge::lua_State* L;
    sf::Sprite m_logo;
    int m_updateCount;
    enum State {
      RUNNING, QUIT, NEXT, RELOAD
    } m_state;
  };
}

#endif