// CRIANDO UMA FILA

/* 1. Criar a variável global na sessão RECURSOS RTOS */
QueueHandle_t xQueueButId;

/* 2. Criar o semáforo na main */

// OBS: Trocar pelo tamanho desejado e variável usada
xQueueButId = xQueueCreate(32, sizeof(char));
if (xQueueButId == NULL) {
    printf("Falha em criar xTimerSemaphoree \n");
    } else {
    printf("xTimerSemaphore criado \n");
}
// LENDO E ENVIANDO VALORES DE UMA FILA

// Se for liberado de uma função de callback (ISR)
xQueueSendFromISR(xQueueButId, &id, 0);
// Se for liberado por uma task
xQueueSend(xQueueButId, &id);

/* Para esperar pelo semáforo */
xQueueReceive(xQueueButId, &id, (TickType_t)500);

/* -----------------------CÓDIGO EXEMPLO-----------------------*/
// CÓDIGO EXEMPLO

// fila
QueueHandle_t xQueueButId;

void but1_callback(void) {
    // envia o char `1` na fila
    char id = '1';
    xQueueSendFromISR(xQueueButId, &id, 0);
}

void but2_callback(void) {
    // envia o char `2` na fila
    char id = '2';
    xQueueSendFromISR(xQueueButId, &id, 0);
}

static void task_led(void *pvParameters) {
    init_led1(); // inicializa LED1
    init_but1(); // inicializa botao 1, com callback
    init_but2(); // inicializa botao 2, com callback

    // variável local para leitura do dado da fila
    char id;

    for (;;) {
        // aguarda por até 500 ms pelo se for liberado entra no if
        if (xQueueReceive(xQueueButId, &id, (TickType_t)500)) {

            if (id == '1')
                pin_toggle(LED1_PIO, LED1_PIO_IDX_MASK);
            else if (id == '2')
                pin_toggle(LED2_PIO, LED2_PIO_IDX_MASK);
        }
    }
}

void main(void) {
    // .... //

    // cria fila de 32 slots de char
    xQueueButId = xQueueCreate(32, sizeof(char));

    // verifica se fila foi criada corretamente
    if (xQueueButId == NULL)
        printf("falha em criar a fila \n");
}
