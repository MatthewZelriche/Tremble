#include "geometry_constructor.hpp"

#include <glm/gtc/epsilon.hpp>
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace TR;

GeometryConstructor::GeometryConstructor() {}

std::vector<Face> GeometryConstructor::Build(const Brush &brush) {
   std::vector<Face> faces;
   size_t numPlanes = brush.planes.size();
   faces.resize(numPlanes);

   for (int i = 0; i <= numPlanes - 3; i++) {
      for (int j = i; j <= numPlanes - 2; j++) {
         for (int k = j; k <= numPlanes - 1; k++) {
            if (i != j != k) {
               bool validVert = true;
               std::optional<glm::vec3> vert = HalfSpaceIntersect(
                   brush.planes.at(i).equation, brush.planes.at(j).equation,
                   brush.planes.at(k).equation);
               if (vert.has_value()) {
                  for (int l = 0; l < numPlanes; l++) {
                     float bab =
                         glm::dot(brush.planes.at(l).equation.normal, vert.value());
                     float dist = brush.planes.at(l).equation.dist;
                     float ttl = bab + dist;
                     if (ttl > 0) {
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
                     faces.at(j).vertices.push_back(dataj);
                     faces.at(k).vertices.push_back(datak);
                  }
               }
            }
         }
      }
   }
   return faces;
}

// TODO: IMPORTANT! Proper handling of texture directory
glm::vec2 GeometryConstructor::ComputeTexCoords(const glm::vec3 &vert,
                                                const Plane &plane) {
   glm::vec2 coords;
   auto dims = GetTextureDims(plane.texturePath + ".png");

   coords.x = ((glm::dot(vert, glm::vec3(plane.uvs[0])) / dims.x) / plane.uScale) +
              (plane.uvs[0].w / dims.x);
   coords.y = ((glm::dot(vert, glm::vec3(plane.uvs[1])) / dims.y) / plane.vScale) +
              (plane.uvs[1].w / dims.y);

   return coords;
}

// TODO: Could I only store the hash value for the string instead of the whole string?
glm::ivec2 GeometryConstructor::GetTextureDims(std::string texPathWithExt) {
   if (mTexDimCache.count(texPathWithExt) != 0) { return mTexDimCache[texPathWithExt]; }

   glm::ivec2 dims {0};
   int channels = 0;
   if (!stbi_info(texPathWithExt.c_str(), &dims.x, &dims.y, &channels)) {
      throw std::runtime_error("Missing texture " + texPathWithExt);
   }

   mTexDimCache.insert({texPathWithExt, dims});
   return dims;
}

// https://raw.githubusercontent.com/stefanha/map-files/master/MAPFiles.pdf
std::optional<glm::vec3> GeometryConstructor::HalfSpaceIntersect(PlaneEq plane1,
                                                                 PlaneEq plane2,
                                                                 PlaneEq plane3) {
   float denom = glm::dot(plane1.normal, glm::cross(plane2.normal, plane3.normal));
   if (glm::epsilonEqual(denom, 0.0f, FLT_EPSILON)) { return std::nullopt; }

   glm::vec3 vert = (-plane1.dist * glm::cross(plane2.normal, plane3.normal) -
                     plane2.dist * glm::cross(plane3.normal, plane1.normal) -
                     plane3.dist * glm::cross(plane1.normal, plane2.normal)) /
                    denom;
   return vert;
}