#include "rendering.hpp"

#include "rendering_utils.hpp"
#include <cmath>

// void renderTopDown(FrameBuffer& fb, Map& map, const Player player,
//                    std::vector<Sprite>& enemies, std::string filename, Textures& textures) {
//   fb.wipe();

//   // draw map
//   for(size_t j{ 0 }; j < map.mapH; j++) {
//     for(size_t i{ 0 }; i < map.mapW; i++) {
//       char symbol{ map.getSymbol(i, j) };
//       if(symbol == ' ') { continue; }

//       size_t rectX{ i*map.gridW }, rectY{ j*map.gridH };
//       drawRect(fb, rectX, rectY, map.gridW, map.gridH,
//                textures.getWallColour(symbol));
//     }
//   }

//   // draw player
//   size_t xCentre{ (size_t)(player.xPos*map.gridW - 2.5) },
//          yCentre{ (size_t)(player.yPos*map.gridH - 2.5) };
//   drawRect(fb, xCentre, yCentre, 5, 5, packColour(0, 255, 255));

//   // draw rays
//   double rayAngle{ player.viewAngle - player.FOV/2 }, stepSize{ player.FOV/fb.w };
//   for(size_t i{ 0 }; i < fb.w; ++i) {
//     rayAngle += stepSize;
//     for(float c{ 0 }; c < 100; c += 0.01) {
//       float x{ player.xPos + c*cos(rayAngle) },
//             y{ player.yPos + c*sin(rayAngle) };
//       char symbol{ map.getSymbol((size_t)x, (size_t)y) };
//       if(symbol != ' ') { break; }

//       size_t xPx{ x*map.gridW }, yPx{ y*map.gridH };
//       fb.setPx(xPx, yPx, packColour(0, 255, 255));
//     }
//   }

//   // draw enemies
//   for(Sprite s : enemies) {
//     size_t eXCentre{ (size_t)(s.xPos*map.gridW - 2.5) },
//            eYCentre{ (size_t)(s.yPos*map.gridW - 2.5) };
//     drawRect(fb, eXCentre, eYCentre, 5, 5, RED);
//   }

//   writePPMImg(filename, fb);
// }

void renderPlayerView(FrameBuffer& fb, Map& map, const Player player,
                      std::vector<Sprite>& enemies, Textures& wallTex, Textures& enemiesTex) {
  fb.wipe();

  std::vector<double> depthArr(fb.w);

  // draw player view
  double rayAngle{ player.viewAngle - player.FOV/2 }, stepSize{ player.FOV/fb.w };
  for(size_t i{ 0 }; i < fb.w; ++i) {
    rayAngle += stepSize;
    for(float c{ 0 }; c < 100; c += 0.01) {
      float x{ player.xPos + c*cos(rayAngle) },
            y{ player.yPos + c*sin(rayAngle) };
      char symbol{ map.getSymbol((size_t)x, (size_t)y) };
      if(symbol == ' ') { continue; }

      depthArr[i] = c*cos(rayAngle - player.viewAngle);

      size_t wallHeight{ fb.h/(c*cos(rayAngle - player.viewAngle)) };
      wallTex.drawTextureSlice(symbol, x, y, fb.h/2 - wallHeight/2, wallHeight, fb, i);

      break;
    }
  }

  // draw enemies
  for(Sprite s : enemies) {
    drawSprite(s, fb, player, enemiesTex, depthArr);
  }
}

// void renderDualView(FrameBuffer& pv, FrameBuffer& td, Map& map, const Player player,
//                     std::vector<Sprite>& enemies, std::string tdFilename,
//                     std::string pvFilename, Textures& textures) {
//   pv.wipe();
//   td.wipe();

//   // draw map
//   for(size_t j{ 0 }; j < map.mapH; j++) {
//     for(size_t i{ 0 }; i < map.mapW; i++) {
//       char symbol{ map.getSymbol(i, j) };
//       if(symbol == ' ') { continue; }

//       size_t rectX{ i*map.gridW }, rectY{ j*map.gridH };
//       drawRect(td, rectX, rectY, map.gridW, map.gridH,
//                textures.getWallColour(symbol));
//     }
//   }

//   // draw player and enemies
//   size_t xCentre{ (size_t)(player.xPos*map.gridW - 2.5) },
//          yCentre{ (size_t)(player.yPos*map.gridH - 2.5) };
//   drawRect(td, xCentre, yCentre, 5, 5, packColour(0, 255, 255));

//   for(Sprite s : enemies) {
//     size_t eXCentre{ (size_t)(s.xPos*map.gridW - 2.5) },
//            eYCentre{ (size_t)(s.yPos*map.gridW - 2.5) };
//     drawRect(td, eXCentre, eYCentre, 5, 5, RED);
//   }
    
//   // draw player FOV
//   double rayAngle{ player.viewAngle - player.FOV/2 }, stepSize{ player.FOV/pv.w };
//   for(size_t i{ 0 }; i < pv.w; ++i) {
//     rayAngle += stepSize;
//     for(float c{ 0 }; c < 100; c += 0.01) {
//       float x{ player.xPos + c*cos(rayAngle) },
//             y{ player.yPos + c*sin(rayAngle) };
//       char symbol{ map.getSymbol((size_t)x, (size_t)y) };
//       if(symbol == ' ') {
//         size_t xPx{ x*map.gridW }, yPx{ y*map.gridH };
//         td.setPx(xPx, yPx, packColour(0, 255, 255));

//         continue;
//       }

//       size_t wallHeight{ pv.h/(c*cos(rayAngle - player.viewAngle)) };

//       textures.drawTextureSlice(symbol, x, y, pv.w/2 - wallHeight/2, wallHeight, pv, i);

//       break;
//     }
//   }

//   writePPMImg(tdFilename, pv);
//   writePPMImg(pvFilename, td);
// }