#include <iostream>

#include "file_parser.hpp"
#include "geometry_constructor.hpp"

#include "serialization.hpp"

int main(int argc, char **argv) {
   if (argc != 3) {
      std::cout << "Incorrect number of arguments supplied" << std::endl;
      std::cout
          << "Example use: TrembleMapParser.exe <input-path>.map <output-path>.trmap"
          << std::endl;
      return -1;
   }

   std::string mapPath = std::string(argv[1]);
   std::string compiledPath = std::string(argv[2]);
   std::cout << "Begin parsing " << mapPath << "..." << std::endl;
   try {
      TR::MapFileParser parser = TR::MapFileParser(mapPath);
      std::cout << "Begin serializing binary data to file " << compiledPath << std::endl;
      parser.SaveToBinaryFile(compiledPath);
      std::cout << "Successfully serialized binary data." << std::endl;

   } catch (std::exception e) {
      std::cout << "Failed to parse map file with error: " << e.what() << std::endl;
      return -1;
   }

   std::cout << "Finished compiling map file " << mapPath << "." << std::endl;
   return 0;
}