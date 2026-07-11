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
#include <stdio.h>
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

typedef enum
{
    MERGE_ALLY_UNIT,
    MERGE_ATTACK_UNIT
} MERGE_STATES;

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
static Unit *attackTargetUnit;
static Vector2 mergeTargetPosition;
static MERGE_STATES currentMergeState;
static Unit *mergeAllyUnit;

static Timer cpuTurnTimer;
static int cpuCurrentUnitIndex = 0;

void Move(void);
void Attack(void);
void Merge(void);
void Defend(void);

// Update States

void UpdateIdleState(void);
void UpdateMoveState(void);
void UpdateAttackState(void);
void UpdateMergeState(void);

// Merge Update States
void UpdateMergeAllyUnitState(void);
void UpdateMergeAttackUnitState(void);

// Draw States
void DrawIdleState(void);
void DrawMoveState(void);
void DrawAttackState(void);
void DrawMergeState(void);

// Merge Draw Status
void DrawMergeAllyUnitState(void);
void DrawMergeAttackUnitState(void);

// Game State
void DrawGameState(void);

// Callbacks
void finishTurnCallback(void);
void onCPuTurnTimerComplete(void);
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
    InitTimer(&cpuTurnTimer, 90, true, false, onCPuTurnTimerComplete);
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    if (game.turn == TEAM1_TURN)
    {

        HandleInput(); // Handle input for the gameplay screen

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
            UpdateMergeState();
            break;
        case DEFEND_STATE:
            break;
        }

        for (int i = 0; i < MAX_UNITS; i++)
        {
            UpdateUnit(&game.playerTeam.units[i]);
        }
    }
    else
    {

        UpdateTimer(&cpuTurnTimer);
    }
    UpdateContextMessage(&contextMessage);
    UpdateButton(&finishTurnButton, GetMousePosition(), GetLastInputAction() == CONFIRM && game.turn == TEAM1_TURN);
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
    attackTargetPosition = (Vector2){(int)(GetMousePosition().x / 32) * 32 + 16, (int)(GetMousePosition().y / 32) * 32 + 16};

    for (int i = 0; i < MAX_UNITS; i++)
    {
        if (CheckCollisionPointRec(GetMousePosition(), game.enemyTeam.units[i].boundingBox) && game.enemyTeam.units[i].active)
        {
            float distance = CalculateDistance(selectedUnit->position, attackTargetPosition);
            if (GetLastInputAction() == CONFIRM && distance <= selectedUnit->attack_range)
            {
                attackTargetUnit = &game.enemyTeam.units[i];
                int damage = AttackUnit(selectedUnit, attackTargetUnit);
                TraceLog(LOG_INFO, "%s attack %s with %d damage", GetUnitTypeName(selectedUnit->type), GetUnitTypeName(attackTargetUnit->type), damage);
                const char *buffer = malloc(50 * sizeof(char));
                sprintf(buffer, "%s attack %s with %d damage", GetUnitTypeName(selectedUnit->type), GetUnitTypeName(attackTargetUnit->type), damage);
                ShowContextMessage(&contextMessage,
                                   buffer, 120, MESSAGE_ERROR, onContextMessageClose);
                recalculateActiveUnitsCount(&game.enemyTeam);
                selectedUnit->attacked = true;
                selectedUnit->moved = true;
                currentAction = IDLE;
                attackTargetUnit = NULL;
                selectedUnit = NULL;
                break;
            }
        }
    }
    if (GetLastInputAction() == CANCEL)
    {
        currentAction = IDLE;
        attackTargetUnit = NULL;
    }
}

void UpdateMergeState(void)
{

    switch (currentMergeState)
    {
    case MERGE_ALLY_UNIT:
        UpdateMergeAllyUnitState();
        break;
    case MERGE_ATTACK_UNIT:
        UpdateMergeAttackUnitState();
        break;
    }
    if (GetLastInputAction() == CANCEL)
    {
        currentAction = IDLE;
        currentMergeState = MERGE_ALLY_UNIT;
        mergeAllyUnit = NULL;
        attackTargetUnit = NULL;
        return;
    }
}

void UpdateMergeAllyUnitState(void)
{

    mergeTargetPosition = (Vector2){(int)(GetMousePosition().x / 32) * 32 + 16, (int)(GetMousePosition().y / 32) * 32 + 16};
    for (int i = 0; i < MAX_UNITS; i++)
    {
        if (CheckCollisionPointRec(mergeTargetPosition, game.playerTeam.units[i].boundingBox) && game.playerTeam.units[i].active && &game.playerTeam.units[i] != selectedUnit)
        {
            float distance = CalculateDistance(selectedUnit->position, mergeTargetPosition);
            if (GetLastInputAction() == CONFIRM && distance <= MERGE_RANGE)
            {
                mergeAllyUnit = &game.playerTeam.units[i];
                currentMergeState = MERGE_ATTACK_UNIT;
                ShowContextMessage(&contextMessage, "Select Enemy Unit to Attack", 120, MESSAGE_INFO, onContextMessageClose);
                break;
            }
        }
    }
}

