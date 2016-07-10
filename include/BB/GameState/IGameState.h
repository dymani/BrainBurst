#ifndef I_GAME_STATE_H
#define I_GAME_STATE_H

#include "BB/Game.h"
#include <SFML/Graphics.hpp>

namespace bb {
    class IGameState {
    public:
        IGameState(Game& game):m_game(game) {
        }
        virtual ~IGameState() {
        }
        virtual void handleInput() = 0;
        virtual bool update() = 0;
        virtual void draw(const double dt) = 0;
    protected:
        Game& m_game;
    };
}

#endif