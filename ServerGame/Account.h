#ifndef __ACCOUNT_H_
#define __ACCOUNT_H_

#include "Library.h"
#include "Connect.h"

struct MyClassAccount
{
 unsigned int AccountUID;
 unsigned int MapUID;
 unsigned int GuildUID;
 unsigned int PartyUID;
 
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
 
 bool ActiveSendAndRecv;
 unsigned int direction;
};

int LoginAccept(MYSQL *, char *Login, char *Password);
void AccountOffline(unsigned int AccountUID, MYSQL *);
void SendInfo(unsigned int p_index, unsigned int type);
void SendAccountInfo(MyClassAccount *, unsigned int p_index, MYSQL *);

void SaveAccountDB(MyClassAccount *, MYSQL *);

void SendInfoToPlayerIndex(MyClassAccount *, char *, unsigned int);

void SendMap(unsigned int p_index, MyClassAccount *, MYSQL *);
void SendCoord(unsigned int, MyClassAccount *);

#endif
