#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <array>
#include <optional>

#include <glm/glm.hpp>

namespace TR {

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

struct BrushEntity {
   std::optional<glm::vec3> origin;
   std::unordered_map<std::string, std::string> properties;
   std::vector<Brush> brushes;
};

struct VertexData {
   glm::vec3 vertices;
   glm::vec2 uvs;
};

struct Face {
   std::vector<VertexData> vertices;
   std::vector<uint16_t> indices;
};

struct BuiltEntity {
   std::unordered_map<std::string, std::string> properties;
   std::optional<Face> geo;
};

using SerializableData = std::vector<BuiltEntity>;

} // namespace TR
