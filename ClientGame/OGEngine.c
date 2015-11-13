#include "OGEngine.h"

/* Inicia a Engine com os param pre-definidos*/
ClassOGEngine::ClassOGEngine(unsigned altura, unsigned largura)
{
 old_fps = 0;
 new_fps = 0;
 fps_speed = 0;
 
 allegro_init();

 install_keyboard();
 install_mouse();
 install_timer();
 
 set_window_title("OGEngine");
 
 set_color_depth(32);
 set_gfx_mode(GFX_AUTODETECT_WINDOWED, altura, largura, 0, 0);
 GameBuffer = create_bitmap(altura, largura);
 
 set_display_switch_mode(SWITCH_BACKGROUND);
 
 install_int(frame_rate, 1000);
 install_int_ex(fps_increment, BPS_TO_TIMER(40));
}

ClassOGEngine::~ClassOGEngine()
{
 destroy_bitmap(this->GameBuffer);
}

void ClassOGEngine::LoginInfo(char *l, char *s)
{
 strncpy(login, l, 16);
 strncpy(senha, s, 16);
}

/* Inicia a conexão com o servidor */
unsigned int ClassOGEngine::ConnectGame()
{
 WSAStartup(0x202, &data);
 GSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 if(GSocket == SOCKET_ERROR)
  return STATEERROR;
  
 addr_sock.sin_family = AF_INET;
 addr_sock.sin_port = htons(9000);
 addr_sock.sin_addr.s_addr = inet_addr("192.168.1.31");
 
 if(connect(GSocket, (SOCKADDR*)&addr_sock, sizeof(struct sockaddr)) == SOCKET_ERROR)
  return STATEOFFLINE;
  
 char buffer[sizeof(TypeInfoPacket)];
 TypeInfoPacket *SIT = (TypeInfoPacket *)&buffer;
 
 int BytesRecv;
 BytesRecv = RecvPacket(buffer, sizeof(TypeInfoPacket));
 if(BytesRecv == -1 || BytesRecv == 0)
  return DESCONNECT;
  
 if(SIT->type_one == 0x01 && SIT->type_two == 0x00)
  return STATEFULL;
 else if(SIT->type_one == 0x01 && SIT->type_two == 0x06)
  return STARTGAME;
  
 return STATEERROR;
}

void ClassOGEngine::SendPacket(char *b, int size)
{
 send(GSocket, b, size, 0);
}

int ClassOGEngine::RecvPacket(char *b, int size)
{
 recv(GSocket, b, size, 0);
}

void ClassOGEngine::ImageClear()
{
 clear(GameBuffer);
}
     
/* 'Main Engine' principal */   
void ClassOGEngine::MainGame()
{
 Stat = ConnectGame();
 ConnectError(Stat);
 
 Stat = SendAccount(login, senha);
 
 if(Stat == 0x04)
 {
  allegro_message("Conta inexistente");
  exit(1);
 }
 else if(Stat == 0x02)
 {
  allegro_message("Account Banida");
  exit(1);
 }
 else if(Stat == 0x01)
 {
  allegro_message("Account Logada");
  exit(1);
 }
 else
 {
  Stat = STARTGAME;
  Player.InitPlayer();
  CMonsters.InitMonsters();
  Player.RecvStatPlayer(GSocket);
  Map.StatRecvMap(GSocket);
  Players.InitPlayers();
  CChat.InitChat();
  CreateThread(0, 0, (LPTHREAD_START_ROUTINE)RecvThread, 0, 0, 0);
 }
 
 fps_speed = 0;
 
 while(Stat == STARTGAME)
 {
   while(fps_speed > 0)
   {
     ImageClear();      // Limpa a tela do jogo  
     
     MapLogicGame();
     MapDrawGame();
     
     fps_speed--;
     new_fps++;
   }
 }
 
 show_mouse(NULL);
 Map.Destrocy();
 Player.Destroy();
}

/* Desenha o jogo ultima função a ser chamada para desenhar na screen */
void ClassOGEngine::GameDraw()
{
 //textprintf_ex(GameBuffer, font, 10, 10, makecol(255, 0, 0), -1, "FPS: %d", this->old_fps);
 show_mouse(GameBuffer);
 blit(GameBuffer, screen, 0, 0, 0, 0, ALTURA, LARGURA);
 show_mouse(NULL);
}

/* Logica de todas as funções do mapa */
void ClassOGEngine::MapLogicGame()
{
 bool blocked = false;
 Player.StatCoordMap_x = Player.CoordMap_x;
 Player.StatCoordMap_y = Player.CoordMap_y;
 
 Player.PlayerLogic(GSocket);
 Player.PlayerAttack(GSocket);
 
 if(Map.TilesBlocked(&Player.CoordMap_x, &Player.CoordMap_y))
  blocked = true;
 if(Players.PlayerColision(&Player.CoordMap_x, &Player.CoordMap_y))
  blocked = true;
 if(CMonsters.ColisionMonsters(&Player.CoordMap_x, &Player.CoordMap_y))
  blocked = true;
   
 if(blocked)
 {
  Player.Direction = 0;
  Player.CoordMap_x = Player.StatCoordMap_x;
  Player.CoordMap_y = Player.StatCoordMap_y;
  Player.PacketSendCoord(GSocket); // pede coordenadas ...
 }
 else
  Player.SendMoveDirection(GSocket);
}

/* Desenha todas as funções do mapa */
void ClassOGEngine::MapDrawGame()
{
 Map.DrawMapLayer1(GameBuffer);
 Map.DrawMapLayer3(GameBuffer);
 Map.DrawMapColision(GameBuffer);
 
 Players.DrawPlayers(GameBuffer);
 Player.DrawPlayer(GameBuffer);
 CMonsters.DrawMonsters(GameBuffer);
 
 Map.DrawMapLayer2(GameBuffer);
 
 Player.DrawStates(GameBuffer);
 Player.DrawStatBar(GameBuffer);
 //CChat.DrawChat(GameBuffer);
 GameDraw();
}

/* Envia Login + Senha para o servidor */
int ClassOGEngine::SendAccount(char *Login, char *Password)
{
 int BytesRecv;
 char buffer[sizeof(StructSendAccount)];
 memset(buffer, 0, sizeof(buffer));
 
 StructSendAccount *SA = (StructSendAccount*)&buffer;
 strncpy(SA->Login, Login, 16);
 strncpy(SA->Password, Password, 16);
 SendPacket(buffer, sizeof(StructSendAccount));
 
 char buffer_t[sizeof(TypeInfoPacket)];
 TypeInfoPacket *TIP = (TypeInfoPacket *)&buffer;
 BytesRecv = RecvPacket(buffer, sizeof(TypeInfoPacket));
 if(BytesRecv == -1 || BytesRecv == 0)
  ConnectError(DESCONNECT);
 else
  return TIP->type_two;
}

void ClassOGEngine::ConnectError(int Stat)
{
 if(Stat == DESCONNECT)
  allegro_message("Servidor Desconectado");
 else if(Stat == STATEFULL)
  allegro_message("Servidor Lotado");
 else if(Stat == STATEERROR)
  allegro_message("Erro ao tentar se conectar");
 else if(Stat == STATEOFFLINE)
  allegro_message("Servidor Offline");
 
 if(Stat != STARTGAME)
  exit(1);
}

unsigned int ClassOGEngine::GetSocket()
{
 return GSocket;
}
