#pragma once

#include <stdint.h>

#define SCANF_MAX_TASK_NAME_LEN 64

typedef enum { SCANF_TASK_CREATE, SCANF_TASK_SWITCHED_IN, SCANF_TASK_SWITCHED_OUT } SCANF_EventType;

typedef struct __attribute__((__packed__)) {
    uint8_t event_type;
    uint32_t timestamp;
} SCANF_TraceMessage;

typedef struct __attribute__((__packed__)) {
    uint8_t event_type;
    uint32_t timestamp;
    uint32_t task_number;
    char task_name[SCANF_MAX_TASK_NAME_LEN];
} SCANF_TASK_CREATE_TraceMessage;

typedef struct __attribute__((__packed__)) {
    uint8_t event_type;
    uint32_t timestamp;
    uint32_t task_number;
} SCANF_TASK_SWITCHED_IN_TraceMessage;

typedef struct __attribute__((__packed__)) {
    uint8_t event_type;
    uint32_t timestamp;
    uint32_t task_number;
} SCANF_TASK_SWITCHED_OUT_TraceMessage;
