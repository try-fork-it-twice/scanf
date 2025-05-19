#include <FreeRTOS.h>
#include <assert.h>
#include <queue.h>
#include <semphr.h>
#include <stdio.h>
#include <stdlib.h>
#include <task.h>
#include <timers.h>

#include <scanf.h>

static void exampleTask(void* parameters) __attribute__((noreturn));

static void exampleTask(void* parameters) {
    size_t i = 0;
    for (;;) {
        printf("Tick 0\n");
        for (size_t j = 0; j < 1000; j++) {
            printf("1\n");
        }
        vTaskDelay(100);

        if (i++ == 100) {
            SCANF_Tracelog *tl = scanf_get_tracelog();
            printf("Saving trace log. Trace log size %d\n", tl->size);
            scanf_stop_tracing();
            scanf_save_tracelog("data.bin");
            exit(0);
        }
    }
}


static void exampleTask1(void* parameters) __attribute__((noreturn));

static void exampleTask1(void* parameters) {
    size_t i = 0;
    for (;;) {
        printf("Tick 1\n");
        for (size_t j = 0; j < 5000; j++) {
            printf("2\n");
        }
        vTaskDelay(500);
    }
}


static void exampleTask2(void* parameters) __attribute__((noreturn));

static void exampleTask2(void* parameters) {
    size_t i = 0;
    for (;;) {
        printf("Tick 2\n");
        for (size_t j = 0; j < 10000; j++) {
            printf("3\n");
        }
        vTaskDelay(1000);
    }
}


int main(void) {
    static StaticTask_t exampleTaskTCB;
    static StackType_t exampleTaskStack[configMINIMAL_STACK_SIZE];

    static StaticTask_t exampleTask1TCB;
    static StackType_t exampleTask1Stack[configMINIMAL_STACK_SIZE];

    static StaticTask_t exampleTask2TCB;
    static StackType_t exampleTask2Stack[configMINIMAL_STACK_SIZE];


    printf("Example FreeRTOS Project\n");

    const int BUFFER_SIZE = 1024;
    SCANF_EventType messages[BUFFER_SIZE];
    SCANF_Tracelog tracelog = {
        .size = 0,
        .capacity = BUFFER_SIZE,
        .messages = messages,
    };
    scanf_init(&tracelog);

    scanf_start_tracing();

    xTaskCreateStatic(exampleTask, "task0", configMINIMAL_STACK_SIZE, NULL,
        configMAX_PRIORITIES - 1U, &(exampleTaskStack[0]), &(exampleTaskTCB));

    xTaskCreateStatic(exampleTask1, "task1", configMINIMAL_STACK_SIZE, NULL,
        configMAX_PRIORITIES - 1U, &(exampleTask1Stack[0]), &(exampleTask1TCB));

    xTaskCreateStatic(exampleTask2, "task2", configMINIMAL_STACK_SIZE, NULL,
        configMAX_PRIORITIES - 1U, &(exampleTask2Stack[0]), &(exampleTask2TCB));

    vTaskStartScheduler();
    assert(0 && "Should not reach here.");
    return 0;
}

#if (configCHECK_FOR_STACK_OVERFLOW > 0)
void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName) {}
#endif /* #if ( configCHECK_FOR_STACK_OVERFLOW > 0 ) */
