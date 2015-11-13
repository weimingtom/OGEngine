#ifndef __SOCKET_H_
#define __SOCKET_H_

#include "Library.h"
#include "StructPacket.h"

void ClearSocket();
int InitSocket(SOCKET);
int c_recv(int, char*, int);
void s_send(int, char*, int);
void close_socket(int);
int GetTimer(int);
void SetTimer(int);
bool GetAction(int);
void DecrementTimer(int);
SOCKET GetSocket(int);

#endif
