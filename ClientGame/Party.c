#include "Party.h"

void ClassParty::InitParty()
{
 PartyGUI.InitGUI("GUI/PartyGUI.bmp", 200, 200);
}

bool ClassParty::OpenningPartyGUI()
{
 return PartyGUI.SetGUI();
}

/* Recebe as info da Party do servidor */
void ClassParty::PacketRecvPartyInfo(char *buffer)
{
 StructRecvPartyInfo *RPI = (StructRecvPartyInfo*)buffer;
 StatPartyLider = RPI->StatPartyLider;
 
 strcpy(PartyName, RPI->PartyName);
 
 for(int i = 0; i < 4; i++)
  strncpy(AllNameParty[i], RPI->AllNameParty[i], 16);
  
 strncpy(PartyMaster, RPI->PartyMaster, 16);
}

/* Desenha a GUI da Party */
void ClassParty::DrawParty(BITMAP *GameBuffer)
{
 PartyGUI.DrawGUI(GameBuffer);
 PartyGUI.InfoText(GameBuffer, "Party Master", 0, 0, makecol(0, 0, 255));
 if(PartyMaster[0] == 0x00)
  PartyGUI.InfoText(GameBuffer, "None", 120, 0, makecol(0, 255, 0));
 else
  PartyGUI.InfoText(GameBuffer, PartyMaster, 120, 0, makecol(0, 255, 0));
 
 for(int i = 0; i < 4; i++)
 {
  PartyGUI.InfoText(GameBuffer, "Party Member", 0, 15*(i+1), makecol(0, 0, 0));
  if(AllNameParty[i][0] == 0x00)
   PartyGUI.InfoText(GameBuffer, "None", 120, 15*(i+1), makecol(255, 0, 0));
  else
   PartyGUI.InfoText(GameBuffer, AllNameParty[i], 120, 15*(i+1), makecol(255, 0, 0));
 }
 
 PartyGUI.LogicGUI(mouse_x, mouse_y, mouse_b);
}
