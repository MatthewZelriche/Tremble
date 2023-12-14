#include "Tremble/c_api/core/engine.hpp"

#include "Tremble/core/engine.hpp"
#include <exception>

using namespace TR;

extern "C" {

hEngine *engine_create() { return reinterpret_cast<hEngine *>(new Engine()); }
}