#pragma once

#include <gl.h>

#include <GLFW/glfw3.h>

namespace TR {

class GLContext {
  public:
   GLContext(GLFWwindow *windowHandle);

  private:
   GladGLContext mContext;
};

} // namespace TR