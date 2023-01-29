#pragma once

#include <cstdint>
#include <memory>

#include "../rendering/GLContext.hpp"

class GLFWwindow;

namespace TR {

using WindowID = uint32_t;

class Window {
  public:
   Window();
   ~Window();
   Window(const Window &other) = delete;
   Window &Window::operator=(const Window &other) = delete;

   bool RequestedClose();

   inline GLContext *GetGLContext() { return mContext.get(); }

   static void PollEvents();

  private:
   std::unique_ptr<GLContext> mContext {nullptr};
   GLFWwindow *mHandle {nullptr};
   WindowID mID {0};
   inline static WindowID GetNewID() {
      static WindowID nextID = 1;
      return nextID++;
   }
};

} // namespace TR