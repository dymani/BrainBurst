#ifndef I_GUI_ELEMENT_H
#define I_GUI_ELEMENT_H

#include <SFML/Graphics.hpp>
#include "BB/Handler/Gui/IGuiElementData.h"

namespace bb {
    class IGuiElement {
    public:
        IGuiElement() {
        }
        virtual ~IGuiElement() {
        }
        virtual bool handleInput(sf::RenderWindow& window, sf::Event& windowEvent) = 0;
        virtual int update() = 0;
        virtual void draw(sf::RenderWindow& window) = 0;
    protected:
        std::map<int, std::unique_ptr<IGuiElementData>> m_data;
    };
}

#endif