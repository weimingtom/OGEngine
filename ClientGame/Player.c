#include "Player.h"

/* Inicia o player com as info pré-definido da classe */
void ClassPlayer::InitPlayer()
{
 stat_hp = load_bitmap("Sprite/stat_hp.bmp", NULL);
 if(stat_hp == NULL)
 {
  allegro_message("Impossivel carregar o SPRITE [stat_hp.bmp]");
  exit(1);
 }
 
 hp_hit = load_bitmap("Sprite/hp_hit.bmp", NULL);
 if(hp_hit == NULL)
 {
  allegro_message("Impossivel carregar o SPRITE [hp_hit.bmp]");
  exit(1);
 }
 
 ////////////////////////////////////////
 
 stat_mp = load_bitmap("Sprite/stat_mp.bmp", NULL);
 if(stat_hp == NULL)
 {
  allegro_message("Impossivel carregar o SPRITE [stat_mp.bmp]");
  exit(1);
 }
 
 mp_hit = load_bitmap("Sprite/mp_hit.bmp", NULL);
 if(hp_hit == NULL)
 {
  allegro_message("Impossivel carregar o SPRITE [mp_hit.bmp]");
  exit(1);
 }
 
 ////////////////////////////////////////
 
 stat_exp = load_bitmap("Sprite/stat_exp.bmp", NULL);
 if(stat_exp == NULL)
 {
  allegro_message("Impossivel carregar o SPRITE [stat_exp.bmp]");
  exit(1);
 }
 
 exp_hit = load_bitmap("Sprite/exp_hit.bmp", NULL);
 if(exp_hit == NULL)
 {
  allegro_message("Impossivel carregar o SPRITE [exp_hit.bmp]");
  exit(1);
 }
 
 ////////////////////////////////////////
 cifra = load_bitmap("SFX/cifra.bmp", NULL);
 if(cifra == NULL)
 {
  allegro_message("Impossivel carregar o SPRITE [cifra.bmp]");
  exit(1);
 }
 
 StatusButton.InitButton("GUI/ButtonStatusBar.bmp", "GUI/ButtonSelectStatusBar.bmp", 320, 5);
 GuildButton.InitButton("GUI/ButtonGuildBar.bmp", "GUI/ButtonSelectGuildBar.bmp", 416, 5);
 PartyButton.InitButton("GUI/ButtonPartyBar.bmp", "GUI/ButtonSelectPartyBar.bmp", 512, 5);
 InventoryButton.InitButton("GUI/ButtonInventoryBar.bmp", "GUI/ButtonSelectInventoryBar.bmp", 608, 5);
 ExitButton.InitButton("GUI/ButtonExitBar.bmp", "GUI/ButtonSelectExitBar.bmp", 704, 5);
 
 StatusGUI.InitGUI("GUI/StatusGUI.bmp", 200, 200);

 Guild.InitGuild();
 Party.InitParty();
 
 SaveMoviment = 0;
 Moviment = 0;
 Direction = 0;
 
 MovX = -1;
 MovY = -1;
 
 TimeKeyButton = 0;
 TimeButton = 0;
 
 p_class = 0;
 for(int i = 0; i < 5; i++)
  memset(LogMessage[i], 0, sizeof(LogMessage[0]));
}

/* Carrega a sprite do player a ser utilizada no jogo */
void ClassPlayer::LoadingSpritePlayer(char *filename)
{
 BITMAP *tmp = load_bitmap(filename, NULL);

 for(int i = 0; i < 16; i++)
 {
  SpritePlayer[i] = NULL;
 }
 
 if(tmp == NULL)
 {
  allegro_message("Impossivel carregar o SPRITE [%s]", filename);
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
   SpritePlayer[p_tmp] = create_bitmap(tx, ty);
   blit(tmp, SpritePlayer[p_tmp], x*tx, y*ty, 0, 0, tx, ty);  
   p_tmp++;
  }
 }
 
 destroy_bitmap(tmp);
}

