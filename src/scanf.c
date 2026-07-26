#include "scanf.h"

#include <FreeRTOS.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <task.h>

static SCANF_Tracelog *_tracelog = NULL;
static bool _is_tracing = false;

int scanf_init(SCANF_Tracelog *tracelog) {
    if (!tracelog) return -EINVAL;
    if (!tracelog->messages) return -EINVAL;
    if (tracelog->capacity < SCANF_TRACELOG_MIN_CAPACITY) return -EINVAL;
    _tracelog = tracelog;
    scanf_reset();
    return 0;
}

SCANF_Tracelog *scanf_get_tracelog(void) { return _tracelog; }

void scanf_reset(void) {
    if (!_tracelog) return;
    _tracelog->size = 0;
    _tracelog->dropped = 0;
    _tracelog->dropped_at = 0;

    SCANF_CONFIG_TraceMessage config_message = {0};
    config_message.event_type = SCANF_CONFIG;
    // TODO: Pass version through CMake
    config_message.version.major = 1;
    config_message.version.minor = 0;
    config_message.version.patch = 0;
    config_message.config_max_task_name_len = SCANF_MAX_TASK_NAME_LEN;

    bool prev_is_tracing = _is_tracing;
    _is_tracing = true;
    _scanf_save_trace((void *)&config_message, sizeof(config_message));
    _is_tracing = prev_is_tracing;
}

void scanf_start_tracing(void) { _is_tracing = true; }

void scanf_stop_tracing(void) { _is_tracing = false; }

int scanf_save_tracelog(const char *filepath) {
    if (!_tracelog) return -EINVAL;
    if (_is_tracing) return -EBUSY;
    FILE *file = fopen(filepath, "wb");
    if (!file) {
        return -EINVAL;
    }
    unsigned long bytes = fwrite(_tracelog->messages, 1, _tracelog->size, file);
    if (bytes < _tracelog->size) {
        fclose(file);
        return -EFAULT;
    }
    if (_tracelog->dropped > 0) {
        SCANF_OVERFLOW_TraceMessage overflow = {0};
        overflow.event_type = SCANF_OVERFLOW;
        overflow.timestamp = _tracelog->dropped_at;
        overflow.dropped = _tracelog->dropped;
        if (fwrite(&overflow, 1, sizeof(overflow), file) < sizeof(overflow)) {
            fclose(file);
            return -EFAULT;
        }
    }
    fclose(file);
    return 0;
}

void _scanf_save_trace(const void *message, uint32_t message_size) {
    if (!_tracelog) return;
    if (!_is_tracing) return;
    /* Subtraction rather than addition: size never exceeds capacity, so this
     * cannot wrap, whereas size + message_size could. */
    if (message_size > (_tracelog->capacity - _tracelog->size)) {
        if (_tracelog->dropped == 0) _tracelog->dropped_at = _scanf_get_timestamp();
        _tracelog->dropped++;
        return;
    }
    memcpy(&_tracelog->messages[_tracelog->size], message, message_size);
    _tracelog->size += message_size;
}

uint32_t _scanf_get_timestamp(void) {
    /* Widened: at 100 Hz the 32-bit product overflowed after ~43 seconds. The
     * microsecond result still wraps after ~71 minutes of uptime. */
    return (uint32_t)(((uint64_t)xTaskGetTickCount() * 1000000ULL) / configTICK_RATE_HZ);
}
