#include "asf.h"
#include "conf_board.h"
// Includes OLED
// #include "gfx_mono_text.h"
// #include "gfx_mono_ug_2832hsweg04.h"
// #include "sysfont.h"
#include "gfx_mono_ug_2832hsweg04.h"
#include "gfx_mono_text.h"
#include "sysfont.h"
#include "sensor.h"
/* --- --- --- --- --- --- --- --- --- --- --- --- */
// CONFIG DA PLACA

/* Botao da placa */
#define LED_1_PIO PIOA
#define LED_1_PIO_ID ID_PIOA
#define LED_1_IDX 0
#define LED_1_IDX_MASK (1 << LED_1_IDX)

#define LED_2_PIO PIOC
#define LED_2_PIO_ID ID_PIOC
#define LED_2_IDX 30
#define LED_2_IDX_MASK (1 << LED_2_IDX)

#define LED_3_PIO PIOB
#define LED_3_PIO_ID ID_PIOB
#define LED_3_IDX 2
#define LED_3_IDX_MASK (1 << LED_3_IDX)

#define BUT_1_PIO PIOD
#define BUT_1_PIO_ID ID_PIOD
#define BUT_1_IDX 28
#define BUT_1_IDX_MASK (1u << BUT_1_IDX)

#define BUT_2_PIO PIOA
#define BUT_2_PIO_ID ID_PIOA
#define BUT_2_IDX 19
#define BUT_2_IDX_MASK (1u << BUT_2_IDX)

#define BUT_3_PIO PIOC
#define BUT_3_PIO_ID ID_PIOC
#define BUT_3_IDX 31
#define BUT_3_IDX_MASK (1u << BUT_3_IDX)

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// CONFIG UART

#define USART_COM_ID ID_USART1
#define USART_COM USART1

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// CONFIG RTOS
#define TASK_STACK_SIZE (4096 / sizeof(portSTACK_TYPE))
#define TASK_STACK_PRIORITY (tskIDLE_PRIORITY)

extern void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName);
extern void vApplicationIdleHook(void);
extern void vApplicationTickHook(void);
extern void vApplicationMallocFailedHook(void);
extern void xPortSysTickHandler(void);

