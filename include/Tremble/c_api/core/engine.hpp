#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "Tremble/core/tremble_export.hpp"
#include "Tremble/c_api/error/tremble_error.hpp"

struct hEngine;
typedef struct hEngine hEngine;

TREMBLE_EXPORT TrembleError engine_create(hEngine **self);
TREMBLE_EXPORT void engine_destroy(hEngine *self);
TREMBLE_EXPORT void engine_update(hEngine *handle, double delta);

#ifdef __cplusplus
}
#endif