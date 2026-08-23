#ifndef RENDERING_HPP
#define RENDERING_HPP

#include <cstdint>
#include <vector>
#include "structs.h"
#include "Textures.hpp"

// these function signatures are seriously fucked

void renderTopDown(std::vector<uint32_t>& img, const Resolutions res, const char* map,
                   const Player player, std::string filename, Textures& textures);

void renderPlayerView(std::vector<uint32_t>& img, const Resolutions res, const char* map,
                      const Player player, std::string filename, Textures& textures);

void renderDualView(std::vector<uint32_t>& td, std::vector<uint32_t>& pv,
                    const Resolutions res, const char* map, const Player player,
                    std::string tdFilename, std::string pvFilename, Textures& textures);

#endif