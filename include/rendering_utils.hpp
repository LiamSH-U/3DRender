#ifndef RENDERING_UTILS_HPP
#define RENDERING_UTILS_HPP

#include "structs.hpp"
#include "Textures.hpp"
#include "user_specs.hpp"
#include <cstdint>
#include <vector>
#include <string>

#define WHITE 4294967295
#define BLACK 4278190080
#define RED 4278190335
#define GREEN 4278255360
#define BLUE 4294901760

uint32_t packColour(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 255);

void unpackColour(const uint32_t &color, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a);

void drawRect(std::vector<uint32_t>& img, const size_t imgW, const size_t imgH,
              const size_t xPos, const size_t yPos, const size_t rectW, const size_t rectH,
              const uint32_t colour);

void writePPMImg(const std::string filename, const std::vector<uint32_t>& img,
                  const size_t w, const size_t h);

void renderTopDown(std::vector<uint32_t>& img, const Resolutions res, const char* map,
                   const Player player, std::string filename, Textures& textures);

void renderPlayerView(std::vector<uint32_t>& img, const Resolutions res, const char* map,
                      const Player player, std::string filename, Textures& textures);

void renderDualView(std::vector<uint32_t>& td, std::vector<uint32_t>& pv,
                    const Resolutions res, const char* map, const Player player,
                    std::string tdFilename, std::string pvFilename, Textures& textures);

#endif