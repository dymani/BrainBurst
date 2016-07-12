#include "BB/Handler/Gui/Button.h"
#include "BB/Handler/ResourceHandler.h"
#include "BB/Handler/WindowHandler.h"

namespace bb {
  ButtonHandler::ButtonHandler(ResourceHandler& resourceHandler, WindowHandler& windowHandler) :
    IGuiElementHandler(resourceHandler, windowHandler) {
  }

  void ButtonHandler::addElementStyle(luabridge::LuaRef& luaElementStyle) {
    luabridge::LuaRef luaName = luaElementStyle["name"];
    luabridge::LuaRef luaSize = luaElementStyle["size"];
    luabridge::LuaRef luaTexture = luaElementStyle["texture"];
    luabridge::LuaRef luaTextureRect = luaElementStyle["textureRect"];
    luabridge::LuaRef luaFont = luaElementStyle["font"];
    assert(luaName.isString());
    assert(luaSize.isTable());
    assert(luaSize[1].isNumber());
    assert(luaSize[2].isNumber());
    assert(luaTexture.isString());
    assert(luaTextureRect.isTable());
    assert(luaTextureRect[1].isNumber());
    assert(luaTextureRect[2].isNumber());
    assert(luaTextureRect[3].isNumber());
    assert(luaTextureRect[4].isNumber());
    assert(luaFont.isString());
    auto* buttonStyle = new Button();
    buttonStyle->m_isStyle = true;
    buttonStyle->m_size = {luaSize[1].cast<int>(), luaSize[2].cast<int>()};
    buttonStyle->m_sprite.setTexture(m_resourceHandler.getTexture(luaTexture.cast<std::string>()));
    sf::IntRect rect = {luaTextureRect[1].cast<int>(), luaTextureRect[2].cast<int>(),
      luaTextureRect[3].cast<int>(), luaTextureRect[4].cast<int>()};
    buttonStyle->m_textureRect[0] = {rect.left, rect.top, rect.width / 3, rect.height};
    buttonStyle->m_textureRect[1] = {rect.width / 3, rect.top, rect.width / 3, rect.height};
    buttonStyle->m_textureRect[2] = {rect.width / 3 * 2, rect.top, rect.width / 3, rect.height};
    buttonStyle->m_sprite.setTextureRect(buttonStyle->m_textureRect[0]);
    float scale = float(m_windowHandler.getWindow().getSize().x) / 1920.0F;
    buttonStyle->m_sprite.setScale({float(buttonStyle->m_size.x) / buttonStyle->m_textureRect->width * scale,
      float(buttonStyle->m_size.y) / buttonStyle->m_textureRect->height * scale});
    buttonStyle->m_text.setFont(m_resourceHandler.getFont(luaFont.cast<std::string>()));
    buttonStyle->m_text.setCharacterSize(int(luaSize[2].cast<int>() * 0.7F * scale));
    m_styles[luaName.cast<std::string>()] = std::unique_ptr<IGuiElement>(buttonStyle);
  }

  IGuiElement* ButtonHandler::addElement(luabridge::LuaRef& luaElement) {
    assert(luaElement.isTable());
    luabridge::LuaRef luaType = luaElement["type"];
    luabridge::LuaRef luaStyle = luaElement["style"];
    luabridge::LuaRef luaPosition = luaElement["position"];
    luabridge::LuaRef luaText = luaElement["text"];
    assert(luaType.isString());
    assert(luaType.cast<std::string>() == "button");
    assert(luaStyle.isString());
    assert(luaPosition.isTable());
    assert(luaPosition[1].isNumber());
    assert(luaPosition[2].isNumber());
    assert(luaText.isString());
    auto* style = dynamic_cast<Button*>(m_styles[luaStyle.cast<std::string>()].get());
    Button* button = new Button(*style);
    button->m_isStyle = false;
    button->m_type = luaType.cast<std::string>();
    button->m_position = {luaPosition[1].cast<int>(), luaPosition[2].cast<int>()};
    float scale = float(m_windowHandler.getWindow().getSize().x) / 1920.0F;
    button->m_sprite.setPosition(sf::Vector2f(button->m_position) * scale);
    button->m_text.setString(luaText.cast<std::string>());
    button->m_text.setPosition({button->m_sprite.getPosition().x + (button->m_sprite.getGlobalBounds().width
      - button->m_text.getGlobalBounds().width) / 2,
      button->m_sprite.getPosition().y + (button->m_sprite.getGlobalBounds().height
        - button->m_text.getCharacterSize()) / 4});
    button->m_state = Button::IDLE;
    return button;
  }

  bool ButtonHandler::handleInput(sf::Event& windowEvent, IGuiElement* element) {
    auto& window = m_windowHandler.getWindow();
    int mouseX = sf::Mouse::getPosition(window).x;
    int mouseY = sf::Mouse::getPosition(window).y;
    bool isLeftPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    auto* button = dynamic_cast<Button*>(element);
    bool isHover = mouseX > button->m_sprite.getPosition().x
      && mouseX < button->m_sprite.getPosition().x + button->m_sprite.getGlobalBounds().width
      && mouseY > button->m_sprite.getPosition().y
      && mouseY < button->m_sprite.getPosition().y + button->m_sprite.getGlobalBounds().height;
    if (isHover) {
      if (isLeftPressed) {
        if (button->m_state == Button::HOVER || button->m_state == Button::LEFT) {
          button->m_state = Button::PRESSED;
          button->m_sprite.setTextureRect(button->m_textureRect[2]);
        }
      }
      else {
        if (button->m_state == Button::IDLE || button->m_state == Button::LEFT) {
          button->m_state = Button::HOVER;
          button->m_sprite.setTextureRect(button->m_textureRect[1]);
        }
        else if (button->m_state == Button::PRESSED) {
          button->m_state = Button::RELEASED;
          button->m_sprite.setTextureRect(button->m_textureRect[1]);
        }
      }
      return true;
    }
    else {
      if (isLeftPressed) {
        if (button->m_state == Button::HOVER) {
          button->m_state = Button::IDLE;
          button->m_sprite.setTextureRect(button->m_textureRect[0]);
        }
        else if (button->m_state == Button::PRESSED) {
          button->m_state = Button::LEFT;
          button->m_sprite.setTextureRect(button->m_textureRect[0]);

        }
      }
      else {
        if (button->m_state == Button::HOVER || button->m_state == Button::LEFT) {
          button->m_state = Button::IDLE;
          button->m_sprite.setTextureRect(button->m_textureRect[0]);
        }
      }
    }
    return false;
  }

  bool ButtonHandler::update(IGuiElement* element) {
    auto* button = dynamic_cast<Button*>(element);
    if (button->m_state == Button::RELEASED) {
      button->m_state = Button::HOVER;
      return true;
    }
    return false;
  }

  void ButtonHandler::draw(IGuiElement* element) {
    auto* button = dynamic_cast<Button*>(element);
    m_windowHandler.getWindow().draw(button->m_sprite);
    m_windowHandler.getWindow().draw(button->m_text);
  }
}