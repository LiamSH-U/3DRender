#include "utilities.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cassert>
#include <cmath>
#include <SDL2/SDL.h>

double degToRad(double degrees) { return (degrees/180)*M_PI; }

double radToDeg(double radians) { return (radians*180)/M_PI; }

uint32_t packColour(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) {
  return r + (g << 8) + (b << 16) + (a << 24);
}

void unpackColour(const uint32_t &color, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a) {
  r = color & 255;
  g = (color >> 8) & 255;
  b = (color >> 16) & 255;
  a = (color >> 24) & 255;
}

uint32_t getWallType(char symbol) {
  uint32_t ret;
  switch(symbol) {
    case '0': ret = RED; break;

    case '1': ret = BLACK; break;

    case '2': ret = GREEN; break;
          
    case '3': ret = BLUE; break;
          
    case '4': ret = 358323589; break;

    case '5': ret = 2358932849; break;
  }

  return ret;
}