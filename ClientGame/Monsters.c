#include "Monsters.h"

ClassMonsters::ClassMonsters()
{
}

ClassMonsters::~ClassMonsters()
{
 for(int x = 0; x < MAX_PACKSPRITE; x++)
 {
  for(int y = 0; y < 16; y++)
  {
   if(SpriteMonsters[x][y] != NULL)
    destroy_bitmap(SpriteMonsters[x][y]);
  }
 }
}

void ClassMonsters::InitMonsters()
{
 this->count_sprite = 0;
 RGM.pRGM = NULL;
 
 for(int x = 0; x < MAX_PACKSPRITE; x++)
 {
  for(int y = 0; y < 16; y++)
   SpriteMonsters[x][y] = NULL;
 }
 
 char buffer[127];
 
 unsigned int number_of_sprite = GetPrivateProfileInt("SpriteInfo", "SpriteNumber", 1, "Data/MonsterSprite.ini");
 
 for(int i = 1; i <= number_of_sprite; i++)
 {
  sprintf(buffer, "SpriteMonster%0.3d", i);
  GetPrivateProfileString("MonsterSprite", buffer, "nofound", buffer, sizeof(buffer), "Data/MonsterSprite.ini");
  LoadingSpriteMonsters(buffer);
 }
 
 ResetMonsters();
 RGM.MonsterType = 0;
 RGM.MonsterUID = 0;
 
 // Test
 /*char buffer[1024];
 RecvGameMonsters *pMonsters = (RecvGameMonsters *)&buffer;
 pMonsters->MonsterID = 1;
 pMonsters->MonsterType = 0;
 strncpy(pMonsters->MonsterName, "Monsters", 16);
 pMonsters->CoordX = 192;
 pMonsters->CoordY = 384;
 pMonsters->Direction = 0;
 FuncRecvCreateMonsters(buffer);*/
}

void ClassMonsters::LoadingSpriteMonsters(char *filename)
{
 BITMAP *tmp = load_bitmap(filename, NULL);
 
 if(tmp == NULL)
 {
  allegro_message("Impossivel carregar a SPRITE [%s]", filename);
  exit(1);
 }

 tx = tmp->w / 4;
 ty = tmp->h / 4;
 int xtiles = tmp->w / tx;
 int ytiles = tmp->h / ty;

 int p_tmp = 0;
 for(int y = 0; y < ytiles; y++)
 {
  for(int x = 0; x < xtiles; x++)
  {
   SpriteMonsters[count_sprite][p_tmp] = create_bitmap(tx, ty);
   blit(tmp, SpriteMonsters[count_sprite][p_tmp], x*tx, y*ty, 0, 0, tx, ty);  
   p_tmp++;
  }
 }
 
 count_sprite++;
 destroy_bitmap(tmp);
}

void ClassMonsters::MoveDirectionMonsters()
{
 RecvGameMonsters *pList = &RGM;
 for(;;)
 {
  if(pList->pRGM == NULL)
   break;
   
  pList = pList->pRGM;
  
  if(pList->Direction == 1 && pList->MovY > 0)
  {
   pList->CoordY -= COORD_ADD;
   pList->MovY--;
  }
  else if(pList->Direction == 2 && pList->MovY > 0)
  {
   pList->CoordY += COORD_ADD;
   pList->MovY--;
  }
  else if(pList->Direction == 3 && pList->MovX > 0)
  {
   pList->CoordX -= COORD_ADD;
   pList->MovX--;
  }
  else if(pList->Direction == 4 && pList->MovX > 0)
  {
   pList->CoordX += COORD_ADD;
   pList->MovX--;
  }
  else
  {
   pList->Direction = 0;
  }
 }
}

bool ClassMonsters::ColisionMonsters(int *Player_X, int *Player_Y)
{
 RecvGameMonsters *pList = &RGM;
 
 for(;;)
 {
  if(pList->pRGM == NULL)
   break;
      
  pList = pList->pRGM;
  
  if(*Player_X+31 >= pList->CoordX && *Player_X <= pList->CoordX+31)
  {
   if(*Player_Y+31 >= pList->CoordY && *Player_Y <= pList->CoordY+31)
   {
    return true;
   }
  }
 }
 return false;
}
       
void ClassMonsters::FuncRecvCreateMonsters(char *buffer)
{
 RecvGameMonsters *pList = &RGM;
 StructSendCreateMonsters *Packet = (StructSendCreateMonsters *)buffer;
 
 //printf("MonsterUID [%d] Create\n", Packet->MonsterUID);
 
 int index = 0;
 for(;;)
 {
  if(pList->pRGM == NULL)
   break;
      
   pList = pList->pRGM;
   index++;
  }
  
  pList->pRGM = new RecvGameMonsters;
  pList = pList->pRGM;
  
  pList->MonsterUID = Packet->MonsterUID;
  pList->MonsterType = Packet->MonsterType;
  pList->Level =  Packet->Level;
  pList->hp = Packet->HP;
  strncpy(pList->MonsterName, Packet->MonsterName, 16);
  
  pList->CoordX = Packet->Map_X;
  pList->CoordY = Packet->Map_Y;
  pList->Direction = 2;
  pList->SaveMoviment = 0;
  
  pList->Moviment = 0;
  
  /*if(Packet->Direction == 0x01)
   pList->SaveMoviment = 12;
  else if(Packet->Direction == 0x02)
   pList->SaveMoviment = 0;
  else if(Packet->Direction == 0x03)
   pList->SaveMoviment = 4;
  else if(Packet->Direction == 0x04)
   pList->SaveMoviment = 8;*/
  
  pList->MovX = 0;
  pList->MovY = 0;
  
  pList->pRGM = NULL;
}

