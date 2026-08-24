#include "rendering_utils.hpp"

#include "structs.h"
#include "FrameBuffer.hpp"
#include "user_specs.hpp"
#include <cassert>
#include <iostream>
#include <fstream>
#include <cmath>

uint32_t packColour(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) {
  return r + (g << 8) + (b << 16) + (a << 24);
}

void unpackColour(const uint32_t &colour, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a) {
  r = colour & 255;
  g = (colour >> 8) & 255;
  b = (colour >> 16) & 255;
  a = (colour >> 24) & 255;
}

void drawRect(FrameBuffer& fb, const size_t xPos, const size_t yPos,
              const size_t rectW, const size_t rectH, const uint32_t colour) {
  for(size_t i{ 0 }; i < rectW; ++i) {
    for(size_t j{ 0 }; j < rectH; ++j) {
      size_t xPx{ xPos + i }, yPx{ yPos + j };

      assert(xPx < fb.w && yPx < fb.h);

      fb.setPx(xPx, yPx, colour);
    }
  }
}

void writePPMImg(const std::string filename, const FrameBuffer& fb) {
  std::ofstream ofs(filename, std::ios::binary);
  ofs << "P6\n" << fb.w << ' ' << fb.h << "\n255\n";
  
  for(size_t i{ 0 }; i < fb.w*fb.h; ++i) {
    uint8_t r, g, b, a;
    unpackColour(fb.buffer[i], r, g, b, a);
    ofs << static_cast<char>(r) << static_cast<char>(g) << static_cast<char>(b);
  }
}

void drawSprite(const Sprite& sprite, FrameBuffer& fb, const Player& player,
                Textures& spriteTex, std::vector<double>& depthArr) {
  double yDiff{ sprite.yPos - player.yPos }, xDiff{ sprite.xPos - player.xPos };
  double spriteDist{ std::sqrt(pow(xDiff, 2) + pow(yDiff, 2)) };

  double spriteDir{ atan2(yDiff, xDiff) };
  while(spriteDir - player.viewAngle >  M_PI) { spriteDir -= 2*M_PI; }
  while(spriteDir - player.viewAngle < -M_PI) { spriteDir += 2*M_PI; }
  
  size_t spriteSize = std::min(1000, (int)(fb.h/spriteDist));

  int hOffset{ (spriteDir - player.viewAngle)*fb.w/player.FOV + fb.w/2 - spriteSize/2 },
      vOffset{ fb.h/2 - spriteSize/2 };
  
  for(size_t i{ 0 }; i < spriteSize; ++i) {
    if(hOffset + (int)i < 0 || hOffset + i >= fb.w) { continue; }
    else if(depthArr[hOffset + i] < spriteDist) { continue; }
    
    for(size_t j{ 0 }; j < spriteSize; ++j) {
      if(vOffset + (int)j < 0 || vOffset + j >= fb.h) { continue; }

      size_t x{ (double)i/(double)spriteSize*(double)spriteTex.textureSize() },
             y{ (double)j/(double)spriteSize*(double)spriteTex.textureSize() };
      uint32_t colour{ spriteTex.getPx(sprite.textureID + '0', x, y) };
      uint8_t r, g, b, a;
      unpackColour(colour, r, g, b, a);

      if(a > 128) { fb.setPx(hOffset + i, vOffset + j, colour); }
    }
  }
}