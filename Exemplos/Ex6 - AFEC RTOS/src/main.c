// INCLUDES
#include "asf.h"
#include "conf_board.h"
#include "rtos_config/rtos_config.h"
#include "perifericos/perifericos.h"

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// DEFINES

#define TASK_STACK_SIZE (4096 / sizeof(portSTACK_TYPE))
#define TASK_STACK_PRIORITY (tskIDLE_PRIORITY)

/* Coloque seus defines aqui /*

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// RECURSOS RTOS

TimerHandle_t xTimerStrain;



/* --- --- --- --- --- --- --- --- --- --- --- --- */
// PROTOTYPES

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// HANDLERS E CALLBACKS

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// TASKS

void vTimerCallback(TimerHandle_t xTimerStrain) {
	/* Selecina canal e inicializa conversão */
	afec_channel_enable(AFEC_STRAIN, AFEC_STRAIN_CHANNEL);
	afec_start_software_conversion(AFEC_STRAIN);
}

static void task(void *pvParameters) {
	// Inits:
	config_AFEC_strain(AFEC_STRAIN, AFEC_STRAIN_ID, AFEC_STRAIN_CHANNEL, AFEC_strain_callback());
	xTimerStrain = xTimerCreate(/* Just a text name, not used by the RTOS
				kernel. */
				"Timer",
				/* The timer period in ticks, must be
				greater than 0. */
				100,
				/* The timers will auto-reload themselves
				when they expire. */
				pdTRUE,
				/* The ID is used to store a count of the
				number of times the timer has expired, which
				is initialised to 0. */
				(void *)0,
				/* Timer callback */
				vTimerCallback);
	xTimerStart(xTimerStrain, 0);
	// Variáveis
	uint32_t sD;
	while (1) {
	    if (xQueueReceive(xQueueStrain, &(sD), 1000)) {
		    printf("Strain Gauge: %d \n", sD);
		    } else {
		    printf("Nao chegou um novo dado em 1 segundo");
	    }
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
	xQueueStrain = xQueueCreate(100, sizeof(uint32_t));
	if (xQueueStrain == NULL)
	printf("falha em criar a queue xQueueStrain \n");

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