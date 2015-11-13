#ifndef __GUILD_H_
#define __GUILD_H_

#include "Library.h"
#include "PacketGame.h"
#include "GUI.H"

class ClassGuild
{
 private:
         ClassGUI GuildGUI;
         
         unsigned int StatGuildLider;
         char GuildMaster[16];
         char AllNameGuild[15][16];
 public:
        char GuildName[MAX_GUILDNAME];
        
        void InitGuild();
        bool OpenningGuildGUI();
        void PacketRecvGuildInfo(char *buffer);
        void DrawGuild(BITMAP *);
        
};

#endif
