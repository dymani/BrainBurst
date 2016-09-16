#include "BB/Game.h"
#include "BB/GameState/IGameState.h"

namespace bb {
  Game::Game() {

  }

  Game::~Game() {
    while (!m_states.empty()) popState();
  }

  int Game::run() {
    clock_t previous, lag, current, elapsed;
    previous = clock();
    lag = 0;
    bool isRunning = true;
    while (isRunning) {
      current = clock();
      elapsed = current - previous;
      previous = current;
      lag += elapsed;

      assert(peekState() != nullptr);

      peekState()->handleInput();

      while (lag >= MS_PER_UPDATE) {
        isRunning = peekState()->update();
        lag -= MS_PER_UPDATE;
      }

      peekState()->draw(double(lag) / double(MS_PER_UPDATE));
    }
    while (!m_states.empty())
      popState();
    return 0;
  }

  void Game::pushState(IGameState* state) {
    m_states.push(state);
    return;
  }

  void Game::popState() {
    if (m_states.empty()) return;
    delete m_states.top();
    m_states.pop();
    return;
  }

  void Game::changeState(IGameState* state) {
    if (!m_states.empty()) {
      popState();
    }
    pushState(state);
    return;
  }

  IGameState* Game::peekState() {
    if (m_states.empty()) return nullptr;
    return m_states.top();
  }
}