void ClassPlayer::Destroy()
{
 for(int i = 0; i < 12; i++)
 {
  if(SpritePlayer[i] != NULL)
   destroy_bitmap(SpritePlayer[i]);
 }
 
 destroy_bitmap(stat_hp);
 destroy_bitmap(hp_hit);
 destroy_bitmap(stat_mp);
 destroy_bitmap(mp_hit);
 destroy_bitmap(stat_exp);
 destroy_bitmap(exp_hit);
 destroy_bitmap(cifra);
}

/* Logica primário do Player */
void ClassPlayer::PlayerLogic(SOCKET GSocket)
{  
 StatKeyPlayer();
 if(Direction == 1 && MovY > 0)
 {
  CoordMap_y -= COORD_ADD;
  MovY--;
 }
 else if(Direction == 2 && MovY > 0)
 {
  CoordMap_y += COORD_ADD;
  MovY--;
 }
 else if(Direction == 3 && MovX > 0)
 {
  CoordMap_x -= COORD_ADD;
  MovX--;
 }
 else if(Direction == 4 && MovX > 0)
 {
  CoordMap_x += COORD_ADD;
  MovX--;
 }
 else
 {
  Direction = 0;
 }
 
 if(CoordMap_x >= ALTURA-PLAYER_W)
 {
  Direction = 0;
  CoordMap_x = ALTURA-PLAYER_W;
 }
 else if(CoordMap_y >= LARGURA-PLAYER_H)
 {
  Direction = 0;
  CoordMap_y = LARGURA-PLAYER_H;
 }
 else if(CoordMap_x < 0)
 {
  Direction = 0;
  CoordMap_x = 0;
 }
 else if(CoordMap_y < 0)
 {
  Direction = 0;
  CoordMap_y = 0;
 }
 
 if(RecvGuildInfo == true)
 {
  char buffer[sizeof(TypeInfoPacket)];
  memset(buffer, 0, sizeof(buffer));
  TypeInfoPacket *TIP = (TypeInfoPacket*)&buffer;
  TIP->type_one = 0x00;
  TIP->type_two = 0x03;
  send(GSocket, buffer, sizeof(TypeInfoPacket), 0);
  RecvGuildInfo = false;
 }
 
 if(RecvPartyInfo == true)
 {
  char buffer[sizeof(TypeInfoPacket)];
  memset(buffer, 0, sizeof(buffer));
  TypeInfoPacket *TIP = (TypeInfoPacket*)&buffer;
  TIP->type_one = 0x00;
  TIP->type_two = 0x04;
  send(GSocket, buffer, sizeof(TypeInfoPacket), 0);
  RecvPartyInfo = false;
 }
}

void ClassPlayer::PlayerAttack(SOCKET GSocket)
{
 if(SendAttackInfoZ)
 {
  char buffer[sizeof(TypeInfoPacket)];
  memset(buffer, 0, sizeof(buffer));
  TypeInfoPacket *TIP = (TypeInfoPacket*)&buffer;
  TIP->type_one = 0x00;
  TIP->type_two = 0x20;
  send(GSocket, buffer, sizeof(TypeInfoPacket), 0);
  SendAttackInfoZ = false;
 }
 if(SendAttackInfoX)
 {
  char buffer[sizeof(TypeInfoPacket)];
  memset(buffer, 0, sizeof(buffer));
  TypeInfoPacket *TIP = (TypeInfoPacket*)&buffer;
  TIP->type_one = 0x01;
  TIP->type_two = 0x20;
  send(GSocket, buffer, sizeof(TypeInfoPacket), 0);
  SendAttackInfoX = false;
 }
}
        
