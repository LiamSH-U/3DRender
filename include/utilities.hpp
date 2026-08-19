#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <cstdint>
#include <vector>
#include <string>

#define WHITE 4294967295
#define BLACK 4278190080
#define RED 4278190335
#define GREEN 4278255360
#define BLUE 4294901760

struct Player {
  double xPos, yPos, FOV, viewAngle;
};

struct Sprite {
  size_t textureID;
  double xPos, yPos;
};

double degToRad(double degrees);

double radToDeg(double radians);

uint32_t packColour(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 255);

void unpackColour(const uint32_t &color, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a);

uint32_t getWallType(char symbol);

#endif