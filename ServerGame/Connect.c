#include "Connect.h"

ClassConnect::ClassConnect()
{
 WSAStartup(0x202, &data);
 
 sock.sin_family = AF_INET;
 sock.sin_addr.s_addr = INADDR_ANY;
}

ClassConnect::~ClassConnect()
{
 closesocket(winsock);
}

void ClassConnect::InitConnect(int port)
{
 sock.sin_port=htons(port);
 
 winsock = socket(AF_INET, SOCK_STREAM, 0);
 
 bind(winsock, (SOCKADDR*)&sock, sizeof(sock));
 listen(winsock, 5);
}

int ClassConnect::AcceptSocket()
{
 struct sockaddr_in client_info;
 int lenght = sizeof(struct sockaddr_in);
 return InitSocket(accept(winsock, (struct sockaddr *)&client_info, &lenght));
}
