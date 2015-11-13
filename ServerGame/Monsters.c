#include "Monsters.h"

void InitMonsters(StructMonsters *SM, char *file)
{
 int counts = 0;
 FILE *fmonsters = fopen(file, "rb+");
 if(fmonsters == NULL)
 {
  printf("Impossivel encontrar [%s]", file);
  exit(1);
 }
 rewind(fmonsters);
 fseek(fmonsters, 0, SEEK_SET);
 while(true)
 {
  fscanf(fmonsters, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %s",
                                                                &SM[counts].Level, 
                                                                &SM[counts].MapUID,
                                                                &SM[counts].MonsterType,
                                                                &SM[counts].MAX_HP,
                                                                &SM[counts].ATK,
                                                                &SM[counts].DEF,
                                                                &SM[counts].Map_X,
                                                                &SM[counts].Map_Y,
                                                                &SM[counts].Rect_X,
                                                                &SM[counts].Rect_Y,
                                                                &SM[counts].ItemDropUID,
                                                                &SM[counts].EXP,
                                                                &SM[counts].ReviveTime,
                                                                &SM[counts].MoneyDrop,
                                                                &SM[counts].MonsterName);
  if(SM[counts].MapUID == 0 || counts >= MAX_MONSTERS_SERVER)
   break;
   
  SM[counts].MonsterUID = counts+1;
  SM[counts].CopyMap_X = SM[counts].Map_X;
  SM[counts].CopyMap_Y = SM[counts].Map_Y;    
   
  SM[counts].HP = SM[counts].MAX_HP;                                
  SM[counts].RealTime = SM[counts].ReviveTime;
  SM[counts].ActiveMonster = 0;
   
  counts++;
 }
 printf("[%d] MONSTROS CARREGADOS ...\n", counts);
 fclose(fmonsters);
}

void SendAllMonsters(unsigned int p_index, MyClassAccount *MCA, StructMonsters *SM)
{
 char buffer[sizeof(StructSendCreateMonsters)];
 StructSendCreateMonsters *SSCM = (StructSendCreateMonsters*)&buffer;
 
 SSCM->type_one = 0x05;
 SSCM->type_two = 0x01;
   
 int counts = 0;
 for(;;)
 {
  if(SM[counts].MonsterUID == 0)
   break;
   
  if(SM[counts].MapUID == MCA[p_index].MapUID && SM[counts].HP >= 1)
  {
   SSCM->ATK = SM[counts].ATK;
   SSCM->DEF = SM[counts].DEF;
   SSCM->HP = SM[counts].HP;
   SSCM->Map_X = SM[counts].Map_X*32;
   SSCM->Map_Y = SM[counts].Map_Y*32;
   SSCM->MonsterUID = SM[counts].MonsterUID;
   SSCM->MonsterType = SM[counts].MonsterType;
   SSCM->Level = SM[counts].Level;
   strncpy(SSCM->MonsterName, SM[counts].MonsterName, 16);
   
   s_send(p_index, buffer, sizeof(StructSendCreateMonsters));
  }
  counts++;
 }
}

void SendMonster(StructMonsters *SM, MyClassAccount *MCA)
{
 char buffer[sizeof(StructSendCreateMonsters)];
 StructSendCreateMonsters *SSCM = (StructSendCreateMonsters*)&buffer;
 
 SSCM->type_one = 0x05;
 SSCM->type_two = 0x01;
 SSCM->ATK = SM->ATK;
 SSCM->DEF = SM->DEF;
 SSCM->HP = SM->HP;
 SSCM->Map_X = SM->Map_X*32;
 SSCM->Map_Y = SM->Map_Y*32;
 SSCM->MonsterUID = SM->MonsterUID;
 SSCM->MonsterType = SM->MonsterType;
 SSCM->Level = SM->Level;
 strncpy(SSCM->MonsterName, SM->MonsterName, 16);
    
 for(int i = 0; i < MAX_PLAYER; i++)
 {
  if(MCA[i].ActiveSendAndRecv == 1)
  {   
   if(SM->MapUID == MCA[i].MapUID && SM->HP >= 1)
   {
    s_send(i, buffer, sizeof(StructSendCreateMonsters));
   }
  }
 }
}

