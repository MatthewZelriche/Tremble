#pragma once

#include <gl.h>
#include <lib_types.hpp>
#include <memory>

#include "shader_program.hpp"
#include "camera.hpp"

namespace TR {

struct Renderable {
   unsigned int vao;
   std::string texturePath;
   unsigned int textureHandle;
   int vertCount;
};

class NaiveRenderer {
  public:
   NaiveRenderer();
   void AddMapFace(const Face &face);

   void Draw(const Camera &camera);

  private:
   unsigned int LoadTexture(const std::string &path);

   std::unordered_map<std::string, unsigned int> mTextureCache;
   GladGLContext mContext;
   std::vector<Renderable> mVAOs;
   std::unique_ptr<ShaderProgram> mProgram;
   float m_coordTranslate[16] = {0, -1, 0, 0, 0, 0, 1, 0, -1, 0, 0, 0, 0, 0, 0, 1};

   bool enabled = false;
};

} // namespace TR