#pragma once

#include <assert.h>
#include <stdint.h>

#define SCANF_MAX_TASK_NAME_LEN 64

typedef enum __attribute__((__packed__)) {
    SCANF_CONFIG = 0,
    SCANF_TASK_CREATE,
    SCANF_TASK_SWITCHED_IN,
    SCANF_TASK_SWITCHED_OUT
} SCANF_EventType;
static_assert(sizeof(SCANF_EventType) == 1, "sizeof(SCANF_EventType) > 1");

typedef struct __attribute__((__packed__)) {
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
} SCANF_Version;

typedef struct __attribute__((__packed__)) {
    SCANF_EventType event_type;
    SCANF_Version version;
    uint8_t config_max_task_name_len;
} SCANF_CONFIG_TraceMessage;

typedef struct __attribute__((__packed__)) {
    SCANF_EventType event_type;
    uint32_t timestamp;
} SCANF_TraceMessage;

typedef struct __attribute__((__packed__)) {
    SCANF_EventType event_type;
    uint32_t timestamp;
    uint32_t task_number;
    char task_name[SCANF_MAX_TASK_NAME_LEN];
} SCANF_TASK_CREATE_TraceMessage;

typedef struct __attribute__((__packed__)) {
    SCANF_EventType event_type;
    uint32_t timestamp;
    uint32_t task_number;
} SCANF_TASK_SWITCHED_IN_TraceMessage;

typedef struct __attribute__((__packed__)) {
    SCANF_EventType event_type;
    uint32_t timestamp;
    uint32_t task_number;
} SCANF_TASK_SWITCHED_OUT_TraceMessage;
