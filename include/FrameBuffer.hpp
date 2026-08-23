#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <cstdint>
#include <vector>

class FrameBuffer {
public:
  size_t w, h;
  std::vector<uint32_t> buffer;

  FrameBuffer(const size_t winW_in, const size_t winH_in);

  uint32_t getPx(const size_t x, const size_t y);

  void setPx(const size_t x, const size_t y, uint32_t px);

  void wipe();
};

#endif