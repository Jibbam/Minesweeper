// Button.h
#pragma once

#include "Globals.h"
#include "Rectangle.h"

namespace Engine {
class Button : public Rectangle {
 public:
  Button(int x, int y, int w, int h) : Rectangle{x, y, w, h} {
    setColor(Config::BUTTON_COLOR);
  };

  virtual void HandleEvent(SDL_Event const& E) {
    if (isDisabled) return;

    if (E.type == SDL_MOUSEMOTION) {
      HandleMouseMotion(E.motion);
    } else if (E.type == SDL_MOUSEBUTTONDOWN) {
      HandleMouseButton(E.button);
    }
  }

 protected:
  virtual void HandleLeftClick() {}
  virtual void HandleRightClick() {}
  virtual void HandleMouseEnter() { setColor(Config::BUTTON_HOVER_COLOR); }
  virtual void HandleMouseExit() { setColor(Config::BUTTON_COLOR); }

  void HandleMouseMotion(SDL_MouseMotionEvent const& E) {
    if (isWithinBounds(E.x, E.y)) {
      HandleMouseEnter();
    } else {
      HandleMouseExit();
    }
  }

  void HandleMouseButton(SDL_MouseButtonEvent const& E) {
    if (!isWithinBounds(E.x, E.y)) {
      return;
    }
    const Uint8 button{E.button};
    if (button == SDL_BUTTON_LEFT) {
      HandleLeftClick();
    } else if (button == SDL_BUTTON_RIGHT) {
      HandleRightClick();
    }
  }

  void setIsDisabled(bool newValue) { isDisabled = newValue; }

 bool isDisabled{false};
};

}  // namespace Engine