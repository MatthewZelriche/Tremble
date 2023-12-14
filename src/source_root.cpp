#include "Tremble/source_root.hpp"

#include <source_location>
#include <filesystem>

std::string_view GetSourceRoot() {
   static std::string root =
       std::filesystem::path(std::source_location::current().file_name())
           .remove_filename()
           .string();
   return std::string_view(root);
}