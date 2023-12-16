#include "Tremble/io/window.hpp"

#include "Tremble/io/log.hpp"
#include "Tremble/error/exception.hpp"

using namespace TR;

Window::Window(const WindowProps &props) {
   if (!glfwInit()) { throw third_party_init_error("glfwInit failed"); }

   handle = glfwCreateWindow(props.resX, props.resY, "Tremble Engine", nullptr, nullptr);
   if (handle == nullptr) { throw third_party_init_error("glfwCreateWindow failed"); }
}

Window::~Window() { glfwDestroyWindow(handle); }

Window::Window(Window &&other) noexcept {
   this->handle = other.handle;
   other.handle = nullptr;
}

Window &Window::operator=(Window &&other) noexcept {
   this->handle = other.handle;
   other.handle = nullptr;
   return *this;
}

bool Window::WindowRequestedClose() const noexcept {
   return glfwWindowShouldClose(handle);
}

void Window::Draw() { glfwPollEvents(); }