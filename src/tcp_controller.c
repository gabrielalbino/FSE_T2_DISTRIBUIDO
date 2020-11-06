#include "tcp_controller.h"

int servidorSocket;
int socketCliente;
struct sockaddr_in servidorAddr;
struct sockaddr_in clienteAddr;
unsigned int clienteLength;

void tcp_setup(){
  if((servidorSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		printf("falha no socker do Servidor\n");

	// Montar a estrutura sockaddr_in
	memset(&servidorAddr, 0, sizeof(servidorAddr)); // Zerando a estrutura de dados
	servidorAddr.sin_family = AF_INET;
	servidorAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servidorAddr.sin_port = htons(10116);

	// Bind
	if(bind(servidorSocket, (struct sockaddr *) &servidorAddr, sizeof(servidorAddr)) < 0)
		printf("Falha no Bind\n");

	// Listen
	if(listen(servidorSocket, 10) < 0)
		printf("Falha no Listen\n");		
}

void tcp_shutdown(){
  close(servidorSocket);
}

void tcp_acceptConnection(volatile dadosServidor* dados){
  while(1) {
    clienteLength = sizeof(clienteAddr);
    if((socketCliente = accept(servidorSocket, (struct sockaddr *) &clienteAddr, &clienteLength)) >= 0){
      TrataClienteTCP(socketCliente, dados);
      close(socketCliente);
    }
  }
}
void TrataClienteTCP(int socketCliente, volatile dadosServidor *dados){
  requisicao req;
  int tamanhoRecebido;
  if((tamanhoRecebido = recv(socketCliente, &req, sizeof(req), 0)) < 0)
    return;

  switch(req.command){
    case 1:
      dados->estadoAr = req.payload[0];
      dados->temperaturaAr = req.fPayload;
      break;
    case 2:
      dados->estadoLampadas[req.payload[0]] = req.payload[1];
  }
}