void FollowMonsterPlayerRect(StructMonsters *SM, MyClassAccount *MCA, unsigned int player_id)
{
 char buffer[sizeof(StructSendMoveMonsters)];
 StructSendMoveMonsters *SSMM = (StructSendMoveMonsters*)&buffer;
 
 /*printf("X [%d] [%d]\n", SM->Map_X, MCA[player_id].Player_x/32);
 printf("Y [%d] [%d]\n", SM->Map_Y, MCA[player_id].Player_y/32);*/
 
 if(SM->Map_X-1 > MCA[player_id].Player_x/32){
  SSMM->Direction = 3;
  SM->Map_X -= 1;
 }
 else if(SM->Map_X+1 < MCA[player_id].Player_x/32){
  SSMM->Direction = 4;
  SM->Map_X += 1;
 }
 else if(SM->Map_Y-1 > MCA[player_id].Player_y/32){
  SSMM->Direction = 1;
  SM->Map_Y -= 1;
 }
 else if(SM->Map_Y+1 < MCA[player_id].Player_y/32){
  SSMM->Direction = 2;
  SM->Map_Y += 1;
 }
 else{
  char LogMessage[127];
  memset(LogMessage, 0, sizeof(LogMessage));
  if(MCA[player_id].s_def > SM->ATK)
  {
   MCA[player_id].hp--;
   sprintf(LogMessage, "[%s] atacou voce, voce perdeu [1] de HP", SM->MonsterName);
  }
  else
  {
   MCA[player_id].hp -= (SM->ATK - MCA[player_id].s_def);
   
   if(MCA[player_id].hp <= 0)
    MCA[player_id].hp = 0;
    
   sprintf(LogMessage, "[%s] atacou voce, voce perdeu [%d] de HP", SM->MonsterName, SM->ATK - MCA[player_id].s_def);
  }
  
  SendInfoToPlayerIndex(&MCA[player_id], LogMessage, player_id);
 }
 
 SSMM->type_one = 0x05;
 SSMM->type_two = 0x03;
 SSMM->MonsterUID = SM->MonsterUID;
  
 for(int i = 0; i < MAX_PLAYER; i++)
 {
  if(MCA[i].ActiveSendAndRecv && MCA[i].MapUID == SM->MapUID)
  {
   s_send(i, buffer, sizeof(StructSendMoveMonsters));
  }
 }
}

void CheckAndMoveMonsters(StructMonsters *SM, MyClassAccount *MCA)
{
 bool TestColision = false;
 for(int i = 0; i < MAX_PLAYER; i++)
 {
  if(MCA[i].ActiveSendAndRecv && MCA[i].MapUID == SM->MapUID && MCA[i].hp >= 1)
  {
   if(MCA[i].Player_x/32 >= SM->CopyMap_X && MCA[i].Player_x/32 <= SM->CopyMap_X+SM->Rect_X-1 && MCA[i].Player_y/32 >= SM->CopyMap_Y && MCA[i].Player_y/32 <= SM->CopyMap_Y+SM->Rect_Y-1)
   {
    //printf("Colision [%d] [%d] [%d]!!!\n", i, MCA[i].Player_x/32, SM->CopyMap_X);
    // Follow Player que colidiu
    if(SM->PlayerUIDFollow == 0)
     SM->PlayerUIDFollow = MCA[i].AccountUID;
     
    if(SM->PlayerUIDFollow == MCA[i].AccountUID)
    {
     //printf("Follow Player UID [%d]\n", MCA[i].AccountUID);
     FollowMonsterPlayerRect(SM, MCA, i);
     TestColision = true;
     break;
    }
   }
  }
 }
 
 if(TestColision == false)
 {
  SM->PlayerUIDFollow = 0;
  // Rand Moviment !!!
  char buffer[sizeof(StructSendMoveMonsters)];
  StructSendMoveMonsters *SSMM = (StructSendMoveMonsters*)&buffer;
  SSMM->type_one = 0x05;
  SSMM->type_two = 0x03;
  SSMM->MonsterUID = SM->MonsterUID;
  
  if(rand() % 2 == 0)
  {
   if(rand() % 2 == 0)
   {
    if(rand() % 2 == 0)
    {   
     if(SM->Map_X < SM->CopyMap_X+SM->Rect_X-1)
     {
      SSMM->Direction = 4;
      SM->Map_X += 1;
     }
     else if(SM->Map_X >= SM->CopyMap_X+1)
     {
      SSMM->Direction = 3;
      SM->Map_X -= 1;
     }
    }
    else
    {
     if(SM->Map_X >= SM->CopyMap_X+1)
     {
      SSMM->Direction = 3;
      SM->Map_X -= 1;
     }
     else
     {
      if(SM->Map_X < SM->CopyMap_X+SM->Rect_X-1)
      {
       SSMM->Direction = 4;
       SM->Map_X += 1;
      }
     }
    }
   }
   else
   {
    if(rand() % 2 == 0)
    { 
     if(SM->Map_Y < SM->CopyMap_Y+SM->Rect_Y-1)
     {
      SSMM->Direction = 2;
      SM->Map_Y += 1;
     }
     else if(SM->Map_Y >= SM->CopyMap_Y+1)
     {
      SSMM->Direction = 1;
      SM->Map_Y -= 1;
     }
    }
    else
    {
     if(SM->Map_Y >= SM->CopyMap_Y+1)
     {
      SSMM->Direction = 1;
      SM->Map_Y -= 1;
     }
     else if(SM->Map_Y < SM->CopyMap_Y+SM->Rect_Y-1)
     {
      SSMM->Direction = 2;
      SM->Map_Y += 1;
     }
    }
   }
   
   //printf("MonsterUID [%d] Move\n", SM->MonsterUID);
   for(int i = 0; i < MAX_PLAYER; i++)
   {
    if(MCA[i].ActiveSendAndRecv && MCA[i].MapUID == SM->MapUID)
    {
     s_send(i, buffer, sizeof(StructSendMoveMonsters));
    }
   }
  }
 }
}

