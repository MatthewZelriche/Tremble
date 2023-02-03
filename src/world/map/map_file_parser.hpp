#pragma once

#include <string>
#include <optional>
#include <vector>
#include <unordered_map>
#include <array>
#include <cstdint>
#include <glm/glm.hpp>

namespace TR {

using OptPair = std::optional<std::pair<std::string, std::string>>;

struct FaceData {
   std::array<glm::highp_dvec3, 3> planes;
   std::string texture;
   std::array<glm::highp_dvec4, 2> uvs;
   uint32_t texRot {0};
   uint32_t uScale {0};
   uint32_t vScale {0};
};

struct MapBrush {
   std::vector<FaceData> faceData;
};

struct MapEntity {
   std::vector<MapBrush> brushes;
   std::unordered_map<std::string, std::string> properties;
};

class MapFileParser {
  public:
   MapFileParser(std::string_view filename);

  private:
   MapEntity BuildEntity(std::ifstream &def);
   MapBrush BuildBrush(std::ifstream &def);
   OptPair ParseProperty(const std::string &line);
   FaceData ParseFaceData(std::string_view def);
   std::vector<MapEntity> mEntities;

   const std::string halfPlaneFmt = "( %lf %lf %lf ) ( %lf %lf %lf ) ( %lf %lf %lf ) %*s "
                                    " [ %lf %lf %lf %lf ] [ %lf %lf %lf %lf ] %d %d %d";
};

} // namespace TR