#pragma once

/* NOTE: Disabling formatter because ordering matters */
/* clang-format off */
#include "FreeRTOSConfigDefaults.h"

#undef configTICK_RATE_HZ
#define configTICK_RATE_HZ                         ( 10000000U )

#include <scanf_FreeRTOSConfig.h>
/* clang-format on */
