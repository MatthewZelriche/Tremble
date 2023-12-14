#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "Tremble/core/tremble_export.hpp"

struct hEngine;
typedef struct hEngine hEngine;

TREMBLE_EXPORT hEngine *engine_create();
TREMBLE_EXPORT void engine_update(hEngine *handle, double delta);

#ifdef __cplusplus
}
#endif