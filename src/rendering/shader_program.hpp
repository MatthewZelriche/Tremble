#pragma once

#include <string>

#include <gl.h>

namespace TR {

class ShaderProgram {
  public:
   ShaderProgram(std::string_view vertPath, std::string_view fragPath,
                 const GladGLContext &context);

   const unsigned int ShaderHandle() const { return mHandle; }

  private:
   unsigned int mHandle;
};

} // namespace TR