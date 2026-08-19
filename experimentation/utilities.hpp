#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cassert>
#include <cmath>
#include <SDL2/SDL.h>
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

double degToRad(double degrees) { return (degrees/180)*M_PI; }

double radToDeg(double radians) { return (radians*180)/M_PI; }

uint32_t packColour(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 255) {
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
  assert(img.size() == w * h);
  
  std::ofstream ofs(filename, std::ios::binary);
  ofs << "P6\n" << w << ' ' << h << "\n255\n";
  
  for(size_t i{ 0 }; i < w*h; ++i) {
    uint8_t r, g, b, a;
    unpackColour(img[i], r, g, b, a);
    ofs << static_cast<char>(r) << static_cast<char>(g) << static_cast<char>(b);
  }
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