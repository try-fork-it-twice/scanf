#include "scanf.h"

#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

static SCANF_Tracelog *_tracelog = NULL;
static bool _is_tracing = false;

void scanf_init(SCANF_Tracelog *tracelog)
{
    if (!tracelog) return;
    _tracelog = tracelog;
}

SCANF_Tracelog *scanf_get_tracelog()
{
    return _tracelog;
}

void scanf_reset()
{
    if (!_tracelog) return;
    _tracelog->size = 0;
}

void scanf_start_tracing()
{
    _is_tracing = true;
}

void scanf_stop_tracing()
{
    _is_tracing = false;
}

int scanf_save_tracelog(const char *filepath)
{
    if (!_tracelog) return -EINVAL;
    if (_is_tracing) return -EBUSY;
    FILE *file = fopen(filepath, "wb");
    if (!file) {
        perror("scanf: failed to open a file to save trace log");
        return -EINVAL;
    }
    unsigned long bytes = fwrite(_tracelog->messages, 1, _tracelog->size, file);
    if (bytes < _tracelog->size) {
        perror("scanf: failed to write whole log to a file");
        fclose(file);
        return -EFAULT;
    }
    fclose(file);
    return 0;
}

void _scanf_save_trace(const void *message, uint32_t message_size)
{
    if (!_tracelog) return;
    if (!_is_tracing) return;
    if ((_tracelog->size + message_size) >= _tracelog->capacity) return;
    memcpy(&_tracelog->messages[_tracelog->size], message, message_size);
    _tracelog->size += message_size;
}
