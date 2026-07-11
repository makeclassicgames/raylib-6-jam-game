/**********************************************************************************************
 *
 *   raylib - Advance Game template
 *
 *   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
 *
 *   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
 *
 *   This software is provided "as-is", without any express or implied warranty. In no event
 *   will the authors be held liable for any damages arising from the use of this software.
 *
 *   Permission is granted to anyone to use this software for any purpose, including commercial
 *   applications, and to alter it and redistribute it freely, subject to the following restrictions:
 *
 *     1. The origin of this software must not be misrepresented; you must not claim that you
 *     wrote the original software. If you use this software in a product, an acknowledgment
 *     in the product documentation would be appreciated but is not required.
 *
 *     2. Altered source versions must be plainly marked as such, and must not be misrepresented
 *     as being the original software.
 *
 *     3. This notice may not be removed or altered from any source distribution.
 *
 **********************************************************************************************/

#include <stdlib.h>
#include <math.h>
#include <raylib.h>
#include "screens.h"
#include "game.h"
#include "map.h"
#include "unit.h"
#include "ui.h"
#include "physics.h"
#include "input.h"

#define MAP_WIDTH 18
#define MAP_HEIGHT 23

typedef enum
{
    IDLE,
    MOVE_STATE,
    ATTACK_STATE,
    MERGE_STATE,
    DEFEND_STATE,
    GAME_OVER_STATE
} GAMEPLAY_ACTION;

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
static Map gameMap; // Declare a Map variable to hold the game map
static Game game;   // Declare a Game variable to hold the game state
static ContextMenu unitContextMenu;
static Unit *selectedUnit;
static GAMEPLAY_ACTION currentAction;
static Vector2 targetPosition;
static ContextMessage contextMessage;
static Button finishTurnButton;
static Vector2 attackTargetPosition;
static Unit* attackTargetUnit;

void Move(void);
void Attack(void);
void Merge(void);
void Defend(void);

// Update States

void UpdateIdleState(void);
void UpdateMoveState(void);
void UpdateAttackState(void);

// Draw States
void DrawIdleState(void);
void DrawMoveState(void);
void DrawAttackState(void);

// Game State
void DrawGameState(void);

// Callbacks
void finishTurnCallback(void);
void onContextMessageClose(void)
{
    HideContextMessage(&contextMessage);
}

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;
    currentAction = IDLE;
    InitMap(&gameMap, MAP_WIDTH, MAP_HEIGHT);
    InitGame(&game);
    InitContextMenu(&unitContextMenu);
    AddButtonToContextMenu(&unitContextMenu, "Move", Move);
    AddButtonToContextMenu(&unitContextMenu, "Attack", Attack);
    AddButtonToContextMenu(&unitContextMenu, "Merge", Merge);
    AddButtonToContextMenu(&unitContextMenu, "Defend", Defend);
    InitContextMesage(&contextMessage, (Vector2){0, 0});
    InitButton(&finishTurnButton, "Finish Turn", (Vector2){GetScreenWidth() - 120, 440}, finishTurnCallback);
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    HandleInput(); // Handle input for the gameplay screen
    // TODO: Update GAMEPLAY screen variables here!

    switch (currentAction)
    {
    case IDLE:
        UpdateIdleState();
        break;
    case MOVE_STATE:
        UpdateMoveState();
        break;
    case ATTACK_STATE:
        UpdateAttackState();
        break;
    case MERGE_STATE:
        break;
    case DEFEND_STATE:
        break;
    }

    for (int i = 0; i < MAX_UNITS; i++)
    {
        UpdateUnit(&game.playerTeam.units[i]);
    }
    UpdateContextMessage(&contextMessage);
    UpdateButton(&finishTurnButton, GetMousePosition(), GetLastInputAction() == CONFIRM);
}

void UpdateIdleState(void)
{
    if (selectedUnit != NULL)
    {
        Vector2 unitPosition = {selectedUnit->boundingBox.x + selectedUnit->boundingBox.width, selectedUnit->boundingBox.y};
        UpdateContextMenu(&unitContextMenu, unitPosition, GetLastInputAction() == CONFIRM);
    }

    if (GetLastInputAction() == CONFIRM)
    {
        HideContextMenu(&unitContextMenu);
        if (selectedUnit != NULL && currentAction == IDLE)
        {
            selectedUnit = NULL;
        }

        for (int i = 0; i < MAX_UNITS; i++)
        {
            if (CheckCollisionPointRec(GetMousePosition(), game.playerTeam.units[i].boundingBox))
            {
                selectedUnit = &game.playerTeam.units[i];
                ShowContextMenu(&unitContextMenu);
                break;
            }
        }
    }
}

