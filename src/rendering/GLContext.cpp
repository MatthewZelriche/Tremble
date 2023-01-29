#include "GLContext.hpp"
#include <loguru.hpp>

#define GLAD_GL_IMPLEMENTATION
#include <gl.h>

using namespace TR;

GLContext::GLContext(GLFWwindow *windowHandle) {
   glfwMakeContextCurrent(windowHandle);
   int version = gladLoadGLContext(&mContext, glfwGetProcAddress);
   DLOG_F(INFO, "Successfully initialized OpenGL %d.%d renderer",
          GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
}