#include "BB/Handler/WindowHandler.h"

namespace bb {
  WindowHandler::WindowHandler() {
  }

  void WindowHandler::createWindow(sf::VideoMode videoMode, const std::string& title, sf::Uint32 style,
    const sf::ContextSettings& settings) {
    m_window.create(videoMode, title, style, settings);
  }

  sf::RenderWindow& WindowHandler::getWindow() {
    assert(m_window.isOpen());
    return m_window;
  }
}