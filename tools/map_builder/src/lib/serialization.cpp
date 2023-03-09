#include "serialization.hpp"
#include <fstream>
#include <cstring>

using namespace TR;

void TR::SerializeToDisk(std::string_view filename, const Map &data) {
   char hdr[8] = {0};
   std::memcpy(hdr, "TRMAP", 5);

   std::ofstream file(filename.data(), std::ios::binary | std::ios::trunc);
   cereal::BinaryOutputArchive oarchive(file);
   oarchive(cereal::binary_data(hdr, sizeof(char) * 8), data);
}

Map TR::SerializeFromDisk(std::string_view filename) {
   char hdr[8] = {0};

   Map data;
   std::ifstream file(filename.data(), std::ios::binary);
   cereal::BinaryInputArchive iarchive(file);
   iarchive(cereal::binary_data(hdr, sizeof(char) * 8), data);

   if (std::strcmp(hdr, "TRMAP") != 0) {
      throw std::runtime_error("Unrecognize TRMAP file");
   }

   return data;
}