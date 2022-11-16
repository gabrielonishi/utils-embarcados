#include "asf.h"
#include "conf_board.h"

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// CONFIG DA PLACA

// Led Embutido
#define LED_PIO PIOC
#define LED_PIO_ID ID_PIOC
#define LED_PIO_IDX 8
#define LED_PIO_IDX_MASK (1 << LED_PIO_IDX)

// Struct p/ RTC
typedef struct  {
  uint32_t year;
  uint32_t month;
  uint32_t day;
  uint32_t week;
  uint32_t hour;
  uint32_t minute;
  uint32_t second;
} calendar;

volatile char flag_rtc_alarm = 0;

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

void init_led(Pio *pio, uint32_t id, uint32_t mask);
void RTC_init(Rtc *rtc, uint32_t id_rtc, calendar t, uint32_t irq_type);
static void USART1_init(void);

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// HANDLERS E CALLBACKS

void RTC_Handler(void) {
    uint32_t ul_status = rtc_get_status(RTC);
	
    /* seccond tick */
    if ((ul_status & RTC_SR_SEC) == RTC_SR_SEC) {	
		printf("%d \n", ul_status);
    }
	
    /* Time or date alarm */
    if ((ul_status & RTC_SR_ALARM) == RTC_SR_ALARM) {
    	// o código para irq de alame vem aqui
        flag_rtc_alarm = 1;
// 		printf("ul_status: %d \n", ul_status);
// 		printf("RTC_SR_ALARM: %d \n", RTC_SR_ALARM);
// 		printf("RTC_SR_SEC: %d \n", RTC_SR_SEC);

    }

    rtc_clear_status(RTC, RTC_SCCR_SECCLR);
    rtc_clear_status(RTC, RTC_SCCR_ALRCLR);
    rtc_clear_status(RTC, RTC_SCCR_ACKCLR);
    rtc_clear_status(RTC, RTC_SCCR_TIMCLR);
    rtc_clear_status(RTC, RTC_SCCR_CALCLR);
    rtc_clear_status(RTC, RTC_SCCR_TDERRCLR);
}

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// TASKS

static void task(void *pvParameters) {

    // Inits:
    init_led(LED_PIO, LED_PIO_ID, LED_PIO_IDX_MASK);
    // Configura o tempo inicial
    calendar rtc_initial = {2018, 3, 19, 12, 15, 45 ,1}; 
    // Configura com IRQ com alarme
    // RTC_init(RTC, ID_RTC, rtc_initial, RTC_IER_ALREN);
	RTC_init(RTC, ID_RTC, rtc_initial, RTC_IER_SECEN);
    // Leitura do valor atual do RTC       
    uint32_t current_hour, current_min, current_sec;
    uint32_t current_year, current_month, current_day, current_week;
    rtc_get_time(RTC, &current_hour, &current_min, &current_sec);
    rtc_get_date(RTC, &current_year, &current_month, &current_day, &current_week);
	
	printf("Ano: %d\n", current_year);
	printf("Mes: %d\n", current_month);
	printf("Semana: %d\n", current_week);
	printf("Dia: %d\n", current_day);
	printf("Hora: %d\n", current_hour);
	printf("Minuto: %d\n", current_min);
	printf("Segundo: %d\n", current_sec);
	
	//rtc_set_date_alarm(RTC, 1, current_month, 1, current_day);
	//rtc_set_time_alarm(RTC, 1, current_hour, 1, current_min, 1, current_sec + 10);

    while (1) {
		if(flag_rtc_alarm){
			for(int i=0; i<10; i++){
				pio_clear(LED_PIO, LED_PIO_IDX_MASK);
				delay_ms(100);
				pio_set(LED_PIO, LED_PIO_IDX_MASK);
				delay_ms(100);
			}
		}
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

void init_led(Pio *pio, uint32_t id, uint32_t mask) {
    // Config do LED
    pmc_enable_periph_clk(id);
    pio_set_output(pio, mask, 0, 0, 0);
}

void RTC_init(Rtc *rtc, uint32_t id_rtc, calendar t, uint32_t irq_type) {

	pmc_enable_periph_clk(ID_RTC);

	// Configura tipo de relógio (24hrs)
	rtc_set_hour_mode(rtc, 0);

	// Configura data e hora manualmente
	rtc_set_date(rtc, t.year, t.month, t.day, t.week);
	rtc_set_time(rtc, t.hour, t.minute, t.second);

	// Configure RTC interrupts
	NVIC_DisableIRQ(id_rtc);
	NVIC_ClearPendingIRQ(id_rtc);
	NVIC_SetPriority(id_rtc, 4);
	NVIC_EnableIRQ(id_rtc);

	// Ativa interrupcao via alarme
	rtc_enable_interrupt(rtc,  irq_type);
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