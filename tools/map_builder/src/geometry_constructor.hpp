#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <optional>
#include <unordered_map>

#include "types.hpp"

namespace TR {

enum PlanePos { FRONT, BACK, ON };

class GeometryConstructor {
  public:
   GeometryConstructor();

   std::vector<Face> Build(const Brush &brush);

  private:
   PlanePos PointPos(const PlaneEq &plane, const glm::vec3 &point);
   glm::vec3 GetFaceCenter(const Face &face);
   std::vector<VertexData> SortVertices(const Face &unsorted);
   glm::ivec2 GetTextureDims(std::string texPathWithExt);
   glm::vec2 ComputeTexCoords(const glm::vec3 &vert, const Plane &plane);
   std::optional<glm::vec3> HalfSpaceIntersect(PlaneEq plane1, PlaneEq plane2,
                                               PlaneEq plane3);
   std::unordered_map<std::string, glm::vec2> mTexDimCache;
};

} // namespace TR