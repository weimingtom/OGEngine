#include "Socket.h"
#include "Library.h"

s_con p_connection[MAX_PLAYER];

static int count_p_online;

void ClearSocket()
{
 for(int i = 0; i < MAX_PLAYER; i++)
 {
  p_connection[i].action_type = false;
  p_connection[i].timer = -1;
 }
}

int InitSocket(SOCKET winsock)
{
 char buffer[sizeof(StructRecvAccount)];
 int i;
 
 TypeInfoPacket *SIT = (TypeInfoPacket *)&buffer;
 
 for(i = 0; i < MAX_PLAYER; i++)
 {
  if(p_connection[i].action_type == false)
  {
   p_connection[i].winsock = winsock;
   count_p_online++;
   break;
  }
 }
 
 if(i == MAX_PLAYER)
 {
  // Servidor Cheio
  SIT->type_one = 0x01;
  SIT->type_two = 0x00;
  send(winsock, buffer, sizeof(TypeInfoPacket), 0);
  printf("Servidor Cheio\n");
  closesocket(winsock);
  return -1;
 }
 else
 {
  // Server Slot Reserved
  printf("Player [%d] - Conectado\n", i);
  SIT->type_one = 0x01;
  SIT->type_two = 0x06;
  send(winsock, buffer, sizeof(TypeInfoPacket), 0);
  p_connection[i].action_type = true;
  return i;
 }
}

int c_recv(int index, char*s, int len)
{
 return recv(p_connection[index].winsock, s, len, 0);
}

void s_send(int index, char*s, int len)
{
 send(p_connection[index].winsock, s, len, 0);
}

void close_socket(int index)
{
 closesocket(p_connection[index].winsock);
 count_p_online--;
 p_connection[index].action_type = 0;
}

int GetTimer(int index)
{
 return p_connection[index].timer;
}

void SetTimer(int index)
{
 p_connection[index].timer = 10;
}

bool GetAction(int index)
{
 return p_connection[index].action_type;
}

void DecrementTimer(int index)
{
 p_connection[index].timer--;
}

SOCKET GetSocket(int index)
{
 return p_connection[index].winsock;
}
