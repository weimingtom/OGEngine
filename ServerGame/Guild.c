#include "Guild.h"

void ClassGuild::SGuildInfo(int p_index, MYSQL *SQLConnect, MyClassAccount *MCA)
{
 char buffer[sizeof(SendGuildInfo)];
 char search[1024];
 
 SGI = (SendGuildInfo*)&buffer;
 memset(buffer, 0, sizeof(SendGuildInfo));
 
 SGI->type_one = 0x00;
 SGI->type_two = 0x03;
 
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
    {
     if(atol(guild_rol[1]) == MCA->AccountUID)
      SGI->StatGuildLider = 1;
      
     strcpy(SGI->GuildName, guild_rol[2]);
     
     sprintf(search, "SELECT * from account where GuildUID = '%d';", MCA->GuildUID);
     mysql_query(SQLConnect, search);
     MYSQL_RES *account_rj = mysql_store_result(SQLConnect);
     MYSQL_ROW account_rol = mysql_fetch_row(account_rj);
     
     int count = 0;
     
     if(account_rol != NULL)
     { 
      do
      {
       if(count >= 15)
         break;
         
      if(atol(guild_rol[1]) == atol(account_rol[0]))
       strncpy(SGI->GuildMaster, account_rol[1], 16);
      else
      {
       for(int i = 0; i < 15; i++)
       {
        if(atol(guild_rol[3+i]) == atol(account_rol[0]))
        {
         strncpy(SGI->AllNameGuild[count], account_rol[1], sizeof(SGI->AllNameGuild[count]));
         count++;
         break;
        }
       }
      }
      }while(account_rol = mysql_fetch_row(account_rj));
     }
     mysql_free_result(account_rj);
    }
    else
    {
     sprintf(search, "UPDATE account SET GuildUID = '0' where AccountUID = '%d';", MCA->AccountUID);
     mysql_query(SQLConnect, search);
    }
   }
  }while(guild_rol = mysql_fetch_row(guild_rj));
 }
 mysql_free_result(guild_rj);
 
 s_send(p_index, buffer, sizeof(SendGuildInfo));
}