void AttackMonster(unsigned int p_index, StructMonsters *SM, MyClassAccount *MCA)
{
 bool AttackInformation = false;
 int monster_id;
 for(monster_id = 0; monster_id < MAX_MONSTERS_SERVER; monster_id++)
 {
  if(MCA[p_index].MapUID == SM[monster_id].MapUID && SM[monster_id].HP >= 1)
  {
   if(MCA[p_index].Player_y/32 == SM[monster_id].Map_Y)
   {
    if(MCA[p_index].Player_x/32 == SM[monster_id].Map_X+1 || MCA[p_index].Player_x/32 == SM[monster_id].Map_X-1 || MCA[p_index].Player_x/32 == SM[monster_id].Map_X)
     AttackInformation = true;
   }
   else if(MCA[p_index].Player_x/32 == SM[monster_id].Map_X)
   {
    if(MCA[p_index].Player_y/32 == SM[monster_id].Map_Y+1 || MCA[p_index].Player_y/32 == SM[monster_id].Map_Y-1 || MCA[p_index].Player_y/32 == SM[monster_id].Map_Y)
     AttackInformation = true;
   }
   if(AttackInformation)
    break;
  }
 }
 if(AttackInformation)
 {
  char LogMessage[127];
  if(SM[monster_id].DEF < MCA[p_index].s_str)
  {
   SM[monster_id].HP -= MCA[p_index].s_str - SM[monster_id].DEF;
   sprintf(LogMessage, "voce atacou [%s] e tirou [%d] de HP", SM[monster_id].MonsterName, MCA[p_index].s_str - SM[monster_id].DEF);
  }
  else
  {
   SM[monster_id].HP -= 1;
   sprintf(LogMessage, "voce atacou [%s] e tirou [1] de HP", SM[monster_id].MonsterName);
  }
  
  if(SM[monster_id].HP <= 0)
  {
   MonsterKill(&SM[monster_id], MCA);
   SM[monster_id].ActiveMonster = 1;
   sprintf(LogMessage, "voce atacou [%s] e matou - exp ganho [%d]", SM[monster_id].MonsterName, SM[monster_id].EXP);
   MCA[p_index].exp += SM[monster_id].EXP;
   MCA[p_index].money  += SM[monster_id].MoneyDrop;
   if(MCA[p_index].exp >= MCA[p_index].max_exp)
   {
     int tmp_max_exp = MCA[p_index].max_exp;
     MCA[p_index].points += (MCA[p_index].exp/MCA[p_index].max_exp)*2;
     MCA[p_index].level += MCA[p_index].exp/MCA[p_index].max_exp;
     MCA[p_index].max_exp += (MCA[p_index].exp/MCA[p_index].max_exp)*60;
     MCA[p_index].exp = (MCA[p_index].exp - ((MCA[p_index].exp/tmp_max_exp)*tmp_max_exp));
   }
  }
  else
   SendInfoMonsters(&SM[monster_id]);
  SendInfoToPlayerIndex(&MCA[p_index], LogMessage, p_index);
 }
}

void MonsterKill(StructMonsters *SM, MyClassAccount *MCA)
{
 char buffer[sizeof(StructSendDeleteMonsters)];
 StructSendDeleteMonsters *SSDM = (StructSendDeleteMonsters*)&buffer;
 SSDM->type_one = 0x05;
 SSDM->type_two = 0x02;
 SSDM->MonsterUID = SM->MonsterUID;
 
 SM->Map_X = SM->CopyMap_X;
 SM->Map_Y = SM->CopyMap_Y;
 
 for(int i = 0; i < MAX_PLAYER; i++)
 {
  if(MCA[i].ActiveSendAndRecv == 1)
  {   
   if(SM->MapUID == MCA[i].MapUID)
   {
    s_send(i, buffer, sizeof(StructSendDeleteMonsters));
   }
  }
 }
}

void ResetMonsters(unsigned int p_index)
{
 char buffer[sizeof(TypeInfoPacket)];
 TypeInfoPacket *TIP = (TypeInfoPacket *)&buffer;
 TIP->type_one = 0x04;
 TIP->type_two = 0x05;
 s_send(p_index, buffer, sizeof(TypeInfoPacket));
}

void SendInfoMonsters(StructMonsters*SM)
{
 char buffer[sizeof(RecvInfoMonsters)];
 RecvInfoMonsters *RIM = (RecvInfoMonsters *)&buffer;
 
 RIM->type_one = 0x20;
 RIM->type_two = 0x03;
 
 for(int i = 0; i < MAX_PLAYER; i++)
 {
  RIM->HP = SM->HP;
  RIM->MonsterUID = SM->MonsterUID;
  s_send(i, buffer, sizeof(RecvInfoMonsters));
 }
}
