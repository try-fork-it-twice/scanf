#include <FreeRTOS.h>
#include <assert.h>
#include <queue.h>
#include <semphr.h>
#include <stdio.h>
#include <task.h>
#include <timers.h>

#include <scanf.h>

static void exampleTask(void* parameters) __attribute__((noreturn));

static void exampleTask(void* parameters) {
    for (;;) {
        printf("Tick\n");
        vTaskDelay(100);
    }
}

int main(void) {
    static StaticTask_t exampleTaskTCB;
    static StackType_t exampleTaskStack[configMINIMAL_STACK_SIZE];

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

    xTaskCreateStatic(exampleTask, "example", configMINIMAL_STACK_SIZE, NULL,
        configMAX_PRIORITIES - 1U, &(exampleTaskStack[0]), &(exampleTaskTCB));

    scanf_stop_tracing();

    scanf_save_tracelog("data.bin");

    vTaskStartScheduler();
    assert(0 && "Should not reach here.");
    return 0;
}

#if (configCHECK_FOR_STACK_OVERFLOW > 0)
void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName) {}
#endif /* #if ( configCHECK_FOR_STACK_OVERFLOW > 0 ) */
