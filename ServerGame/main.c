#include "Library.h"
#include "Connect.h"
#include "Account.h"
#include "Guild.h"
#include "Party.h"
#include "Players.h"
#include "Monsters.h"

#define PORT 9000
#define SQLPORT 3306

ClassConnect Connection;
ClassGuild CGuild;
ClassParty CParty;
ClassPlayers CPlayers;

StructMonsters SMonsters[MAX_MONSTERS_SERVER];

DWORD ThreadConnect(LPVOID index);
DWORD ThreadMonsters();
DWORD ThreadGeral();
DWORD AutenticationTimer();

char ip[20];
int port;
char sqluser[32];
char sqlpass[32];
char sql_db[32];

void OpenIni();

MYSQL SQLConnectGame;

MyClassAccount MCA[MAX_PLAYER];

int main(int argc, char *argv[])
{
    OpenIni();
    
    printf("*******************************\n");
    printf("******** GameServer ********\n");
    printf("*******************************\n");
    
    printf("INICIANDO SQL    [%s]:[%d]\n", ip, SQLPORT);
    
    mysql_init(&SQLConnectGame);
    if(!mysql_real_connect(&SQLConnectGame, ip, sqluser, sqlpass, sql_db,  0, NULL, 0))
    {
      printf("ERRO AO TENTAR INICIAR A SQL!!!\n");
      goto exit;
    }
    
    // Reset All Stat Online Player
    mysql_query(&SQLConnectGame, "UPDATE account SET Online = '0';");
    
    printf("SQL INICIADA COM SUCESSO!\n");
    mysql_close(&SQLConnectGame);
    
    printf("INICIANDO SOCKET [%s]:[%d]\n", ip, port);
    Connection.InitConnect(PORT);
    
    ClearSocket();
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AutenticationTimer, (LPVOID)0, 0, 0);
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadMonsters, (LPVOID)0, 0, 0);
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadGeral, (LPVOID)0, 0, 0);
    
    int index;
    
    while(true)
    {
     index = Connection.AcceptSocket();
     MCA[index].ActiveSendAndRecv = false;
     if(index >= 0)
      CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadConnect, (LPVOID)index, 0, 0);
      
     Sleep(100);
    }
    
    exit:
    mysql_close(&SQLConnectGame);
    getch();
    return 0;
}

DWORD AutenticationTimer()
{
 while(true)
 {
  for(int i = 0; i < MAX_PLAYER; i++)
  {
   if(GetAction(i) == true && GetTimer(i) > 0)
   {
    DecrementTimer(i);
   }
  }
  
  for(int i = 0; i < MAX_PLAYER; i++)
  {
   if(GetAction(i) == true)
   {
    if(GetTimer(i) == 0)
    {
     printf("[%d] timed current termined\n", i);
     close_socket(i);
     break;
    }
   }
  }
  Sleep(3000);
 }
}

