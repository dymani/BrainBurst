#ifndef GAME_STATE_SPLASH_H
#define GAME_STATE_SPLASH_H

#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include <math.h>
#include <memory>
#include "BB/GameState/IGameState.h"
#include "BB/Handler/LogHandler.h"
#include "BB/Handler/ResourceHandler.h"
#include "BB/Handler/WindowHandler.h"

namespace bb {
    class Game;

    class GameStateSplash: public IGameState{
    public:
        GameStateSplash(Game& game, ResourceHandler* resourceHandler, WindowHandler* windowHandler,
          luabridge::lua_State* L);
        void handleInput();
        bool update();
        void draw(const double dt);
    private:
        std::unique_ptr<ResourceHandler> m_resourceHandler;
        std::unique_ptr<WindowHandler> m_windowHandler;
        luabridge::lua_State* L;
        int m_updateCounter, m_splashesNo, m_durationTicks, m_splashCurr;
        std::vector<sf::Sprite> m_splashes;
        enum State {
            RUNNING, QUIT, NEXT, RELOAD
        } m_state;
    };
}

#endif