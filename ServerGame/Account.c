#include "Account.h"

int LoginAccept(MYSQL *SQLConnect, char *Login, char *Password)
{
 char search[1024];
 
 printf("Login [%s] Password [%s]\n", Login, Password);
 sprintf(search, "SELECT * from account where Login = '%s' and Password = '%s';", Login, Password);
 
 mysql_query(SQLConnect, search);
 MYSQL_RES *rj = mysql_store_result(SQLConnect);
 MYSQL_ROW rol = mysql_fetch_row(rj);
 
 if(rol != NULL)
 { 
  do
  {
   if(!strcmp(rol[1], Login) && !strcmp(rol[2], Password))
   {
    if(atol(rol[4]) == 1)
     return -1;
    if(atol(rol[5]) == 1)
     return -2;    // Banned
     
    sprintf(search, "UPDATE account SET Online = '1' where AccountUID = '%d';", atol(rol[0]));
    mysql_query(SQLConnect, search);
    
    return atol(rol[0]);
   }
  }while(rol = mysql_fetch_row(rj));
 }
 
 mysql_free_result(rj);
 return 0;             // Invalid Account
}

void AccountOffline(unsigned int AccountUID, MYSQL *SQLConnect)
{
 char search[1024];
 sprintf(search, "UPDATE account SET Online = '0' where AccountUID = '%d';", AccountUID);
 mysql_query(SQLConnect, search);
}

void SendInfo(unsigned int p_index, unsigned int type)
{
 char buffer[1024];
 TypeInfoPacket *SIT = (TypeInfoPacket *)&buffer;
 SIT->type_one = 01;
 SIT->type_two = type;
 s_send(p_index, buffer, sizeof(TypeInfoPacket));
}

