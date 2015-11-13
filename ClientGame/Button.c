#include "Button.h"

ClassButton::ClassButton()
{
}

ClassButton::~ClassButton()
{
 destroy_bitmap(ButtonNull);
 destroy_bitmap(ButtonSelect);
}

/* Inicia o botão com as informações pre-definidas pelos param */
void ClassButton::InitButton(char *FileButtonOne, char *FileButtonTwo, int x, int y)
{
 this->x = x;
 this->y = y;
 
 ButtonNull = load_bitmap(FileButtonOne, NULL);
 if(ButtonNull == NULL)
 {
  allegro_message("Impossivel carregar o SPRITE [%s]", FileButtonOne);
  exit(1);
 }
 
 ButtonSelect = load_bitmap(FileButtonTwo, NULL);
 if(ButtonSelect == NULL)
 {
  allegro_message("Impossivel carregar o SPRITE [%s]", FileButtonTwo);
  exit(1);
 }
}

/* Verificar se o botão foi pressionado */
bool ClassButton::MouseButtonSelect(int mouse_x, int mouse_y)
{
 if(mouse_x >= x && mouse_x <= x+ButtonNull->w)
 {
  if(mouse_y >= y && mouse_y <= y+ButtonNull->h)
  {
   clicked = true;
   return true;
  }
 }
 
 clicked = false;
 return false;
}

/* Desenha o botão */
void ClassButton::DrawButton(BITMAP *GameBuffer)
{
 set_trans_blender(0, 0, 0, 180);
 if(clicked)
  draw_trans_sprite(GameBuffer, ButtonSelect, x, y);
 else
  draw_trans_sprite(GameBuffer, ButtonNull, x, y);
}
