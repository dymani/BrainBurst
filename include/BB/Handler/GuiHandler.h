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
#include "BB/Handler/Gui/IGuiElementHandler.h"

namespace bb {
  class WindowHandler;
  class ResourceHandler;

  class GuiHandler {
  public:
    GuiHandler(WindowHandler& windowHandler, ResourceHandler& resourceHandler, luabridge::lua_State* L);
    void load();
    void loadElements(luabridge::LuaRef& luaElements);
    void handleInput(sf::Event& windowEvent);
    int update();
    void draw();
  private:
    WindowHandler& m_windowHandler;
    ResourceHandler& m_resourceHandler;
    luabridge::lua_State* L;
    std::map<std::string, std::unique_ptr<IGuiElementHandler>> m_elementHandlers;
    std::map<int, std::unique_ptr<IGuiElement>> m_elements;
  };
}

#endif