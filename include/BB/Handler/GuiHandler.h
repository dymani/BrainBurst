#ifndef GUI_HANDLER_H
#define GUI_HANDLER_H

#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include <memory>
#include "BB/Handler/Gui/IGuiElement.h"

namespace bb {
  class WindowHandler;
  class ResourceHandler;

  class GuiHandler {
  public:
    GuiHandler(WindowHandler& windowHandler, ResourceHandler& resourceHandler);
    void load(luabridge::LuaRef& luaGui);
    void handleInput(sf::Event& windowEvent);
    int update();
    void draw();
  private:
    WindowHandler& m_windowHandler;
    ResourceHandler& m_resourceHandler;
    std::map<int, std::unique_ptr<IGuiElement>> m_elements;
  };
}

#endif