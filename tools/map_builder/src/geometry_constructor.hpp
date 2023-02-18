#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <optional>
#include <unordered_map>

#include "types.hpp"

namespace TR {

class GeometryConstructor {
  public:
   GeometryConstructor();

   std::vector<Face> Build(const Brush &brush);

  private:
   glm::ivec2 GetTextureDims(std::string texPathWithExt);
   glm::vec2 ComputeTexCoords(const glm::vec3 &vert, const Plane &plane);
   std::optional<glm::vec3> HalfSpaceIntersect(PlaneEq plane1, PlaneEq plane2,
                                               PlaneEq plane3);
   std::unordered_map<std::string, glm::vec2> mTexDimCache;
};

} // namespace TR