#include "gpio_controller.h"

void gpio_setup(){
  // Define entradas
  bcm2835_gpio_fsel(IN_PRESENCA1, BCM2835_GPIO_FSEL_INPT); //Sensor de Presença 01 (Sala)
  bcm2835_gpio_fsel(IN_PRESENCA2, BCM2835_GPIO_FSEL_INPT); //Sensor de Presença 02 (Cozinha)
  bcm2835_gpio_fsel(IN_ABERTURA1, BCM2835_GPIO_FSEL_INPT); // Sensor Abertura 01 
  bcm2835_gpio_fsel(IN_ABERTURA2, BCM2835_GPIO_FSEL_INPT); // Sensor Abertura 02
  bcm2835_gpio_fsel(IN_ABERTURA3, BCM2835_GPIO_FSEL_INPT); // Sensor Abertura 03 
  bcm2835_gpio_fsel(IN_ABERTURA4, BCM2835_GPIO_FSEL_INPT); // Sensor Abertura 04 
  bcm2835_gpio_fsel(IN_ABERTURA5, BCM2835_GPIO_FSEL_INPT); // Sensor Abertura 05 
  bcm2835_gpio_fsel(IN_ABERTURA6, BCM2835_GPIO_FSEL_INPT); // Sensor Abertura 06 
  // define saidas
  bcm2835_gpio_fsel(OUT_LAMPADA1, BCM2835_GPIO_FSEL_OUTP); // Lâmpada 1
  bcm2835_gpio_fsel(OUT_LAMPADA2, BCM2835_GPIO_FSEL_OUTP); // Lâmpada 2
  bcm2835_gpio_fsel(OUT_LAMPADA3, BCM2835_GPIO_FSEL_OUTP); // Lâmpada 3
  bcm2835_gpio_fsel(OUT_LAMPADA4, BCM2835_GPIO_FSEL_OUTP); // Lâmpada 4
  bcm2835_gpio_fsel(OUT_AR1, BCM2835_GPIO_FSEL_OUTP); // ar 1
  bcm2835_gpio_fsel(OUT_AR2, BCM2835_GPIO_FSEL_OUTP); // ar 2
}

void gpio_shutdown(){
  bcm2835_gpio_clr(OUT_LAMPADA1);
  bcm2835_gpio_clr(OUT_LAMPADA2);
  bcm2835_gpio_clr(OUT_LAMPADA3);
  bcm2835_gpio_clr(OUT_LAMPADA4);
  bcm2835_gpio_write(OUT_AR1, LOW);
  bcm2835_gpio_write(OUT_AR2, LOW);
  bcm2835_close();
}

void gpio_update(volatile dadosServidor* dados){
  int temp;
  //setando dispositivos
  dados->estadoLampadas[0] == 0 ? bcm2835_gpio_clr(OUT_LAMPADA1) : bcm2835_gpio_set(OUT_LAMPADA1);
  dados->estadoLampadas[1] == 0 ? bcm2835_gpio_clr(OUT_LAMPADA2) : bcm2835_gpio_set(OUT_LAMPADA2);
  dados->estadoLampadas[2] == 0 ? bcm2835_gpio_clr(OUT_LAMPADA3) : bcm2835_gpio_set(OUT_LAMPADA3);
  dados->estadoLampadas[3] == 0 ? bcm2835_gpio_clr(OUT_LAMPADA4) : bcm2835_gpio_set(OUT_LAMPADA4);
  
  if(dados->temperatura > dados->temperaturaAr && dados->estadoAr == 1){
    bcm2835_gpio_write(OUT_AR1, HIGH);
    bcm2835_gpio_write(OUT_AR2, HIGH);
  }
  else{
    bcm2835_gpio_write(OUT_AR1, LOW);
    bcm2835_gpio_write(OUT_AR2, LOW);
  }
  //recueperando infos
  temp = bcm2835_gpio_lev(IN_PRESENCA1);
  if(dados->estadoSensoresPresenca[0] != temp){
    dados->estadoSensoresPresenca[0] = temp;
    dados->requestFlag = 2; // acender luz 01
    dados->payload = dados->estadoSensoresPresenca[0];
  }
  temp = bcm2835_gpio_lev(IN_PRESENCA2);
  if(!dados->requestFlag && dados->estadoSensoresPresenca[1] != temp){
    dados->estadoSensoresPresenca[1] = temp;
    dados->requestFlag = 3; // acender luz 02
    dados->payload = dados->estadoSensoresPresenca[1];
  } 
  temp = bcm2835_gpio_lev(IN_ABERTURA1);
  if(!dados->requestFlag && dados->estadoSensoresAbertura[0] != temp){
    dados->estadoSensoresAbertura[0] = temp;
    dados->requestFlag = 1; // alarme
    dados->payload = dados->estadoSensoresAbertura[0];
  } 
  temp = bcm2835_gpio_lev(IN_ABERTURA2);
  if(!dados->requestFlag && dados->estadoSensoresAbertura[1] != temp){
    dados->estadoSensoresAbertura[1] = temp;
    dados->requestFlag = 1; // alarme
    dados->payload = dados->estadoSensoresAbertura[0];
  } 
  temp = bcm2835_gpio_lev(IN_ABERTURA3);
  if(!dados->requestFlag && dados->estadoSensoresAbertura[2] != temp){
    dados->estadoSensoresAbertura[2] = temp;
    dados->requestFlag = 1; // alarme
    dados->payload = dados->estadoSensoresAbertura[0];
  } 
  temp = bcm2835_gpio_lev(IN_ABERTURA4);
  if(!dados->requestFlag && dados->estadoSensoresAbertura[3] != temp){
    dados->estadoSensoresAbertura[3] = temp;
    dados->requestFlag = 1; // alarme
    dados->payload = dados->estadoSensoresAbertura[0];
  } 
  temp = bcm2835_gpio_lev(IN_ABERTURA5);
  if(!dados->requestFlag && dados->estadoSensoresAbertura[4] != temp){
    dados->estadoSensoresAbertura[4] = temp;
    dados->requestFlag = 1; // alarme
    dados->payload = dados->estadoSensoresAbertura[0];
  } 
  temp = bcm2835_gpio_lev(IN_ABERTURA6);
  if(!dados->requestFlag && dados->estadoSensoresAbertura[5] != temp){
    dados->estadoSensoresAbertura[5] = temp;
    dados->requestFlag = 1; // alarme
    dados->payload = dados->estadoSensoresAbertura[0];
  } 

}