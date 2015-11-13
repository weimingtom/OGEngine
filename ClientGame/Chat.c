#include "Chat.h"

void ClassChat::InitChat()
{
 ChatGUI = load_bitmap("GUI/ChatGUI.bmp", NULL);
 if(ChatGUI == NULL)
 {
  allegro_message("Impossivel carregar o GUI [ChatGUI.bmp]");
  exit(1);
 }
}

void ClassChat::CaptureChatKeyboard()
{
}
        
void ClassChat::DrawChat(BITMAP *GameBuffer)
{
 set_trans_blender(0, 0, 0, 150);
 draw_trans_sprite(GameBuffer, ChatGUI, 5, GameBuffer->h-ChatGUI->h-25);
}

void ClassChat::DrawTextChat()
{
}

void ClassChat::DrawTextSendChat()
{
}
