#include "Players.h"

ClassPlayers::ClassPlayers()
{
}

ClassPlayers::~ClassPlayers()
{
 for(int i = 0; i < 16; i++)
 {
  destroy_bitmap(Class01[i]);
  destroy_bitmap(Class02[i]);
  destroy_bitmap(Class03[i]);
  destroy_bitmap(Class04[i]);
 }
}

/* Inicia os players com as informações prè-definida */      
void ClassPlayers::InitPlayers()
{ 
 ResetPlayers();
 
 ClassType = (BITMAP**)&Class01;
 LoadingSpritePlayer("Sprite/Class01.bmp");
 
 ClassType = (BITMAP**)&Class02;
 LoadingSpritePlayer("Sprite/Class02.bmp");
 
 ClassType = (BITMAP**)&Class03;
 LoadingSpritePlayer("Sprite/Class03.bmp");
 
 ClassType = (BITMAP**)&Class04;
 LoadingSpritePlayer("Sprite/Class04.bmp");
}

/* Carrega as sprites dos players */
void ClassPlayers::LoadingSpritePlayer(char *filename)
{
 BITMAP *tmp = load_bitmap(filename, NULL);

 for(int i = 0; i < 16; i++)
  ClassType[i] = NULL;
 
 if(tmp == NULL)
 {
  allegro_message("Impossivel carregar a SPRITE [%s]", filename);
  exit(1);
 }

 int xtiles = tmp->w / PLAYER_W;
 int ytiles = tmp->h / PLAYER_H;

 int p_tmp = 0;
 for(int y = 0; y < ytiles; y++)
 {
  for(int x = 0; x < xtiles; x++)
  {
   ClassType[p_tmp] = create_bitmap(PLAYER_W, PLAYER_H);
   blit(tmp, ClassType[p_tmp], x*PLAYER_W, y*PLAYER_H, 0, 0, PLAYER_W, PLAYER_H);  
   p_tmp++;
  }
 }
 
 destroy_bitmap(tmp);
}

/* Logica de movimento dos players */
void ClassPlayers::MoveDirectionPlayers()
{
 for(int i = 0; i < MAX_PLAYERS; i++)
 {
  if(RGP[i].ActionPlayer == true)
  {
   if(RGP[i].Direction == 1 && RGP[i].MovY > 0)
   {
    RGP[i].CoordY -= COORD_ADD;
    RGP[i].MovY--;
   }
   else if(RGP[i].Direction == 2 && RGP[i].MovY > 0)
   {
    RGP[i].CoordY += COORD_ADD;
    RGP[i].MovY--;
   }
   else if(RGP[i].Direction == 3 && RGP[i].MovX > 0)
   {
    RGP[i].CoordX -= COORD_ADD;
    RGP[i].MovX--;
   }
   else if(RGP[i].Direction == 4 && RGP[i].MovX > 0)
   {
    RGP[i].CoordX += COORD_ADD;
    RGP[i].MovX--;
   }
   else
   {
    RGP[i].Direction = 0;
   }
  }
 }
}

bool ClassPlayers::PlayerColision(int *Player_X, int *Player_Y)
{
 for(int i = 0; i < MAX_PLAYERS; i++)
 {
  if(RGP[i].ActionPlayer)
  {
   if(*Player_X+31 >= RGP[i].CoordX && *Player_X <= RGP[i].CoordX+31)
   {
    if(*Player_Y+31 >= RGP[i].CoordY && *Player_Y <= RGP[i].CoordY+31)
    {
     return true;
    }
   }
  }
 }
 return false;
}

/* Recebe do servidor o movimento produzido pelos personagens */
void ClassPlayers::FuncRecvMovePlayer(char *buffer)
{
 pRPD = (RecvPlayersDirection*)buffer;
 while(true)
 {
  if(pRPD->Direction == 0x01)
  {
    RGP[pRPD->player_id].Direction = pRPD->Direction;
    RGP[pRPD->player_id].MovY = 32/COORD_ADD;
    RGP[pRPD->player_id].Moviment = 12;
  }
  else if(pRPD->Direction == 0x02)
  {
   RGP[pRPD->player_id].Direction = pRPD->Direction;
   RGP[pRPD->player_id].MovY = 32/COORD_ADD;
   RGP[pRPD->player_id].Moviment = 0;
  }
  else if(pRPD->Direction == 0x03)
  {
   RGP[pRPD->player_id].Direction = pRPD->Direction;
   RGP[pRPD->player_id].MovX = 32/COORD_ADD;
   RGP[pRPD->player_id].Moviment = 4;
  }
  else if(pRPD->Direction == 0x04)
  {
   RGP[pRPD->player_id].Direction = pRPD->Direction;
   RGP[pRPD->player_id].MovX = 32/COORD_ADD;
   RGP[pRPD->player_id].Moviment = 8;
  }
  pRPD->Direction = 0;
  if(RGP[pRPD->player_id].MovX <= 0 && RGP[pRPD->player_id].MovY <= 0)
   break;
  Sleep(1);
 }
}

