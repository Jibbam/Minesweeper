#include "ImageExtended.h"

// Constructors
Image::Image(const std::string& File, ScalingMode ScalingMode,
             SDL_PixelFormat* PrefPixelFormat)
    : mScalingMode{ScalingMode}, mPrefPixelFormat{PrefPixelFormat} {
  loadFile(File);
  if (mImageSurface) {
    Initialize({0, 0, mImageSurface->w, mImageSurface->h},
               mAppliedSrcRectangle);
  } else {
    std::cout << "Error loading Image, invalid surface";
  }
}

Image::Image(const std::string& File, const SourceRect& SourceRect,
             ScalingMode ScalingMode,
             SDL_PixelFormat* PrefPixelFormat)
    : mScalingMode{ScalingMode}, mPrefPixelFormat{PrefPixelFormat} {
  loadFile(File);

  if (mImageSurface) {
    Initialize(SourceRect, mAppliedSrcRectangle);
  } else {
    std::cout << "Error loading Image, invalid surface";
  }
}

Image::Image(const std::string& File, const DestRect& DestRect,
             ScalingMode ScalingMode,
             SDL_PixelFormat* PrefPixelFormat)
    : mScalingMode{ScalingMode}, mPrefPixelFormat{PrefPixelFormat} {
  loadFile(File);
  if (mImageSurface) {
    Initialize({0, 0, mImageSurface->w, mImageSurface->h}, DestRect);
  } else {
    std::cout << "Error loading Image, invalid surface";
  }
}
Image::Image(const std::string& File, const SourceRect& SourceRect,
             const DestRect& DestRect, ScalingMode ScalingMode,
             SDL_PixelFormat* PrefPixelFormat)
    : mScalingMode{ScalingMode}, mPrefPixelFormat{PrefPixelFormat} {
  loadFile(File);

  if (mImageSurface) {
    Initialize(SourceRect, DestRect);
  } else {
    std::cout << "Error loading Image, invalid surface";
  }
}

void Image::Initialize(SDL_Rect const& SourceRect, SDL_Rect const& DestRect) {
  SetSourceRect(SourceRect);
  SetDestRect(DestRect);
}

// Destructors
Image::~Image() { SDL_FreeSurface(mImageSurface); }

// Member functions
void Image::loadFile(const std::string& File) {
  if (File == mFile) return;
  SDL_Surface* nextSurface{IMG_Load(File.c_str())};
  if (isValidSurface(nextSurface, "Loading new Image file")) {
    SDL_FreeSurface(mImageSurface);
    mFile = File;
    mImageSurface = nextSurface;
  }

  if (mPrefPixelFormat) {
    ConvertSurface();
  }
}

int Image::SetSourceRect(const SDL_Rect& Rect) {
  mRequestedSrcRectangle = Rect;
  if (validateRectangle(Rect, mImageSurface, "Source Rectangle")) {
    mAppliedSrcRectangle = Rect;
    return 0;
  } else {
    mAppliedSrcRectangle = SDL_Rect{0, 0, mImageSurface->w, mImageSurface->h};
    return -1;
  }
}

int Image::SetDestRect(const SDL_Rect& Rect) {
  mRequestedDestRectangle = Rect;
  if (validateRectangle(Rect, nullptr, "Destination Rectangle")) {
    mAppliedDestRectangle =
        (mScalingMode == ScalingMode::Contain)
            ? MatchAspectRatio(mRequestedDestRectangle, mAppliedSrcRectangle)
            : Rect;
    return 0;
  } else {
    mAppliedDestRectangle =
        SDL_Rect{0, 0, mAppliedSrcRectangle.w, mAppliedSrcRectangle.h};
    return -1;
  }
}

bool Image::validateRectangle(const SDL_Rect& Rect, const SDL_Surface* Surface,
                              const std::string& Context) const {
  if (SDL_RectEmpty(&Rect)) {
    std::cout << "\n[Error]: " << Context << "Rectangle has no area";
    return false;
  } else if (Surface && !RectangleWithinSurface(Rect, Surface)) {
    std::cout << "\n[Error]: " << Context
              << "Rectangle is not within target surface";
    return false;
  }
  return true;
}

bool Image::RectangleWithinSurface(const SDL_Rect& Rect,
                                   const SDL_Surface* Surface) const {
  if (Rect.x < 0 || Rect.y < 0 || Rect.x + Rect.w > Surface->w ||
      Rect.y + Rect.h > Surface->h) {
    return false;
  }

  return true;
}

SDL_Rect Image::MatchAspectRatio(SDL_Rect const& Source,
                                 SDL_Rect const& Target) const {
  float srcRatio = Source.w / static_cast<float>(Source.h);
  float targetRatio = Target.w / static_cast<float>(Target.h);
  SDL_Rect result = Source;
  if (srcRatio < targetRatio) {
    result.h = static_cast<int>(Source.w / targetRatio);
  } else if (srcRatio > targetRatio) {
    result.w = static_cast<int>(targetRatio * Source.h);
  }
  return result;
}

void Image::Render(SDL_Surface* Surface) {
#ifdef DEBUG
  int initialWidth = mAppliedDestRectangle.w;
  int initialHeight = mAppliedDestRectangle.h;
#endif  // DEBUG

  if (mScalingMode == ScalingMode::None) {
    SDL_BlitSurface(mImageSurface, &mAppliedSrcRectangle, Surface,
                    &mAppliedDestRectangle);
  } else {
    SDL_BlitScaled(mImageSurface, &mAppliedSrcRectangle, Surface,
                   &mAppliedDestRectangle);
  }

#ifdef DEBUG
  if (initialWidth != mAppliedDestRectangle.w) {
    std::cout << "Horizontal clipping";
  } else if (initialHeight != mAppliedDestRectangle.h) {
    std::cout << "Vertical clipping";
  }
#endif  // DEBUG
}

bool Image::isValidSurface(const SDL_Surface* const Surface,
                           std::string const Context) const {
  if (!Surface) {
    std::cout << "\nError:  " << Context << ": " << SDL_GetError() << "\n";
    return false;
  }
  return true;
}

int Image::SetFile(std::string const& File) {
  loadFile(File);
  if (mImageSurface) {
    mRequestedSrcRectangle = {0, 0, mImageSurface->w, mImageSurface->h};
    SetSourceRect(mRequestedSrcRectangle);
    return 0;
  }
  return -1;
}

int Image::SetFile(std::string const& File, SourceRect const& SourceRect) {
  loadFile(File);
  if (mImageSurface) {
    mRequestedSrcRectangle = SourceRect;
    SetSourceRect(SourceRect);
    return 0;
  }
  return -1;
}

int Image::SetScalingMode(ScalingMode Scalingmode) {
  mScalingMode = Scalingmode;
  return SetDestRect(mAppliedSrcRectangle);
}
int Image::SetScalingMode(ScalingMode Scalingmode, DestRect const& DestRect) {
  mScalingMode = Scalingmode;
  return SetDestRect(DestRect);
}

int Image::ConvertSurface() {
  SDL_Surface* convertedSurface{
      SDL_ConvertSurface(mImageSurface, mPrefPixelFormat, 0)};

  if (isValidSurface(convertedSurface, "Converting Surface")) {
    SDL_FreeSurface(mImageSurface);
    mImageSurface = convertedSurface;
    return 0;
  }
  return -1;
}

int Image::SetPixelFormat(SDL_PixelFormat* PrefPixelFormat) {
  mPrefPixelFormat = PrefPixelFormat;
  return ConvertSurface();
}