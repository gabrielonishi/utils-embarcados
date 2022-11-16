#ifndef RTOS_CONFIG_H_
#define RTOS_CONFIG_H_

// Defines do terminal
#define USART_COM_ID ID_USART1
#define USART_COM USART1

#include "asf.h"
#include "conf_board.h"

// Funções do terminal
static void configure_console(void);

// Funções do RTOS
extern void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName);
extern void vApplicationIdleHook(void);
extern void vApplicationTickHook(void);
extern void vApplicationMallocFailedHook(void);
extern void xPortSysTickHandler(void);

#endif /* RTOS_CONFIG_H_ */