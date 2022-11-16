// INCLUDES
#include "asf.h"
#include "conf_board.h"
#include "uart_config/uart_config.h"
#include "rtos_config/rtos_config.h"

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// DEFINES

#define TASK_STACK_SIZE (4096 / sizeof(portSTACK_TYPE))
#define TASK_STACK_PRIORITY (tskIDLE_PRIORITY)

/* Coloque seus defines aqui /*

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// RECURSOS RTOS

/* Coloque os semáforos e filas aqui /*

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// PROTOTYPES

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// HANDLERS E CALLBACKS

/* Coloque suas funções de callback aqui /*

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// TASKS

static void task(void *pvParameters) {

    // Inits:

    // Variáveis

    while (1) {
        // Rotina
    }
}

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// FUNÇÕES

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// INICIALIZAÇÕES

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// MAIN

int main(void) {
    sysclk_init();
    board_init();
    configure_console();

    printf("Sistema inicializado \n");

    // Crie suas filas e semáforos aqui

    // Crie suas tasks aqui

    if (xTaskCreate(task, "task", TASK_STACK_SIZE, NULL, TASK_STACK_PRIORITY, NULL) != pdPASS) {
        printf("Falha em criar task\r\n");
    } else {
        printf("Task criada \r\n");
    }

    vTaskStartScheduler();

    while (1) {
    }
    return 0;
}