/* Input keyboard, movimento do player */
void ClassPlayer::StatKeyPlayer()
{
 if(TimeKeyButton == 0)
 {
  if(Direction == 0)
  {
   if(key[KEY_UP])
   {
    Direction = 1;
    MovY = 32/COORD_ADD;
    Moviment = 12;
    TimeKeyButton = TIMEBUTTON;
   }
   else if(key[KEY_DOWN])
   {
    Direction = 2;
    MovY = 32/COORD_ADD;
    Moviment = 0;
    TimeKeyButton = TIMEBUTTON;
   }
   else if(key[KEY_LEFT])
   {
    Direction = 3;
    MovX = 32/COORD_ADD;
    Moviment = 4;
    TimeKeyButton = TIMEBUTTON;
   }
   else if(key[KEY_RIGHT])
   {
    Direction = 4;
    MovX = 32/COORD_ADD;
    Moviment = 8;
    TimeKeyButton = TIMEBUTTON;
   }
   else if(key[KEY_Z])
   {
    SendAttackInfoZ = true;
    TimeKeyButton = TIMEBUTTON;
   }
   else if(key[KEY_X])
   {
    SendAttackInfoX = true;
    TimeKeyButton = TIMEBUTTON;
   }
  }
 }
 else
 {
  TimeKeyButton--;
 }
}

/* Desenha o personagem na tela & logica de movimentação */
void ClassPlayer::DrawPlayer(BITMAP *GameBuffer)
{ 
 if(Direction == 1)
 {
  SaveMoviment = 12;
  if(Moviment >= SaveMoviment+2)
   Moviment = SaveMoviment;
  else
   Moviment++;
 }
 else if(Direction == 2)
 {
  SaveMoviment = 0;
  if(Moviment >= SaveMoviment+2)
   Moviment = SaveMoviment;
  else
   Moviment++;
 }
 else if(Direction == 3)
 {
  SaveMoviment = 4;
  if(Moviment >= SaveMoviment+2)
   Moviment = SaveMoviment;
  else
   Moviment++;
 }
 else if(Direction == 4)
 {
  SaveMoviment = 8;
  if(Moviment >= SaveMoviment+2)
   Moviment = SaveMoviment;
  else
   Moviment++;
 }
 else
 {
  Moviment = SaveMoviment;
 }
 
 //printf("X [%d] Y [%d]\n", CoordMap_x, CoordMap_y);
 
 draw_sprite(GameBuffer, SpritePlayer[Moviment], CoordMap_x, CoordMap_y);
}

