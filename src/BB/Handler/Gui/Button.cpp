#include "BB/Handler/Gui/Button.h"
#include "BB/Handler/ResourceHandler.h"
#include "BB/Handler/WindowHandler.h"

namespace bb {
    Button::Button(ResourceHandler& resourceHandler, WindowHandler& windowHandler
        , luabridge::LuaRef& luaElement) : m_resourceHandler(resourceHandler) {
        using namespace luabridge;
        if(!luaElement.isTable()) return;
        for(int i = 1; i <= luaElement.length(); i++) {
            LuaRef luaData = luaElement[i];
            LuaRef luaId = luaData["id"];
            LuaRef luaPosition = luaData["position"];
            LuaRef luaSize = luaData["size"];
            LuaRef luaTexture = luaData["texture"];
            LuaRef luaTextureRect = luaData["textureRect"];
            LuaRef luaText = luaData["text"];
            LuaRef luaFont = luaData["font"];
            auto* data = new ButtonData();
            data->m_position = {luaPosition[1].cast<int>(), luaPosition[2].cast<int>()};
            data->m_size = {luaSize[1].cast<int>(), luaSize[2].cast<int>()};
            data->m_sprite.setTexture(m_resourceHandler.getTexture(luaTexture.cast<std::string>()));
            sf::IntRect rect = {luaTextureRect[1].cast<int>(), luaTextureRect[2].cast<int>(),
                luaTextureRect[3].cast<int>(), luaTextureRect[4].cast<int>()};
            data->m_textureRect[0] = {rect.left, rect.top, rect.width / 3, rect.height};
            data->m_textureRect[1] = {rect.width / 3, rect.top, rect.width / 3, rect.height};
            data->m_textureRect[2] = {rect.width / 3 * 2, rect.top, rect.width / 3, rect.height};
            data->m_sprite.setTextureRect(data->m_textureRect[0]);
            float scale = float(windowHandler.getWindow().getSize().x) / 1920.0F;
            data->m_sprite.setPosition(sf::Vector2f(data->m_position) * scale);
            data->m_sprite.setScale({float(data->m_size.x) / data->m_textureRect->width * scale,
                float(data->m_size.y) / data->m_textureRect->height * scale});
            data->m_text.setFont(m_resourceHandler.getFont(luaFont.cast<std::string>()));
            data->m_text.setString(luaText.cast<std::string>());
            data->m_text.setCharacterSize(int(luaSize[2].cast<int>() * 0.7F * scale));
            data->m_text.setPosition({data->m_sprite.getPosition().x +
                (data->m_sprite.getGlobalBounds().width - data->m_text.getGlobalBounds().width) / 2, data->m_sprite.getPosition().y + (data->m_sprite.getGlobalBounds().height
                    - data->m_text.getCharacterSize()) / 4});
            data->m_state = ButtonData::IDLE;
            m_data[luaId.cast<int>()] = std::unique_ptr<IGuiElementData>(data);
        }
    }

    bool Button::handleInput(sf::RenderWindow& window, sf::Event& windowEvent) {
        int mouseX = sf::Mouse::getPosition(window).x;
        int mouseY = sf::Mouse::getPosition(window).y;
        bool isLeftPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        for(auto& data : m_data) {
            auto& button = *dynamic_cast<ButtonData*>(data.second.get());
            bool isHover = mouseX > button.m_sprite.getPosition().x
                && mouseX < button.m_sprite.getPosition().x + button.m_sprite.getGlobalBounds().width
                && mouseY > button.m_sprite.getPosition().y
                && mouseY < button.m_sprite.getPosition().y + button.m_sprite.getGlobalBounds().height;
            if(isHover) {
                if(isLeftPressed) {
                    if(button.m_state == ButtonData::HOVER || button.m_state == ButtonData::LEFT) {
                        button.m_state = ButtonData::PRESSED;
                        button.m_sprite.setTextureRect(button.m_textureRect[2]);

                    }
                } else {
                    if(button.m_state == ButtonData::IDLE || button.m_state == ButtonData::LEFT) {
                        button.m_state = ButtonData::HOVER;
                        button.m_sprite.setTextureRect(button.m_textureRect[1]);
                    } else if(button.m_state == ButtonData::PRESSED) {
                        button.m_state = ButtonData::RELEASED;
                        button.m_sprite.setTextureRect(button.m_textureRect[1]);
                    }
                }
                return true;
            } else {
                if(isLeftPressed) {
                    if(button.m_state == ButtonData::HOVER) {
                        button.m_state = ButtonData::IDLE;
                        button.m_sprite.setTextureRect(button.m_textureRect[0]);
                    } else if(button.m_state == ButtonData::PRESSED) {
                        button.m_state = ButtonData::LEFT;
                        button.m_sprite.setTextureRect(button.m_textureRect[0]);

                    }
                } else {
                    if(button.m_state == ButtonData::HOVER || button.m_state == ButtonData::LEFT) {
                        button.m_state = ButtonData::IDLE;
                        button.m_sprite.setTextureRect(button.m_textureRect[0]);
                    }
                }
            }
        }
        return false;
    }

    int Button::update() {
        for(auto& data : m_data) {
            auto* button = dynamic_cast<ButtonData*>(data.second.get());
            if(button->m_state == ButtonData::RELEASED) {
                button->m_state = ButtonData::HOVER;
                return data.first;
            }
        }
        return -1;
    }

    void Button::draw(sf::RenderWindow& window) {
        for(auto& data : m_data) {
            auto* button = dynamic_cast<ButtonData*>(data.second.get());
            window.draw(button->m_sprite);
            window.draw(button->m_text);
        }
    }
}