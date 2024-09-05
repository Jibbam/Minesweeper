#pragma once
#include <SDL.h>
#include <iostream>
#include <SDL.h>



namespace Engine {

class Rectangle {
 public:
  Rectangle(int x, int y, int w, int h, SDL_Color Color = {0, 0, 0, 255})
      : Rect{x, y, w, h}, Color{Color} {};

  virtual void Render(SDL_Surface* Surface) {
    SDL_FillRect(Surface, &Rect,
                 SDL_MapRGB(Surface->format, Color.r, Color.g, Color.b));
  };

  void setColor(SDL_Color C) { Color = C; }

  bool isWithinBounds(int x, int y) const {
    if (x < Rect.x || x > (Rect.x + Rect.w) || y < Rect.y ||
        y > (Rect.y + Rect.h)) {
      return false;
    } else {
      return true;
    }
  }

  SDL_Rect* GetRect() { return &Rect; }
  virtual ~Rectangle() = default;

 private:
  SDL_Rect Rect{0, 0, 0, 0};
  SDL_Color Color{0, 0, 0, 0};
};

}  // namespace Engine
