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

struct Resolutions {
  size_t winW, winH;
  size_t mapW, mapH;
  size_t rectW, rectH;
};

#endif