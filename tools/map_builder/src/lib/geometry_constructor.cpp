#include "geometry_constructor.hpp"

#include <stdexcept>
#include <algorithm>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace TR;

GeometryConstructor::GeometryConstructor() {}

BuiltBrush GeometryConstructor::Build(const Brush &brush) {
   BuiltBrush faces;
   size_t numPlanes = brush.planes.size();
   faces.resize(numPlanes);

   for (int i = 0; i <= numPlanes - 3; i++) {
      for (int j = i; j <= numPlanes - 2; j++) {
         for (int k = j; k <= numPlanes - 1; k++) {
            if (i != j != k) {
               bool validVert = true;
               std::optional<Vec3> vert = HalfSpaceIntersect(brush.planes.at(i).equation,
                                                             brush.planes.at(j).equation,
                                                             brush.planes.at(k).equation);
               if (vert.has_value()) {
                  for (int l = 0; l < numPlanes; l++) {
                     if (GetPointPos(brush.planes.at(l).equation, vert.value()) ==
                         PointPos::FRONT) {
                        validVert = false;
                        break;
                     }
                  }

                  if (validVert) {
                     VertexData datai;
                     VertexData dataj;
                     VertexData datak;

                     datai.vertices = vert.value();
                     dataj.vertices = vert.value();
                     datak.vertices = vert.value();

                     datai.uvs = ComputeTexCoords(datai.vertices, brush.planes.at(i));
                     dataj.uvs = ComputeTexCoords(datai.vertices, brush.planes.at(j));
                     datak.uvs = ComputeTexCoords(datai.vertices, brush.planes.at(k));

                     faces.at(i).vertices.push_back(datai);
                     faces.at(i).normalDir = brush.planes.at(i).equation.normal;
                     faces.at(i).textureID = brush.planes.at(i).textureID;
                     faces.at(j).vertices.push_back(dataj);
                     faces.at(j).normalDir = brush.planes.at(j).equation.normal;
                     faces.at(j).textureID = brush.planes.at(j).textureID;
                     faces.at(k).vertices.push_back(datak);
                     faces.at(k).normalDir = brush.planes.at(k).equation.normal;
                     faces.at(k).textureID = brush.planes.at(k).textureID;
                  }
               }
            }
         }
      }
   }

   for (auto &face : faces) {
      // Before producing the indices, we should sort the faces verts
      face.vertices = SortVertices(face);
      face.indices = GenerateIndices(face);
   }

   return faces;
}

std::vector<unsigned int> GeometryConstructor::GenerateIndices(const Face &face) {
   std::vector<unsigned int> indices;
   // Each face must consist of at least 1 triangle
   // After accounting for this 1 guarunteed triangle, there are
   // n-3 vertices remaining (can be zero)
   int numTris = 1;
   int remainingVerts = face.vertices.size() - 3;
   // Each additional triangle in this face (if it exists) always adds exactly 1 vertex.
   numTris += remainingVerts;
   for (int i = 0; i < numTris; i++) {
      // Each triangle has a predictable pattern
      indices.push_back(0);
      indices.push_back(i + 1);
      indices.push_back(i + 2);
   }

   return indices;
}

#include <glm/glm.hpp>

PointPos GeometryConstructor::GetPointPos(const PlaneEq &plane, const Vec3 &point) {
   float dist = SignedDistToPlane(plane, point);
   if (dist > BIG_EPS) {
      return PointPos::FRONT;
   } else if (dist < -BIG_EPS) {
      return PointPos::BACK;
   } else {
      return PointPos::ON;
   }
}

// Sorts verts counterclockwise
std::vector<VertexData> GeometryConstructor::SortVertices(const Face &unsorted) {
   std::vector<VertexData> sorted = std::vector<VertexData>(unsorted.vertices);
   Vec3 faceCenter = GetFaceCenter(unsorted);

   // from https://github.com/QodotPlugin/Qodot/blob/main/addons/qodot/src/core/GeoGenerator.cs
   Vec3 wind = Normalize(unsorted.vertices[1].vertices - unsorted.vertices[0].vertices);
   Vec3 normal = unsorted.normalDir;
   std::sort(sorted.begin(), sorted.end(),
             [wind, normal, faceCenter](VertexData l, VertexData r) {
                Vec3 a = wind;
                Vec3 b = CrossProduct(a, normal);

                Vec3 loc_a = l.vertices - faceCenter;
                float a_pu = DotProduct(loc_a, a);
                float a_pv = DotProduct(loc_a, b);

                Vec3 loc_b = r.vertices - faceCenter;
                float b_pu = DotProduct(loc_b, a);
                float b_pv = DotProduct(loc_b, b);

                float a_angle = std::atan2(a_pv, a_pu);
                float b_angle = std::atan2(b_pv, b_pu);

                return a_angle > b_angle ? false : true;
             });

   // Reverse based on normal
   PlaneEq vertPlane =
       CreatePlane(sorted[0].vertices, sorted[1].vertices, sorted[2].vertices);
   if (DotProduct(vertPlane.normal, unsorted.normalDir) > 0) {
      std::reverse(sorted.begin(), sorted.end());
   }

   return sorted;
}

Vec3 GeometryConstructor::GetFaceCenter(const Face &face) {
   Vec3 center = Vec3(0.0f, 0.0f, 0.0f);
   for (const auto &vert : face.vertices) { center += vert.vertices; }
   center /= face.vertices.size();

   return center;
}

// TODO: IMPORTANT! Proper handling of texture directory
Vec2 GeometryConstructor::ComputeTexCoords(const Vec3 &vert, const Plane &plane) {
   Vec2 coords;
   auto dims = GetTextureDims(plane.texturePath + ".png");

   coords.x = ((DotProduct(vert, Vec3(plane.uvs[0])) / dims.x) / plane.uScale) +
              (plane.uvs[0].w / dims.x);
   coords.y = ((DotProduct(vert, Vec3(plane.uvs[1])) / dims.y) / plane.vScale) +
              (plane.uvs[1].w / dims.y);

   return coords;
}

// TODO: Could I only store the hash value for the string instead of the whole string?
IVec2 GeometryConstructor::GetTextureDims(std::string texPathWithExt) {
   if (mTexDimCache.count(texPathWithExt) != 0) { return mTexDimCache[texPathWithExt]; }

   IVec2 dims {0};
   int channels = 0;
   if (!stbi_info(texPathWithExt.c_str(), &dims.x, &dims.y, &channels)) {
      throw std::runtime_error("Missing texture " + texPathWithExt);
   }

   mTexDimCache.insert({texPathWithExt, dims});
   return dims;
}

// https://raw.githubusercontent.com/stefanha/map-files/master/MAPFiles.pdf
std::optional<TR::Vec3> GeometryConstructor::HalfSpaceIntersect(PlaneEq plane1,
                                                                PlaneEq plane2,
                                                                PlaneEq plane3) {
   float denom = DotProduct(plane1.normal, CrossProduct(plane2.normal, plane3.normal));
   if (std::abs(denom - 0.0f) < FLT_EPSILON) { return std::nullopt; }

   Vec3 vert = (-plane1.dist * CrossProduct(plane2.normal, plane3.normal) -
                plane2.dist * CrossProduct(plane3.normal, plane1.normal) -
                plane3.dist * CrossProduct(plane1.normal, plane2.normal)) /
               denom;
   return vert;
}