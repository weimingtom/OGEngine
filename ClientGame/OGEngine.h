#ifndef __OGENGINE_H_
#define __OGENGINE_H_

#include "Library.h"
#include "Map.h"
#include "Player.h"
#include "Players.h"
#include "Chat.h"
#include "Monsters.h"
#include "PacketGame.h"

/* Classe principal da Engine */

extern volatile int mouse_x;
extern volatile int mouse_y;
extern volatile int mouse_z;
extern volatile int mouse_b;
extern volatile int mouse_pos;

class ClassOGEngine
{
 private:        
         WSADATA data;
         SOCKADDR_IN addr_sock;
         SOCKET GSocket;
         
         char login[16];
         char senha[16];
 public:          
        ClassMap Map;
        ClassPlayer Player;
        ClassPlayers Players;
        ClassChat CChat;
        ClassMonsters CMonsters;
         
        unsigned int Stat;
        
        int new_fps;
        int old_fps;
         
        int fps_speed;
        
        BITMAP *GameBuffer;
        
        ClassOGEngine(unsigned, unsigned);
        ~ClassOGEngine();
        
        void LoginInfo(char *, char *);
        
        unsigned int ConnectGame();
        void SendPacket(char *b, int);
        int RecvPacket(char *b, int);
        
        int SendAccount(char *Login, char *Password);
        
        void ImageClear();
        
        void MainGame();
        
        void GameDraw();
        
        void MapLogicGame();
        void MapDrawGame();
        
        void ConnectError(int);
        unsigned int GetSocket();
};

#endif
