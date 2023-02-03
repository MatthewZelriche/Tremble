#pragma once

#include <cstdint>

namespace TR {

struct Vec3 {
   int32_t x {0};
   int32_t y {0};
   int32_t z {0};
};

struct Vec4 {
   int32_t x;
   int32_t y;
   int32_t z;
   int32_t w;
};

} // namespace TR