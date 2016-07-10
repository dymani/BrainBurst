#include "BB/GameState/GameStateSplash.h"
#include "BB/Game.h"
#include "BB/GameState/GameStateTitle.h"
#include "BB/GameState/GameStateInit.h"

namespace bb {
  GameStateSplash::GameStateSplash(Game& game, ResourceHandler* resourceHandler, WindowHandler* windowHandler,
    luabridge::lua_State* L) : IGameState(game),
    m_resourceHandler(std::unique_ptr<ResourceHandler>(resourceHandler)),
    m_windowHandler(std::unique_ptr<WindowHandler>(windowHandler)) {
    this->L = L;

    using namespace luabridge;

    assert(!luaL_loadfile(L, "assets/data/gameStates/splash.lua") && !lua_pcall(L, 0, 0, 0));

    luabridge::LuaRef luaSplashes = getGlobal(L, "splashes");
    luabridge::LuaRef luaDuration = getGlobal(L, "duration");
    luabridge::LuaRef luaResolution = getGlobal(L, "resolution");
    assert(luaSplashes.isTable());
    assert(luaDuration.isNumber());
    assert(luaResolution.isTable());

    m_durationTicks = luaDuration.cast<int>() * 50;
    m_splashesNo = 0;
    for (int i = 1; i <= luaSplashes.length(); i++) {
      luabridge::LuaRef luaSplash = luaSplashes[i];
      if (luaSplash.isNil()) {
        continue;
      }
      if (!luaSplash.isString()) {
        LogHandler::log<GameStateSplash>("splash[" + std::to_string(i)
          + "] not a string in splash.lua.", ERR);
        assert(false);
        continue;
      }
      sf::Sprite splash;
      splash.setTexture(m_resourceHandler->getTexture(luaSplash.cast<std::string>()));
      splash.setColor({255, 255, 255, 0});
      splash.setScale(float(luaResolution[1].cast<float>() / 1920),
        float(luaResolution[2].cast<float>() / 1080));
      m_splashes.push_back(splash);

      m_splashesNo++;
    }
    std::string title = "Brain Burst 2039";
  #ifdef _DEBUG
    title += " [WIP]";
  #endif
    m_windowHandler->createWindow(sf::VideoMode(luaResolution[1].cast<int>(),
      luaResolution[2].cast<int>()), title, sf::Style::Close);
    m_state = RUNNING;
    m_updateCounter = int(m_durationTicks * -0.5);
    m_splashCurr = 0;
  }

  void GameStateSplash::handleInput() {
    sf::Event windowEvent;
    while (m_windowHandler->getWindow().pollEvent(windowEvent)) {
      if (windowEvent.type == sf::Event::Closed) {
        m_state = QUIT;
        return;
      }
      else if (windowEvent.type == sf::Event::KeyPressed) {
        switch (windowEvent.key.code) {
          case sf::Keyboard::Escape:
            m_state = QUIT;
            return;
          case sf::Keyboard::F5:
            m_state = RELOAD;
            break;
        }
      }
    }
  }

  bool GameStateSplash::update() {
    switch (m_state) {
      case RUNNING:
        if (m_splashesNo == 0) {
          m_state = NEXT;
        }
        else {
          m_updateCounter++;
          if (m_updateCounter > 0) {
            int currTicks = m_updateCounter - m_splashCurr * m_durationTicks;
            if (currTicks >= m_durationTicks) {
              m_splashCurr++;
              if (m_splashCurr >= m_splashesNo)
                m_state = NEXT;
            }
            else {
              int alpha = int(sin(double(currTicks) / double(m_durationTicks) * 3.14159265) * 255);
              m_splashes[m_splashCurr].setColor({255, 255, 255, sf::Uint8(alpha)});
            }
          }
        }
        break;
      case QUIT:
        return false;
      case NEXT:
        m_game.changeState(new GameStateTitle(m_game, m_resourceHandler.release(),
          m_windowHandler.release(), L));
        break;
      case RELOAD:
        m_game.changeState(new GameStateInit(m_game));
        break;
    }
    return true;
  }

  void GameStateSplash::draw(const double dt) {
    m_windowHandler->getWindow().clear({3, 3, 3, 255});
    if (m_splashesNo != 0 && m_state == RUNNING)
      m_windowHandler->getWindow().draw(m_splashes[m_splashCurr]);
    m_windowHandler->getWindow().display();
  }
}
