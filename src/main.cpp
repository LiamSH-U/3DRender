#include "rendering_utils.hpp"
#include "rendering.hpp"
#include "Textures.hpp"
#include "FrameBuffer.hpp"
#include "Map.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <SDL2/SDL.h>

struct DistSort {
  bool operator()(Sprite& s1, Sprite& s2) { return s1.distToPlayer >= s2.distToPlayer; }
};

int main() {
  // constants and whatnot
  constexpr Resolutions res{ 512, 512,
                             16, 16,
                             512/16, 512/16 };

  FrameBuffer fb(res.winW, res.winH);

  std::string mapS = "0000111122223333"\
                     "1              3"\
                     "1      22222   3"\
                     "1     1        3"\
                     "1     1  0000002"\
                     "1     1        2"\
                     "1   40000      2"\
                     "1   4   00000  2"\
                     "5   4   0      1"\
                     "5   4   0  00001"\
                     "5       0      1"\
                     "5       0      1"\
                     "3       0      3"\
                     "3 2222222      3"\
                     "3              3"\
                     "3555533333333333";
  Map map(res.mapW, res.mapH, fb, mapS);

  Textures wallTex;
  Textures enemiesTex;
  if(!wallTex.loadTextures("./resources/walltextures.bmp", SDL_PIXELFORMAT_ABGR8888) ||
     !enemiesTex.loadTextures("./resources/enemysprites.bmp", SDL_PIXELFORMAT_ABGR8888)) {
    return -1;
  }

  Player player{ 3.456, 2.345, M_PI/3.0, 1.523 };

  std::vector<Sprite> sprites{ {2, 3.253, 3.812, 0.0}, {0, 1.834, 8.765, 0.0},
                               {1, 5.323, 5.365, 0.0}, {1, 4.123, 10.265, 0.0} };
  for(Sprite& s : sprites) {
    s.distToPlayer = std::sqrt(pow(s.xPos - player.xPos, 2) + pow(s.yPos - player.yPos, 2));
  }
  DistSort d;
  std::sort(sprites.begin(), sprites.end(), d);

  std::vector<double> depthArr(res.winW);


  renderPlayerView(fb, map, player, sprites, wallTex, enemiesTex);

  SDL_Window* window{ nullptr };
  SDL_Renderer* renderer{ nullptr };

  if(SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << "Error: " << SDL_GetError() << std::endl;
    
    return -1;
  }

  if(SDL_CreateWindowAndRenderer(fb.w, fb.h, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS,
                                 &window, &renderer)) {
    std::cerr << "Error: " << SDL_GetError() << std::endl;
    
    return -1;
  }

  SDL_Texture* fbTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888,
                                             SDL_TEXTUREACCESS_STREAMING, fb.w, fb.h);
  SDL_UpdateTexture(fbTexture, NULL, reinterpret_cast<void*>(fb.buffer.data()), fb.w*4);

  bool running{ true };
  int playerYWalk{ 0 }, playerXWalk{ 0 }, playerTurn{ 0 };
  SDL_Event event;
  while(running) {

    if(SDL_PollEvent(&event)) {
      switch(event.type) {
        case SDL_QUIT: running = false;
        break;

        case SDL_KEYUP:
          switch(event.key.keysym.sym) {
            case 'w': playerYWalk = 0;
            break;

            case 'a': playerXWalk = 0;
            break;

            case 's': playerYWalk = 0;
            break;

            case 'd': playerXWalk = 0;
            break;

            case SDLK_LEFT: playerTurn = 0;
            break;

            case SDLK_RIGHT: playerTurn = 0;
            break;
          }
        break;

        case SDL_KEYDOWN:
          switch(event.key.keysym.sym) {
            case 'w': playerYWalk = 1;
            break;

            case 'a': playerXWalk = -1;
            break;

            case 's': playerYWalk = -1;
            break;

            case 'd': playerXWalk = 1;
            break;

            case SDLK_LEFT: playerTurn = -1;
            break;

            case SDLK_RIGHT: playerTurn = 1;
            break;

            case SDLK_ESCAPE: running = false;
            break;
          }
        break;
      }
    }

    player.viewAngle += (double)playerTurn*0.01;
    double newX{ player.xPos + (playerXWalk*cos(player.viewAngle + M_PI/2.0) +
                                playerYWalk*cos(player.viewAngle))*0.02 },
           newY{ player.yPos + (playerYWalk*sin(player.viewAngle) +
                                playerXWalk*sin(player.viewAngle + M_PI/2.0))*0.02 };
    // double newX{ player.xPos + playerWalk*cos(player.viewAngle) },
    //        newY{ player.yPos + playerWalk*sin(player.viewAngle) };

    bool newPos{ false };
    if(newX >= 0 && newX < (double)map.mapW &&
       newY >= 0 && newY < (double)map.mapH) {
      if(map.getSymbol((size_t)newX, (size_t)player.yPos) == ' ') {
        player.xPos = newX;
        newPos = true;
      }
      if(map.getSymbol((size_t)player.xPos, (size_t)newY) == ' ') {
        player.yPos = newY;
        newPos = true;
      }
    }

    if(newPos) {
      for(Sprite& s : sprites) {
        s.distToPlayer = std::sqrt(pow(s.xPos - player.xPos, 2) + pow(s.yPos - player.yPos, 2));
      }
      std::sort(sprites.begin(), sprites.end(), d);
    }

    std::cerr << "xPos: " << player.xPos << ' '
              << "yPos: " << player.yPos << '\n'
              << std::endl;

    renderPlayerView(fb, map, player, sprites, wallTex, enemiesTex);
    SDL_UpdateTexture(fbTexture, NULL, reinterpret_cast<void*>(fb.buffer.data()), fb.w*4);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, fbTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture(fbTexture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();
}