#include "rendering_utils.hpp"
#include "rendering.hpp"
#include "Textures.hpp"
#include "FrameBuffer.hpp"
#include "Map.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cassert>
#include <cmath>

int main() {
  // constants and whatnot
  constexpr Resolutions res{ 512, 512,
                             16, 16,
                             512/16, 512/16 };

  FrameBuffer pv(res.winW, res.winH);
  FrameBuffer td(res.winW, res.winH);

  std::string mapS = "0000111122223333"\
                     "1              3"\
                     "1      22222   3"\
                     "1     1        3"\
                     "1     1  0000002"\
                     "1     1        2"\
                     "1   40000      2"\
                     "1   4   00000  2"\
                     "5   4   0      1"\
                     "5   4   0  00001"\
                     "5       0      1"\
                     "5       0      1"\
                     "3       0      3"\
                     "3 2222222      3"\
                     "3              3"\
                     "3555533333333333";
  Map map(res.mapW, res.mapH, pv, mapS);

  Textures textures;
  if(!textures.loadTextures("./resources/walltextures.png")) { return -1; }

  Player player{ 3.456, 2.345, M_PI/3.0, 1.523 };
  std::vector<Sprite> sprites{ {0, 1.834, 8.765}, {1, 5.323, 5.365}, {1, 4.123, 10.265} };

  for(size_t frame{ 0 }; frame < 360; ++frame) {
    std::stringstream topdownName, playerviewName;
    topdownName << "topdown" << std::setfill('0') << std::setw(5) << frame << ".ppm";
    playerviewName << "playerview" << std::setfill('0') << std::setw(5) << frame << ".ppm";

    player.viewAngle += 2.0*M_PI/360.0;

    renderPlayerView(pv, map, player, playerviewName.str(), textures);
    //renderDualView(pv, td, map, player, topdownName.str(), playerviewName.str(), textures);
  }

  return 0;
}