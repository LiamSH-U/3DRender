#include "Textures.hpp"

#include "rendering_utils.hpp"
#include "FrameBuffer.hpp"
#include <iostream>
#include <cstdint>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Textures::Textures()
  : w(0), h(0), size(0), count(0), pxMap(std::vector<uint32_t>()) {
}

size_t Textures::textureSize() { return size; }

uint32_t Textures::getPx(const char symbol, const size_t x, const size_t y) {
  return pxMap[(symbol - '0')*size + x + y*w];
}

// this function is a little confusing, explanation below:
/**
 * 1. xOffset and yOffset are calculated to show the distance from the nearest gridline.
 *    The seemingly arbitrary 0.5 added to x and y is there to correct for rounding so
 *    that x = 3.4 will give an offset of 0.4 since 3.4 is 0.4 "ahead" of x = 3 and x = 3.6
 *    will give an offset of -0.4 since 3.6 is 0.4 "behind" x = 4.
 * 2. We then take whichever has the largest magnitude since a ray hitting the wall implies
 *    that we are directly at a gridline; if the wall is horizontal, yOffset should be 0 and
 *    if the wall is vertical, xOffset should be 0.
 * 3. Finally, we re-add size to tex if it is negative since the index cannot be negative.
 */
size_t Textures::getWallXCoord(const double x, const double y) {
  double xOffset{ x - floor(x + 0.5) }, yOffset{ y - floor(y + 0.5) };
  int tex = std::abs(yOffset) > std::abs(xOffset) ? yOffset*size : xOffset*size;
  if(tex < 0) { tex += size; }
  
  assert(tex >= 0 && tex < (int)size);

  return (size_t)tex;
}

bool Textures::loadTextures(const std::string filename) {
  int numChans;
  unsigned char* img = stbi_load(filename.c_str(), &w, &h, &numChans, 0);
  if(!img) {
    std::cerr << "Error: textures failed to load" << std::endl;

    return false;
  }
  else if(numChans != 4) {
    std::cerr << "Error: texture must be a 32-bit image" << std::endl;
    stbi_image_free(img);

    return false;
  }

  count = w/h;
  size = w/count;
  if(w != h*(int)count) {
    std::cerr << "Error: texture file must contain n square textures packed horizontally"
              << std::endl;
    stbi_image_free(img);

    return false;
  }

  pxMap.resize(w*h);
  for(size_t j{ 0 }; j < h; ++j) {
    for(size_t i{ 0 }; i < w; ++i) {
      uint8_t r{ img[4*(i + j*w) + 0] }, g{ img[4*(i + j*w) + 1] },
              b{ img[4*(i + j*w) + 2] }, a{ img[4*(i + j*w) + 3] };
      pxMap[i + j*w] = packColour(r, g, b, a);
    }
  }

  stbi_image_free(img);

  return true;
}

uint32_t Textures::getWallColour(const char symbol) { return pxMap[(symbol-'0')*size]; }

void Textures::drawTextureSlice(const char symbol, const double x, const double y,
                                const size_t colStart, const size_t colHeight,
                                FrameBuffer& fb, const size_t imgX) {
  size_t textureXCoord = getWallXCoord(x, y);

  for(size_t i{ 0 }; i < colHeight; ++i) {
    fb.setPx(imgX, i + colStart, getPx(symbol, textureXCoord, (i*size)/colHeight));
  }
}