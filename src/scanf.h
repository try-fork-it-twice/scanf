#pragma once

#include <stdint.h>

#include "messages.h"

typedef struct SCANF_Tracelog {
    uint32_t size;       /* bytes currently used */
    uint32_t capacity;   /* size of the messages buffer, in bytes */
    uint32_t dropped;    /* events discarded because the buffer was full */
    uint32_t dropped_at; /* timestamp of the first discarded event */
    uint8_t *messages;
} SCANF_Tracelog;

/**
 * Smallest trace log capacity scanf_init() will accept, in bytes.
 */
#define SCANF_TRACELOG_MIN_CAPACITY (sizeof(SCANF_CONFIG_TraceMessage))

/**
 * Define a trace log with static storage duration.
 *
 * Use this instead of declaring SCANF_Tracelog by hand: the log must outlive
 * vTaskStartScheduler(), which never returns, so automatic storage would leave
 * scanf holding a dangling pointer into a dead stack frame.
 *
 * Works at file scope and inside a function alike. `capacity_bytes` is a size
 * in bytes and must be an integer constant expression.
 */
#define SCANF_DEFINE_TRACELOG(name, capacity_bytes)  \
    static uint8_t name##_storage[(capacity_bytes)]; \
    static SCANF_Tracelog name = {                   \
        .size = 0,                                   \
        .capacity = (capacity_bytes),                \
        .dropped = 0,                                \
        .dropped_at = 0,                             \
        .messages = name##_storage,                  \
    }

/**
 * Initialize the global trace log pointer with a preallocated trace log structure.
 *
 * The trace log and its buffer must have static storage duration: scanf keeps
 * the pointer for the lifetime of the program and there is no way to revoke it.
 * Use SCANF_DEFINE_TRACELOG() to get that right by construction.
 *
 * Returns 0 on success, or -EINVAL if the trace log is missing a buffer or is
 * smaller than SCANF_TRACELOG_MIN_CAPACITY.
 */
int scanf_init(SCANF_Tracelog *tracelog);

/**
 * Retrieve the global trace log handler.
 *
 * The `dropped` field is non-zero if the buffer overflowed and events were lost.
 */
SCANF_Tracelog *scanf_get_tracelog(void);

/**
 * Reset the trace log size and the overflow counters.
 */
void scanf_reset(void);

/**
 * Start recording events in the trace log.
 */
void scanf_start_tracing(void);

/**
 * Stop recording events in the trace log.
 */
void scanf_stop_tracing(void);

/**
 * Save recorded trace log to the file.
 *
 * If events were lost, a trailing SCANF_OVERFLOW record is appended to the file
 * so that a truncated trace cannot be mistaken for a complete one.
 */
int scanf_save_tracelog(const char *filepath);
