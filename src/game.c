#include "game.h"

void InitGame(Game *game){
    game->day = 1;
    game->turn = TEAM1_TURN;
    InitTeam(&game->playerTeam, BLUE, TEAM_PLAYER);
    InitTeam(&game->enemyTeam, RED, TEAM_CPU);
}
void InitTeam(Team *team, Color color, TeamType type){
    team->color = color;
    team->activeUnitsCount = MAX_UNITS;
    team->type = type;
    InitUnit(&team->units[0], SOLDIER,GetRandomValue(1, 17), GetRandomValue(1, 21), type);
    InitUnit(&team->units[1], SOLDIER,GetRandomValue(1, 17), GetRandomValue(1, 21), type);
    InitUnit(&team->units[2], SOLDIER,GetRandomValue(1, 17), GetRandomValue(1, 21), type);
    InitUnit(&team->units[3], SOLDIER,GetRandomValue(1, 17), GetRandomValue(1, 21), type);
    InitUnit(&team->units[4], TANK,GetRandomValue(1, 17), GetRandomValue(1, 21), type);
    InitUnit(&team->units[5], TANK,GetRandomValue(1, 17), GetRandomValue(1, 21), type);
    InitUnit(&team->units[6], TANK,GetRandomValue(1, 17), GetRandomValue(1, 21), type);
    InitUnit(&team->units[7], PLANE,GetRandomValue(1, 17), GetRandomValue(1, 21), type);
    InitUnit(&team->units[8], PLANE,GetRandomValue(1, 17), GetRandomValue(1, 21), type);
}

void recalculateActiveUnitsCount(Team *team){
    int count = 0;
    for(int i=0;i<MAX_UNITS;i++){
        if(team->units[i].active){
            count++;
        }
    }
    team->activeUnitsCount = count;
}

void resetTurn(Game* game){
    for(int i=0;i<MAX_UNITS;i++){
        resetUnit(&game->playerTeam.units[i]);
        resetUnit(&game->enemyTeam.units[i]);
    }
    recalculateActiveUnitsCount(&game->playerTeam);
    recalculateActiveUnitsCount(&game->enemyTeam);
}

int FinishedGame(Game *game){
    if(game->playerTeam.activeUnitsCount == 0){
        return LOSE;
    }
    if(game->enemyTeam.activeUnitsCount == 0){
        return WIN;
    }
    return ON_GAME;
}