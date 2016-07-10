#include "BB/Game.h"
#include "BB/GameState/GameStateInit.h"

int main() {
  bb::Game game;
  game.pushState(new bb::GameStateInit{game});
  int returnVal = game.run();
#if _DEBUG
  system("PAUSE");
#endif
  return returnVal;
}