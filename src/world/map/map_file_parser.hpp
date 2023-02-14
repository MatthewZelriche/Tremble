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

struct PlaneData {
   glm::vec3 normal;
   glm::vec3 point;
   float dist;
};

struct FaceData {
   PlaneData plane;
   std::string texture;
   std::array<glm::vec4, 2> uvs;
   uint32_t texRot {0};
   uint32_t uScale {0};
   uint32_t vScale {0};
};

struct Poly {
   std::vector<glm::vec3> vertices;
   std::vector<float> indices;
};

struct MapBrush {
   std::vector<FaceData> faceData;
   std::vector<Poly> polys;
};

struct MapEntity {
   std::vector<MapBrush> brushes;
   std::unordered_map<std::string, std::string> properties;
};

class CSGMap {
  public:
   CSGMap(std::string_view filename);

   MapEntity &GetEntity(const std::string &classname);

  private:
   void BuildEntity(std::ifstream &def);
   MapBrush BuildBrush(std::ifstream &def);
   OptPair ParseProperty(const std::string &line);
   FaceData ParseFaceData(std::string_view def);
   PlaneData ComputePlane(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3);
   std::optional<glm::vec3> HalfSpaceIntersect(PlaneData plane1, PlaneData plane2,
                                               PlaneData plane3);

   std::unordered_map<std::string, MapEntity> mEntities;

   const std::string halfPlaneFmt = "( %f %f %f ) ( %f %f %f ) ( %f %f %f ) %*s "
                                    " [ %f %f %f %f ] [ %f %f %f %f ] %d %d %d";
};

} // namespace TR