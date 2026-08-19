#include "utilities.hpp"

int main() {
  // constants and whatnot
  constexpr size_t winW{ 512 }, winH{ 512 };
  constexpr size_t mapW{ 16 }, mapH{ 16 };
  constexpr size_t rectW{ winW/mapW }, rectH{ winH/mapH };
  std::vector<uint32_t> topDownMap(winW*winH, WHITE);
  std::vector<uint32_t> frameBuffer(winW*winH, WHITE);
  const char map[] = "0000111122223333"\
                     "1              3"\
                     "1      22222   3"\
                     "1     1        3"\
                     "1     1  //////2"\
                     "1     1        2"\
                     "1   4////      2"\
                     "1   4   /////  2"\
                     "5   4   /      1"\
                     "5   4   /  ////1"\
                     "5       /      1"\
                     "5       /      1"\
                     "3       /      3"\
                     "3 2222222      3"\
                     "3              3"\
                     "3555533333333333";
  assert(sizeof(map) == mapW*mapH + 1);
  Player player{ 3.456, 2.346, degToRad(60.0), 1.523 };

  for(size_t frame{ 0 }; frame < 360; ++frame) {
    std::stringstream topdownName, playerviewName;
    topdownName << "topdown" << std::setfill('0') << std::setw(5) << frame << ".ppm";
    playerviewName << "playerview" << std::setfill('0') << std::setw(5) << frame << ".ppm";

    std::stringstream topdownDBGName;
    topdownName << "topdowndbg" << std::setfill('0') << std::setw(5) << frame << ".ppm";

    player.viewAngle += 2*M_PI/360;

    // clear buffers
    topDownMap = std::vector<uint32_t>(winW*winH, WHITE);
    frameBuffer = std::vector<uint32_t>(winW*winH, WHITE);

    // draw map
    for(size_t j{ 0 }; j < mapH; j++) {
      for(size_t i{ 0 }; i < mapW; i++) {
        if(map[i + j*mapW] == ' ') { continue; }

        size_t rectX{ i*rectW }, rectY{ j*rectH };
        drawRect(topDownMap, winW, winH, rectX, rectY, rectW, rectH,
                getWallType(map[i + j*mapW]));
      }
    }

    // draw player
    drawRect(topDownMap, winW, winH, (size_t)(player.xPos*rectW) - 2,
             (size_t)(player.yPos*rectH) - 2, 5, 5, packColour(0, 255, 255));
    
    // draw player FOV
    double rayAngle{ player.viewAngle - player.FOV/2 }, stepSize{ player.FOV/winW };
    for(size_t i{ 0 }; i < winW; ++i) {
      rayAngle += stepSize;
      for(float c{ 0 }; c < 100; c += 0.05) {
        float cx{ player.xPos + c*cos(rayAngle) },
              cy{ player.yPos + c*sin(rayAngle) };

        if(map[(size_t)cx + ((size_t)cy)*mapW] != ' ') {
          size_t wallHeight = winH/(c*cos(rayAngle - player.viewAngle));
          uint32_t wallColour = getWallType(map[(size_t)cx + ((size_t)cy)*mapW]);

          drawRect(frameBuffer, winW, winH, i, winH/2 - wallHeight/2, 1, wallHeight, wallColour);

          break;
        }

        size_t xPx{ cx*rectW }, yPx{ cy*rectH };
        topDownMap[xPx + yPx*winH] = packColour(0, 255, 255);
      }
    }

    writePPMImg(topdownName.str(), topDownMap, winW, winH);
    writePPMImg(playerviewName.str(), frameBuffer, winW, winH);
  }

  return 0;
}