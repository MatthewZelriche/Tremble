#include "Tremble/io/window.hpp"

#include "Tremble/io/log.hpp"
#include "Tremble/error/exception.hpp"

using namespace TR;

Window::Window() {
   if (!glfwInit()) { throw third_party_init_error("glfwInit failed"); }
}