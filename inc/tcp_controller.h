#ifndef __TCP__
#define __TCP__

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "types.h"

void tcp_setup();
void tcp_shutdown();
void tcp_acceptConnection(volatile dadosServidor* dados);
void TrataClienteTCP(int socketCliente, volatile dadosServidor *dados);

#endif