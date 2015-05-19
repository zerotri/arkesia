#include <iostream>
#include "arkesia.hpp"
#include "arkesia_plugins.hpp"
#include <tuple>
#include <SDL.h>
#include <vector>
#include <memory>
#include <iostream>
#include "term11/term11.hpp"


int main( int argc, const char* argv[] ) {
  // build arguments list from argc and argv
  std::vector< std::string > arguments{ argv + 1, argv + argc };
  // seed randomness
  std::srand( std::time(nullptr) );
  struct ark::memory::block game_memory;
  game_memory.create( ark::memory::gigabytes(1) );

  unsigned char* offset = game_memory.get_offset< unsigned char* >( 32 );
  std::cout << term11::fg( term11::Cyan )
            << "Offset from base is "
            << term11::reset()
            << reinterpret_cast<void *>( offset )
            << std::endl;

  auto plugin = ark::plugin::load( "/plugins/game_state_main/game_state_main.dylib" );

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return 1;
  }
#ifndef USE_SDL_RENDERER
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif

  SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL);
  if (win == nullptr){
    std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

#ifndef USE_SDL_RENDERER
  SDL_GLContext context = SDL_GL_CreateContext(win);
  if (context == nullptr){
    SDL_DestroyWindow(win);
    std::cout << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }
  auto swap_window = [win]{
    SDL_GL_SwapWindow(win);
  };
  auto destroy_renderer = [context]{
    SDL_GL_DeleteContext(context);
  };
#else
  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == nullptr){
    SDL_DestroyWindow(win);
    std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }
  auto swap_window = [ren]{
    SDL_RenderClear(ren);
    SDL_RenderPresent(ren);
  };
  auto destroy_renderer = [ren]{
    SDL_DestroyRenderer(ren);
  };
#endif

  if( plugin.draw == 0 ) std::cout << "No draw function" <<std::endl;

  SDL_Event windowEvent;
  while (true)
  {
      if (SDL_PollEvent(&windowEvent))
      {
          if (windowEvent.type == SDL_QUIT) break;
          if (windowEvent.type == SDL_KEYUP &&
              windowEvent.key.keysym.sym == SDLK_ESCAPE) break;
      }

      if( plugin.draw != 0 ) plugin.draw();
      swap_window();
  }


  destroy_renderer();
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}