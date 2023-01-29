#pragma once

#include <memory>

#include "../io/window.hpp"

namespace TR {

class Engine {
  public:
   Engine();
   void Run();

  private:
   std::unique_ptr<Window> mWindow {nullptr};
};

} // namespace TR