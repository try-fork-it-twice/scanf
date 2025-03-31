#pragma once

#include <stdio.h>
#include <string.h>

#include "messages.h"
#include "scanf.h"

#ifdef __cplusplus
extern "C" {
#endif

void _scanf_save_trace(void *message, uint32_t size);

#define traceTASK_CREATE(pxNewTCB)                              \
    do {                                                        \
        SCANF_TASK_CREATE_TraceMessage message;                 \
        message.event_type = SCANF_TASK_CREATE;                 \
        message.timestamp = (uint32_t)xTaskGetTickCount();      \
        message.task_number = (uint32_t)pxNewTCB->uxTaskNumber; \
        strcpy(message.task_name, pxNewTCB->pcTaskName);        \
        _scanf_save_trace(&message, sizeof(message));           \
    } while (0)

#define traceTASK_SWITCHED_IN()                                     \
    do {                                                            \
        SCANF_TASK_SWITCHED_IN_TraceMessage message;                \
        message.event_type = SCANF_TASK_SWITCHED_IN;                \
        message.timestamp = (uint32_t)xTaskGetTickCount();          \
        message.task_number = (uint32_t)pxCurrentTCB->uxTaskNumber; \
        _scanf_save_trace(&message, sizeof(message));               \
    } while (0)

#define traceTASK_SWITCHED_OUT()                                    \
    do {                                                            \
        SCANF_TASK_SWITCHED_OUT_TraceMessage message;               \
        message.event_type = SCANF_TASK_SWITCHED_OUT;               \
        message.timestamp = (uint32_t)xTaskGetTickCount();          \
        message.task_number = (uint32_t)pxCurrentTCB->uxTaskNumber; \
        _scanf_save_trace(&message, sizeof(message));               \
    } while (0)

#ifdef __cplusplus
}
#endif
