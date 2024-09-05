#pragma once

#include "Button.h"
#include "Text.h"

class NewGameButton : public Engine::Button {
 public:
  NewGameButton(int x, int y, int w, int h)
      : Button{x, y, w, h}, Text{x, y, w, h, "NEW GAME", {}, 10} {};

  void Render(SDL_Surface* TargetSurface) override {
    Button::Render(TargetSurface);
    Text.Render(TargetSurface);
  }

  void HandleLeftClick() override {
    SDL_Event Event{UserEvents::NEW_GAME};
    SDL_PushEvent(&Event);
  }

 private:
  Engine::Text Text;
};