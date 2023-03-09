#pragma once

#include "cereal/cereal.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/unordered_map.hpp"
#include "cereal/types/optional.hpp"

#include "lib_types.hpp"

namespace cereal {

template<class Archive>
void serialize(Archive &archive, TR::Vec2 &v) {
   archive(v.x, v.y);
}

template<class Archive>
void serialize(Archive &archive, TR::Vec3 &v) {
   archive(v.x, v.y, v.z);
}

template<class Archive>
void serialize(Archive &archive, TR::VertexData &vert) {
   archive(vert.vertices, vert.uvs);
}

template<class Archive>
void serialize(Archive &archive, TR::Face &face) {
   archive(face.normalDir, face.textureID, face.vertices, face.indices);
}

template<class Archive>
void serialize(Archive &archive, TR::BuiltEntity &entity) {
   archive(entity.properties, entity.geo);
}

template<class Archive>
void serialize(Archive &archive, TR::Map &map, std::uint32_t const ver) {
   if (ver != TR::MAP_VERS) {
      throw std::runtime_error("Invalid trmap version: " +
                               std::string(std::to_string(ver)));
   }
   archive(map.textureTable, map.entities);
}
} // namespace cereal
CEREAL_CLASS_VERSION(TR::Map, TR::MAP_VERS);

namespace TR {
void SerializeToDisk(std::string_view filename, const Map &data);
Map SerializeFromDisk(std::string_view filename);
} // namespace TR