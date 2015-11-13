#ifndef __LIBRARY_H_
#define __LIBRARY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro.h>
#include <winalleg.h>

#define MAX_PLAYERS 50
#define COORD_ADD 4

#define ALTURA 800
#define LARGURA 640

#define MAX_TILES 300

#define MAX_PLAYERNAME 16
#define MAX_GUILDNAME 16
#define MAX_PARTYNAME 16

#define PLAYER_H 48
#define PLAYER_W 32

#define TIMEBUTTON 10

enum
{
    STARTGAME,
    
    STATECONNECT,
    STATEFULL,
    STATEERROR,
    STATEEXIT,
    STATEOFFLINE,
    
    DESCONNECT,
    INVALIDACCOUNT,
    ACCOUNTEXIST,
};

enum
{
    THREAD_WAIT,
    THREAD_RUNNING
};

void frame_rate();
void fps_increment();

DWORD RecvThread();

#endif