void UpdateMergeAttackUnitState(void)
{

    attackTargetPosition = (Vector2){(int)(GetMousePosition().x / 32) * 32 + 16, (int)(GetMousePosition().y / 32) * 32 + 16};
    for (int i = 0; i < MAX_UNITS; i++)
    {
        if (CheckCollisionPointRec(GetMousePosition(), game.enemyTeam.units[i].boundingBox) && game.enemyTeam.units[i].active)
        {
            float distance = CalculateDistance(selectedUnit->position, attackTargetPosition);
            if (GetLastInputAction() == CONFIRM && distance <= selectedUnit->attack_range)
            {
                Unit *targetUnit = &game.enemyTeam.units[i];
                int damage = MergeUnits(selectedUnit, mergeAllyUnit, targetUnit);
                TraceLog(LOG_INFO, "%s and %s merge to attack %s with %d damage", GetUnitTypeName(selectedUnit->type), GetUnitTypeName(mergeAllyUnit->type), GetUnitTypeName(targetUnit->type), damage);
                const char *buffer = malloc(50 * sizeof(char));
                sprintf(buffer, "%s and %s attack %s with %d damage", GetUnitTypeName(selectedUnit->type), GetUnitTypeName(mergeAllyUnit->type), GetUnitTypeName(targetUnit->type), damage);
                ShowContextMessage(&contextMessage,
                                   buffer, 120, MESSAGE_ERROR, onContextMessageClose);
                recalculateActiveUnitsCount(&game.enemyTeam);
                selectedUnit->attacked = true;
                selectedUnit->moved = true;
                mergeAllyUnit->attacked = true;
                mergeAllyUnit->moved = true;
                currentAction = IDLE;
                attackTargetUnit = NULL;
                selectedUnit = NULL;
                mergeAllyUnit = NULL;
                mergeTargetPosition = (Vector2){0, 0};
                currentMergeState = MERGE_ALLY_UNIT;
                break;
            }
        }
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
        DrawMergeState();
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
    for (int i = 0; i < MAX_UNITS; i++)
    {
        if (CheckCollisionPointRec(GetMousePosition(), game.enemyTeam.units[i].boundingBox) && game.enemyTeam.units[i].active)
        {
            distance = CalculateDistance(selectedUnit->position, attackTargetPosition);
            lineColor = (distance <= selectedUnit->attack_range) ? GREEN : RED;
            break;
        }
    }
    DrawRectangleLines(attackTargetPosition.x - 16, attackTargetPosition.y - 16, 32, 32, lineColor);
}

void DrawMergeState(void)
{

    DrawRectangleLines(selectedUnit->boundingBox.x, selectedUnit->boundingBox.y, selectedUnit->boundingBox.width, selectedUnit->boundingBox.height, YELLOW);

    switch (currentMergeState)
    {
    case MERGE_ALLY_UNIT:
        DrawMergeAllyUnitState();
        break;
    case MERGE_ATTACK_UNIT:
        DrawMergeAttackUnitState();
        break;
    default:
        break;
    }
}

// Merge Draw Status
void DrawMergeAllyUnitState(void)
{
    Color lineColor = RED;
    for (int i = 0; i < MAX_UNITS; i++)
    {
        if (CheckCollisionPointRec(mergeTargetPosition, game.playerTeam.units[i].boundingBox) && game.playerTeam.units[i].active && &game.playerTeam.units[i] != selectedUnit)
        {
            float distance = CalculateDistance(selectedUnit->position, mergeTargetPosition);
            lineColor = (distance <= MERGE_RANGE) ? GREEN : RED;
            break;
        }
    }
    DrawRectangleLines(mergeTargetPosition.x - 16, mergeTargetPosition.y - 16, 32, 32, lineColor);
}
void DrawMergeAttackUnitState(void)
{

    Color lineColor = RED;
    for (int i = 0; i < MAX_UNITS; i++)
    {
        if (CheckCollisionPointRec(attackTargetPosition, game.enemyTeam.units[i].boundingBox) && game.enemyTeam.units[i].active)
        {
            float distance = CalculateDistance(selectedUnit->position, attackTargetPosition);
            lineColor = (distance <= selectedUnit->attack_range) ? GREEN : RED;
            break;
        }
    }
    DrawRectangleLines(attackTargetPosition.x - 16, attackTargetPosition.y - 16, 32, 32, lineColor);
    DrawRectangleLines(mergeAllyUnit->boundingBox.x, mergeAllyUnit->boundingBox.y, mergeAllyUnit->boundingBox.width, mergeAllyUnit->boundingBox.height, GREEN);
}

void DrawGameState(void)
{
    DrawRectangle(GetScreenWidth() - 144, 0, 144, GetScreenHeight(), DARKGRAY);
    DrawText(TextFormat("Day: %d", game.day), GetScreenWidth() - 100, 10, 20, WHITE);
    DrawText(TextFormat("%s Turn", (game.turn == TEAM1_TURN) ? "Player" : "Enemy"), GetScreenWidth() - 135, 40, 20, WHITE);
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
    if (selectedUnit->defending)
    {
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
    if (selectedUnit->attacked || selectedUnit->defending)
    {
        ShowContextMessage(&contextMessage, "Unit has attacked or is defending", 120, MESSAGE_ERROR, onContextMessageClose);
        currentAction = IDLE;
        return;
    }
    else
    {
        ShowContextMessage(&contextMessage, "Select ally unit to merge with (Right Click to cancel)", 120, MESSAGE_INFO, onContextMessageClose);
        currentAction = MERGE_STATE;
    }
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
        if (selectedUnit->moved || selectedUnit->attacked)
        {
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
        StartTimer(&cpuTurnTimer);
        cpuCurrentUnitIndex = 0;
        ShowContextMessage(&contextMessage, "Enemy Turn", 120, MESSAGE_INFO, onContextMessageClose);
    }
    else
    {
        game.turn = TEAM1_TURN;
        game.day++;
        StopTimer(&cpuTurnTimer);
        resetTurn(&game);
        ShowContextMessage(&contextMessage, "Player Turn", 120, MESSAGE_INFO, onContextMessageClose);
    }
}

void onCPuTurnTimerComplete(void)
{

    Unit *currentUnit = NULL;
    for (int i = cpuCurrentUnitIndex; i < MAX_UNITS; i++)
    {
        if (game.enemyTeam.units[i].active && !game.enemyTeam.units[i].moved && !game.enemyTeam.units[i].attacked)
        {
            currentUnit = &game.enemyTeam.units[i];
            cpuCurrentUnitIndex = i;
            break;
        }
    }

    for (int i = 0; i < MAX_UNITS; i++)
    {
        if (game.playerTeam.units[i].active && !currentUnit->attacked)
        {
            float distance = CalculateDistance(currentUnit->position, game.playerTeam.units[i].position);
            if (distance <= currentUnit->attack_range)
            {
                AttackUnit(currentUnit, &game.playerTeam.units[i]);
                TraceLog(LOG_INFO, "%s attack %s with %.1f damage", GetUnitTypeName(currentUnit->type), GetUnitTypeName(game.playerTeam.units[i].type), currentUnit->damage);
                const char *buffer = malloc(50 * sizeof(char));
                sprintf(buffer, "%s attack %s with %.1f damage", GetUnitTypeName(currentUnit->type), GetUnitTypeName(game.playerTeam.units[i].type), currentUnit->damage);
                ShowContextMessage(&contextMessage,
                                   buffer, 120, MESSAGE_ERROR, onContextMessageClose);
                recalculateActiveUnitsCount(&game.playerTeam);
                currentUnit->attacked = true;
                currentUnit->moved = true;
            }
        }
    }

    for (int i = 0; i < MAX_UNITS; i++)
    {
        if (game.playerTeam.units[i].active && !currentUnit->moved)
        {
            int dx = GetRandomValue(-currentUnit->speed, currentUnit->speed);
            int dy = GetRandomValue(-currentUnit->speed, currentUnit->speed);
            Vector2 targetPosition = (Vector2){(int)(currentUnit->position.x + dx), (int)(currentUnit->position.y + dy)};
            TraceLog(LOG_INFO, "CPU unit %s moving to (%d, %d)", GetUnitTypeName(currentUnit->type), (int)targetPosition.x, (int)targetPosition.y);
            if (targetPosition.x > MAP_WIDTH)
            {
                targetPosition.x = MAP_WIDTH;
            }
            if (targetPosition.y > MAP_HEIGHT)
            {
                targetPosition.y = MAP_HEIGHT;
            }
            if (targetPosition.x < 1)
            {
                targetPosition.x = 1;
            }
            if (targetPosition.y < 1)
            {
                targetPosition.y = 1;
            }
            TraceLog(LOG_INFO, "CPU unit %s moving to (%d, %d)", GetUnitTypeName(currentUnit->type), (int)targetPosition.x, (int)targetPosition.y);

            MoveUnit(currentUnit, targetPosition);
            currentUnit->moved = true;
        }
    }

    cpuCurrentUnitIndex++;

    if (cpuCurrentUnitIndex >= MAX_UNITS)
    {
        finishTurnCallback();
        return;
    }
}