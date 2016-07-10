#ifndef GAME_H
#define GAME_H

#include <time.h>
#include <stack>
#include <windows.h>
#include <cassert>

namespace bb {
    class IGameState;

    class Game {
    public:
        static const int MS_PER_UPDATE = 20;

        Game();
        ~Game();
        int run();

        void pushState(IGameState* state);
        void popState();
        void changeState(IGameState* state);
        IGameState* peekState();
    private:
        std::stack<IGameState*> m_states;
    };
}

#endif