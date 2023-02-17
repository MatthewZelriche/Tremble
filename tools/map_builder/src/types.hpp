#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <array>
#include <optional>

#include <glm/glm.hpp>

namespace TR {

/****************************************************
 * Types exclusive to the parser
 ****************************************************/
struct PlaneEq {
   glm::vec3 normal;
   glm::vec3 point;
   float dist;
};

struct Plane {
   PlaneEq equation;
   std::string texturePath;
   std::array<glm::vec4, 2> uvs;
   uint32_t texRot {0};
   uint32_t uScale {0};
   uint32_t vScale {0};
};

struct Brush {
   std::vector<Plane> planes;
};

struct MapEntity {
   std::optional<glm::vec3> origin;
   std::unordered_map<std::string, std::string> properties;
   std::vector<Brush> brushes;
};

/****************************************************
 * Types exclusive to geometry constructor
 ****************************************************/
struct VertexData {
   glm::vec3 vertices;
   glm::vec2 uvs;
};

struct Face {
   std::vector<VertexData> vertices;
   std::vector<uint16_t> indices;
};

} // namespace TR
