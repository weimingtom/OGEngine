#include "Guild.h"

void ClassGuild::InitGuild()
{
 GuildGUI.InitGUI("GUI/GuildGUI.bmp", 200, 200);
}

bool ClassGuild::OpenningGuildGUI()
{
 return GuildGUI.SetGUI();
}

/* Recebe as Info da Guild do servidor */
void ClassGuild::PacketRecvGuildInfo(char *buffer)
{
 StructRecvGuildInfo *RGI = (StructRecvGuildInfo*)buffer;
 StatGuildLider = RGI->StatGuildLider;
 
 strcpy(GuildName, RGI->GuildName);
 
 for(int i = 0; i < 15; i++)
  strncpy(AllNameGuild[i], RGI->AllNameGuild[i], 16);
  
 strncpy(GuildMaster, RGI->GuildMaster, 16);
}

/* Desenha a GUI da Guild */
void ClassGuild::DrawGuild(BITMAP *GameBuffer)
{
 GuildGUI.DrawGUI(GameBuffer);
 GuildGUI.InfoText(GameBuffer, "Guild Master", 0, 0, makecol(0, 0, 255));
 if(GuildMaster[0] == 0x00)
  GuildGUI.InfoText(GameBuffer, "None", 120, 0, makecol(0, 255, 0));
 else
  GuildGUI.InfoText(GameBuffer, GuildMaster, 120, 0, makecol(0, 255, 0));
 
 for(int i = 0; i < 15; i++)
 {
  GuildGUI.InfoText(GameBuffer, "Guild Member", 0, 15*(i+1), makecol(0, 0, 0));
  if(AllNameGuild[i][0] == 0x00)
   GuildGUI.InfoText(GameBuffer, "None", 120, 15*(i+1), makecol(255, 0, 0));
  else
   GuildGUI.InfoText(GameBuffer, AllNameGuild[i], 120, 15*(i+1), makecol(255, 0, 0));
 }
 
 GuildGUI.LogicGUI(mouse_x, mouse_y, mouse_b);
}
