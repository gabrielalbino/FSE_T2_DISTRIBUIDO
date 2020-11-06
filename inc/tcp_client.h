#ifndef __TCPC__
#define __TCPC__
#include "types.h"
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void client_send(requisicao req);

#endif