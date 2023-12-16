#include "Tremble/c_api/core/engine.hpp"

#include "Tremble/core/engine.hpp"
#include "Tremble/error/exception.hpp"

using namespace TR;

extern "C" {

TrembleError engine_create(hEngine **self) {
   try {
      *self = reinterpret_cast<hEngine *>(new Engine());
      return TrembleError::NO_ERROR;
   } catch (tremble_exception e) { return e.GetErrorCode(); } catch (std::exception e) {
      return TrembleError::UNKNOWN_ERROR;
   }
}

void engine_destroy(hEngine *self) { delete self; }

int engine_requested_shutdown(hEngine *self) {
   return (int)reinterpret_cast<Engine *>(self)->RequestedShutdown();
}

void engine_update(hEngine *handle, double delta) {
   reinterpret_cast<Engine *>(handle)->Update(delta);
}
}
