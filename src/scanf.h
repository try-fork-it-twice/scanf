#pragma once

#include <stdint.h>

#include "messages.h"

typedef struct SCANF_Tracelog {
    uint32_t size;
    uint32_t capacity;
    SCANF_EventType *messages;
} SCANF_Tracelog;

/**
 * Initialize the global trace log pointer with a preallocated trace log structure.
 */
void scanf_init(SCANF_Tracelog *tracelog);

/**
 * Retrieve the global trace log handler.
 */
SCANF_Tracelog *scanf_get_tracelog();

/**
 * Reset the trace log size.
 */
void scanf_reset();

/**
 * Start recording events in the trace log.
 */
void scanf_start_tracing();

/**
 * Stop recording events in the trace log.
 */
void scanf_stop_tracing();

/**
 * Save recorder trace log to the file.
 */
int scanf_save_tracelog(const char *filepath);
