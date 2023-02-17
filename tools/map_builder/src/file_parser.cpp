#include "file_parser.hpp"

#include <stdexcept>
#include <fstream>

#include <glm/gtx/normal.hpp>

using namespace TR;

MapFileParser::MapFileParser(std::string_view filename) {
   std::ifstream fileStream(filename.data(), std::ifstream::in);
   if (!fileStream.is_open()) { throw std::runtime_error("Failed to open file"); }
   std::string currLine;

   while (std::getline(fileStream, currLine)) {
      if (currLine.empty() || currLine.rfind("//", 0) != std::string::npos) {
         // Throw away commented and empty lines
         continue;
      }

      // Lexing error
      if (currLine == "}") { throw std::runtime_error("Unexpected open brace"); }

      if (currLine == "{") {
         // Valid start of a new entity
         mEntities.insert(ParseEntity(fileStream));
      } else {
         // Unexpected error
         throw std::runtime_error("Unexpected start of new line");
      }
   }

   // Sanity checks - must contain one worldspawn
   if (mEntities.count("worldspawn") == 0) {
      throw std::runtime_error("Map file did not contain worldspawn");
   }
}

std::pair<std::string, MapEntity> MapFileParser::ParseEntity(std::ifstream &entityDef) {
   MapEntity ent;
   std::string currLine;
   std::string classname;
   bool hasMapVersion = false;

   while (std::getline(entityDef, currLine)) {
      if (currLine.empty() || currLine.substr(0, 2) == "//") {
         // Ignore commented and empty line
         continue;
      }

      // Must be the start of a brush definition
      if (currLine == "{") {
         // New brush scope
         ent.brushes.push_back(ParseBrush(entityDef));
      }
      // Reached the end of this entity def, commit entity
      else if (currLine == "}") {
         break;
      }
      // Only option left is that this is an entity property
      else {
         auto property = ParseProperty(currLine);
         if (property.first == "mapversion") {
            // Quick sanity check for correct map version
            if (property.second != "220") {
               throw std::runtime_error("Invalid map version");
            }
            hasMapVersion = true;
         }
         ent.properties.insert(property);
      }
   }

   // Extract classname, no need to store it in the properties because
   // its implicitly stored as this entity's key.
   if (ent.properties.count("classname") == 0) {
      throw std::runtime_error("Entity was lacking a classname property");
   }
   classname = ent.properties["classname"];
   ent.properties.erase("classname");

   // Extract origin, if it exists (not required), since its stored in a separate
   // vec3.
   if (ent.properties.count("origin") != 0) {
      // TODO: Is this a problem for multithreading?
      std::string originStr = ent.properties["origin"];
      size_t firstDelimiter = originStr.find(" ");
      float x = std::stof(originStr.substr(0, firstDelimiter));
      size_t secondDelimiter = originStr.find(" ", firstDelimiter + 1);
      float y = std::stof(
          originStr.substr(firstDelimiter + 1, secondDelimiter - firstDelimiter + 1));
      size_t thirdDelimiter = originStr.length();
      float z = std::stof(
          originStr.substr(secondDelimiter + 1, thirdDelimiter - secondDelimiter + 1));

      ent.origin = {x, y, z};
   }

   return {classname, ent};
}

Brush MapFileParser::ParseBrush(std::ifstream &def) {
   std::string currLine;
   Brush brush;

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
         brush.planes.push_back(ParsePlane(currLine));
      }
   }

   if (brush.planes.size() < 4) {
      throw std::runtime_error("Brush does not contain at least 4 plane definitions");
   }
   return brush;
}

PlaneEq MapFileParser::ComputePlane(const glm::vec3 &p1, const glm::vec3 &p2,
                                    const glm::vec3 &p3) {
   PlaneEq equation;
   equation.normal = glm::triangleNormal(p1, p2, p3);
   equation.point = p1;
   equation.dist = (equation.normal.x * -p1.x) + (equation.normal.y * -p1.y) +
                   (equation.normal.z * -p1.z);

   return equation;
}

Plane MapFileParser::ParsePlane(std::string_view def) {
   static std::string halfPlaneFmt = "( %f %f %f ) ( %f %f %f ) ( %f %f %f ) %*s "
                                     " [ %f %f %f %f ] [ %f %f %f %f ] %d %d %d";

   Plane plane;
   size_t hsEnd = def.find_last_of(")");
   size_t textureEnd = def.find(" ", hsEnd + 2);
   plane.texturePath = def.substr(hsEnd + 2, textureEnd - (hsEnd + 2));

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
                                                           &plane.uvs[0].x, 
                                                           &plane.uvs[0].y,
                                                           &plane.uvs[0].z,
                                                           &plane.uvs[0].w,
                                                           &plane.uvs[1].x, 
                                                           &plane.uvs[1].y,
                                                           &plane.uvs[1].z,
                                                           &plane.uvs[1].w,
                                                           &plane.texRot,
                                                           &plane.uScale,
                                                           &plane.vScale);
   // clang-format on
   plane.equation = ComputePlane(p1, p2, p3);

   if (res != 20) { throw std::runtime_error("Malformatted half-space definition"); }
   return plane;
}

std::pair<std::string, std::string> MapFileParser::ParseProperty(
    const std::string &propertyDef) {
   // Quick sanity check to ensure its an entity property
   if (propertyDef[0] != '\"') { throw std::runtime_error("Invalid entity property"); }

   // Extract key and value, not including the enclosing quotation marks
   size_t closeQuoteKey = propertyDef.find("\"", 1);
   size_t gapSize = std::strlen("\" \"");
   size_t closeQuoteVal = propertyDef.find("\"", closeQuoteKey + gapSize);
   std::string propertyKey = propertyDef.substr(1, closeQuoteKey - 1);
   std::string propertyVal = propertyDef.substr(
       closeQuoteKey + gapSize, closeQuoteVal - (closeQuoteKey + gapSize));

   return {propertyKey, propertyVal};
}