// UI.h
#pragma once
#include <SDL.h>

#include "Globals.h"
#include "Grid.h"
#include "NewGameButton.h"
#include "FlagCounter.h"

class MinesweeperUI {
 public:
  MinesweeperUI() : Grid{Config::PADDING, Config::PADDING} {};

  void Render(SDL_Surface* Surface) {
    Grid.Render(Surface);
    Button.Render(Surface);
    FlagCounter.Render(Surface);
  }

  void HandleEvent(const SDL_Event& E) {
    Grid.HandleEvent(E);
    Button.HandleEvent(E);
    FlagCounter.HandleEvent(E);
  }

 private:
  MinesweeperGrid Grid;
  NewGameButton Button{
      Config::PADDING, Config::GRID_HEIGHT + Config::PADDING * 2,
      Config::WINDOW_WIDTH - Config::PADDING * 3 - Config::FLAG_COUNTER_WIDTH,
      Config::FOOTERHEIGHT - Config::PADDING};
  FlagCounter FlagCounter{
      Config::WINDOW_WIDTH - Config::PADDING - Config::FLAG_COUNTER_WIDTH,
      Config::GRID_HEIGHT + Config::PADDING * 2, Config::FLAG_COUNTER_WIDTH,
      Config::FOOTERHEIGHT - Config::PADDING};
};
