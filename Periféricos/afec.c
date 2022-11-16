/* *
 * Arquivo com instruções de uso de AFECs (leitura analógica)
 * Utilizado p/ potenciômetros entre outros
 * Passos:
 *  - 1: Declarar entradas
 *  - 2: Configurar função de init
 *  - 3: Delimitar timer de leitura
 *
 * README do Corsi: https://github.com/Insper/SAME70-examples/tree/master/Perifericos-uC/RTOS-IRQ-ADC
 * bom pq mostra outras entradas p/ AFECs
 */

// OBS: Esse exemplo usa a config de um potenciômetro
// Mude os nomes de variáveis de acordo

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// 1. 
// Em DEFINES

// Definindo entradas analógicas
#define AFEC_POT AFEC0
#define AFEC_POT_ID ID_AFEC0
#define AFEC_POT_CHANNEL 0 // Canal do pino PD30

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// 2. 
// Em PROTOTYPES
void config_AFEC_pot(Afec *afec, uint32_t afec_id, uint32_t afec_channel, afec_callback_t callback);

// Em INITS
void config_AFEC_pot(Afec *afec, uint32_t afec_id, uint32_t afec_channel, afec_callback_t callback) {

  /* Ativa AFEC - 0 */
  afec_enable(afec);

  /* struct de configuracao do AFEC */
  struct afec_config afec_cfg;

  /* Carrega parametros padrao */
  afec_get_config_defaults(&afec_cfg);

  /* Configura AFEC */
  afec_init(afec, &afec_cfg);

  /* Configura trigger por software */
  afec_set_trigger(afec, AFEC_TRIG_SW);

  /*** Configuracao específica do canal AFEC ***/
  struct afec_ch_config afec_ch_cfg;
  afec_ch_get_config_defaults(&afec_ch_cfg);
  afec_ch_cfg.gain = AFEC_GAINVALUE_0;
  afec_ch_set_config(afec, afec_channel, &afec_ch_cfg);

  /*
  * Calibracao:
  * Because the internal ADC offset is 0x200, it should cancel it and shift
  down to 0.
  */
  afec_channel_set_analog_offset(afec, afec_channel, 0x200);

  /***  Configura sensor de temperatura ***/
  struct afec_temp_sensor_config afec_temp_sensor_cfg;

  afec_temp_sensor_get_config_defaults(&afec_temp_sensor_cfg);
  afec_temp_sensor_set_config(afec, &afec_temp_sensor_cfg);

  /* configura IRQ */
  afec_set_callback(afec, afec_channel, callback, 1);
  NVIC_SetPriority(afec_id, 4);
  NVIC_EnableIRQ(afec_id);
}

// Na task, fora do while (só precisa ser feito uma vez)
config_AFEC_pot(AFEC_POT, AFEC_POT_ID, AFEC_POT_CHANNEL, AFEC_pot_Callback);
//OBS: AFEC_pot_Callback é o nome da função de Callback do potenciômetro

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// 3.
// Na task