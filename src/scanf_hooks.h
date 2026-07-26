#pragma once

#include <stdio.h>
#include <string.h>

#include "messages.h"
#include "scanf.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Task names are copied verbatim into the trace record, so the record field
 * must be wide enough to hold whatever FreeRTOS is configured to allow. */
static_assert(configMAX_TASK_NAME_LEN <= SCANF_MAX_TASK_NAME_LEN,
              "configMAX_TASK_NAME_LEN exceeds SCANF_MAX_TASK_NAME_LEN");

void _scanf_save_trace(const void *message, uint32_t size);
uint32_t _scanf_get_timestamp(void);

/* Every message is zero-initialized before use: the records are written to the
 * trace byte for byte, so any field left untouched would leak stack contents
 * into the trace and make it non-reproducible. */

#define traceTASK_CREATE(pxNewTCB)                                        \
    do {                                                                  \
        SCANF_TASK_CREATE_TraceMessage message = {0};                     \
        size_t name_len = strlen(pxNewTCB->pcTaskName);                   \
        if (name_len >= sizeof(message.task_name)) {                      \
            name_len = sizeof(message.task_name) - 1U;                    \
        }                                                                 \
        message.event_type = SCANF_TASK_CREATE;                           \
        message.timestamp = _scanf_get_timestamp();                       \
        message.task_number = (uint32_t)pxNewTCB->uxTCBNumber;            \
        memcpy(message.task_name, pxNewTCB->pcTaskName, name_len);        \
        _scanf_save_trace(&message, sizeof(message));                     \
    } while (0)

#define traceTASK_SWITCHED_IN()                                    \
    do {                                                           \
        SCANF_TASK_SWITCHED_IN_TraceMessage message = {0};         \
        message.event_type = SCANF_TASK_SWITCHED_IN;               \
        message.timestamp = _scanf_get_timestamp();                \
        message.task_number = (uint32_t)pxCurrentTCB->uxTCBNumber; \
        _scanf_save_trace(&message, sizeof(message));              \
    } while (0)

#define traceTASK_SWITCHED_OUT()                                   \
    do {                                                           \
        SCANF_TASK_SWITCHED_OUT_TraceMessage message = {0};        \
        message.event_type = SCANF_TASK_SWITCHED_OUT;              \
        message.timestamp = _scanf_get_timestamp();                \
        message.task_number = (uint32_t)pxCurrentTCB->uxTCBNumber; \
        _scanf_save_trace(&message, sizeof(message));              \
    } while (0)

#ifdef __cplusplus
}
#endif