/* Desenha os status do player na tela */
void ClassPlayer::DrawStates(BITMAP *GameBuffer)
{
 char buffer[127];
 draw_sprite(GameBuffer, hp_hit, 25, 20);
 masked_blit(stat_hp, GameBuffer, 0, 0, 28, 23, (144*hp)/max_hp, 14);
 
 draw_sprite(GameBuffer, mp_hit, 25, 45);
 masked_blit(stat_mp, GameBuffer, 0, 0, 28, 48, (144*mp)/max_mp, 14);
 
 draw_sprite(GameBuffer, exp_hit, 0, 635-15);
 masked_blit(stat_exp, GameBuffer, 0, 0, 2, 637-15, (800*exp)/max_exp, 15);
 
 draw_sprite(GameBuffer, cifra, 75+28-35-10, 70);
 
 textprintf_ex(GameBuffer, font, 7, 27, makecol(255, 255, 255), -1, "HP");
 textprintf_ex(GameBuffer, font, 7, 52, makecol(255, 255, 255), -1, "MP");
 
 textprintf_ex(GameBuffer, font, 75+28-35, 27, makecol(0, 0, 0), -1, "%d/%d", hp, max_hp);
 textprintf_ex(GameBuffer, font, 75+28-35, 52, makecol(0, 0, 0), -1, "%d/%d", mp, max_mp);
 
 sprintf(buffer, "Exp %d/%d", exp, max_exp);
 textprintf_ex(GameBuffer, font, (800/2)-text_length(font, buffer)/2, 624, makecol(0, 0, 0), -1, "%s", buffer);
 
 textprintf_ex(GameBuffer, font, 75+28-35, 7, makecol(255, 255, 255), -1, "Level %d", level);
 textprintf_ex(GameBuffer, font, 75+28-35+5, 76, makecol(255, 255, 255), -1, " %d", money);
 
 int tmp_integer = 0;
 
 if(vip == 0)
  textprintf_ex(GameBuffer, font, CoordMap_x-(text_length(font, PlayerName)/2)+15, CoordMap_y-10, makecol(255, 255, 255), -1, "%s", PlayerName);
 else
  textprintf_ex(GameBuffer, font, CoordMap_x-(text_length(font, PlayerName)/2)+15, CoordMap_y-10, makecol(0, 255, 0), -1, "%s", PlayerName);
 
 if(GuildName[0] != 0x00)
 {
  textprintf_ex(GameBuffer, font, CoordMap_x-(text_length(font, GuildName)/2)+15, CoordMap_y+PLAYER_H+2+tmp_integer, makecol(255, 0, 0), -1, "%s", GuildName);
  tmp_integer += 12;
 }
 if(PartyName[0] != 0x00)
 {
  textprintf_ex(GameBuffer, font, CoordMap_x-(text_length(font, PartyName)/2)+15, CoordMap_y+PLAYER_H+2+tmp_integer, makecol(0, 0, 255), -1, "%s", PartyName);
 }
 
 int t_x = 0;
 
 for(int i = 4; i >= 0; i--)
 {
  if(LogMessage[i][0] != 0)
  {
   textprintf_ex(GameBuffer, font, 320, 70+t_x, makecol(255, 255, 255), -1, "MESSAGE %s", LogMessage[i]);
   t_x -= 10;
  }
 }
 
 //textprintf_ex(GameBuffer, font, 10, 20, makecol(255, 255, 0), -1, "Player CoordX: %d", CoordMap_x);
 //textprintf_ex(GameBuffer, font, 10, 30, makecol(255, 255, 0), -1, "Player CoordY: %d", CoordMap_y);
}

/* Desenha a barra de status, Guild, Party, ... */
void ClassPlayer::DrawStatBar(BITMAP *GameBuffer)
{  
 StatusButton.DrawButton(GameBuffer);
 GuildButton.DrawButton(GameBuffer);
 PartyButton.DrawButton(GameBuffer);
 InventoryButton.DrawButton(GameBuffer);
 ExitButton.DrawButton(GameBuffer);
 
 if(StatusButton.MouseButtonSelect(mouse_x, mouse_y))
 {
  if(TimeButton == TIMEBUTTON)
  {
   if(mouse_b & 1)
   {
    StatusGUI.SetGUI();
    TimeButton = 0;
   }
  }
  else
   TimeButton++;
 }
 
 if(GuildButton.MouseButtonSelect(mouse_x, mouse_y))
 {
  if(TimeButton == TIMEBUTTON)
  {
   if(mouse_b & 1)
   {
    RecvGuildInfo = Guild.OpenningGuildGUI();
    TimeButton = 0;
   }
  }
  else
   TimeButton++;
 }
 
 if(PartyButton.MouseButtonSelect(mouse_x, mouse_y))
 {
  if(TimeButton == TIMEBUTTON)
  {
   if(mouse_b & 1)
   {
    RecvPartyInfo = Party.OpenningPartyGUI();
    TimeButton = 0;
   }
  }
  else
   TimeButton++;
 }
 
 if(InventoryButton.MouseButtonSelect(mouse_x, mouse_y))
 {
  if(TimeButton == TIMEBUTTON)
  {
   if(mouse_b & 1)
   {
    TimeButton = 0;
   }
  }
  else
   TimeButton++;
 }
 
 if(ExitButton.MouseButtonSelect(mouse_x, mouse_y))
 {
  if(TimeButton == TIMEBUTTON)
  {
   if(mouse_b & 1)
   {
    exit(1);
   }
  }
  else
   TimeButton++;
 }
 
 DrawGUIStat(GameBuffer);
}