void SendAccountInfo(MyClassAccount *MCA, unsigned int p_index, MYSQL *SQLConnect)
{
 char buffer[1024];
 char search[1024];
 
 memset(buffer, 0, sizeof(buffer));
 
 SendPlayerInfo *SIP = (SendPlayerInfo *)&buffer;
 sprintf(search, "SELECT * from account where AccountUID = '%d';", MCA->AccountUID);
 mysql_query(SQLConnect, search);
 MYSQL_RES *account_rj = mysql_store_result(SQLConnect);
 MYSQL_ROW account_rol = mysql_fetch_row(account_rj);
 
 if(account_rol != NULL)
 { 
  do
  {
   if(atol(account_rol[0]) == MCA->AccountUID)
   {
    strcpy(SIP->PlayerName, account_rol[1]);
    SIP->PlayerName[MAX_PLAYERNAME-1] = 0x00;
    
    MCA->GuildUID = atol(account_rol[6]);
    
    sprintf(search, "SELECT * from guild where GuildUID = '%d';", MCA->GuildUID);
    mysql_query(SQLConnect, search);
    MYSQL_RES *guild_rj = mysql_store_result(SQLConnect);
    MYSQL_ROW guild_rol = mysql_fetch_row(guild_rj);
    
    if(guild_rol != NULL)
    { 
      do
      {
       if(atol(guild_rol[0]) == MCA->GuildUID)
       {
        if(atol(guild_rol[1]) == MCA->AccountUID || atol(guild_rol[3]) == MCA->AccountUID || atol(guild_rol[4]) == MCA->AccountUID || atol(guild_rol[5]) == MCA->AccountUID || atol(guild_rol[6]) == MCA->AccountUID || atol(guild_rol[7]) == MCA->AccountUID || atol(guild_rol[8]) == MCA->AccountUID || atol(guild_rol[9]) == MCA->AccountUID || atol(guild_rol[10]) == MCA->AccountUID || atol(guild_rol[11]) == MCA->AccountUID || atol(guild_rol[12]) == MCA->AccountUID || atol(guild_rol[13]) == MCA->AccountUID || atol(guild_rol[14]) == MCA->AccountUID || atol(guild_rol[15]) == MCA->AccountUID || atol(guild_rol[16]) == MCA->AccountUID || atol(guild_rol[17]) == MCA->AccountUID || atol(guild_rol[18]) == MCA->AccountUID)
         strcpy(SIP->GuildName, guild_rol[2]);
        else
        {
         sprintf(search, "UPDATE account SET GuildUID = '0' where AccountUID = '%d';", MCA->AccountUID);
         mysql_query(SQLConnect, search);
        }
       }
      }while(guild_rol = mysql_fetch_row(guild_rj));
    }
    mysql_free_result(guild_rj);
    
    SIP->PlayerName[MAX_GUILDNAME-1] = 0x00;
    
    MCA->PartyUID = atol(account_rol[7]);
    
    sprintf(search, "SELECT * from party where PartyUID = '%d';", MCA->PartyUID);
    mysql_query(SQLConnect, search);
    MYSQL_RES *party_rj = mysql_store_result(SQLConnect);
    MYSQL_ROW party_rol = mysql_fetch_row(party_rj);
    
    if(party_rol != NULL)
    { 
      do
      {
       if(atol(party_rol[0]) == MCA->PartyUID)
       {
        if(atol(party_rol[1]) == MCA->AccountUID || atol(party_rol[3]) == MCA->AccountUID || atol(party_rol[4]) == MCA->AccountUID || atol(party_rol[5]) == MCA->AccountUID || atol(party_rol[6]) == MCA->AccountUID)
         strcpy(SIP->PartyName, party_rol[2]);
        else
        {
         sprintf(search, "UPDATE account SET PartyUID = '0' where AccountUID = '%d';", MCA->AccountUID);
         mysql_query(SQLConnect, search);
        }
       }
      }while(party_rol = mysql_fetch_row(party_rj));
    }
    mysql_free_result(party_rj);
    
    SIP->PlayerName[MAX_PARTYNAME-1] = 0x00;
    
    SIP->level = atol(account_rol[8]);
    MCA->level = SIP->level;
    
    SIP->max_hp = atol(account_rol[9]);
    SIP->hp = atol(account_rol[10]);
    MCA->max_hp = SIP->max_hp;
    MCA->hp = SIP->hp;
    
    SIP->max_mp = atol(account_rol[11]);
    SIP->mp = atol(account_rol[12]);
    MCA->max_mp = SIP->max_mp;
    MCA->mp = SIP->mp; 
    
    SIP->s_str = atol(account_rol[13]);
    SIP->s_def = atol(account_rol[14]);
    SIP->s_int = atol(account_rol[15]);
    SIP->s_agi = atol(account_rol[16]);
    
    MCA->s_str = SIP->s_str;
    MCA->s_def = SIP->s_def;
    MCA->s_int = SIP->s_int;
    MCA->s_agi = SIP->s_agi;
    
    SIP->points = atol(account_rol[17]);
    MCA->points = SIP->points;
    
    SIP->money = atol(account_rol[18]);
    MCA->money = SIP->money;
    
    SIP->vip = atol(account_rol[19]);
    MCA->vip = SIP->vip;
    
    SIP->p_class = atol(account_rol[20]);
    MCA->p_class = SIP->p_class;
    
    SIP->exp = atol(account_rol[21]);
    MCA->exp = SIP->exp;
    
    SIP->max_exp = atol(account_rol[22]);
    MCA->max_exp = SIP->max_exp;
    
    SIP->Player_x = atol(account_rol[24]);
    SIP->Player_x = SIP->Player_x*32;
    MCA->Player_x = SIP->Player_x;
    
    SIP->Player_y = atol(account_rol[25]);
    SIP->Player_y = SIP->Player_y*32;
    MCA->Player_y = SIP->Player_y;
    
    MCA->MapUID = atol(account_rol[23]);
    
    s_send(p_index, buffer, sizeof(SendPlayerInfo));
    
    memset(buffer, 0, sizeof(buffer));
    InitSendPlayerMap *SPM = (InitSendPlayerMap *)&buffer;
    
    sprintf(search, "SELECT * from map where MapUID = '%d';", MCA->MapUID);
    mysql_query(SQLConnect, search);
    MYSQL_RES *map_rj = mysql_store_result(SQLConnect);
    MYSQL_ROW map_rol = mysql_fetch_row(map_rj);
    
    if(map_rol != NULL)
    {
      do
      {
       if(atol(map_rol[0]) == MCA->MapUID)
       {
        strcpy(SPM->FileMap, map_rol[1]);
        SPM->FileMap[MAX_SIZE_MAP-1] = 0x00;
        /*strcpy(SPM->FileColision, map_rol[2]);
        SPM->FileColision[MAX_SIZE_MAP-1] = 0x00;*/
        strcpy(SPM->FileTiles, map_rol[2]);
        SPM->FileTiles[MAX_SIZE_MAP-1] = 0x00;
       }
      }while(map_rol = mysql_fetch_row(map_rj));
    }
    else
    {
     strcpy(SPM->FileMap, "InvalidMap");
    }
    
    s_send(p_index, buffer, sizeof(InitSendPlayerMap));
    
    mysql_free_result(map_rj);
   }
  }while(account_rol = mysql_fetch_row(account_rj));
 }
 
 mysql_free_result(account_rj);
}

