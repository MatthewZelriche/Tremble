#include "log.hpp"
#include <loguru.hpp>

void TR::GLFWErrorCallback(int error_code, const char *description) {
   LOG_F(ERROR, "GLFW Error %d: %s", error_code, description);
}

void TR::glDebugCallback(GLenum source, GLenum type, unsigned int id, GLenum severity,
                         GLsizei length, const char *message, const void *userParam) {
   switch (severity) {
      case GL_DEBUG_SEVERITY_HIGH: LOG_F(ERROR, "OpenGL: %s", message);
      case GL_DEBUG_SEVERITY_MEDIUM: LOG_F(WARNING, "OpenGL: %s", message);
      case GL_DEBUG_SEVERITY_LOW: LOG_F(INFO, "OpenGL: %s", message);
   }
}