// Criando um semáforo:

/* 1. Criar a variável global na sessão RECURSOS RTOS */
SemaphoreHandle_t xSemaphore;

/* 2. Criar o semáforo na main */
xSemaphoreCreateBinary();

/* Para liberar um semáforo */

// Se for liberado de uma função de callback (ISR)
xSemaphoreGiveFromISR(xSemaphore, 0);
// Se for liberado por uma task
xSemaphoreGive(xSemaphore);

/* Para esperar pelo semáforo */
if (xSemaphoreTake(xSemaphore, 500 / portTICK_PERIOD_MS) == pdTRUE)

    /* -----------------------CÓDIGO EXEMPLO-----------------------*/
    // CÓDIGO EXEMPLO

    SemaphoreHandle_t xSemaphore;

void but_callback(void) {
    // libera semáforo
    xSemaphoreGiveFromISR(xSemaphore, 0);
}

static void task_led(void *pvParameters) {
    init_led1(); // inicializa LED1
    init_but1(); // inicializa botao 1, com callback

    for (;;) {
        // aguarda por até 500 ms pelo se for liberado entra no if
        if (xSemaphoreTake(xSemaphore, 500 / portTICK_PERIOD_MS) == pdTRUE) {
            LED_Toggle(LED0);
        }
    }
}

void main(void) {
    // .... //

    // cria semáforo binário
    xSemaphore = xSemaphoreCreateBinary();

    // verifica se semáforo foi criado corretamente
    if (xSemaphore == NULL)
        printf("falha em criar o semaforo \n");
