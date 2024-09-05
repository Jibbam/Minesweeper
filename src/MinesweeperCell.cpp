#include "MinesweeperCell.h"

MinesweeperCell::MinesweeperCell(int x, int y, int w, int h, int COL, int ROW)
    : Button{x, y, w, h},
      COL{COL},
      ROW{ROW},
      BombImage{x, y, w, h, Config::BOMB_IMAGE},
      adjacentBombs{0},
      Text{x,
           y,
           w,
           h,
           std::to_string(adjacentBombs),
           Config::TEXT_COLORS[adjacentBombs]},
      FlagImage{x, y, w, h, Config::FLAG_IMAGE} {};

void MinesweeperCell::HandleEvent(SDL_Event const& E) {
  if (E.type == UserEvents::CELL_CLEARED) {
    HandleClearedCell(E.user);
  } else if (E.type == UserEvents::BOMB_PLACED) {
    HandleBombPlacement(E.user);
  } else if (E.type == UserEvents::GAME_LOST) {
    if (hasBomb) {
      isCleared = true;
      setColor(Config::BUTTON_FAILURE_COLOR);
    }
    setIsDisabled(true);
  } else if (E.type == UserEvents::GAME_WON) {
    setIsDisabled(true);
    if (hasBomb) {
      hasFlag = hasBomb;
      setColor(Config::BUTTON_SUCCESS_COLOR);
    }
  }
  Engine::Button::HandleEvent(E);
}

void MinesweeperCell::Render(SDL_Surface* Surface) {
  Engine::Button::Render(Surface);
  if (hasFlag) {
    FlagImage.Render(Surface);
  } else if (hasBomb && isCleared) {
    BombImage.Render(Surface);
  } else if (isCleared and adjacentBombs > 0) {
    Text.Render(Surface);
  }

#ifdef SHOW_DEBUG_HELPERS
  if (hasBomb) {
    BombImage.Render(Surface);
  }
#endif  // SHOW_DEBUG_HELPERS
}

void MinesweeperCell::HandleLeftClick() {
  if (!hasFlag) {
    ClearCell();
  }
}

void MinesweeperCell::HandleRightClick() {
  if (isDisabled) return;
  if (!hasFlag) {
    ReportEvent(UserEvents::FLAG_PLACED);
    hasFlag = true;
  } else {
    ReportEvent(UserEvents::FLAG_REMOVED);
    hasFlag = false;
  }
}

void MinesweeperCell::ClearCell() {
  if (isDisabled) return;
  isCleared = true;
  setIsDisabled(true);
  setColor(Config::BUTTON_CLEARED_COLOR);
  ReportEvent(UserEvents::CELL_CLEARED);
}

void MinesweeperCell::ReportEvent(Uint32 Eventtype) {
  SDL_Event Event{Eventtype};
  Event.user.data1 = this;
  SDL_PushEvent(&Event);
}

bool MinesweeperCell::PlaceBomb() {
  if (hasBomb) return false;
  hasBomb = true;
  ReportEvent(UserEvents::BOMB_PLACED);
  return true;
}

bool MinesweeperCell::isAdjacent(MinesweeperCell* Other) const {
  return (!(Other == this) and std::abs(getCol() - Other->getCol()) <= 1 and
          std::abs(getRow() - Other->getRow()) <= 1);
}

void MinesweeperCell::HandleBombPlacement(const SDL_UserEvent& Event) {
  MinesweeperCell* Cell{static_cast<MinesweeperCell*>(Event.data1)};

  if (isAdjacent(Cell)) {
    ++adjacentBombs;
    Text.SetText(std::to_string(adjacentBombs),
                 Config::TEXT_COLORS[adjacentBombs]);
  }
}

void MinesweeperCell::HandleClearedCell(const SDL_UserEvent& Event) {
  MinesweeperCell* Cell{static_cast<MinesweeperCell*>(Event.data1)};

  if (Cell->hasBomb) return;

  if (isAdjacent(Cell) && Cell->adjacentBombs == 0) {
    ClearCell();
  }
}

void MinesweeperCell::Reset() {
  isCleared = false;
  setIsDisabled(false);
  hasBomb = false;
  hasFlag = false;
  adjacentBombs = 0;
  Text.SetText(std::to_string(adjacentBombs), Config::BUTTON_COLOR);
}