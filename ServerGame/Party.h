#ifndef __PARTY_H_
#define __PARTY_H_

#include "Library.h"
#include "Connect.h"
#include "StructPacket.h"
#include "Account.h"

class ClassParty
{
 private:
         StructSendPartyInfo *SSPI;
 public:
        void SPartyInfo(int, MYSQL *, MyClassAccount *);
};

#endif
