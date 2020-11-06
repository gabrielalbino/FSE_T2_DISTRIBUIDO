#ifndef __TYPES__
#define __TYPES__


typedef struct {
  float temperatura;
  float umidade;
  int estadoLampadas[4];
  int estadoAr;
  float temperaturaAr;
  int estadoSensoresPresenca[2];
  int estadoSensoresAbertura[6];
  int requestFlag;
  int payload;
  int statusAlarme;
} dadosServidor;

typedef struct {
  int command;
  int payload[2];
  float fPayload;
  dadosServidor dadosServidor;
} requisicao;

#endif
