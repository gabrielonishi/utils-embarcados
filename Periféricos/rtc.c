/* *
 * Arquivo com inicialização do Real Time Controller (RTC)
 *
 * Passos:
 *  1. Definir struct calendar
 *  2. Declarar função init, definindo função (alarme ou a cada segundo)
 *  3. Definir Handler
 *  4. Setar alarme, se nessa função
 */

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// 1.

// Em CONFIG DA PLACA

// Struct p/ RTC
typedef struct  {
  uint32_t year;
  uint32_t month;
  uint32_t day;
  uint32_t week;
  uint32_t hour;
  uint32_t minute;
  uint32_t second;
} calendar;

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// 2.

// Em PROTOTYPES
void RTC_init(Rtc *rtc, uint32_t id_rtc, calendar t, uint32_t irq_type);

// Em INICIALIZAÇÕES

void RTC_init(Rtc *rtc, uint32_t id_rtc, calendar t, uint32_t irq_type) {

	pmc_enable_periph_clk(ID_RTC);

	// Configura tipo de relógio (24hrs)
	rtc_set_hour_mode(rtc, 0);

	// Configura data e hora manualmente
	rtc_set_date(rtc, t.year, t.month, t.day, t.week);
	rtc_set_time(rtc, t.hour, t.minute, t.second);

	// Configure RTC interrupts
	NVIC_DisableIRQ(id_rtc);
	NVIC_ClearPendingIRQ(id_rtc);
	NVIC_SetPriority(id_rtc, 4);
	NVIC_EnableIRQ(id_rtc);

	// Ativa interrupcao via alarme
	rtc_enable_interrupt(rtc,  irq_type);
}

// Na main ou na task

// Configura o tempo inicial
calendar rtc_initial = {2018, 3, 19, 12, 15, 45 ,1}; 

// Configura com IRQ com alarme                                         
RTC_init(RTC, ID_RTC, rtc_initial, RTC_IER_ALREN);

// Configuara com IRQ a cada segundo
// RTC_init(RTC, ID_RTC, rtc_initial, RTC_IER_SECEN);

// Leitura do valor atual do RTC       
uint32_t current_hour, current_min, current_sec;
uint32_t current_year, current_month, current_day, current_week;
rtc_get_time(RTC, &current_hour, &current_min, &current_sec);
rtc_get_date(RTC, &current_year, &current_month, &current_day, &current_week);

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// 3.

// Em CONFIG DA PLACA

volatile char flag_rtc_alarm = 0;

// Em HANDLERS E CALLBACKS

void RTC_Handler(void) {
    uint32_t ul_status = rtc_get_status(RTC);
	
    /* seccond tick */
    if ((ul_status & RTC_SR_SEC) == RTC_SR_SEC) {	
	    // o código para IRQ a cada segundo vem aqui
    }
	
    /* Time or date alarm */
    if ((ul_status & RTC_SR_ALARM) == RTC_SR_ALARM) {
    	// o código para IRQ de alame vem aqui
        flag_rtc_alarm = 1;
    }

    rtc_clear_status(RTC, RTC_SCCR_SECCLR);
    rtc_clear_status(RTC, RTC_SCCR_ALRCLR);
    rtc_clear_status(RTC, RTC_SCCR_ACKCLR);
    rtc_clear_status(RTC, RTC_SCCR_TIMCLR);
    rtc_clear_status(RTC, RTC_SCCR_CALCLR);
    rtc_clear_status(RTC, RTC_SCCR_TDERRCLR);
}

/* --- --- --- --- --- --- --- --- --- --- --- --- */
// 4.

// Na main ou na task
                                                                 
rtc_set_date_alarm(RTC, 1, current_month, 1, current_day);                              
rtc_set_time_alarm(RTC, 1, current_hour, 1, current_min, 1, current_sec + 20);
