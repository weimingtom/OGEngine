#ifndef PLAYERS_H_
#define PLAYERS_H_

#include "Library.h"
#include "PacketGame.h"
#include "GUI.H"

/* Structura dos players logados no servidor */

class ClassPlayers
{
 private:
         RecvGamePlayers RGP[MAX_PLAYERS];
         
         RecvGamePlayers *pRGP;
         RecvPlayersDirection *pRPD;
         RecvRemovePlayer * pRRP;
         
         BITMAP **ClassType;
         
         BITMAP *Class01[16];
         BITMAP *Class02[16];
         BITMAP *Class03[16];
         BITMAP *Class04[16];
         
 public:
        ClassPlayers();
        ~ClassPlayers();
        void InitPlayers();
        
        void LoadingSpritePlayer(char *);
        
        void MoveDirectionPlayers();
        
        bool PlayerColision(int *, int *);
        
        void FuncRecvMovePlayer(char *);
        void FuncRecvGamePlayers(char *);
        void FuncRemovePlayer(char *);
        
        void ResetPlayers();
        
        void DrawPlayers(BITMAP *);
};

#endif
