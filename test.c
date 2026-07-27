#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Shared with the tracer so the two can never drift apart. */
#include "src/messages.h"

static int read_file(const char* path, char** out_buf, size_t* out_size) {
    FILE* fd = fopen(path, "rb");
    if (!fd) {
        fprintf(stderr, "fopen(%s) failed\n", path);
        return -1;
    }
    if (fseek(fd, 0, SEEK_END) != 0) {
        fprintf(stderr, "fseek() failed\n");
        fclose(fd);
        return -1;
    }
    long file_size = ftell(fd);
    if (file_size < 0) {
        fprintf(stderr, "ftell() failed\n");
        fclose(fd);
        return -1;
    }
    rewind(fd);

    char* buf = malloc((size_t)file_size + 1U);
    if (!buf) {
        fprintf(stderr, "malloc() failed\n");
        fclose(fd);
        return -1;
    }
    size_t size = fread(buf, 1, (size_t)file_size, fd);
    if (size != (size_t)file_size) {
        fprintf(stderr, "fread() failed: got %zu of %ld\n", size, file_size);
        free(buf);
        fclose(fd);
        return -1;
    }
    fclose(fd);

    *out_buf = buf;
    *out_size = size;
    return 0;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <tracelog_file>\n", argv[0]);
        return 1;
    }

    char* buf = NULL;
    size_t size = 0;
    if (read_file(argv[1], &buf, &size) != 0) {
        return 1;
    }

    int status = 0;
    size_t i = 0;
    int first = 1;
    printf("[\n");
    while (i < size) {
        size_t remaining = size - i;
        SCANF_EventType* message_type = (SCANF_EventType*)(&buf[i]);

        /* Refuse to read past the end of a truncated final record. */
        size_t needed = 0;
        switch (*message_type) {
            case SCANF_CONFIG: needed = sizeof(SCANF_CONFIG_TraceMessage); break;
            case SCANF_TASK_CREATE: needed = sizeof(SCANF_TASK_CREATE_TraceMessage); break;
            case SCANF_TASK_SWITCHED_IN:
                needed = sizeof(SCANF_TASK_SWITCHED_IN_TraceMessage);
                break;
            case SCANF_TASK_SWITCHED_OUT:
                needed = sizeof(SCANF_TASK_SWITCHED_OUT_TraceMessage);
                break;
            case SCANF_OVERFLOW: needed = sizeof(SCANF_OVERFLOW_TraceMessage); break;
            default:
                fprintf(stderr, "unknown event type %d at offset %zu\n", (int)*message_type, i);
                status = 1;
                goto done;
        }
        if (remaining < needed) {
            fprintf(stderr, "truncated record at offset %zu: need %zu bytes, have %zu\n", i, needed,
                    remaining);
            status = 1;
            goto done;
        }

        if (!first) printf("  },\n");
        printf("  {\n");
        first = 0;

        switch (*message_type) {
            case SCANF_CONFIG: {
                SCANF_CONFIG_TraceMessage* message = (SCANF_CONFIG_TraceMessage*)(&buf[i]);
                printf("    \"event_type\": \"SCANF_CONFIG\",\n");
                printf("    \"version\": \"%d.%d.%d\",\n", message->version.major,
                       message->version.minor, message->version.patch);
                printf("    \"config_max_task_name_len\": %d\n",
                       message->config_max_task_name_len);
            } break;
            case SCANF_TASK_CREATE: {
                SCANF_TASK_CREATE_TraceMessage* message =
                    (SCANF_TASK_CREATE_TraceMessage*)(&buf[i]);
                printf("    \"event_type\": \"SCANF_TASK_CREATE\",\n");
                printf("    \"timestamp\": %" PRIu32 ",\n", message->timestamp);
                printf("    \"task_number\": %" PRIu32 ",\n", message->task_number);
                printf("    \"task_name\": \"%.*s\"\n", SCANF_MAX_TASK_NAME_LEN,
                       message->task_name);
            } break;
            case SCANF_TASK_SWITCHED_IN: {
                SCANF_TASK_SWITCHED_IN_TraceMessage* message =
                    (SCANF_TASK_SWITCHED_IN_TraceMessage*)(&buf[i]);
                printf("    \"event_type\": \"SCANF_TASK_SWITCHED_IN\",\n");
                printf("    \"timestamp\": %" PRIu32 ",\n", message->timestamp);
                printf("    \"task_number\": %" PRIu32 "\n", message->task_number);
            } break;
            case SCANF_TASK_SWITCHED_OUT: {
                SCANF_TASK_SWITCHED_OUT_TraceMessage* message =
                    (SCANF_TASK_SWITCHED_OUT_TraceMessage*)(&buf[i]);
                printf("    \"event_type\": \"SCANF_TASK_SWITCHED_OUT\",\n");
                printf("    \"timestamp\": %" PRIu32 ",\n", message->timestamp);
                printf("    \"task_number\": %" PRIu32 "\n", message->task_number);
            } break;
            case SCANF_OVERFLOW: {
                SCANF_OVERFLOW_TraceMessage* message = (SCANF_OVERFLOW_TraceMessage*)(&buf[i]);
                printf("    \"event_type\": \"SCANF_OVERFLOW\",\n");
                printf("    \"timestamp\": %" PRIu32 ",\n", message->timestamp);
                printf("    \"dropped\": %" PRIu32 "\n", message->dropped);
            } break;
            default: break; /* already rejected above */
        }
        i += needed;
    }
    if (!first) printf("  }\n");

done:
    printf("]\n");
    free(buf);
    return status;
}
