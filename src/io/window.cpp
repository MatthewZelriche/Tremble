#include "window.hpp"

#include "../util/log.hpp"
#include "input_manager.hpp"

#include <loguru.hpp>

using namespace TR;

// TODO: Passing in custom window attributes
Window::Window() {
   glfwSetErrorCallback(GLFWErrorCallback);
   if (!glfwInit()) { ABORT_F("Failed to initialize GLFW! Cannot continue."); }

   mHandle = glfwCreateWindow(800, 600, "Tremble", nullptr, nullptr);
   if (!mHandle) { ABORT_F("Failed to construct GLFW Window! Cannot continue."); }

   mContext = std::make_unique<GLContext>(mHandle);
   glfwSetKeyCallback(mHandle, InputManager::GLFWCallback);

   mID = GetNewID();
   DLOG_F(INFO, "Successfully constructed window with ID: %d", mID);
}

bool Window::RequestedClose() { return glfwWindowShouldClose(mHandle); }

void Window::PollEvents() { glfwPollEvents(); }

Window::~Window() { glfwDestroyWindow(mHandle); }