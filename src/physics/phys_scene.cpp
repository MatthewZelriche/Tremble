#include "phys_scene.hpp"

#include <foundation/PxPhysicsVersion.h>
#include <omnipvd/PxOmniPvd.h>
#include <OmniPvdWriter.h>
#include <OmniPvdFileWriteStream.h>
#include <cstdlib>
#include <extensions/PxExtensionsAPI.h>
#include <PxPhysicsAPI.h>

using namespace TR;
using namespace physx;

PhysScene::PhysScene() {
   if (!mFirstInit) {
      // Perform first time init of PhysX
      InitPhysicsEngine();
   }

   // TODO: Dehardcode number of worker threads.
   PxSceneDesc sceneDesc(mPhysHandle->getTolerancesScale());
   sceneDesc.gravity = PxVec3(0.0f, 0.0f, GRAV_SPEED);
   sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
   sceneDesc.filterShader = PxDefaultSimulationFilterShader;
   mSceneHandle = mPhysHandle->createScene(sceneDesc);

   // 32 Kib of scratch space
   mScratchMem = AlignedMalloc(32768);
}

PhysScene::~PhysScene() { AlignedFree(mScratchMem); }

bool PhysScene::TimeToStep(float deltaTime) {
   mTimeAcc += deltaTime;

   if (mTimeAcc < STEP_INTERVAL) { return false; }
   return true;
}

void PhysScene::BeginNewStep() {
   // Previous Step completed, time for a new step.
   mTimeAcc -= STEP_INTERVAL;
   mSceneHandle->simulate(STEP_INTERVAL, nullptr, mScratchMem, 32768);
   mDidCompletePrevStep = false;
}

bool PhysScene::TryFinishStep() {
   if (mSceneHandle->checkResults(false)) {
      // Physx has finished this sim step
      DCHECK_F(mSceneHandle->fetchResults(false),
               "Tried to fetch non-ready results. This should never happen.");
      mDidCompletePrevStep = true;
      return mDidCompletePrevStep;
   }
   return false;
}

void PhysScene::InitPhysicsEngine() {
   mUnitScale.length = 1 / UNIT_TO_METER;
   mUnitScale.speed = -GRAV_SPEED;
   if ((mfoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mAlloc, mLogger)) ==
       nullptr) {
      LOG_F(ERROR, "Failed to initialize Physx!");
      std::abort();
   }

#ifdef NDEBUG
   if ((mPhysHandle = PxCreatePhysics(PX_PHYSICS_VERSION, *mfoundation, mUnitScale)) ==
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

   if ((mPhysHandle = PxCreatePhysics(PX_PHYSICS_VERSION, *mfoundation, mUnitScale, false,
                                      0, omniPvd)) == nullptr) {
      LOG_F(ERROR, "Failed to initialize Physx!");
      std::abort();
   }

   omniFileWriteStream->setFileName("physDebug.ovd");
   omniPvd->startSampling();
#endif

   if (!PxInitExtensions(*mPhysHandle, 0)) {
      LOG_F(ERROR, "Failed to init Physx extensions");
      std::abort();
   }

   DLOG_F(INFO, "Initialized PhysX version %d.%d.%d", PX_PHYSICS_VERSION_MAJOR,
          PX_PHYSICS_VERSION_MINOR, PX_PHYSICS_VERSION_BUGFIX);
}