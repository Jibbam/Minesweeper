#pragma once
#include <SDL.h>

#include "Globals.h"

namespace Engine {
class Window {
 public:
  Window()
      : mWindowWidth{Config::WINDOW_WIDTH},
        mWindowHeigth{Config::WINDOW_HEIGHT} {
    SDLWindow =
        SDL_CreateWindow(Config::GAME_NAME.c_str(), SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED, Config::WINDOW_WIDTH,
                         Config::WINDOW_HEIGHT, 0);
  }

  void Render() {
    SDL_FillRect(
        GetSurface(), nullptr,
        SDL_MapRGB(GetSurface()->format, Config::BACKGROUND_COLOR.r,
                   Config::BACKGROUND_COLOR.g, Config::BACKGROUND_COLOR.b));
  }

  void Update() { SDL_UpdateWindowSurface(SDLWindow); }

  SDL_Surface* GetSurface() { return SDL_GetWindowSurface(SDLWindow); }

  int getWindowWidth() { return mWindowWidth; }

  ~Window() { SDL_DestroyWindow(SDLWindow); }

  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

 private:
  SDL_Window* SDLWindow;
  int mWindowWidth;
  int mWindowHeigth;
};
}  // namespace Engine