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
/* Declaração da variável de lugares disponíveis */
int lugares = 5;

void setup() {
  Serial.begin(9600);

  /* Criando os semáforos */
  vSemaphoreCreateBinary(semBarber);
  semCustomers = xSemaphoreCreateCounting(5, 0);
  mutexLugares = xSemaphoreCreateMutex();

  /* Se está tudo ok com os semáforos, vamos criar as tarefas */
  if (semBarber != NULL && semCustomers != NULL && mutexLugares != NULL) {
    xTaskCreate(barber, "Barber", 200, NULL, 1, NULL);
    xTaskCreate(customer, "Customer", 200, NULL, 1, NULL);
  }
}

static void barber(void *pvParameters) {
  
}

static void customer(void *pvParameters) {
  
}


void loop() { }
