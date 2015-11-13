#ifndef __GUILD_H_
#define __GUILD_H_

#include "Library.h"
#include "Connect.h"
#include "StructPacket.h"
#include "Account.h"

class ClassGuild
{
 private:
         SendGuildInfo *SGI;
 public:
        void SGuildInfo(int, MYSQL *, MyClassAccount *);
};

#endif