DWORD ThreadConnect(LPVOID index)
{
 MYSQL SQLConnect;
 
 mysql_init(&SQLConnect);
 mysql_real_connect(&SQLConnect, ip, sqluser, sqlpass, sql_db,  0, NULL, 0);

 //MyClassAccount MCA;
 int typewait = true;
 int p_index = (int)index;
 unsigned int BytesRecv;
 
 int OfflineAccept = 0;
 
 TypeInfoPacket *SI;
 StructRecvAccount *SA;
 RecvGameInfo *RGI;
 
 char buffer[1024];
 memset(&buffer, 0, sizeof(buffer));
 
 SI = (TypeInfoPacket*)&buffer;
 SA = (StructRecvAccount*)&buffer;
 RGI = (RecvGameInfo *)&buffer;
 
 //SetTimer(p_index);
 
 memset(&buffer, 0, sizeof(buffer));
 BytesRecv = c_recv(p_index, buffer, sizeof(StructRecvAccount));
 if(BytesRecv == -1 || BytesRecv == 0)
  typewait = false;
 
 if(typewait)
  MCA[p_index].AccountUID = LoginAccept(&SQLConnect, SA->Login, SA->Password);
  
 if(MCA[p_index].AccountUID == -2)
 {
  typewait = false;
  SendInfo(p_index, 0x01); // Send 'Conta já Logada'
 }
 else if(MCA[p_index].AccountUID == -1)
 {
  typewait = false;
  SendInfo(p_index, 0x02); // Send 'Conta Banida'
 }
 else if(MCA[p_index].AccountUID == 0)
 {
  typewait = false;
  SendInfo(p_index, 0x04); // Send 'Não Existe'
 }
 else 
 {
  OfflineAccept = true;
  SendInfo(p_index, 0x05);
  SendAccountInfo(&MCA[p_index], p_index, &SQLConnect);
  
  MCA[p_index].direction = 1;
 
  CGuild.SGuildInfo(p_index, &SQLConnect, &MCA[p_index]);
  CParty.SPartyInfo(p_index, &SQLConnect, &MCA[p_index]);
  CPlayers.PlayerOnline(p_index, MCA, &SQLConnect);
  CPlayers.SendAllPlayersOnline(p_index, MCA, &SQLConnect);
  SendAllMonsters(p_index, MCA, SMonsters);
  MCA[p_index].ActiveSendAndRecv = true;
 }
 
 while(typewait)
 {
  memset(&buffer, 0, sizeof(buffer));
  BytesRecv = c_recv(p_index, buffer, sizeof(TypeInfoPacket));
  if(BytesRecv == -1 || BytesRecv == 0)
   typewait = false;
   
  // Andar
  if(SI->type_one == 0x00 && SI->type_two == 0x02) // Walk
  {
   BytesRecv = c_recv(p_index, buffer, sizeof(RecvGameInfo));
   if(RGI->Direction == 0x01)
    MCA[p_index].Player_y -= 32;
   else if(RGI->Direction == 0x02)
    MCA[p_index].Player_y += 32;
   else if(RGI->Direction == 0x03)
    MCA[p_index].Player_x -= 32;
   else if(RGI->Direction == 0x04)
    MCA[p_index].Player_x += 32;
   
   MCA[p_index].direction = RGI->Direction; 
   
   if(RGI->Direction > 0)
   {
    CPlayers.SendAllMove(p_index, MCA);
    // send all players
   }
  }
  else if(SI->type_one == 0x00 && SI->type_two == 0x03) // Info da Guild
   CGuild.SGuildInfo(p_index, &SQLConnect, &MCA[p_index]);
  else if(SI->type_one == 0x00 && SI->type_two == 0x04) // Info da Party
   CParty.SPartyInfo(p_index, &SQLConnect, &MCA[p_index]);
  else if(SI->type_one == 0x00 && SI->type_two == 0x20) // Attack Z (Ataque normal)
  {
   AttackMonster(p_index, SMonsters, MCA);
   // AttackPlayers ...
  }
  else if(SI->type_one == 0x01 && SI->type_two == 0x20) // Attack X (Ataque de Skill)
  {
  }
  else if(SI->type_one == 0x01 && SI->type_two == 0x19) // Recv Coord Player
   SendCoord(p_index, &MCA[p_index]);
  else // Packet Invalid
   typewait = false;
   
  Sleep(1);
 }
 
 // Offline apenas se a pessoa sair normalmente, caso contrario continua conectada!!!
 if(OfflineAccept)
  AccountOffline(MCA[p_index].AccountUID, &SQLConnect);
 
 if(MCA[p_index].AccountUID >= 1)
 {
  CPlayers.PlayerOffline(p_index, MCA);
  SaveAccountDB(&MCA[p_index], &SQLConnect);
 }
 
 mysql_close(&SQLConnect);
 close_socket(p_index);
 MCA[p_index].ActiveSendAndRecv = false;
 printf("Player [%d] - Desconectado\n", p_index);
}

