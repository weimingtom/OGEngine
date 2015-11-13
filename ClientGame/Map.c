#include "Map.h"
#include <conio.h>

/* Carrega o mapa definido pelo param */
void ClassMap::LoadingMap(char *filename)
{
 FILE *file = fopen(filename, "rb");
 
 if(file == NULL)
 {
  allegro_message("Impossivel carregar o MAP [%s]", filename);
  exit(1);
 }
 
 fread(&info, sizeof(MAPINFO), 1, file);
 
 int value;
 int tmp_count = 0;
 for(int x = 0; x < info.h; x++)
 {
  for(int y = 0; y < info.w; y++)
  {
   fread(&value, sizeof(int), 1, file);
   c_map_layer1[tmp_count] = value;
   tmp_count++;
  }
 }

 tmp_count = 0;
 for(int x = 0; x < info.h; x++)
 {
  for(int y = 0; y < info.w; y++)
  {
   fread(&value, sizeof(int), 1, file);
   c_map_layer2[tmp_count] = value;
   tmp_count++;
  }
 }
 
 tmp_count = 0;
 for(int x = 0; x < info.h; x++)
 {
  for(int y = 0; y < info.w; y++)
  {
   fread(&value, sizeof(int), 1, file);
   c_map_layer3[tmp_count] = value;
   tmp_count++;
  }
 }
 
 tmp_count = 0;
 for(int x = 0; x < info.h; x++)
 {
  for(int y = 0; y < info.w; y++)
  {
   fread(&value, sizeof(int), 1, file);
   c_blocked_map[tmp_count] = value;
   tmp_count++;
  }
 }
 
 fclose(file);
}

/* Função substituida!! */

/*void ClassMap::LoadingColision(char *filename)
{
 FILE *file = fopen(filename, "rb");
 
 if(file == NULL)
 {
  allegro_message("Impossivel carregar o MAPCOLISION [%s]", filename);
  exit(1);
 }
 
 fread(&info, sizeof(MAPINFO), 1, file);
 
 int value;
 int tmp_count = 0;
 for(int x = 0; x < info.h; x++)
 {
  for(int y = 0; y < info.w; y++)
  {
   fread(&value, sizeof(int), 1, file);
   c_blocked_map[tmp_count] = value;
   tmp_count++;
  }
 }
  
 fclose(file);
}*/

/* Carrega o tile definido pelo param */
void ClassMap::LoadingTiles(char *filename)
{
 BITMAP *tmp = load_bitmap(filename, NULL);
 
 for(int i = 0; i < MAX_TILES; i++)
 {
  Tiles[i] = NULL;
 }
 
 if(tmp == NULL)
 {
  allegro_message("Impossivel carregar o TILES [%s]", filename);
  exit(1);
 }
 
 int xtiles = tmp->w / 32;
 int ytiles = tmp->h / 32;
 
 int c_tmp = 0;
 for(int y = 0; y < ytiles; y++)
 {
  for(int x = 0; x < xtiles; x++)
  {
   Tiles[c_tmp] = create_bitmap(32, 32);
   blit(tmp, Tiles[c_tmp], x * 32, y * 32, 0, 0, 32, 32);  
   c_tmp++;
  }
 }
 destroy_bitmap(tmp);
}

void ClassMap::Destrocy()
{
 for(int i = 0; i < MAX_TILES; i++)
 {
  if(Tiles[i] != NULL)
   destroy_bitmap(Tiles[i]);
 }
}

/* Verificação de colisão com os blocos, 
/*recebe os param dos pointer do player, CoordX & CoordY */
bool ClassMap::TilesBlocked(int *Coordx, int *Coordy)
{
 int tmp_count = 0;
 for(int x = 0; x < info.h; x++)
 {
  for(int y = 0; y < info.w; y++)
  {
   if(c_blocked_map[tmp_count] == 0) // se for 0 é colisão ...
   {
    if(*Coordx+31 >= y*32 && *Coordx <= (y*32)+31)
    {
     if(*Coordy+63 >= x*32 && *Coordy <= (x*32)-1)
     {
      return true;
     }
    }
   }
   tmp_count++;
  }
 }
 return false;
}

/* Desenha layer primario */
void ClassMap::DrawMapLayer1(BITMAP *GameBuffer)
{
 int tmp_count = 0;
 for(int x = 0; x < info.h; x++)
 {
  for(int y = 0; y < info.w; y++)
  {
   draw_sprite(GameBuffer, Tiles[c_map_layer1[tmp_count]], y*32, x*32);
   tmp_count++;
  }
 }
}

/* Desenha layer secundario */
void ClassMap::DrawMapLayer2(BITMAP *GameBuffer)
{
 int tmp_count = 0;
 for(int x = 0; x < info.h; x++)
 {
  for(int y = 0; y < info.w; y++)
  {
   draw_sprite(GameBuffer, Tiles[c_map_layer2[tmp_count]], y*32, x*32);
   tmp_count++;
  }
 }
}

void ClassMap::DrawMapLayer3(BITMAP *GameBuffer)
{
 int tmp_count = 0;
 for(int x = 0; x < info.h; x++)
 {
  for(int y = 0; y < info.w; y++)
  {
   draw_sprite(GameBuffer, Tiles[c_map_layer3[tmp_count]], y*32, x*32);
   tmp_count++;
  }
 }
}

/* Desenha áreas em colisão */
void ClassMap::DrawMapColision(BITMAP *GameBuffer)
{
 int tmp_count = 0;
 
 if(key[KEY_Q])
 {
  for(int x = 0; x < info.h; x++)
  {
   for(int y = 0; y < info.w; y++)
   {
    draw_sprite(GameBuffer, Tiles[c_blocked_map[tmp_count]], y*32, x*32);
    tmp_count++;
   }
  }
 }
}

/* Recebe as info do servidor para carregar o mapa */
/* Nome da File & Nome do Tile */
int ClassMap::StatRecvMap(SOCKET GSocket)
{
 char Loading[127];
 int BytesRecv;
 char buffer[sizeof(RecvPlayerMap)];
 RecvPlayerMap * RPM = (RecvPlayerMap*)&buffer;
 recv(GSocket, buffer, sizeof(RecvPlayerMap), 0);
 
 strcpy(Loading, "Map/");
 strcat(Loading, RPM->FileMap);
 LoadingMap(Loading);
 
 /*
 strcpy(Loading, "Map/");
 strcat(Loading, RPM->FileColision);
 LoadingColision(Loading);
*/

 strcpy(Loading, "Map/");
 strcat(Loading, RPM->FileTiles);
 LoadingTiles(Loading);
 
 return BytesRecv;
}
