#ifndef MAP_HPP
#define MAP_HPP

#include "FrameBuffer.hpp"
#include <cstddef>
#include <string>

class Map {
public:
  size_t mapW, mapH;
  size_t gridW, gridH;
  std::string mapS;

  Map(const size_t mapW_in, const size_t mapH_in,
      const FrameBuffer& fb, const std::string& mapS);

  char getSymbol(const size_t x, const size_t y);
};

#endif