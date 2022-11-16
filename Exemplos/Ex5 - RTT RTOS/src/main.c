// INCLUDES
#include "asf.h"
#include "conf_board.h"
#include "rtos_config/rtos_config.h"
#include "uart_config/uart_config.h"

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// DEFINES

// Led Embutido
#define LED_PIO PIOC
#define LED_PIO_ID ID_PIOC
#define LED_PIO_IDX 8
#define LED_PIO_IDX_MASK (1 << LED_PIO_IDX)

#define TASK_STACK_SIZE (4096 / sizeof(portSTACK_TYPE))
#define TASK_STACK_PRIORITY (tskIDLE_PRIORITY)

volatile char flag_rtt = 0;

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// RECURSOS RTOS

/* Coloque os semáforos e filas aqui /*

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// PROTOTYPES

static void RTT_init(float freqPrescale, uint32_t IrqNPulses, uint32_t rttIRQSource);

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// HANDLERS E CALLBACKS

void RTT_Handler(void) {
    uint32_t ul_status;
    ul_status = rtt_get_status(RTT);

    /* IRQ due to Alarm */
    if ((ul_status & RTT_SR_ALMS) == RTT_SR_ALMS) {
        flag_rtt = 1;
    }
}
/* --- --- --- --- --- --- --- --- --- --- --- --- */
// TASKS

static void task(void *pvParameters) {

    // Inits:

    // inicializa rtt como alarme
    RTT_init(4, 16, RTT_MR_ALMIEN);  

    // Variáveis

    while (1) {
        if(flag_rtt){
            	for(int i=0; i<10; i++){
				pio_clear(LED_PIO, LED_PIO_IDX_MASK);
				delay_ms(100);
				pio_set(LED_PIO, LED_PIO_IDX_MASK);
				delay_ms(100);
			    }
                flag_rtt = 0;
                RTT_init(4, 16, RTT_MR_ALMIEN); 
        }
    }
}

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// FUNÇÕES

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// INICIALIZAÇÕES

// Inicializa o RTT
static void RTT_init(float freqPrescale, uint32_t IrqNPulses, uint32_t rttIRQSource) {

    uint16_t pllPreScale = (int)(((float)32768) / freqPrescale);

    rtt_sel_source(RTT, false);
    rtt_init(RTT, pllPreScale);

    if (rttIRQSource & RTT_MR_ALMIEN) {
        uint32_t ul_previous_time;
        ul_previous_time = rtt_read_timer_value(RTT);
        while (ul_previous_time == rtt_read_timer_value(RTT));
        rtt_write_alarm_time(RTT, IrqNPulses + ul_previous_time);
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