#ifndef __PACKETLOGIN_H_
#define __PACKETLOGIN_H_

// Struct dos packet

#define MAX_CARACTER 32

struct TypeInfoPacket
{
 unsigned int type_one;
 unsigned int type_two;
};

struct StructSendAccount
{
 char Login[MAX_CARACTER];
 char Password[MAX_CARACTER];
};

struct RecvPlayerInfo
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

struct RecvPlayerMap
{
 char FileMap[16];
 /*char FileColision[16];*/
 char FileTiles[16];
};


/////////////////////////////////////
// Monsters
/////////////////////////////////////

struct StructSendCreateMonsters
{
 unsigned int MonsterUID;
 unsigned int MonsterType;
       
 unsigned int HP;
 unsigned int ATK;
 unsigned int DEF;
       
 unsigned int Map_X;
 unsigned int Map_Y;
 
 unsigned int Level;
 
 char MonsterName[16];
};

struct RecvGameMonsters
{
 unsigned int MonsterUID;
 unsigned int MonsterType;
 
 unsigned int Level;
 unsigned int hp;

 char MonsterName[16];
 unsigned int CoordX;
 unsigned int CoordY;
 unsigned int Direction;
 
 unsigned int Moviment, SaveMoviment;
 unsigned int MovX, MovY;
 RecvGameMonsters *pRGM;
};

struct RecvDeleteMonsters
{
 unsigned int MonsterID;
};

struct RecvMoveMonsters
{
 unsigned int MonsterUID;
 unsigned int Direction;
};

struct RecvInfoMonsters
{
 unsigned int MonsterUID;
 unsigned int HP;
};

/////////////////////////////////////

/////////////////////////////////////
// Players
/////////////////////////////////////

struct RecvGamePlayers
{
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

struct RecvPlayersDirection
{
 unsigned int player_id;
 unsigned int Direction;
};

struct RecvRemovePlayer
{
 unsigned int player_id;
};

struct RecvCoord
{
 int Player_X;
 int Player_Y;
};

/////////////////////////////////////

struct SendGameInfo
{
 unsigned int type_one, type_two;
 unsigned int Direction;
};

struct StructRecvGuildInfo
{
 char GuildName[16];
 unsigned int StatGuildLider;
 char GuildMaster[16];
 char AllNameGuild[15][16];
};

struct StructRecvPartyInfo
{
 char PartyName[16];
 unsigned int StatPartyLider;
 char PartyMaster[16];
 char AllNameParty[15][16];
};

struct StructSendInfoPlayer
{
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

#define MAX_MSG_CHAT 127

struct SendMsgChat
{
 unsigned int type_one, type_two;
 char Msg[MAX_MSG_CHAT];
};

#endif
