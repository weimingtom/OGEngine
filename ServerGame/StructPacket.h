#ifndef __STRUCTPACKET_H_
#define __STRUCTPACKET_H_

#include "Library.h"

// PACKET LOGIN

#define MAX_CARACTER 32

struct TypeInfoPacket
{
 unsigned int type_one;
 unsigned int type_two;
};

struct StructRecvAccount
{
 char Login[MAX_CARACTER];
 char Password[MAX_CARACTER];
};

struct SendPlayerInfo
{
 unsigned int type_one, type_two;
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
  
 unsigned int Player_x;
 unsigned int Player_y;
};

#define MAX_SIZE_MAP 16


struct InitSendPlayerMap
{
 char FileMap[MAX_SIZE_MAP];
 /*char FileColision[MAX_SIZE_MAP];*/
 char FileTiles[MAX_SIZE_MAP];
};

struct SendPlayerMap
{
 unsigned int type_one, type_two;
 char FileMap[MAX_SIZE_MAP];
 /*char FileColision[MAX_SIZE_MAP];*/
 char FileTiles[MAX_SIZE_MAP];
};

struct StructSendCoord
{
 unsigned int type_one, type_two;
 int Player_X;
 int Player_Y;
};

struct RecvGameInfo
{
 unsigned int Direction;
};

///////////////////////////////////////

struct SendGamePlayers
{
 unsigned int type_one, type_two;
 unsigned int player_id;
 char PlayerName[16];
 char GuildName[16];
 char PartyName[16];
 unsigned int VIP;
 unsigned int CoordX;
 unsigned int CoordY;
 unsigned int Direction;
 unsigned int Class;
 
 bool ActionPlayer;
 unsigned int Moviment, SaveMoviment;
 unsigned int MovX, MovY;
};

struct SendPlayersDirection
{
 unsigned int type_one;
 unsigned int type_two;
 unsigned int player_id;
 unsigned int Direction;
};

struct SendRemovePlayer
{
 unsigned int type_one, type_two;
 unsigned int player_id;
};

///////////////////////////////////////

struct SendGuildInfo
{
 unsigned int type_one;
 unsigned int type_two;
 char GuildName[16];
 unsigned int StatGuildLider;
 char GuildMaster[16];
 char AllNameGuild[15][16];
};

struct StructSendPartyInfo
{
 unsigned int type_one;
 unsigned int type_two;
 char PartyName[16];
 unsigned int StatPartyLider;
 char PartyMaster[16];
 char AllNameParty[15][16];
};

struct StructSendInfoPlayer
{
 unsigned int type_one, type_two;
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

 unsigned int points;
 unsigned int exp;
 unsigned int max_exp;

 char log_message[127];
};

#endif
