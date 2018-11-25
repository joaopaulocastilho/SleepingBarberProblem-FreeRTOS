#include "FreeRTOS_AVR.h"
//#include "task.h"
//#include "semphr.h"

#define pos1 (1UL << 0UL)
#define pos2 (1UL << 1UL)
#define pos3 (1UL << 2UL)
#define pos4 (1UL << 3UL)
#define pos5 (1UL << 4UL)

/* Cabeçalho do barber e do customer */
static void barber(void *pvParameters);
static void customer(void *pvParameters);

/* Declaração da variável de lugares disponíveis */
int lugares = 5;

EventGroupHandle_t xEventGroup[5];
EventGroupHandle_t controle;

void setup() {
  Serial.begin(9600);

  /* Para usar o random */
  srand(567);
  for (int i = 0; i < lugares; i++) 
    xEventGroup[i] = xEventGroupCreate();
  controle = xEventGroupCreate();
  
  xTaskCreate(barber, "Barber", 200, NULL, 1, NULL);
  xTaskCreate(customer, "Customer", 200, NULL, 1, NULL);
  vTaskStartScheduler();
  for( ; ; );
  return 0;
}

static void barber(void *pvParameters) {
  while (true) {
    EventBits_t tmp;
    const EventBits_t espera = (pos1);
    tmp = xEventGroupWaitBits(controle, espera, pdTRUE, pdFALSE, portMAX_DELAY);

    Serial.println("Oi");
    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
  /*while (true) {
    xSemaphoreTake(semCustomers, portMAX_DELAY);
    
    xSemaphoreTake(mutexLugares, portMAX_DELAY);
    xSemaphoreTake(mutexImprime, portMAX_DELAY);
      Serial.print("Barbeiro pegou cliente, total na fila: ");
      Serial.println(5-(lugares+1));
      Serial.flush();
      vTaskDelay(3000 / portTICK_PERIOD_MS);
    xSemaphoreGive(mutexImprime);
    
      lugares++;
      
      xSemaphoreTake(semBarber, portMAX_DELAY);
      
    xSemaphoreGive(mutexLugares);
    xSemaphoreTake(mutexImprime, portMAX_DELAY);
        Serial.println("Cortando Cabelo");
        Serial.flush();
        vTaskDelay(3000 / portTICK_PERIOD_MS);
      xSemaphoreGive(mutexImprime);
      vTaskDelay((rand() % 3000) / portTICK_PERIOD_MS);
      xSemaphoreGive(semBarber);
      xSemaphoreTake(mutexImprime, portMAX_DELAY);
        Serial.println("Cliente que estava cortando foi embora");
        Serial.flush();
        vTaskDelay(3000 / portTICK_PERIOD_MS);
      xSemaphoreGive(mutexImprime);
  }
  vTaskDelay((rand() % 3000) / portTICK_PERIOD_MS);*/
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
      Serial.print("1 --> ");
      Serial.println(pos);
      vTaskDelay(3000 / portTICK_PERIOD_MS);  
    } else {
      Serial.println("2");
      vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
  }
  /*while (true) {
    xSemaphoreTake(mutexLugares, portMAX_DELAY);
      if (lugares > 0) {
        lugares--;

          xSemaphoreTake(mutexImprime, portMAX_DELAY);
            Serial.print("Chegou um novo cliente, total na fila: ");
            Serial.println(5-lugares);
            Serial.flush();
            vTaskDelay(3000 / portTICK_PERIOD_MS);
          xSemaphoreGive(mutexImprime);
        
         xSemaphoreGive(semCustomers);
         xSemaphoreGive(mutexLugares);
      } else xSemaphoreGive(mutexLugares);
    //xSemaphoreGive(mutexLugares);
    vTaskDelay((rand() % 3000) / portTICK_PERIOD_MS);
  }*/
}


void loop() { }
