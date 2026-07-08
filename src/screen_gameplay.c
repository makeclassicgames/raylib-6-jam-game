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

#include "raylib.h"
#include "screens.h"
#include "map.h"
#include "unit.h"
#include "input.h"

#define MAP_WIDTH 23
#define MAP_HEIGHT 23

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
static Map gameMap; // Declare a Map variable to hold the game map
static Unit playerUnits[3]; // Declare a Unit variable to hold the player unit

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;
    InitMap(&gameMap, MAP_WIDTH, MAP_HEIGHT);
    InitUnit(&playerUnits[0], SOLDIER, 5, 5, 1); // Initialize a player unit at position (5, 5) with owner 1
    InitUnit(&playerUnits[1], TANK, 10, 10, 1); // Initialize another player unit at position (10, 10) with owner 1
    InitUnit(&playerUnits[2], PLANE, 15, 15, 1);
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    HandleInput(); // Handle input for the gameplay screen
    // TODO: Update GAMEPLAY screen variables here!

    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }

    for (int i = 0; i < 2; i++)
    {
        UpdateUnit(&playerUnits[i]);
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