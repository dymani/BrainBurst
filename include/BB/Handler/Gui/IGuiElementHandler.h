#ifndef I_GUI_ELEMENT_HANDLER_H
#define I_GUI_ELEMENT_HANDLER_H

#include <memory>
#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include <SFML/Graphics.hpp>
#include "BB/Handler/Gui/IGuiElement.h"

namespace bb {
  class ResourceHandler;
  class WindowHandler;

  class IGuiElementHandler {
  public:
    IGuiElementHandler(ResourceHandler& resourceHandler, WindowHandler& windowHandler)
    : m_resourceHandler(resourceHandler), m_windowHandler(windowHandler) {
    }
    virtual ~IGuiElementHandler() {
    }
    virtual void addElementStyle(luabridge::LuaRef& luaElementStyle) = 0;
    virtual IGuiElement* addElement(luabridge::LuaRef& luaElement) = 0;
    virtual bool handleInput(sf::Event& windowEvent, IGuiElement* element) = 0;
    virtual bool update(IGuiElement* element) = 0;
    virtual void draw(IGuiElement* element) = 0;
  protected:
    ResourceHandler& m_resourceHandler;
    WindowHandler& m_windowHandler;
    std::map<std::string, std::unique_ptr<IGuiElement>> m_styles;
  };
}

#endif