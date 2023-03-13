#pragma once

#include "util.hpp"
#include <common/PxTolerancesScale.h>
#include <foundation/PxFoundation.h>
#include <PxPhysics.h>
#include <PxScene.h>

namespace TR {

constexpr float UNIT_TO_METER = 0.01905f;
constexpr float GRAV_SPEED = -800.0f;

class PhysScene {
  public:
   PhysScene();
   ~PhysScene();
   bool TimeToStep(float deltaTime);
   void BeginNewStep();
   bool TryFinishStep();
   bool IsSimulating() { return !mDidCompletePrevStep; }

  private:
   static void InitPhysicsEngine();
   bool mFirstInit {false};
   physx::PxScene *mSceneHandle {nullptr};
   float mTimeAcc {0.0f};
   bool mDidCompletePrevStep {true};
   void *mScratchMem {nullptr};

   static constexpr float STEP_INTERVAL = 1.0f / 60.0f;
   inline static physx::PxFoundation *mfoundation;
   inline static physx::PxPhysics *mPhysHandle;
   inline static physx::PxTolerancesScale mUnitScale;
   inline static PhysxLogger mLogger;
   inline static PhysxAlloc mAlloc;
};

}; // namespace TR