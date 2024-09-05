#pragma once

#include <iostream>
#include <string>

#include "SDL.h"
#include "SDL_image.h"

namespace Engine {
class Image {
 public:
  Image(int x, int y, int w, int h, const std::string& Filename,
        int Padding = 12)
      : Destination{x + Padding / 2, y + Padding / 2, w - Padding,
                    h - Padding} {
    ImageSurface = IMG_Load(Filename.c_str());
#ifdef SHOW_DEBUG_HELPERS
    Utils::CheckSDLError("IMG_Load");
#endif  // SHOW_DEBUG_HELPERS
  }

  ~Image() {
    if (ImageSurface) {
      SDL_FreeSurface(ImageSurface);
    }
  }
  Image(Image const&){}

  void Render(SDL_Surface* Surface) {
#ifdef SHOW_DEBUG_HELPERS
    int wtemp = Destination.w;
    int htemp = Destination.h;
#endif  // SHOW_DEBUG_HELPERS

    SDL_BlitScaled(ImageSurface, nullptr, Surface, &Destination);

#ifdef SHOW_DEBUG_HELPERS
    if (Destination.w != wtemp) {
      std::cout << "Horizontal clipping occurred";
    } else if (Destination.h != htemp)
      std::cout << "Vertical clipping occurred";
#endif  // SHOW_DEBUG_HELPERS
  }

 private:
  SDL_Surface* ImageSurface{nullptr};
  SDL_Rect Destination{0, 0, 0, 0};
};
}  // namespace Engine