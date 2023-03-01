#pragma once

#include <cstdint>
#include <memory>
#include <GLFW/glfw3.h>

namespace TR {

using WindowID = uint32_t;

class Window {
  public:
   Window();
   ~Window();
   Window(const Window &other) = delete;
   Window &operator=(const Window &other) = delete;
   GLFWwindow *GetHandle() { return mHandle; }
   void MakeContextCurrent() { glfwMakeContextCurrent(mHandle); }

   bool RequestedClose();

   static void PollEvents();

  private:
   GLFWwindow *mHandle {nullptr};
   WindowID mID {0};
   inline static WindowID GetNewID() {
      static WindowID nextID = 1;
      return nextID++;
   }
};

} // namespace TR