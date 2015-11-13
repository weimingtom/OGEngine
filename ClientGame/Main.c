#include "Library.h"
#include "OGEngine.h"

#include <conio.h>

ClassOGEngine *pOGEngine;

char login[16];
char senha[16];

int main() 
{    
    ClassOGEngine OGEngine(ALTURA, LARGURA);
    
    /*Deixa um ponteiro para o main global*/
    pOGEngine = &OGEngine;
    
    printf("Login ");
    scanf("%16s", login);
    printf("Senha ");
    scanf("%16s", senha);
    OGEngine.LoginInfo(login, senha);
    
    /* Inicia o jogo */
    OGEngine.MainGame();
	return 0;
}
END_OF_MAIN();

/* Contagem dos frames */
void frame_rate()
{
 pOGEngine->old_fps = pOGEngine->new_fps;
 pOGEngine->new_fps = 0;
}

void fps_increment()
{
 pOGEngine->fps_speed++;
}

/* Thread principal do recv */
DWORD RecvThread()
{
 int RecvBytes = 0;
 bool ThreadFlagsAction = true;
 char buffer[1024];
 TypeInfoPacket *TIP = (TypeInfoPacket*)&buffer;
 
 while(ThreadFlagsAction)
 {
  memset(buffer, 0, sizeof(buffer));
  RecvBytes = pOGEngine->RecvPacket(buffer, sizeof(TypeInfoPacket));
  
  if(RecvBytes == -1 || RecvBytes == 0)
  {
   allegro_message("Voce foi desconectado.");
   ThreadFlagsAction = false;
   pOGEngine->Stat = STATEOFFLINE;
  }
  
  if(TIP->type_one == 0x00 && TIP->type_two == 0x03) // Recv Guild Info
  {
   pOGEngine->RecvPacket(buffer, sizeof(StructRecvGuildInfo));
   pOGEngine->Player.PacketRecvGuildInfo(buffer);
  }
  else if(TIP->type_one == 0x00 && TIP->type_two == 0x04) // Recv Party Info
  {
   pOGEngine->RecvPacket(buffer, sizeof(StructRecvPartyInfo));
   pOGEngine->Player.PacketRecvPartyInfo(buffer);
  }
  else if(TIP->type_one == 0x01 && TIP->type_two == 0x05) // Inicia um Player
  {
   pOGEngine->RecvPacket(buffer, sizeof(RecvGamePlayers));
   pOGEngine->Players.FuncRecvGamePlayers(buffer);
  }
  else if(TIP->type_one == 0x02 && TIP->type_two == 0x05) // Move um Player
  {
   pOGEngine->RecvPacket(buffer, sizeof(RecvPlayersDirection));
   pOGEngine->Players.FuncRecvMovePlayer(buffer);
  }
  else if(TIP->type_one == 0x03 && TIP->type_two == 0x05) // Delete Player
  {
   pOGEngine->RecvPacket(buffer, sizeof(RecvRemovePlayer));
   pOGEngine->Players.FuncRemovePlayer(buffer);
  }
  else if(TIP->type_one == 0x04 && TIP->type_two == 0x05) // Reset Player
  {
   pOGEngine->Players.ResetPlayers();
  }
  // Monster
  else if(TIP->type_one == 0x05 && TIP->type_two == 0x01) // Create Monster
  {
   pOGEngine->RecvPacket(buffer, sizeof(StructSendCreateMonsters));
   pOGEngine->CMonsters.FuncRecvCreateMonsters(buffer);
  }
  else if(TIP->type_one == 0x05 && TIP->type_two == 0x02) // Remove Monster
  {
   pOGEngine->RecvPacket(buffer, sizeof(RecvDeleteMonsters));
   pOGEngine->CMonsters.FuncRecvDeleteMonsters(buffer);
  }
  else if(TIP->type_one == 0x05 && TIP->type_two == 0x03) // Move Monster
  {
   pOGEngine->RecvPacket(buffer, sizeof(RecvMoveMonsters));
   pOGEngine->CMonsters.FuncMoveMonsters(buffer);
  }
  else if(TIP->type_one == 0x05 && TIP->type_two == 0x04) // Reset Monster
  {
   pOGEngine->CMonsters.ResetMonsters();
  }
  else if(TIP->type_one == 0x06 && TIP->type_two == 0x00)
  {
   pOGEngine->RecvPacket(buffer, sizeof(StructSendInfoPlayer));
   pOGEngine->Player.PacketRecvAllInfo(buffer);
  }
  else if(TIP->type_one == 0x06 && TIP->type_two == 0x01) // Recv Map
  {
   pOGEngine->Map.StatRecvMap(pOGEngine->GetSocket());
  }
  else if(TIP->type_one == 0x06 && TIP->type_two == 0x02) // Recv Player Coord
  {
   pOGEngine->RecvPacket(buffer, sizeof(RecvCoord));
   pOGEngine->Player.PacketRecvCoord(buffer);
  }
  else if(TIP->type_one == 0x20 && TIP->type_two == 0x03) // Recv Info Monsters
  {
   pOGEngine->RecvPacket(buffer, sizeof(RecvInfoMonsters));
   pOGEngine->CMonsters.FuncRecvInfoMonsters(buffer);
  }
  Sleep(1);
 }
}