void OpenIni()
{
 int i;
 char PATH_FILE[FILENAME_MAX];
 char PATH_FILE_TMP[FILENAME_MAX];
 
 GetModuleFileName(0, PATH_FILE, FILENAME_MAX);
 
 i = strlen(PATH_FILE);
 
 for(i; i > 0; i--)
 {
  if(PATH_FILE[i] == '\\')
  {
   break;
  }
 }
 
 strncpy(PATH_FILE_TMP, PATH_FILE, i+1);
 PATH_FILE_TMP[i+1] = '\0';
 strcat(PATH_FILE_TMP, "config.ini");
 
 port = GetPrivateProfileInt("SERVERCONFIG", "PORT", 9000, PATH_FILE_TMP);
 GetPrivateProfileString("SERVERCONFIG", "IP", "127.0.0.1", ip, sizeof(ip), PATH_FILE_TMP);
 GetPrivateProfileString("SERVERCONFIG", "SQLUSER", "root", sqluser, sizeof(sqluser), PATH_FILE_TMP);
 GetPrivateProfileString("SERVERCONFIG", "SQLPASS", "", sqlpass, sizeof(sqlpass), PATH_FILE_TMP);
 GetPrivateProfileString("SERVERCONFIG", "SQLDB", "OGEngine", sql_db, sizeof(sqlpass), PATH_FILE_TMP);
}

DWORD ThreadMonsters()
{
 srand (time(NULL));
 InitMonsters(SMonsters, "Map/Monsters.OG");
 while(true)
 {
  for(int i = 0; i < MAX_MONSTERS_SERVER; i++)
  {
   if(SMonsters[i].MonsterUID >= 1)
   {
    if(SMonsters[i].RealTime <= 0 && SMonsters[i].ActiveMonster == true)
    {
     //printf("New Monster [%d]\n", i);
     SMonsters[i].RealTime = SMonsters[i].ReviveTime;    // Coloca o RealTime igual ao ReviveTime novamente
     SMonsters[i].HP = SMonsters[i].MAX_HP;
     SMonsters[i].ActiveMonster = 0;                     // Revive o monstro
     // (Function) Reviver Monstro ...
     SendMonster(&SMonsters[i], MCA);
     //printf("Send Monster [%d] para os clients\n", SMonsters[i].MonsterUID);
    }
    else if(SMonsters[i].ActiveMonster == true)
    {
     // RealTime -> é o tempo que leva para reviver o monstro
     //printf("RealTime [%d][%d]\n", i, SMonsters[i].RealTime);
     SMonsters[i].RealTime--;
    }
    else if(SMonsters[i].HP >= 1)
    {
     CheckAndMoveMonsters(&SMonsters[i], MCA);
    }
   }
  }
  Sleep(1000);
 }
}

DWORD ThreadGeral()
{
 MYSQL SQLConnect;
 
 mysql_init(&SQLConnect);
 mysql_real_connect(&SQLConnect, ip, sqluser, sqlpass, sql_db,  0, NULL, 0);
 
 while(true)
 {
  for(int i = 0; i < MAX_PLAYER; i++)
  {
   if(MCA[i].ActiveSendAndRecv == true)
   {
    // Revive Player com HP menor que zero ...
    if(MCA[i].hp <= 0)
    {
     MCA[i].ActiveSendAndRecv = false;
     ResetMonsters(i);
     CPlayers.ResetPlayers(i);
    
     CPlayers.PlayerOffline(i, MCA);
    
     // Onde o personagem deve reviver ...
     MCA[i].Player_y = 32*12;
     MCA[i].Player_x = 32*4;
     MCA[i].MapUID = 1;
     MCA[i].direction = 1;
     MCA[i].hp = MCA[i].max_hp;
     MCA[i].mp = MCA[i].max_mp;
     
     SendCoord(i, &MCA[i]);
     SendInfoToPlayerIndex(&MCA[i], "Voce morreu!", i);
     SendMap(i, &MCA[i], &SQLConnect);
     CPlayers.PlayerOnline(i, MCA, &SQLConnect);
     CPlayers.SendAllPlayersOnline(i, MCA, &SQLConnect);
     SendAllMonsters(i, MCA, SMonsters);
    
     MCA[i].ActiveSendAndRecv = true;
    }
   }
  }
 Sleep(100);
 }
}
