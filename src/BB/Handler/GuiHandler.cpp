#include "BB/Handler/GuiHandler.h"
#include "BB/Handler/WindowHandler.h"
#include "BB/Handler/ResourceHandler.h"
#include "BB/Handler/Gui/Button.h"

namespace bb {
  GuiHandler::GuiHandler(WindowHandler& windowHandler, ResourceHandler& resourceHandler)
    : m_windowHandler(windowHandler), m_resourceHandler(resourceHandler) {
  }

  void GuiHandler::load(luabridge::LuaRef& luaGui) {
    using namespace luabridge;
    LuaRef luaButton = luaGui[1];
    Button* button = new Button(m_resourceHandler, m_windowHandler, luaButton);
    m_elements[0] = std::unique_ptr<IGuiElement>(button);
  }

  void GuiHandler::handleInput(sf::Event& windowEvent) {
    for (auto& e : m_elements) {
      if (e.second->handleInput(m_windowHandler.getWindow(), windowEvent))
        break;
    }
  }

  int GuiHandler::update() {
    int update;
    for (auto& e : m_elements) {
      update = e.second->update();
      if (update != -1)
        return update;
    }
    return -1;
  }

  void GuiHandler::draw() {
    for (auto& e : m_elements) {
      e.second->draw(m_windowHandler.getWindow());
    }
  }
}