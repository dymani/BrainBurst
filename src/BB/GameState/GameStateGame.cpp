#include "BB/GameState/GameStateGame.h"
#include "BB/Game.h"
#include "BB/GameState/GameStateInit.h"

namespace bb {
  GameStateGame::GameStateGame(Game& game, ResourceHandler* resourceHandler, WindowHandler* windowHandler,
    luabridge::lua_State* L) : IGameState(game),
    m_resourceHandler(std::unique_ptr<ResourceHandler>(resourceHandler)),
    m_windowHandler(std::unique_ptr<WindowHandler>(windowHandler)),
    m_world(new World(*m_resourceHandler, *m_windowHandler, L)) {

    this->L = L;

    m_windowHandler->getWindow().setView(m_windowHandler->getWindow().getDefaultView());

    m_state = RUNNING;

    m_world->init();

    m_world->load("test");
  }

  void GameStateGame::handleInput() {
    sf::Event windowEvent;
    while (m_windowHandler->getWindow().pollEvent(windowEvent)) {
      m_world->handleInput(windowEvent);
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

  bool GameStateGame::update() {
    if (m_world->update())
      m_state = QUIT;
    switch (m_state) {
      case RUNNING:
        break;
      case QUIT:
        return false;
      case RELOAD:
        m_game.changeState(new GameStateInit(m_game));
        break;
    }
    return true;
  }

  void GameStateGame::draw(const double dt) {
    m_windowHandler->getWindow().clear(sf::Color::Black);
    m_world->draw(dt);
    m_windowHandler->getWindow().display();
  }
}