/* Desenha a GUI do Status */
void ClassPlayer::DrawGUIStat(BITMAP *GameBuffer)
{
 char buffer[127];
 StatusGUI.DrawGUI(GameBuffer);
 Guild.DrawGuild(GameBuffer);
 Party.DrawParty(GameBuffer);
 
 StatusGUI.InfoText(GameBuffer, "Nome ", 0, 0, makecol(0, 0, 0));
 StatusGUI.InfoText(GameBuffer, PlayerName, 100, 0, makecol(0, 255, 0));
 
 StatusGUI.InfoText(GameBuffer, "Class ", 0, 20, makecol(0, 0, 0));
 
 if(p_class == 1)
  StatusGUI.InfoText(GameBuffer, "Class1", 100, 20, makecol(255, 0, 0));
 else if(p_class == 2)
  StatusGUI.InfoText(GameBuffer, "Class2", 100, 20, makecol(255, 0, 0));
 else if(p_class == 3)
  StatusGUI.InfoText(GameBuffer, "Class3", 100, 20, makecol(255, 0, 0));
 else if(p_class == 4)
  StatusGUI.InfoText(GameBuffer, "Class4", 100, 20, makecol(255, 0, 0));
 
 sprintf(buffer, "%d", level);
 StatusGUI.InfoText(GameBuffer, "Level ", 0, 40, makecol(0, 0, 0));
 StatusGUI.InfoText(GameBuffer, buffer, 100, 40, makecol(0, 0, 255));
 
 sprintf(buffer, "%d", s_str);
 StatusGUI.InfoText(GameBuffer, "Str ", 0, 60, makecol(0, 0, 0));
 StatusGUI.InfoText(GameBuffer, buffer, 100, 60, makecol(0, 0, 255));
 
 sprintf(buffer, "%d", s_def);
 StatusGUI.InfoText(GameBuffer, "Def ", 0, 80, makecol(0, 0, 0));
 StatusGUI.InfoText(GameBuffer, buffer, 100, 80, makecol(0, 0, 255));
 
 sprintf(buffer, "%d", s_int);
 StatusGUI.InfoText(GameBuffer, "Int ", 0, 100, makecol(0, 0, 0));
 StatusGUI.InfoText(GameBuffer, buffer, 100, 100, makecol(0, 0, 255));
 
 sprintf(buffer, "%d", s_agi);
 StatusGUI.InfoText(GameBuffer, "Agi ", 0, 120, makecol(0, 0, 0));
 StatusGUI.InfoText(GameBuffer, buffer, 100, 120, makecol(0, 0, 255));
 
 sprintf(buffer, "%d", money);
 StatusGUI.InfoText(GameBuffer, "Money ", 0, 140, makecol(0, 0, 0));
 StatusGUI.InfoText(GameBuffer, buffer, 100, 140, makecol(0, 0, 255));
 
 sprintf(buffer, "%d", points);
 StatusGUI.InfoText(GameBuffer, "Points ", 0, 160, makecol(0, 0, 0));
 StatusGUI.InfoText(GameBuffer, buffer, 100, 160, makecol(0, 0, 255));
 
 /////////////////////////////////////////////////////
 
 StatusGUI.LogicGUI(mouse_x, mouse_y, mouse_b);
}

