#include "file_parser.hpp"

#include <stdexcept>
#include <fstream>
#include <iostream>

#include "math.hpp"

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
         mEntities.push_back(ParseEntity(fileStream));
      } else {
         // Unexpected error
         throw std::runtime_error("Unexpected start of new line");
      }
   }

   // Sanity checks - must contain one worldspawn
   if (!mHasWorldspawn) {
      throw std::runtime_error("Map file did not contain worldspawn");
   } else if (!mMapVersionDefined) {
      throw std::runtime_error("No map version defined in map file");
   }

   // Build geometry and finalize entities for serialization
   std::cout << "Building geometry from planes..." << std::endl;
   GeometryConstructor geoConstructor;
   for (auto &ent : mEntities) {
      // Copy the properties no matter what
      BuiltEntity built;
      built.properties = ent.properties;

      if (ent.brushes.size() > 0) {
         // Also need to build geo
         built.geo = std::vector<std::vector<Face>>();
         for (auto &brush : ent.brushes) {
            built.geo.value().push_back(geoConstructor.Build(brush));
         }
      }

      mBuiltEntities.push_back(built);
   }
}

BrushEntity MapFileParser::ParseEntity(std::ifstream &entityDef) {
   BrushEntity ent;
   std::string currLine;

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
            mMapVersionDefined = true;
         }
         ent.properties.insert(property);
      }
   }

   // TODO: Multithreading: Barrier to synchronize all properties parsed
   // before checking for an origin?

   // Extract classname, no need to store it in the properties because
   // its implicitly stored as this entity's key.
   if (ent.properties.count("classname") == 0) {
      throw std::runtime_error("Entity was lacking a classname property");
   }
   // Sanity check to ensure at least one worldspawn exists
   if (ent.properties["classname"] == "worldspawn") { mHasWorldspawn = true; }

   return ent;
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

Plane MapFileParser::ParsePlane(std::string_view def) {
   static std::string halfPlaneFmt = "( %f %f %f ) ( %f %f %f ) ( %f %f %f ) %*s "
                                     " [ %f %f %f %f ] [ %f %f %f %f ] %d %d %d";

   Plane plane;
   size_t hsEnd = def.find_last_of(")");
   size_t textureEnd = def.find(" ", hsEnd + 2);
   plane.texturePath = def.substr(hsEnd + 2, textureEnd - (hsEnd + 2));

   Vec3 p1;
   Vec3 p2;
   Vec3 p3;
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
   plane.equation = CreatePlane(p1, p2, p3);

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