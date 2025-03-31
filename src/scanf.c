#include "scanf.h"

#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

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

void _scanf_save_trace(const void *message, uint32_t message_size)
{
    if (!_tracelog) return;
    if (!_is_tracing) return;
    if ((_tracelog->size + message_size) >= _tracelog->capacity) return;
    printf("Saving event\n");
    memcpy(&_tracelog->messages[_tracelog->size], message, message_size);
    _tracelog->size += message_size;
}