extern void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName) {
    printf("stack overflow %x %s\r\n", pxTask, (portCHAR *)pcTaskName);
    while (1) {
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

SemaphoreHandle_t xBut1Semaphore;
SemaphoreHandle_t xTimerSemaphore;
QueueHandle_t xCompressorQueue;

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// PROTOTYPES

static void USART1_init(void);
void io_init(void);
static void RTT_init(float freqPrescale, uint32_t IrqNPulses, uint32_t rttIRQSource);

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// HANDLERS E CALLBACKS

void RTT_Handler(void) {
	uint32_t ul_status;
	ul_status = rtt_get_status(RTT);

	/* IRQ due to Alarm */
	if ((ul_status & RTT_SR_ALMS) == RTT_SR_ALMS) {
		xSemaphoreGiveFromISR(xTimerSemaphore, 0);
	}
}

void but1_callback(void) {
	xSemaphoreGiveFromISR(xBut1Semaphore, 0);
}

void but2_callback(void) { }

void but3_callback(void) { }
/* --- --- --- --- --- --- --- --- --- --- --- --- */
// TASKS

static void task_main(void *pvParameters) {

    // Inits:
    gfx_mono_ssd1306_init();
	io_init();
	RTT_init(4, 16, RTT_MR_ALMIEN);

    // Variáveis
	int contador=0;
	
	gfx_mono_draw_string("NORMAL", 0, 0, &sysfont);
	pio_set(LED_1_PIO, LED_1_IDX_MASK);
	pio_set(LED_2_PIO, LED_2_IDX_MASK);
	pio_set(LED_3_PIO, LED_3_IDX_MASK);

    while (1) {
        if(xSemaphoreTake(xBut1Semaphore, 500/portTICK_PERIOD_MS)==pdTRUE){
			contador++;
			switch(contador%4){
				case 0:
					gfx_mono_draw_string("NORMAL", 0, 0, &sysfont);
					delay_s(1);
					gfx_mono_draw_string("*     ", 0, 0, &sysfont);
					xQueueSend(xCompressorQueue, &contador, 0);
					delay_s(1);
					gfx_mono_draw_string("NORMAL", 0, 0, &sysfont);
				break;
				case 1:
					gfx_mono_draw_string("ECO   ", 0, 0, &sysfont);
					delay_s(1);
					gfx_mono_draw_string("*     ", 0, 0, &sysfont);
					xQueueSend(xCompressorQueue, &contador, 0);
					delay_s(1);
					gfx_mono_draw_string("ECO   ", 0, 0, &sysfont);

				break;
				case 2:
					gfx_mono_draw_string("TURBO ", 0, 0, &sysfont);
					delay_s(1);
					gfx_mono_draw_string("*     ", 0, 0, &sysfont);
					xQueueSend(xCompressorQueue, &contador, 0);
					delay_s(1);
					gfx_mono_draw_string("TURBO ", 0, 0, &sysfont);
				break;
				case 3:
					gfx_mono_draw_string("FESTA ", 0, 0, &sysfont);
					delay_s(1);
					gfx_mono_draw_string("*     ", 0, 0, &sysfont);
					xQueueSend(xCompressorQueue, &contador, 0);
					delay_s(1);
					gfx_mono_draw_string("FESTA ", 0, 0, &sysfont);
				break;
			}
		}		
	}
}

void pisca_led(int on, int off){
	pio_clear(LED_1_PIO, LED_1_IDX_MASK);
	delay_s(on);
	pio_set(LED_1_PIO, LED_1_IDX_MASK);
	delay_s(off);
}

static void task_compressor(void *pvParameters) {
	// Inits:
	io_init();
	// Definições
	int contador = 0;
	int on = 3;
	int off = 3;
	// Rotina principal
	pio_set(LED_1_PIO, LED_1_IDX_MASK);
	pio_set(LED_2_PIO, LED_2_IDX_MASK);
	pio_set(LED_3_PIO, LED_3_IDX_MASK);
	
	while (1) {
		if(xQueueReceive(xCompressorQueue, &contador, ( TickType_t) 500)){
			printf("%d", contador);
			switch(contador%4){
				case 0:
				on = 3;
				off = 3;
				break;
				case 1:
				on = 1;
				off = 5;
				break;
				case 2:
				on = 5;
				off = 3;
				break;
				case 3:
				on = 5;
				off = 1;
				break;
			}		
		}
		pisca_led(on, off);
	}
}

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// FUNÇÕES

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// CONFIGURAÇÃO DO TERMINAL

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

// Inicializa o RTT
static void RTT_init(float freqPrescale, uint32_t IrqNPulses, uint32_t rttIRQSource) {

	uint16_t pllPreScale = (int) (((float) 32768) / freqPrescale);
	
	rtt_sel_source(RTT, false);
	rtt_init(RTT, pllPreScale);
	
	if (rttIRQSource & RTT_MR_ALMIEN) {
		uint32_t ul_previous_time;
		ul_previous_time = rtt_read_timer_value(RTT);
		while (ul_previous_time == rtt_read_timer_value(RTT));
		rtt_write_alarm_time(RTT, IrqNPulses+ul_previous_time);
	}

	// config NVIC
	NVIC_DisableIRQ(RTT_IRQn);
	NVIC_ClearPendingIRQ(RTT_IRQn);
	NVIC_SetPriority(RTT_IRQn, 4);
	NVIC_EnableIRQ(RTT_IRQn);

	/* Enable RTT interrupt */
	if (rttIRQSource & (RTT_MR_RTTINCIEN | RTT_MR_ALMIEN))
	rtt_enable_interrupt(RTT, rttIRQSource);
	else
	rtt_disable_interrupt(RTT, RTT_MR_RTTINCIEN | RTT_MR_ALMIEN);

}

void io_init(void) {
	pmc_enable_periph_clk(LED_1_PIO_ID);
	pmc_enable_periph_clk(LED_2_PIO_ID);
	pmc_enable_periph_clk(LED_3_PIO_ID);
	pmc_enable_periph_clk(BUT_1_PIO_ID);
	pmc_enable_periph_clk(BUT_2_PIO_ID);
	pmc_enable_periph_clk(BUT_3_PIO_ID);

	pio_configure(LED_1_PIO, PIO_OUTPUT_0, LED_1_IDX_MASK, PIO_DEFAULT);
	pio_configure(LED_2_PIO, PIO_OUTPUT_0, LED_2_IDX_MASK, PIO_DEFAULT);
	pio_configure(LED_3_PIO, PIO_OUTPUT_0, LED_3_IDX_MASK, PIO_DEFAULT);

	pio_configure(BUT_1_PIO, PIO_INPUT, BUT_1_IDX_MASK, PIO_PULLUP| PIO_DEBOUNCE);
	pio_configure(BUT_2_PIO, PIO_INPUT, BUT_2_IDX_MASK, PIO_PULLUP| PIO_DEBOUNCE);
	pio_configure(BUT_3_PIO, PIO_INPUT, BUT_3_IDX_MASK, PIO_PULLUP| PIO_DEBOUNCE);

	pio_handler_set(BUT_1_PIO, BUT_1_PIO_ID, BUT_1_IDX_MASK, PIO_IT_FALL_EDGE,
	but1_callback);
	pio_handler_set(BUT_2_PIO, BUT_2_PIO_ID, BUT_2_IDX_MASK, PIO_IT_FALL_EDGE,
	but3_callback);
	pio_handler_set(BUT_3_PIO, BUT_3_PIO_ID, BUT_3_IDX_MASK, PIO_IT_FALL_EDGE,
	but2_callback);

	pio_enable_interrupt(BUT_1_PIO, BUT_1_IDX_MASK);
	pio_enable_interrupt(BUT_2_PIO, BUT_2_IDX_MASK);
	pio_enable_interrupt(BUT_3_PIO, BUT_3_IDX_MASK);

	pio_get_interrupt_status(BUT_1_PIO);
	pio_get_interrupt_status(BUT_2_PIO);
	pio_get_interrupt_status(BUT_3_PIO);

	NVIC_EnableIRQ(BUT_1_PIO_ID);
	NVIC_SetPriority(BUT_1_PIO_ID, 4);

	NVIC_EnableIRQ(BUT_2_PIO_ID);
	NVIC_SetPriority(BUT_2_PIO_ID, 4);

	NVIC_EnableIRQ(BUT_3_PIO_ID);
	NVIC_SetPriority(BUT_3_PIO_ID, 4);
}


/* --- --- --- --- --- --- --- --- --- --- --- --- */
// MAIN

int main(void) {
    sysclk_init();
    board_init();
    configure_console();

    printf("Sistema inicializado \n");

    // Crie suas filas e semáforos aqui 
    xBut1Semaphore = xSemaphoreCreateBinary();
    if (xBut1Semaphore == NULL) {
        printf("Falha em criar xBut1Semaphoree \n");
    } else {
        printf("xBut1Semaphore criado \n");
    }

    // Crie suas filas e semáforos aqui
    xTimerSemaphore = xSemaphoreCreateBinary();
    if (xTimerSemaphore == NULL) {
	    printf("Falha em criar xTimerSemaphoree \n");
	    } else {
	    printf("xTimerSemaphore criado \n");
    }
	
	xCompressorQueue = xQueueCreate(32, sizeof(int));
    if (xCompressorQueue == NULL) {
	    printf("Falha em criar xCompressorQueue \n");
	    } else {
	    printf("xCompressorQueue criado \n");
    }
	
    // Crie suas tasks aqui

    if (xTaskCreate(task_main, "task_main", TASK_STACK_SIZE, NULL, TASK_STACK_PRIORITY, NULL) != pdPASS) {
        printf("Falha em criar task_main\r\n");
    } else {
        printf("task_main criada \r\n");
    }
	
	if (xTaskCreate(task_compressor, "task_compressor", TASK_STACK_SIZE, NULL, TASK_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Falha em criar task_compressor\r\n");
		} else {
		printf("task_compressor criada \r\n");
	}

    vTaskStartScheduler();

    while (1) {
    }
    return 0;
}