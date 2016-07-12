#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include "BB/Handler/Gui/IGuiElementHandler.h"
#include "BB/Handler/Gui/IGuiElement.h"

namespace bb {
  class ResourceHandler;
  class WindowHandler;

  class ButtonHandler : public IGuiElementHandler {
    friend class GuiHandler;
  public:
    ButtonHandler(ResourceHandler& resourceHandler, WindowHandler& windowHandler);
    void addElementStyle(luabridge::LuaRef& luaElementStyle);
    IGuiElement* addElement(luabridge::LuaRef& luaElement);
    bool handleInput(sf::Event& windowEvent, IGuiElement* element);
    bool update(IGuiElement* element);
    void draw(IGuiElement* element);
  };

  class Button : public IGuiElement {
    friend class ButtonHandler;
    friend class GuiHandler;
  private:
    bool m_isStyle = false;
    sf::Vector2i m_position;
    sf::Vector2i m_size;
    sf::Sprite m_sprite;
    sf::IntRect m_textureRect[3];
    sf::Text m_text;
    enum ButtonState {
      IDLE, HOVER, PRESSED, LEFT, RELEASED
    } m_state;
  };
}

#endif