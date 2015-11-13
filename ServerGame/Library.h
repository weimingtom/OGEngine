#ifndef __LIBRARY_H_
#define __LIBRARY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#include <mysql/mysql.h>

#define MAX_PLAYER 50
#define MAX_MONSTERS_SERVER 999


#define MAX_PLAYERNAME 16
#define MAX_GUILDNAME 16
#define MAX_PARTYNAME 16

struct s_con
{
 SOCKET winsock;
 bool action_type;
 int timer;
};

#endif
