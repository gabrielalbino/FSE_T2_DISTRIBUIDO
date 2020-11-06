#include <stdio.h>
#include <pthread.h>
#include <bcm2835.h>
#include <sched.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <signal.h>
#include "bme280_controller.h"
#include "log_controller.h"
#include "tcp_controller.h"
#include "gpio_controller.h"
#include "tcp_client.h"
#include "types.h"

int lockerSensor = 0;
pthread_t ptSensor, ptLog, ptTCP, ptGPIO, ptAtualizadorCentral;
volatile dadosServidor dados;

void mostraMenu();
void encerrar();

void *threadSensor(void *args);
void *threadAtualizadorCentral(void *args);
void *threadLog(void *args);
void *threadTCP(void *args);
void *threadGPIO(void *args);

int main(){
  /* Iniciando data */
  dados.estadoAr = 0;
  dados.temperaturaAr = 0.f;
  dados.estadoLampadas[0] = 0;
  dados.estadoLampadas[1] = 0;
  dados.estadoLampadas[2] = 0;
  dados.estadoLampadas[3] = 0;
  dados.estadoSensoresAbertura[0] = 0;
  dados.estadoSensoresAbertura[1] = 0;
  dados.estadoSensoresAbertura[2] = 0;
  dados.estadoSensoresAbertura[3] = 0;
  dados.estadoSensoresAbertura[4] = 0;
  dados.estadoSensoresAbertura[5] = 0;
  dados.temperatura = 0.f;
  dados.umidade = 0.f;
  dados.requestFlag = 0;

  /* Setando sinais*/
  signal(SIGINT, encerrar);

  /* Inicializando dispositivos */
  initCSV();
  BME280_setup();
  tcp_setup();
  if (!bcm2835_init())
    return 1;
  gpio_setup();


  /* Criando threads */
  pthread_create(&ptSensor, NULL, &threadSensor, NULL);
  pthread_create(&ptLog, NULL, &threadLog, NULL);
  pthread_create(&ptTCP, NULL, &threadTCP, NULL);
  pthread_create(&ptGPIO, NULL, &threadGPIO, NULL);
  pthread_create(&ptAtualizadorCentral, NULL, &threadAtualizadorCentral, NULL);

  while(1){
    mostraMenu();
    sleep(1);
  }
  return 0;
};

void mostraMenu(){
  struct tm *data_hora_atual;
  time_t segundos;
  time(&segundos);   
  data_hora_atual = localtime(&segundos);
  system("clear");
  printf("---------------- SERVIDOR DISTRIBUIDO ----");
  printf("%2d:", data_hora_atual->tm_hour); //hora
  printf("%2d:",data_hora_atual->tm_min);//minuto
  printf("%2d----\n",data_hora_atual->tm_sec);//segundo  printf("--\n");
  printf("Temperatura atual: %4.2f                             |\nUmidade atual: %4.2f                                 |\n", dados.temperatura, dados.umidade);
  printf("Ar : %s                                       |\n", dados.estadoAr == 1 ? "ligado   " : "desligado");
  printf("Temperatura do Ar : %.2f                             |\n", dados.temperaturaAr);
  for (int i = 0; i < 4; i++){
    printf("Lampada %d: %s                                 |\n", i + 1, dados.estadoLampadas[i] == 1 ? "ligado   " : "desligado");
  }
  for (int i = 0; i < 6; i++){
    printf("Sensor Abertura %d: %s                         |\n", i + 1, dados.estadoSensoresAbertura[i] == 1 ? "ligado   " : "desligado");
  }
  for (int i = 0; i < 2; i++){
    printf("Sensor Presença %d: %s                         |\n", i + 1, dados.estadoSensoresPresenca[i] == 1 ? "ligado   " : "desligado");
  }

  printf("------------------------------------------------------\n");
}

void *threadSensor(void *args){
  while(1){
    BME280_updateTemperature(&dados);
    sleep(1);
  }
  return NULL;
}

void *threadLog(void *args){
  while(1){
    saveInFile((void *)&dados);
    sleep(1);
  }
  return NULL;
}

void *threadAtualizadorCentral(void *args){
  requisicao req;

  while(1){
    req.command = 4; //UPDATE
    req.dadosServidor = dados;
    client_send(req);
    sleep(1);
  }
  return NULL;
}

void *threadTCP(void *args){
  tcp_acceptConnection(&dados);
  return NULL;
}

void* threadGPIO(void* args){
  requisicao req;
  while(1){
    gpio_update(&dados);
    if(dados.requestFlag){
      req.command = dados.requestFlag;
      req.payload[0] = dados.payload;
      client_send(req);
      dados.requestFlag = 0;
    }
    usleep(300000);
  }

  return NULL;
}

void encerrar(){
  tcp_shutdown();
  BME280_shutdown();    
  gpio_shutdown();
  exit(0);
}
