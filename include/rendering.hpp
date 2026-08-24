#ifndef RENDERING_HPP
#define RENDERING_HPP

#include <cstdint>
#include <vector>
#include "structs.h"
#include "Textures.hpp"
#include "FrameBuffer.hpp"
#include "Map.hpp"

// void renderTopDown(FrameBuffer& fb, Map& map, const Player player,
//                    std::vector<Sprite>& enemies, std::string filename, Textures& textures);

void renderPlayerView(FrameBuffer& fb, Map& map, const Player player,
                      std::vector<Sprite>& enemies, Textures& wallTex, Textures& enemiesTex);

// void renderDualView(FrameBuffer& pv, FrameBuffer& td, Map& map, const Player player,
//                     std::vector<Sprite>& enemies, std::string tdFilename,
//                     std::string pvFilename, Textures& textures);

#endif