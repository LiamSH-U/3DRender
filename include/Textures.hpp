#ifndef TEXTURES_HPP
#define TEXTURES_HPP

#include "FrameBuffer.hpp"
#include <cstdint>
#include <vector>
#include <string>

class Textures {
public:
  Textures();

  uint32_t getPx(const char symbol, const size_t x, const size_t y);

  size_t getWallXCoord(const double x, const double y);

  size_t textureSize();

  bool loadTextures(const std::string filename, const uint32_t format);

  uint32_t getWallColour(const char symbol);

  void drawTextureSlice(const char symbol, const double x, const double y,
                        const size_t colStart, const size_t colHeight,
                        FrameBuffer& fb, const size_t imgX);

private:
  int w, h;
  size_t size, count;
  std::vector<uint32_t> pxMap;
};

#endif