#include "FreeRTOS_AVR.h"
//#include "task.h"
//#include "semphr.h"

#define pos1 (1UL << 0UL)

/* Cabeçalho do barber e do customer */
static void barber(void *pvParameters);
static void customer(void *pvParameters);

/* Declaração da variável de lugares disponíveis */
int lugares = 5;

/* Declaração dos grupos de eventos */
EventGroupHandle_t xEventGroup[5];
EventGroupHandle_t controle;

/* Mutex para controlar impressões na tela */
SemaphoreHandle_t mutexImprime;

void setup() {
  Serial.begin(9600);

  /* Para usar o random */
  srand(567);

  /* Inicializa os grupos de eventos */
  for (int i = 0; i < lugares; i++) 
    xEventGroup[i] = xEventGroupCreate();
  controle = xEventGroupCreate();

  /* Inicializa o mutex para controlar impressões na tela */
  mutexImprime = xSemaphoreCreateMutex();
  
  xTaskCreate(barber, "Barber", 200, NULL, 1, NULL);
  xTaskCreate(customer, "Customer", 200, NULL, 1, NULL);
  vTaskStartScheduler();
  for( ; ; );
  return 0;
}

int totalFila() {
  int ret = 0;
  EventBits_t tmp;
  const EventBits_t espera = (pos1);
  for (int i = 0; i < lugares; i++) {
    tmp = xEventGroupWaitBits(xEventGroup[i], espera, pdFALSE, pdFALSE, 0);
    if ((tmp & pos1) != 0) ret++;
  }
  return ret;
}

static void barber(void *pvParameters) {
  while (true) {
    int corte, flag = 0;
    EventBits_t tmp;
    const EventBits_t espera = (pos1);
    tmp = xEventGroupWaitBits(controle, espera, pdTRUE, pdFALSE, portMAX_DELAY);
    for (int i = 0; i < lugares; i++) {
      if (flag == 0) tmp = xEventGroupWaitBits(xEventGroup[i], espera, pdTRUE, pdFALSE, 0);
      else tmp = xEventGroupWaitBits(xEventGroup[i], espera, pdFALSE, pdFALSE, 0);
      if ((tmp & pos1) != 0) {
        if (flag == 0) { 
          xSemaphoreTake(mutexImprime, portMAX_DELAY);
            Serial.print("Barbeiro pegou cliente, total na fila: ");
            Serial.println(totalFila());
            Serial.flush();
            vTaskDelay(2000 / portTICK_PERIOD_MS);
          xSemaphoreGive(mutexImprime);
          flag = 1;
          corte = i + 1;
        }
        else xEventGroupSetBits(controle, pos1);
      }
    }
    xSemaphoreTake(mutexImprime, portMAX_DELAY);
      Serial.println("Cortanto o cabelo do cliente");
      Serial.flush();
      vTaskDelay(2000 / portTICK_PERIOD_MS);
    xSemaphoreGive(mutexImprime);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    xSemaphoreTake(mutexImprime, portMAX_DELAY);
      Serial.println("Cliente que estava cortando foi embora");
      Serial.flush();
    xSemaphoreGive(mutexImprime);
    vTaskDelay((rand() % 3000) / portTICK_PERIOD_MS);
  }
}

int colocaEspera() {
  EventBits_t tmp;
  const EventBits_t espera = (pos1);
  for (int i = 0; i < lugares; i++) {
    tmp = xEventGroupWaitBits(xEventGroup[i], espera, pdFALSE, pdFALSE, 0);
    if ((tmp & pos1) == 0) {
      xEventGroupSetBits(xEventGroup[i], pos1);
      xEventGroupSetBits(controle, pos1);
      return (i + 1);
    }
  }
  return -1;
}

static void customer(void *pvParameters) {
  while (true) {
    int pos;
    if ((pos = colocaEspera()) != -1) {
      xSemaphoreTake(mutexImprime, portMAX_DELAY);
        Serial.print("Chegou um novo cliente na sala de espera, total esperando: ");
        Serial.println(totalFila());
        Serial.flush();
        vTaskDelay(2000 / portTICK_PERIOD_MS);
      xSemaphoreGive(mutexImprime);
    } else {
      xSemaphoreTake(mutexImprime, portMAX_DELAY);
        Serial.println("Chegou um cliente, mas a sala de espera esta cheia.");
        Serial.flush();
        vTaskDelay(2000 / portTICK_PERIOD_MS);
      xSemaphoreGive(mutexImprime);
    }
    vTaskDelay((rand() % 3000) / portTICK_PERIOD_MS);
  }
}


void loop() { }
