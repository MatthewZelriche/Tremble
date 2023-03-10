#include "naive_renderer.hpp"

#include <GLFW/glfw3.h>
#include <stdexcept>
#include <loguru.hpp>

#include "../util/log.hpp"

#define GLAD_GL_IMPLEMENTATION
#include <gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "../io/input_manager.hpp"

using namespace TR;

NaiveRenderer::NaiveRenderer() {
   // Set up context
   int version = gladLoadGLContext(&mContext, glfwGetProcAddress);

   // TODO: Events to inform of viewport changes.
   mContext.Viewport(0, 0, 1280, 1024);

   mProgram = std::make_unique<ShaderProgram>("res/shaders/vert.glsl",
                                              "res/shaders/frag.glsl", mContext);
#ifndef NDEBUG
   // Set up debug extension.
   if (!mContext.KHR_debug) {
      DLOG_F(WARNING, "Debug extension GL_KHR_debug appears to not be supported, extra "
                      "debug functionality will not be available.");
   } else {
      mContext.Enable(GL_DEBUG_OUTPUT);
      mContext.Enable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      mContext.DebugMessageCallback(glDebugCallback, nullptr);
      mContext.DebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr,
                                   GL_TRUE);
   }
#endif
   mContext.Enable(GL_DEPTH_TEST);
   mContext.Enable(GL_CULL_FACE);

   mContext.UseProgram(mProgram->ShaderHandle());

   DLOG_F(INFO, "Initialized OpenGL Version %d.%d Renderer", GLAD_VERSION_MAJOR(version),
          GLAD_VERSION_MINOR(version));
}

unsigned int NaiveRenderer::LoadTexture(const std::string &path) {
   // Have we already loaded this texture?
   if (mTextureCache.count(path)) { return mTextureCache[path]; }

   std::string pathplusExt = std::string(path + ".png");

   int texWidth, texHeight, texChannels;
   unsigned char *tex = nullptr;
   if ((tex = stbi_load(pathplusExt.c_str(), &texWidth, &texHeight, &texChannels, 0)) ==
       nullptr) {
      // Attempt to load an "image not found" placeholder.
      // TODO: proper filepath setup
      if ((tex = stbi_load("err.png", &texWidth, &texHeight, &texChannels, 0)) ==
          nullptr) {
         // Now something has really gone wrong...give up.
         throw std::runtime_error(
             "Failed to load texture, including backup error texture.");
      }
   }

   unsigned int texture;
   mContext.GenTextures(1, &texture);
   mContext.BindTexture(GL_TEXTURE_2D, texture);

   mContext.TexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGBA,
                       GL_UNSIGNED_BYTE, tex);
   mContext.GenerateMipmap(GL_TEXTURE_2D);

   mContext.BindTexture(GL_TEXTURE_2D, 0);
   stbi_image_free(tex);

   mTextureCache.insert({path, texture});
   return texture;
}

void NaiveRenderer::AddMapRenderable(const MapRenderData &data, const std::string &tex) {
   unsigned int vao;
   mContext.GenVertexArrays(1, &vao);
   mContext.BindVertexArray(vao);

   unsigned int vbo;
   mContext.GenBuffers(1, &vbo);
   mContext.BindBuffer(GL_ARRAY_BUFFER, vbo);

   unsigned int ebo;
   mContext.GenBuffers(1, &ebo);
   mContext.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
   mContext.BufferData(GL_ELEMENT_ARRAY_BUFFER,
                       data.indices.size() * sizeof(unsigned int), data.indices.data(),
                       GL_STATIC_DRAW);

   // TODO: Test if we can remove this nonsense
   std::vector<float> verts;
   for (int i = 0; i < data.verts.size(); i++) {
      verts.push_back(data.verts[i].vertices.x);
      verts.push_back(data.verts[i].vertices.y);
      verts.push_back(data.verts[i].vertices.z);
      verts.push_back(data.verts[i].uvs.x);
      verts.push_back(data.verts[i].uvs.y);
   }
   mContext.BufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(),
                       GL_STATIC_DRAW);
   // TODO: Investigate normalizing option
   mContext.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid *)0);
   mContext.EnableVertexAttribArray(0);
   mContext.VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                                (GLvoid *)(3 * sizeof(float)));
   mContext.EnableVertexAttribArray(1);

   Renderable renderable;
   renderable.vao = vao;
   renderable.vertCount = data.indices.size();
   renderable.textureHandle = LoadTexture(tex);
   renderable.texturePath = tex;
   mVAOs.push_back(renderable);

   mContext.BindVertexArray(0);
   mContext.BindBuffer(GL_ARRAY_BUFFER, 0);
   mContext.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void NaiveRenderer::Draw(const Camera &camera) {
   // Debug stuff
   if (InputManager::IsKeyDown(KeyCode::KeyGraveAccent)) {
      enabled = !enabled;
      mContext.PolygonMode(GL_FRONT_AND_BACK, enabled ? GL_LINE : GL_FILL);
   }

   mContext.ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   mContext.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   for (auto &renderable : mVAOs) {
      mContext.BindTexture(GL_TEXTURE_2D, renderable.textureHandle);
      mContext.BindVertexArray(renderable.vao);

      // Convert model from quake coordinates
      // TODO: Better system?
      glm::mat4 model = glm::transpose(glm::make_mat4(m_coordTranslate));
      int modelLoc = mContext.GetUniformLocation(mProgram->ShaderHandle(), "model");
      mContext.UniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

      glm::mat4 viewProjectionMatrix = camera.ViewProjectionMatrix();
      int viewProjLoc = mContext.GetUniformLocation(mProgram->ShaderHandle(), "viewProj");
      mContext.UniformMatrix4fv(viewProjLoc, 1, GL_FALSE,
                                glm::value_ptr(viewProjectionMatrix));

      mContext.DrawElements(GL_TRIANGLES, renderable.vertCount, GL_UNSIGNED_INT, 0);

      mContext.BindVertexArray(0);
   }
}