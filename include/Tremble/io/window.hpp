#pragma once

#include <memory>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "Tremble/io/config.hpp"

namespace TR {

class Window {
  public:
   Window(const WindowProps &props);
   ~Window() noexcept;
   Window(Window &other) = delete;
   Window &operator=(const Window &other) = delete;
   Window(Window &&other) noexcept;
   Window &operator=(Window &&other) noexcept;

   bool WindowRequestedClose() const noexcept;
   void Draw();

  private:
   GLFWwindow *handle = nullptr;
};

} // namespace TR