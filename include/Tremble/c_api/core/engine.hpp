#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "Tremble/core/tremble_export.hpp"

struct hEngine;
typedef struct hEngine hEngine;

TREMBLE_EXPORT hEngine *engine_create();

#ifdef __cplusplus
}
#endif