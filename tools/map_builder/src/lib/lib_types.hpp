#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <array>
#include <optional>

#include "math.hpp"

namespace TR {

struct Plane {
   PlaneEq equation;
   std::string texturePath;
   std::array<Vec4, 2> uvs;
   float texRot {0};
   float uScale {0};
   float vScale {0};
};

struct Brush {
   std::vector<Plane> planes;
};

struct BrushEntity {
   std::unordered_map<std::string, std::string> properties;
   std::vector<Brush> brushes;
};

struct VertexData {
   Vec3 vertices;
   Vec2 uvs;
};

struct Face {
   Vec3 normalDir;
   std::vector<VertexData> vertices;
   std::vector<unsigned int> indices; // TODO: Performance considerations
};

struct BuiltEntity {
   std::unordered_map<std::string, std::string> properties;
   std::optional<std::vector<std::vector<Face>>> geo;
};

using SerializableData = std::vector<BuiltEntity>;

} // namespace TR
