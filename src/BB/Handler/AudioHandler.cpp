#include "BB/Handler/AudioHandler.h"
#include "BB/Handler/ResourceHandler.h"

namespace bb {
  AudioHandler::AudioHandler(ResourceHandler& resourceHandler) : m_resourcehandler(resourceHandler) {
  }

  AudioHandler::~AudioHandler() {
    for (auto& sound : m_sounds) {
      sound.second->stop();
    }
    m_sounds.clear();
  }

  void AudioHandler::update() {
    if (m_sounds.empty())
      return;
    for (auto it = m_sounds.begin(); it != m_sounds.end();) {
      if (it->second->getStatus() == sf::SoundSource::Stopped) {
        it->second->stop();
        m_sounds.erase(it++);
        continue;
      }
      ++it;
    }
  }

  int AudioHandler::play(std::string name, float volume, bool loop) {
    sf::Sound* sound = new sf::Sound();
    sound->setBuffer(m_resourcehandler.getSoundBuffer(name));
    sound->setVolume(volume);
    sound->setLoop(loop);
    m_sounds[id] = std::unique_ptr<sf::Sound>(sound);
    m_sounds[id]->play();
    return id++;
  }

  void AudioHandler::togglePause(int id) {
    auto it = m_sounds.find(id);
    if (it != m_sounds.end()) {
      if (m_sounds[id]->getStatus() == sf::SoundSource::Playing)
        m_sounds[id]->pause();
      else if (m_sounds[id]->getStatus() == sf::SoundSource::Paused)
        m_sounds[id]->play();
    }
    else {
      LogHandler::log<AudioHandler>("Sound id " + std::to_string(id) + " not found", ERR);
      assert(false);
    }
  }

  void AudioHandler::stop(int id) {
    auto it = m_sounds.find(id);
    if (it != m_sounds.end()) {
      m_sounds[id]->stop();
      m_sounds.erase(id);
    }
    else {
      LogHandler::log<AudioHandler>("Sound id " + std::to_string(id) + " not found", ERR);
      assert(false);
    }
  }
}