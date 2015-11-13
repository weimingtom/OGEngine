#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "Library.h"
#include "PacketGame.h"
#include "Button.h"
#include "GUI.h"
#include "Guild.h"
#include "Party.h"

/* Structura do player principal */

class ClassPlayer
{
 private:
         char PlayerName[MAX_PLAYERNAME];
         char GuildName[MAX_GUILDNAME];
         char PartyName[MAX_PARTYNAME];
         
         unsigned int level;
         
         int max_hp;
         int hp;
         
         int max_mp;
         int mp;
         
         unsigned int s_str;
         unsigned int s_def;
         unsigned int s_int;
         unsigned int s_agi;
         
         unsigned int money;
         
         unsigned int vip;
         unsigned int p_class;
         
         unsigned int points;
         
         unsigned int exp;
         unsigned int max_exp;
         
         BITMAP *SpritePlayer[16];
         
         BITMAP *stat_hp;
         BITMAP *hp_hit;
         
         BITMAP *stat_mp;
         BITMAP *mp_hit;
         
         BITMAP *stat_exp;
         BITMAP *exp_hit;
         
         BITMAP *cifra;
         
         ClassButton StatusButton;
         ClassButton GuildButton;
         ClassButton PartyButton;
         ClassButton InventoryButton;
         ClassButton ExitButton;
         
         ClassGUI StatusGUI;
         //ClassGUI GuildGUI;
         ClassGuild Guild;
         ClassParty Party;
         
         unsigned int TimeButton;
         
         bool RecvGuildInfo;
         bool RecvPartyInfo;
         bool SendAttackInfoZ, SendAttackInfoX;
         
         int MovX, MovY;
         
         unsigned int Moviment, SaveMoviment;
         
         unsigned int TimeKeyButton;
         
         char LogMessage[5][127];
         
         int tx, ty;
         /*unsigned int StatGuildLider;
         char GuildMaster[16];
         char AllNameGuild[15][16];*/
         
 public:   
        unsigned int Direction;
        
        int CoordMap_x;
        int CoordMap_y;
        
        int StatCoordMap_x;
        int StatCoordMap_y;
        
        void InitPlayer();
        
        void LoadingSpritePlayer(char *filename);
        void Destroy();
        
        void PlayerLogic(SOCKET GSocket);
        void PlayerAttack(SOCKET GSocket);
        
        void StatKeyPlayer();
        void DrawPlayer(BITMAP *GameBuffer);
        void DrawStates(BITMAP *GameBuffer);
        void DrawStatBar(BITMAP *GameBuffer);
        void DrawGUIStat(BITMAP *GameBuffer);
        
        // ******************************
        
        int RecvStatPlayer(SOCKET GSocket);
        void PacketRecvGuildInfo(char *);
        void PacketRecvPartyInfo(char *);
        void PacketRecvAllInfo(char *);
        void PacketRecvCoord(char*);
        void PacketSendCoord(SOCKET GSocket);
        void SendMoveDirection(SOCKET GSocket);
};

#endif
