#include "Map.hpp"

Map::Map(const size_t mapW_in, const size_t mapH_in,
         const FrameBuffer& fb, const std::string& mapS_in)
  : mapW(mapW_in), mapH(mapH_in), gridW(fb.w/mapW_in),
    gridH(fb.h/mapH_in), mapS(mapS_in) {
}

char Map::getSymbol(const size_t x, const size_t y) { return mapS[x + y*mapW]; }