#include "BB/GameState/GameStateTitle.h"
#include "BB/Game.h"
#include "BB/GameState/GameStateGame.h"
#include "BB/GameState/GameStateInit.h"
#include "BB/Handler/Gui/Button.h"

namespace bb {
  GameStateTitle::GameStateTitle(Game& game, ResourceHandler* resourceHandler, WindowHandler* windowHandler,
    luabridge::lua_State* L) : IGameState(game),
    m_resourceHandler(std::unique_ptr<ResourceHandler>(resourceHandler)),
    m_windowHandler(std::unique_ptr<WindowHandler>(windowHandler)) {
    this->L = L;

    m_windowHandler->getWindow().setView(m_windowHandler->getWindow().getDefaultView());
    m_guiHandler = std::unique_ptr<GuiHandler>(new GuiHandler(*m_windowHandler.get(),
      *m_resourceHandler.get(), L));
    m_guiHandler->load();

    m_state = RUNNING;

    using namespace luabridge;

    assert(!luaL_loadfile(L, "assets/data/gameStates/title.lua") && !lua_pcall(L, 0, 0, 0));

    luabridge::LuaRef luaBackground = getGlobal(L, "background");
    assert(luaBackground.isString());
    m_background.setTexture(m_resourceHandler->getTexture(luaBackground.cast<std::string>()));
    m_background.setScale(
      float(m_windowHandler->getWindow().getSize().x) / float(m_background.getTexture()->getSize().x),
      float(m_windowHandler->getWindow().getSize().y) / float(m_background.getTexture()->getSize().y));

    luabridge::LuaRef luaText = getGlobal(L, "text");
    assert(luaText.isTable());
    luabridge::LuaRef luaString = luaText["string"];
    luabridge::LuaRef luaSize = luaText["size"];
    luabridge::LuaRef luaColor = luaText["color"];
    luabridge::LuaRef luaFont = luaText["font"];
    assert(luaString.isString());
    assert(luaSize.isNumber());
    assert(luaColor.isTable());
    assert(luaColor[1].isNumber());
    assert(luaColor[2].isNumber());
    assert(luaColor[3].isNumber());
    assert(luaFont.isString());
    m_text.setString(luaString.cast<std::string>());
    auto size = unsigned int(luaSize.cast<float>() * m_background.getScale().y);
    m_text.setCharacterSize(size);
    m_text.setColor({luaColor[1].cast<sf::Uint8>(), luaColor[2].cast<sf::Uint8>(),
      luaColor[3].cast<sf::Uint8>()});
    m_text.setFont(m_resourceHandler->getFont(luaFont.cast<std::string>()));
    m_text.setPosition({float(size) / 2.0f, m_windowHandler->getWindow().getSize().y - float(size) / 2.0f
      - m_text.getGlobalBounds().height});

    luabridge::LuaRef luaGui = getGlobal(L, "gui");
    m_guiHandler->loadElements(luaGui);
  }

  void GameStateTitle::handleInput() {
    sf::Event windowEvent;
    while (m_windowHandler->getWindow().pollEvent(windowEvent)) {
      m_guiHandler->handleInput(windowEvent);
      if (windowEvent.type == sf::Event::Closed) {
        m_state = QUIT;
        return;
      }
      else if (windowEvent.type == sf::Event::KeyPressed) {
        switch (windowEvent.key.code) {
          case sf::Keyboard::Return:
            m_state = GAME;
            break;
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

  bool GameStateTitle::update() {
    int i = m_guiHandler->update();
    if (i != -1) {
      m_state = State(i);
    }
    switch (m_state) {
      case RUNNING:
        break;
      case QUIT:
        return false;
      case GAME:
        m_game.changeState(new GameStateGame(m_game, m_resourceHandler.release(),
          m_windowHandler.release(), L));
        break;
      case RELOAD:
        m_game.changeState(new GameStateInit(m_game));
        break;
    }
    return true;
  }

  void GameStateTitle::draw(const double dt) {
    m_windowHandler->getWindow().clear(sf::Color::Black);
    m_windowHandler->getWindow().draw(m_background);
    m_windowHandler->getWindow().draw(m_text);
    m_guiHandler->draw();
    m_windowHandler->getWindow().display();
  }
}