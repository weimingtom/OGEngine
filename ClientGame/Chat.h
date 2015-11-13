#ifndef __CHAT_H_
#define __CHAT_H_

#include "Library.h"
#include "PacketGame.h"

class ClassChat
{
 private:
         char MsgChat[6][MAX_MSG_CHAT];
         BITMAP *ChatGUI;
         SendMsgChat *SMC;
 public:
        void InitChat();
        void CaptureChatKeyboard();
        
        void DrawChat(BITMAP *);
        void DrawTextChat();
        void DrawTextSendChat();
};

#endif
