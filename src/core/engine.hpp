#pragma once

#include "../io/window.hpp"
#include "../rendering/naive_renderer.hpp"

#include <memory>

namespace TR {

class Engine {
  public:
   Engine();
   void Run();

  private:
   std::unique_ptr<Window> mWindow {nullptr};
   std::unique_ptr<NaiveRenderer> mRenderer;
};

} // namespace TR