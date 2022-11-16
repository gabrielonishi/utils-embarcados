/* *
 * Arquivo com inicialização de rotinas de interrupção (IRQ)
 *
 * Passos sem RTOS:
 * 1 - Declarar variável global
 * 2 - Fazer função de callback
 * 3 - Colocar init
 *
 * README do Corsi: https://github.com/Insper/SAME70-examples/tree/master/Perifericos-uC/PIO-IRQ
 */

// 1.
volatile _Bool but_flag = 0;

// 2.
void but_callback(void) {
    but_flag = 1;
}

// 3.
//  Troque pro que você estiver usando
void init_IRQ(void) {
    pio_handler_set(BUT_PIO,
                    BUT_PIO_ID,
                    BUT_PIO_IDX_MASK,
                    PIO_IT_EDGE,
                    but_callback);

    pio_enable_interrupt(BUT_PIO, BUT_PIO_IDX_MASK);
    pio_get_interrupt_status(BUT_PIO);

    NVIC_EnableIRQ(BUT_PIO_ID);
    NVIC_SetPriority(BUT_PIO_ID, 4);
}

// Se estiver fazendo um arquivo novo, não esqueça de importar!
pmc_sleep(SAM_PM_SMODE_SLEEP_WFI);