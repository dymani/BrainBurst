#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include <memory>
#include "BB/Handler/Gui/IGuiElement.h"
#include "BB/Handler/Gui/IGuiElementData.h"

namespace bb {
    class ResourceHandler;
    class WindowHandler;

    class Button : public IGuiElement {
        friend class GuiHandler;
    public:
        Button(ResourceHandler& resourceHandler, WindowHandler& windowHandler, luabridge::LuaRef& luaElement);
        bool handleInput(sf::RenderWindow& window, sf::Event& windowEvent);
        int update();
        void draw(sf::RenderWindow& window);
    private:
        ResourceHandler& m_resourceHandler;
    };

    class ButtonData : public IGuiElementData {
        friend class Button;
    private:
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