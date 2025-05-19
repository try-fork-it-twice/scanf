#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <tracelog_file>\n", argv[0]);
        return 1;
    }

    FILE* fd = fopen(argv[1], "r");
    char buf[1024];
    size_t size = fread(buf, 1, sizeof(buf), fd);
    if (size == 0 && ferror(fd)) {
        fprintf(stderr, "fread() failed: %zu\n", size);
        exit(EXIT_FAILURE);
    }

    size_t i = 0;
    printf("[\n");
    while (i < size) {
        printf("  {\n");
        SCANF_EventType* message_type = (SCANF_EventType*)(&buf[i]);
        switch (*message_type) {
            case SCANF_CONFIG: {
                SCANF_CONFIG_TraceMessage* message = (SCANF_CONFIG_TraceMessage*)(&buf[i]);
                printf("    \"event_type:\": \"SCANF_CONFIG\",\n");
                printf("    \"version:\": \"%d.%d.%d\",\n", message->version.major,
                       message->version.minor, message->version.patch);
                printf("    \"config_max_task_name_len:\": %d\n",
                       message->config_max_task_name_len);
                i += sizeof(SCANF_CONFIG_TraceMessage);
            } break;
            case SCANF_TASK_CREATE: {
                SCANF_TASK_CREATE_TraceMessage* message =
                    (SCANF_TASK_CREATE_TraceMessage*)(&buf[i]);
                printf("    \"event_type:\": \"SCANF_TASK_CREATE\",\n");
                printf("    \"timestamp:\": %d,\n", message->timestamp);
                printf("    \"task_number:\": %d,\n", message->task_number);
                printf("    \"task_name:\": \"%.64s\"\n", message->task_name);
                i += sizeof(SCANF_TASK_CREATE_TraceMessage);
            } break;
            case SCANF_TASK_SWITCHED_IN: {
                SCANF_TASK_SWITCHED_IN_TraceMessage* message =
                    (SCANF_TASK_SWITCHED_IN_TraceMessage*)(&buf[i]);
                printf("    \"event_type:\": \"SCANF_TASK_SWITCHED_IN\",\n");
                printf("    \"timestamp:\": %d,\n", message->timestamp);
                printf("    \"task_number:\": %d\n", message->task_number);
                i += sizeof(SCANF_TASK_SWITCHED_IN_TraceMessage);
            } break;
            case SCANF_TASK_SWITCHED_OUT: {
                SCANF_TASK_SWITCHED_OUT_TraceMessage* message =
                    (SCANF_TASK_SWITCHED_OUT_TraceMessage*)(&buf[i]);
                printf("    \"event_type:\": \"SCANF_TASK_SWITCHED_OUT\",\n");
                printf("    \"timestamp:\": %d,\n", message->timestamp);
                printf("    \"task_number:\": %d\n", message->task_number);
                i += sizeof(SCANF_TASK_SWITCHED_OUT_TraceMessage);
            } break;
            default: {
                printf("unreachable\n");
                return 1;
            };
        }
        printf("  },\n");
    }
    printf("]\n");
    return 0;
}