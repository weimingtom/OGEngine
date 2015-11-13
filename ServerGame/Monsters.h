#ifndef __MONSTERS_H_
#define __MONSTERS_H_

#include "Library.h"
#include "Connect.h"
#include "StructPacket.h"
#include "Account.h"

struct StructMonsters
{
       unsigned int MonsterUID;
       unsigned int MapUID;
       unsigned int MonsterType;
       
       unsigned int Level;
       unsigned int EXP;
       
       int MAX_HP;
       int HP;
       
       unsigned int ATK;
       unsigned int DEF;
       
       unsigned int CopyMap_X;
       unsigned int CopyMap_Y;
       
       unsigned int Map_X;
       unsigned int Map_Y;
       
       unsigned int Rect_X;
       unsigned int Rect_Y;
       
       unsigned int ItemDropUID;
       unsigned int MoneyDrop;
       
       unsigned int PlayerUIDFollow;
       
       int ReviveTime;
       int RealTime;
       
       char MonsterName[16];
       
       bool ActiveMonster;
       unsigned int TimeWalk;
};

// Packets ...

struct StructSendCreateMonsters
{
 unsigned int type_one, type_two;
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

struct StructSendMoveMonsters
{
 unsigned int type_one, type_two;
 unsigned int MonsterUID;
 unsigned int Direction;
};

struct StructSendDeleteMonsters
{
 unsigned int type_one, type_two;
 unsigned int MonsterUID;
};

struct RecvInfoMonsters
{
 unsigned int type_one, type_two;
 unsigned int MonsterUID;
 unsigned int HP;
};

void InitMonsters(StructMonsters *, char *);
void SendAllMonsters(unsigned int, MyClassAccount *, StructMonsters*);                                  // Envia monstros ativos para quem entrou
void SendMonster(StructMonsters *, MyClassAccount *);                                      // Envia um monstro que foi ativo                               // Checa o limite do rect dos monstros
void FollowMonsterPlayerRect(StructMonsters*, MyClassAccount *, unsigned int);        // Segue player caso chegue perto do limite do monstro
//void DropItemMonsters(StructMonsters*); Next ...
void CheckAndMoveMonsters(StructMonsters*, MyClassAccount *);
void AttackMonster(unsigned int, StructMonsters*, MyClassAccount *);
void MonsterKill(StructMonsters*, MyClassAccount *);
void ResetMonsters(unsigned int);
void SendInfoMonsters(StructMonsters*);

#endif
