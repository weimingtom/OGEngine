#ifndef __CONNECT_H_
#define __CONNECT_H_

#include "Library.h"
#include "Socket.h"

class ClassConnect
{
      private:
              WSADATA data;
              SOCKET winsock;
              SOCKADDR_IN sock;
      public:
             ClassConnect();
             ~ClassConnect();
             void InitConnect(int);
             int AcceptSocket();
             
};

#endif
