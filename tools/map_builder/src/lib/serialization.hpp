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
   archive(face.normalDir, face.texturePath, face.vertices, face.indices);
}

template<class Archive>
void serialize(Archive &archive, TR::BuiltEntity &entity) {
   archive(entity.properties, entity.geo);
}
} // namespace cereal

namespace TR {
void SerializeToDisk(std::string_view filename, const SerializableData &data);
SerializableData SerializeFromDisk(std::string_view filename);
} // namespace TR