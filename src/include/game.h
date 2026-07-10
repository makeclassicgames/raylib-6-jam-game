#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include "unit.h"

#define MAX_UNITS 9

typedef enum{
   TEAM1_TURN,
   TEAM2_TURN
}TURN;

typedef enum{
    TEAM_PLAYER,
    TEAM_CPU
}TeamType;

typedef struct{
    Color color;
    Unit units[MAX_UNITS];
    int activeUnitsCount;
    TeamType type;
}Team;

typedef struct{
    int day;
    TURN turn;
    Team playerTeam;
    Team enemyTeam;
}Game;

void InitGame(Game *game);
void InitTeam(Team *team, Color color, TeamType type);

Unit* getActiveUnitsFromTeam(Team *team, int *unitCount);

#endif