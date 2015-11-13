#include "Players.h"

void ClassPlayers::PlayerOnline(unsigned int p_index, MyClassAccount *MCA, MYSQL *SQLConnect)
{
 char search[1024];
 char buffer[sizeof(SendGamePlayers)];
 memset(buffer, 0, sizeof(buffer));
 
 pSGP = (SendGamePlayers*)&buffer;
 
 pSGP->type_one = 0x01;
 pSGP->type_two = 0x05;
 
 pSGP->player_id = p_index;
    
 sprintf(search, "SELECT * from account where AccountUID = '%d';", MCA[p_index].AccountUID);
 mysql_query(SQLConnect, search);
 MYSQL_RES *account_rj = mysql_store_result(SQLConnect);
 MYSQL_ROW account_rol = mysql_fetch_row(account_rj);
 
 if(account_rol != NULL)
 { 
  do
  {
   strncpy(pSGP->PlayerName, account_rol[1], 16);
  }while(account_rol = mysql_fetch_row(account_rj));
 }
 mysql_free_result(account_rj);
    
 sprintf(search, "SELECT * from guild where GuildUID = '%d';", MCA[p_index].GuildUID);
 mysql_query(SQLConnect, search);
 MYSQL_RES *guild_rj = mysql_store_result(SQLConnect);
 MYSQL_ROW guild_rol = mysql_fetch_row(guild_rj);
 
 if(guild_rol != NULL)
 { 
  do
  {
   strncpy(pSGP->GuildName, guild_rol[2], 16);
  }while(guild_rol = mysql_fetch_row(guild_rj));
 }
 mysql_free_result(guild_rj);
    
 sprintf(search, "SELECT * from party where PartyUID = '%d';", MCA[p_index].PartyUID);
 mysql_query(SQLConnect, search);
 MYSQL_RES *party_rj = mysql_store_result(SQLConnect);
 MYSQL_ROW party_rol = mysql_fetch_row(party_rj);
 
 if(party_rol != NULL)
 { 
  do
  {
   strncpy(pSGP->PartyName, party_rol[2], 16);
  }while(party_rol = mysql_fetch_row(party_rj));
 }
 mysql_free_result(party_rj);
   
 pSGP->VIP = MCA[p_index].vip;
 pSGP->CoordX = MCA[p_index].Player_x;
 pSGP->CoordY = MCA[p_index].Player_y;
   
 pSGP->Direction = 2;
 pSGP->Class = MCA[p_index].p_class;
 
 for(int i = 0; i < MAX_PLAYER; i++)
 {
  if(MCA[i].ActiveSendAndRecv == true && i != p_index)
  {
   if(MCA[i].MapUID == MCA[p_index].MapUID)
   {
    s_send(i, buffer, sizeof(SendGamePlayers));
   }
  }
 }
}

void ClassPlayers::PlayerOffline(unsigned int p_index, MyClassAccount *MCA)
{
 char buffer[sizeof(SendRemovePlayer)];
 memset(buffer, 0, sizeof(buffer));
 
 pSRP = (SendRemovePlayer*)&buffer;
 
 pSRP->type_one = 0x03;
 pSRP->type_two = 0x05;
 
 pSRP->player_id = p_index;
 
 for(int i = 0; i < MAX_PLAYER; i++)
 {
  if(MCA[i].ActiveSendAndRecv == true && i != p_index)
  {
   if(MCA[i].MapUID == MCA[p_index].MapUID)
   {
    s_send(i, buffer, sizeof(SendRemovePlayer));
   }
  }
 }
}

void ClassPlayers::SendAllPlayersOnline(unsigned int p_index, MyClassAccount *MCA, MYSQL *SQLConnect)
{
 char search[1024];
 char buffer[sizeof(SendGamePlayers)];
 
 pSGP = (SendGamePlayers*)&buffer;
 
 for(int i = 0; i < MAX_PLAYER; i++)
 {
  if(MCA[i].ActiveSendAndRecv == true && i != p_index)
  {
   memset(buffer, 0, sizeof(buffer));
   pSGP->type_one = 0x01;
   pSGP->type_two = 0x05;
 
   if(MCA[i].MapUID == MCA[p_index].MapUID)
   {
    sprintf(search, "SELECT * from account where AccountUID = '%d';", MCA[i].AccountUID);
    mysql_query(SQLConnect, search);
    MYSQL_RES *account_rj = mysql_store_result(SQLConnect);
    MYSQL_ROW account_rol = mysql_fetch_row(account_rj);
 
    if(account_rol != NULL)
    { 
     do
     {
      strncpy(pSGP->PlayerName, account_rol[1], 16);
     }while(account_rol = mysql_fetch_row(account_rj));
    }
    mysql_free_result(account_rj);
    
    sprintf(search, "SELECT * from guild where GuildUID = '%d';", MCA[i].GuildUID);
    mysql_query(SQLConnect, search);
    MYSQL_RES *guild_rj = mysql_store_result(SQLConnect);
    MYSQL_ROW guild_rol = mysql_fetch_row(guild_rj);
 
    if(guild_rol != NULL)
    { 
     do
     {
      strncpy(pSGP->GuildName, guild_rol[2], 16);
     }while(guild_rol = mysql_fetch_row(guild_rj));
    }
    mysql_free_result(guild_rj);
    
    sprintf(search, "SELECT * from party where PartyUID = '%d';", MCA[i].PartyUID);
    mysql_query(SQLConnect, search);
    MYSQL_RES *party_rj = mysql_store_result(SQLConnect);
    MYSQL_ROW party_rol = mysql_fetch_row(party_rj);
 
    if(party_rol != NULL)
    { 
     do
     {
      strncpy(pSGP->PartyName, party_rol[2], 16);
     }while(party_rol = mysql_fetch_row(party_rj));
    }
    mysql_free_result(party_rj);
   
    pSGP->VIP = MCA[i].vip;
    pSGP->CoordX = MCA[i].Player_x;
    pSGP->CoordY = MCA[i].Player_y;
   
    pSGP->Direction = MCA[i].direction;
    pSGP->Class = MCA[i].p_class;
 
    pSGP->player_id = i;
    s_send(p_index, buffer, sizeof(SendGamePlayers));
   }
  }
 }
}

void ClassPlayers::SendAllMove(unsigned int p_index, MyClassAccount *MCA)
{
 char buffer[sizeof(SendPlayersDirection)];
 memset(buffer, 0, sizeof(buffer));
 pSPD = (SendPlayersDirection*)&buffer;
 
 pSPD->type_one = 0x02;
 pSPD->type_two = 0x05;
 
 pSPD->player_id = p_index;
 pSPD->Direction = MCA[p_index].direction;
 
 for(int i = 0; i < MAX_PLAYER; i++)
 {
  if(MCA[i].ActiveSendAndRecv == true && i != p_index)
  {
   if(MCA[i].MapUID == MCA[p_index].MapUID)
   {
    s_send(i, buffer, sizeof(SendPlayersDirection));
   }
  }
 }
}

void ClassPlayers::ResetPlayers(unsigned int p_index)
{
 char buffer[sizeof(TypeInfoPacket)];
 TypeInfoPacket *TIP = (TypeInfoPacket *)&buffer;
 TIP->type_one = 0x05;
 TIP->type_two = 0x04;
 s_send(p_index, buffer, sizeof(TypeInfoPacket));
}
