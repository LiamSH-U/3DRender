#include "rendering_utils.hpp"

#include "structs.h"
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

void drawRect(std::vector<uint32_t>& img, const size_t imgW, const size_t imgH,
              const size_t xPos, const size_t yPos, const size_t rectW, const size_t rectH,
              const uint32_t colour) {
  assert(img.size() == imgW*imgH);

  for(size_t i{ 0 }; i < rectW; ++i) {
    for(size_t j{ 0 }; j < rectH; ++j) {
      size_t xPx{ xPos + i }, yPx{ yPos + j };

      assert(xPx < imgW && yPx < imgH);

      img[xPx + yPx*imgW] = colour;
    }
  }
}

void writePPMImg(const std::string filename, const std::vector<uint32_t>& img,
                  const size_t w, const size_t h) {
  assert(img.size() == w*h);
  
  std::ofstream ofs(filename, std::ios::binary);
  ofs << "P6\n" << w << ' ' << h << "\n255\n";
  
  for(size_t i{ 0 }; i < w*h; ++i) {
    uint8_t r, g, b, a;
    unpackColour(img[i], r, g, b, a);
    ofs << static_cast<char>(r) << static_cast<char>(g) << static_cast<char>(b);
  }
}