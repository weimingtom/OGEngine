#ifndef __PARTY_H_
#define __PARTY_H_

#include "Library.h"
#include "PacketGame.h"
#include "GUI.H"

class ClassParty
{
 private:
         ClassGUI PartyGUI;
         
         unsigned int StatPartyLider;
         char PartyMaster[16];
         char AllNameParty[4][16];
 public:
        char PartyName[MAX_GUILDNAME];
        
        void InitParty();
        bool OpenningPartyGUI();
        void PacketRecvPartyInfo(char *buffer);
        void DrawParty(BITMAP *);
        
};

#endif
