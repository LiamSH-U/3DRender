#ifndef STRUCTS_H
#define STRUCTS_H

#include <stddef.h>
#include <cstdint>
#include <vector>

struct Player {
  double xPos, yPos, FOV, viewAngle;
};

struct Sprite {
  size_t textureID;
  double xPos, yPos;
};

struct TexArray {
  uint32_t (*getWallColour)(char);
  size_t size, count;
  std::vector<uint32_t> pxMap;
};

struct Resolutions {
  size_t winW, winH;
  size_t mapW, mapH;
  size_t rectW, rectH;
};

#endif