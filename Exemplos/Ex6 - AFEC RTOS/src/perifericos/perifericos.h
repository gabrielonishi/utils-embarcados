#ifndef PERIFERICOS_H_
#define PERIFERICOS_H_

#include "asf.h"
// Definindo entradas analógicas
#define AFEC_STRAIN AFEC0
#define AFEC_STRAIN_ID ID_AFEC0
#define AFEC_STRAIN_CHANNEL 0 // Canal do pino PD30

QueueHandle_t xQueueStrain;

void config_AFEC_strain(Afec *afec, uint32_t afec_id, uint32_t afec_channel, afec_callback_t callback);

void AFEC_strain_callback(void);

#endif /* PERIFERICOS_H_ */