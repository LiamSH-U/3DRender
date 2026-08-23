#include "FrameBuffer.hpp"

#define WHITE 4294967295

FrameBuffer::FrameBuffer(const size_t winW_in, const size_t winH_in)
  : w(winW_in), h(winH_in), buffer(std::vector<uint32_t>(w*h)) {
}

uint32_t FrameBuffer::getPx(const size_t x, const size_t y) { return buffer[x + y*w]; }

void FrameBuffer::setPx(const size_t x, const size_t y, const uint32_t px) {
  buffer[x + y*w] = px;
}

void FrameBuffer::wipe() { buffer = std::vector<uint32_t>(w*h, WHITE); }