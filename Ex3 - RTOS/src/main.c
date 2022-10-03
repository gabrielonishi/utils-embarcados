#include "asf.h"
#include "conf_board.h"

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// CONFIG DA PLACA

// LED 1
#define LED1_PIO PIOA
#define LED1_PIO_ID ID_PIOA
#define LED1_PIO_IDX 0
#define LED1_PIO_IDX_MASK (1 << LED1_PIO_IDX)

// Config do BUT1
#define BUT1_PIO PIOD
#define BUT1_PIO_ID ID_PIOD
#define BUT1_PIO_IDX 28
#define BUT1_PIO_IDX_MASK (1u << BUT1_PIO_IDX)
// Config do BUT2
#define BUT2_PIO PIOC
#define BUT2_PIO_ID ID_PIOC
#define BUT2_PIO_IDX 31
#define BUT2_PIO_IDX_MASK (1u << BUT2_PIO_IDX)
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

extern void vApplicationStackOverflowHook(xTaskHandle *pxTask,
                                          signed char *pcTaskName);
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

SemaphoreHandle_t xSemaphoreBut1;
SemaphoreHandle_t xSemaphoreBut2;

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// PROTOTYPES

static void USART1_init(void);
void init_but(Pio *pio, uint32_t id, uint32_t mask);
void init_led(Pio *pio, uint32_t id, uint32_t mask);
void init_IRQ(void);
void pisca_led(int n);

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// HANDLERS E CALLBACKS

void but1_callback(void) {
    xSemaphoreGiveFromISR(xSemaphoreBut1, 0);
}

void but2_callback(void) {
    xSemaphoreGiveFromISR(xSemaphoreBut2, 0);
}

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// TASKS

static void task_led(void *pvParameters) {

    printf("Entrou");

    init_led(LED1_PIO, LED1_PIO_ID, LED1_PIO_IDX_MASK);
    init_but(BUT1_PIO, BUT1_PIO_ID, BUT1_PIO_IDX_MASK);
    init_but(BUT2_PIO, BUT2_PIO_ID, BUT2_PIO_IDX_MASK);
    init_IRQ();

    int beeps = 5;

    while (1) {
        if (xSemaphoreTake(xSemaphoreBut1, 500 / portTICK_PERIOD_MS) == pdTRUE) {
            printf("task_led: %d \n", beeps);
            beeps++;
            pisca_led(beeps);
        }
        if (xSemaphoreTake(xSemaphoreBut2, 500 / portTICK_PERIOD_MS) == pdTRUE) {
            printf("task_led: %d \n", beeps);
            beeps--;
            pisca_led(beeps);
        }
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

void pisca_led(int n) {
    for (int i = 0; i < n; i++) {
        pio_clear(LED1_PIO, LED1_PIO_IDX_MASK);
        delay_ms(300);
        pio_set(LED1_PIO, LED1_PIO_IDX_MASK);
        delay_ms(300);
    }
}

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// INICIALIZAÇÕES

void init_but(Pio *pio, uint32_t id, uint32_t mask) {
    // Config do Botão
    pmc_enable_periph_clk(id);
    pio_set_input(pio, mask, PIO_DEFAULT);
    pio_pull_up(pio, mask, 1);
    pio_set_debounce_filter(pio, mask, 60);
}

void init_led(Pio *pio, uint32_t id, uint32_t mask) {
    // Config do LED
    pmc_enable_periph_clk(id);
    pio_set_output(pio, mask, 0, 0, 0);
}

void init_IRQ(void) {
    pio_handler_set(BUT1_PIO,
                    BUT1_PIO_ID,
                    BUT1_PIO_IDX_MASK,
                    PIO_IT_EDGE,
                    but1_callback);

    pio_enable_interrupt(BUT1_PIO, BUT1_PIO_IDX_MASK);
    pio_get_interrupt_status(BUT1_PIO);

    NVIC_EnableIRQ(BUT1_PIO_ID);
    NVIC_SetPriority(BUT1_PIO_ID, 4);

    pio_handler_set(BUT2_PIO,
                    BUT2_PIO_ID,
                    BUT2_PIO_IDX_MASK,
                    PIO_IT_EDGE,
                    but2_callback);

    pio_enable_interrupt(BUT2_PIO, BUT2_PIO_IDX_MASK);
    pio_get_interrupt_status(BUT2_PIO);

    NVIC_EnableIRQ(BUT2_PIO_ID);
    NVIC_SetPriority(BUT2_PIO_ID, 4);
}

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// MAIN

int main(void) {
    sysclk_init();
    board_init();
    configure_console();

    printf("Sistema inicializado \n");

    xSemaphoreBut1 = xSemaphoreCreateBinary();
    xSemaphoreBut2 = xSemaphoreCreateBinary();

    if (xSemaphoreBut1 == NULL) {
        printf("Falha em criar xSemaphoreBut1 \n");
    } else {
        printf("xSemaphoreBut1 criado \n");
    }
    if (xSemaphoreBut2 == NULL) {
        printf("Falha em criar xSemaphoreBut2 \n");
    } else {
        printf("xSemaphoreBut1 criado \n");
    }

    if (xTaskCreate(task_led, "Led", TASK_LED_STACK_SIZE, NULL, TASK_LED_STACK_PRIORITY, NULL) != pdPASS) {
        printf("Failed to create test led task\r\n");
    } else {
        printf("task led created \r\n");
    }

    vTaskStartScheduler();

    while (1) {
    }
    return 0;
}