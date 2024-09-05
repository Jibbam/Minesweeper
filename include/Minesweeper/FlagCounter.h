#pragma once
#include "Globals.h"
#include "Image.h"
#include "Rectangle.h"
#include "Text.h"

class FlagCounter : public Engine::Rectangle {
 public:
  FlagCounter(int x, int y, int w, int h)
      : Rectangle{x, y, w, h, Config::FLAG_COUNTER_COLOR},
        FlagImage{x,
                  y,
                  Config::FOOTERHEIGHT - Config::PADDING,
                  Config::FOOTERHEIGHT - Config::PADDING,
                  Config::FLAG_IMAGE,
                  24},
        Text{x + Config::FOOTERHEIGHT,
             y,
             w - Config::FOOTERHEIGHT - 24,
             h,
             std::to_string(Config::BOMBS_COUNT),
             {255, 255, 255, 255},
             20} {};

  void Render(SDL_Surface* TargetSurface) override {
    Rectangle::Render(TargetSurface);
    FlagImage.Render(TargetSurface);
    Text.Render(TargetSurface);
  }

  void HandleEvent(const SDL_Event& Event) {
    if (Event.type == UserEvents::FLAG_PLACED) {
      --flagsAvailable;
    } else if (Event.type == UserEvents::FLAG_REMOVED) {
      ++flagsAvailable;
    } else if (Event.type == UserEvents::GAME_WON) {
      flagsAvailable = 0;
    } else if (Event.type == UserEvents::NEW_GAME) {
      flagsAvailable = Config::BOMBS_COUNT;
    } else {
      return;
    }
    Text.SetText(std::to_string(flagsAvailable));
  }

 private:
  Engine::Image FlagImage;
  Engine::Text Text;
  int flagsAvailable{Config::BOMBS_COUNT};
};