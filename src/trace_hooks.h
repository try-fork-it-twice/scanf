#ifndef SCANF_TRACE_HOOKS_H
#define SCANF_TRACE_HOOKS_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define traceENTER_xTaskCreateStatic(pxTaskCode, pcName, uxStackDepth, pvParameters, uxPriority, puxStackBuffer, pxTaskBuffer) \
    do { \
        printf("traceENTER_xTaskCreateStatic: %s\n", pcName); \
    } while (0)


#ifdef __cplusplus
}
#endif

#endif /* SCANF_TRACE_HOOKS_H */
