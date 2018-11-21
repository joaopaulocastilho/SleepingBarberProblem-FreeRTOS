#include "FreeRTOS_AVR.h"
//#include "task.h"
//#include "semphr.h"

/* Cabeçalho do barber e do customer */
static void barber(void *pvParameters);
static void custumer(void *pvParameters);

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
    Serial.print("Deu certo!");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