/* Recebe as informações do player do servidor */
int ClassPlayer::RecvStatPlayer(SOCKET GSocket)
{
 int BytesRecv;
 char buffer[sizeof(RecvPlayerInfo)];
 RecvPlayerInfo * RPM = (RecvPlayerInfo*)&buffer;
 recv(GSocket, buffer, sizeof(RecvPlayerInfo), 0);
 
 strcpy(PlayerName, RPM->PlayerName);
 strcpy(GuildName, RPM->GuildName);
 strcpy(PartyName, RPM->PartyName);
 level = RPM->level;
 
 max_hp = RPM->max_hp;
 hp = RPM->hp;
 
 max_mp = RPM->max_mp;
 mp = RPM->mp;
 
 s_str = RPM->s_str;
 s_def = RPM->s_def;
 s_int = RPM->s_int;
 s_agi = RPM->s_agi;
 
 money = RPM->money;
 vip = RPM->vip;
 
 points = RPM->points;
 
 if(p_class == 0)
 {
  p_class = RPM->p_class;
  if(p_class == 1)
   LoadingSpritePlayer("Sprite/Class01.bmp");
  else if(p_class == 2)
   LoadingSpritePlayer("Sprite/Class02.bmp");
  else if(p_class == 3)
   LoadingSpritePlayer("Sprite/Class03.bmp");
  else if(p_class == 4)
   LoadingSpritePlayer("Sprite/Class04.bmp");
 }
 
 CoordMap_x = RPM->Player_x;
 CoordMap_y = RPM->Player_y;
 
 exp = RPM->exp;
 max_exp = RPM->max_exp;
 
 return BytesRecv;
}

/* Recebe as informações da Guild do servidor & envia para sua respectiva class */
void ClassPlayer::PacketRecvGuildInfo(char *buffer)
{
 StructRecvGuildInfo *RGI = (StructRecvGuildInfo*)buffer;
  
 strcpy(GuildName, RGI->GuildName);
 Guild.PacketRecvGuildInfo(buffer);
}

/* Recebe as informações da Party pelo servidor & envia para sua respectiva class */
void ClassPlayer::PacketRecvPartyInfo(char *buffer)
{
 StructRecvPartyInfo *RPI = (StructRecvPartyInfo*)buffer;
  
 strcpy(PartyName, RPI->PartyName);
 Party.PacketRecvPartyInfo(buffer);
}

void ClassPlayer::PacketRecvAllInfo(char *buffer)
{
 StructSendInfoPlayer *SSIP = (StructSendInfoPlayer *)buffer;
 level = SSIP->level;

 max_hp = SSIP->max_hp;
 hp = SSIP->hp;
         
 max_mp = SSIP->max_mp;
 mp = SSIP->mp;

 s_str = SSIP->s_str;
 s_def = SSIP->s_def;
 s_int = SSIP->s_int;
 s_agi = SSIP->s_agi;
         
 money = SSIP->money;

 points = SSIP->points;
 exp = SSIP->exp;
 max_exp = SSIP->max_exp;
 
 for(int i = 0; i < 4; i++)
  strncpy(LogMessage[i], LogMessage[i+1], sizeof(LogMessage[0]));
  
 strncpy(LogMessage[4], SSIP->log_message, sizeof(LogMessage[0]));
}

void ClassPlayer::PacketRecvCoord(char *buffer)
{
 RecvCoord *RC = (RecvCoord*)buffer;
 this->MovX = 0;
 this->MovY = 0;
 CoordMap_x = RC->Player_X;
 CoordMap_y = RC->Player_Y;
}

void ClassPlayer::PacketSendCoord(SOCKET GSocket)
{
 char buffer[sizeof(TypeInfoPacket)];
 TypeInfoPacket *TIP = (TypeInfoPacket *)&buffer;
 TIP->type_one = 0x01;
 TIP->type_two = 0x19;
 send(GSocket, buffer, sizeof(TypeInfoPacket), 0);
}

void ClassPlayer::SendMoveDirection(SOCKET GSocket)
{
 if(Direction != 0 && MovY == 0 || MovX == 0)
 {
  char buffer[sizeof(SendGameInfo)];
  memset(buffer, 0, sizeof(buffer));
  SendGameInfo*SGI = (SendGameInfo*)&buffer;
  
  SGI->type_one = 0x00;
  SGI->type_two = 0x02;
  
  SGI->Direction = Direction;
  send(GSocket, buffer, sizeof(SendGameInfo), 0);
  
  MovY = -1;
  MovX = -1;
 }
}
