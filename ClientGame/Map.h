#ifndef __MAP_H_
#define __MAP_H_

#include "Library.h"
#include "PacketGame.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
 int w;
 int h;   
} MAPINFO;

class ClassMap
{
      private:
              BITMAP *Tiles[MAX_TILES];
              
              MAPINFO info;
              
              int c_blocked_map[501];
              int c_map_layer1[501], c_map_layer2[501], c_map_layer3[501];
      public:
             void LoadingMap(char *);
             void LoadingColision(char *);
             void LoadingTiles(char *);
             void Destrocy();
             bool TilesBlocked(int*, int*);
             void DrawMapLayer1(BITMAP *);
             void DrawMapLayer2(BITMAP *);
             void DrawMapLayer3(BITMAP *);
             void DrawMapColision(BITMAP *);
             // **********************
             int StatRecvMap(SOCKET GSocket);
};

#endif
