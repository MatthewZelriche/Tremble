#pragma once

#include <string>
#include <optional>

#include "lib_types.hpp"
#include "geometry_constructor.hpp"

namespace TR {

using OptPair = std::optional<std::pair<std::string, std::string>>;

class MapFileParser {
  public:
   /**
   * @brief Parses a map file located at filename
   * 
   * @throws runtime_error if an unrecoverable parsing error occurs.
   * @param filename The path of the map file to parse.
   */
   MapFileParser(std::string_view filename);
   void SaveToBinaryFile(std::string_view filename);

  private:
   /**
    * @brief Parses a single brace-enclosed entity
    * 
    * Upon return, the input filestream will be modified so that the entity definition 
    * is deleted from the filestream, and the position will now be at the line 
    * immediately following the closing bracket for this entity definition.
    * 
    * @throw runtime_error if an unrecoverable parsing error occurs.
    * @param entityDef Position in the filestream where the entity definition
    * appears, starting at the line immediately after the opening brace.
    * @return A parsed brush entity.
    */
   BrushEntity ParseEntity(std::ifstream &entityDef);
   /**
    * @brief Parses a single property within an entity definition.
    * 
    * @throw runtime_error if an unrecoverable parsing error occurs.
    * @param propertyDef An immutable reference to a single property definition.
    * @return A pair, whose key is the property name, and whose value is the property value.
    */
   std::pair<std::string, std::string> ParseProperty(const std::string &propertyDef);
   /**
    * @brief Parse a single brush, containing at least four plane definitions
    * 
    * Upon return, the input filestream will be modified so that the brush definition 
    * is deleted from the filestream, and the position will now be at the line 
    * immediately following the closing bracket for this entity definition.
    * 
    * @throw runtime_error if an unrecoverable parsing error occurs.
    * @param def Position in the filestream where the brush definition appears, starting
    * at the line immediately after the opening brace.
    * @return A parsed brush struct, containing at least four plane definitions.
    */
   Brush ParseBrush(std::ifstream &def);
   /**
    * @brief Parse a single plane definition inside a brush definition.
    * 
    * @throw runtime_error if an unrecoverable parsing error occurs.
    * @param def A single plane definition.
    * @return A parsed plane struct.
    */
   Plane ParsePlane(std::string_view def);

   bool mHasWorldspawn {false};
   bool mMapVersionDefined {false};
   std::vector<BrushEntity> mEntities;

   // Needed for fast lookup - increased memory, but this tool isn't perf or memory sensitive
   std::unordered_map<std::string, uint32_t> mTextureIndices;

   Map mMap;
};

} // namespace TR