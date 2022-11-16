/* *
 * Arquivo com inicialização do Real Time Timer (RTT)
 *
 * Passos:
 *  1. Inicializar RTT
 *  2. Definir Handler
 * 
 * Funções:
 *  - uint32_t rtt_read_timer_value(Rtt *p_rtt): Retorna o valor atual do contador do RTT
 *  - void rtt_disable(Rtt *p_rtt): Desativa o RTT
 * 
 * README do Corsi: https://github.com/Insper/SAME70-examples/tree/master/Perifericos-uC/RTT-IRQ
 */

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// 1.

// Em PROTOTYPES
static void RTT_init(float freqPrescale, uint32_t IrqNPulses, uint32_t rttIRQSource);

/** 
 * Configura RTT
 *
 * arg0 pllPreScale  : Frequência na qual o contador irá incrementar
 * arg1 IrqNPulses   : Valor do alarme 
 * arg2 rttIRQSource : Pode ser uma 
 *     - 0: 
 *     - RTT_MR_RTTINCIEN: Interrupção por incremento (pllPreScale)
 *     - RTT_MR_ALMIEN : Interrupção por alarme
 */

// Em INICIALIZAÇÕES

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

// Na main ou na task
RTT_init(4, 16, RTT_MR_ALMIEN);

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// 2. 

// Em DEFINES

// Flag do alarme
volatile char flag_rtt = 0;

// Em HANDLERS E CALLBACKS
 
void RTT_Handler(void) {
  uint32_t ul_status;
  ul_status = rtt_get_status(RTT);

  /* IRQ due to Alarm */
  if ((ul_status & RTT_SR_ALMS) == RTT_SR_ALMS) {
      flag_rtt = 1;
   }  
}

