/* *
 * Arquivo com inicialização de LEDs
 *
 * Defines e configs de:
 *  - LED embutido
 *  - LED 1 da placa OLED1
 *  - LED 2 da placa OLED1
 *  - LED 3 da placa OLED1
 *  - Todos juntos
 *  - LED qualquer (chamado pelos parâmetros)
 *
 * OBS: o código só funciona quando na entrada EXT1
 * NÃO ESQUEÇA DE CHAMAR A FUNÇÃO NA MAIN
 */

/* --- --- --- --- --- --- --- --- --- --- --- --- --- */

// LED embutido
#define LED_PIO PIOC
#define LED_PIO_ID ID_PIOC
#define LED_PIO_IDX 8
#define LED_PIO_IDX_MASK (1 << LED_PIO_IDX)

void init_led_embutido(void) {
    // Config do LED
    pmc_enable_periph_clk(LED_PIO_ID);
    pio_set_output(LED_PIO, LED_PIO_IDX_MASK, 0, 0, 0);
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- */

// LED 1
#define LED1_PIO PIOA
#define LED1_PIO_ID ID_PIOA
#define LED1_PIO_IDX 0
#define LED1_PIO_IDX_MASK (1 << LED1_PIO_IDX)

void init_led1(void) {
    // LED 1
    pmc_enable_periph_clk(LED1_PIO_ID);
    pio_set_output(LED1_PIO, LED1_PIO_IDX_MASK, 0, 0, 0);
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- */

// LED 2
#define LED2_PIO PIOC
#define LED2_PIO_ID ID_PIOC
#define LED2_PIO_IDX 30
#define LED2_PIO_IDX_MASK (1 << LED2_PIO_IDX)

void init_led1(void) {
    // LED 2
    pmc_enable_periph_clk(LED2_PIO_ID);
    pio_set_output(LED2_PIO, LED2_PIO_IDX_MASK, 0, 0, 0);
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- */

// LED 3
#define LED3_PIO PIOB
#define LED3_PIO_ID ID_PIOB
#define LED3_PIO_IDX 2
#define LED3_PIO_IDX_MASK (1 << LED3_PIO_IDX)

void init_led1(void) {
    // LED 3
    pmc_enable_periph_clk(LED3_PIO_ID);
    pio_set_output(LED3_PIO, LED3_PIO_IDX_MASK, 0, 0, 0);
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- */

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

void init_leds(void) {
    // Config do LED
    pmc_enable_periph_clk(LED_PIO_ID);
    pio_set_output(LED_PIO, LED_PIO_IDX_MASK, 0, 0, 0);
    // LED 1
    pmc_enable_periph_clk(LED1_PIO_ID);
    pio_set_output(LED1_PIO, LED1_PIO_IDX_MASK, 0, 0, 0);
    // LED 2
    pmc_enable_periph_clk(LED2_PIO_ID);
    pio_set_output(LED2_PIO, LED2_PIO_IDX_MASK, 0, 0, 0);
    // LED 3
    pmc_enable_periph_clk(LED3_PIO_ID);
    pio_set_output(LED3_PIO, LED3_PIO_IDX_MASK, 0, 0, 0);
}

/* --- --- --- --- --- --- --- --- --- --- --- --- --- */

void init_led(Pio *pio, uint32_t id, uint32_t mask) {
    // Config do LED
    pmc_enable_periph_clk(id);
    pio_set_output(pio, mask, 0, 0, 0);
}