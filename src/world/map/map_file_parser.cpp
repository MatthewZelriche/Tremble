#include "map_file_parser.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <stack>

using namespace TR;

MapFileParser::MapFileParser(std::string_view filename) {
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
         mEntities.push_back(BuildEntity(fileStream));
      }
   }
}

MapEntity MapFileParser::BuildEntity(std::ifstream &def) {
   std::string currLine;
   MapEntity ent;
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
         if (!ent.properties.count("classname")) {
            throw std::runtime_error("Entity missing classname definition");
         }
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
         ent.properties.insert(pair.value());
      }
   }
   return ent;
}

MapBrush MapFileParser::BuildBrush(std::ifstream &def) {
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

   // We've parsed the brush, now we have to actually build it.
   return brush;
}

OptPair MapFileParser::ParseProperty(const std::string &line) {
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

FaceData MapFileParser::ParseFaceData(std::string_view def) {
   FaceData data;
   size_t hsEnd = def.find_last_of(")");
   size_t textureEnd = def.find(" ", hsEnd + 2);
   data.texture = def.substr(hsEnd + 2, textureEnd - (hsEnd + 2));

   // clang-format off
   int res = std::sscanf(def.data(), halfPlaneFmt.c_str(), &data.planes[0].x,
                                                           &data.planes[0].y,
                                                           &data.planes[0].z,
                                                           &data.planes[1].x,
                                                           &data.planes[1].y, 
                                                           &data.planes[1].z,
                                                           &data.planes[2].x,
                                                           &data.planes[2].y, 
                                                           &data.planes[2].z,
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

   if (res != 20) { throw std::runtime_error("Malformatted half-space definition"); }
   return data;
}