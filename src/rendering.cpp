#include "rendering.hpp"

#include "rendering_utils.hpp"
#include <cmath>

void renderTopDown(std::vector<uint32_t>& img, const Resolutions res, const char* map,
                   const Player player, std::string filename, Textures& textures) {
  img = std::vector<uint32_t>(res.winW*res.winH, WHITE);

  // draw map
  for(size_t j{ 0 }; j < res.mapH; j++) {
    for(size_t i{ 0 }; i < res.mapW; i++) {
      if(map[i + j*res.mapW] == ' ') { continue; }

      size_t rectX{ i*res.rectW }, rectY{ j*res.rectH };
      drawRect(img, res.winW, res.winH, rectX, rectY, res.rectW, res.rectH,
               textures.getWallColour(map[i + j*res.mapW]));
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
                      const Player player, std::string filename, Textures& textures) {
  img = std::vector<uint32_t>(res.winW*res.winH, WHITE);

  // draw player view
  double rayAngle{ player.viewAngle - player.FOV/2 }, stepSize{ player.FOV/res.winW };
  for(size_t i{ 0 }; i < res.winW; ++i) {
    rayAngle += stepSize;
    for(float c{ 0 }; c < 100; c += 0.01) {
      float x{ player.xPos + c*cos(rayAngle) },
            y{ player.yPos + c*sin(rayAngle) };
      char symbol{ map[(size_t)x + ((size_t)y)*res.mapW] };

      if(symbol == ' ') { continue; }

      size_t wallHeight{ res.winH/(c*cos(rayAngle - player.viewAngle)) };

      textures.drawTextureSlice(symbol, x, y, res.winH/2 - wallHeight/2, wallHeight,
                                img, res.winW, i);

      break;
    }
  }

  writePPMImg(filename, img, res.winW, res.winH);
}

void renderDualView(std::vector<uint32_t>& td, std::vector<uint32_t>& pv,
                    const Resolutions res, const char* map, const Player player,
                    std::string tdFilename, std::string pvFilename, Textures& textures) {
  td = std::vector<uint32_t>(res.winW*res.winH, WHITE);
  pv = std::vector<uint32_t>(res.winW*res.winH, WHITE);

  // draw map
  for(size_t j{ 0 }; j < res.mapH; j++) {
    for(size_t i{ 0 }; i < res.mapW; i++) {
      if(map[i + j*res.mapW] == ' ') { continue; }

      size_t rectX{ i*res.rectW }, rectY{ j*res.rectH };
      drawRect(td, res.winW, res.winH, rectX, rectY, res.rectW, res.rectH,
               textures.getWallColour(map[i + j*res.mapW]));
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
      float x{ player.xPos + c*cos(rayAngle) },
            y{ player.yPos + c*sin(rayAngle) };
      char symbol{ map[(size_t)x + ((size_t)y)*res.mapW] };

      if(symbol == ' ') {
        size_t xPx{ x*res.rectW }, yPx{ y*res.rectH };
        td[xPx + yPx*res.winH] = packColour(0, 255, 255);

        continue;
      }

      size_t wallHeight{ res.winH/(c*cos(rayAngle - player.viewAngle)) };

      textures.drawTextureSlice(symbol, x, y, res.winW/2 - wallHeight/2, wallHeight,
                                pv, res.winW, i);

      break;
    }
  }

  writePPMImg(tdFilename, td, res.winW, res.winH);
  writePPMImg(pvFilename, pv, res.winW, res.winH);
}