#ifndef RESOURCE_HANDLER_H
#define RESOURCE_HANDLER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <memory>
#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include "BB/Handler/LogHandler.h"

namespace bb {
  class Game;

  class ResourceHandler {
  public:
    ResourceHandler(Game& game);
    void load(luabridge::lua_State* L);
    sf::Texture& getTexture(std::string name);
    sf::SoundBuffer& getSoundBuffer(std::string name);
    sf::Font& getFont(std::string name);
    bool load();
  private:
    Game& m_game;
    std::map<std::string, std::string> m_texturesLoading, m_soundBuffersLoading, m_fontsLoading;
    std::map<std::string, sf::Texture> m_textures;
    std::map<std::string, sf::SoundBuffer> m_soundBuffers;
    std::map<std::string, sf::Font> m_fonts;
  };
}

#endif