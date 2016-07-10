#ifndef WINDOW_HANDLER_H
#define WINDOW_HANDLER_H

#include <SFML/Graphics.hpp>
#include <cassert>

namespace bb {
  class WindowHandler {
  public:
    WindowHandler();
    void createWindow(sf::VideoMode videoMode, const std::string& title, sf::Uint32 style = sf::Style::Default,
      const sf::ContextSettings& settings = sf::ContextSettings());
    sf::RenderWindow& getWindow();
  private:
    sf::RenderWindow m_window;
  };
}

#endif