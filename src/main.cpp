#include <windows.h>
#include "BB/Game.h"
#include "BB/GameState/GameStateInit.h"

int main() {
#if _DEBUG
  HWND hWnd = GetConsoleWindow();
  ShowWindow(hWnd, SW_SHOW);
#else
  HWND hWnd = GetConsoleWindow();
  ShowWindow(hWnd, SW_HIDE);
#endif
  bb::Game game;
  game.pushState(new bb::GameStateInit{game});
  int returnVal = game.run();
#if _DEBUG
  system("PAUSE");
#endif
  return returnVal;
}