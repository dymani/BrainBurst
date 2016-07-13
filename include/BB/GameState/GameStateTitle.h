#ifndef GAME_STATE_TITLE_H
#define GAME_STATE_TITLE_H

#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include <memory>
#include "BB/GameState/IGameState.h"
#include "BB/Handler/ResourceHandler.h"
#include "BB/Handler/WindowHandler.h"
#include "BB/Handler/GuiHandler.h"

namespace bb {
  class Game;

  class GameStateTitle : public IGameState {
  public:
    GameStateTitle(Game& game, ResourceHandler* resourceHandler, WindowHandler* windowHandler, luabridge::lua_State* L);
    void handleInput();
    bool update();
    void draw(const double dt);
  private:
    std::unique_ptr<ResourceHandler> m_resourceHandler;
    std::unique_ptr<WindowHandler> m_windowHandler;
    std::unique_ptr<GuiHandler> m_guiHandler;
    luabridge::lua_State* L;
    enum State {
      RUNNING, QUIT, GAME, RELOAD
    } m_state;
    sf::Sprite m_background;
    sf::Text m_text;
  };
}

#endif