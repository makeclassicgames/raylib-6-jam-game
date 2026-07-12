/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Title Screen Functions Definitions (Init, Update, Draw, Unload)
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

#define TITLE_PNG "resources/title.png"

void onStartButtonClick(void);
void onCreditsButtonClick(void);
void onSoundButtonClick(void);

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
static Button startButton;
static Button creditsButton;
static Button soundButton;
Texture2D titleScreen;

//----------------------------------------------------------------------------------
// Title Screen Functions Definition
//----------------------------------------------------------------------------------

// Title Screen Initialization logic
void InitTitleScreen(void)
{
    // TODO: Initialize TITLE screen variables here!
    framesCounter = 0;
    finishScreen = 0;
    titleScreen = LoadTexture(TITLE_PNG);
    InitButton(&startButton, "START", (Vector2){ 160, 560 }, onStartButtonClick);
    InitButton(&creditsButton, "CREDITS", (Vector2){ 420, 560 }, onCreditsButtonClick);
    InitButton(&soundButton, "SOUND: ON", (Vector2){ 580, 560 }, onSoundButtonClick);
    SetSoundVolume(themeMusic, 0.5f);
}

// Title Screen Update logic
void UpdateTitleScreen(void)
{
    // TODO: Update TITLE screen variables here!
    HandleInput(); // Handle input events
    // Press enter or tap to change to GAMEPLAY screen
    if(IsSoundPlaying(themeMusic) == false) PlaySound(themeMusic);
    UpdateButton(&startButton, GetMousePosition(), GetLastInputAction() == CONFIRM);
    UpdateButton(&creditsButton, GetMousePosition(), GetLastInputAction() == CONFIRM);
    UpdateButton(&soundButton, GetMousePosition(), GetLastInputAction() == CONFIRM);
}

// Title Screen Draw logic
void DrawTitleScreen(void)
{
    DrawTexture(titleScreen, 0, 0, WHITE);
    DrawButton(&startButton);
    DrawButton(&creditsButton);
    DrawButton(&soundButton);
}

// Title Screen Unload logic
void UnloadTitleScreen(void)
{
    UnloadTexture(titleScreen);
}

// Title Screen should finish?
int FinishTitleScreen(void)
{
    return finishScreen;
}

void onStartButtonClick(void){
    finishScreen = 1;   // GAMEPLAY
    PlaySound(fxSelect);
    StopSound(themeMusic);
}
void onCreditsButtonClick(void){
    finishScreen = 2;   // CREDITS
    PlaySound(fxSelect);
}

void onSoundButtonClick(void){
    soundOn = !soundOn;
    if (soundOn) {
        SetSoundVolume(themeMusic, 0.5f);
        SetSoundVolume(fxSelect, 0.5f);
        SetSoundVolume(fxHit, 0.5f);
        SetSoundVolume(victoryTheme, 0.5f);
        SetSoundVolume(fxMoveCPU, 0.5f);
        SetSoundVolume(fxHitCPU, 0.5f);
        PlaySound(themeMusic);
        soundButton.text = "SOUND: ON";
    } else {
        SetSoundVolume(themeMusic, 0.0f);
        SetSoundVolume(fxSelect, 0.0f);
        SetSoundVolume(fxHit, 0.0f);
        SetSoundVolume(victoryTheme, 0.0f);
        SetSoundVolume(fxMoveCPU, 0.0f);
        SetSoundVolume(fxHitCPU, 0.0f);
        StopSound(themeMusic);
        soundButton.text = "SOUND: OFF";
    }
}