#include "Party.h"

void ClassParty::SPartyInfo(int p_index, MYSQL *SQLConnect, MyClassAccount *MCA)
{
 char buffer[sizeof(StructSendPartyInfo)];
 char search[1024];
 
 SSPI = (StructSendPartyInfo*)&buffer;
 memset(buffer, 0, sizeof(StructSendPartyInfo));
 
 SSPI->type_one = 0x00;
 SSPI->type_two = 0x04;
 
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
    {
     if(atol(party_rol[1]) == MCA->AccountUID)
      SSPI->StatPartyLider = 1;
      
     strncpy(SSPI->PartyName, party_rol[2], 16);
     
     sprintf(search, "SELECT * from account where PartyUID = '%d';", MCA->PartyUID);
     mysql_query(SQLConnect, search);
     MYSQL_RES *account_rj = mysql_store_result(SQLConnect);
     MYSQL_ROW account_rol = mysql_fetch_row(account_rj);
     
     int count = 0;
     
     if(account_rol != NULL)
     { 
      do
      {
       if(count >= 4)
         break;
         
      if(atol(party_rol[1]) == atol(account_rol[0]))
       strncpy(SSPI->PartyMaster, account_rol[1], 16);
      else
      {
       for(int i = 0; i < 4; i++)
       {
        if(atol(party_rol[3+i]) == atol(account_rol[0]))
        {
         strncpy(SSPI->AllNameParty[count], account_rol[1], sizeof(SSPI->AllNameParty[count]));
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
     sprintf(search, "UPDATE account SET PartyUID = '0' where AccountUID = '%d';", MCA->AccountUID);
     mysql_query(SQLConnect, search);
    }
   }
  }while(party_rol = mysql_fetch_row(party_rj));
 }
 mysql_free_result(party_rj);
 
 s_send(p_index, buffer, sizeof(StructSendPartyInfo));
}