/* Inicia um player a ser exibido na tela */
void ClassPlayers::FuncRecvGamePlayers(char *buffer)
{
 pRGP = (RecvGamePlayers*)buffer;
 
 RGP[pRGP->player_id].ActionPlayer = true;
 
 RGP[pRGP->player_id].Class = pRGP->Class;
 RGP[pRGP->player_id].CoordX = pRGP->CoordX;
 RGP[pRGP->player_id].CoordY = pRGP->CoordY;
 RGP[pRGP->player_id].Direction = pRGP->Direction;
 if(pRGP->Direction == 0x01)
  RGP[pRGP->player_id].SaveMoviment = 12;
 else if(pRGP->Direction == 0x02)
  RGP[pRGP->player_id].SaveMoviment = 0;
 else if(pRGP->Direction == 0x03)
  RGP[pRGP->player_id].SaveMoviment = 4;
 else if(pRGP->Direction == 0x04)
  RGP[pRGP->player_id].SaveMoviment = 8;
  
 strncpy(RGP[pRGP->player_id].PlayerName, pRGP->PlayerName, 16);
 strncpy(RGP[pRGP->player_id].GuildName, pRGP->GuildName, 16);
 strncpy(RGP[pRGP->player_id].PartyName, pRGP->PartyName, 16);
 
 RGP[pRGP->player_id].VIP = pRGP->VIP;
}

/* Remove um player */
void ClassPlayers::FuncRemovePlayer(char *buffer)
{
 pRRP = (RecvRemovePlayer*)buffer;
 RGP[pRRP->player_id].ActionPlayer = false;
}

/* Reseta todos os players */
void ClassPlayers::ResetPlayers()
{
 for(int i = 0; i < MAX_PLAYERS; i++)
 {
  RGP[i].ActionPlayer = false;
  RGP[i].GuildName[0] = 0x00;
  RGP[i].PartyName[0] = 0x00;
  RGP[i].Moviment = 0;
  RGP[i].SaveMoviment = 0;
  RGP[i].Direction = 0;
  RGP[i].MovX = 0;
  RGP[i].MovY = 0;
 }
}

/* Desenha os players na tela */
void ClassPlayers::DrawPlayers(BITMAP *GameBuffer)
{
 MoveDirectionPlayers();
 
 for(int i = 0; i < MAX_PLAYERS; i++)
 {
  if(RGP[i].ActionPlayer == true)
  {
   if(RGP[i].Direction == 1)
   {
    RGP[i].SaveMoviment = 12;
    if(RGP[i].Moviment >= RGP[i].SaveMoviment+3)
     RGP[i].Moviment = RGP[i].SaveMoviment;
    else
     RGP[i].Moviment++;
   }
   else if(RGP[i].Direction == 2)
   {
    RGP[i].SaveMoviment = 0;
    if(RGP[i].Moviment >= RGP[i].SaveMoviment+3)
     RGP[i].Moviment = RGP[i].SaveMoviment;
    else
     RGP[i].Moviment++;
   }
   else if(RGP[i].Direction == 3)
   {
    RGP[i].SaveMoviment = 4;
    if(RGP[i].Moviment >= RGP[i].SaveMoviment+3)
     RGP[i].Moviment = RGP[i].SaveMoviment;
    else
     RGP[i].Moviment++;
   }
   else if(RGP[i].Direction == 4)
   {
    RGP[i].SaveMoviment = 8;
    if(RGP[i].Moviment >= RGP[i].SaveMoviment+3)
     RGP[i].Moviment = RGP[i].SaveMoviment;
    else
     RGP[i].Moviment++;
   }
   else
   {
    RGP[i].Moviment = RGP[i].SaveMoviment;
   }
   
   if(RGP[i].VIP == 0)
    textprintf_ex(GameBuffer, font, RGP[i].CoordX-(text_length(font, RGP[i].PlayerName)/2)+15, RGP[i].CoordY-10, makecol(255, 255, 255), -1, "%s", RGP[i].PlayerName);
   else
    textprintf_ex(GameBuffer, font, RGP[i].CoordX-(text_length(font, RGP[i].PlayerName)/2)+15, RGP[i].CoordY-10, makecol(0, 255, 0), -1, "%s", RGP[i].PlayerName);
   
   int tmp_integer = 0;
    
   if(RGP[i].GuildName[0] != 0x00)
   {
    textprintf_ex(GameBuffer, font, RGP[i].CoordX-(text_length(font, RGP[i].GuildName)/2)+15, RGP[i].CoordY+PLAYER_H+2+tmp_integer, makecol(255, 0, 0), -1, "%s", RGP[i].GuildName);
    tmp_integer += 12;
   }
   
   if(RGP[i].PartyName[0] != 0x00)
   {
    textprintf_ex(GameBuffer, font, RGP[i].CoordX-(text_length(font, RGP[i].PartyName)/2)+15, RGP[i].CoordY+PLAYER_H+2+tmp_integer, makecol(0, 0, 255), -1, "%s", RGP[i].PartyName);
   }
 
   if(RGP[i].Class == 0x01)
    draw_sprite(GameBuffer, Class01[RGP[i].Moviment], RGP[i].CoordX, RGP[i].CoordY);
   else if(RGP[i].Class == 0x02)
    draw_sprite(GameBuffer, Class02[RGP[i].Moviment], RGP[i].CoordX, RGP[i].CoordY);
   else if(RGP[i].Class == 0x03)
    draw_sprite(GameBuffer, Class03[RGP[i].Moviment], RGP[i].CoordX, RGP[i].CoordY);
   else if(RGP[i].Class == 0x04)
    draw_sprite(GameBuffer, Class04[RGP[i].Moviment], RGP[i].CoordX, RGP[i].CoordY);
  }
 }
}
