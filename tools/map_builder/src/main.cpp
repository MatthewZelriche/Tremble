#include <iostream>

#include "file_parser.hpp"

int main() {
   // TODO: Dehardcode filepath
   std::string filepath = "unnamed.map";
   std::cout << "Begin building " << filepath << "..." << std::endl;
   try {
      TR::MapFileParser parser = TR::MapFileParser(filepath);
      std::cout << "Successfully completed parsing stage." << std::endl;

      // TODO: Geometry construction
   } catch (std::exception e) {
      std::cout << "Failed to parse map file with err: " << e.what();
   }

   std::cout << "Successfully built map file " << filepath << " into binary format."
             << std::endl;
   return 0;
}