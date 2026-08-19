#ifndef RENDERING_HPP
#define RENDERING_HPP

#include "user_specs.hpp"
#include "utilities.hpp"
#include <cstdint>
#include <vector>
#include <string>

void drawRect(std::vector<uint32_t>& img, const size_t imgW, const size_t imgH,
              const size_t xPos, const size_t yPos, const size_t rectW, const size_t rectH,
              const uint32_t colour);

void writePPMImg(const std::string filename, const std::vector<uint32_t>& img,
                  const size_t w, const size_t h);

void renderTopDown(std::vector<uint32_t>& img, const Resolutions res, const char* map,
                   const Player player, std::string filename);

void renderPlayerView(std::vector<uint32_t>& img, const Resolutions res, const char* map,
                      const Player player, std::string filename);

void renderDualView(std::vector<uint32_t>& td, std::vector<uint32_t>& pv,
                    const Resolutions res, const char* map, const Player player,
                    std::string tdFilename, std::string pvFilename);

void animate360View();

#endif