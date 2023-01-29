#pragma once

#include "../io/window.hpp"

#include <memory>

namespace TR {

class Engine {
  public:
   Engine();
   void Run();

  private:
   std::unique_ptr<Window> mWindow {nullptr};
};

} // namespace TR