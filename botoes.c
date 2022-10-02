/* *
 * Arquivo com inicialização de LEDs
 * 
 * Defines e configs de:
 *  - Botão embutido
 *  - Botão 1 da placa OLED1
 *  - Botão 2 da placa OLED1
 *  - Botão 3 da placa OLED1
 *  - Todos juntos
 *
 * OBS: o código só funciona quando na entrada EXT1
 * NÃO ESQUEÇA DE CHAMAR A FUNÇÃO NA MAIN 
 */

/* --- --- --- --- --- --- --- --- --- --- --- --- --- */

// Config do botão embutido
#define BUT_PIO PIOA
#define BUT_PIO_ID ID_PIOA
#define BUT_PIO_IDX 11
#define BUT_PIO_IDX_MASK (1u << BUT_PIO_IDX)

void init_but_embutido(void){
  // Config do Botão
  pmc_enable_periph_clk(BUT_PIO_ID);
  pio_set_input(BUT_PIO, BUT_PIO_IDX_MASK, PIO_DEFAULT);
  pio_pull_up(BUT_PIO, BUT_PIO_IDX_MASK, 1);
  pio_set_debounce_filter(BUT_PIO, BUT_PIO_IDX_MASK, 60);

}
/* --- --- --- --- --- --- --- --- --- --- --- --- --- */

// Config do BUT1
#define BUT1_PIO PIOD
#define BUT1_PIO_ID ID_PIOD
#define BUT1_PIO_IDX 28
#define BUT1_PIO_IDX_MASK (1u << BUT1_PIO_IDX)

void init_but1(void){
  //BUT1
  pmc_enable_periph_clk(BUT1_PIO_ID);
  pio_set_input(BUT1_PIO, BUT1_PIO_IDX_MASK, PIO_DEFAULT);
  pio_pull_up(BUT1_PIO, BUT1_PIO_IDX_MASK, 1);
  pio_set_debounce_filter(BUT1_PIO, BUT1_PIO_IDX_MASK, 60);
}
/* --- --- --- --- --- --- --- --- --- --- --- --- --- */

// Config do BUT2
#define BUT2_PIO PIOC
#define BUT2_PIO_ID ID_PIOC
#define BUT2_PIO_IDX 31
#define BUT2_PIO_IDX_MASK (1u << BUT2_PIO_IDX)

void init_but2(void){
  //BUT2
  pmc_enable_periph_clk(BUT2_PIO_ID);
  pio_set_input(BUT2_PIO, BUT2_PIO_IDX_MASK, PIO_DEFAULT);
  pio_pull_up(BUT2_PIO, BUT2_PIO_IDX_MASK, 1);
  pio_set_debounce_filter(BUT2_PIO, BUT2_PIO_IDX_MASK, 60);
}
/* --- --- --- --- --- --- --- --- --- --- --- --- --- */

// Config do BUT3
#define BUT3_PIO PIOA
#define BUT3_PIO_ID ID_PIOA
#define BUT3_PIO_IDX 19
#define BUT3_PIO_IDX_MASK (1u << BUT3_PIO_IDX)

void init_but3(void){
  //BUT3
  pmc_enable_periph_clk(BUT3_PIO_ID);
  pio_set_input(BUT3_PIO, BUT3_PIO_IDX_MASK, PIO_DEFAULT);
  pio_pull_up(BUT3_PIO, BUT3_PIO_IDX_MASK, 1);
  pio_set_debounce_filter(BUT3_PIO, BUT3_PIO_IDX_MASK, 60);
}
/* --- --- --- --- --- --- --- --- --- --- --- --- --- */

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