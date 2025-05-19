#pragma once

#include <stdio.h>
#include <string.h>

#include "messages.h"
#include "scanf.h"

#ifdef __cplusplus
extern "C" {
#endif

void _scanf_save_trace(const void *message, uint32_t size);
uint32_t _scanf_get_timestamp();

#define traceTASK_CREATE(pxNewTCB)                             \
    do {                                                       \
        SCANF_TASK_CREATE_TraceMessage message;                \
        message.event_type = SCANF_TASK_CREATE;                \
        message.timestamp = _scanf_get_timestamp();            \
        message.task_number = (uint32_t)pxNewTCB->uxTCBNumber; \
        strcpy(message.task_name, pxNewTCB->pcTaskName);       \
        _scanf_save_trace(&message, sizeof(message));          \
    } while (0)

#define traceTASK_SWITCHED_IN()                                    \
    do {                                                           \
        SCANF_TASK_SWITCHED_IN_TraceMessage message;               \
        message.event_type = SCANF_TASK_SWITCHED_IN;               \
        message.timestamp = _scanf_get_timestamp();                \
        message.task_number = (uint32_t)pxCurrentTCB->uxTCBNumber; \
        _scanf_save_trace(&message, sizeof(message));              \
    } while (0)

#define traceTASK_SWITCHED_OUT()                                   \
    do {                                                           \
        SCANF_TASK_SWITCHED_OUT_TraceMessage message;              \
        message.event_type = SCANF_TASK_SWITCHED_OUT;              \
        message.timestamp = _scanf_get_timestamp();                \
        message.task_number = (uint32_t)pxCurrentTCB->uxTCBNumber; \
        _scanf_save_trace(&message, sizeof(message));              \
    } while (0)

#ifdef __cplusplus
}
#endif
