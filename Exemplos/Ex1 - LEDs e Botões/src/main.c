/* *
 * Arquivo baseado no Lab. 1 de Embarcados
 * 
 * Acende e apaga os LEDs da placa de acordo com os
 * botões apertados
 * 
 * Trechos de código retirados do Nick (Nicolas Queiroga)
 * com a permissão dele
 */

#include "asf.h"

// Config dos LEDs
#define LED_PIO PIOC
#define LED_PIO_ID ID_PIOC
#define LED_PIO_IDX 8
#define LED_PIO_IDX_MASK (1 << LED_PIO_IDX)
// LED 1
#define LED1_PIO PIOA
#define LED1_PIO_ID ID_PIOA
#define LED1_PIO_IDX 0
#define LED1_PIO_IDX_MASK (1 << LED1_PIO_IDX)
// LED 2
#define LED2_PIO PIOC
#define LED2_PIO_ID ID_PIOC
#define LED2_PIO_IDX 30
#define LED2_PIO_IDX_MASK (1 << LED2_PIO_IDX)
// LED 3
#define LED3_PIO PIOB
#define LED3_PIO_ID ID_PIOB
#define LED3_PIO_IDX 2
#define LED3_PIO_IDX_MASK (1 << LED3_PIO_IDX)
// LED Fora
#define LED4_PIO PIOD
#define LED4_PIO_ID ID_PIOD
#define LED4_PIO_IDX 30
#define LED4_PIO_IDX_MASK (1 << LED4_PIO_IDX)

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
// Config do BUT2
#define BUT2_PIO PIOC
#define BUT2_PIO_ID ID_PIOC
#define BUT2_PIO_IDX 31
#define BUT2_PIO_IDX_MASK (1u << BUT2_PIO_IDX)
// Config do BUT3
#define BUT3_PIO PIOA
#define BUT3_PIO_ID ID_PIOA
#define BUT3_PIO_IDX 19
#define BUT3_PIO_IDX_MASK (1u << BUT3_PIO_IDX)

void init(void);
void init_leds(void);
void init_buts(void);

void init(void){
	// Initialize the board clock
	sysclk_init();
	// Disativa WatchDog Timer
	WDT->WDT_MR = WDT_MR_WDDIS;
}

void init_leds(void){
	// Config do LED
	pmc_enable_periph_clk(LED_PIO_ID);
	pio_set_output(LED_PIO, LED_PIO_IDX_MASK, 0, 0, 0);
	//LED 1
	pmc_enable_periph_clk(LED1_PIO_ID);
	pio_set_output(LED1_PIO, LED1_PIO_IDX_MASK, 0, 0, 0);
	//LED 2
	pmc_enable_periph_clk(LED2_PIO_ID);
	pio_set_output(LED2_PIO, LED2_PIO_IDX_MASK, 0, 0, 0);
	//LED 3
	pmc_enable_periph_clk(LED3_PIO_ID);
	pio_set_output(LED3_PIO, LED3_PIO_IDX_MASK, 0, 0, 0);
	//LED 3
	pmc_enable_periph_clk(LED4_PIO_ID);
	pio_set_output(LED4_PIO, LED4_PIO_IDX_MASK, 0, 0, 0);
}

void init_buts(void){
	// Config do Botão
	pmc_enable_periph_clk(BUT_PIO_ID);
	pio_set_input(BUT_PIO, BUT_PIO_IDX_MASK, PIO_DEFAULT);
	pio_pull_up(BUT_PIO, BUT_PIO_IDX_MASK, 1);
	pio_set_debounce_filter(BUT_PIO, BUT_PIO_IDX_MASK, 60);
	//BUT1
	pmc_enable_periph_clk(BUT1_PIO_ID);
	pio_set_input(BUT1_PIO, BUT1_PIO_IDX_MASK, PIO_DEFAULT);
	pio_pull_up(BUT1_PIO, BUT1_PIO_IDX_MASK, 1);
	pio_set_debounce_filter(BUT1_PIO, BUT1_PIO_IDX_MASK, 60);
	//BUT2
	pmc_enable_periph_clk(BUT2_PIO_ID);
	pio_set_input(BUT2_PIO, BUT2_PIO_IDX_MASK, PIO_DEFAULT);
	pio_pull_up(BUT2_PIO, BUT2_PIO_IDX_MASK, 1);
	pio_set_debounce_filter(BUT2_PIO, BUT2_PIO_IDX_MASK, 60);
	//BUT3
	pmc_enable_periph_clk(BUT3_PIO_ID);
	pio_set_input(BUT3_PIO, BUT3_PIO_IDX_MASK, PIO_DEFAULT);
	pio_pull_up(BUT3_PIO, BUT3_PIO_IDX_MASK, 1);
	pio_set_debounce_filter(BUT3_PIO, BUT3_PIO_IDX_MASK, 60);
}

int main(void)
{
	init();
	init_leds();
	init_buts();
	
	while (1){
		_Bool btn = !pio_get(BUT_PIO, PIO_INPUT, BUT_PIO_IDX_MASK);
		_Bool btn1 = !pio_get(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK);
		_Bool btn2 = !pio_get(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK);
		_Bool btn3 = !pio_get(BUT3_PIO, PIO_INPUT, BUT3_PIO_IDX_MASK);
		
		if(btn){
			pio_clear(LED_PIO, LED_PIO_IDX_MASK);
			pio_set(LED4_PIO, LED4_PIO_IDX_MASK);
		} else if(btn1){
			pio_clear(LED1_PIO, LED1_PIO_IDX_MASK);
		} else if(btn2){
			pio_clear(LED2_PIO, LED2_PIO_IDX_MASK);
		} else if(btn3){
			pio_clear(LED3_PIO, LED3_PIO_IDX_MASK);
		} else{
			pio_set(LED_PIO, LED_PIO_IDX_MASK);
			pio_set(LED1_PIO, LED1_PIO_IDX_MASK);
			pio_set(LED2_PIO, LED2_PIO_IDX_MASK);
			pio_set(LED3_PIO, LED3_PIO_IDX_MASK);
			pio_clear(LED4_PIO, LED4_PIO_IDX_MASK);
		}
	}
	return 0;
}