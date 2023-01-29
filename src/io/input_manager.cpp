#include "input_manager.hpp"

#include "../util/log.hpp"

#include <loguru.hpp>

using namespace TR;

void InputManager::Initialize() {
   glfwSetErrorCallback(GLFWErrorCallback);
   if (!glfwInit()) { ABORT_F("Failed to initialize GLFW! Cannot continue."); }

   sKeyStates.fill(false);

   DLOG_F(INFO, "Successfully initialized input module");
}

bool InputManager::IsKeyDown(KeyCode code) { return sKeyStates[(int)code]; }

void InputManager::GLFWCallback(GLFWwindow *window, int key, int scancode, int action,
                                int mods) {
   // TODO: Events instead of per-frame querying.
   if (action == GLFW_PRESS) {
      sKeyStates[key] = true;
   } else if (action == GLFW_RELEASE) {
      sKeyStates[key] = false;
   }
}