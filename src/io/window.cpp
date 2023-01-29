#include "window.hpp"

#include "../util/log.hpp"

#include <GLFW/glfw3.h>
#include <loguru.hpp>

using namespace TR;

// TODO: Passing in custom window attributes
Window::Window() {
   glfwSetErrorCallback(GLFWErrorCallback);
   if (!glfwInit()) { ABORT_F("Failed to initialize GLFW! Cannot continue."); }

   mHandle = glfwCreateWindow(800, 600, "Tremble", nullptr, nullptr);
   if (!mHandle) { ABORT_F("Failed to construct GLFW Window! Cannot continue."); }

   mContext = std::make_unique<GLContext>(mHandle);

   mID = GetNewID();
   DLOG_F(INFO, "Successfully constructed window with ID: %d", mID);
}

void Window::PollEvents() { glfwPollEvents(); }

Window::~Window() { glfwDestroyWindow(mHandle); }