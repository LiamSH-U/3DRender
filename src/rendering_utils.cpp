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

void unpackColour(const uint32_t &color, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a) {
  r = color & 255;
  g = (color >> 8) & 255;
  b = (color >> 16) & 255;
  a = (color >> 24) & 255;
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

void drawSprite(Sprite& sprite, FrameBuffer& fb);