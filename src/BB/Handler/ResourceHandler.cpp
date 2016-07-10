#include "BB/Handler/ResourceHandler.h"

namespace bb {
  ResourceHandler::ResourceHandler(Game& game) : m_game(game) {

  }

  void ResourceHandler::load(luabridge::lua_State* L) {
    using namespace luabridge;

    luaL_openlibs(L);
    assert(!luaL_loadfile(L, "assets/data/resource.lua"));
    assert(!lua_pcall(L, 0, 0, 0));

    luabridge::LuaRef luaTextures = getGlobal(L, "textures");
    assert(luaTextures.isTable());

    std::string name, file;
    for (int i = 1; i <= luaTextures.length(); i++) {
      luabridge::LuaRef luaTexture = luaTextures[i];
      if (!luaTexture.isTable()) {
        LogHandler::log<ResourceHandler>("Incorrect data format for texture[" + std::to_string(i) + "]", ERR);
        assert(false);
      }

      luabridge::LuaRef luaName = luaTexture["name"];
      luabridge::LuaRef luaFile = luaTexture["file"];
      if (!luaName.isString() || !luaFile.isString()) {
        LogHandler::log<ResourceHandler>("Incorrect data format for texture[" + std::to_string(i) + "]", ERR);
        assert(luaName.isString());
        assert(luaFile.isString());
        continue;
      }
      name = luaName.cast<std::string>();
      file = luaFile.cast<std::string>();
      m_texturesLoading[name] = file;
    }

    luabridge::LuaRef luaSounds = getGlobal(L, "sounds");
    assert(luaSounds.isTable());

    for (int i = 1; i <= luaSounds.length(); i++) {
      luabridge::LuaRef luaSound = luaSounds[i];
      if (!luaSound.isTable()) {
        LogHandler::log<ResourceHandler>("Incorrect data format for sound[" + std::to_string(i) + "]", ERR);
        assert(false);
        continue;
      }

      luabridge::LuaRef luaName = luaSound["name"];
      luabridge::LuaRef luaFile = luaSound["file"];
      if (!luaName.isString() || !luaFile.isString()) {
        LogHandler::log<ResourceHandler>("Incorrect data format for sound[" + std::to_string(i) + "]", ERR);
        assert(luaName.isString());
        assert(luaFile.isString());
        continue;
      }
      name = luaName.cast<std::string>();
      file = luaFile.cast<std::string>();
      m_soundBuffersLoading[name] = file;
    }

    luabridge::LuaRef luaFonts = getGlobal(L, "fonts");
    assert(luaFonts.isTable());

    for (int i = 1; i <= luaFonts.length(); i++) {
      luabridge::LuaRef luaFont = luaFonts[i];
      if (!luaFont.isTable()) {
        LogHandler::log<ResourceHandler>("Incorrect data format for font[" + std::to_string(i) + "]", ERR);
        assert(false);
        continue;
      }
      luabridge::LuaRef luaName = luaFont["name"];
      luabridge::LuaRef luaFile = luaFont["file"];
      if (!luaName.isString() || !luaFile.isString()) {
        LogHandler::log<ResourceHandler>("Incorrect data format for font[" + std::to_string(i) + "]", ERR);
        assert(luaName.isString());
        assert(luaFile.isString());
        continue;
      }
      name = luaName.cast<std::string>();
      file = luaFile.cast<std::string>();
      m_fontsLoading[name] = file;
    }
  }

