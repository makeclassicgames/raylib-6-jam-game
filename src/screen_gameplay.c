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
#include "map.h"
#include "unit.h"
#include "ui.h"
#include "input.h"

#define MAP_WIDTH 23
#define MAP_HEIGHT 23

#define MAX_UNITS 3

typedef enum{
    IDLE,
    MOVE_STATE,
    ATTACK_STATE,
    MERGE_STATE,
    WAIT_STATE
} GAMEPLAY_ACTION;

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
static Map gameMap;                 // Declare a Map variable to hold the game map
static Unit playerUnits[MAX_UNITS]; // Declare a Unit variable to hold the player unit
static ContextMenu unitContextMenu;
static Unit *selectedUnit;
static GAMEPLAY_ACTION currentAction;
static Vector2 targetPosition;
static ContextMessage contextMessage;

void MoveUnit(void);
void AttackUnit(void);
void MergeUnits(void);
void WaitUnit(void);


//Update States

void UpdateIdleState(void);
void UpdateMoveState(void);

//Draw States
void DrawIdleState(void);
void DrawMoveState(void);

//Callbacks

void onContextMessageClose(void){
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
    InitUnit(&playerUnits[0], SOLDIER, 5, 5, 1); // Initialize a player unit at position (5, 5) with owner 1
    InitUnit(&playerUnits[1], TANK, 10, 10, 1);  // Initialize another player unit at position (10, 10) with owner 1
    InitUnit(&playerUnits[2], PLANE, 15, 15, 1);
    InitContextMenu(&unitContextMenu);
    AddButtonToContextMenu(&unitContextMenu, "Move", MoveUnit);
    AddButtonToContextMenu(&unitContextMenu, "Attack", AttackUnit);
    AddButtonToContextMenu(&unitContextMenu, "Merge", MergeUnits);
    AddButtonToContextMenu(&unitContextMenu, "Wait", WaitUnit);
    InitContextMesage(&contextMessage, (Vector2){0,0});
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
        break;
        case MERGE_STATE:
        break;
        case WAIT_STATE:
        break;
    }
   
    

    for (int i = 0; i < 2; i++)
    {
        UpdateUnit(&playerUnits[i]);
    }
    UpdateContextMessage(&contextMessage);
}


void UpdateIdleState(void)
{
    if(selectedUnit !=NULL){
        Vector2 unitPosition = {selectedUnit->boundingBox.x + selectedUnit->boundingBox.width, selectedUnit->boundingBox.y};
        UpdateContextMenu(&unitContextMenu, unitPosition, IsMouseButtonPressed(MOUSE_LEFT_BUTTON));
    }
    
    if (GetLastInputAction() == SELECT)
    {
        HideContextMenu(&unitContextMenu);
        if(selectedUnit != NULL && currentAction == IDLE){
            selectedUnit = NULL;
        }

        for (int i = 0; i < MAX_UNITS; i++)
        {
            if (CheckCollisionPointRec(GetMousePosition(), playerUnits[i].boundingBox))
            {
                selectedUnit = &playerUnits[i];
                ShowContextMenu(&unitContextMenu);
                break;
            }
        }
        
    }
    

    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }
}

void UpdateMoveState(void){

    if(selectedUnit !=NULL && selectedUnit->moved){
        currentAction = IDLE;
        selectedUnit = NULL;
    }
    targetPosition = (Vector2){(int)(GetMousePosition().x / 32) * 32 +16, (int)(GetMousePosition().y / 32) * 32 +16};
    float distance = sqrtf(powf((targetPosition.x - (selectedUnit->position.x - 1) * 32 - 16), 2) + powf((targetPosition.y - (selectedUnit->position.y - 1) * 32 - 16), 2)) / 32.0f;

    if(GetLastInputAction() == SELECT && distance <= selectedUnit->speed){
        selectedUnit->position = (Vector2){(int)(targetPosition.x / 32) +1, (int)(targetPosition.y / 32) +1};
        selectedUnit->boundingBox = (Rectangle){(selectedUnit->position.x-1) * 32, (selectedUnit->position.y-1) * 32, 32, 32};
        currentAction = IDLE;
        selectedUnit->moved = true;
        selectedUnit = NULL;
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    // TODO: Draw GAMEPLAY screen here!
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), PURPLE);
    DrawMap(&gameMap);
    for (int i = 0; i < 3; i++)
    {
        DrawUnit(&playerUnits[i]);
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
        break;
        case MERGE_STATE:
        break;
        case WAIT_STATE:
        break;
    }
    DrawContextMessage(&contextMessage);
}

void DrawIdleState(void){
    if (selectedUnit != NULL)
    {
        DrawRectangleLines(selectedUnit->boundingBox.x, selectedUnit->boundingBox.y, selectedUnit->boundingBox.width, selectedUnit->boundingBox.height, YELLOW);
        DrawContextMenu(&unitContextMenu, (Vector2){selectedUnit->boundingBox.x + selectedUnit->boundingBox.width, selectedUnit->boundingBox.y});
    }
}

void DrawMoveState(void){
    if (selectedUnit != NULL)
    {
        DrawRectangleLines(selectedUnit->boundingBox.x, selectedUnit->boundingBox.y, selectedUnit->boundingBox.width, selectedUnit->boundingBox.height, YELLOW);
    }
    
    float distance = sqrtf(powf((targetPosition.x - (selectedUnit->position.x - 1) * 32 - 16), 2) + powf((targetPosition.y - (selectedUnit->position.y - 1) * 32 - 16), 2)) / 32.0f;
    Color lineColor = (distance <= selectedUnit->speed) ? GREEN : RED;
    DrawRectangleLines(targetPosition.x - 16, targetPosition.y - 16, 32, 32, lineColor);
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

void MoveUnit(void)
{
    currentAction = MOVE_STATE;
    if(selectedUnit->moved) {
        ShowContextMessage(&contextMessage, "Unit has moved", 120, MESSAGE_ERROR, onContextMessageClose);
    }else{
        ShowContextMessage(&contextMessage, "Select Target Destination", 120, MESSAGE_INFO, onContextMessageClose);
    }
}

void AttackUnit(void)
{
    TraceLog(LOG_INFO, "Attack Unit");
}
void MergeUnits(void)
{
    TraceLog(LOG_INFO, "Merge Units");
}
void WaitUnit(void)
{
    TraceLog(LOG_INFO, "Wait Unit");
}