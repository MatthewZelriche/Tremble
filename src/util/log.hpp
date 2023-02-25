#pragma once

#include <gl.h>

namespace TR {

void glDebugCallback(GLenum source, GLenum type, unsigned int id, GLenum severity,
                     GLsizei length, const char *message, const void *userParam);

void GLFWErrorCallback(int error_code, const char *description);

} // namespace TR