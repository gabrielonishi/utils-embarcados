// Em DEFINES

// Definição da Task
#define TASK_STACK_SIZE (4096 / sizeof(portSTACK_TYPE))
#define TASK_STACK_PRIORITY (tskIDLE_PRIORITY)


/* --- --- --- --- --- --- --- --- --- --- --- --- */
// Em TASKS

static void task(void *pvParameters) {

    // Inits:
	
    // Definições

    // Rotina principal
    while (1) {

    }
}

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// Na MAIN

if (xTaskCreate(task, "task", TASK_STACK_SIZE, NULL, TASK_STACK_PRIORITY, NULL) != pdPASS) {
    printf("Falha em criar task\r\n");
} else {
    printf("Task criada \r\n");
}

vTaskStartScheduler();