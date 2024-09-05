#pragma once

#include <SDL.h>

#include <vector>

#include "Globals.h"
#include "MinesweeperCell.h"
#include "Random.h"

class MinesweeperGrid {
 public:
  MinesweeperGrid(int x, int y) {
    CreateGrid(x, y);
    PlaceBombs();
  }

  void Render(SDL_Surface* TargetSurface) {
    for (auto& Child : Children) {
      Child.Render(TargetSurface);
    }
  }
  void HandleEvent(SDL_Event const& E) {
    if (E.type == UserEvents::CELL_CLEARED) {
      HandleCellCleared(E.user);
    } else if (E.type == UserEvents::NEW_GAME) {
      for (auto& Child : Children) {
        Child.Reset();
      }
      PlaceBombs();
    }
    for (auto& Child : Children) {
      Child.HandleEvent(E);
    }
  }

 private:
  void PlaceBombs() {
    int numOfBombsLeft = Config::BOMBS_COUNT;
    numOfCellsToBeCleared =
        Config::GRID_ROWS * Config::GRID_COLUMNS - Config::BOMBS_COUNT;
    while (numOfBombsLeft > 0) {
      size_t randomIndex = Engine::Random::Int(0, Children.size() - 1);
      if (Children[randomIndex].PlaceBomb()) {
        --numOfBombsLeft;
      }
    }
  }

  void CreateGrid(int x, int y) {
    using namespace Config;
    Children.reserve(GRID_COLUMNS * GRID_ROWS);
    for (int Col{1}; Col <= GRID_COLUMNS; ++Col) {
      for (int Row{1}; Row <= GRID_ROWS; ++Row) {
        constexpr int Spacing{PADDING + CELL_SIZE};
        int xpos = x + (Col - 1) * Spacing;
        int ypos = y + (Row - 1) * Spacing;
        Children.emplace_back(xpos, ypos, CELL_SIZE, CELL_SIZE, Col, Row);
      }
    }
  }

  void HandleCellCleared(const SDL_UserEvent& Event) {
    auto* Cell{static_cast<MinesweeperCell*>(Event.data1)};

    if (Cell->getHasBomb()) {
      SDL_Event Event{UserEvents::GAME_LOST};
#ifdef SHOW_DEBUG_HELPERS
      std::cout << "Game lost";
#endif
      SDL_PushEvent(&Event);
    } else {
      --numOfCellsToBeCleared;
#ifdef SHOW_DEBUG_HELPERS
      std::cout << numOfCellsToBeCleared << "\n";
#endif
      if (numOfCellsToBeCleared == 0) {
        SDL_Event Event{UserEvents::GAME_WON};
#ifdef SHOW_DEBUG_HELPERS
        std::cout << "Game won";
#endif
        SDL_PushEvent(&Event);
      }
    }
  }

  std::vector<MinesweeperCell> Children;
  int numOfCellsToBeCleared{};
};