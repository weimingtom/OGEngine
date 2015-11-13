#ifndef __MONSTERS_H_
#define __MONSTERS_H_

#include "Library.h"
#include "PacketGame.h"

#define MAX_PACKSPRITE 999 

class ClassMonsters
{
      private:
              RecvGameMonsters RGM;
              //BITMAP *SpriteMonsters[MAX_PACKSPRITE][16];
              BITMAP *SpriteMonsters[MAX_PACKSPRITE][16];
              int count_sprite;
              int tx, ty;
      public:
             ClassMonsters();
             ~ClassMonsters();
             void InitMonsters();
             
             void LoadingSpriteMonsters(char *);
             
             void MoveDirectionMonsters();
             
             bool ColisionMonsters(int*, int*);
             
             void FuncRecvCreateMonsters(char *);
             
             void FuncRecvDeleteMonsters(char *);
             void DeleteMonsters(int);
             
             void FuncMoveMonsters(char *);
             void FuncRecvInfoMonsters(char *);
             
             bool ResetMonsters();
             void DrawMonsters(BITMAP *);
};

#endif