  sf::Texture& ResourceHandler::getTexture(std::string name) {
    auto textureIt = m_texturesLoading.find(name);
    if (textureIt != m_texturesLoading.end()) {
      sf::Texture texture;
      if (!texture.loadFromFile("assets/textures/" + m_texturesLoading[name])) {
        LogHandler::log<ResourceHandler>("Texture \"" + m_texturesLoading[name] + "\" not found", ERR);
        assert(false);
        return m_textures["NULL"];
      }
      m_textures[name] = texture;
      m_textures[name].setSmooth(false);
      m_textures[name].setRepeated(true);
      m_texturesLoading.erase(textureIt);
    }
    else {
      auto it = m_textures.find(name);
      if (it == m_textures.end()) {
        LogHandler::log<ResourceHandler>("Texture \"" + name + "\" not found", ERR);
        assert(false);
        return m_textures["NULL"];
      }
    }
    return m_textures[name];
  }

  sf::SoundBuffer& ResourceHandler::getSoundBuffer(std::string name) {
    auto soundBufferIt = m_soundBuffersLoading.find(name);
    if (soundBufferIt != m_soundBuffersLoading.end()) {
      sf::SoundBuffer soundBuffer;
      if (!soundBuffer.loadFromFile("assets/sounds/" + m_soundBuffersLoading[name])) {
        LogHandler::log<ResourceHandler>("Sound \"" + m_soundBuffersLoading[name] + "\" not found", ERR);
        assert(false);
        return m_soundBuffers["NULL"];
      }
      m_soundBuffers[name] = soundBuffer;
      m_soundBuffersLoading.erase(soundBufferIt);
      return m_soundBuffers[name];
    }
    else {
      auto it = m_soundBuffers.find(name);
      if (it == m_soundBuffers.end()) {
        LogHandler::log<ResourceHandler>("Sound \"" + name + "\" not found", ERR);
        assert(false);
        return m_soundBuffers["NULL"];
      }
      return m_soundBuffers[name];
    }
  }

  sf::Font& ResourceHandler::getFont(std::string name) {
    auto fontIt = m_fontsLoading.find(name);
    if (fontIt != m_fontsLoading.end()) {
      sf::Font font;
      if (!font.loadFromFile("assets/fonts/" + m_fontsLoading[name])) {
        LogHandler::log<ResourceHandler>("Font \"" + m_fontsLoading[name] + "\" not found", ERR);
        assert(false);
        return m_fonts["NULL"];
      }
      m_fonts[name] = font;
      m_fontsLoading.erase(fontIt);
      return m_fonts[name];
    }
    else {
      auto it = m_fonts.find(name);
      if (it == m_fonts.end()) {
        LogHandler::log<ResourceHandler>("Font\"" + name + "\" not found", ERR);
        assert(false);
        return m_fonts["NULL"];
      }
      return m_fonts[name];
    }
  }

  bool ResourceHandler::load() {
    if (!m_texturesLoading.empty()) {
      auto& it = m_texturesLoading.begin();
      sf::Texture texture;
      if (!texture.loadFromFile("assets/textures/" + it->second)) {
        LogHandler::log<ResourceHandler>("Texture \"" + it->second + "\" not found", ERR);
        assert(false);
        return false;
      }
      texture.setSmooth(true);
      m_textures[it->first] = texture;
      m_texturesLoading.erase(it);
      return false;
    }
    if (!m_soundBuffersLoading.empty()) {
      auto& it = m_soundBuffersLoading.begin();
      sf::SoundBuffer soundBuffer;
      if (!soundBuffer.loadFromFile("assets/sounds/" + it->second)) {
        LogHandler::log<ResourceHandler>("Sound \"" + it->second + "\" not found", ERR);
        assert(false);
        return false;
      }
      m_soundBuffers[it->first] = soundBuffer;
      m_soundBuffersLoading.erase(it);
      return false;
    }
    if (!m_fontsLoading.empty()) {
      auto& it = m_fontsLoading.begin();
      sf::Font font;
      if (!font.loadFromFile("assets/fonts/" + it->second)) {
        LogHandler::log<ResourceHandler>("Font \"" + it->second + "\" not found", ERR);
        assert(false);
        return false;
      }
      m_fonts[it->first] = font;
      m_fontsLoading.erase(it);
      return false;
    }
    return true;
  }
}