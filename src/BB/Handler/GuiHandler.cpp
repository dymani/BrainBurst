#include "BB/Handler/GuiHandler.h"
#include "BB/Handler/WindowHandler.h"
#include "BB/Handler/ResourceHandler.h"
#include "BB/Handler/Gui/Button.h"

namespace bb {
  GuiHandler::GuiHandler(WindowHandler& windowHandler, ResourceHandler& resourceHandler,
    luabridge::lua_State* L) : m_windowHandler(windowHandler), m_resourceHandler(resourceHandler) {
    this->L = L;

    m_elementHandlers["button"] = std::unique_ptr<ButtonHandler>(new ButtonHandler(m_resourceHandler,
      m_windowHandler));
  }

  void GuiHandler::load() {
    using namespace luabridge;
    assert(!luaL_loadfile(L, "assets/data/gui.lua") && !lua_pcall(L, 0, 0, 0));

    luabridge::LuaRef luaButtons = getGlobal(L, "buttons");
    assert(luaButtons.isTable());

    for (int i = 1; i <= luaButtons.length(); i++) {
      luabridge::LuaRef luaButton = luaButtons[i];
      assert(luaButton.isTable());
      m_elementHandlers["button"]->addElementStyle(luaButton);
    }
  }

  void GuiHandler::loadElements(luabridge::LuaRef& luaElements) {
    assert(luaElements.isTable());
    for (int i = 1; i <= luaElements.length(); i++) {
      luabridge::LuaRef luaElement = luaElements[i];
      assert(luaElement.isTable());
      luabridge::LuaRef luaId = luaElement["id"];
      luabridge::LuaRef luaType = luaElement["type"];
      assert(luaId.isNumber());
      assert(luaType.isString());
      auto* element = m_elementHandlers[luaType.cast<std::string>()]->addElement(luaElement);
      m_elements[luaId.cast<int>()] = std::unique_ptr<IGuiElement>(element);
    }
  }

  void GuiHandler::handleInput(sf::Event& windowEvent) {
    bool isActive;
    for (auto& e : m_elements) {
      isActive = m_elementHandlers[e.second->m_type]->handleInput(windowEvent, e.second.get());
      if (isActive)
        break;
    }
  }

  int GuiHandler::update() {
    bool isActive;
    for (auto& e : m_elements) {
      isActive = m_elementHandlers[e.second->m_type]->update(e.second.get());
      if (isActive)
        return e.first;
    }
    return -1;
  }

  void GuiHandler::draw() {
    for (auto& e : m_elements) {
      m_elementHandlers[e.second->m_type]->draw(e.second.get());
    }
  }
}