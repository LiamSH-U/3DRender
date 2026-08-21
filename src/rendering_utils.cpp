#include "rendering_utils.hpp"

#include "structs.hpp"
#include "user_specs.hpp"
#include <cassert>
#include <iostream>
#include <fstream>
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

uint32_t TexArray::getWallType(char symbol) { return pxMap[(symbol-'0')*size]; }

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

bool loadTexture(const std::string filename, TexArray& texArray) {
  int numChans{ -1 }, w, h;
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

  texArray.count = w/h;
  texArray.size = w/texArray.count;
  if(w != h*(int)texArray.count) {
    std::cerr << "Error: texture file must contain n square textures packed horizontally"
              << std::endl;
    stbi_image_free(img);

    return false;
  }

  texArray.pxMap.resize(w*h);
  for(size_t j{ 0 }; j < h; ++j) {
    for(size_t i{ 0 }; i < w; ++i) {
      uint8_t r{ img[4*(i + j*w) + 0] }, g{ img[4*(i + j*w) + 1] },
              b{ img[4*(i + j*w) + 2] }, a{ img[4*(i + j*w) + 3] };
      texArray.pxMap[i + j*w] = packColour(r, g, b, a);
    }
  }

  stbi_image_free(img);

  return true;
}

void renderTopDown(std::vector<uint32_t>& img, const Resolutions res, const char* map,
                   const Player player, std::string filename, TexArray& textures) {
  img = std::vector<uint32_t>(res.winW*res.winH, WHITE);

  // draw map
  for(size_t j{ 0 }; j < res.mapH; j++) {
    for(size_t i{ 0 }; i < res.mapW; i++) {
      if(map[i + j*res.mapW] == ' ') { continue; }

      size_t rectX{ i*res.rectW }, rectY{ j*res.rectH };
      drawRect(img, res.winW, res.winH, rectX, rectY, res.rectW, res.rectH,
               textures.getWallType(map[i + j*res.mapW]));
    }
  }

  // draw player
  size_t xCentre{ (size_t)(player.xPos*res.rectW - 2.5) },
         yCentre{ (size_t)(player.yPos*res.rectH - 2.5) };
  drawRect(img, res.winW, res.winH, xCentre, yCentre, 5, 5, packColour(0, 255, 255));

  // draw rays
  double rayAngle{ player.viewAngle - player.FOV/2 }, stepSize{ player.FOV/res.winW };
  for(size_t i{ 0 }; i < res.winW; ++i) {
    rayAngle += stepSize;
    for(float c{ 0 }; c < 100; c += 0.05) {
      float cx{ player.xPos + c*cos(rayAngle) },
            cy{ player.yPos + c*sin(rayAngle) };

      size_t xPx{ cx*res.rectW }, yPx{ cy*res.rectH };
      img[xPx + yPx*res.winH] = packColour(0, 255, 255);
    }
  }

  writePPMImg(filename, img, res.winW, res.winH);
}

void renderPlayerView(std::vector<uint32_t>& img, const Resolutions res, const char* map,
                      const Player player, std::string filename, TexArray& textures) {
  img = std::vector<uint32_t>(res.winW*res.winH, WHITE);

  // draw player view
  double rayAngle{ player.viewAngle - player.FOV/2 }, stepSize{ player.FOV/res.winW };
  for(size_t i{ 0 }; i < res.winW; ++i) {
    rayAngle += stepSize;
    for(float c{ 0 }; c < 100; c += 0.05) {
      float cx{ player.xPos + c*cos(rayAngle) },
            cy{ player.yPos + c*sin(rayAngle) };

      if(map[(size_t)cx + ((size_t)cy)*res.mapW] != ' ') {
        size_t wallHeight{ res.winH/(c*cos(rayAngle - player.viewAngle)) };
        uint32_t wallColour{ textures.getWallType(map[(size_t)cx + ((size_t)cy)*res.mapW]) };

        drawRect(img, res.winW, res.winH, i, res.winH/2 - wallHeight/2,
                 1, wallHeight, wallColour);

        break;
      }
    }
  }

  writePPMImg(filename, img, res.winW, res.winH);
}

void renderDualView(std::vector<uint32_t>& td, std::vector<uint32_t>& pv,
                    const Resolutions res, const char* map, const Player player,
                    std::string tdFilename, std::string pvFilename, TexArray& textures) {
  td = std::vector<uint32_t>(res.winW*res.winH, WHITE);
  pv = std::vector<uint32_t>(res.winW*res.winH, WHITE);

  // draw map
  for(size_t j{ 0 }; j < res.mapH; j++) {
    for(size_t i{ 0 }; i < res.mapW; i++) {
      if(map[i + j*res.mapW] == ' ') { continue; }

      size_t rectX{ i*res.rectW }, rectY{ j*res.rectH };
      drawRect(td, res.winW, res.winH, rectX, rectY, res.rectW, res.rectH,
               textures.getWallType(map[i + j*res.mapW]));
    }
  }

  // draw player
  size_t xCentre{ (size_t)(player.xPos*res.rectW - 2.5) },
         yCentre{ (size_t)(player.yPos*res.rectH - 2.5) };
  drawRect(td, res.winW, res.winH, xCentre, yCentre, 5, 5, packColour(0, 255, 255));
    
  // draw player FOV
  double rayAngle{ player.viewAngle - player.FOV/2 }, stepSize{ player.FOV/res.winW };
  for(size_t i{ 0 }; i < res.winW; ++i) {
    rayAngle += stepSize;
    for(float c{ 0 }; c < 100; c += 0.05) {
      float cx{ player.xPos + c*cos(rayAngle) },
            cy{ player.yPos + c*sin(rayAngle) };

      if(map[(size_t)cx + ((size_t)cy)*res.mapW] != ' ') {
        size_t wallHeight{ res.winH/(c*cos(rayAngle - player.viewAngle)) };
        uint32_t wallColour{ textures.getWallType(map[(size_t)cx + ((size_t)cy)*res.mapW]) };

        drawRect(pv, res.winW, res.winH, i, res.winH/2 - wallHeight/2, 1,
                 wallHeight, wallColour);

        break;
      }

      size_t xPx{ cx*res.rectW }, yPx{ cy*res.rectH };
      td[xPx + yPx*res.winH] = packColour(0, 255, 255);
    }
  }

  writePPMImg(tdFilename, td, res.winW, res.winH);
  writePPMImg(pvFilename, pv, res.winW, res.winH);
}

void animate360View() {

}