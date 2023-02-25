#include "shader_program.hpp"

#include <fstream>
#include <cassert>
#include <string.h>
#include <filesystem>

#include <loguru.hpp>

using namespace TR;

ShaderProgram::ShaderProgram(std::string_view vertPath, std::string_view fragPath,
                             const GladGLContext &context) {
   std::fstream vertFile(vertPath.data());
   std::fstream fragFile(fragPath.data());
   bool openFileSuccess = true;
   if (vertFile.fail()) {
      DLOG_F(WARNING, "Could not load shader resource %s: %s", vertPath.data(),
             strerror(errno));
      openFileSuccess = false;
   }
   if (fragFile.fail()) {
      DLOG_F(WARNING, "Could not load shader resource %s: %s", fragPath.data(),
             strerror(errno));
      openFileSuccess = false;
   }
   if (!openFileSuccess) {
      LOG_F(ERROR, "Failed to create shader program.\n");
      throw std::runtime_error("Failed to read shader source.");
   }

   int vertSourceLen = std::filesystem::file_size(vertPath);
   std::string vertSource;
   vertSource.resize(vertSourceLen);
   vertFile.read(vertSource.data(), vertSourceLen);

   int fragSourceLen = std::filesystem::file_size(fragPath);
   std::string fragSource;
   fragSource.resize(fragSourceLen);
   fragFile.read(fragSource.data(), fragSourceLen);

   GLuint vertShader = context.CreateShader(GL_VERTEX_SHADER);
   GLuint fragShader = context.CreateShader(GL_FRAGMENT_SHADER);
   const char *vertCstr = vertSource.c_str();
   const char *fragCstr = fragSource.c_str();
   context.ShaderSource(vertShader, 1, &vertCstr, nullptr);
   context.ShaderSource(fragShader, 1, &fragCstr, nullptr);
   context.CompileShader(vertShader);
   context.CompileShader(fragShader);

   GLint successfulCompile = 0;
   context.GetShaderiv(vertShader, GL_COMPILE_STATUS, &successfulCompile);
   if (!successfulCompile) {
      GLchar err[512];
      context.GetShaderInfoLog(vertShader, 512, nullptr, err);
      LOG_F(ERROR, "Failed to compile shader %s: %s\n", vertPath.data(), (char *)err);
      throw std::runtime_error("");
   }
   context.GetShaderiv(fragShader, GL_COMPILE_STATUS, &successfulCompile);
   if (!successfulCompile) {
      GLchar err[512];
      context.GetShaderInfoLog(fragShader, 512, nullptr, err);
      LOG_F(ERROR, "Failed to compile shader %s: %s\n", fragPath.data(), (char *)err);
      throw std::runtime_error("");
   }

   mHandle = context.CreateProgram();
   context.AttachShader(mHandle, vertShader);
   context.AttachShader(mHandle, fragShader);
   context.LinkProgram(mHandle);

   context.DetachShader(mHandle, vertShader);
   context.DetachShader(mHandle, fragShader);
   context.DeleteShader(vertShader);
   context.DeleteShader(fragShader);

   GLint successfullLink = 0;
   context.GetProgramiv(mHandle, GL_LINK_STATUS, &successfullLink);
   if (!successfullLink) {
      char err[512];
      context.GetProgramInfoLog(mHandle, 512, nullptr, err);
      LOG_F(ERROR, "Shader program construction from source files %s and %s failed: %s",
            vertPath.data(), fragPath.data(), (char *)err);
      throw std::runtime_error("Failed shader link");
   }
}