#include "GUI.h"

ClassGUI::ClassGUI()
{
 ActiveGUI = false;
}

ClassGUI::~ClassGUI()
{
 destroy_bitmap(ImageGUI);
}

/* Inicia a GUI com os param pré-definidos pelos param */
void ClassGUI::InitGUI(char *FileGUI, int x, int y)
{
 ImageGUI = load_bitmap(FileGUI, NULL);
 if(ImageGUI == NULL)
 {
  allegro_message("Impossivel carregar o GUI [%s]", FileGUI);
  exit(1);
 }
 this->gui_x = x;
 this->gui_y = y;
}

/* Logica principal da GUI */
void ClassGUI::LogicGUI(int mouse_x, int mouse_y, int mouse_b)
{
 if(ActiveGUI)
 {
  if(mouse_x >= gui_x && mouse_x <= gui_x+ImageGUI->w)
  {
   if(mouse_y >= gui_y && mouse_y <= gui_y+29)
   {
    if(mouse_b & 1)
     MoveGUI(mouse_x, mouse_y);
   }
  }
  
  if(mouse_x >= gui_x+ImageGUI->w-23 && mouse_x <= gui_x+ImageGUI->w-5)
  {
   if(mouse_y >= gui_y+5 && mouse_y <= gui_y+26)
   {
    if(mouse_b & 1)
     CloseGUI();
   }
  }
  
 }
 
 aux_x = mouse_x;
 aux_y = mouse_y;
}

/* Move a GUI para a posição do mouse */
void ClassGUI::MoveGUI(int mouse_x, int mouse_y)
{
  gui_x += mouse_x-aux_x;
  gui_y += mouse_y-aux_y;
}

/* Seta a GUI com SetGUI() função generalizada*/
void ClassGUI::CloseGUI()
{
 if(ActiveGUI)
 {
  SetGUI();
 }
}

/* Seta text dentro da GUI */
void ClassGUI::InfoText(BITMAP *GameBuffer, char *Text, int x, int y, int color)
{
 if(ActiveGUI)
 {
  textprintf_ex(GameBuffer, font, x+gui_x+10, y+gui_y+38, color, -1, Text);
 }
}

/* Desenha a GUI no Buffer */
void ClassGUI::DrawGUI(BITMAP *GameBuffer)
{
 if(ActiveGUI)
 {
  set_trans_blender(0, 0, 0, 200);
  draw_trans_sprite(GameBuffer, ImageGUI, gui_x, gui_y);
 }
}

/* Ativa & Desativa Invible da GUI */
bool ClassGUI::SetGUI()
{
 if(ActiveGUI)
  ActiveGUI = false;
 else
  ActiveGUI = true;
  
 return ActiveGUI;
}
