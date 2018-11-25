#include "FreeRTOS_AVR.h"
//#include "task.h"
//#include "semphr.h"

/* Cabeçalho do barber e do customer */
static void barber(void *pvParameters);
static void customer(void *pvParameters);

/* Declaração dos semáforos e do mutex */
SemaphoreHandle_t semBarber;
SemaphoreHandle_t semCustomers;
SemaphoreHandle_t mutexLugares;
SemaphoreHandle_t mutexImprime;
/* Declaração da variável de lugares disponíveis */
int lugares = 5;

void setup() {
  Serial.begin(9600);

  /* Criando os semáforos */
  vSemaphoreCreateBinary(semBarber);
  semCustomers = xSemaphoreCreateCounting(5, 0);
  mutexLugares = xSemaphoreCreateMutex();
  mutexImprime = xSemaphoreCreateMutex();

  /* Para usar o random */
  srand(567);

  /* Se está tudo ok com os semáforos, vamos criar as tarefas */
  if (semBarber != NULL && semCustomers != NULL && mutexLugares != NULL
      && mutexImprime != NULL) {
    xTaskCreate(barber, "Barber", 200, NULL, 1, NULL);
    xTaskCreate(customer, "Customer", 200, NULL, 1, NULL);
    vTaskStartScheduler();
  }
  for( ; ; );
  return 0;
}

static void barber(void *pvParameters) {
  while (true) {
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
  vTaskDelay((rand() % 3000) / portTICK_PERIOD_MS);
}

static void customer(void *pvParameters) {
  while (true) {
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
      } else {
        xSemaphoreTake(mutexImprime, portMAX_DELAY);
        Serial.println("Chegou um cliente, mas a sala de espera esta cheia.");
        Serial.flush();
        vTaskDelay(2000 / portTICK_PERIOD_MS);
      xSemaphoreGive(mutexImprime);
        xSemaphoreGive(mutexLugares);
      }
    vTaskDelay((rand() % 3000) / portTICK_PERIOD_MS);
  }
}


void loop() { }
