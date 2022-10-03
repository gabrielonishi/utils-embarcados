#include "asf.h"
#include "conf_board.h"

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// CONFIG DA PLACA

/* Coloque as configurações de LEDs e botões aqui /*

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// CONFIG UART

#define USART_COM_ID ID_USART1
#define USART_COM USART1

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// CONFIG RTOS
#define TASK_LED_STACK_SIZE (4096 / sizeof(portSTACK_TYPE))
#define TASK_LED_STACK_PRIORITY (tskIDLE_PRIORITY)
#define TASK_BUT_STACK_SIZE (4096 / sizeof(portSTACK_TYPE))
#define TASK_BUT_STACK_PRIORITY (tskIDLE_PRIORITY)

extern void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName);
extern void vApplicationIdleHook(void);
extern void vApplicationTickHook(void);
extern void vApplicationMallocFailedHook(void);
extern void xPortSysTickHandler(void);

extern void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName) {
    printf("stack overflow %x %s\r\n", pxTask, (portCHAR *)pcTaskName);
    for (;;) {
    }
}

extern void vApplicationIdleHook(void) {
    pmc_sleep(SAM_PM_SMODE_SLEEP_WFI);
}

extern void vApplicationTickHook(void) {}

extern void vApplicationMallocFailedHook(void) {
    configASSERT((volatile void *)NULL);
}

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// RECURSOS RTOS

/* Coloque os semáforos e filas aqui /*

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// PROTOTYPES

void init(void);
void init_IRQ(void);
static void USART1_init(void);

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// HANDLERS E CALLBACKS

/* Coloque suas funções de callback aqui /*

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// TASKS

static void task(void *pvParameters) {

    // Inits:
    init_IRQ();

    // Variáveis
    int beeps = 5;

    while (1) {
        // Rotina
    }
}
/* --- --- --- --- --- --- --- --- --- --- --- --- */
// Funções

// Configura UART
static void configure_console(void) {
    const usart_serial_options_t uart_serial_options = {
        .baudrate = CONF_UART_BAUDRATE,
        .charlength = CONF_UART_CHAR_LENGTH,
        .paritytype = CONF_UART_PARITY,
        .stopbits = CONF_UART_STOP_BITS,
    };

    /* Configure console UART. */
    stdio_serial_init(CONF_UART, &uart_serial_options);

    /* Specify that stdout should not be buffered. */
    setbuf(stdout, NULL);
}

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// INICIALIZAÇÕES

void init_IRQ(void) {
    pio_handler_set(BUT_PIO,
                    BUT_PIO_ID,
                    BUT_PIO_IDX_MASK,
                    PIO_IT_EDGE,
                    but_callback);

    pio_enable_interrupt(BUT_PIO, BUT_PIO_IDX_MASK);
    pio_get_interrupt_status(BUT_PIO);

    NVIC_EnableIRQ(BUT_PIO_ID);
    NVIC_SetPriority(BUT_PIO_ID, 4);
}

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// MAIN

int main(void) {
    sysclk_init();
    board_init();
    configure_console();

    printf("Sistema inicializado \n");

    // Crie suas filas e semáforos aqui

    // Crie suas tasks aqui

    if (xTaskCreate(task, "task", TASK_LED_STACK_SIZE, NULL, TASK_LED_STACK_PRIORITY, NULL) != pdPASS) {
        printf("Falha em criar task\r\n");
    } else {
        printf("Task criada \r\n");
    }

    vTaskStartScheduler();

    while (1) {
    }
    return 0;
}