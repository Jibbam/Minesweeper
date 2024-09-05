#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Window.h"
#include "UI.h"


int main(int argc, char** argv) {
  SDL_Init(SDL_INIT_VIDEO);
#ifdef SHOW_DEBUG_HELPERS
  Utils::CheckSDLError("SDL_Init");
#endif  

  IMG_Init(IMG_INIT_PNG);
#ifdef SHOW_DEBUG_HELPERS
  Utils::CheckSDLError("SDL_Init");
#endif  

  TTF_Init();
#ifdef SHOW_DEBUG_HELPERS
  Utils::CheckSDLError("SDL_Init");
#endif  

  Engine::Window GameWindow;
  MinesweeperUI UI;
 
  SDL_Event Event;
  bool shouldQuit{false};

  while (!shouldQuit) {
    while (SDL_PollEvent(&Event)) {
      if (Event.type == SDL_QUIT) {
        shouldQuit = true;
      } else {
        UI.HandleEvent(Event);
      }
    }
    GameWindow.Render();
    UI.Render(GameWindow.GetSurface());    
    GameWindow.Update();
  }
  
  SDL_Quit();
  /*TTF_Quit();*/
  return 0;
}