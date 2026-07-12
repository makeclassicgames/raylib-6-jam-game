/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Options Screen Functions Definitions (Init, Update, Draw, Unload)
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
#include "input.h"
#include "ui.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
static Button backButton;

void onBackButtonClick(void);

//----------------------------------------------------------------------------------
// Credits Screen Functions Definition
//----------------------------------------------------------------------------------

// Credits Screen Initialization logic
void InitCreditsScreen(void)
{
    // TODO: Initialize CREDITS screen variables here!
    framesCounter = 0;
    finishScreen = 0;
    InitButton(&backButton, "BACK", (Vector2){ 300, 360 }, onBackButtonClick);
}

// Credits Screen Update logic
void UpdateCreditsScreen(void)
{
    HandleInput(); // Handle input for the credits screen
    UpdateButton(&backButton, GetMousePosition(), GetLastInputAction() == CONFIRM);
    
}

// Credits Screen Draw logic
void DrawCreditsScreen(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), PURPLE);
    DrawText("CREDITS", GetScreenWidth() / 2 - MeasureText("CREDITS", 40) / 2, 100, 40, WHITE);
    DrawText("Game developed by: Zerasul (Make Classic Games) and Emiliollbb", GetScreenWidth() / 2 - MeasureText("Game developed by: Zerasul (Make Classic Games) and Emiliollbb", 20) / 2, 200, 20, WHITE);
    DrawText("Graphics by: Emiliollbb", GetScreenWidth() / 2 - MeasureText("Graphics by: Emiliollbb", 20) / 2, 250, 20, WHITE);
    DrawText("Music by: Emiliollbb and Reset1001", GetScreenWidth() / 2 - MeasureText("Music by: Emiliollbb and Reset1001", 20) / 2, 300, 20, WHITE);
    DrawButton(&backButton);
}

// Credits Screen Unload logic
void UnloadCreditsScreen(void)
{
    // TODO: Unload CREDITS screen variables here!
}

// Credits Screen should finish?
int FinishCreditsScreen(void)
{
    return finishScreen;
}

void onBackButtonClick(void)
{
    PlaySound(fxSelect);
    finishScreen = 1;
}