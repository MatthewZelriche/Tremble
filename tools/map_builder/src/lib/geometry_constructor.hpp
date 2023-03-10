#pragma once

#include <vector>
#include <optional>
#include <unordered_map>

#include "lib_types.hpp"
#include "math.hpp"

namespace TR {

enum PointPos { FRONT, BACK, ON };

class GeometryConstructor {
  public:
   GeometryConstructor();

   BuiltBrush Build(const Brush &brush);

  private:
   std::vector<unsigned int> GenerateIndices(const Face &face);
   PointPos GetPointPos(const PlaneEq &plane, const Vec3 &point);
   Vec3 GetFaceCenter(const Face &face);
   std::vector<VertexData> SortVertices(const Face &unsorted);
   IVec2 GetTextureDims(std::string texPathWithExt);
   Vec2 ComputeTexCoords(const Vec3 &vert, const Plane &plane);
   std::optional<Vec3> HalfSpaceIntersect(PlaneEq plane1, PlaneEq plane2, PlaneEq plane3);
   std::unordered_map<std::string, Vec2> mTexDimCache;
};

} // namespace TR