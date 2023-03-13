#pragma once

#include "../io/window.hpp"
#include "../rendering/naive_renderer.hpp"
#include "../physics/phys_scene.hpp"

#include <memory>

namespace TR {

class Engine {
  public:
   Engine();
   void Run();

  private:
   void PhysicsUpdate();

   std::unique_ptr<Window> mWindow {nullptr};
   std::unique_ptr<NaiveRenderer> mRenderer;
   std::unique_ptr<PhysScene> mActiveScene;

   double mFrameStartTime {0.0f};
   double mFrameEndTime {0.0f};
   double mDeltaTime {1.0f / 60.0f};
};

} // namespace TR