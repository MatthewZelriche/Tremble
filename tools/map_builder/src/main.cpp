#include <iostream>

#include "file_parser.hpp"
#include "geometry_constructor.hpp"

int main() {
   // TODO: Dehardcode filepath
   std::string filepath = "unnamed.map";
   std::cout << "Begin compiling " << filepath << "..." << std::endl;
   try {
      TR::MapFileParser parser = TR::MapFileParser(filepath);
      std::cout << "Successfully completed parsing stage." << std::endl;

      std::cout << "Begin building geometry from planes..." << std::endl;
      try {
         TR::GeometryConstructor constructor = TR::GeometryConstructor(parser);
         std::cout << "Successfully built geometry." << std::endl;
         TR::SerializableData data = constructor.GetBuiltData();
         // TODO: Serialize data to file.
      } catch (std::exception e) {
         std::cout << "Failed to build geometry with error: " << e.what() << std::endl;
         return -1;
      }
   } catch (std::exception e) {
      std::cout << "Failed to parse map file with error: " << e.what() << std::endl;
      return -1;
   }

   std::cout << "Finished compiling map file " << filepath << " into binary format."
             << std::endl;
   return 0;
}