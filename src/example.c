/*
 * FreeRTOS Kernel <DEVELOPMENT BRANCH>
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/*
 * This is a simple main that will start the FreeRTOS-Kernel and run a periodic task
 * that only delays if compiled with the template port, this project will do nothing.
 * For more information on getting started please look here:
 * https://www.freertos.org/Documentation/01-FreeRTOS-quick-start/01-Beginners-guide/02-Quick-start-guide
 */

#include <FreeRTOS.h>
#include <assert.h>
#include <queue.h>
#include <semphr.h>
#include <stdio.h>
#include <task.h>
#include <timers.h>

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

    // traceENTER_xTaskCreateStatic(1, 2, 3, 4, 5, 6);

    xTaskCreateStatic(exampleTask, "example", configMINIMAL_STACK_SIZE, NULL,
                      configMAX_PRIORITIES - 1U, &(exampleTaskStack[0]), &(exampleTaskTCB));

    vTaskStartScheduler();
    assert(0 && "Should not reach here.");
    return 0;
}

#if (configCHECK_FOR_STACK_OVERFLOW > 0)
void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName) {}
#endif /* #if ( configCHECK_FOR_STACK_OVERFLOW > 0 ) */
