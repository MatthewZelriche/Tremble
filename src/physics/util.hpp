#pragma once

#include <loguru.hpp>
#include <foundation/PxErrorCallback.h>
#include <foundation/PxAllocatorCallback.h>
#include <malloc.h>

namespace TR {

static void *AlignedMalloc(size_t size) {
#ifdef _WIN64
   return _aligned_malloc(size, 16);
#elif __linux__ && !__ANDROID__
   return memalign(16, size);
#endif
}

static void AlignedFree(void *ptr) {
#ifdef _WIN64
   return _aligned_free(ptr);
#elif __linux__ && !__ANDROID__
   free(ptr);
#endif
}

class PhysxLogger : public physx::PxErrorCallback {
  public:
   virtual void reportError(physx::PxErrorCode::Enum code, const char *message,
                            const char *file, int line) {
      LOG_F(ERROR, "Physx error %d: %s in file %s, line %d", code, message, file, line);
   }
};

class PhysxAlloc : public physx::PxAllocatorCallback {
  public:
   virtual void *allocate(size_t size, const char *typeName, const char *filename,
                          int line) {
      //DLOG_F(INFO, "Physx allocating %d bytes for type (%s)", size, typeName);
      return AlignedMalloc(size);
   }

   virtual void deallocate(void *ptr) { AlignedFree(ptr); }
};

} // namespace TR