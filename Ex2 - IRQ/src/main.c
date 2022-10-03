/* *
 * Arquivo baseado no Lab. 2 de Embarcados
 * 
 * Trabalha em sleep mode com interrupções para piscar LEDs
 * 
 * Trechos de código retirados do Nick (Nicolas Queiroga)
 * com a permissão dele
 */

#include "asf.h"
//LED embutido
#define LED_PIO PIOC
#define LED_PIO_ID ID_PIOC
#define LED_PIO_IDX 8
#define LED_PIO_IDX_MASK (1 << LED_PIO_IDX)
// LED 1
#define LED1_PIO PIOA
#define LED1_PIO_ID ID_PIOA
#define LED1_PIO_IDX 0
#define LED1_PIO_IDX_MASK (1 << LED1_PIO_IDX)
// Config do botão embutido
#define BUT_PIO PIOA
#define BUT_PIO_ID ID_PIOA
#define BUT_PIO_IDX 11
#define BUT_PIO_IDX_MASK (1u << BUT_PIO_IDX)
// Config do BUT1
#define BUT1_PIO PIOD
#define BUT1_PIO_ID ID_PIOD
#define BUT1_PIO_IDX 28
#define BUT1_PIO_IDX_MASK (1u << BUT1_PIO_IDX)

void init(void);
void init_led_embutido(void);
void init_led1(void);
void init_but_embutido(void);
void init_but1(void);
void init_IRQ(void);

volatile _Bool but_flag = 0;
volatile _Bool but1_flag = 0;

void but_callback(void){
	but_flag = 1;
}

void but1_callback(void){
	but1_flag = 1;
}

void init(void){
	// Initialize the board clock
	sysclk_init();
	// Disativa WatchDog Timer
	WDT->WDT_MR = WDT_MR_WDDIS;
}

void init_led_embutido(void){
	// Config do LED
	pmc_enable_periph_clk(LED_PIO_ID);
	pio_set_output(LED_PIO, LED_PIO_IDX_MASK, 0, 0, 0);
}

void init_led1(void){
  //LED 1
  pmc_enable_periph_clk(LED1_PIO_ID);
  pio_set_output(LED1_PIO, LED1_PIO_IDX_MASK, 0, 0, 0);
}

void init_but_embutido(void){
	// Config do Botão
	pmc_enable_periph_clk(BUT_PIO_ID);
	pio_set_input(BUT_PIO, BUT_PIO_IDX_MASK, PIO_DEFAULT);
	pio_pull_up(BUT_PIO, BUT_PIO_IDX_MASK, 1);
	pio_set_debounce_filter(BUT_PIO, BUT_PIO_IDX_MASK, 60);
}

void init_but1(void){
  //BUT1
  pmc_enable_periph_clk(BUT1_PIO_ID);
  pio_set_input(BUT1_PIO, BUT1_PIO_IDX_MASK, PIO_DEFAULT);
  pio_pull_up(BUT1_PIO, BUT1_PIO_IDX_MASK, 1);
  pio_set_debounce_filter(BUT1_PIO, BUT1_PIO_IDX_MASK, 60);
}

void init_IRQ(void){
	pio_handler_set(BUT_PIO,
	BUT_PIO_ID,
	BUT_PIO_IDX_MASK,
	PIO_IT_EDGE,
	but_callback);

	// Config da interrupção do botão embutido
	pio_enable_interrupt(BUT_PIO, BUT_PIO_IDX_MASK);
	pio_get_interrupt_status(BUT_PIO);

	NVIC_EnableIRQ(BUT_PIO_ID);
	NVIC_SetPriority(BUT_PIO_ID, 4);
	
	pio_handler_set(BUT1_PIO,
	BUT1_PIO_ID,
	BUT1_PIO_IDX_MASK,
	PIO_IT_EDGE,
	but1_callback);

	// Config da interrupção do botão 1
	pio_enable_interrupt(BUT1_PIO, BUT1_PIO_IDX_MASK);
	pio_get_interrupt_status(BUT1_PIO);

	NVIC_EnableIRQ(BUT1_PIO_ID);
	NVIC_SetPriority(BUT1_PIO_ID, 4);
}

void pisca_led(int but, int n, int t){
	switch(but){
		case 0:
			for (int i = 0; i < n; i++){
				pio_clear(LED_PIO, LED_PIO_IDX_MASK);
				delay_ms(t);
				pio_set(LED_PIO, LED_PIO_IDX_MASK);
				delay_ms(t);
			}		
		break;

		case 1:
			for (int i = 0; i < n; i++){
				pio_clear(LED1_PIO, LED1_PIO_IDX_MASK);
				delay_ms(t);
				pio_set(LED1_PIO, LED1_PIO_IDX_MASK);
				delay_ms(t);
			}
		break;
		
		// Não faz nada se não passar um but
		default: 0;
		}
}

int main(void)
{
	init();
	init_led_embutido();
	init_led1();
	init_but_embutido();
	init_but1();
	init_IRQ();
	
	while (1){
		if(but_flag){
			pisca_led(0, 5, 100);
			but_flag = 0;
		}
		if(but1_flag){
			pisca_led(1, 5, 100);
			but1_flag = 0;
		}
		// Se estiver fazendo um arquivo novo, não esqueça de importar!
		pmc_sleep(SAM_PM_SMODE_SLEEP_WFI);
	}
	return 0;
}