void UpdateMoveState(void)
{

    if (selectedUnit != NULL && selectedUnit->moved)
    {
        currentAction = IDLE;
        selectedUnit = NULL;
    }
    targetPosition = (Vector2){(int)(GetMousePosition().x / 32) * 32 + 16, (int)(GetMousePosition().y / 32) * 32 + 16};
    float distance = sqrtf(powf((targetPosition.x - (selectedUnit->position.x - 1) * 32 - 16), 2) + powf((targetPosition.y - (selectedUnit->position.y - 1) * 32 - 16), 2)) / 32.0f;

    if (GetLastInputAction() == CONFIRM && distance <= selectedUnit->speed)
    {
        Vector2 movePosition = (Vector2){(int)(targetPosition.x / 32) + 1, (int)(targetPosition.y / 32) + 1};
        MoveUnit(selectedUnit, movePosition);
        currentAction = IDLE;
        selectedUnit = NULL;
    }

    if (GetLastInputAction() == CANCEL)
    {
        currentAction = IDLE;
    }
}

void UpdateAttackState(void)
{
    if (selectedUnit != NULL && selectedUnit->attacked)
    {
        currentAction = IDLE;
        selectedUnit = NULL;
    }
    attackTargetPosition= (Vector2){(int)(GetMousePosition().x / 32) * 32 + 16, (int)(GetMousePosition().y / 32) * 32 + 16};
   
    for(int i=0;i<MAX_UNITS;i++){
        if(CheckCollisionPointRec(GetMousePosition(), game.enemyTeam.units[i].boundingBox) && game.enemyTeam.units[i].active){
            float distance = CalculateDistance(selectedUnit->position,attackTargetPosition);
            if(GetLastInputAction()==CONFIRM && distance<=selectedUnit->attack_range){
               attackTargetUnit = &game.enemyTeam.units[i];
               //TOOD: Calculate Attack
               break;
            }
        }
    }
    if (GetLastInputAction() == CANCEL)
    {
        currentAction = IDLE;
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    // TODO: Draw GAMEPLAY screen here!
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), PURPLE);
    DrawMap(&gameMap);
    // PlayerUnits
    for (int i = 0; i < MAX_UNITS; i++)
    {
        DrawUnit(&game.playerTeam.units[i], 0);
    }

    // EnemyUnits
    for (int i = 0; i < MAX_UNITS; i++)
    {
        DrawUnit(&game.enemyTeam.units[i], 1);
    }

    switch (currentAction)
    {
    case IDLE:
        DrawIdleState();
        break;
    case MOVE_STATE:
        DrawMoveState();
        break;
    case ATTACK_STATE:
        DrawAttackState();
        break;
    case MERGE_STATE:
        break;
    case DEFEND_STATE:
        break;
    }
    DrawGameState();

    DrawContextMessage(&contextMessage);
}

void DrawIdleState(void)
{
    if (selectedUnit != NULL)
    {
        DrawRectangleLines(selectedUnit->boundingBox.x, selectedUnit->boundingBox.y, selectedUnit->boundingBox.width, selectedUnit->boundingBox.height, YELLOW);
        DrawContextMenu(&unitContextMenu, (Vector2){selectedUnit->boundingBox.x + selectedUnit->boundingBox.width, selectedUnit->boundingBox.y});
    }
}

void DrawMoveState(void)
{
    if (selectedUnit != NULL)
    {
        DrawRectangleLines(selectedUnit->boundingBox.x, selectedUnit->boundingBox.y, selectedUnit->boundingBox.width, selectedUnit->boundingBox.height, YELLOW);
    }

    float distance = sqrtf(powf((targetPosition.x - (selectedUnit->position.x - 1) * 32 - 16), 2) + powf((targetPosition.y - (selectedUnit->position.y - 1) * 32 - 16), 2)) / 32.0f;
    Color lineColor = (distance <= selectedUnit->speed) ? GREEN : RED;
    DrawRectangleLines(targetPosition.x - 16, targetPosition.y - 16, 32, 32, lineColor);
}

void DrawAttackState(void)
{
    if (selectedUnit != NULL)
    {
        DrawRectangleLines(selectedUnit->boundingBox.x, selectedUnit->boundingBox.y, selectedUnit->boundingBox.width, selectedUnit->boundingBox.height, YELLOW);
    }
    
    float distance = CalculateDistance(selectedUnit->position, attackTargetPosition);
    Color lineColor = RED;
    for(int i=0;i<MAX_UNITS;i++){
        if(CheckCollisionPointRec(GetMousePosition(), game.enemyTeam.units[i].boundingBox) && game.enemyTeam.units[i].active){
            distance = CalculateDistance(selectedUnit->position, attackTargetPosition);
            lineColor = (distance <= selectedUnit->attack_range) ? GREEN : RED;
            break;
        }
    }
    DrawRectangleLines(attackTargetPosition.x - 16, attackTargetPosition.y - 16, 32, 32, lineColor);
}

