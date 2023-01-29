#pragma once

#include <gl.h>

class GLFWwindow;

namespace TR {

class GLContext {
  public:
   GLContext(GLFWwindow *windowHandle);

  private:
   GladGLContext mContext;
};

} // namespace TR