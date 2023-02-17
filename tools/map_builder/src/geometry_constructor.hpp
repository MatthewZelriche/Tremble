#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <optional>
#include <unordered_map>

#include "file_parser.hpp"

namespace TR {

class GeometryConstructor {
  public:
   GeometryConstructor(const MapFileParser &parsedMap);
   // TODO: Figure out a less dumb way to do this
   SerializableData &&GetBuiltData();

  private:
   SerializableData mData;
};

} // namespace TR