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
   uint32_t textureID;
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
   uint32_t textureID;
   std::vector<VertexData> vertices;
   std::vector<unsigned int> indices; // TODO: Performance considerations
};

using BuiltBrush = std::vector<Face>;

// TODO: Don't forget to modify serialization function
struct ConvexCollider {};

struct BuiltEntity {
   std::unordered_map<std::string, std::string> properties;
   std::optional<std::vector<ConvexCollider>> collisionData;
};

// Sort vertices by texture to avoid texture binding overhead
struct MapRenderData {
   uint32_t textureID;
   std::vector<VertexData> verts;
   std::vector<unsigned int> indices;
};

constexpr uint32_t MAP_VERS = 3;
struct Map {
   std::vector<std::string> textureTable;
   std::vector<MapRenderData> renderData;
   std::vector<BuiltEntity> entities;
};
} // namespace TR
