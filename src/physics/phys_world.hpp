#pragma once

#include "util.hpp"
#include <common/PxTolerancesScale.h>
#include <foundation/PxFoundation.h>
#include <PxPhysics.h>

namespace TR {

constexpr float UNIT_TO_METER = 0.01905f;

class PhysWorld {
  public:
   PhysWorld();

  private:
   physx::PxFoundation *mfoundation;
   physx::PxPhysics *mHandle;
   physx::PxTolerancesScale mUnitScale;
   inline static PhysxLogger mLogger;
   inline static PhysxAlloc mAlloc;
};

}; // namespace TR