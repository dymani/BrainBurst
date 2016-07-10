#include "BB/GameState/GameStateInit.h"
#include "BB/Game.h"
#include "BB/GameState/GameStateSplash.h"

#if defined (SFML_SYSTEM_WINDOWS)
#include <windows.h>
bool setShape(HWND hWnd, const sf::Image& image) {
  const sf::Uint8* pixelData = image.getPixelsPtr();
  HRGN hRegion = CreateRectRgn(0, 0, image.getSize().x, image.getSize().y);
  for (unsigned int y = 0; y < image.getSize().y; y++) {
    for (unsigned int x = 0; x < image.getSize().x; x++) {
      if (pixelData[y * image.getSize().x * 4 + x * 4 + 3] == 0) {
        HRGN hRegionDest = CreateRectRgn(0, 0, 1, 1);
        HRGN hRegionPixel = CreateRectRgn(x, y, x + 1, y + 1);
        CombineRgn(hRegionDest, hRegion, hRegionPixel, RGN_XOR);
        DeleteObject(hRegion);
        DeleteObject(hRegionPixel);
        hRegion = hRegionDest;
      }
    }
  }
  SetWindowRgn(hWnd, hRegion, true);
  DeleteObject(hRegion);
  return true;
}

bool setTransparency(HWND hWnd, unsigned char alpha) {
  SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
  SetLayeredWindowAttributes(hWnd, 0, alpha, LWA_ALPHA);
  return true;
}
#endif

namespace bb {
  GameStateInit::GameStateInit(Game& game) :IGameState(game) {
    m_windowHandler = std::unique_ptr<WindowHandler>{new WindowHandler{}};
    m_resourceHandler = std::unique_ptr<ResourceHandler>{new ResourceHandler{m_game}};
    m_state = RUNNING;
    m_updateCount = -50;

    using namespace luabridge;
    L = luaL_newstate();

    m_resourceHandler->load(L);

    assert(!luaL_loadfile(L, "assets/data/gameStates/init.lua"));
    assert(!lua_pcall(L, 0, 0, 0));

    luabridge::LuaRef luaTexture = getGlobal(L, "texture");
    assert(luaTexture.isString());
    m_logo.setTexture(m_resourceHandler->getTexture(luaTexture.cast<std::string>()));

    auto init = m_resourceHandler->getTexture(luaTexture.cast<std::string>()).copyToImage();
    m_windowHandler->createWindow(sf::VideoMode{init.getSize().x, init.getSize().y, 32},
      "Brain Burst 2039", sf::Style::None, sf::ContextSettings{});
    m_windowHandler->getWindow().setPosition(sf::Vector2i{int((sf::VideoMode::getDesktopMode().width
      - init.getSize().x) / 2), int((sf::VideoMode::getDesktopMode().height - init.getSize().y) / 2)});
    setShape(m_windowHandler->getWindow().getSystemHandle(), init);
    setTransparency(m_windowHandler->getWindow().getSystemHandle(), 200);
  }

  GameStateInit::~GameStateInit() {
  }

  void GameStateInit::handleInput() {
    sf::Event windowEvent;
    while (m_windowHandler->getWindow().pollEvent(windowEvent)) {
      if (windowEvent.type == sf::Event::KeyPressed) {
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

  bool GameStateInit::update() {
    m_updateCount++;
    switch (m_state) {
      case RUNNING:
        if (m_resourceHandler->load()) {
          LogHandler::log<GameStateInit>("Finished loading resources");
          m_state = NEXT;
        }
        break;
      case QUIT:
        return false;
      case NEXT:
        if (m_updateCount >= 0) {
          m_game.changeState(new GameStateSplash{m_game, m_resourceHandler.release(),
            m_windowHandler.release(), L});
        }
        break;
      case RELOAD:
        m_game.changeState(new GameStateInit{m_game});
        break;
    }
    return true;
  }

  void GameStateInit::draw(const double dt) {
    m_windowHandler->getWindow().clear(sf::Color::Transparent);
    m_windowHandler->getWindow().draw(m_logo);
    m_windowHandler->getWindow().display();
  }
}