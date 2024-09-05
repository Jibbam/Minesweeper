#pragma once
#include <SDL.h>

#include <iostream>

#include "Button.h"
#include "Image.h"
#include "Text.h"

class MinesweeperCell : public Engine::Button {
 public:
  MinesweeperCell(int x, int y, int w, int h, int COL, int ROW);

  void HandleEvent(SDL_Event const& E) override;

  void Render(SDL_Surface* Surface) override;

  bool PlaceBomb();

  void Reset();

  [[nodiscard]] int getRow() const { return ROW; }
  [[nodiscard]] int getCol() const { return COL; }
  [[nodiscard]] bool getHasBomb() const { return hasBomb; }

 protected:
  void HandleLeftClick() override;
  void HandleRightClick() override;

 private:
  void ReportEvent(Uint32 Eventtype);
  void ClearCell();
  void HandleBombPlacement(const SDL_UserEvent& Event);
  void HandleClearedCell(const SDL_UserEvent& Event);
  bool isAdjacent(MinesweeperCell* Other) const;

  int COL{};
  int ROW{};
  bool isCleared{false};
  bool hasBomb{false};  
  Engine::Image BombImage;
  bool hasFlag{false};
  Engine::Image FlagImage;
  int adjacentBombs{0};
  Engine::Text Text;

};