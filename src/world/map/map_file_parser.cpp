#include "map_file_parser.hpp"

#include <fstream>
#include <climits>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <stack>
#include <cstring>
#include <glm/gtx/normal.hpp>
#include <glm/gtc/epsilon.hpp>

using namespace TR;

CSGMap::CSGMap(std::string_view filename) {
   // Read in file
   std::ifstream fileStream(filename.data(), std::ifstream::in);
   if (!fileStream.is_open()) { throw std::runtime_error("Failed to open file"); }
   std::string currLine;

   while (std::getline(fileStream, currLine)) {
      if (currLine.empty() || currLine.substr(0, 2) == "//") {
         // Ignore commented and empty line
         continue;
      }

      if (currLine == "}") {
         throw std::runtime_error("Unexpected open brace");
      } else if (currLine == "{") {
         // New entity scope
         BuildEntity(fileStream);
      }
   }
}

MapEntity &CSGMap::GetEntity(const std::string &classname) {
   if (!mEntities.count(classname)) {
      throw std::runtime_error("No entity of classname " + classname);
   }
   return mEntities[classname];
}

void CSGMap::BuildEntity(std::ifstream &def) {
   std::string currLine;
   MapEntity ent;
   std::string classname;
   while (std::getline(def, currLine)) {
      if (currLine.empty() || currLine.substr(0, 2) == "//") {
         // Ignore commented and empty line
         continue;
      }

      // Must be the start of a brush definition
      if (currLine == "{") {
         // New brush scope
         ent.brushes.push_back(BuildBrush(def));
      }
      // Reached the end of this entity def, commit entity
      else if (currLine == "}") {
         break;
      }
      // Only option left is that this is an entity property
      else {
         // Quick sanity check to ensure its an entity property
         if (currLine[0] != '\"') {
            throw std::runtime_error("Invalid entity property found on line: " +
                                     currLine);
         }
         OptPair pair = ParseProperty(currLine);
         if (!pair.has_value()) { continue; }

         // Sanity check for no duplicate property defines
         if (ent.properties.count(pair.value().first)) {
            throw std::runtime_error("Multiple definition of property on line: " +
                                     currLine);
         }
         if (pair.value().first == "classname") {
            classname = pair.value().second;
         } else {
            ent.properties.insert(pair.value());
         }
      }
   }

   if (classname.empty()) {
      throw std::runtime_error("Entity definition missing classname");
   }

   mEntities.insert({classname, ent});
}

MapBrush CSGMap::BuildBrush(std::ifstream &def) {
   std::string currLine;
   MapBrush brush;
   while (std::getline(def, currLine)) {
      if (currLine.empty() || currLine.substr(0, 2) == "//") {
         // Ignore commented and empty line
         continue;
      }

      if (currLine == "{") {
         throw std::runtime_error("Unexpected open brace");
      } else if (currLine == "}") {
         // Done brush, commit
         break;
      } else {
         // Half-space definition, including texture info
         brush.faceData.push_back(ParseFaceData(currLine));
      }
   }

   size_t numFaces = brush.faceData.size();
   // We've parsed the brush, now we have to actually build its vertices
   brush.polys.resize(numFaces);
   for (int i = 0; i <= numFaces - 3; i++) {
      for (int j = i; j <= numFaces - 2; j++) {
         for (int k = j; k <= numFaces - 1; k++) {
            if (i != j != k) {
               bool validVert = true;
               std::optional<glm::vec3> vert = HalfSpaceIntersect(
                   brush.faceData.at(i).plane, brush.faceData.at(j).plane,
                   brush.faceData.at(k).plane);
               if (vert.has_value()) {
                  for (int l = 0; l < numFaces; l++) {
                     if (glm::dot(brush.faceData.at(l).plane.normal, vert.value()) +
                             brush.faceData.at(l).plane.dist >
                         0) {
                        validVert = false;
                        break;
                     }
                  }
                  if (validVert) {
                     brush.polys.at(i).vertices.push_back(vert.value());
                     brush.polys.at(j).vertices.push_back(vert.value());
                     brush.polys.at(k).vertices.push_back(vert.value());

                     // TODO: UVs
                  }
               }
            }
         }
      }
   }

   return brush;
}

OptPair CSGMap::ParseProperty(const std::string &line) {
   // Extract key and value, not including the enclosing quotation marks
   size_t closeQuoteKey = line.find("\"", 1);
   size_t gapSize = std::strlen("\" \"");
   size_t closeQuoteVal = line.find("\"", closeQuoteKey + gapSize);
   std::string propertyKey = line.substr(1, closeQuoteKey - 1);
   std::string propertyVal =
       line.substr(closeQuoteKey + gapSize, closeQuoteVal - (closeQuoteKey + gapSize));

   // Don't store mapversion, just do a quick sanity check
   if (propertyKey == "mapversion") {
      if (propertyVal != "220") { throw std::runtime_error("Invalid map version"); }
      return std::nullopt;
   } else {
      return OptPair({propertyKey, propertyVal});
   }
}

PlaneData CSGMap::ComputePlane(const glm::vec3 &p1, const glm::vec3 &p2,
                               const glm::vec3 &p3) {
   PlaneData plane;
   plane.normal = glm::triangleNormal(p1, p2, p3);
   plane.point = p1;
   plane.dist =
       (plane.normal.x * -p1.x) + (plane.normal.y * -p1.y) + (plane.normal.z * -p1.z);

   return plane;
}

// https://raw.githubusercontent.com/stefanha/map-files/master/MAPFiles.pdf
std::optional<glm::vec3> CSGMap::HalfSpaceIntersect(PlaneData plane1, PlaneData plane2,
                                                    PlaneData plane3) {
   float denom = glm::dot(plane1.normal, glm::cross(plane2.normal, plane3.normal));
   if (glm::epsilonEqual(denom, 0.0f, FLT_EPSILON)) { return std::nullopt; }

   glm::vec3 vert = (-plane1.dist * glm::cross(plane2.normal, plane3.normal) -
                     plane2.dist * glm::cross(plane3.normal, plane1.normal) -
                     plane3.dist * glm::cross(plane1.normal, plane2.normal)) /
                    denom;
   return vert;
}

FaceData CSGMap::ParseFaceData(std::string_view def) {
   FaceData data;
   size_t hsEnd = def.find_last_of(")");
   size_t textureEnd = def.find(" ", hsEnd + 2);
   data.texture = def.substr(hsEnd + 2, textureEnd - (hsEnd + 2));

   glm::vec3 p1;
   glm::vec3 p2;
   glm::vec3 p3;
   // clang-format off
   int res = std::sscanf(def.data(), halfPlaneFmt.c_str(), &p1.x,
                                                           &p1.y,
                                                           &p1.z,
                                                           &p2.x,
                                                           &p2.y, 
                                                           &p2.z,
                                                           &p3.x,
                                                           &p3.y, 
                                                           &p3.z,
                                                           &data.uvs[0].x, 
                                                           &data.uvs[0].y,
                                                           &data.uvs[0].z,
                                                           &data.uvs[0].w,
                                                           &data.uvs[1].x, 
                                                           &data.uvs[1].y,
                                                           &data.uvs[1].z,
                                                           &data.uvs[1].w,
                                                           &data.texRot,
                                                           &data.uScale,
                                                           &data.vScale);
   // clang-format on
   // Calculation plane equation
   data.plane = ComputePlane(p1, p2, p3);

   if (res != 20) { throw std::runtime_error("Malformatted half-space definition"); }
   return data;
}