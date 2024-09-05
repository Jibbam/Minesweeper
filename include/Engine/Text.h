#pragma once
#include <iostream>

#include "Globals.h"
#include "SDL.h"
#include "SDL_ttf.h"

namespace Engine {
class Text {
 public:
  Text(int x, int y, int w, int h, const std::string& Content,
       SDL_Color Color = {0, 0, 0, 255}, int fontSize = 20)
      : mDestinationRect{x, y, w, h}, mColor{Color} {
    mFont = TTF_OpenFont(Config::FONT.c_str(), fontSize);
#ifdef SHOW_DEBUG_HELPERS
    Utils::CheckSDLError("Loading font");
#endif  // SHOW_DEBUG_HELPERS
    SetText(Content);
  }
  
  ~Text() {
    if (mTextSurface) {
      SDL_FreeSurface(mTextSurface);
    }

    if (mFont) {
      TTF_CloseFont(mFont);
    }
  }

  void SetText(const std::string& Text) { SetText(Text, mColor); }
  void SetText(const std::string& Text, SDL_Color newColor) {
    if (mTextSurface) {
      SDL_FreeSurface(mTextSurface);
    }

    mColor = newColor;
    mTextSurface = TTF_RenderUTF8_Blended(mFont, Text.c_str(), mColor);

    auto [x, y, w, h] = mDestinationRect;

    const int wDif{(w - mTextSurface->w)};
    const int lOffset{wDif / 2};
    const int hDif{(h - mTextSurface->h)};
    const int tOffset{hDif / 2};

    mTextPosition = {x + lOffset, y + tOffset, w, h};
  }

  void Render(SDL_Surface* DestinationSurface) {
    SDL_BlitSurface(mTextSurface, nullptr, DestinationSurface, &mTextPosition);
  }

 private:
  TTF_Font* mFont{nullptr};
  SDL_Surface* mTextSurface{nullptr};
  SDL_Rect mDestinationRect{0, 0, 0, 0};
  SDL_Rect mTextPosition{0, 0, 0, 0};
  SDL_Color mColor{0, 0, 0, 255};
};
}  // namespace Engine
