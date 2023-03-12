#include "phys_world.hpp"

#include <foundation/PxPhysicsVersion.h>
#include <omnipvd/PxOmniPvd.h>
#include <OmniPvdWriter.h>
#include <OmniPvdFileWriteStream.h>
#include <PxSceneDesc.h>
#include <PxScene.h>
#include <cstdlib>
#include <extensions/PxExtensionsAPI.h>

using namespace TR;
using namespace physx;

PhysWorld::PhysWorld() {
   mUnitScale.length = 1 / UNIT_TO_METER;
   mUnitScale.speed = 9.8f / UNIT_TO_METER;
   if ((mfoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mAlloc, mLogger)) ==
       nullptr) {
      LOG_F(ERROR, "Failed to initialize Physx!");
      std::abort();
   }

#ifdef NDEBUG
   if ((mHandle = PxCreatePhysics(PX_PHYSICS_VERSION, *mfoundation, mUnitScale)) ==
       nullptr) {
      LOG_F(ERROR, "Failed to initialize Physx!");
      std::abort();
   }
#else
   PxOmniPvd *omniPvd = PxCreateOmniPvd(*mfoundation);
   if (!omniPvd) {
      DLOG_F(ERROR, "Failed to init physx debugger!");
      std::abort();
   }
   OmniPvdWriter *omniWriter = omniPvd->getWriter();
   OmniPvdFileWriteStream *omniFileWriteStream = omniPvd->getFileWriteStream();
   if (omniWriter && omniFileWriteStream) {
      omniWriter->setWriteStream(
          reinterpret_cast<OmniPvdWriteStream *>(omniFileWriteStream));
   }

   // TODO: Set up physx visual debugger
   if ((mHandle = PxCreatePhysics(PX_PHYSICS_VERSION, *mfoundation, mUnitScale, false, 0,
                                  omniPvd)) == nullptr) {
      LOG_F(ERROR, "Failed to initialize Physx!");
      std::abort();
   }

   omniFileWriteStream->setFileName("myoutpufile.ovd");
   omniPvd->startSampling();
#endif

   DLOG_F(INFO, "Initialized PhysX version %d.%d.%d", PX_PHYSICS_VERSION_MAJOR,
          PX_PHYSICS_VERSION_MINOR, PX_PHYSICS_VERSION_BUGFIX);
}