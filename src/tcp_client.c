#include "tcp_client.h"

int clienteSocket;
struct sockaddr_in servidorAddr;
char buffer[16];
unsigned int tamanhoMensagem;
const char IP_Servidor[] = "192.168.0.53";
const unsigned short servidorPorta = 10016;

void client_send(requisicao req){

	// Criar Socket
	if((clienteSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		printf("Erro no socket()\n");

	// Construir struct sockaddr_in
	memset(&servidorAddr, 0, sizeof(servidorAddr)); // Zerando a estrutura de dados
	servidorAddr.sin_family = AF_INET;
	servidorAddr.sin_addr.s_addr = inet_addr(IP_Servidor);
	servidorAddr.sin_port = htons(servidorPorta);

	// Connect
	if(connect(clienteSocket, (struct sockaddr *) &servidorAddr, 
							sizeof(servidorAddr)) < 0)
		printf("Erro no connect()\n");

	tamanhoMensagem = sizeof(req);

	send(clienteSocket, &req, tamanhoMensagem, 0);
  
	close(clienteSocket);

}
