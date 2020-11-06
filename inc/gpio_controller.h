#ifndef __GPIO__
#define __GPIO__

#define IN_PRESENCA1 RPI_V2_GPIO_P1_22
#define IN_PRESENCA2 RPI_V2_GPIO_P1_37
#define IN_ABERTURA1 RPI_V2_GPIO_P1_29
#define IN_ABERTURA2 RPI_V2_GPIO_P1_31
#define IN_ABERTURA3 RPI_V2_GPIO_P1_32
#define IN_ABERTURA4 RPI_V2_GPIO_P1_36
#define IN_ABERTURA5 RPI_V2_GPIO_P1_38
#define IN_ABERTURA6 RPI_V2_GPIO_P1_40

#define OUT_LAMPADA1 RPI_V2_GPIO_P1_11
#define OUT_LAMPADA2 RPI_V2_GPIO_P1_12
#define OUT_LAMPADA3 RPI_V2_GPIO_P1_13
#define OUT_LAMPADA4 RPI_V2_GPIO_P1_15
#define OUT_AR1 RPI_V2_GPIO_P1_16
#define OUT_AR2 RPI_V2_GPIO_P1_18

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <bcm2835.h>
#include "types.h"

void gpio_setup();
void gpio_update(volatile dadosServidor* dados);
void gpio_shutdown();

#endif