void DrawGameState(void)
{
    DrawRectangle(GetScreenWidth() - 144, 0, 144, GetScreenHeight(), DARKGRAY);
    DrawText(TextFormat("Day: %d", game.day), GetScreenWidth() - 100, 10, 20, WHITE);
    DrawText(TextFormat("Turn: %d", (game.turn == TEAM1_TURN) ? 1 : 2), GetScreenWidth() - 110, 40, 20, WHITE);
    DrawText(TextFormat("Player Units: %d", game.playerTeam.activeUnitsCount), GetScreenWidth() - 130, 70, 17, BLUE);
    DrawText(TextFormat("Enemy Units: %d", game.enemyTeam.activeUnitsCount), GetScreenWidth() - 125, 100, 17, RED);
    DrawRectangle(GetScreenWidth() - 130, 150, 120, 270, GRAY);
    if (selectedUnit != NULL)
    {
        DrawText(TextFormat("%s", GetUnitTypeName(selectedUnit->type)), GetScreenWidth() - 100, 160, 17, WHITE);
        DrawText(TextFormat("HP: %d/%d", selectedUnit->hp, selectedUnit->max_hp), GetScreenWidth() - 120, 190, 17, WHITE);
        DrawText(TextFormat("Damage: %.1f", selectedUnit->damage), GetScreenWidth() - 120, 220, 17, WHITE);
        DrawText(TextFormat("Speed: %d", selectedUnit->speed), GetScreenWidth() - 120, 250, 17, WHITE);
        DrawText(TextFormat("Range: %d", selectedUnit->attack_range), GetScreenWidth() - 120, 280, 17, WHITE);
        DrawText(TextFormat("Armor: %d", selectedUnit->armor), GetScreenWidth() - 120, 310, 17, WHITE);
        DrawText(TextFormat("Moved: %s", selectedUnit->moved ? "Yes" : "No"), GetScreenWidth() - 120, 340, 17, WHITE);
        DrawText(TextFormat("Attacked: %s", selectedUnit->attacked ? "Yes" : "No"), GetScreenWidth() - 120, 370, 17, WHITE);
        DrawText(TextFormat("Defend: %s", selectedUnit->defending ? "Yes" : "No"), GetScreenWidth() - 120, 400, 17, WHITE);
    }
    DrawButton(&finishTurnButton);
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}

void Move(void)
{
    currentAction = MOVE_STATE;
    if(selectedUnit->defending){
        ShowContextMessage(&contextMessage, "Unit is defending and cannot move", 120, MESSAGE_ERROR, onContextMessageClose);
        currentAction = IDLE;
        return;
    }
    if (selectedUnit->moved || selectedUnit->attacked)
    {
        ShowContextMessage(&contextMessage, "Unit has moved or attacked", 120, MESSAGE_ERROR, onContextMessageClose);
        currentAction = IDLE;
    }
    else
    {
        ShowContextMessage(&contextMessage, "Select Target Destination (Right Click to cancel)", 120, MESSAGE_INFO, onContextMessageClose);
    }
}

void Attack(void)
{
    currentAction = ATTACK_STATE;
    if (selectedUnit->attacked || selectedUnit->defending)
    {
        ShowContextMessage(&contextMessage, "Unit has attacked or is defending", 120, MESSAGE_ERROR, onContextMessageClose);
        currentAction = IDLE;
    }
    else
    {
        ShowContextMessage(&contextMessage, "Select Target Unit (Right Click to cancel)", 120, MESSAGE_INFO, onContextMessageClose);
    }
 
}
void Merge(void)
{
    TraceLog(LOG_INFO, "Merge Units");
}
void Defend(void)
{
    if (selectedUnit->defending)
    {
        ShowContextMessage(&contextMessage, "Unit is already defending", 120, MESSAGE_ERROR, onContextMessageClose);
        currentAction = IDLE;
    }
    else
    {
        if(selectedUnit->moved || selectedUnit->attacked){
            ShowContextMessage(&contextMessage, "Unit has already moved or attacked", 120, MESSAGE_ERROR, onContextMessageClose);
            currentAction = IDLE;
            return;
        }
        DefendUnit(selectedUnit);
        ShowContextMessage(&contextMessage, "Unit is now defending", 120, MESSAGE_INFO, onContextMessageClose);
        currentAction = IDLE;
    }
    
}

void finishTurnCallback(void)
{

    if (game.turn == TEAM1_TURN)
    {
        game.turn = TEAM2_TURN;
        ShowContextMessage(&contextMessage, "Enemy Turn", 120, MESSAGE_INFO, onContextMessageClose);
    }
    else
    {
        game.turn = TEAM1_TURN;
        game.day++;
        ShowContextMessage(&contextMessage, "Player Turn", 120, MESSAGE_INFO, onContextMessageClose);
    }
}
