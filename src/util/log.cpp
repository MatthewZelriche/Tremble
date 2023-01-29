#include "log.hpp"
#include <loguru.hpp>

void TR::GLFWErrorCallback(int error_code, const char *description) {
   LOG_F(ERROR, "GLFW Error %d: %s", error_code, description);
}