#pragma once

#include <glm/glm.hpp>
#include "types.hpp"

namespace TR {
PlaneEq ComputePlane(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3);
}
