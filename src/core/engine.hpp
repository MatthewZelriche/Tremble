#pragma once

#include "../io/window.hpp"
#include "../rendering/naive_renderer.hpp"
#include "../physics/phys_world.hpp"

#include <memory>

namespace TR {

class Engine {
  public:
   Engine();
   void Run();

  private:
   std::unique_ptr<Window> mWindow {nullptr};
   std::unique_ptr<NaiveRenderer> mRenderer;
   std::unique_ptr<PhysWorld> mPhysics;
};

} // namespace TR