void SaveAccountDB(MyClassAccount *MCA, MYSQL *SQLConnect)
{
 char save[1024];
 
 sprintf(save, "UPDATE account SET Level = '%d' where AccountUID = '%d';", MCA->level, MCA->AccountUID);
 mysql_query(SQLConnect, save);
 
 sprintf(save, "UPDATE account SET MaxHp = '%d' where AccountUID = '%d';", MCA->max_hp, MCA->AccountUID);
 mysql_query(SQLConnect, save);
 sprintf(save, "UPDATE account SET Hp = '%d' where AccountUID = '%d';", MCA->hp, MCA->AccountUID);
 mysql_query(SQLConnect, save);
 
 sprintf(save, "UPDATE account SET MaxMp = '%d' where AccountUID = '%d';", MCA->max_mp, MCA->AccountUID);
 mysql_query(SQLConnect, save);
 sprintf(save, "UPDATE account SET Mp = '%d' where AccountUID = '%d';", MCA->mp, MCA->AccountUID);
 mysql_query(SQLConnect, save);
 
 sprintf(save, "UPDATE account SET S_str = '%d' where AccountUID = '%d';", MCA->s_str, MCA->AccountUID);
 mysql_query(SQLConnect, save);
 sprintf(save, "UPDATE account SET S_def = '%d' where AccountUID = '%d';", MCA->s_def, MCA->AccountUID);
 mysql_query(SQLConnect, save);
 sprintf(save, "UPDATE account SET S_agi = '%d' where AccountUID = '%d';", MCA->s_agi, MCA->AccountUID);
 mysql_query(SQLConnect, save);
 sprintf(save, "UPDATE account SET S_int = '%d' where AccountUID = '%d';", MCA->s_int, MCA->AccountUID);
 mysql_query(SQLConnect, save);
 
 sprintf(save, "UPDATE account SET Points = '%d' where AccountUID = '%d';", MCA->points, MCA->AccountUID);
 mysql_query(SQLConnect, save);
 
 sprintf(save, "UPDATE account SET Money = '%d' where AccountUID = '%d';", MCA->money, MCA->AccountUID);
 mysql_query(SQLConnect, save);
 
 sprintf(save, "UPDATE account SET Exp = '%d' where AccountUID = '%d';", MCA->exp, MCA->AccountUID);
 mysql_query(SQLConnect, save);
 sprintf(save, "UPDATE account SET MaxExp = '%d' where AccountUID = '%d';", MCA->max_exp, MCA->AccountUID);
 mysql_query(SQLConnect, save);
 
 sprintf(save, "UPDATE account SET CoordX = '%d' where AccountUID = '%d';", MCA->Player_x/32, MCA->AccountUID);
 mysql_query(SQLConnect, save);
 sprintf(save, "UPDATE account SET CoordY = '%d' where AccountUID = '%d';", MCA->Player_y/32, MCA->AccountUID);
 mysql_query(SQLConnect, save);
 
 sprintf(save, "UPDATE account SET MapUID = '%d' where AccountUID = '%d';", MCA->MapUID, MCA->AccountUID);
 mysql_query(SQLConnect, save);
}

void SendInfoToPlayerIndex(MyClassAccount *MCA, char *LogMessage, unsigned int player_id)
{
 StructSendInfoPlayer *SSIP;
 char buffer[sizeof(StructSendInfoPlayer)];
 
 SSIP = (StructSendInfoPlayer*)&buffer;
 
 SSIP->type_one = 0x06;
 SSIP->type_two = 0x00;
 
 SSIP->level = MCA->level;
     
 SSIP->max_hp = MCA->max_hp;
 SSIP->hp = MCA->hp;
     
 SSIP->max_mp = MCA->max_mp;
 SSIP->mp = MCA->mp;

 SSIP->s_str = MCA->s_str;
 SSIP->s_def = MCA->s_def;
 SSIP->s_int = MCA->s_int;
 SSIP->s_agi = MCA->s_agi;
  
 SSIP->money = MCA->money;
 
 SSIP->points = MCA->points;
 SSIP->exp = MCA->exp;
 SSIP->max_exp = MCA->max_exp;
 
 strncpy(SSIP->log_message, LogMessage, 127);
 s_send(player_id, buffer, sizeof(StructSendInfoPlayer));
}

void SendMap(unsigned int p_index, MyClassAccount *MCA, MYSQL *SQLConnect)
{
 char search[1024];
 char buffer[sizeof(SendPlayerMap)];
 memset(buffer, 0, sizeof(buffer));
 SendPlayerMap *SPM = (SendPlayerMap *)&buffer;
 
 SPM->type_one = 0x06;
 SPM->type_two = 0x01;   
 
 sprintf(search, "SELECT * from map where MapUID = '%d';", MCA->MapUID);
 mysql_query(SQLConnect, search);
 MYSQL_RES *map_rj = mysql_store_result(SQLConnect);
 MYSQL_ROW map_rol = mysql_fetch_row(map_rj);
    
 if(map_rol != NULL)
 {
  do
  {
   if(atol(map_rol[0]) == MCA->MapUID)
   {
    strcpy(SPM->FileMap, map_rol[1]);
    SPM->FileMap[MAX_SIZE_MAP-1] = 0x00;
    strcpy(SPM->FileTiles, map_rol[2]);
    SPM->FileTiles[MAX_SIZE_MAP-1] = 0x00;
   }
  }while(map_rol = mysql_fetch_row(map_rj));
 }
 else
 {
  strcpy(SPM->FileMap, "InvalidMap");
 }
    
 s_send(p_index, buffer, sizeof(SendPlayerMap));
 mysql_free_result(map_rj);
}

void SendCoord(unsigned int p_index, MyClassAccount *MCA)
{
 char buffer[sizeof(StructSendCoord)];
 StructSendCoord *SC = (StructSendCoord *)&buffer;
 SC->type_one = 0x06;
 SC->type_two = 0x02;
 SC->Player_X = MCA->Player_x;
 SC->Player_Y = MCA->Player_y;
 s_send(p_index, buffer, sizeof(StructSendCoord));
}