void ClassMonsters::FuncRecvDeleteMonsters(char *buffer)
{
 RecvDeleteMonsters *RDM = (RecvDeleteMonsters *)buffer;
 DeleteMonsters(RDM->MonsterID);
}

void ClassMonsters::DeleteMonsters(int MonsterUID)
{
 RecvGameMonsters *pList = &RGM;
 RecvGameMonsters *pOldList = &RGM;
    
 for(;;)
 {
  if(pList->pRGM == NULL)
   break;
      
  pList = pList->pRGM;
  
  if(pList->MonsterUID == MonsterUID)
   break;
  pOldList = pList;
 }
 
 pOldList->pRGM = pList->pRGM;
 free(pList);
}

void ClassMonsters::FuncMoveMonsters(char *buffer)
{
 RecvGameMonsters *pList = &RGM;
 RecvMoveMonsters *RMM = (RecvMoveMonsters *)buffer;
 for(;;)
 {
  if(pList->pRGM == NULL)
   break;
      
  pList = pList->pRGM;
  
  if(pList->MonsterUID == RMM->MonsterUID)
  {
   while(true)
   {
    if(pList->MovX <= 0 && pList->MovY <= 0)
    {
     if(RMM->Direction == 0x01)
     {
      pList->Direction = RMM->Direction;
      pList->MovY = 32/COORD_ADD;
      pList->Moviment = 12;
     }
     else if(RMM->Direction == 0x02)
     {
      pList->Direction = RMM->Direction;
      pList->MovY = 32/COORD_ADD;
      pList->Moviment = 0;
     }
     else if(RMM->Direction == 0x03)
     {
      pList->Direction = RMM->Direction;
      pList->MovX = 32/COORD_ADD;
      pList->Moviment = 4;
     }
     else if(RMM->Direction == 0x04)
     {
      pList->Direction = RMM->Direction;
      pList->MovX = 32/COORD_ADD;
      pList->Moviment = 8;
     }
     RMM->Direction = 0;
     break;
    }
   }
  }
  
  if(pList->MonsterUID == RMM->MonsterUID)
   break;
 }
}

void ClassMonsters::FuncRecvInfoMonsters(char *buffer)
{
 RecvGameMonsters *pList = &RGM;
 RecvInfoMonsters *RIM = (RecvInfoMonsters *)buffer;
 for(;;)
 {
  if(pList->pRGM == NULL)
   break;
      
  pList = pList->pRGM;
  
  if(pList->MonsterUID == RIM->MonsterUID)
  {
   pList->hp = RIM->HP;
   break;
  }
 }
}
             
bool ClassMonsters::ResetMonsters()
{
 RecvGameMonsters *pList = &RGM;
 RecvGameMonsters *pOldList = &RGM;
    
 for(;;)
 {
  if(pList->pRGM == NULL)
   return 0;
      
  pList = pList->pRGM;
 
  pOldList->pRGM = pList->pRGM;
  free(pList);
 }
}

void ClassMonsters::DrawMonsters(BITMAP *GameBuffer)
{
 char buffer[127];
 
 RecvGameMonsters *pList = &RGM;
 MoveDirectionMonsters();
 
 for(;;)
 {
  if(pList->pRGM == NULL)
   break;
   
  pList = pList->pRGM;
  
  if(pList->Direction == 1)
  {
   pList->SaveMoviment = 12;
   if(pList->Moviment >= pList->SaveMoviment+3)
    pList->Moviment = pList->SaveMoviment;
   else
    pList->Moviment++;
  }
  else if(pList->Direction == 2)
  {
   pList->SaveMoviment = 0;
   if(pList->Moviment >= pList->SaveMoviment+3)
    pList->Moviment = pList->SaveMoviment;
   else
    pList->Moviment++;
  }
  else if(pList->Direction == 3)
  {
   pList->SaveMoviment = 4;
   if(pList->Moviment >= pList->SaveMoviment+3)
    pList->Moviment = pList->SaveMoviment;
   else
    pList->Moviment++;
  }
  else if(pList->Direction == 4)
  {
   pList->SaveMoviment = 8;
   if(pList->Moviment >= pList->SaveMoviment+3)
    pList->Moviment = pList->SaveMoviment;
   else
    pList->Moviment++;
  }
  else
  {
   pList->Moviment = pList->SaveMoviment;
  }
  
  sprintf(buffer, "%s Lv[%d] Hp[%d]", pList->MonsterName, pList->Level, pList->hp);
  
  draw_sprite(GameBuffer, SpriteMonsters[pList->MonsterType][pList->Moviment], pList->CoordX-((tx-32)/2), pList->CoordY-((ty-32)/2));
  if(pList->MonsterName[0] != 0x00)
   textprintf_ex(GameBuffer, font, pList->CoordX-(text_length(font, buffer)/2)+20, pList->CoordY+ty+2, makecol(210, 210, 0), -1, "%s", buffer);
  //textprintf_ex(GameBuffer, font, pList->CoordX-(text_length(font, buffer)/2)+(32/2), pList->CoordY+ty+12, makecol(210, 210, 0), -1, "Lv%d [%d]", pList->Level, pList->hp);
 }
}
