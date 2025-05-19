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

void scanf_init(SCANF_Tracelog *tracelog) {
    if (!tracelog) return;
    _tracelog = tracelog;
    scanf_reset();
}

SCANF_Tracelog *scanf_get_tracelog() { return _tracelog; }

void scanf_reset() {
    if (!_tracelog) return;
    _tracelog->size = 0;

    static SCANF_CONFIG_TraceMessage config_message;
    config_message.event_type = SCANF_CONFIG;
    // TODO: Pass version through CMake
    config_message.version.major = 0;
    config_message.version.minor = 2;
    config_message.version.patch = 0;
    config_message.config_max_task_name_len = SCANF_MAX_TASK_NAME_LEN;
    _scanf_save_trace((void *)&config_message, sizeof(config_message));
}

void scanf_start_tracing() { _is_tracing = true; }

void scanf_stop_tracing() { _is_tracing = false; }

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
    fclose(file);
    return 0;
}

void _scanf_save_trace(const void *message, uint32_t message_size) {
    if (!_tracelog) return;
    if (!_is_tracing) return;
    if ((_tracelog->size + message_size) >= _tracelog->capacity) return;
    memcpy(&_tracelog->messages[_tracelog->size], message, message_size);
    _tracelog->size += message_size;
}

uint32_t _scanf_get_timestamp() { return (xTaskGetTickCount() * 1000000) / configTICK_RATE_HZ; }
