#pragma once

#include "key_codes.hpp"

#include <array>
#include <GLFW/glfw3.h>

namespace TR {

class InputManager {
  public:
   static void Initialize();
   static bool IsKeyDown(KeyCode code);
   static void GLFWCallback(GLFWwindow *window, int key, int scancode, int action,
                            int mods);

  private:
   inline static std::array<bool, GLFW_KEY_LAST> sKeyStates;
};

} // namespace TR