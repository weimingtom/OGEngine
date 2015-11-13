#ifndef __PLAYERS_H_
#define __PLAYERS_H_

#include "Library.h"
#include "Connect.h"
#include "StructPacket.h"
#include "Account.h"

class ClassPlayers
{
 private:
         SendGamePlayers *pSGP;
         SendPlayersDirection *pSPD;
         SendRemovePlayer * pSRP;
 public:
        void PlayerOnline(unsigned int, MyClassAccount*, MYSQL *);
        void PlayerOffline(unsigned int, MyClassAccount *MCA);
        void SendAllPlayersOnline(unsigned int, MyClassAccount *, MYSQL *);
        void SendAllMove(unsigned int, MyClassAccount *MCA);
        void ResetPlayers(unsigned int);
};

#endif
