#include <iostream>
#include <filesystem>

#include "lib/file_parser.hpp"
#include "lib/serialization.hpp"

int main(int argc, char **argv) {
   if (argc != 3) {
      std::cout << "Incorrect number of arguments supplied" << std::endl;
      std::cout
          << "Example use: TrembleMapParser.exe <input-path>.map <output-path>.trmap"
          << std::endl;
      return -1;
   }

   auto oldWorkingDir = std::filesystem::current_path();
   auto mapPath = std::filesystem::path(std::string(argv[1]));
   std::string compiledPath = std::string(argv[2]);

   // Switch working dir to be relative to map file
   std::filesystem::current_path(mapPath.parent_path());

   std::cout << "Begin parsing " << mapPath << "..." << std::endl;
   try {
      TR::MapFileParser parser = TR::MapFileParser(mapPath.filename().c_str());
      std::cout << "Begin serializing binary data to file " << compiledPath << std::endl;

      // Restore original working dir
      std::filesystem::current_path(oldWorkingDir);
      parser.SaveToBinaryFile(compiledPath);
      std::cout << "Successfully serialized binary data." << std::endl;

   } catch (std::exception e) {
      std::cout << "Failed to parse map file with error: " << e.what() << std::endl;
      return -1;
   }

   std::cout << "Finished compiling map file " << mapPath << "." << std::endl;
   return 0;
}